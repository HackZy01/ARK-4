#include <pspsdk.h>
#include <pspsysmem_kernel.h>
#include <psputilsforkernel.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <systemctrl_private.h>
#include <globals.h> 
#include "functions.h"
#include "macros.h"
#include "exitgame.h"
#include "popspatch.h"
#include "vitamem.h"
#include "libs/graphics/graphics.h"

extern STMOD_HANDLER previous;

extern void exitLauncher();

KernelFunctions _ktbl = { // for vita flash patcher
    .KernelDcacheInvalidateRange = &sceKernelDcacheInvalidateRange,
    .KernelIcacheInvalidateAll = &sceKernelIcacheInvalidateAll,
    .KernelDcacheWritebackInvalidateAll = &sceKernelDcacheWritebackInvalidateAll,
    .KernelIOOpen = &sceIoOpen,
    .KernelIORead = &sceIoRead,
    .KernelIOWrite = &sceIoWrite,
    .KernelIOClose = &sceIoClose,
    .KernelIOMkdir = &sceIoMkdir,
    .KernelDelayThread = &sceKernelDelayThread,
};

// Return Game Product ID of currently running Game
int sctrlARKGetGameID(char gameid[GAME_ID_MINIMUM_BUFFER_SIZE])
{
    // Invalid Arguments
    if(gameid == NULL) return -1;
    
    // Elevate Permission Level
    unsigned int k1 = pspSdkSetK1(0);
    
    // Fetch Game Information Structure
    void * gameinfo = SysMemForKernel_EF29061C_Fixed();
    
    // Restore Permission Level
    pspSdkSetK1(k1);
    
    // Game Information unavailable
    if(gameinfo == NULL) return -3;
    
    // Copy Product Code
    memcpy(gameid, gameinfo + 0x44, GAME_ID_MINIMUM_BUFFER_SIZE - 1);
    
    // Terminate Product Code
    gameid[GAME_ID_MINIMUM_BUFFER_SIZE - 1] = 0;
    
    // Return Success
    return 0;
}

// This patch injects Inferno with no ISO to simulate an empty UMD drive on homebrew
int sctrlKernelLoadExecVSHWithApitypeWithUMDemu(int apitype, const char * file, struct SceKernelLoadExecVSHParam * param)
{
    // Elevate Permission Level
    unsigned int k1 = pspSdkSetK1(0);
    
    if (apitype == 0x141){ // homebrew API
        sctrlSESetBootConfFileIndex(MODE_INFERNO); // force inferno to simulate UMD drive
        sctrlSESetUmdFile("");
    }
    
    // Find Target Function
    int (* _LoadExecVSHWithApitype)(int, const char*, struct SceKernelLoadExecVSHParam*, unsigned int)
        = (void *)findFirstJAL(sctrlHENFindFunction("sceLoadExec", "LoadExecForKernel", 0xD8320A28));

    // Load Execute Module
    int result = _LoadExecVSHWithApitype(apitype, file, param, 0x10000);
    
    // Restore Permission Level on Failure
    pspSdkSetK1(k1);
    
    // Return Error Code
    return result;
}

void patchLoadExecUMDemu(){
    // highjack SystemControl
    u32 func = K_EXTRACT_IMPORT(&sctrlKernelLoadExecVSHWithApitype);
    _sw(JUMP(sctrlKernelLoadExecVSHWithApitypeWithUMDemu), func);
    _sw(NOP, func+4);
    flushCache();
}

// Return Boot Status
int isSystemBooted(void)
{

    // Find Function
    int (* _sceKernelGetSystemStatus)(void) = (void*)sctrlHENFindFunction("sceSystemMemoryManager", "SysMemForKernel", 0x452E3696);
    
    // Get System Status
    int result = _sceKernelGetSystemStatus();
        
    // System booted
    if(result == 0x20000) return 1;
    
    // Still booting
    return 0;
}

int use_mscache = 0;
int use_highmem = 0;
void settingsHandler(char* path){
    int apitype = sceKernelInitApitype();
    if (strcasecmp(path, "highmem") == 0){
        use_highmem = 1;
        unlockVitaMemory();
    }
    else if (strcasecmp(path, "mscache") == 0){
        use_mscache = 1; // enable ms cache for speedup
    }
    else if (strcasecmp(path, "infernocache") == 0){
        if (apitype == 0x123 || apitype == 0x125){
            void (*CacheSetPolicy)(int) = sctrlHENFindFunction("PRO_Inferno_Driver", "inferno_driver", 0xC0736FD6);
            int (*CacheInit)(int, int, int) = sctrlHENFindFunction("PRO_Inferno_Driver", "inferno_driver", 0x8CDE7F95);
            if (CacheSetPolicy && CacheInit){
                CacheSetPolicy(CACHE_POLICY_LRU);
                CacheInit(32 * 1024, 64, (use_highmem)?2:11); // 4M cache
            }
        }
    }
}

void ARKVitaOnModuleStart(SceModule2 * mod){

    // System fully booted Status
    static int booted = 0;
    
    patchGameInfoGetter(mod);
    
    // Patch sceKernelExitGame Syscalls
    if(strcmp(mod->modname, "sceLoadExec") == 0)
    {
        sctrlHENPatchSyscall((void*)sctrlHENFindFunction(mod->modname, "LoadExecForUser", 0x05572A5F), K_EXTRACT_IMPORT(exitLauncher));
        sctrlHENPatchSyscall((void*)sctrlHENFindFunction(mod->modname, "LoadExecForUser", 0x2AC9954B), K_EXTRACT_IMPORT(exitLauncher));
        sctrlHENPatchSyscall((void*)sctrlHENFindFunction(mod->modname, "LoadExecForUser", 0x08F7166C), K_EXTRACT_IMPORT(exitLauncher));
        goto flush;
    }
    
    // Patch Kermit Peripheral Module to load flash0
    if(strcmp(mod->modname, "sceKermitPeripheral_Driver") == 0)
    {
        patchKermitPeripheral(&_ktbl);
        goto flush;
    }
    
    // Patch Vita Popsman
    if (strcmp(mod->modname, "scePops_Manager") == 0){
        //patchVitaPopsman(mod);
        // Hook scePopsManExitVSHKernel
        sctrlHENPatchSyscall((void *)sctrlHENFindFunction("scePops_Manager", "scePopsMan", 0x0090B2C8), K_EXTRACT_IMPORT(exitLauncher));
        goto flush;
    }
    
    // Patch POPS SPU
    if (strcmp(mod->modname, "pops") == 0)
    {
        //patchVitaPopsSpu(mod);
        goto flush;
    }
    
    // load and process settings file
    if(strcmp(mod->modname, "sceMediaSync") == 0)
    {
        loadSettings(&settingsHandler);
        goto flush;
    }
    
    if (strcmp(mod->modname, "Legacy_Software_Loader") == 0){
        // Remove patch of sceKernelGetUserLevel on sceLFatFs_Driver
        _sw(0, mod->text_addr + 0x1140);
        goto flush;
    }

    // VLF Module Patches
    if(strcmp(mod->modname, "VLF_Module") == 0)
    {
        // Patch VLF Module
        patchVLF(mod);
        // Exit Handler
        goto flush;
    }
       
    // Boot Complete Action not done yet
    if(booted == 0)
    {
        // Boot is complete
        if(isSystemBooted())
        {
            // Initialize Memory Stick Speedup Cache
            if (use_mscache) msstorCacheInit("ms", 8 * 1024);
            // Apply Directory IO PSP Emulation
            patchFileSystemDirSyscall();
            // Boot Complete Action done
            booted = 1;
            goto flush;
        }
    }

flush:
    flushCache();

exit:
       // Forward to previous Handler
    if(previous) previous(mod);
}

void PROVitaSysPatch(){
    SceModule2* mod = NULL;
    // filesystem patches
    initFileSystem();
    // patch loadexec to use inferno for UMD drive emulation (needed for some homebrews to load)
    patchLoadExecUMDemu();
    // unprotect flash0 ramfs for user access
    unprotectVitaMem();
}

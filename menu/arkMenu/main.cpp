#include <pspkernel.h>
#include "gfx.h"
#include "debug.h"
#include "common.h"
#include "system_mgr.h"

PSP_MODULE_INFO("ARKMENU", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(20*1024);

using namespace std;

int main(int argc, char** argv){

    intraFontInit();
    ya2d_init();

    common::loadData(argc, argv);

    SystemMgr::initMenu();
    SystemMgr::startMenu();
    SystemMgr::endMenu();

    common::deleteData();
    
    intraFontShutdown();
    
    ya2d_shutdown();

    sceKernelExitGame();
    
    return 0;
}

	.set noreorder

#include "pspimport.s"

// Build files
// SystemCtrlForKernel_XXXX.o

#ifdef F_SystemCtrlForKernel_0000
	IMPORT_START "SystemCtrlForKernel",0x00090000
#endif
#ifdef F_SystemCtrlForKernel_0001
	IMPORT_FUNC  "SystemCtrlForKernel",0x2D10FB28,sctrlKernelLoadExecVSHWithApitype
#endif
#ifdef F_SystemCtrlForKernel_0002
	IMPORT_FUNC  "SystemCtrlForKernel",0xEB74FE45,sctrlKernelSetUserLevel
#endif
#ifdef F_SystemCtrlForKernel_0003
	IMPORT_FUNC  "SystemCtrlForKernel",0xD8FF9B99,sctrlKernelSetDevkitVersion
#endif
#ifdef F_SystemCtrlForKernel_0004
	IMPORT_FUNC  "SystemCtrlForKernel",0x1090A2E1,sctrlHENGetVersion
#endif
#ifdef F_SystemCtrlForKernel_0005
	IMPORT_FUNC  "SystemCtrlForKernel",0x5328B431,sctrlHENGetMinorVersion
#endif
#ifdef F_SystemCtrlForKernel_0006
	IMPORT_FUNC  "SystemCtrlForKernel",0x78E46415,sctrlHENFindDriver
#endif
#ifdef F_SystemCtrlForKernel_0007
	IMPORT_FUNC  "SystemCtrlForKernel",0x159AF5CC,sctrlHENFindFunction
#endif
#ifdef F_SystemCtrlForKernel_0008
	IMPORT_FUNC  "SystemCtrlForKernel",0xF988C1DC,sctrlHENPatchSyscall
#endif
#ifdef F_SystemCtrlForKernel_0009
	IMPORT_FUNC  "SystemCtrlForKernel",0x826668E9,sctrlHENPatchSyscall
#endif
#ifdef F_SystemCtrlForKernel_0010
	IMPORT_FUNC  "SystemCtrlForKernel",0x02BFCB5F,sctrlHENPatchSyscall
#endif
#ifdef F_SystemCtrlForKernel_0011
	IMPORT_FUNC  "SystemCtrlForKernel",0x1C90BECB,sctrlHENSetStartModuleHandler
#endif
#ifdef F_SystemCtrlForKernel_0012
	IMPORT_FUNC  "SystemCtrlForKernel",0xCE0A654E,sctrlHENLoadModuleOnReboot
#endif
#ifdef F_SystemCtrlForKernel_0013
	IMPORT_FUNC  "SystemCtrlForKernel",0x1E5436EE,oe_mallocinit
#endif
#ifdef F_SystemCtrlForKernel_0014
	IMPORT_FUNC  "SystemCtrlForKernel",0xF9584CAD,oe_malloc
#endif
#ifdef F_SystemCtrlForKernel_0015
	IMPORT_FUNC  "SystemCtrlForKernel",0xA65E8BC4,oe_free
#endif
#ifdef F_SystemCtrlForKernel_0016
	IMPORT_FUNC  "SystemCtrlForKernel",0xE34A0D97,oe_mallocterminate
#endif
#ifdef F_SystemCtrlForKernel_0017
	IMPORT_FUNC  "SystemCtrlForKernel",0xBA21998E,sctrlSEGetUmdFile
#endif
#ifdef F_SystemCtrlForKernel_0018
	IMPORT_FUNC  "SystemCtrlForKernel",0xAC56B90B,GetUmdFile
#endif
#ifdef F_SystemCtrlForKernel_0019
	IMPORT_FUNC  "SystemCtrlForKernel",0x5A35C948,sctrlSESetUmdFile
#endif
#ifdef F_SystemCtrlForKernel_0020
	IMPORT_FUNC  "SystemCtrlForKernel",0xB64186D0,SetUmdFile
#endif
#ifdef F_SystemCtrlForKernel_0021
	IMPORT_FUNC  "SystemCtrlForKernel",0x8D5BE1F0,sctrlKernelSetInitApitype
#endif
#ifdef F_SystemCtrlForKernel_0022
	IMPORT_FUNC  "SystemCtrlForKernel",0x128112C3,sctrlKernelSetInitFileName
#endif
#ifdef F_SystemCtrlForKernel_0023
	IMPORT_FUNC  "SystemCtrlForKernel",0x62CAC4CF,sctrlPatchModule
#endif
#ifdef F_SystemCtrlForKernel_0024
	IMPORT_FUNC  "SystemCtrlForKernel",0x2A868045,sctrlModuleTextAddr
#endif
#ifdef F_SystemCtrlForKernel_0025
	IMPORT_FUNC  "SystemCtrlForKernel",0x557F0B8C,sctrlGetInitTextAddr
#endif
#ifdef F_SystemCtrlForKernel_0026
	IMPORT_FUNC  "SystemCtrlForKernel",0x72F29A6E,sctrlGetInitTextAddr
#endif
#ifdef F_SystemCtrlForKernel_0027
	IMPORT_FUNC  "SystemCtrlForKernel",0x259B51CE,sctrlSetCustomStartModule
#endif
#ifdef F_SystemCtrlForKernel_0028
	IMPORT_FUNC  "SystemCtrlForKernel",0x603EE1D0,sctrlKernelSetNidResolver
#endif
#ifdef F_SystemCtrlForKernel_0029
	IMPORT_FUNC  "SystemCtrlForKernel",0xB364FBB4,sctrlKernelRand
#endif
#ifdef F_SystemCtrlForKernel_0030
	IMPORT_FUNC  "SystemCtrlForKernel",0x028687B4,findImportLib
#endif
#ifdef F_SystemCtrlForKernel_0031
	IMPORT_FUNC  "SystemCtrlForKernel",0x16C3137E,findImportByNID
#endif
#ifdef F_SystemCtrlForKernel_0032
	IMPORT_FUNC  "SystemCtrlForKernel",0x869F24E9,hookImportByNID
#endif
#ifdef F_SystemCtrlForKernel_0033
	IMPORT_FUNC  "SystemCtrlForKernel",0x324DF3DD,flushCache
#endif
#ifdef F_SystemCtrlForKernel_0034
	IMPORT_FUNC  "SystemCtrlForKernel",0xCC9ADCF8,sctrlHENSetSpeed
#endif
#ifdef F_SystemCtrlForKernel_0035
	IMPORT_FUNC  "SystemCtrlForKernel",0x3DF7F7D8,printk
#endif
#ifdef F_SystemCtrlForKernel_0036
	IMPORT_FUNC  "SystemCtrlForKernel",0xCB739F42,printkCached
#endif
#ifdef F_SystemCtrlForKernel_0037
	IMPORT_FUNC  "SystemCtrlForKernel",0x5C94CB48,printkSync
#endif
#ifdef F_SystemCtrlForKernel_0038
	IMPORT_FUNC  "SystemCtrlForKernel",0x68363B69,installJALTrace
#endif
#ifdef F_SystemCtrlForKernel_0039
	IMPORT_FUNC  "SystemCtrlForKernel",0xBB365B42,installMemoryJALTrace
#endif
#ifdef F_SystemCtrlForKernel_0040
	IMPORT_FUNC  "SystemCtrlForKernel",0x804572A3,installModuleJALTrace
#endif
#ifdef F_SystemCtrlForKernel_0041
	IMPORT_FUNC  "SystemCtrlForKernel",0xBC939DC1,sctrlSESetBootConfFileIndex
#endif
#ifdef F_SystemCtrlForKernel_0042
	IMPORT_FUNC  "SystemCtrlForKernel",0x70B92B45,sctrlSEGetBootConfFileIndex
#endif
#ifdef F_SystemCtrlForKernel_0043
	IMPORT_FUNC  "SystemCtrlForKernel",0x5CB025F0,sctrlSESetBootConfFileIndex
#endif
#ifdef F_SystemCtrlForKernel_0044
	IMPORT_FUNC  "SystemCtrlForKernel",0x31C6160D,sctrlSESetDiscType
#endif
#ifdef F_SystemCtrlForKernel_0045
	IMPORT_FUNC  "SystemCtrlForKernel",0xABEF849B,sctrlSEGetDiscType
#endif
#ifdef F_SystemCtrlForKernel_0046
	IMPORT_FUNC  "SystemCtrlForKernel",0x2BA770CD,sctrlKernelSetUMDEmuFile
#endif
#ifdef F_SystemCtrlForKernel_0047
	IMPORT_FUNC  "SystemCtrlForKernel",0x56CEAF00,sctrlKernelQuerySystemCall
#endif
#ifdef F_SystemCtrlForKernel_0048
	IMPORT_FUNC  "SystemCtrlForKernel",0x76C382FF,lzo1x_decompress
#endif
#ifdef F_SystemCtrlForKernel_0049
	IMPORT_FUNC  "SystemCtrlForKernel",0xAD9849FE,sctrlKernelMsIsEf
#endif
#ifdef F_SystemCtrlForKernel_0050
	IMPORT_FUNC  "SystemCtrlForKernel",0xFCE44FB8,sctrlGetInitPARAM
#endif
#ifdef F_SystemCtrlForKernel_0051
	IMPORT_FUNC  "SystemCtrlForKernel",0x39DF1C8B,sctrlHENFindImport
#endif
#ifdef F_SystemCtrlForKernel_0052
	IMPORT_FUNC  "SystemCtrlForKernel",0xB00B1E55,sctrlHENGetArkConfig
#endif
#ifdef F_SystemCtrlForKernel_0053
	IMPORT_FUNC  "SystemCtrlForKernel",0x16100529,LZ4_decompress_fast
#endif
#ifdef F_SystemCtrlForKernel_0054
	IMPORT_FUNC  "SystemCtrlForKernel",0x2794CCF4,sctrlKernelExitVSH
#endif
#ifdef F_SystemCtrlForKernel_0055
	IMPORT_FUNC  "SystemCtrlForKernel",0xAD4D5EA5,sctrlSESetConfigEx
#endif
#ifdef F_SystemCtrlForKernel_0056
	IMPORT_FUNC  "SystemCtrlForKernel",0x8E426F09,sctrlSEGetConfigEx
#endif
#ifdef F_SystemCtrlForKernel_0057
	IMPORT_FUNC  "SystemCtrlForKernel",0x1DDDAD0C,sctrlSESetConfig
#endif
#ifdef F_SystemCtrlForKernel_0058
	IMPORT_FUNC  "SystemCtrlForKernel",0x16C3B7EE,sctrlSEGetConfig
#endif
#ifdef F_SystemCtrlForKernel_0059
    IMPORT_FUNC "SystemCtrlForKernel",0x162DC9FC,sctrlExitToLauncher
#endif
#ifdef F_SystemCtrlForKernel_0060
    IMPORT_FUNC "SystemCtrlForKernel",0xD339E2E9,sctrlHENIsSE
#endif
#ifdef F_SystemCtrlForKernel_0061
    IMPORT_FUNC "SystemCtrlForKernel",0x2E2935EF,sctrlHENIsDevhook
#endif
#ifdef F_SystemCtrlForKernel_0062
    IMPORT_FUNC "SystemCtrlForKernel",0x287C1E4F,sctrlPatchExitGame
#endif
#ifdef F_SystemCtrlForKernel_0063
    IMPORT_FUNC "SystemCtrlForKernel",0x79553CCE,sctrlExitGameMenu
#endif
#ifdef F_SystemCtrlForKernel_0064
    IMPORT_FUNC "SystemCtrlForKernel",0x99C088A5,sctrlExitToRecovery
#endif

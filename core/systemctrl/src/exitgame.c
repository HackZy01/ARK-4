/*
 * This file is part of PRO CFW.

 * PRO CFW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO CFW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO CFW. If not, see <http://www.gnu.org/licenses/ .
 */

#include <pspsdk.h>
#include <pspinit.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <macros.h>
#include <string.h>
#include <globals.h>
#include <functions.h>
#include <graphics.h>

// Exit Button Mask
#define EXIT_MASK (PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_START | PSP_CTRL_DOWN)

extern ARKConfig* ark_config;

void exitLauncher()
{

    // Refuse Operation in Save dialog
	if(sceKernelFindModuleByName("sceVshSDUtility_Module") != NULL) return;
	
	// Refuse Operation in Dialog
	if(sceKernelFindModuleByName("sceDialogmain_Module") != NULL) return;

    // Load Execute Parameter
    struct SceKernelLoadExecVSHParam param;
    
    // set exit app
    char path[ARK_PATH_SIZE];
    strcpy(path, ark_config->arkpath);
    if (ark_config->recovery) strcat(path, ARK_RECOVERY);
    else if (ark_config->launcher[0]) strcat(path, ark_config->launcher);
    else strcat(path, ARK_MENU);
    
    // Clear Memory
    memset(&param, 0, sizeof(param));

    // Configure Parameters
    param.size = sizeof(param);
    param.args = strlen(path) + 1;
    param.argp = path;
    param.key = "game";

    // set default mode
    sctrlSESetUmdFile("");
    sctrlSESetBootConfFileIndex(MODE_UMD);
    
    // Trigger Reboot
    sctrlKernelLoadExecVSHWithApitype(0x141, path, &param);
}

// Gamepad Hook #1
int (*CtrlPeekBufferPositive)(SceCtrlData *, int) = NULL;
int peek_positive(SceCtrlData * pad_data, int count)
{
	// Capture Gamepad Input
	count = CtrlPeekBufferPositive(pad_data, count);
	
	// Check for Exit Mask
	if((pad_data[0].Buttons & EXIT_MASK) == EXIT_MASK)
	{
		// Exit to PRO VSH
		exitLauncher();
	}
	
	// Return Number of Input Frames
	return count;
}

// Gamepad Hook #2
int (*CtrlPeekBufferNegative)(SceCtrlData *, int) = NULL;
int peek_negative(SceCtrlData * pad_data, int count)
{
	// Capture Gamepad Input
	count = CtrlPeekBufferNegative(pad_data, count);
	
	// Check for Exit Mask
	if((pad_data[0].Buttons & EXIT_MASK) == 0)
	{
		// Exit to PRO VSH
		exitLauncher();
	}
	
	// Return Number of Input Frames
	return count;
}

// Gamepad Hook #3
int (*CtrlReadBufferPositive)(SceCtrlData *, int) = NULL;
int read_positive(SceCtrlData * pad_data, int count)
{
	// Capture Gamepad Input
	count = CtrlReadBufferPositive(pad_data, count);
	
	// Check for Exit Mask
	if((pad_data[0].Buttons & EXIT_MASK) == EXIT_MASK)
	{
		// Exit to PRO VSH
		exitLauncher();
	}
	
	// Return Number of Input Frames
	return count;
}

// Gamepad Hook #4
int (*CtrlReadBufferNegative)(SceCtrlData *, int) = NULL;
int read_negative(SceCtrlData * pad_data, int count)
{
	// Capture Gamepad Input
	count = CtrlReadBufferNegative(pad_data, count);
	
	// Check for Exit Mask
	if((pad_data[0].Buttons & EXIT_MASK) == 0)
	{
		// Exit to PRO VSH
		exitLauncher();
	}
	
	// Return Number of Input Frames
	return count;
}

// Hook Gamepad Input
void patchController(void)
{
	// Hook Gamepad Input Syscalls (user input hooking only)
    CtrlPeekBufferPositive = (void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x3A622550);
    CtrlPeekBufferNegative = (void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0xC152080A);
    CtrlReadBufferPositive = (void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x1F803938);
    CtrlReadBufferNegative = (void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x60B81F86);
	sctrlHENPatchSyscall((void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x3A622550), peek_positive);
	sctrlHENPatchSyscall((void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0xC152080A), peek_negative);
	sctrlHENPatchSyscall((void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x1F803938), read_positive);
	sctrlHENPatchSyscall((void *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x60B81F86), read_negative);
}
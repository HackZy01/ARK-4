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
#include <pspkernel.h>
#include <psputilsforkernel.h>
#include <string.h>
#include <rebootconfig.h>
#include <systemctrl.h>
#include "imports.h"
#include "rebootex.h"

extern ARKConfig* ark_config;

// Original Load Reboot Buffer Function
int (* OrigLoadReboot)(void * arg1, unsigned int arg2, void * arg3, unsigned int arg4) = NULL;

// Reboot Buffer Backup
#include "loader/rebootex/payload.h"
RebootConfigARK rebootex_config = {
    .magic = ARK_CONFIG_MAGIC,
    .reboot_buffer_size = REBOOTEX_MAX_SIZE
};

// custom rebootex
void* custom_rebootex = NULL;
void* external_rebootex = NULL;
int rebootheap = -1;

// Backup Reboot Buffer
void backupRebootBuffer(void)
{

    // Copy Reboot Buffer Configuration
    RebootConfigARK* backup_conf = (RebootConfigARK*)REBOOTEX_CONFIG;
    if (IS_ARK_CONFIG(backup_conf) && backup_conf->reboot_buffer_size){
        memcpy(&rebootex_config, backup_conf, sizeof(RebootConfigARK));
    }
    
    // Copy ARK runtime Config
    if (IS_ARK_CONFIG(ARK_CONFIG))
        memcpy(ark_config, ARK_CONFIG, sizeof(ARKConfig));
    
    // Flush Cache
    flushCache();
}

// Restore Reboot Buffer
void restoreRebootBuffer(void)
{

    u8* rebootex = custom_rebootex; // try custom rebootex from sctrlHENSetRebootexOverride
    if (rebootex == NULL) rebootex = external_rebootex; // try custom rebootex from REBOOT.BIN in ARK savedata
    if (rebootex == NULL) rebootex = rebootbuffer; // use built-in rebootex

    // Restore Reboot Buffer Payload
    if (rebootex[0] == 0x1F && rebootex[1] == 0x8B) // gzip packed rebootex
        sceKernelGzipDecompress((unsigned char *)REBOOTEX_TEXT, REBOOTEX_MAX_SIZE, rebootex, NULL);
    else // plain payload
        memcpy((void *)REBOOTEX_TEXT, rebootex, REBOOTEX_MAX_SIZE);
        
    // Restore Reboot Buffer Configuration
    memcpy((void *)REBOOTEX_CONFIG, &rebootex_config, sizeof(RebootConfigARK));

    // Restore ARK Configuration
    ark_config->recovery = 0; // reset recovery mode for next reboot
    memcpy(ARK_CONFIG, ark_config, sizeof(ARKConfig));
}

// Reboot Buffer Loader
int LoadReboot(void * arg1, unsigned int arg2, void * arg3, unsigned int arg4)
{
    // Restore Reboot Buffer
    restoreRebootBuffer();
    // Load Sony Reboot Buffer
    return OrigLoadReboot(arg1, arg2, arg3, arg4);
}

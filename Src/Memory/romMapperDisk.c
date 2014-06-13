/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperDisk.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/02/13 21:20:00 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
******************************************************************************
*/
#include "romMapperDisk.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static UInt16 patchAddress[] = { 0x4010, 0x4013, 0x4016, 0x401c, 0x401f, 0 };

typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
} RomMapperDisk;

static void destroy(RomMapperDisk* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

int romMapperDiskCreate(char* filename, UInt8* romData, 
                        int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperDisk* rm;
    int pages = size / 0x2000;
    int i;

    if (pages == 0 || (startPage + pages) > 8) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperDisk));

    rm->deviceHandle = deviceManagerRegister(ROM_DISKPATCH, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    // Patch the disk rom
    for (i = 0; patchAddress[i]; i++) {
        UInt8* ptr = rm->romData + patchAddress[i] - 0x4000;
        ptr[0] = 0xed;
        ptr[1] = 0xfe;
        ptr[2] = 0xc9;
    }

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, rm->romData + 0x2000 * i, 1, 0);
    }

    return 1;
}


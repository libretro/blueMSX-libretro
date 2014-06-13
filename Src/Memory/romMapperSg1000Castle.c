/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSg1000Castle.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/10/30 01:49:54 $
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
#include "romMapperSg1000Castle.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "sramLoader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt8* romData;
    UInt8 sram[0x2000];
    char sramFilename[512];
    int slot;
    int sslot;
    int startPage;
} RomMapperSg1000Castle;

static void destroy(RomMapperSg1000Castle* rm)
{
    sramSave(rm->sramFilename, rm->sram, 0x2000, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

int romMapperSg1000CastleCreate(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperSg1000Castle* rm;
    int pages = size / 0x2000 + ((size & 0x1fff) ? 1 : 0);
    int i;

    if (size != 0x8000 || startPage != 0) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperSg1000Castle));

    rm->deviceHandle = deviceManagerRegister(ROM_SG1000CASTLE, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(pages * 0x2000);
    memcpy(rm->romData, romData, size);
    memset(rm->sram, 0, 0x2000);

    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    
    strcpy(rm->sramFilename, sramCreateFilename(filename));
    sramLoad(rm->sramFilename, rm->sram, 0x2000, NULL, 0);

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, rm->romData + 0x2000 * i, 1, 0);
    }
    slotMapPage(rm->slot, rm->sslot, 4 + rm->startPage, rm->sram, 1, 1);

    return 1;
}


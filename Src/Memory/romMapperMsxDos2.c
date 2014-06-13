/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperMsxDos2.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/02/13 21:20:01 $
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
#include "romMapperMsxDos2.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
    int size;
    int romMapper[4];
} RomMapperMsxDos2;

static void saveState(RomMapperMsxDos2* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperMsxDos2");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateClose(state);
}

static void loadState(RomMapperMsxDos2* rm)
{
    SaveState* state = saveStateOpenForRead("mapperMsxDos2");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }

    saveStateClose(state);

    for (i = 0; i < 4; i += 2) {
        UInt8* bankData = rm->romData + (rm->romMapper[i] << 14);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     bankData, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, bankData + 0x2000, 1, 0);
    }
}

static void destroy(RomMapperMsxDos2* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperMsxDos2* rm, UInt16 address, UInt8 value) 
{
    int bank;

    address += 0x4000;

    if (address < 0x4000 && address >= 0xC000) {
        return;
    }

    bank = (address & 0x8000) >> 14;

    if (rm->romMapper[bank] != value) {
        UInt8* bankData;

        if (value > rm->size / 0x4000) {
            value %= rm->size / 0x4000;
        }

        bankData = rm->romData + ((int)value << 14);
        
        rm->romMapper[bank] = value;

        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank,     bankData, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank + 1, bankData + 0x2000, 1, 0);
    }
}

int romMapperMsxDos2Create(char* filename, UInt8* romData, 
                           int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperMsxDos2* rm;
    int i;

    if (size < 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperMsxDos2));

    rm->deviceHandle = deviceManagerRegister(ROM_MSXDOS2, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    rm->size = size;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->romMapper[0] = 0;
    rm->romMapper[2] = size / 0x2000 - 2;

    for (i = 0; i < 4; i += 2) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, rm->romData + rm->romMapper[i] * 0x2000 + 0x2000, 1, 0);
    }

    return 1;
}


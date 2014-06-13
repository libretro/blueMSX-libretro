/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperKonami4.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/03/11 09:15:57 $
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
#include "romMapperKonami4.h"
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
} RomMapperKonami4;

static void saveState(RomMapperKonami4* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperKonami4");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateClose(state);
}

static void loadState(RomMapperKonami4* rm)
{
    SaveState* state = saveStateOpenForRead("mapperKonami4");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }

    saveStateClose(state);

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }
}

static void destroy(RomMapperKonami4* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperKonami4* rm, UInt16 address, UInt8 value) 
{
    int bank;

    address += 0x4000;

    /* Page at 4000h is fixed */
    if (address < 0x6000 || address > 0xA000) {
        return;
    }

    bank = (address - 0x4000) >> 13;

    value %= rm->size / 0x2000;
    if (rm->romMapper[bank] != value) {
        UInt8* bankData = rm->romData + ((int)value << 13);

        rm->romMapper[bank] = value;
        
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, bankData, 1, 0);
    }
}

int romMapperKonami4Create(char* filename, UInt8* romData, 
                           int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperKonami4* rm;
    int romSize;
    int i;

    if (size < 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperKonami4));

    rm->deviceHandle = deviceManagerRegister(ROM_KONAMI4, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    romSize = size > 0x40000 ? size : 0x40000;
    rm->romData = malloc(romSize);
    memcpy(rm->romData, romData, size);

    if (size < 0x40000) {
        memset(rm->romData + size, 0xff, 0x40000 - size);
    }

    rm->size = romSize;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 1;
    rm->romMapper[2] = 2;
    rm->romMapper[3] = 3;

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }

    return 1;
}


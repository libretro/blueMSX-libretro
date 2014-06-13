/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperASCII8.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2005/06/20 00:31:20 $
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
#include "romMapperASCII8.h"
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
    UInt32 romMask;
    int romMapper[4];
} RomMapperASCII8;

static void saveState(RomMapperASCII8* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperASCII8");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateClose(state);
}

static void loadState(RomMapperASCII8* rm)
{
    SaveState* state = saveStateOpenForRead("mapperASCII8");
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

static void destroy(RomMapperASCII8* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperASCII8* rm, UInt16 address, UInt8 value) 
{
    int bank;

    address += 0x4000;
    
    if (address < 0x6000 || address > 0x7fff) {
        return;
    }

    bank = (address & 0x1800) >> 11;

    value &= rm->romMask;
    if (rm->romMapper[bank] != value) {
        UInt8* bankData = rm->romData + ((int)value << 13);

        rm->romMapper[bank] = value;
        
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, bankData, 1, 0);
    }
}

int romMapperASCII8Create(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperASCII8* rm;
    int i;

    int origSize = size;
    
    size = 0x8000;
    while (size < origSize) {
        size *= 2;
    }

    rm = malloc(sizeof(RomMapperASCII8));

    rm->deviceHandle = deviceManagerRegister(ROM_ASCII8, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    rm->romData = calloc(1, size);
    memcpy(rm->romData, romData, origSize);
    rm->romMask = size / 0x2000 - 1;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 0;
    rm->romMapper[2] = 0;
    rm->romMapper[3] = 0;

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }

    return 1;
}


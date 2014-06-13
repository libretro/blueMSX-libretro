/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperASCII8sram.c,v $
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
#include "romMapperASCII8sram.h"
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
    int sramEnabled;
    UInt32 romMask;
    int romMapper[4];
} RomMapperASCII8sram;

static void saveState(RomMapperASCII8sram* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperASCII8sram");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }
    
    saveStateSet(state, "sramEnabled", rm->sramEnabled);

    saveStateClose(state);
}

static void loadState(RomMapperASCII8sram* rm)
{
    SaveState* state = saveStateOpenForRead("mapperASCII8sram");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }
    
    rm->sramEnabled = saveStateGet(state, "sramEnabled", 0);

    saveStateClose(state);

    for (i = 0; i < 4; i++) {   
        if (rm->sramEnabled & (1 << i)) {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->sram, 1, 0);
        }
        else {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
        }
    }
}

static void destroy(RomMapperASCII8sram* rm)
{
    sramSave(rm->sramFilename, rm->sram, 0x2000, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperASCII8sram* rm, UInt16 address, UInt8 value) 
{
    address += 0x4000;

    if (address >= 0x6000 && address < 0x8000) {
        int bank = (address & 0x1800) >> 11;
        int    writeCache = 0;
        UInt8* bankData;

        if (value & ~rm->romMask) {
            bankData = rm->sram;
            writeCache = bank > 1;
            rm->sramEnabled |= (1 << bank);
        }
        else {
            bankData = rm->romData + ((int)value << 13);
            rm->sramEnabled &= ~(1 << bank);
        }

        rm->romMapper[bank] = value;
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, bankData, 1, writeCache);
    }
}

int romMapperASCII8sramCreate(char* filename, UInt8* romData, 
                              int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperASCII8sram* rm;
    int i;

    int origSize = size;
    
    size = 0x8000;
    while (size < origSize) {
        size *= 2;
    }

    rm = malloc(sizeof(RomMapperASCII8sram));

    rm->deviceHandle = deviceManagerRegister(ROM_ASCII8SRAM, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    rm->romData = calloc(1, size);
    memcpy(rm->romData, romData, origSize);
    memset(rm->sram, 0, 0x2000);
    rm->romMask = size / 0x2000 - 1;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    rm->sramEnabled = 0;
    strcpy(rm->sramFilename, sramCreateFilename(filename));

    sramLoad(rm->sramFilename, rm->sram, 0x2000, NULL, 0);

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 0;
    rm->romMapper[2] = 0;
    rm->romMapper[3] = 0;

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }

    return 1;
}


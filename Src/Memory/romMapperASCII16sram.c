/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperASCII16sram.c,v $
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
#include "romMapperASCII16sram.h"
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
} RomMapperASCII16sram;

static void saveState(RomMapperASCII16sram* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperASCII16sram");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }
    
    saveStateSet(state, "sramEnabled", rm->sramEnabled);

    saveStateClose(state);
}

static void loadState(RomMapperASCII16sram* rm)
{
    SaveState* state = saveStateOpenForRead("mapperASCII16sram");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }
    
    rm->sramEnabled = saveStateGet(state, "sramEnabled", 0);

    saveStateClose(state);

    for (i = 0; i < 4; i += 2) {   
        if (rm->sramEnabled & (1 << ((i >> 1) + 1))) {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     rm->sram, 1, 0);
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, rm->sram, 1, 0);
        }
        else {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     rm->romData + rm->romMapper[i] * 0x4000, 1, 0);
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, rm->romData + rm->romMapper[i] * 0x4000 + 0x2000, 1, 0);
        }
    }
}

static void destroy(RomMapperASCII16sram* rm)
{
    sramSave(rm->sramFilename, rm->sram, 0x800, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperASCII16sram* rm, UInt16 address, UInt8 value) 
{
    address += 0x4000;

    if (address >= 0x6000 && address < 0x7800 && !(address & 0x0800)) {
        int bank = (address & 0x1000) >> 11;
        UInt8* bankData1;
        UInt8* bankData2;

        if (value & ~rm->romMask) {
            bankData1 = rm->sram;
            bankData2 = rm->sram;
            rm->sramEnabled |= (1 << ((bank >> 1) + 1));
        }
        else {
            bankData1 = rm->romData + ((int)value << 14);
            bankData2 = rm->romData + ((int)value << 14) + 0x2000;
            rm->sramEnabled &= ~(1 << ((bank >> 1) + 1));
        }

        rm->romMapper[bank] = value;

        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank,     bankData1, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank + 1, bankData2, 1, 0);
    }
    else if ((1 << (address >> 14)) & rm->sramEnabled & 0x4) {
        int i;
        address &= 0x7ff;
        for (i = 0; i < 4; i++) {
            rm->sram[address] = value;
            address += 0x800;
        }
    }
}

int romMapperASCII16sramCreate(char* filename, UInt8* romData, 
                               int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperASCII16sram* rm;
    int offset;
    int i;

    int origSize = size;
    
    size = 0x8000;
    while (size < origSize) {
        size *= 2;
    }

    rm = malloc(sizeof(RomMapperASCII16sram));

    rm->deviceHandle = deviceManagerRegister(ROM_ASCII16SRAM, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    rm->romData = calloc(1, size);
    memcpy(rm->romData, romData, origSize);
    memset(rm->sram, 0, 0x2000);
    rm->romMask = size / 0x4000 - 1;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    rm->sramEnabled = 0;
    strcpy(rm->sramFilename, sramCreateFilename(filename));

    sramLoad(rm->sramFilename, rm->sram, 0x800, NULL, 0);

    // Mirror SRAM
    for (offset = 0x800; offset < 0x2000; offset += 0x800) {
        memcpy(rm->sram + offset, rm->sram, 0x800);
    }

    rm->romMapper[0] = 0;
    rm->romMapper[2] = 0;

    for (i = 0; i < 4; i += 2) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, rm->romData + rm->romMapper[i] * 0x2000 + 0x2000, 1, 0);
    }

    return 1;
}


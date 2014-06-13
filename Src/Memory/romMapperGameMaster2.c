/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperGameMaster2.c,v $
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
#include "romMapperGameMaster2.h"
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
    UInt8 sram[0x4000];
    char sramFilename[512];
    int slot;
    int sslot;
    int startPage;
    int sramEnabled;
    int size;
    int sramBank;
    int romMapper[4];
} RomMapperGameMaster2;

static void saveState(RomMapperGameMaster2* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperGameMaster2");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }
    
    saveStateSet(state, "sramEnabled", rm->sramEnabled);

    saveStateClose(state);
}

static void loadState(RomMapperGameMaster2* rm)
{
    SaveState* state = saveStateOpenForRead("mapperGameMaster2");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }
    
    rm->sramEnabled = saveStateGet(state, "sramEnabled", 0);

    saveStateClose(state);

    for (i = 0; i < 4; i++) {   
        if (rm->sramEnabled & (1 << (i + 2))) {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->sram, 1, 0);
        }
        else {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
        }
    }
}

static void destroy(RomMapperGameMaster2* rm)
{
    memcpy(rm->sram + 0x1000, rm->sram + 0x2000, 0x1000);

    sramSave(rm->sramFilename, rm->sram, 0x2000, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperGameMaster2* rm, UInt16 address, UInt8 value) 
{
    address += 0x4000;

    if (address >= 0x6000 && address <= 0xa000 && !(address & 0x1fff)) {
		int bank = (address - 0x4000) >> 13;

        if ((address >> 12) == 0x0a) {
			rm->sramEnabled = value & 0x10;
		}

        if (value & 0x10) {
            // Select SRAM startPage
            if (value & 0x20) {
                slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->sram + 0x2000, 1, 0);
                rm->sramBank = 1;
            }
            else {
                slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->sram, 1, 0);
                rm->sramBank = 0;
            }
        }
        else {
            value &= 0x0f;

            rm->romMapper[bank] = value;
            
            slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->romData + ((int)value << 13), 1, 0);
        }
    }

    else if (address >= 0xb000 && address < 0xc000 && rm->sramEnabled) {
        int bankOffset = (address & 0x0fff) + rm->sramBank * 0x2000;
        // Write to SRAM
        rm->sram[bankOffset] = value;
        rm->sram[bankOffset + 0x1000] = value;
    }
}

int romMapperGameMaster2Create(char* filename, UInt8* romData, 
                               int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperGameMaster2* rm;
    int i;

    if (size < 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperGameMaster2));

    rm->deviceHandle = deviceManagerRegister(ROM_GAMEMASTER2, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    memset(rm->sram, 0xff, 0x4000);
    rm->size = size;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    rm->sramBank = 0;
    rm->sramEnabled = 0;
    strcpy(rm->sramFilename, sramCreateFilename(filename));

    sramLoad(rm->sramFilename, rm->sram, 0x2000, NULL, 0);

    /* Mirror GameMaster2 SRAM as needed */
    memcpy(rm->sram + 0x2000, rm->sram + 0x1000, 0x1000);
    memcpy(rm->sram + 0x3000, rm->sram + 0x1000, 0x1000);
    memcpy(rm->sram + 0x1000, rm->sram, 0x1000);

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 0;
    rm->romMapper[2] = 0;
    rm->romMapper[3] = 0;

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }

    return 1;
}


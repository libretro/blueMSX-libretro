/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperPanasonic.c,v $
**
** $Revision: 1.8 $
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
#include "romMapperPanasonic.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "Board.h"
#include "SaveState.h"
#include "sramLoader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum {
    READ_SRAM,
    READ_RAM,
    READ_ROM
};

typedef struct {
    int deviceHandle;
    UInt8* romData;
    UInt8* sram;
    int    readSection;
    int    readOffset;
    UInt8* readBlock;
    int    sramSize;
    char   sramFilename[512];
    int    maxSRAMBank;
    int    romSize;
    UInt8  control;
    int    romMapper[8];
    int    slot;
    int    sslot;
    int    startPage;
} RomMapperPanasonic;

static int SRAM_BASE = 0x80;
static int RAM_BASE  = 0x180;

static void changeBank(RomMapperPanasonic* rm, int region, int bank);

static void saveState(RomMapperPanasonic* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperPanasonic");
    char tag[16];
    int i;

    for (i = 0; i < 8; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }
    
    saveStateGet(state, "readSection", rm->readSection);
    saveStateGet(state, "readOffset", rm->readOffset);
    saveStateSet(state, "control", rm->control);

    saveStateClose(state);
}

static void loadState(RomMapperPanasonic* rm)
{
    SaveState* state = saveStateOpenForRead("mapperPanasonic");
    int romMapper[8];
    char tag[16];
    int i;

    for (i = 0; i < 8; i++) {
        sprintf(tag, "romMapper%d", i);
        romMapper[i] = (UInt8)saveStateGet(state, tag, 0);
    }
    
    rm->readSection = saveStateGet(state, "readSection", 0);
    rm->readOffset  = saveStateGet(state, "readOffset", 0);
    rm->control     = (UInt8)saveStateGet(state, "control", 0);

    saveStateClose(state);

    for (i = 0; i < 8; i++) {
        changeBank(rm, i, romMapper[i]);
    }

    switch (rm->readSection) {
    case READ_SRAM:
        rm->readBlock = rm->sram + rm->readOffset;
        break;
    case READ_RAM:
        rm->readBlock = boardGetRamPage(rm->readOffset);
        break;
    case READ_ROM:
        rm->readBlock = rm->romData + rm->readOffset;
        break;
    }
}

static void destroy(RomMapperPanasonic* rm)
{
    sramSave(rm->sramFilename, rm->sram, rm->sramSize, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->sram);
    free(rm->romData);
    free(rm);
}

static void changeBank(RomMapperPanasonic* rm, int region, int bank)
{
	if (bank == rm->romMapper[region]) {
		return;
	}
	rm->romMapper[region] = bank;
	if (rm->sramSize > 0 && bank >= SRAM_BASE && bank < rm->maxSRAMBank) {
		int offset = (bank - SRAM_BASE) * 0x2000 & (rm->sramSize - 1);
        if (region == 3) {
            rm->readSection = READ_SRAM;
            rm->readOffset = offset;
            rm->readBlock = rm->sram + offset;
        }
        slotMapPage(rm->slot, rm->sslot, region, rm->sram + offset, region != 3, 0);
	} 
    else if (bank >= RAM_BASE) {
        if (region == 3) {
            rm->readSection = READ_RAM;
            rm->readOffset = bank - RAM_BASE;
            rm->readBlock = boardGetRamPage(bank - RAM_BASE);
        }
        slotMapPage(rm->slot, rm->sslot, region, boardGetRamPage(bank - RAM_BASE), region != 3, 0);
	} 
    else {
		int offset = bank * 0x2000 & (rm->romSize - 1);
        if (region == 3) {
            rm->readSection = READ_ROM;
            rm->readOffset = offset;
            rm->readBlock = rm->romData + offset;
        }
        slotMapPage(rm->slot, rm->sslot, region, rm->romData + offset, region != 3, 0);
	}
}

static UInt8 read(RomMapperPanasonic* rm, UInt16 address) 
{
    int bank;

	if ((rm->control & 0x04) && address >= 0x7ff0 && address < 0x7ff8) {
		return rm->romMapper[address & 7] & 0xff;
	} 
    
    if ((rm->control & 0x10) && address == 0x7ff8) {
        int i;
		UInt8 result = 0;
		for (i = 7; i >= 0; i--) {
			result <<= 1;
			if (rm->romMapper[i] & 0x100) {
				result++;
			}
		}
        return result;
	} 
    
    if ((rm->control & 0x08) && address == 0x7ff9) {
		return rm->control;
	} 

    return rm->readBlock[address & 0x1fff];

	bank = rm->romMapper[address >> 13];
    if (bank < SRAM_BASE) {
        return rm->romData[(bank * 0x2000 & (rm->romSize - 1))  + (address & 0x1fff)];
    }
    if (rm->sramSize > 0 && bank >= SRAM_BASE && bank < rm->maxSRAMBank) {
		int offset = (bank - SRAM_BASE) * 0x2000 & (rm->sramSize - 1);
        return rm->sram[offset + (address & 0x1fff)];
	} 
    if (bank >= RAM_BASE) {
        UInt8* ram = boardGetRamPage(bank - RAM_BASE);
        return ram[address & 0x1fff];
	}

    return 0xff;
}

static void write(RomMapperPanasonic* rm, UInt16 address, UInt8 value) 
{
    int region;
    int bank;
    int newBank;

    if (address >= 0x6000 && address < 0x7ff0) {
		region = (address & 0x1c00) >> 10;
        if (region == 5 || region == 6) {
            region ^= 3;
        }

		bank = rm->romMapper[region];
		newBank = (bank & ~0xff) | value;
		changeBank(rm, region, newBank);
        return;
	} 
    
    if (address == 0x7ff8) {
		for (region = 0; region < 8; region++) {
			if (value & 1) {
				changeBank(rm, region, rm->romMapper[region] |  0x100);
			} else {
				changeBank(rm, region, rm->romMapper[region] & ~0x100);
			}
			value >>= 1;
		}
        return;
	} 
    
    if (address == 0x7ff9) {
		rm->control = value;
        return;
	} 

    if (address >= 0x8000 && address < 0xC000) {
		region = address >> 13;
		bank = rm->romMapper[region];
		
        if (rm->sramSize > 0 && bank >= SRAM_BASE && bank < rm->maxSRAMBank) {
		    int offset = (bank - SRAM_BASE) * 0x2000 & (rm->sramSize - 1);
            rm->sram[offset + (address & 0x1fff)] = value;
		} 
        else if (bank >= RAM_BASE) {
            UInt8* ram = boardGetRamPage(bank - RAM_BASE);
            ram[address & 0x1fff] = value;
		}
	} 
}

static void reset(RomMapperPanasonic* rm)
{
    int i;

    rm->control = 0;
    
    for (i = 0; i < 8; i++) {
        rm->romMapper[i] = 0;
        slotMapPage(rm->slot, rm->sslot, i, rm->romData, i != 3, 0);
    }
}

int romMapperPanasonicCreate(char* filename, UInt8* romData, 
                             int size, int slot, int sslot, int startPage,
                             int sramSize) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperPanasonic* rm;
    char suffix[16];

    if (size < 0x8000 || startPage != 0) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperPanasonic));

    rm->deviceHandle = deviceManagerRegister(sramSize == 0x4000 ? ROM_PANASONIC16 : ROM_PANASONIC32, &callbacks, rm);
    slotRegister(slot, sslot, 0, 8, read, read, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    rm->romSize = size;
    rm->sramSize = sramSize;
    rm->sram = malloc(sramSize);
    memset(rm->sram, 0xff, sramSize);
    rm->maxSRAMBank = SRAM_BASE + sramSize / 0x2000;
    memset(rm->romMapper, 0, sizeof(rm->romMapper));
    rm->readBlock = rm->romData;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    sprintf(suffix, "_%d", sramSize / 1024);
    strcpy(rm->sramFilename, sramCreateFilenameWithSuffix(filename, suffix));

    sramLoad(rm->sramFilename, rm->sram, rm->sramSize, NULL, 0);

    reset(rm);

    return 1;
}

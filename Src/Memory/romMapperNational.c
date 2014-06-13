/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperNational.c,v $
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
#include "romMapperNational.h"
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
    UInt8 sram[0x1000];
    char sramFilename[512];
    int slot;
    int sslot;
    int size;
    int control;
    int sramAddr;
    int romMapper[8];
} RomMapperNational;

static void saveState(RomMapperNational* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperNational");
    char tag[16];
    int i;

    for (i = 0; i < 8; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }
    
    saveStateSet(state, "control",  rm->control);
    saveStateSet(state, "sramAddr", rm->sramAddr);

    saveStateClose(state);
}

static void loadState(RomMapperNational* rm)
{
    SaveState* state = saveStateOpenForRead("mapperNational");
    char tag[16];
    int i;

    for (i = 0; i < 8; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }
    
    rm->control  = saveStateGet(state, "control",  0);
    rm->sramAddr = saveStateGet(state, "sramAddr", 0);

    saveStateClose(state);

    for (i = 0; i < 8; i += 2) {
        slotMapPage(rm->slot, rm->sslot, i, rm->romData + rm->romMapper[i] * 0x4000, 1, 0);
    }
}

static void destroy(RomMapperNational* rm)
{
    sramSave(rm->sramFilename, rm->sram, 0x800, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, 0);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static UInt8 read(RomMapperNational* rm, UInt16 address) 
{
	if ((rm->control & 0x04) && (address & 0x7ff9) == 0x7ff0) {
		return rm->romMapper[address & 0x06];
	}
	
    if ((rm->control & 0x02) && ((address & 0x3fff) == 0x3ffd)) {
		return rm->sram[rm->sramAddr++ & 0x0fff];
	}
    
    return rm->romData[(rm->romMapper[(address >> 13) & ~1] << 14) + (address & 0x3fff)];
}

static UInt8 peek(RomMapperNational* rm, UInt16 address) 
{
	if ((rm->control & 0x04) && (address & 0x7ff9) == 0x7ff0) {
		return rm->romMapper[address & 0x06];
	}
	
    if ((rm->control & 0x02) && ((address & 0x3fff) == 0x3ffd)) {
		return rm->sram[rm->sramAddr & 0x0fff];
	}
    
    return rm->romData[(rm->romMapper[(address >> 13) & ~1] << 14) + (address & 0x3fff)];
}

static void write(RomMapperNational* rm, UInt16 address, UInt8 value) 
{
	if (address == 0x6000) {
        rm->romMapper[2] = value;
        slotMapPage(rm->slot, rm->sslot, 2, rm->romData + ((int)value << 14), 1, 0);
	} 
    else if (address == 0x6400) {
        rm->romMapper[0] = value;
        slotMapPage(rm->slot, rm->sslot, 0, rm->romData + ((int)value << 14), 1, 0);
	} 
    else if (address == 0x7000) {
        rm->romMapper[4] = value;
        slotMapPage(rm->slot, rm->sslot, 4, rm->romData + ((int)value << 14), 1, 0);
	} 
    else if (address == 0x7400) {
        rm->romMapper[6] = value;
        slotMapPage(rm->slot, rm->sslot, 6, rm->romData + ((int)value << 14), 1, 0);
	} 
    else if (address == 0x7ff9) {
		rm->control = value;
	} 
    else if (rm->control & 0x02) {
		address &= 0x3fff;
		if (address == 0x3ffa) {
			rm->sramAddr = (rm->sramAddr & 0x00fff) | value << 16;
		} 
        else if (address == 0x3ffb) {
			rm->sramAddr = (rm->sramAddr & 0xff00ff) | value << 8;
		} 
        else if (address == 0x3ffc) {
			rm->sramAddr = (rm->sramAddr & 0xffff00) | value;
		} 
        else if (address == 0x3ffd) {
            rm->sram[rm->sramAddr++ & 0x0fff] = value;
		}
	}
}

int romMapperNationalCreate(char* filename, UInt8* romData, 
                               int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperNational* rm;
    int i;

    if (size < 0x8000 || startPage != 0) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperNational));

    rm->deviceHandle = deviceManagerRegister(ROM_NATIONAL, &callbacks, rm);
    slotRegister(slot, sslot, 0, 8, read, peek, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    memset(rm->sram, 0xff, 0x1000);
    rm->size = size;
    rm->sramAddr = 0;
    rm->control = 0;
    rm->slot  = slot;
    rm->sslot = sslot;
    strcpy(rm->sramFilename, sramCreateFilename(filename));

    sramLoad(rm->sramFilename, rm->sram, 0x1000, NULL, 0);

    rm->romMapper[0] = 0;
    rm->romMapper[2] = 0;
    rm->romMapper[4] = 0;
    rm->romMapper[6] = 0;

    for (i = 0; i < 8; i += 2) {   
        slotMapPage(slot, sslot, i,     rm->romData + rm->romMapper[i] * 0x2000, 1, 0 );
        slotMapPage(slot, sslot, i + 1, rm->romData + rm->romMapper[i] * 0x2000 + 0x2000, 0, 0);
    }

    return 1;
}


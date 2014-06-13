/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperTC8566AF.c,v $
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
#include "romMapperTC8566AF.h"
#include "TC8566AF.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    TC8566AF* fdc;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
    UInt32 romMask;
    int romMapper[4];
} RomMapperTC8566AF;

static void saveState(RomMapperTC8566AF* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperTC8566AF");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateClose(state);
    
    tc8566afSaveState(rm->fdc);
}

static void loadState(RomMapperTC8566AF* rm)
{
    SaveState* state = saveStateOpenForRead("mapperTC8566AF");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }

    saveStateClose(state);

    for (i = 0; i < 4; i += 2) {
        UInt8* bankData = rm->romData + (rm->romMapper[i] << 14);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     bankData, 0, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, bankData + 0x2000, 0, 0);
    }

    tc8566afLoadState(rm->fdc);
}

static void destroy(RomMapperTC8566AF* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    tc8566afDestroy(rm->fdc);

    free(rm->romData);
    free(rm);
}

static void reset(RomMapperTC8566AF* rm)
{
    int i;

    tc8566afReset(rm->fdc);

    rm->romMapper[0] = 0;
    rm->romMapper[2] = 0;

    for (i = 0; i < 4; i += 2) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     rm->romData + rm->romMapper[i] * 0x2000, 0, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, rm->romData + rm->romMapper[i] * 0x2000 + 0x2000, 0, 0);
    }
}

static UInt8 read(RomMapperTC8566AF* rm, UInt16 address) 
{
    address += 0x4000;

	if ((address & 0x3fff) >= 0x3ff0) {
		switch (address & 0x3FFF) {
		case 0x3ff1:
            return 0x03 | (tc8566afDiskChanged(rm->fdc, 0) ? 0x00 : 0x10) | (tc8566afDiskChanged(rm->fdc, 1) ? 0x00 : 0x20);
		case 0x3ff4:
		case 0x3ffa:
            return tc8566afReadRegister(rm->fdc, 4);
		case 0x3ff5:
		case 0x3ffb:
            return tc8566afReadRegister(rm->fdc, 5);
		default:
			return 0xff;
		}
	} 
   
    if (address >= 0x4000 && address < 0x8000) {
		return rm->romData[0x4000 * rm->romMapper[0] + (address & 0x3fff)];
	} 

    return 0xff;
}

static UInt8 peek(RomMapperTC8566AF* rm, UInt16 address) 
{
    address += 0x4000;

	if ((address & 0x3fff) >= 0x3ff0) {
		switch (address & 0x3FFF) {
		case 0x3ff1:
            return 0xff; // Get from fdc
		case 0x3ff4:
		case 0x3ffa:
            return 0xff; // Get from fdc
		case 0x3ff5:
		case 0x3ffb:
            return 0xff; // Get from fdc
		default:
			return 0xff;
		}
	} 
   
    if (address >= 0x4000 && address < 0x8000) {
		return rm->romData[0x4000 * rm->romMapper[0] + (address & 0x3fff)];
	} 

    return 0xff;
}

static void write(RomMapperTC8566AF* rm, UInt16 address, UInt8 value) 
{
    address += 0x4000;
	
    if ((address == 0x6000) || (address == 0x7ff0) || (address == 0x7ffe)) {
		rm->romMapper[0] =  value & rm->romMask;
		return;
	} 
    else {
		switch (address & 0x3fff) {
		case 0x3ff2:
		case 0x3ff8:
            tc8566afWriteRegister(rm->fdc, 2, value);
			break;
		case 0x3ff3:
		case 0x3ff9:
            tc8566afWriteRegister(rm->fdc, 3, value);
			break;
		case 0x3ff4:
		case 0x3ffa:
            tc8566afWriteRegister(rm->fdc, 4, value);
			break;
		case 0x3ff5:
		case 0x3ffb:
            tc8566afWriteRegister(rm->fdc, 5, value);
			break;
		}
    }
}

int romMapperTC8566AFCreate(char* filename, UInt8* romData, 
                           int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperTC8566AF* rm;

    rm = malloc(sizeof(RomMapperTC8566AF));

    rm->deviceHandle = deviceManagerRegister(ROM_TC8566AF, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, read, peek, write, destroy, rm);

    size = (size + 0x3fff) & ~0x3fff;

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    rm->romMask = size / 0x4000 - 1;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->fdc = tc8566afCreate();

    reset(rm);

    return 1;
}


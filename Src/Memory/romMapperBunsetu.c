/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperBunsetu.c,v $
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
#include "romMapperBunsetu.h"
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
    UInt8* jisyoData;
    UInt32 address;
    int slot;
    int sslot;
    int startPage;
} RomMapperBunsetu;

static void saveState(RomMapperBunsetu* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperBunsetu");

    saveStateSet(state, "address", rm->address);
    
    saveStateClose(state);
}

static void loadState(RomMapperBunsetu* rm)
{
    SaveState* state = saveStateOpenForRead("mapperBunsetu");

    rm->address = saveStateGet(state, "address", 0);

    saveStateClose(state);
}

static void destroy(RomMapperBunsetu* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    if (rm->jisyoData) {
        free(rm->jisyoData);
    }
    free(rm);
}

static UInt8 read(RomMapperBunsetu* rm, UInt16 address) 
{
    address += 0x4000;
    
    if (address == 0xbfff) {
        if (rm->jisyoData) {
            UInt8 value = rm->jisyoData[rm->address];
            rm->address = (rm->address + 1) & 0x0ffff;
            return value;
        }
    }
    
    return rm->romData[address - 0x4000];
}

static UInt8 peek(RomMapperBunsetu* rm, UInt16 address) 
{
    address += 0x4000;
    
    if (address == 0xbfff) {
        if (rm->jisyoData) {
            return rm->jisyoData[rm->address];
        }
    }
    
    return rm->romData[address - 0x4000];
}

static void write(RomMapperBunsetu* rm, UInt16 address, UInt8 value) 
{
    address += 0x4000;
    
	switch (address) {
	case 0xbffc:
		rm->address = (rm->address & 0x1ff00) | value;
		break;
	case 0xbffd:
		rm->address = (rm->address & 0x100ff) | (value << 8);
		break;
	case 0xbffe:
		rm->address = (rm->address & 0x0ffff) | ((value & 1) << 16);
		break;
	}
}

int romMapperBunsetuCreate(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage,
                          void* jisyoRom, int jisyoSize) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperBunsetu* rm;

    if (size != 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperBunsetu));

    rm->deviceHandle = deviceManagerRegister(ROM_BUNSETU, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, read, peek, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    if (jisyoRom) {
        rm->jisyoData = malloc(jisyoSize);
        memcpy(rm->jisyoData, jisyoRom, jisyoSize);
    }
    else {
        rm->jisyoData = NULL;
    }

    rm->address   = 0;
    rm->slot      = slot;
    rm->sslot     = sslot;
    rm->startPage = startPage;

    slotMapPage(rm->slot, rm->sslot, rm->startPage + 0, rm->romData + 0x0000, 1, 0);
    slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, rm->romData + 0x2000, 1, 0);
    slotMapPage(rm->slot, rm->sslot, rm->startPage + 2, rm->romData + 0x4000, 1, 0);
    slotMapPage(rm->slot, rm->sslot, rm->startPage + 3, rm->romData + 0x6000, 0, 0);

    return 1;
}


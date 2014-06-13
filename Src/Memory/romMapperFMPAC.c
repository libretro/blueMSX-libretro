/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperFMPAC.c,v $
**
** $Revision: 1.15 $
**
** $Date: 2006/06/29 15:58:56 $
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
#include "romMapperFMPAC.h"
#include "MediaDb.h"
#include "IoPort.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "YM2413.h"
#include "Board.h"
#include "SaveState.h"
#include "sramLoader.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>


static char pacHeader[] = "PAC2 BACKUP DATA";

typedef struct {
    int deviceHandle;
    int debugHandle;
    YM_2413* ym2413;
    UInt8 romData[0x10000];
    UInt8 sram[0x2000];
    char sramFilename[512];
    int bankSelect;
    int slot;
    int sslot;
    int startPage;
    int sramEnabled;
    int enable;
    UInt8 reg1ffe;
    UInt8 reg1fff;
} RomMapperFMPAC;

static void saveState(RomMapperFMPAC* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperFMPAC");

    saveStateSet(state, "bankSelect",  rm->bankSelect);
    saveStateSet(state, "enable",      rm->enable);
    saveStateSet(state, "sramEnabled", rm->sramEnabled);
    saveStateSet(state, "reg1ffe",     rm->reg1ffe);
    saveStateSet(state, "reg1fff",     rm->reg1fff);
    
    saveStateSetBuffer(state, "sram", rm->sram, sizeof(rm->sram));

    saveStateClose(state);
    
    if (rm->ym2413 != NULL) {
        ym2413SaveState(rm->ym2413);
    }
}

static void loadState(RomMapperFMPAC* rm)
{
    SaveState* state = saveStateOpenForRead("mapperFMPAC");
    
    rm->bankSelect  =        saveStateGet(state, "bankSelect", 0);
    rm->enable      =        saveStateGet(state, "enable", 0);
    rm->sramEnabled =        saveStateGet(state, "sramEnabled", 0);
    rm->reg1ffe     = (UInt8)saveStateGet(state, "reg1ffe", 0);
    rm->reg1fff     = (UInt8)saveStateGet(state, "reg1fff", 0);
    
    saveStateGetBuffer(state, "sram", rm->sram, sizeof(rm->sram));

    saveStateClose(state);

    if (rm->ym2413 != NULL) {
        ym2413LoadState(rm->ym2413);
    }
}

static void destroy(RomMapperFMPAC* rm)
{
    sramSave(rm->sramFilename, rm->sram, 0x1ffe, pacHeader, strlen(pacHeader));

    ioPortUnregister(0x7c);
    ioPortUnregister(0x7d);
    
    if (rm->ym2413 != NULL) {
        ym2413Destroy(rm->ym2413);
    }

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    free(rm);
}

static void reset(RomMapperFMPAC* rm) 
{
    if (rm->ym2413 != NULL) {
        ym2413Reset(rm->ym2413);
    }

    rm->reg1ffe    = 0xff;
    rm->reg1fff    = 0xff;
    rm->enable     = 0;
    rm->bankSelect = 0;
}

static UInt8 read(RomMapperFMPAC* rm, UInt16 address) 
{
	address &= 0x3fff;

    if (address == 0x3ff6) {
        return rm->enable;
    }

    if (address == 0x3ff7) {
        return rm->bankSelect;
    }

    if (!rm->sramEnabled) {
		return rm->romData[(rm->bankSelect << 14) + address];
    }

	if (address < 0x1ffe) {
		return rm->sram[address];
	} 

    if (address == 0x1ffe) {
		return rm->reg1ffe;
	} 
    
    if (address == 0x1fff) {
		return rm->reg1fff;
	} 

    return 0xff;
}

static void write(RomMapperFMPAC* rm, UInt16 address, UInt8 value) 
{
    address &= 0x3fff;

    switch (address) {
    case 0x1ffe:
        if ((rm->enable & 0x10) == 0) {
            rm->reg1ffe = value;
            rm->sramEnabled = rm->reg1ffe == 0x4d && rm->reg1fff == 0x69;
        }
        break;
    case 0x1fff:
        if ((rm->enable & 0x10) == 0) {
            rm->reg1fff = value;
            rm->sramEnabled = rm->reg1ffe == 0x4d && rm->reg1fff == 0x69;
        }
        break;
	case 0x3ff4:
        if (rm->ym2413 != NULL) {
            ym2413WriteAddress(rm->ym2413, value);
        }
		break;
	case 0x3ff5:
        if (rm->ym2413 != NULL) {
            ym2413WriteData(rm->ym2413, value);
        }
		break;
	case 0x3ff6:
        rm->enable = value & 0x11;
        if (rm->enable & 0x10) {
            rm->reg1ffe = 0;
            rm->reg1fff = 0;
            rm->sramEnabled = 0;
        }
		break;
	case 0x3ff7:
        rm->bankSelect = value & 3;
		break;
	default:
		if (rm->sramEnabled && address < 0x1ffe) {
            rm->sram[address] = value;
		}
        break;
    }
}

static void writeIo(RomMapperFMPAC* rm, UInt16 port, UInt8 data)
{
    if (rm->enable & 1) {
        switch (port & 1) {
        case 0:
            ym2413WriteAddress(rm->ym2413, data);
            break;
        case 1:
            ym2413WriteData(rm->ym2413, data);
            break;
        }
    }
}

static void getDebugInfo(RomMapperFMPAC* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    if (rm->enable & 1) {
        ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevFmpac(), 2);
        dbgIoPortsAddPort(ioPorts, 0, 0x7c, DBG_IO_WRITE, 0);
        dbgIoPortsAddPort(ioPorts, 1, 0x7d, DBG_IO_WRITE, 0);
    }
    ym2413GetDebugInfo(rm->ym2413, dbgDevice);
}

int romMapperFMPACCreate(char* filename, UInt8* romData, 
                         int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    RomMapperFMPAC* rm;

    if (size != 0x10000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperFMPAC));

    rm->deviceHandle = deviceManagerRegister(ROM_FMPAC, &callbacks, rm);

    slotRegister(slot, sslot, startPage, 2, read, read, write, destroy, rm);

    rm->ym2413 = NULL;
    if (boardGetYm2413Enable()) {
        rm->ym2413 = ym2413Create(boardGetMixer());
        rm->debugHandle = debugDeviceRegister(DBGTYPE_AUDIO, langDbgDevFmpac(), &dbgCallbacks, rm);
        ioPortRegister(0x7c, NULL, writeIo, rm);
        ioPortRegister(0x7d, NULL, writeIo, rm);
    }

    memcpy(rm->romData, romData, 0x10000);
    memset(rm->sram, 0xff, 0x2000);
    rm->bankSelect = 0;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    rm->sramEnabled = 0;
    strcpy(rm->sramFilename, sramCreateFilename(filename));

    sramLoad(rm->sramFilename, rm->sram, 0x1ffe, pacHeader, strlen(pacHeader));

    slotMapPage(rm->slot, rm->sslot, rm->startPage,     rm->romData, 0, 0);
    slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, rm->romData + 0x2000, 0, 0);

    reset(rm);

    return 1;
}

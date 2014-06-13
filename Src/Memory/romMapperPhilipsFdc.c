/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperPhilipsFdc.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2006/06/02 02:28:28 $
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
#include "romMapperPhilipsFdc.h"
#include "WD2793.h"
#include "MediaDb.h"
#include "Disk.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt8* romData;
    WD2793* fdc;
    int slot;
    int sslot;
    int startPage;
    UInt8 sideReg;
    UInt8 driveReg;
} RomMapperPhilipsFdc;

static void saveState(RomMapperPhilipsFdc* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperPhilipsFdc");

    saveStateSet(state, "sideReg",  rm->sideReg);
    saveStateSet(state, "driveReg", rm->driveReg);
    
    saveStateClose(state);

    wd2793SaveState(rm->fdc);
}

static void loadState(RomMapperPhilipsFdc* rm)
{
    SaveState* state = saveStateOpenForRead("mapperPhilipsFdc");

    rm->sideReg  = (UInt8)saveStateGet(state, "sideReg",  0);
    rm->driveReg = (UInt8)saveStateGet(state, "driveReg", 0);

    saveStateClose(state);

    wd2793LoadState(rm->fdc);
}

static void destroy(RomMapperPhilipsFdc* rm)
{
    wd2793Destroy(rm->fdc);
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static UInt8 read(RomMapperPhilipsFdc* rm, UInt16 address) 
{
	switch (address & 0x3fff) {
	case 0x3ff8:
		return wd2793GetStatusReg(rm->fdc);
	case 0x3ff9:
		return wd2793GetTrackReg(rm->fdc);
	case 0x3ffa:
		return wd2793GetSectorReg(rm->fdc);
	case 0x3ffb:
		return wd2793GetDataReg(rm->fdc);
	case 0x3ffc:
        return rm->sideReg;
	case 0x3ffd:
        return rm->driveReg;
	case 0x3ffe:
        return 0xff;
	case 0x3fff:
        return (wd2793GetIrq(rm->fdc) ? 0 : 0x40) | (wd2793GetDataRequest(rm->fdc) ? 0 : 0x80);
    }

    return address < 0x4000 ? rm->romData[address] : 0xff;
}

static UInt8 peek(RomMapperPhilipsFdc* rm, UInt16 address) 
{
	switch (address & 0x3fff) {
	case 0x3ff8:
		return 0xff; // Get from fdc
	case 0x3ff9:
		return 0xff; // Get from fdc
	case 0x3ffa:
		return 0xff; // Get from fdc
	case 0x3ffb:
		return 0xff; // Get from fdc
	case 0x3ffc:
        return rm->sideReg;
	case 0x3ffd:
        return rm->driveReg;
	case 0x3ffe:
        return 0xff;
	case 0x3fff:
		return 0xff; // Get from fdc
    }

    return address < 0x4000 ? rm->romData[address] : 0xff;
}

static void write(RomMapperPhilipsFdc* rm, UInt16 address, UInt8 value) 
{
	switch (address & 0x3fff) {
	case 0x3ff8:
		wd2793SetCommandReg(rm->fdc, value);
		break;
	case 0x3ff9:
	    wd2793SetTrackReg(rm->fdc, value);
		break;
	case 0x3ffa:
		wd2793SetSectorReg(rm->fdc, value);
		break;
	case 0x3ffb:
		wd2793SetDataReg(rm->fdc, value);
		break;
	case 0x3ffc:
        rm->sideReg = value;
        wd2793SetSide(rm->fdc, value & 1);
        break;
	case 0x3ffd:
        switch (value & 3) {
        case 0:
		case 2:
			wd2793SetDrive(rm->fdc, 0);
            wd2793SetMotor(rm->fdc, value & 0x80);
			break;
		case 1:
			wd2793SetDrive(rm->fdc, 1);
            wd2793SetMotor(rm->fdc, value & 0x80);
			break;
		default:
			wd2793SetDrive(rm->fdc, -1);
            wd2793SetMotor(rm->fdc, 0);
		}
		break;
	}
}	

static void reset(RomMapperPhilipsFdc* rm)
{
    wd2793Reset(rm->fdc);
    write(rm, 0xffc, 0);
    write(rm, 0xffd, 0);
}

int romMapperPhilipsFdcCreate(char* filename, UInt8* romData, 
                              int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperPhilipsFdc* rm;
    int pages = 4;
    int i;

    if ((startPage + pages) > 8) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperPhilipsFdc));

    rm->deviceHandle = deviceManagerRegister(ROM_PHILIPSFDC, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, read, peek, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    rm->sideReg = 0;
    rm->driveReg = 0;

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, NULL, 0, 0);
    }

    rm->fdc = wd2793Create();

    reset(rm);

    return 1;
}

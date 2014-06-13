/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSvi738Fdc.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2006/06/03 17:55:54 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Tomas Karlsson
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
#include "romMapperSvi738Fdc.h"
#include "WD2793.h"
#include "Led.h"
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
    UInt8 drvSelect;
} RomMapperSvi738Fdc;

static void saveState(RomMapperSvi738Fdc* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperSvi738Fdc");

    saveStateSet(state, "drvSelect",  rm->drvSelect);
    
    saveStateClose(state);

    wd2793SaveState(rm->fdc);
}

static void loadState(RomMapperSvi738Fdc* rm)
{
    SaveState* state = saveStateOpenForRead("mapperSvi738Fdc");

    rm->drvSelect  = (UInt8)saveStateGet(state, "drvSelect",  0);

    saveStateClose(state);

    wd2793LoadState(rm->fdc);
}

static void destroy(RomMapperSvi738Fdc* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    wd2793Destroy(rm->fdc);

    free(rm->romData);
    free(rm);
}

static UInt8 read(RomMapperSvi738Fdc* rm, UInt16 address) 
{
    switch (address & 0x3fff) {
        case 0x3fb8:
            return wd2793GetStatusReg(rm->fdc);
        case 0x3fb9:
            return wd2793GetTrackReg(rm->fdc);
        case 0x3fba:
            return wd2793GetSectorReg(rm->fdc);
        case 0x3fbb:
            return wd2793GetDataReg(rm->fdc);
        case 0x3fbc:
            return (wd2793GetIrq(rm->fdc)?0x80:0) | (wd2793GetDataRequest(rm->fdc)?0:0x40);
        case 0x3fbd:
            return 0xff;
        case 0x3fbe:
            return 0xff;
        case 0x3fbf:
            return 0xff;
    }
    return address < 0x4000 ? rm->romData[address] : 0xff;
}

static UInt8 peek(RomMapperSvi738Fdc* rm, UInt16 address) 
{
    switch (address & 0x3fff) {
        case 0x3fb8:
            return 0xff; // Get from fdc
        case 0x3fb9:
            return 0xff; // Get from fdc
        case 0x3fba:
            return 0xff; // Get from fdc
        case 0x3fbb:
            return 0xff; // Get from fdc
        case 0x3fbc:
            return 0xff; // Get from fdc
        case 0x3fbd:
            return 0xff;
        case 0x3fbe:
            return 0xff;
        case 0x3fbf:
            return 0xff;
    }
    return address < 0x4000 ? rm->romData[address] : 0xff;
}

static void write(RomMapperSvi738Fdc* rm, UInt16 address, UInt8 value) 
{
    switch (address & 0x3fff) {
        case 0x3fb8:
            wd2793SetCommandReg(rm->fdc, value);
            break;
        case 0x3fb9:
            wd2793SetTrackReg(rm->fdc, value);
            break;
        case 0x3fba:
            wd2793SetSectorReg(rm->fdc, value);
            break;
        case 0x3fbb:
            wd2793SetDataReg(rm->fdc, value);
            break;
        case 0x3fbc:
            rm->drvSelect = value & 0x3f;
            wd2793SetSide(rm->fdc, value & 4);
            wd2793SetMotor(rm->fdc, value & 8);
            if (diskEnabled(0)) ledSetFdd1(value & 1);
            if (diskEnabled(1)) ledSetFdd2(value & 2);
            switch (value & 3) {
                case 1:
                    wd2793SetDrive(rm->fdc, 0);
                    break;
                case 2:
                    wd2793SetDrive(rm->fdc, 1);
                    break;
                default:
                    wd2793SetDrive(rm->fdc, -1);
            }
            break;
    }
}       

static void reset(RomMapperSvi738Fdc* rm)
{
    wd2793Reset(rm->fdc);
    write(rm, 0xffc, 0);
    write(rm, 0xffd, 0);
}

int romMapperSvi738FdcCreate(char* filename, UInt8* romData, 
                              int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperSvi738Fdc* rm;
    int pages = 4;
    int i;

    if ((startPage + pages) > 8) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperSvi738Fdc));

    rm->deviceHandle = deviceManagerRegister(ROM_SVI738FDC, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, read, peek, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, NULL, 0, 0);
    }

    rm->fdc = wd2793Create();

    reset(rm);

    return 1;
}

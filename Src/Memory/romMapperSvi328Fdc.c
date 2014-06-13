/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSvi328Fdc.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/14 19:59:52 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Tomas Karlsson
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
#include "romMapperSvi328Fdc.h"
#include "WD2793.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "IoPort.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int deviceHandle;
    int debugHandle;
    WD2793* fdc;
    UInt8 drvSelect;
} Svi328Fdc;

static void saveState(Svi328Fdc* rm)
{
    SaveState* state = saveStateOpenForWrite("Svi328Fdc");

    saveStateSet(state, "drvSelect",  rm->drvSelect);
    
    saveStateClose(state);

    wd2793SaveState(rm->fdc);
}

static void loadState(Svi328Fdc* rm)
{
    SaveState* state = saveStateOpenForRead("Svi328Fdc");

    rm->drvSelect  = (UInt8)saveStateGet(state, "drvSelect",  0);

    saveStateClose(state);

    wd2793LoadState(rm->fdc);
}

static void destroy(Svi328Fdc* rm)
{
    ioPortUnregister(0x30);
    ioPortUnregister(0x31);
    ioPortUnregister(0x32);
    ioPortUnregister(0x33);
    ioPortUnregister(0x34);
    ioPortUnregister(0x38);

    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    wd2793Destroy(rm->fdc);

    free(rm);
}

static UInt8 peekIo(Svi328Fdc* rm, UInt16 ioPort) 
{
    switch (ioPort) {
        case 0x30:
            return wd2793PeekStatusReg(rm->fdc);
        case 0x31:
            return wd2793PeekTrackReg(rm->fdc);
        case 0x32:
            return wd2793PeekSectorReg(rm->fdc);
        case 0x33:
            return wd2793PeekDataReg(rm->fdc);
        case 0x34:
            return rm->drvSelect;
        case 0x38:
            return 0xff;
    }
    return 0xff;
}

static UInt8 readIo(Svi328Fdc* rm, UInt16 ioPort) 
{
    switch (ioPort) {
        case 0x30:
            return wd2793GetStatusReg(rm->fdc);
        case 0x31:
            return wd2793GetTrackReg(rm->fdc);
        case 0x32:
            return wd2793GetSectorReg(rm->fdc);
        case 0x33:
            return wd2793GetDataReg(rm->fdc);
        case 0x34:
            return (wd2793GetIrq(rm->fdc) ? 0x80:0) | (wd2793GetDataRequest(rm->fdc) ? 0x40:0);
    }
    return 0xff;
}

static void writeIo(Svi328Fdc* rm, UInt16 ioPort, UInt8 value) 
{
    switch (ioPort) {
        case 0x30:
            wd2793SetCommandReg(rm->fdc, value);
            break;
        case 0x31:
            wd2793SetTrackReg(rm->fdc, value);
            break;
        case 0x32:
            wd2793SetSectorReg(rm->fdc, value);
            break;
        case 0x33:
            wd2793SetDataReg(rm->fdc, value);
            break;
        case 0x34:
            rm->drvSelect = value & 0x3f;
            switch (value & 3) {
                case 1:
                    wd2793SetDrive(rm->fdc, 0);
                    wd2793SetMotor(rm->fdc, 1);
                    break;
                case 2:
                    wd2793SetDrive(rm->fdc, 1);
                    wd2793SetMotor(rm->fdc, 1);
                    break;
                default:
                    wd2793SetDrive(rm->fdc, -1);
                    wd2793SetMotor(rm->fdc, 0);
            }
            break;
        case 0x38:
            wd2793SetDensity(rm->fdc, value & 0x01 ? 1:0);
            wd2793SetSide(rm->fdc, value & 0x02 ? 1:0);
			
            break;

	}
}       

static void reset(Svi328Fdc* rm)
{
    wd2793Reset(rm->fdc);
}

static void getDebugInfo(Svi328Fdc* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;
    int i;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevSviFdc(), 5);
    for (i = 0; i < 5; i++) {
        dbgIoPortsAddPort(ioPorts, i, 0x30 + i, DBG_IO_READWRITE, peekIo(rm, 0x30 + i));
    }
}

int svi328FdcCreate(void) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    Svi328Fdc* rm;

    rm = malloc(sizeof(Svi328Fdc));
    
    rm->deviceHandle = deviceManagerRegister(ROM_SVI328FDC, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_PORT, langDbgDevSviFdc(), &dbgCallbacks, rm);

    ioPortRegister(0x30, readIo, writeIo, rm);
    ioPortRegister(0x31, readIo, writeIo, rm);
    ioPortRegister(0x32, readIo, writeIo, rm);
    ioPortRegister(0x33, readIo, writeIo, rm);
    ioPortRegister(0x34, readIo, writeIo, rm);
    ioPortRegister(0x38, readIo, writeIo, rm);

    rm->fdc = wd2793Create();

    reset(rm);

    return 1;
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperTurboRPcm.c,v $
**
** $Revision: 1.10 $
**
** $Date: 2006/06/14 19:59:52 $
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
#include "romMapperTurboRPcm.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "Board.h"
#include "DAC.h"
#include "IoPort.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    DAC*   dac;
    int    deviceHandle;
    int    debugHandle;
    UInt8  sample;
    UInt8  status;
    UInt8  time;
    UInt32 refTime;
    UInt32 refFrag;
} RomMapperTurboRPcm;

static void saveState(RomMapperTurboRPcm* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperTurboRPcm");

    saveStateSet(state, "sample",  rm->sample);
    saveStateSet(state, "status",  rm->status);
    saveStateSet(state, "time",    rm->time);
    saveStateSet(state, "refTime", rm->refTime);
    saveStateSet(state, "refFrag", rm->refFrag);
    
    saveStateClose(state);
}

static void loadState(RomMapperTurboRPcm* rm)
{
    SaveState* state = saveStateOpenForRead("mapperTurboRPcm");

    rm->sample  = (UInt8)saveStateGet(state, "sample",  0);
    rm->status  = (UInt8)saveStateGet(state, "status",  0);
    rm->time    = (UInt8)saveStateGet(state, "time",    0);
    rm->refTime =        saveStateGet(state, "refTime", 0);
    rm->refFrag =        saveStateGet(state, "refFrag", 0);

    saveStateClose(state);
}

static void destroy(RomMapperTurboRPcm* rm)
{
    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);
    dacDestroy(rm->dac);

    ioPortUnregister(0xa4);
    ioPortUnregister(0xa5);

    free(rm);
}

static UInt8 getTimerCounter(RomMapperTurboRPcm* rm)
{
    UInt64 elapsed;
    UInt32 systemTime = boardSystemTime();

    elapsed      = 15750 * (UInt64)(systemTime - rm->refTime) + rm->refFrag;
    rm->refTime  = systemTime;
    rm->refFrag  = (UInt32)(elapsed % boardFrequency());
    rm->time += (UInt8)(elapsed / boardFrequency());

    return rm->time & 0x03;
}

static UInt8 read(RomMapperTurboRPcm* rm, UInt16 ioPort)
{
    switch (ioPort & 0x01) {
	case 0: 
		return getTimerCounter(rm);
	case 1:
		return (~rm->sample & 0x80) | rm->status;
    }
    return 0xff;
}

static UInt8 peek(RomMapperTurboRPcm* rm, UInt16 ioPort)
{
    switch (ioPort & 0x01) {
	case 0: 
        return rm->time & 0x03;
	case 1:
		return (~rm->sample & 0x80) | rm->status;
    }
    return 0xff;
}

static void write(RomMapperTurboRPcm* rm, UInt16 ioPort, UInt8 value)
{
	switch (ioPort & 0x01) {
	case 0:
        getTimerCounter(rm);
        rm->time = 0;
		rm->sample = value;
		if (rm->status & 0x02) {
            dacWrite(rm->dac, DAC_CH_MONO, rm->sample);
		}
		break;

    case 1:
        if ((value & 0x03) == 0x03 && (~rm->status & 0x01)) {
            dacWrite(rm->dac, DAC_CH_MONO, rm->sample);
		}
		rm->status = value & 0x1f;
        break;
    }
}

static void reset(RomMapperTurboRPcm* rm)
{
    rm->refTime = boardSystemTime();
    rm->refFrag = 0;
    rm->time    = 0;
}

static void getDebugInfo(RomMapperTurboRPcm* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevPcm(), 2);
    dbgIoPortsAddPort(ioPorts, 0, 0xa4, DBG_IO_READWRITE, peek(rm, 0xa4));
    dbgIoPortsAddPort(ioPorts, 1, 0xa5, DBG_IO_READWRITE, peek(rm, 0xa5));
}

int romMapperTurboRPcmCreate() 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    RomMapperTurboRPcm* rm = malloc(sizeof(RomMapperTurboRPcm));

    rm->deviceHandle = deviceManagerRegister(ROM_TURBORPCM, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_AUDIO, langDbgDevPcm(), &dbgCallbacks, rm);

    rm->dac    = dacCreate(boardGetMixer(), DAC_MONO);
	rm->status = 0;
    rm->time   = 0;

    ioPortRegister(0xa4, read, write, rm);
    ioPortRegister(0xa5, read, write, rm);

    return 1;
}


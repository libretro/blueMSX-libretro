/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperMoonsound.c,v $
**
** $Revision: 1.6 $
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
#include "romMapperMoonsound.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SlotManager.h"
#include "IoPort.h"
#include "Moonsound.h"
#include "Board.h"
#include "SaveState.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    int      deviceHandle;
    int      debugHandle;
    Moonsound* moonsound;
} RomMapperMoonsound;

static void destroy(RomMapperMoonsound* rm)
{
    ioPortUnregister(0x7e);
    ioPortUnregister(0x7f);
    ioPortUnregister(0xc4);
    ioPortUnregister(0xc5);
    ioPortUnregister(0xc6);
    ioPortUnregister(0xc7);

    if (rm->moonsound != NULL) {
        moonsoundDestroy(rm->moonsound);
    }

    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    free(rm);
}

static void reset(RomMapperMoonsound* rm) 
{
    if (rm->moonsound != NULL) {
        moonsoundReset(rm->moonsound);
    }
}

static void loadState(RomMapperMoonsound* rm)
{
    SaveState* state = saveStateOpenForRead("RomMapperMoonsound");

    saveStateClose(state);
    
    if (rm->moonsound != NULL) {
        moonsoundLoadState(rm->moonsound);
    }
}

static void saveState(RomMapperMoonsound* rm)
{
    SaveState* state = saveStateOpenForWrite("RomMapperMoonsound");

    saveStateClose(state);

    if (rm->moonsound != NULL) {
        moonsoundSaveState(rm->moonsound);
    }
}

static UInt8 read(RomMapperMoonsound* rm, UInt16 ioPort)
{
    return moonsoundRead(rm->moonsound, ioPort);
}

static UInt8 peek(RomMapperMoonsound* rm, UInt16 ioPort)
{
    return moonsoundPeek(rm->moonsound, ioPort);
}

static void write(RomMapperMoonsound* rm, UInt16 ioPort, UInt8 data)
{
    moonsoundWrite(rm->moonsound, ioPort, data);
}

static void getDebugInfo(RomMapperMoonsound* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    moonsoundGetDebugInfo(rm->moonsound, dbgDevice);

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevMoonsound(), 6);
    dbgIoPortsAddPort(ioPorts, 0, 0x7e, DBG_IO_READWRITE, peek(rm, 0x7e));
    dbgIoPortsAddPort(ioPorts, 1, 0x7f, DBG_IO_READWRITE, peek(rm, 0x7f));
    dbgIoPortsAddPort(ioPorts, 2, 0xc4, DBG_IO_READWRITE, peek(rm, 0xc4));
    dbgIoPortsAddPort(ioPorts, 3, 0xc5, DBG_IO_READWRITE, peek(rm, 0xc5));
    dbgIoPortsAddPort(ioPorts, 4, 0xc6, DBG_IO_READWRITE, peek(rm, 0xc6));
    dbgIoPortsAddPort(ioPorts, 5, 0xc7, DBG_IO_READWRITE, peek(rm, 0xc7));
}

int romMapperMoonsoundCreate(char* filename, UInt8* romData, int size, int sramSize)
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    RomMapperMoonsound* rm = malloc(sizeof(RomMapperMoonsound));

    rm->deviceHandle = deviceManagerRegister(ROM_MOONSOUND, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_AUDIO, langDbgDevMoonsound(), &dbgCallbacks, rm);
    
    rm->moonsound = NULL;

    if (boardGetMoonsoundEnable()) {
        rm->moonsound = moonsoundCreate(boardGetMixer(), romData, size, sramSize);
        ioPortRegister(0x7e, read, write, rm);
        ioPortRegister(0x7f, read, write, rm);
        ioPortRegister(0xc4, read, write, rm);
        ioPortRegister(0xc5, read, write, rm);
        ioPortRegister(0xc6, read, write, rm);
        ioPortRegister(0xc7, read, write, rm);
    }

    reset(rm);

    return 1;
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/TurboRIO.c,v $
**
** $Revision: 1.3 $
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
#include "TurboRIO.h"
#include "IoPort.h"
#include "Board.h"
#include "Led.h"
#include "Switches.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "Language.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct TurboRIO {
    int deviceHandle;
    int debugHandle;
} TurboRIO;

static void write(TurboRIO* turboRIO, UInt16 ioPort, UInt8 value)
{
	ledSetPause(value & 0x01);
	ledSetTurboR(value & 0x80);
}

static UInt8 read(TurboRIO* turboRIO, UInt16 ioPort)
{
    return switchGetPause() ? 1 : 0;
}

static void destroy(TurboRIO* turboRIO)
{
    ioPortUnregister(0xa7);

    debugDeviceUnregister(turboRIO->debugHandle);
    deviceManagerUnregister(turboRIO->deviceHandle);

    free(turboRIO);
}

static void getDebugInfo(TurboRIO* turboRIO, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevTrPause(), 1);
    dbgIoPortsAddPort(ioPorts, 0, 0xa7, DBG_IO_READWRITE, read(turboRIO, 0xa7));
}

int romMapperTurboRIOCreate()
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };

    TurboRIO* turboRIO = (TurboRIO*)calloc(1, sizeof(TurboRIO));

    turboRIO->deviceHandle = deviceManagerRegister(ROM_TURBORIO, &callbacks, turboRIO);
    turboRIO->debugHandle = debugDeviceRegister(DBGTYPE_PORT, langDbgDevTrPause(), &dbgCallbacks, turboRIO);

    ioPortRegister(0xa7, read, write, turboRIO);

    return 1;
}

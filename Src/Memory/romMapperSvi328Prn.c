/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSvi328Prn.c,v $
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
#include "romMapperSvi328Prn.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "IoPort.h"
#include "PrinterIO.h"
#include "Language.h"
#include <stdlib.h>

typedef struct {
    int deviceHandle;
    int debugHandle;
    UInt8 prnData;
    UInt8 prnStrobe;
    PrinterIO* printerIO;
} RomMapperSvi328Prn;

static void saveState(RomMapperSvi328Prn* prn)
{
    SaveState* state = saveStateOpenForWrite("Svi328Prn");

    saveStateSet(state, "prnData", prn->prnData);
    saveStateSet(state, "prnStrobe", prn->prnStrobe);

    saveStateClose(state);
}

static void loadState(RomMapperSvi328Prn* prn)
{
    SaveState* state = saveStateOpenForRead("Svi328Prn");

    prn->prnData  = (UInt8)saveStateGet(state, "prnData",  0);
    prn->prnStrobe  = (UInt8)saveStateGet(state, "prnStrobe",  0);

    saveStateClose(state);
}

static void destroy(RomMapperSvi328Prn* prn)
{
    ioPortUnregister(0x10);
    ioPortUnregister(0x11);
    ioPortUnregister(0x12);

    deviceManagerUnregister(prn->deviceHandle);
    debugDeviceUnregister(prn->debugHandle);

    printerIODestroy(prn->printerIO);

    free(prn);
}

static UInt8 peekIo(RomMapperSvi328Prn* prn, UInt16 ioPort) 
{
    switch (ioPort) {
        case 0x10:
            return prn->prnData;
        case 0x11:
            return prn->prnStrobe;
        case 0x12:
            if (printerIOGetStatus(prn->printerIO)) 
                return 0xfe;
    }
    return 0xff;
}

static UInt8 readIo(RomMapperSvi328Prn* prn, UInt16 ioPort) 
{
    if (printerIOGetStatus(prn->printerIO)) 
        return 0xfe;

    return 0xff;
}

static void writeIo(RomMapperSvi328Prn* prn, UInt16 ioPort, UInt8 value) 
{
    switch (ioPort) {
        case 0x10:
            prn->prnData = value;
            break;
        case 0x11:
            if ((prn->prnStrobe & 1) && !(value & 1))
                printerIOWrite(prn->printerIO, prn->prnData);

            prn->prnStrobe = value;
            break;
    }
}  

static void reset(RomMapperSvi328Prn* prn)
{
    prn->prnStrobe = 0;
    prn->prnData = 0;
}

static void getDebugInfo(RomMapperSvi328Prn* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevSviPrn(), 3);
    dbgIoPortsAddPort(ioPorts, 0, 0x10, DBG_IO_READWRITE, peekIo(rm, 0x10));
    dbgIoPortsAddPort(ioPorts, 1, 0x11, DBG_IO_READWRITE, peekIo(rm, 0x11));
    dbgIoPortsAddPort(ioPorts, 2, 0x12, DBG_IO_READWRITE, peekIo(rm, 0x12));
}

int romMapperSvi328PrnCreate(void) 
{
    DeviceCallbacks callbacks = {destroy, reset, saveState, loadState};
    DebugCallbacks dbgCallbacks = {getDebugInfo, NULL, NULL, NULL};
    RomMapperSvi328Prn* prn;

    prn = malloc(sizeof(RomMapperSvi328Prn));

    prn->printerIO = printerIOCreate();

    prn->deviceHandle = deviceManagerRegister(ROM_SVI328PRN, &callbacks, prn);
    prn->debugHandle = debugDeviceRegister(DBGTYPE_BIOS, langDbgDevSviPrn(), &dbgCallbacks, prn);

    ioPortRegister(0x10, NULL, writeIo, prn);
    ioPortRegister(0x11, NULL, writeIo, prn);
    ioPortRegister(0x12, readIo, NULL, prn);

    reset(prn);

    return 1;
}

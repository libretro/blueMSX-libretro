/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSvi80Col.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2006/07/07 08:24:23 $
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
#include "romMapperSvi80Col.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "IoPort.h"
#include "CRTC6845.h"
#include "Language.h"
#include <stdlib.h>

typedef struct {
    int deviceHandle;
    int debugHandle;
    UInt8 memBankCtrl;
    CRTC6845* crtc6845;
} RomMapperSvi80Col;

static RomMapperSvi80Col* svi80colInstance = NULL;

static void saveState(RomMapperSvi80Col* svi80col)
{
    SaveState* state = saveStateOpenForWrite("Svi80Col");

    saveStateSet(state, "memBankCtrl", svi80col->memBankCtrl);
    
    saveStateClose(state);
}

static void loadState(RomMapperSvi80Col* svi80col)
{
    SaveState* state = saveStateOpenForRead("Svi80Col");

    svi80col->memBankCtrl = (UInt8)saveStateGet(state, "memBankCtrl", 0);

    saveStateClose(state);
}

static void destroy(RomMapperSvi80Col* svi80col)
{

    ioPortUnregister(0x50);
    ioPortUnregister(0x51);
    ioPortUnregister(0x58);

    deviceManagerUnregister(svi80col->deviceHandle);
    debugDeviceUnregister(svi80col->debugHandle);

    free(svi80col);
}

static UInt8 peekIo(RomMapperSvi80Col* svi80col, UInt16 ioPort) 
{
    switch (ioPort) {
        case 0x50:
            return svi80col->crtc6845->registers.address;
        case 0x51:
            return 0xff;
        case 0x58:
            return svi80col->memBankCtrl;
    }
    return 0xff;
}
static UInt8 readIo(RomMapperSvi80Col* svi80col, UInt16 ioPort) 
{
    switch (ioPort) {
        case 0x51:
            return crtcRead(svi80col->crtc6845);
        case 0x58:
//            svi80col->memBankCtrl = 1;
            break;
    }
    return 0xff;
}

static void writeIo(RomMapperSvi80Col* svi80col, UInt16 ioPort, UInt8 value) 
{
    switch (ioPort) {
        case 0x50:
            crtcWriteLatch(svi80col->crtc6845, value);
            break;
        case 0x51:
            crtcWrite(svi80col->crtc6845, value);
            break;
        case 0x58:
            svi80col->memBankCtrl = value & 1;
            break;
    }
}  

int svi80colMemBankCtrlStatus(void)
{
   return svi80colInstance->memBankCtrl;
}

void svi80colMemWrite(UInt16 address, UInt8 value)
{
    crtcMemWrite(svi80colInstance->crtc6845, address, value);
}

UInt8 svi80colMemRead(UInt16 address)
{
    return crtcMemRead(svi80colInstance->crtc6845, address);
}

static void reset(RomMapperSvi80Col* svi80col)
{
    svi80col->memBankCtrl = 0;
}

static void getDebugInfo(RomMapperSvi80Col* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevSvi80Col(), 3);
    dbgIoPortsAddPort(ioPorts, 0, 0x50, DBG_IO_READWRITE, peekIo(rm, 0x50));
    dbgIoPortsAddPort(ioPorts, 1, 0x51, DBG_IO_READWRITE, peekIo(rm, 0x51));
    dbgIoPortsAddPort(ioPorts, 2, 0x58, DBG_IO_READWRITE, peekIo(rm, 0x58));
}

int romMapperSvi80ColCreate(int frameRate, UInt8* romData, int size)
{
    DeviceCallbacks callbacks = {destroy, reset, saveState, loadState};
    DebugCallbacks dbgCallbacks = {getDebugInfo, NULL, NULL, NULL};
    RomMapperSvi80Col* svi80col;

    if (size != 0x1000)
    	return 0;

    svi80col = malloc(sizeof(RomMapperSvi80Col));
    svi80colInstance = svi80col;

    svi80col->deviceHandle = deviceManagerRegister(ROM_SVI80COL, &callbacks, svi80col);

    svi80col->crtc6845 = NULL;
    svi80col->crtc6845 = crtc6845Create(frameRate, romData, size, 0x800, 7, 0, 82, 4);

    svi80col->debugHandle = debugDeviceRegister(DBGTYPE_VIDEO, langDbgDevSvi80Col(), &dbgCallbacks, svi80col);

    ioPortRegister(0x50, NULL,   writeIo, svi80col);
    ioPortRegister(0x51, readIo, writeIo, svi80col);
    ioPortRegister(0x58, readIo, writeIo, svi80col);

    reset(svi80col);

    return 1;
}

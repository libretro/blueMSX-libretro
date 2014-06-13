/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperKonamiKeyboardMaster.c,v $
**
** $Revision: 1.4 $
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
#include "romMapperKonamiKeyboardMaster.h"
#include "VLM5030VoiceData.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "VLM5030.h"
#include "Board.h"
#include "SaveState.h"
#include "IoPort.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    int debugHandle;

    UInt8* romData;
    UInt8* voiceData;
	VLM5030* vlm5030;
    int slot;
    int sslot;
    int startPage;
} RomMapperKonamiKeyboardMaster;

static void saveState(RomMapperKonamiKeyboardMaster* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperKonamiKbdMaster");
    
    saveStateClose(state);
    
    vlm5030SaveState(rm->vlm5030);
}

static void loadState(RomMapperKonamiKeyboardMaster* rm)
{
    SaveState* state = saveStateOpenForRead("mapperKonamiKbdMaster");

    saveStateClose(state);
    
    vlm5030LoadState(rm->vlm5030);
}

static void destroy(RomMapperKonamiKeyboardMaster* rm)
{
    ioPortUnregister(0x00);
    ioPortUnregister(0x20);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);
    vlm5030Destroy(rm->vlm5030);

    free(rm->romData);
    free(rm->voiceData);
    free(rm);
}

static UInt8 read(RomMapperKonamiKeyboardMaster* rm, UInt16 ioPort)
{
    switch (ioPort) {
    case 0x00:
        return vlm5030Read(rm->vlm5030, 0);
    case 0x20:
        return vlm5030Read(rm->vlm5030, 1);
        break;
    }
    return 0xff;
}

static void write(RomMapperKonamiKeyboardMaster* rm, UInt16 ioPort, UInt8 value)
{	
    switch (ioPort) {
    case 0x00:
        vlm5030Write(rm->vlm5030, 0, value);
        break;
    case 0x20:
        vlm5030Write(rm->vlm5030, 1, value);
        break;
    }
}

static void getDebugInfo(RomMapperKonamiKeyboardMaster* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevKonamiKbd(), 2);
    dbgIoPortsAddPort(ioPorts, 0, 0x00, DBG_IO_WRITE, 0);
    dbgIoPortsAddPort(ioPorts, 1, 0x20, DBG_IO_READWRITE, read(rm, 0x20));
}

int romMapperKonamiKeyboardMasterCreate(char* filename, UInt8* romData, 
                                        int size, int slot, int sslot, 
                                        int startPage,
                                       void* voiceRom, int voiceSize) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    RomMapperKonamiKeyboardMaster* rm;
    int i;

    if (size != 0x4000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperKonamiKeyboardMaster));

    rm->deviceHandle = deviceManagerRegister(ROM_KONAMKBDMAS, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_CART, langDbgDevKonamiKbd(), &dbgCallbacks, rm);

    slotRegister(slot, sslot, startPage, 4, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);

    rm->voiceData = calloc(1, 0x4000);
    if (voiceRom != NULL) {
        if (voiceSize > 0x4000) {
            voiceSize = 0x4000;
        }
        memcpy(rm->voiceData, voiceRom, voiceSize);
    }
    else {
        memcpy(rm->voiceData, voiceData, 0x4000);
    }

    rm->vlm5030 = vlm5030Create(boardGetMixer(), voiceData, 0x4000);
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    for (i = 0; i < 2; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + i * 0x2000, 1, 0);
    }

    ioPortRegister(0x00, read, write, rm);
    ioPortRegister(0x20, read, write, rm);

    return 1;
}


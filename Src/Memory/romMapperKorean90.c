/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperKorean90.c,v $
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
#include "romMapperKorean90.h"
#include "MediaDb.h"
#include "SlotManager.h"
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
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
    int size;
    int romMapper[4];
} RomMapperKorean90;

static void saveState(RomMapperKorean90* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperKorean90");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateClose(state);
}

static void loadState(RomMapperKorean90* rm)
{
    SaveState* state = saveStateOpenForRead("mapperKorean90");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }

    saveStateClose(state);

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }
}

static void destroy(RomMapperKorean90* rm)
{
    ioPortUnregister(0x77);
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperKorean90* rm, UInt16 address, UInt8 value) 
{
    int page = (value & 0x3f) << 1;
    int mode = value >> 6;
    int i;

    switch (mode) {
    case 0:
    case 1:
        rm->romMapper[0] = page + 0;
        rm->romMapper[1] = page + 1;
        rm->romMapper[2] = page + 0;
        rm->romMapper[3] = page + 1;
        break;
    case 2:
        rm->romMapper[0] = (page & 0xfc) + 0;
        rm->romMapper[1] = (page & 0xfc) + 1;
        rm->romMapper[2] = (page & 0xfc) + 2;
        rm->romMapper[3] = (page & 0xfc) + 3;
        break;
    case 3:
        rm->romMapper[0] = page + 0;
        rm->romMapper[1] = page + 1;
        rm->romMapper[2] = page + 1;
        rm->romMapper[3] = page + 0;
        break;
    }

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }
}

static void getDebugInfo(RomMapperKorean90* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevKorean90(), 1);
    dbgIoPortsAddPort(ioPorts, 0, 0x77, DBG_IO_WRITE, 0);
}

int romMapperKorean90Create(char* filename, UInt8* romData, 
                            int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    RomMapperKorean90* rm;
    int i;

    if (size < 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperKorean90));

    rm->deviceHandle = deviceManagerRegister(ROM_KOREAN90, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_CART, langDbgDevKorean90(), &dbgCallbacks, rm);

    slotRegister(slot, sslot, startPage, 4, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    rm->size = size;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 1;
    rm->romMapper[2] = 2;
    rm->romMapper[3] = 3;

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }
    
    ioPortRegister(0x77, NULL, write, rm);

    return 1;
}


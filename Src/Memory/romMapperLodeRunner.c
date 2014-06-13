/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperLodeRunner.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/02/13 21:20:01 $
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
#include "romMapperLodeRunner.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
    int size;
    int romMapper;
} RomMapperLodeRunner;

static void saveState(RomMapperLodeRunner* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperLodeRunner");

    saveStateSet(state, "romMapper", rm->romMapper);
    
    saveStateClose(state);
}

static void loadState(RomMapperLodeRunner* rm)
{
    SaveState* state = saveStateOpenForRead("mapperLodeRunner");
    UInt8* bankData;

    rm->romMapper = saveStateGet(state, "romMapper", 0);

    saveStateClose(state);

    bankData = rm->romData + (rm->romMapper << 14);
    slotMapPage(rm->slot, rm->sslot, rm->startPage,     bankData, 1, 0);
    slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, bankData + 0x2000, 1, 0);
}

static void destroy(RomMapperLodeRunner* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    free(rm->romData);
    free(rm);
}

static void write(RomMapperLodeRunner* rm, UInt16 address, UInt8 value) 
{
    value &= (rm->size / 0x4000 - 1);

    if (rm->romMapper != value) {
        UInt8* bankData = rm->romData + ((int)value << 14);
        
        rm->romMapper = value;

        slotMapPage(rm->slot, rm->sslot, rm->startPage,     bankData, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, bankData + 0x2000, 1, 0);
    }
}

int romMapperLodeRunnerCreate(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperLodeRunner* rm;

    if (size != 0x20000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperLodeRunner));

    rm->deviceHandle = deviceManagerRegister(ROM_LODERUNNER, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, NULL, destroy, rm);
    slotRegisterWrite0(write, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    rm->size = size;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->romMapper = 0;

    slotMapPage(rm->slot, rm->sslot, rm->startPage,     rm->romData + rm->romMapper * 0x2000, 1, 0);
    slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, rm->romData + rm->romMapper * 0x2000 + 0x2000, 1, 0);

    return 1;
}


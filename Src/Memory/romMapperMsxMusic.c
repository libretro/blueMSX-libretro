/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperMsxMusic.c,v $
**
** $Revision: 1.7 $
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
#include "romMapperMsxMusic.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SlotManager.h"
#include "IoPort.h"
#include "YM2413.h"
#include "Board.h"
#include "SaveState.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    int      deviceHandle;
    int      debugHandle;
    YM_2413* ym2413;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
} MsxMusic;

static void destroy(MsxMusic* rm)
{
    ioPortUnregister(0x7c);
    ioPortUnregister(0x7d);

    if (rm->ym2413 != NULL) {
        ym2413Destroy(rm->ym2413);
    }

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    free(rm->romData);
    free(rm);
}

static void reset(MsxMusic* rm) 
{
    if (rm->ym2413 != NULL) {
        ym2413Reset(rm->ym2413);
    }
}

static void loadState(MsxMusic* rm)
{
    SaveState* state = saveStateOpenForRead("MsxMusic");

    saveStateClose(state);
    
    if (rm->ym2413 != NULL) {
        ym2413LoadState(rm->ym2413);
    }
}

static void saveState(MsxMusic* rm)
{
    SaveState* state = saveStateOpenForWrite("MsxMusic");

    saveStateClose(state);

    if (rm->ym2413 != NULL) {
        ym2413SaveState(rm->ym2413);
    }
}

static void write(MsxMusic* rm, UInt16 ioPort, UInt8 data)
{
    switch (ioPort & 1) {
    case 0:
        ym2413WriteAddress(rm->ym2413, data);
        break;
    case 1:
        ym2413WriteData(rm->ym2413, data);
        break;
    }
}

static void getDebugInfo(MsxMusic* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevMsxMusic(), 2);
    dbgIoPortsAddPort(ioPorts, 0, 0x7c, DBG_IO_WRITE, 0);
    dbgIoPortsAddPort(ioPorts, 1, 0x7d, DBG_IO_WRITE, 0);
    
    ym2413GetDebugInfo(rm->ym2413, dbgDevice);
}

int romMapperMsxMusicCreate(char* filename, UInt8* romData, 
                            int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    MsxMusic* rm = malloc(sizeof(MsxMusic));
    int pages = size / 0x2000 + ((size & 0x1fff) ? 1 : 0);
    int i;

    if (pages == 0 || (startPage + pages) > 8) {
        return 0;
    }

    rm->deviceHandle = deviceManagerRegister(ROM_MSXMUSIC, &callbacks, rm);
    
    rm->ym2413 = NULL;
    if (boardGetYm2413Enable()) {
        rm->ym2413 = ym2413Create(boardGetMixer());
        rm->debugHandle = debugDeviceRegister(DBGTYPE_AUDIO, langDbgDevMsxMusic(), &dbgCallbacks, rm);
        ioPortRegister(0x7c, NULL, write, rm);
        ioPortRegister(0x7d, NULL, write, rm);
    }

    rm->romData = malloc(pages * 0x2000);
    memcpy(rm->romData, romData, size);

    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, rm->romData + 0x2000 * i, 1, 0);
    }

    reset(rm);

    return 1;
}

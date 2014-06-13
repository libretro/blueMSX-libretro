/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSvi727.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/02/18 09:32:32 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik, Tomas Karlsson
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
#include "romMapperSvi727.h"
#include "CRTC6845.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "IoPort.h"
#include "RomLoader.h"
#include <stdlib.h>
#include <memory.h>

typedef struct {
    int deviceHandle;
    UInt8* charData;
    int slot;
    int sslot;
    int startPage;
    CRTC6845* crtc6845;
} RomMapperSvi727;

static void saveState(RomMapperSvi727* rm)
{
    SaveState* state = saveStateOpenForWrite("Svi727");
    saveStateClose(state);
}

static void loadState(RomMapperSvi727* rm)
{
    SaveState* state = saveStateOpenForRead("Svi727");
    saveStateClose(state);
}

static void destroy(RomMapperSvi727* rm)
{
    ioPortUnregister(0x78);
    ioPortUnregister(0x79);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->charData);
    free(rm);
}

static UInt8 read(RomMapperSvi727* rm, UInt16 address)
{
    UInt8 value = 0xff;
    if (address >= 0xb800 && address < 0xc000) {
        value = crtcMemRead(rm->crtc6845, address & 0x07ff);
    }

    return value;
}

static void write(RomMapperSvi727* rm, UInt16 address, UInt8 value) 
{
    if (address >= 0xb800 && address < 0xc000) {
        crtcMemWrite(rm->crtc6845, address & 0x07ff, value);
    }
}

static UInt8 readIo(RomMapperSvi727* rm, UInt16 ioPort) 
{
    UInt8 value = crtcRead(rm->crtc6845);
    return value;
}

static void writeIo(RomMapperSvi727* rm, UInt16 ioPort, UInt8 value) 
{
    switch (ioPort) {
    case 0x78:
        crtcWriteLatch(rm->crtc6845, value);
        break;
    case 0x79:
        crtcWrite(rm->crtc6845, value);
        break;
    }
}

static void reset(RomMapperSvi727* rm)
{
}

int romMapperSvi727Create(char* filename, UInt8* charRom, int charSize,
                                 int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperSvi727* rm;
    int pages = 8;
    int i;

    startPage = 0;

    rm = malloc(sizeof(RomMapperSvi727));

    rm->deviceHandle = deviceManagerRegister(ROM_SVI727, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, read, read, write, destroy, rm);

    rm->charData = calloc(1, 0x2000);
    if (charRom != NULL) {
        charSize += 0x200;
        if (charSize > 0x2000) {
            charSize = 0x2000;
        }
        memcpy(rm->charData + 0x200, charRom, charSize - 0x200);
    }

    rm->crtc6845 = NULL;
    rm->crtc6845 = crtc6845Create(50, rm->charData, charSize, 0x0800, 7, 0, 80, 4);

    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage = startPage;

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, NULL, 0, 0);
    }

    ioPortRegister(0x78, NULL,   writeIo, rm);
    ioPortRegister(0x79, readIo, writeIo, rm);

    reset(rm);

    return 1;
}

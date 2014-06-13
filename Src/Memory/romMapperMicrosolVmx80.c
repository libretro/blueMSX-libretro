/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperMicrosolVmx80.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2006/01/25 21:18:49 $
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
#include "romMapperMicrosolVmx80.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "CRTC6845.h"
#include "RomLoader.h"
#include <stdlib.h>
#include <memory.h>

typedef struct {
    int deviceHandle;
    UInt8* romData;
    UInt8* charData;
    int slot;
    int sslot;
    int startPage;
    CRTC6845* crtc6845;
} RomMapperMicrosolVmx80;

static void saveState(RomMapperMicrosolVmx80* rm)
{
    SaveState* state = saveStateOpenForWrite("Vmx80");
    saveStateClose(state);
}

static void loadState(RomMapperMicrosolVmx80* rm)
{
    SaveState* state = saveStateOpenForRead("Vmx80");
    saveStateClose(state);
}

static void destroy(RomMapperMicrosolVmx80* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm->charData);
    free(rm);
}

static UInt8 read(RomMapperMicrosolVmx80* rm, UInt16 address)
{
    if (address == 0x3001) {
        return crtcRead(rm->crtc6845);
    }

    if (address > 0x1fff  && address < 0x2800) {
        return crtcMemRead(rm->crtc6845, address & 0x07ff);
    }

    return address < 0x4000 ? rm->romData[address] : 0xff;
}

static void write(RomMapperMicrosolVmx80* rm, UInt16 address, UInt8 value) 
{
    if (address >= 0x2000 && address < 0x2800) {
        crtcMemWrite(rm->crtc6845, address & 0x07ff, value);
    }
    if (address >= 0x3000 && address < 0x3800) {
        if (address & 1) {
            crtcWrite(rm->crtc6845, value);
        }
        else {
	        crtcWriteLatch(rm->crtc6845, value);
        }
    }
}
	
static void reset(RomMapperMicrosolVmx80* rm)
{
}

int romMapperMicrosolVmx80Create(char* filename, UInt8* romData, int size,
                                 int slot, int sslot, int startPage,
                                 void* charRom, int charSize) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperMicrosolVmx80* rm;
    int pages = 2;
    int i;

    if ((startPage + pages) > 8) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperMicrosolVmx80));

    rm->deviceHandle = deviceManagerRegister(ROM_MICROSOL80, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 2, read, read, write, destroy, rm);

    rm->charData = calloc(1, 0x2000);
    if (charRom != NULL) {
        if (charSize > 0x2000) {
            charSize = 0x2000;
        }
        memcpy(rm->charData, charRom, charSize);
    }

    rm->crtc6845 = NULL;
    rm->crtc6845 = crtc6845Create(50, rm->charData, charSize, 0x0800, 7, 0, 80, 4);

    rm->romData = calloc(1, size);
    memcpy(rm->romData, romData, size);
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, NULL, 0, 0);
    }

    reset(rm);

    return 1;
}

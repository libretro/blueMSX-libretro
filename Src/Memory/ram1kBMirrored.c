/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Memory/ram1kBMirrored.c,v $
**
** $Revision: 1.12 $
**
** $Date: 2008-05-19 19:25:59 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#include "ram1kBMirrored.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    int debugHandle;
    int slot;
    int sslot;
    int startPage;
    int pages;
    UInt32 mask;
    UInt8 ramData[0x2000];
} Ram1kBMirrored;

static void ram1kb_saveState(void *data)
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data;
    SaveState* state = saveStateOpenForWrite("mapperMirroredRam");

    saveStateSet(state, "mask", rm->mask);
    saveStateSetBuffer(state, "ramData", rm->ramData, rm->mask + 1);

    saveStateClose(state);
}

static void ram1kb_loadState(void *data)
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data;
    SaveState* state = saveStateOpenForRead("mapperMirroredRam");

    rm->mask = saveStateGet(state, "mask", 0x400);
    saveStateGetBuffer(state, "ramData", rm->ramData, rm->mask + 1);

    saveStateClose(state);
}

static void ram1kb_destroy(void *data)
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data;
    debugDeviceUnregister(rm->debugHandle);

    slotUnregister(rm->slot, rm->sslot, 0);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm);
}

static void ram1kb_getDebugInfo(void *data, DbgDevice* dbgDevice)
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data;
    dbgDeviceAddMemoryBlock(dbgDevice, langDbgMemRamNormal(), 0, 0, rm->mask + 1, rm->ramData);
}

static int ram1kb_dbgWriteMemory(void *data1, char* name, void *data2, int start, int size)
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data1;
    if (strcmp(name, "Normal") || start + size >= (int)rm->mask)
        return 0;

    memcpy(rm->ramData + start, data2, size);

    return 1;
}

static UInt8 ram1kb_read(void *data, UInt16 address) 
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data;
    return rm->ramData[address & rm->mask];
}

static void ram1kb_write(void *data, UInt16 address, UInt8 value) 
{
    Ram1kBMirrored *rm = (Ram1kBMirrored*)data;
    rm->ramData[address & rm->mask] = value;
}

int ramMirroredCreate(int size, int slot, int sslot, int startPage, 
                      UInt32 ramBlockSize, UInt8** ramPtr, UInt32* ramSize) 
{
    DeviceCallbacks callbacks = { ram1kb_destroy, NULL, ram1kb_saveState, ram1kb_loadState };
    DebugCallbacks dbgCallbacks = { ram1kb_getDebugInfo, ram1kb_dbgWriteMemory, NULL, NULL };
    Ram1kBMirrored* rm;
    int pages = size / 0x2000;
    int i;

    if (size > 0x10000 || (size & 0x1fff))
        return 0;

    // Start page must be zero (only full slot allowed)
    if (startPage + pages > 8)
        return 0;

    rm            = (Ram1kBMirrored*)malloc(sizeof(Ram1kBMirrored));

    rm->mask      = ramBlockSize - 1;
    rm->slot      = slot;
    rm->sslot     = sslot;
    rm->startPage = startPage;
    rm->pages     = pages;

    memset(rm->ramData, 0, sizeof(rm->ramData));

    rm->debugHandle = debugDeviceRegister(DBGTYPE_RAM, langDbgDevRam(), &dbgCallbacks, rm);

    for (i = 0; i < pages; i++)
        slotMapPage(slot, sslot, i + startPage, NULL, 0, 0);

    rm->deviceHandle = deviceManagerRegister(ramBlockSize == 0x400 ? RAM_1KB_MIRRORED : RAM_2KB_MIRRORED, 
                                             &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, ram1kb_read, ram1kb_read, ram1kb_write, ram1kb_destroy, rm);

    if (ramPtr)
        *ramPtr = rm->ramData;

    if (ramSize)
        *ramSize = rm->pages * 0x2000;

    return 1;
}

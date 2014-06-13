/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/ramMapperIo.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2006/06/16 22:52:10 $
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
#include "ramMapperIo.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "Language.h"
#include "IoPort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int        handle;
    MemIoWrite write;
    void*      ref;
    int        size;
} RamMapperCb;

typedef struct {
    int deviceHandle;
    int debugHandle;
    int handleCount;
    RamMapperCb mapperCb[32];
    int count;
    int mask;
    int port[4];
} RamMapperIo;

RamMapperIo* mapperIo = NULL;


static int ramMapperIoGetMask(RamMapperIo* rm)
{
    int size = 1;
    int i;

    for (i = 0; i < rm->count; i++) {
        while (size < rm->mapperCb[i].size) {
            size <<= 1;
        }
    }

    return (size / 0x4000) - 1;
}

static void saveState(RamMapperIo* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperRamIo");
    saveStateSet(state, "port0", rm->port[0]);
    saveStateSet(state, "port1", rm->port[1]);
    saveStateSet(state, "port2", rm->port[2]);
    saveStateSet(state, "port3", rm->port[3]);

    saveStateClose(state);
}

static void loadState(RamMapperIo* rm)
{
    SaveState* state = saveStateOpenForRead("mapperRamIo");
    rm->port[0] = saveStateGet(state, "port0", 3);
    rm->port[1] = saveStateGet(state, "port1", 2);
    rm->port[2] = saveStateGet(state, "port2", 1);
    rm->port[3] = saveStateGet(state, "port3", 0);
    
    rm->mask = ramMapperIoGetMask(rm);

    saveStateClose(state);
}

static void destroy(RamMapperIo* rm) 
{
    ioPortUnregister(0xfc);
    ioPortUnregister(0xfd);
    ioPortUnregister(0xfe);
    ioPortUnregister(0xff);

    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    free(mapperIo);
    mapperIo = NULL;
}

static UInt8 read(RamMapperIo* rm, UInt16 ioPort)
{
    return rm->port[ioPort & 3] | ~rm->mask;
}

static void write(RamMapperIo* rm, UInt16 ioPort, UInt8 value)
{
    ioPort &= 3;

    if (rm->port[ioPort] != value) {
        int i;

        rm->port[ioPort] = value;

        for (i = 0; i < rm->count; i++) {
            if (rm->mapperCb[i].write != NULL) {
                rm->mapperCb[i].write(rm->mapperCb[i].ref, ioPort, value);
            }
        }
    }
}

static void getDebugInfo(RamMapperIo* rm, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;
    int i;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevRamMapper(), 4);
    for (i = 0; i < 4; i++) {
        dbgIoPortsAddPort(ioPorts, i, 0xfc + i, DBG_IO_READWRITE, read(rm, 0xfc + i));
    }
}

int ramMapperIoCreate() 
{
    RamMapperIo* rm;
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };

    rm = malloc(sizeof(RamMapperIo));
    rm->count = 0;
    rm->mask  = 0;
    rm->handleCount = 0;
    
    rm->port[0] = 3;
    rm->port[1] = 2;
    rm->port[2] = 1;
    rm->port[3] = 0;

    rm->deviceHandle = deviceManagerRegister(RAM_MAPPER, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_BIOS, langDbgDevRamMapper(), &dbgCallbacks, rm);

    ioPortRegister(0xfc, read, write, rm);
    ioPortRegister(0xfd, read, write, rm);
    ioPortRegister(0xfe, read, write, rm);
    ioPortRegister(0xff, read, write, rm);

    mapperIo = rm;

    return 1;
}

int ramMapperIoGetPortValue(int ioPort)
{
    RamMapperIo* rm = mapperIo;
    if (rm == NULL) {
        return 0xff;
    }

    return rm->port[ioPort & 3];
}

int ramMapperIoAdd(int size, MemIoWrite write, void* ref)
{
    RamMapperIo* rm = mapperIo;
    RamMapperCb mapperCb = { ++rm->handleCount, write, ref, size };

    if (rm == NULL || rm->count == 32) {
        return 0;
    }

    rm->mapperCb[rm->count++] = mapperCb;

    rm->mask = ramMapperIoGetMask(rm);

    return rm->handleCount;
}

void ramMapperIoRemove(int handle)
{
    RamMapperIo* rm = mapperIo;
    int i;

    if (rm == NULL || rm->count == 0) {
        return;
    }

    for (i = 0; i < rm->count; i++) {
        if (rm->mapperCb[i].handle == handle) {
            break;
        }
    }

    if (i == rm->count) {
        return;
    }

    rm->count--;
    while (i < rm->count) {
        rm->mapperCb[i] = rm->mapperCb[i + 1];
        i++;
    }

    rm->mask = ramMapperIoGetMask(rm);
}

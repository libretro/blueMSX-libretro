/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperBasic.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/02/13 21:20:00 $
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
#include "romMapperBasic.h"
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
} RomMapperBasic;

static void destroy(void* arg)
{
    RomMapperBasic* rm = (RomMapperBasic*)arg;

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}
int romMapperBasicCreate(char* filename, UInt8* romData, 
                         int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperBasic* rm;

    if (size > 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperBasic));

    rm->romData = malloc(0x8000);
    memset(rm->romData, 0xff, 0x8000);
    memcpy(rm->romData, romData, size);

    rm->size = size;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->deviceHandle = deviceManagerRegister(ROM_BASIC, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, NULL, destroy, rm);

    slotMapPage(slot, sslot, startPage + 0, rm->romData + 0x0000, 1, 0);
    slotMapPage(slot, sslot, startPage + 1, rm->romData + 0x2000, 1, 0);
    slotMapPage(slot, sslot, startPage + 2, rm->romData + 0x4000, 1, 0);
    slotMapPage(slot, sslot, startPage + 3, rm->romData + 0x6000, 1, 0);

    return 1;
}

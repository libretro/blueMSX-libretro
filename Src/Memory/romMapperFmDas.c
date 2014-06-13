/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperFmDas.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2006/02/18 17:21:05 $
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
#include "romMapperFmDas.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include <stdlib.h>
#include <string.h>


typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
} RomMapperFmDas;

static void destroy(RomMapperFmDas* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

int romMapperFmDasCreate(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperFmDas* rm;

    if (size != 0x8000 || startPage != 0) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperFmDas));

    rm->deviceHandle = deviceManagerRegister(ROM_FMDAS, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 8, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(0x8000);
    memcpy(rm->romData, romData, 0x8000);
    memset(rm->romData + 0x2000, 0xff, 0x2000);

    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    slotMapPage(slot, sslot, 0, rm->romData + 0x0000, 1, 0);
    slotMapPage(slot, sslot, 1, rm->romData + 0x2000, 1, 1);
    slotMapPage(slot, sslot, 2, rm->romData + 0x4000, 1, 0);
    slotMapPage(slot, sslot, 3, rm->romData + 0x6000, 1, 0);
    slotMapPage(slot, sslot, 4, rm->romData + 0x0000, 1, 0);
    slotMapPage(slot, sslot, 5, rm->romData + 0x2000, 1, 1);
    slotMapPage(slot, sslot, 6, rm->romData + 0x4000, 1, 0);
    slotMapPage(slot, sslot, 7, rm->romData + 0x6000, 1, 0);

    return 1;
}


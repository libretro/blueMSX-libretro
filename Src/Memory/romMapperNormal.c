/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperNormal.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2005/05/13 19:57:17 $
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
#include "romMapperNormal.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "ArchPrinter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
} RomMapperNormal;

static void destroy(RomMapperNormal* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

int romMapperNormalCreate(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperNormal* rm;
    int pages = size / 0x2000 + ((size & 0x1fff) ? 1 : 0);
    int i;

    if (pages == 0 || (startPage + pages) > 8) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperNormal));

    rm->deviceHandle = deviceManagerRegister(ROM_NORMAL, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(pages * 0x2000);
    memcpy(rm->romData, romData, size);

    if (slot == 0 && sslot == 0 && startPage == 0) {
        archPrinterSetMsxFont(romData, size);
    }

    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, rm->romData + 0x2000 * i, 1, 0);
    }

    return 1;
}


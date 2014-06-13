/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperCasette.c,v $
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
#include "romMapperCasette.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "Board.h"
#include "ArchPrinter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static UInt16 patchAddress[] = { 0x00e1, 0x00e4, 0x00e7, 0x00ea, 0x00ed, 0x00f0, 0x00f3, 0 };
static UInt16 patchAddressSVI[] = {0x006C,0x006F,0x0072,0x0075,0x0078,0x210A,0x21A9,0}; // 0x0069

typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
} RomMapperCasette;

static void destroy(RomMapperCasette* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

int romMapperCasetteCreate(char* filename, UInt8* romData, 
                        int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperCasette* rm;
    int pages = size / 0x2000;
    int i;

    if (pages == 0 || (startPage + pages) > 8) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperCasette));

    rm->deviceHandle = deviceManagerRegister(ROM_CASPATCH, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, NULL, NULL, NULL, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    if (slot == 0 && sslot == 0 && startPage == 0) {
        archPrinterSetMsxFont(romData, size);
    }

    if (boardGetType() == BOARD_SVI) {
        // Patch the SVI-328 BIOS and BASIC for cassette handling
        for (i = 0; patchAddressSVI[i]; i++) {
            UInt8* ptr = rm->romData + patchAddressSVI[i];
            ptr[0] = 0xed;
            ptr[1] = 0xfe;
            ptr[2] = 0xc9;
        }
        rm->romData[0x2073]=0x01;   // Skip delay loop after save
        rm->romData[0x20D0]=0x10;   // Write $55 only $10 times, instead
        rm->romData[0x20D1]=0x00;   //   of $190
        rm->romData[0x20E3]=0x00;   // Cancel instruction
        rm->romData[0x20E4]=0x00;
        rm->romData[0x20E5]=0x00;
        rm->romData[0x20E6]=0xED;
        rm->romData[0x20E7]=0xFE;
    }
    else {
        // Patch the casette rom
        for (i = 0; patchAddress[i]; i++) {
            UInt8* ptr = rm->romData + patchAddress[i];
            ptr[0] = 0xed;
            ptr[1] = 0xfe;
            ptr[2] = 0xc9;
        }
    }

    for (i = 0; i < pages; i++) {
        slotMapPage(slot, sslot, i + startPage, rm->romData + 0x2000 * i, 1, 0);
    }

    return 1;
}

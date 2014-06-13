/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperPlain.c,v $
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
#include "romMapperPlain.h"
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
} RomMapperPlain;

static void destroy(void* arg)
{
    RomMapperPlain* rm = (RomMapperPlain*)arg;

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}



static UInt16 getRomStart(UInt8* romData, int size) 
{
    int pages[3] = { 0, 0, 0 };
    int startPage;
    int i;

    for (startPage = 0; startPage < 2; startPage++) {
        UInt8* romPtr = romData + 0x4000 * startPage;

	    if (romPtr[0] == 'A' && romPtr[1] =='B') {
		    for (i = 0; i < 4; i++) {
                UInt16 address = romPtr[2 * i + 2] + 256 * (UInt16)romPtr[2 * i + 3];

			    if (address > 0) {
				    UInt16 page = address / 0x4000 - startPage;

                    if (page < 3) {
                        pages[page]++;
				    }
			    }
		    }
        }
	}

    if (pages[1] && (pages[1] >= pages[0]) && (pages[1] >= pages[2])) {
		return 0x4000;
	} 
    
    if (pages[0] && pages[0] >= pages[2]) {
		return 0x0000;
	} 
    
    if (pages[2]) {
		return 0x8000;
	}

    return 0x4000;
}

int romMapperPlainCreate(char* filename, UInt8* romData, 
                         int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, NULL, NULL };
    RomMapperPlain* rm;
    int romMapper[8];
    int i;

    if (size > 0x10000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperPlain));

    rm->romData = malloc(0x10000);
    memset(rm->romData, 0xff, 0x10000);
    memcpy(rm->romData, romData, size);

    // Align ROM size up to next valid rom size
    if      (size <= 0x2000)  size = 0x2000;
    else if (size <= 0x4000)  size = 0x4000;
    else if (size <= 0x8000)  size = 0x8000;
    else if (size <= 0xc000)  size = 0xc000;
    else if (size <= 0x10000) size = 0x10000;

    rm->size = size;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    switch (size) {
    case 0x2000:
        for (i = 0; i < 8; i++) {
            romMapper[i] = 0;
        }
        break;

    case 0x4000:
        for (i = 0; i < 8; i++) {
            romMapper[i] = i & 1;
        }
        break;
        
    case 0x8000:
        if (getRomStart(romData, size) == 0x4000) {
            for (i = 0; i < 4; i++) {
                romMapper[i] = i & 1;
                romMapper[i + 4] = 2 + (i & 1);
            }
        }
        else {
            for (i = 0; i < 8; i++) {
                romMapper[i] = i & 3;
            }
        }
        break;
        
    case 0xc000:
        if (getRomStart(romData, size) == 0x4000) {
            romMapper[0] = 0;
            romMapper[1] = 1;

            for (i = 0; i < 6; i++) {
                romMapper[i + 2] = i;
            }
        }
        else {
            for (i = 0; i < 6; i++) {
                romMapper[i] = i;
            }
            
            romMapper[6] = 0;
            romMapper[7] = 1;
        }
        break;

    case 0x10000:
        for (i = 0; i < 8; i++) {
            romMapper[i] = i;
        }
        break;
        
    default:
        free(rm);
        return 0;
    }

    rm->deviceHandle = deviceManagerRegister(ROM_PLAIN, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 8, NULL, NULL, NULL, destroy, rm);

    for (i = 0; i < 8; i++) {
        slotMapPage(slot, sslot, startPage + i, rm->romData + 0x2000 * romMapper[i], 1, 0);
    }

    return 1;
}


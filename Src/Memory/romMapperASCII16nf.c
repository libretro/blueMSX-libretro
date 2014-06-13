/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperASCII16nf.c,v $
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
#include "romMapperASCII16nf.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//  Uses ASCII16 but in different adresses :
//  Know Game : Super Pierrot


typedef struct {
    int deviceHandle;
    UInt8* romData;
    int slot;
    int sslot;
    int startPage;
    UInt32 romMask;
    int romMapper[4];
} RomMapperASCII16nf;

static void saveState(RomMapperASCII16nf* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperASCII16nf");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateClose(state);
}

static void loadState(RomMapperASCII16nf* rm)
{
    SaveState* state = saveStateOpenForRead("mapperASCII16nf");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }

    saveStateClose(state);

    for (i = 0; i < 4; i += 2) {
        UInt8* bankData = rm->romData + (rm->romMapper[i] << 14);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     bankData, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, bankData + 0x2000, 1, 0);
    }
}

static void destroy(RomMapperASCII16nf* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperASCII16nf* rm, UInt16 address, UInt8 value) 
{
    int bank;

    address += 0x4000;

    if (address & 0x0800) {
        return;
    }

    bank = (address & 0x1000) >> 11;

    value &= rm->romMask;

    if (rm->romMapper[bank] != value) {
        UInt8* bankData = rm->romData + ((int)value << 14);
        
        rm->romMapper[bank] = value;

        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank,     bankData, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank + 1, bankData + 0x2000, 1, 0);
    }
}

int romMapperASCII16nfCreate(char* filename, UInt8* romData, 
                             int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperASCII16nf* rm;
    int i;

    rm = malloc(sizeof(RomMapperASCII16nf));

    rm->deviceHandle = deviceManagerRegister(ROM_ASCII16NF, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    size = (size + 0x3fff) & ~0x3fff;

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    rm->romMask = size / 0x4000 - 1;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    rm->romMapper[0] = 0;
    rm->romMapper[2] = 0;

    for (i = 0; i < 4; i += 2) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i,     rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i + 1, rm->romData + rm->romMapper[i] * 0x2000 + 0x2000, 1, 0);
    }

    return 1;
}


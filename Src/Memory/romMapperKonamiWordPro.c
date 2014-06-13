/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperKonamiWordPro.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/12/28 23:39:02 $
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
#include "romMapperKonamiWordPro.h"
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
    UInt8 control;
} RomMapperKonamiWordPro;

static void saveState(RomMapperKonamiWordPro* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperKonamiWordPro");
    saveStateClose(state);
}

static void loadState(RomMapperKonamiWordPro* rm)
{
    SaveState* state = saveStateOpenForRead("mapperKonamiWordPro");
    saveStateClose(state);
}

static void destroy(RomMapperKonamiWordPro* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static UInt8 read(RomMapperKonamiWordPro* rm, UInt16 address)
{
#if 0
    if (address < getPC() - 2 || address > getPC() + 4) {
        printf("R %.4x :\n", address);
    }
    if (rm->control == 0xdb && address < 0x8000) {
        return 0x00;
    }
#endif
    if (address < 0x4000 || address >= 0xc000) {
        return 0xc0;
    }

    return rm->romData[address - 0x4000];
}

static void write(RomMapperKonamiWordPro* rm, UInt16 address, UInt8 value) 
{
#if 0
    printf("W %.4x : 0x%.2x\n", address, value);
#endif
    if (address = 0xbfff) {
        rm->control = value;
    }
}

static void reset(RomMapperKonamiWordPro* rm)
{
    rm->control = 0xff;
}

int romMapperKonamiWordProCreate(char* filename, UInt8* romData, 
                          int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperKonamiWordPro* rm;
    int pages = 8;
    int i;

    if (size != 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperKonamiWordPro));

    if (pages == 8)
        startPage = 0;

    rm->deviceHandle = deviceManagerRegister(ROM_KONWORDPRO, &callbacks, rm);
    slotRegister(slot, sslot, startPage, pages, read, read, write, destroy, rm);

    rm->romData = calloc(1, size);
    memcpy(rm->romData, romData, size);
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    for (i = 0; i < pages; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + i * 0x2000, 0, 0);
    }

    reset(rm);
    return 1;
}


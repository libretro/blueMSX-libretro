/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperKoei.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/02/13 21:20:00 $
**
** More info: http://www.bluemsx.com
**
** Notes:       The Koei roms are similar to the ASCII8 roms but they
**              can have 8kB, 32kB or no SRAM (see the list below).
**              Also writing to SRAM is possible on addresses
**              0x4000 - 0x5fff.
**
**              This implementation does not distinguish between the
**              different SRAM sizes. The Koei way of selecting SRAM
**              bank works the same way in all ROMS and even though
**              this mapper always has 32kB SRAM available, ROMs that
**              has less SRAM will work fine with this mapper (they
**              simply don't use the extra SRAM).
**
**              Daikoukai Jidai (1990)                       8kB SRAM
**              Europe War (1992)                           32kB SRAM
**              Genchohisi (1992)                           32kB SRAM
**              Genghis Khan (1988)                          (?) SRAM
**              Inindo Tado Nobunaga (1991)                  8kB SRAM
**              Isin No Arashi (1989)                        8kB SRAM
**              Lempereur (1990)                             8kB SRAM
**              Nobunaga No Yabou - Bushouhuunroku (1991)   32kB SRAM
**              Nobunaga No Yabou - Senkokugunyuden (1989)   8kB SRAM
**              Nobunaga No Yabou - Zenkokuhan (1987)        8kB SRAM
**              Royal Blood (1991)                           8kB SRAM
**              Sangokusi 1 (1988)                          32kB SRAM
**              Sangokusi 2 (1991)                          32kB SRAM
**              Suikoden (1989)                              8kB SRAM
**              Teitoku No Ketsudan (1991)                  32kB SRAM
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
#include "romMapperKoei.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "sramLoader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define SRAM_PAGES 4

typedef struct {
    int deviceHandle;
    UInt8* romData;
    UInt8 sram[SRAM_PAGES << 13];
    char sramFilename[512];
    int slot;
    int sslot;
    int startPage;
    int sramEnabled;
    UInt32 romMask;
    int romMapper[4];
} RomMapperKoei;

static void saveState(RomMapperKoei* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperKoei");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }
    
    saveStateSet(state, "sramEnabled", rm->sramEnabled);

    saveStateClose(state);
}

static void loadState(RomMapperKoei* rm)
{
    SaveState* state = saveStateOpenForRead("mapperKoei");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }
    
    rm->sramEnabled = saveStateGet(state, "sramEnabled", 0);

    saveStateClose(state);

    for (i = 0; i < 4; i++) {   
        if (rm->sramEnabled & (1 << i)) {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->sram + (rm->romMapper[i] & (SRAM_PAGES - 1)) * 0x2000, 1, 0);
        }
        else {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
        }
    }
}

static void destroy(RomMapperKoei* rm)
{
    sramSave(rm->sramFilename, rm->sram, SRAM_PAGES << 13, NULL, 0);

    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);

    free(rm->romData);
    free(rm);
}

static void write(RomMapperKoei* rm, UInt16 address, UInt8 value) 
{
    address += 0x4000;

    if (address >= 0x6000 && address < 0x8000) {
        int bank = (address & 0x1800) >> 11;
        int    writeCache = 0;
        UInt8* bankData;

        if (value & ~rm->romMask) {
            bankData = rm->sram + ((int)(value & (SRAM_PAGES - 1)) << 13);
            writeCache = bank != 1;
            rm->sramEnabled |= (1 << bank);
        }
        else {
            bankData = rm->romData + ((int)value << 13);
            rm->sramEnabled &= ~(1 << bank);
        }

        rm->romMapper[bank] = value;
        slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, bankData, 1, writeCache);
    }
}

int romMapperKoeiCreate(char* filename, UInt8* romData, 
                        int size, int slot, int sslot, int startPage) 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperKoei* rm;
    int i;

    if (size < 0x8000) {
        return 0;
    }

    rm = malloc(sizeof(RomMapperKoei));

    rm->deviceHandle = deviceManagerRegister(ROM_KOEI, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, NULL, NULL, write, destroy, rm);

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    memset(rm->sram, 0, SRAM_PAGES << 13);
    rm->romMask = size / 8192 - 1;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;
    rm->sramEnabled = 0;
    strcpy(rm->sramFilename, sramCreateFilename(filename));

    sramLoad(rm->sramFilename, rm->sram, SRAM_PAGES << 13, NULL, 0);

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 0;
    rm->romMapper[2] = 0;
    rm->romMapper[3] = 0;

    for (i = 0; i < 4; i++) {   
        slotMapPage(rm->slot, rm->sslot, rm->startPage + i, rm->romData + rm->romMapper[i] * 0x2000, 1, 0);
    }

    return 1;
}

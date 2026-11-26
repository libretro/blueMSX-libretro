/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Memory/AmdFlash.c,v $
**
** $Revision: 1.15 $
**
** $Date: 2008-03-30 18:38:42 $
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
#include "AmdFlash.h"
#include "SaveState.h"
#include "sramLoader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Minimal AMD flash emulation to support the obsonet flash

typedef struct {
    UInt32 address;
    UInt8  value;
} AmdCmd;

#define ST_IDLE     0
#define ST_IDENT    1

struct AmdFlash
{
    UInt8* romData;
    UInt32 cmdAddr1;
    UInt32 cmdAddr2;
    int    state;
    int    flashSize;
    int    sectorSize;
    AmdCmd cmd[8];
    int    cmdIdx;
    int    writeProtectMask;
    UInt32 cmdMask;
    UInt8  manufacturerId;
    UInt8  deviceId;
    int    dirty;
    char   sramFilename[512];
};

static int isSectorProtected(AmdFlash* rm, UInt32 address)
{
    UInt32 sector = address / rm->sectorSize;
    return sector < 32 && ((rm->writeProtectMask >> sector) & 1);
}

static int checkCommandEraseSector(AmdFlash* rm) 
{
    if (rm->cmdIdx > 0 && ((rm->cmd[0].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[0].value != 0xaa)) return 0;
    if (rm->cmdIdx > 1 && ((rm->cmd[1].address & rm->cmdMask) != rm->cmdAddr2 || rm->cmd[1].value != 0x55)) return 0;
    if (rm->cmdIdx > 2 && ((rm->cmd[2].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[2].value != 0x80)) return 0;
    if (rm->cmdIdx > 3 && ((rm->cmd[3].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[3].value != 0xaa)) return 0;
    if (rm->cmdIdx > 4 && ((rm->cmd[4].address & rm->cmdMask) != rm->cmdAddr2 || rm->cmd[4].value != 0x55)) return 0;
    if (rm->cmdIdx > 5 && (                                                rm->cmd[5].value != 0x30)) return 0;

    if (rm->cmdIdx < 6) return 1;

    if (!isSectorProtected(rm, rm->cmd[5].address)) {
        memset(rm->romData + (rm->cmd[5].address & ~(rm->sectorSize - 1) & (rm->flashSize - 1)), 0xff, rm->sectorSize);
        rm->dirty = 1;
    }
    return 0;
}

static int checkCommandEraseChip(AmdFlash* rm) 
{
    if (rm->cmdIdx > 0 && ((rm->cmd[0].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[0].value != 0xaa)) return 0;
    if (rm->cmdIdx > 1 && ((rm->cmd[1].address & rm->cmdMask) != rm->cmdAddr2 || rm->cmd[1].value != 0x55)) return 0;
    if (rm->cmdIdx > 2 && ((rm->cmd[2].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[2].value != 0x80)) return 0;
    if (rm->cmdIdx > 3 && ((rm->cmd[3].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[3].value != 0xaa)) return 0;
    if (rm->cmdIdx > 4 && ((rm->cmd[4].address & rm->cmdMask) != rm->cmdAddr2 || rm->cmd[4].value != 0x55)) return 0;
    if (rm->cmdIdx > 5 && (                                         rm->cmd[5].value != 0x10)) return 0;

    if (rm->cmdIdx < 6) return 1;

    memset(rm->romData, 0xff, rm->flashSize);
    rm->dirty = 1;
    return 0;
}

static int checkCommandProgram(AmdFlash* rm) 
{
    if (rm->cmdIdx > 0 && ((rm->cmd[0].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[0].value != 0xaa)) return 0;
    if (rm->cmdIdx > 1 && ((rm->cmd[1].address & rm->cmdMask) != rm->cmdAddr2 || rm->cmd[1].value != 0x55)) return 0;
    if (rm->cmdIdx > 2 && ((rm->cmd[2].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[2].value != 0xa0)) return 0;

    if (rm->cmdIdx < 4) return 1;

    if (!isSectorProtected(rm, rm->cmd[3].address)) {
        rm->romData[rm->cmd[3].address & (rm->flashSize - 1)] &= rm->cmd[3].value;
        rm->dirty = 1;
    }
    return 0;
}

static int checkCommandManifacturer(AmdFlash* rm) 
{
    if (rm->cmdIdx > 0 && ((rm->cmd[0].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[0].value != 0xaa)) return 0;
    if (rm->cmdIdx > 1 && ((rm->cmd[1].address & rm->cmdMask) != rm->cmdAddr2 || rm->cmd[1].value != 0x55)) return 0;
    if (rm->cmdIdx > 2 && ((rm->cmd[2].address & rm->cmdMask) != rm->cmdAddr1 || rm->cmd[2].value != 0x90)) return 0;

    if (rm->cmdIdx == 3)
        rm->state = ST_IDENT;
    if (rm->cmdIdx < 4) return 1;

    return 0;
}

UInt8 amdFlashRead(AmdFlash* rm, UInt32 address)
{
    if (rm->state == ST_IDENT) {
        rm->cmdIdx = 0;
        switch (address & 0x03) {
        case 0: 
            return rm->manufacturerId;
        case 1: 
            return rm->deviceId;
        case 2:
            return isSectorProtected(rm, address);
        case 3:
            return 0x01;
        }
        return 0xff;
    }

    address &= rm->flashSize - 1;

    return rm->romData[address];
}

void amdFlashWrite(AmdFlash* rm, UInt32 address, UInt8 value)
{
    if (rm->cmdIdx < sizeof(rm->cmd) / sizeof(rm->cmd[0])) {
        int stateValid = 0;

        rm->cmd[rm->cmdIdx].address = address;
        rm->cmd[rm->cmdIdx].value   = value;
        rm->cmdIdx++;
        stateValid |= checkCommandManifacturer(rm);
        stateValid |= checkCommandEraseSector(rm);
        stateValid |= checkCommandProgram(rm);
        stateValid |= checkCommandEraseChip(rm);
        if (stateValid) {
            if (value == 0xf0) {
                rm->state = ST_IDLE;
                rm->cmdIdx = 0;
            }
        }

        if (!stateValid) {
            rm->state = ST_IDLE;
            rm->cmdIdx = 0;
        }
    }
}

UInt8* amdFlashGetPage(AmdFlash* rm, UInt32 address)
{
    address &= rm->flashSize - 1;
    return rm->romData + address;
}

int amdFlashCmdInProgress(AmdFlash* rm)
{
    return rm->cmdIdx != 0;
}

void amdFlashReset(AmdFlash* rm)
{
    rm->cmdIdx = 0;
    rm->state = ST_IDLE;
}

void amdFlashSaveState(AmdFlash* rm)
{
    SaveState* state = saveStateOpenForWrite("amdFlash");
    int i;

    for (i = 0; i < 8; i++) {
        char buf[32];
        sprintf(buf, "cmd_%d_address", i);
        saveStateSet(state, buf,   rm->cmd[i].address);
        sprintf(buf, "cmd_%d_value", i);
        saveStateSet(state, buf,   rm->cmd[i].value);
    }

    saveStateSet(state, "cmdIdx",   rm->cmdIdx);

    saveStateClose(state);
}

void amdFlashLoadState(AmdFlash* rm)
{
    SaveState* state = saveStateOpenForRead("amdFlash");
    int i;

    for (i = 0; i < 8; i++) {
        char buf[32];
        sprintf(buf, "cmd_%d_address", i);
        rm->cmd[i].address = saveStateGet(state, buf,   0);
        sprintf(buf, "cmd_%d_value", i);
        rm->cmd[i].value = (UInt8)saveStateGet(state, buf,   0);
    }

    rm->cmdIdx = saveStateGet(state, "cmdIdx", 0);

    saveStateClose(state);
}

AmdFlash* amdFlashCreate(AmdType type, int flashSize, int sectorSize, UInt32 writeProtectMask, void* romData, int size, const char* sramFilename, int loadSram)
{
    AmdFlash* rm = (AmdFlash*)calloc(1, sizeof(AmdFlash));

    rm->writeProtectMask = writeProtectMask;

    rm->cmdMask        = 0x7ff;
    rm->manufacturerId = 0x01;
    rm->deviceId       = 0xa4;

    if (type == AMD_TYPE_1) {
        rm->cmdAddr1 = 0xaaa;
        rm->cmdAddr2 = 0x555;
    }
    else if (type == AMD_TYPE_M29W640GB) {
        rm->cmdAddr1       = 0xaaa;
        rm->cmdAddr2       = 0x555;
        rm->cmdMask        = 0xfff;
        rm->manufacturerId = 0x20;
        rm->deviceId       = 0x7e;
    }
    else {
        rm->cmdAddr1 = 0x555;
        rm->cmdAddr2 = 0x2aa;
    }

    if (sramFilename != NULL) {
        strcpy(rm->sramFilename, sramFilename);
    }

    rm->flashSize = flashSize;
    rm->sectorSize = sectorSize;

    rm->romData = (UInt8*)malloc(flashSize);
    if (size >= flashSize)
        size = flashSize;

    if (rm->sramFilename[0]) {
        memset(rm->romData + size, 0xff, flashSize - size);
        sramLoad(rm->sramFilename, rm->romData, rm->flashSize, NULL, 0);
    }

    if (size > 0)
        memcpy(rm->romData, romData, size);
#if 0
    if (rm->sramFilename[0] && loadSram) {
        sramLoad(rm->sramFilename, rm->romData, rm->flashSize, NULL, 0);
    }
#endif

    return rm;
}

void amdFlashDestroy(AmdFlash* rm)
{
    /* Only write the backing file when the flash was actually programmed or
    ** erased: large chips (8MB on the Konami Ultimate Collection) would
    ** otherwise be dumped to disk on every cartridge removal. */
    if (rm->sramFilename[0] && rm->dirty)
        sramSave(rm->sramFilename, rm->romData, rm->flashSize, NULL, 0);
    free(rm->romData);
    free(rm);
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSCCplus.c,v $
**
** $Revision: 1.5 $
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
#include "romMapperSCCplus.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SCC.h"
#include "Board.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt8 romData[0x22000];
    int slot;
    int sslot;
    int startPage;
    UInt8 modeRegister;
    UInt8 mapperMask;
    int isMapped[4];
    int isRamSegment[4];
    int romMapper[4];
    SccType sccType;
    SccMode sccMode;
    SCC* scc;
} RomMapperSCCplus;

static void saveState(RomMapperSCCplus* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperSCCplus");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
        
        sprintf(tag, "isRamSegment%d", i);
        saveStateSet(state, tag, rm->isRamSegment[i]);
        
        sprintf(tag, "isMapped%d", i);
        saveStateSet(state, tag,     rm->isMapped[i]);
    }
    
    saveStateSet(state, "modeRegister", rm->modeRegister);
    saveStateSet(state, "sccMode",      rm->sccMode);

    saveStateSetBuffer(state, "romData", rm->romData, sizeof(rm->romData));

    saveStateClose(state);

    sccSaveState(rm->scc);
}

static void loadState(RomMapperSCCplus* rm)
{
    SaveState* state = saveStateOpenForRead("mapperSCCplus");
    char tag[16];
    int bank;
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);

        sprintf(tag, "isRamSegment%d", i);
        rm->isRamSegment[i] = saveStateGet(state, tag, 0);
        
        sprintf(tag, "isMapped%d", i);
        rm->isMapped[i] = saveStateGet(state, tag, 0);
    }
    
    rm->modeRegister = (UInt8)saveStateGet(state, "modeRegister", 0);
    rm->sccMode      =        saveStateGet(state, "sccMode", 0);

    saveStateGetBuffer(state, "romData", rm->romData, sizeof(rm->romData));

    saveStateClose(state);

    sccLoadState(rm->scc);

    for (bank = 0; bank < 4; bank++) {   
        if (rm->isMapped[bank]) {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->romData + 0x2000 * rm->romMapper[bank], 1, 0);
        }
        else {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->romData + 0x20000, 1, 0);
        }
    }
    
    if (rm->sccMode == SCC_PLUS) {
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 2, NULL, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 3, NULL, 0, 0);
    }
    else if (rm->sccMode = SCC_COMPATIBLE) {
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 2, NULL, 0, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 3, NULL, 1, 0);
    }
    else {
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 2, NULL, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 3, NULL, 1, 0);
    }
}

static void destroy(RomMapperSCCplus* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    sccDestroy(rm->scc);

    free(rm);
}

static void reset(RomMapperSCCplus* rm)
{
    sccReset(rm->scc);
}

static UInt8 read(RomMapperSCCplus* rm, UInt16 address) 
{
    int bank;
    
    address += 0x4000;

    if (rm->sccMode == SCC_COMPATIBLE && address >= 0x9800 && address < 0xa000) {
        return sccRead(rm->scc, (UInt8)(address & 0xff));
    }
    if (rm->sccMode == SCC_PLUS && address >= 0xb800 && address < 0xc000) {
        return sccRead(rm->scc, (UInt8)(address & 0xff));
    }

    bank = (address - 0x4000) >> 13;

    if (rm->isMapped[bank]) {
    	return rm->romData[0x2000 * (rm->romMapper[bank] & rm->mapperMask) + (address & 0x1fff)];
    }

    return 0xff;
}

static UInt8 peek(RomMapperSCCplus* rm, UInt16 address) 
{
    int bank;
    
    address += 0x4000;

    if (rm->sccMode == SCC_COMPATIBLE && address >= 0x9800 && address < 0xa000) {
        return sccPeek(rm->scc, (UInt8)(address & 0xff));
    }
    if (rm->sccMode == SCC_PLUS && address >= 0xb800 && address < 0xc000) {
        return sccPeek(rm->scc, (UInt8)(address & 0xff));
    }

    bank = (address - 0x4000) >> 13;

    if (rm->isMapped[bank]) {
    	return rm->romData[0x2000 * (rm->romMapper[bank] & rm->mapperMask) + (address & 0x1fff)];
    }

    return 0xff;
}

static void updateEnable(RomMapperSCCplus* rm)
{
    if ((rm->modeRegister & 0x20) && (rm->romMapper[3] & 0x80)) {
        slotUpdatePage(rm->slot, rm->sslot, rm->startPage + 2, NULL, 1, 0);
        slotUpdatePage(rm->slot, rm->sslot, rm->startPage + 3, NULL, 0, 0);
        sccSetMode(rm->scc, SCC_PLUS);
        rm->sccMode = SCC_PLUS;
    }
    else if (!(rm->modeRegister & 0x20) && (rm->romMapper[2] & 0x3f) == 0x3f) {
        slotUpdatePage(rm->slot, rm->sslot, rm->startPage + 2, NULL, 0, 0);
        slotUpdatePage(rm->slot, rm->sslot, rm->startPage + 3, NULL, 1, 0);
        sccSetMode(rm->scc, SCC_COMPATIBLE);
        rm->sccMode = SCC_COMPATIBLE;
    }
    else {
        slotUpdatePage(rm->slot, rm->sslot, rm->startPage + 2, NULL, 1, 0);
        slotUpdatePage(rm->slot, rm->sslot, rm->startPage + 3, NULL, 1, 0);
        rm->sccMode = SCC_NONE;
    }
}

static void write(RomMapperSCCplus* rm, UInt16 address, UInt8 value) 
{
    int bank;

    address += 0x4000;

    if (address < 0x4000 && address >= 0xc000) {       
        return;
    }

    if ((address | 1) == 0xbfff) {
        rm->modeRegister = value;
        rm->isRamSegment[0] = (value & 0x10) | (value & 0x01);
        rm->isRamSegment[1] = (value & 0x10) | (value & 0x02);
        rm->isRamSegment[2] = (value & 0x10) | ((value & 0x24) == 0x24);
		rm->isRamSegment[3] = (value & 0x10);
        
        updateEnable(rm);

        return;
    }

    bank = (address - 0x4000) >> 13;

    if (rm->isRamSegment[bank]) {
        if (rm->isMapped[bank]) {
        	rm->romData[0x2000 * (rm->romMapper[bank] & rm->mapperMask) + (address & 0x1fff)] = value;
        }
        return;
    }

    if ((address & 0x1800) == 0x1000) {
        rm->romMapper[bank] = value;
        value &= rm->mapperMask;
        rm->isMapped[bank]  = (value >= 8 && rm->sccType != SCC_SNATCHER) || 
                                (value < 8  && rm->sccType != SCC_SDSNATCHER);

        if (rm->isMapped[bank]) {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->romData + 0x2000 * value, 1, 0);
        }
        else {
            slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, rm->romData + 0x20000, 1, 0);
        }

        updateEnable(rm);

        return;
    }

    if (rm->sccMode == SCC_COMPATIBLE && address >= 0x9800 && address < 0xa000) {
        sccWrite(rm->scc, address & 0xff, value);
    }
    else if (rm->sccMode == SCC_PLUS && address >= 0xb800 && address < 0xc000) {
        sccWrite(rm->scc, address & 0xff, value);
    }
}

int romMapperSCCplusCreate(char* filename, UInt8* romData, 
                           int size, int slot, int sslot, int startPage, SccType sccType) 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperSCCplus* rm;

    rm = malloc(sizeof(RomMapperSCCplus));

    rm->deviceHandle = deviceManagerRegister(ROM_SCCEXTENDED, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, read, peek, write, destroy, rm);

    memset(rm->romData, 0xff, 0x22000);

    if (romData) {
        if (size > 0x20000) {
            size = 0x20000;
        }
        memcpy(rm->romData, romData, size);
    }

    rm->slot            = slot;
    rm->sslot           = sslot;
    rm->startPage       = startPage;
    rm->modeRegister    = sccType == SCCP_EXTENDED ? 0x20 : 0;
    rm->isRamSegment[0] = 0;
    rm->isRamSegment[1] = 0;
    rm->isRamSegment[2] = 0;
    rm->isRamSegment[3] = 0;
    rm->isMapped[0]     = sccType != SCC_SDSNATCHER;
    rm->isMapped[1]     = sccType != SCC_SDSNATCHER;
    rm->isMapped[2]     = sccType != SCC_SDSNATCHER;
    rm->isMapped[3]     = sccType != SCC_SDSNATCHER;
    rm->mapperMask      = sccType == SCC_MIRRORED ? 0x07 : 0x0f;
    rm->scc             = sccCreate(boardGetMixer());
    rm->sccType         = sccType;
    rm->sccMode         = SCC_NONE;

    rm->romMapper[0] = 0;
    rm->romMapper[1] = 1;
    rm->romMapper[2] = 2;
    rm->romMapper[3] = 3;

    if (sccType != SCC_SDSNATCHER) {
        slotMapPage(rm->slot, rm->sslot, rm->startPage,     rm->romData + 0x0000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, rm->romData + 0x2000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 2, rm->romData + 0x4000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 3, rm->romData + 0x6000, 1, 0);
    }
    else {
        slotMapPage(rm->slot, rm->sslot, rm->startPage,     rm->romData + 0x20000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 1, rm->romData + 0x20000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 2, rm->romData + 0x20000, 1, 0);
        slotMapPage(rm->slot, rm->sslot, rm->startPage + 3, rm->romData + 0x20000, 1, 0);
    }

    return 1;
}

/*****************************************************************************
**
** Konami Ultimate Collection (Manuel Pazos) flash cartridge mapper.
**
** Ported from the openMSX implementation (src/memory/KonamiUltimateCollection.cc).
**
** More info: http://www.bluemsx.com
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
**
** Documentation as provided by Manuel Pazos, who developed the cartridge:
**
**   Konami Ultimate Collection shares some features of MegaFlashROM SCC+ SD.
**   It uses the same flashROM, SCC/SCC+, Konami and Konami SCC mappers, etc.
**
**   [OFFSET REGISTER (#7FFE)]
**   7-0: Bank offset
**
**   [MAPPER REGISTER (#7FFF)]
**   7     A21 \
**   6     A20 / FlashROM address lines to switch 2 MB banks.
**   5     Mapper mode: Select Konami mapper (0=SCC or 1=normal). [1]
**   4     Flash write enable
**   3     Disable #4000-#5FFF mapper in Konami mode, Enable DAC (works like
**         the DAC of Synthesizer or Majutsushi)
**   2     Disable mapper register
**   1     Disable mapper (bank switching)
**   0     no function anymore (was mapper limits)
**
**   [1] bit 5 only changes the address range of the mapper (Konami or Konami
**   SCC) but the SCC is always available.
**
******************************************************************************
*/
#include "romMapperKonamiUltimateCollection.h"
#include "AmdFlash.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "sramLoader.h"
#include "SCC.h"
#include "DAC.h"
#include "Board.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define KUC_FLASH_SIZE   0x800000   /* 8MB M29W640GB */
#define KUC_SECTOR_SIZE  0x10000

typedef struct {
    int deviceHandle;
    AmdFlash* flash;
    SCC* scc;
    DAC* dac;
    int slot;
    int sslot;
    int startPage;
    UInt8 mapperReg;
    UInt8 offsetReg;
    UInt8 sccMode;
    UInt8 bankRegs[4];
} RomMapperKonamiUltimateCollection;

/* Mapper register helpers */
#define IS_KONAMI_SCC_MODE(rm)      (((rm)->mapperReg & 0x20) == 0)
#define IS_FLASH_WRITE_ENABLED(rm)  (((rm)->mapperReg & 0x10) != 0)
#define IS_BANK0_DISABLED(rm)       (((rm)->mapperReg & 0x08) != 0)
#define IS_MAPPER_REG_ENABLED(rm)   (((rm)->mapperReg & 0x04) == 0)
#define ARE_BANK_REGS_ENABLED(rm)   (((rm)->mapperReg & 0x02) == 0)

/* Flash address of the start of 8kB page 'page' (0..3 = 0x4000..0xBFFF). */
static UInt32 getFlashPageAddr(RomMapperKonamiUltimateCollection* rm, int page)
{
    UInt8 bank = (UInt8)(rm->bankRegs[page] + rm->offsetReg); /* wraps at 8 bit */
    return ((UInt32)(rm->mapperReg & 0xc0) << (21 - 6)) | ((UInt32)bank << 13);
}

/* Which 8kB page (2 or 3) currently has the SCC register window, or -1. */
static int getSccPage(RomMapperKonamiUltimateCollection* rm)
{
    if (rm->sccMode & 0x10) {
        return -1;
    }
    if (rm->sccMode & 0x20) {
        return (rm->bankRegs[3] & 0x80) ? 3 : -1;
    }
    return ((rm->bankRegs[2] & 0x3f) == 0x3f) ? 2 : -1;
}

/* 'address' is the absolute CPU address. */
static int isSccAccess(RomMapperKonamiUltimateCollection* rm, UInt16 address)
{
    int sccPage = getSccPage(rm);

    if (sccPage < 0) {
        return 0;
    }

    /* Address bit 8 must be zero, this is different from a real SCC/SCC+.
    ** According to Manuel Pazos this is a leftover from an earlier version
    ** that had 2 SCCs: the SCC on the left or right channel reacted when
    ** address bit 8 was respectively 0/1. */
    if (address & 0x0100) {
        return 0;
    }

    if (sccPage == 3) {
        /* SCC+ range: 0xB800..0xBFFF, excluding 0xBFFE-0xBFFF */
        return address >= 0xb800 && address < 0xbffe;
    }
    /* SCC range: 0x9800..0x9FFF, excluding 0x9FFE-0x9FFF */
    return address >= 0x9800 && address < 0x9ffe;
}

static void updateMapping(RomMapperKonamiUltimateCollection* rm)
{
    int sccPage = getSccPage(rm);
    /* While flash writes are enabled the chip may be in a command or
    ** autoselect state, so all reads must go through amdFlashRead(). */
    int flashDirect = !IS_FLASH_WRITE_ENABLED(rm);
    int page;

    for (page = 0; page < 4; page++) {
        int readEnable = flashDirect && page != sccPage;
        slotMapPage(rm->slot, rm->sslot, rm->startPage + page,
                    amdFlashGetPage(rm->flash, getFlashPageAddr(rm, page)),
                    readEnable, 0);
    }
}

static UInt8 read(RomMapperKonamiUltimateCollection* rm, UInt16 address)
{
    address += 0x4000;

    if (isSccAccess(rm, address)) {
        return sccRead(rm->scc, (UInt8)(address & 0xff));
    }

    return amdFlashRead(rm->flash,
                        getFlashPageAddr(rm, (address - 0x4000) >> 13) | (address & 0x1fff));
}

static UInt8 peek(RomMapperKonamiUltimateCollection* rm, UInt16 address)
{
    address += 0x4000;

    if (isSccAccess(rm, address)) {
        return sccPeek(rm->scc, (UInt8)(address & 0xff));
    }

    /* Side-effect free: read the array directly instead of amdFlashRead(). */
    return *amdFlashGetPage(rm->flash,
                            getFlashPageAddr(rm, (address - 0x4000) >> 13) | (address & 0x1fff));
}

static void write(RomMapperKonamiUltimateCollection* rm, UInt16 address, UInt8 value)
{
    int page;
    UInt32 flashAddr;
    int remap = 0;

    address += 0x4000;
    page = (address - 0x4000) >> 13;
    if (page < 0 || page >= 4) {
        return;
    }

    /* There are several overlapping functional regions in the address space.
    ** A single write can trigger behaviour in multiple regions. This only
    ** goes for places where the flash is 'seen', so not for the SCC
    ** registers: a write to the SCC blocks all other functions. */
    if (isSccAccess(rm, address)) {
        sccWrite(rm->scc, (UInt8)(address & 0xff), value);
        return;
    }

    /* Address is calculated before writes to other regions take effect */
    flashAddr = getFlashPageAddr(rm, page) | (address & 0x1fff);

    /* Mapper and offset registers */
    if (IS_MAPPER_REG_ENABLED(rm)) {
        if (address == 0x7fff) {
            rm->mapperReg = value;
            remap = 1;
        }
        else if (address == 0x7ffe) {
            rm->offsetReg = value;
            remap = 1;
        }
    }

    /* DAC */
    if (IS_BANK0_DISABLED(rm) && address < 0x6000 && (address & 0x0010) == 0) {
        dacWrite(rm->dac, DAC_CH_MONO, value);
    }

    if (ARE_BANK_REGS_ENABLED(rm)) {
        /* Bank switching */
        if (IS_KONAMI_SCC_MODE(rm)) {
            /* Konami-SCC: [0x5000,0x57FF] [0x7000,0x77FF]
            **             [0x9000,0x97FF] [0xB000,0xB7FF] */
            if ((address & 0x1800) == 0x1000) {
                rm->bankRegs[page] = value;
                remap = 1;
            }
        }
        else {
            /* Konami: [0x5000,0x57FF] asymmetric!!!
            **         [0x6000,0x7FFF] [0x8000,0x9FFF] [0xA000,0xBFFF]
            ** Switching 0x4000-0x5FFF can be disabled (only Konami mode). */
            if (!(IS_BANK0_DISABLED(rm) && address < 0x6000) &&
                !(address < 0x5000 || (address >= 0x5800 && address < 0x6000))) {
                rm->bankRegs[page] = value;
                remap = 1;
            }
        }

        /* SCC mode register */
        if ((address & 0xfffe) == 0xbffe) {
            rm->sccMode = value;
            sccSetMode(rm->scc, (value & 0x20) ? SCC_PLUS : SCC_COMPATIBLE);
            remap = 1;
        }
    }

    if (remap) {
        updateMapping(rm);
    }

    if (IS_FLASH_WRITE_ENABLED(rm)) {
        amdFlashWrite(rm->flash, flashAddr, value);
    }
}

static void reset(RomMapperKonamiUltimateCollection* rm)
{
    int i;

    rm->mapperReg = 0;
    rm->offsetReg = 0;
    rm->sccMode   = 0;
    for (i = 0; i < 4; i++) {
        rm->bankRegs[i] = (UInt8)i;
    }

    amdFlashReset(rm->flash);
    sccReset(rm->scc);
    sccSetMode(rm->scc, SCC_COMPATIBLE);
    dacReset(rm->dac);

    updateMapping(rm);
}

static void saveState(RomMapperKonamiUltimateCollection* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperKonamiUltimateCollection");
    char tag[16];
    int i;

    saveStateSet(state, "mapperReg", rm->mapperReg);
    saveStateSet(state, "offsetReg", rm->offsetReg);
    saveStateSet(state, "sccMode",   rm->sccMode);
    for (i = 0; i < 4; i++) {
        sprintf(tag, "bankReg%d", i);
        saveStateSet(state, tag, rm->bankRegs[i]);
    }

    saveStateClose(state);

    sccSaveState(rm->scc);
    dacSaveState(rm->dac, "mapperKonamiUltimateCollectionDac");
    amdFlashSaveState(rm->flash);
}

static void loadState(RomMapperKonamiUltimateCollection* rm)
{
    SaveState* state = saveStateOpenForRead("mapperKonamiUltimateCollection");
    char tag[16];
    int i;

    rm->mapperReg = (UInt8)saveStateGet(state, "mapperReg", 0);
    rm->offsetReg = (UInt8)saveStateGet(state, "offsetReg", 0);
    rm->sccMode   = (UInt8)saveStateGet(state, "sccMode",   0);
    for (i = 0; i < 4; i++) {
        sprintf(tag, "bankReg%d", i);
        rm->bankRegs[i] = (UInt8)saveStateGet(state, tag, i);
    }

    saveStateClose(state);

    sccLoadState(rm->scc);
    dacLoadState(rm->dac, "mapperKonamiUltimateCollectionDac");
    amdFlashLoadState(rm->flash);

    updateMapping(rm);
}

static void destroy(RomMapperKonamiUltimateCollection* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    amdFlashDestroy(rm->flash);
    sccDestroy(rm->scc);
    dacDestroy(rm->dac);

    free(rm);
}

int romMapperKonamiUltimateCollectionCreate(const char* filename, UInt8* romData,
                                            int size, int slot, int sslot, int startPage)
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperKonamiUltimateCollection* rm;

    rm = calloc(1, sizeof(RomMapperKonamiUltimateCollection));

    rm->deviceHandle = deviceManagerRegister(ROM_KONAMIULTCOLL, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, read, peek, write, destroy, rm);

    rm->slot      = slot;
    rm->sslot     = sslot;
    rm->startPage = startPage;

    if (size > KUC_FLASH_SIZE) {
        size = KUC_FLASH_SIZE;
    }

    rm->flash = amdFlashCreate(AMD_TYPE_M29W640GB, KUC_FLASH_SIZE, KUC_SECTOR_SIZE, 0,
                               romData, size,
                               sramCreateFilenameWithSuffix(filename, "", ".sram"), 1);
    rm->scc = sccCreate(boardGetMixer());
    rm->dac = dacCreate(boardGetMixer(), DAC_MONO);

    reset(rm);

    return 1;
}

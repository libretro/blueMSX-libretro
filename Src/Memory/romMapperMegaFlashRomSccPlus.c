#include "romMapperMegaFlashRomSccPlus.h"
#include "AY8910.h"
#include "AmdFlash.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "sramLoader.h"
#include "SCC.h"
#include "Board.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    int deviceHandle;
    UInt8* romData;
    AmdFlash* flash;
    int slot;
    int sslot;
    int startPage;
    int size;
    int romMask;
    int romMapper[4];
    int flashPage[4];
    SccType sccType;
    SccMode sccMode;
    SCC* scc;
} RomMapperMegaFlashRomSccPlus;

static AY8910*         ay8910;

static UInt8 configReg;
static UInt8 offsetReg;
static UInt8 subslotReg;
static UInt8 bankRegs[4][4];
static UInt8 psgLatch;
static UInt8 sccBanks[4];

static UInt32 sectorSizes[19] = {
	0x4000, 0x2000, 0x2000, 0x8000,      // 16kB + 8kB + 8kB + 32kB
	0x10000, 0x10000, 0x10000, 0x10000,  // 15 * 64kB
	0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000,
};

static SccEnable getSCCEnable()
{
	if ((sccMode & 0x20) && (sccBanks[3] & 0x80)) {
		return EN_SCCPLUS;
	} else if ((!(sccMode & 0x20)) && ((sccBanks[2] & 0x3F) == 0x3F)) {
		return EN_SCC;
	} else {
		return EN_NONE;
	}
}

static UInt8 getSubslot(UInt16 address)
{
	return (configReg & 0x10)
	     ? (subslotReg >> (2 * (address >> 14))) & 0x03
	     : 0;
}

static UInt32 getFlashAddr(UInt16 address) const
{
	unsigned subslot = getSubslot(address);
	unsigned tmp;
	if ((configReg & 0xC0) == 0x40) {
		unsigned bank = bankRegs[subslot][address >> 14] + offsetReg;
		tmp = (bank * 0x4000) + (address & 0x3FFF);
	} else {
		unsigned page = (address >> 13) - 2;
		if (page >= 4) {
			// Bank: -2, -1, 4, 5. So not mapped in this region,
			// returned value should not be used. But querying it
			// anyway is easier, see start of writeMem().
			return unsigned(-1);
		}
		unsigned bank = bankRegs[subslot][page] + offsetReg;
		tmp = (bank * 0x2000) + (address & 0x1FFF);
	}
	return ((0x40000 * subslot) + tmp) & 0xFFFFF; // wrap at 1MB
}

static UInt8 peekMem(RomMapperMegaFlashRomSccPlus* rm, UInt16 address) 
{
	if ((configReg & 0x10) && (address == 0xFFFF)) {
		// read subslot register
		return subslotReg ^ 0xFF;
	}

	if ((configReg & 0xE0) == 0x00) {
		SccEnable enable = getSCCEnable();
		if (((enable == EN_SCC)     && (0x9800 <= address) && (address < 0xA000)) ||
		    ((enable == EN_SCCPLUS) && (0xB800 <= address) && (address < 0xC000))) {
			return scc->peekMem(address & 0xFF);
		}
	}

	if (((configReg & 0xC0) == 0x40) ||
	    ((0x4000 <= address) && (address < 0xC000))) {
		// read (flash)rom content
		UInt32 flashAddr = getFlashAddr(address);
		assert(flashAddr != unsigned(-1));
		return flash->peek(flashAddr);
	} else {
		// unmapped read
		return 0xFF;
	}
}

static UInt8 readMem(RomMapperMegaFlashRomSccPlus* rm, UInt16 address) 
{
	if ((configReg & 0x10) && (address == 0xFFFF)) {
		// read subslot register
		return subslotReg ^ 0xFF;
	}

	if ((configReg & 0xE0) == 0x00) {
		SCCEnable enable = getSCCEnable();
		if (((enable == EN_SCC)     && (0x9800 <= address) && (address < 0xA000)) ||
		    ((enable == EN_SCCPLUS) && (0xB800 <= address) && (address < 0xC000))) {
			return scc->readMem(address & 0xFF);
		}
	}

	if (((configReg & 0xC0) == 0x40) ||
	    ((0x4000 <= address) && (address < 0xC000))) {
		// read (flash)rom content
		UInt32 flashAddr = getFlashAddr(address);
		assert(flashAddr != unsigned(-1));
		return flash->read(flashAddr);
	} else {
		// unmapped read
		return 0xFF;
	}
}

static UInt8* getReadCacheLine(UInt16 address)
{
	if ((configReg & 0x10) &&
	    ((address & CacheLine::HIGH) == (0xFFFF & CacheLine::HIGH))) {
		// read subslot register
		return NULL;
	}

	if ((configReg & 0xE0) == 0x00) {
		SCCEnable enable = getSCCEnable();
		if (((enable == EN_SCC)     && (0x9800 <= address) && (address < 0xA000)) ||
		    ((enable == EN_SCCPLUS) && (0xB800 <= address) && (address < 0xC000))) {
			return NULL;
		}
	}

	if (((configReg & 0xC0) == 0x40) ||
	    ((0x4000 <= address) && (address < 0xC000))) {
		// read (flash)rom content
		UInt32 flashAddr = getFlashAddr(address);
		assert(flashAddr != unsigned(-1));
		return flash->getReadCacheLine(flashAddr);
	} else {
		// unmapped read
		return unmappedRead;
	}
}

static void writeMem(RomMapperMegaFlashRomSccPlus* rm, UInt16 address, UInt8 value) 
{
	// address is calculated before writes to other regions take effect
	UInt32 flashAddr = getFlashAddr(address);

	// There are several overlapping functional regions in the address
	// space. A single write can trigger behaviour in multiple regions. In
	// other words there's no priority amongst the regions where a higher
	// priority region blocks the write from the lower priority regions.
	if ((configReg & 0x10) && (address == 0xFFFF)) {
		// write subslot register
		byte diff = value ^ subslotReg;
		subslotReg = value;
		for (int i = 0; i < 4; ++i) {
			if (diff & (3 << (2 * i))) {
				invalidateMemCache(0x4000 * i, 0x4000);
			}
		}
	}

	if (((configReg & 0x04) == 0x00) && ((address & 0xFFFE) == 0x7FFE)) {
		// write config register
		configReg = value;
		invalidateMemCache(0x0000, 0x10000); // flush all to be sure
	}

	if ((configReg & 0xE0) == 0x00) {
		// Konami-SCC
		if ((address & 0xFFFE) == 0xBFFE) {
			sccMode = value;
			scc->setChipMode((value & 0x20) ? SCC::SCC_plusmode
			                                : SCC::SCC_Compatible);
		}
		SCCEnable enable = getSCCEnable();
		bool isRamSegment2 = ((sccMode & 0x24) == 0x24) ||
		                     ((sccMode & 0x10) == 0x10);
		bool isRamSegment3 = ((sccMode & 0x10) == 0x10);
		if (((enable == EN_SCC)     && !isRamSegment2 &&
		     (0x9800 <= address) && (address < 0xA000)) ||
		    ((enable == EN_SCCPLUS) && !isRamSegment3 &&
		     (0xB800 <= address) && (address < 0xC000))) {
			scc->writeMem(address & 0xFF, value);
		}
	}

	unsigned subslot = getSubslot(address);
	unsigned page8 = (address >> 13) - 2;
	if (((configReg & 0x02) == 0x00) && (page8 < 4)) {
		// (possibly) write to bank registers
		switch (configReg & 0xE0) {
		case 0x00:
			// Konami-SCC
			if ((address & 0x1800) == 0x1000) {
				// Storing 'sccBanks' may seem redundant at
				// first, but it's required to calculate
				// whether the SCC is enabled or not.
				sccBanks[page8] = value;
				if ((value & 0x80) && (page8 == 0)) {
					offsetReg = value & 0x7F;
					invalidateMemCache(0x4000, 0x8000);
				} else {
					// Making of the mapper bits is done on
					// write (and only in Konami(-scc) mode)
					byte mask = (configReg & 0x01) ? 0x3F : 0x7F;
					bankRegs[subslot][page8] = value & mask;
					invalidateMemCache(0x4000 + 0x2000 * page8, 0x2000);
				}
			}
			break;
		case 0x20: {
			// Konami
			if (((configReg & 0x08) == 0x08) && (address < 0x6000)) {
				// Switching 0x4000-0x5FFF disabled.
				// This bit blocks writing to the bank register
				// (an alternative was forcing a 0 on read).
				// It only has effect in Konami mode.
				break;
			}
			// Making of the mapper bits is done on
			// write (and only in Konami(-scc) mode)
			if ((address < 0x5000) || ((0x5800 <= address) && (address < 0x6000))) break; // only SCC range works
			byte mask = (configReg & 0x01) ? 0x1F : 0x7F;
			bankRegs[subslot][page8] = value & mask;
			invalidateMemCache(0x4000 + 0x2000 * page8, 0x2000);
			break;
		}
		case 0x40:
		case 0x60:
			// 64kB
			bankRegs[subslot][page8] = value;
			invalidateMemCache(0x4000 + 0x2000 * page8, 0x2000);
			break;
		case 0x80:
		case 0xA0:
			// ASCII-8
			if ((0x6000 <= address) && (address < 0x8000)) {
				byte bank = (address >> 11) & 0x03;
				bankRegs[subslot][bank] = value;
				invalidateMemCache(0x4000 + 0x2000 * page8, 0x2000);
			}
			break;
		case 0xC0:
		case 0xE0:
			// ASCII-16
			// This behaviour is confirmed by Manuel Pazos (creator
			// of the cartridge): ASCII-16 uses all 4 bank registers
			// and one bank switch changes 2 registers at once.
			// This matters when switching mapper mode, because
			// the content of the bank registers is unchanged after
			// a switch.
			if ((0x6000 <= address) && (address < 0x6800)) {
				bankRegs[subslot][0] = 2 * value + 0;
				bankRegs[subslot][1] = 2 * value + 1;
				invalidateMemCache(0x4000, 0x4000);
			}
			if ((0x7000 <= address) && (address < 0x7800)) {
				bankRegs[subslot][2] = 2 * value + 0;
				bankRegs[subslot][3] = 2 * value + 1;
				invalidateMemCache(0x8000, 0x4000);
			}
			break;
		}
	}

	// write to flash
	if (((configReg & 0xC0) == 0x40) ||
	    ((0x4000 <= address) && (address < 0xC000))) {
		assert(flashAddr != unsigned(-1));
		return flash->write(flashAddr, value);
	}
}

static UInt8* getWriteCacheLine(UInt16 /*addr*/)
{
	return NULL;
}

static UInt8 readIO(UInt16 port)
{
	assert((port & 0xFF) == 0x12); (void)port;
	return psg->readRegister(psgLatch);
}

static UInt8 peekIO(UInt16 port)
{
	assert((port & 0xFF) == 0x12); (void)port;
	return psg->peekRegister(psgLatch);
}

static void writeIO(UInt16 port, UInt8 value)
{
	if ((port & 0xFF) == 0x10) {
		psgLatch = value & 0x0F;
	} else {
		assert((port & 0xFF) == 0x11);
		psg->writeRegister(psgLatch, value);
	}
}

static void reset(RomMapperMegaFlashRomSccPlus* rm)
{
	configReg = 0;
	offsetReg = 0;
	subslotReg = 0;

    amdFlashReset(rm->flash);
    sccReset(rm->scc);
    if (ay8910 != NULL) {
        ay8910Reset(ay8910);
    }
}

static void saveState(RomMapperMegaFlashRomSccPlus* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperMegaFlashRomSccPlus");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        saveStateSet(state, tag, rm->romMapper[i]);
    }

    saveStateSet(state, "sccEnable", rm->sccEnable);

    saveStateClose(state);

    sccSaveState(rm->scc);
    amdFlashSaveState(rm->flash);
}

static void loadState(RomMapperMegaFlashRomSccPlus* rm)
{
    SaveState* state = saveStateOpenForRead("mapperMegaFlashRomSccPlus");
    char tag[16];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(tag, "romMapper%d", i);
        rm->romMapper[i] = saveStateGet(state, tag, 0);
    }
    
    rm->sccEnable = saveStateGet(state, "sccEnable", 0);

    saveStateClose(state);

    sccLoadState(rm->scc);    
    amdFlashLoadState(rm->flash);

    for (i = 0; i < 4; i++) {   
        mapPage(rm, i, rm->romMapper[i]);
    }    
}

static void destroy(RomMapperMegaFlashRomSccPlus* rm)
{
    amdFlashDestroy(rm->flash);
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    sccDestroy(rm->scc);

    free(rm->romData);
    free(rm);

    ay8910SetIoPort(ay8910, NULL, NULL, NULL, NULL);
    ay8910Destroy(ay8910);
    ay8910 = NULL;
}

static void mapPage(RomMapperMegaFlashRomSccPlus* rm, int bank, int page)
{
    int readEnable;
    UInt8* bankData;

    rm->romMapper[bank] = page & (rm->size / 0x2000 - 1);
    rm->flashPage[bank] = page;

    if (rm->flashPage[bank] < 0) {
        bankData = rm->romData + page * 0x2000;
    }
    else {
        bankData = amdFlashGetPage(rm->flash, rm->flashPage[bank] * 0x2000);
    }

    readEnable = (bank == 2 && rm->sccEnable) || rm->flashPage[bank] >= 0 ? 0 : 1;

    slotMapPage(rm->slot, rm->sslot, rm->startPage + bank, bankData, readEnable, 0);
}

int romMapperMegaFlashRomSccPlusCreate(const char* filename, UInt8* romData, 
                                       int size, int slot, int sslot, int startPage,
                                       SccEnable sccEnable, UInt32 writeProtectMask)
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    RomMapperMegaFlashRomSccPlus* rm;
    int i;

    rm = calloc(1, sizeof(RomMapperMegaFlashRomSccPlus));

    rm->deviceHandle = deviceManagerRegister(ROM_MEGAFLSHSCC, &callbacks, rm);
    slotRegister(slot, sslot, startPage, 4, read, peek, write, destroy, rm);

    if (size >= 0x80000) {
        size = 0x80000;
    }
    
    rm->romData = malloc(0x80000);
    memset(rm->romData, 0xff, 0x80000);
    memcpy(rm->romData, romData, size);
    rm->size = 0x80000;
    rm->slot  = slot;
    rm->sslot = sslot;
    rm->romMask = rm->size / 0x2000 - 1;
    rm->startPage  = startPage;
    rm->scc = sccCreate(boardGetMixer());
    sccSetMode(rm->scc, SCC_PLUS);
    rm->sccEnable = 0;

    rm->flash = amdFlashCreate(AMD_TYPE_2, 0x80000, 0x10000, writeProtectMask, romData, size, sramCreateFilenameWithSuffix(filename, "", ".sram"), 1);

    for (i = 0; i < 4; i++) {   
        mapPage(rm, i, i);
    }

    ay8910 = ay8910Create(boardGetMixer(), AY8910_MSX_SCCPLUS, PSGTYPE_AY8910, 0, machine->audio.psgpan);
    ay8910SetIoPort(ay8910, read, peek, write, NULL);

    return 1;
}

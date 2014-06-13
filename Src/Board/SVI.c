/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Board/SVI.c,v $
**
** $Revision: 1.55 $
**
** $Date: 2006/06/17 21:43:22 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Tomas Karlsson
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVI.h"

#include "R800.h"
#include "R800Dasm.h"
#include "R800SaveState.h"
#include "R800Debug.h"

#include "AY8910.h"
#include "SviPPI.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "Led.h"
#include "KeyClick.h"
#include "Casette.h"
#include "Disk.h"
#include "IoPort.h"
#include "MegaromCartridge.h"
#include "SlotManager.h"
#include "RomLoader.h"
#include "romMapperSvi80Col.h"
#include "SviJoyIo.h"

/* Hardware */
static AY8910*         ay8910;
static AudioKeyClick*  keyClick;
static R800*           r800;
static SviJoyIo*       joyIO;
static UInt8           KeyboardMap[16];
static UInt32          sviRamSize;
static UInt8*          sviRam;
static UInt8           psgAYReg15;
static int             svi80ColEnabled;
static UInt8           lastJoystickValue;

extern void PatchZ80(void* ref, CpuRegs* cpu);

static void sviMemWrite(void* ref, UInt16 address, UInt8 value)
{
    if ((svi80ColEnabled && svi80colMemBankCtrlStatus()) && (address & 0xf800) == 0xf000)
    {
        svi80colMemWrite(address & 0xfff, value);
    }
    else
        slotWrite(&ref, address, value);
}

static UInt8 sviMemRead(void* ref, UInt16 address)
{
    if ((svi80ColEnabled && svi80colMemBankCtrlStatus()) && (address & 0xf800) == 0xf000) 
        return svi80colMemRead(address & 0xfff);
    else
        return slotRead(&ref, address);
}

/*
       SLOT 0    SLOT 1    SLOT 2    SLOT 3
FFFF +---------+---------+---------+---------+
     | BANK 02 | BANK 12 | BANK 22 | BANK 32 | PAGE 3
     |   RAM   |ROM CART |   RAM   |   RAM   |
8000 |00000000 |00000000 |10100000 |11110000 | PAGE 2
     +---------+---------+---------+---------+
7FFF | BANK 01 | BANK 11 | BANK 21 | BANK 31 | PAGE 1
     |ROM BASIC|ROM CART |   RAM   |   RAM   |
     |00000000 |00000101 |00001010 |00001111 | PAGE 0
0000 +---------+---------+---------+---------+
*/

typedef enum { BANK_02=0x00, BANK_12=0x00, BANK_22=0xa0, BANK_32=0xf0 } sviBanksHigh;
typedef enum { BANK_01=0x00, BANK_11=0x05, BANK_21=0x0a, BANK_31=0x0f } sviBanksLow;

static void sviMemSetBank(UInt8 value)
{
    UInt8 pages;
    int i;
    psgAYReg15 = value;

    /* Map the SVI-328 bank to pages */
    pages = (value&1)?(value&2)?(value&8)?BANK_01:BANK_31:BANK_21:BANK_11;
    pages |= (value&4)?(value&16)?BANK_02:BANK_32:BANK_22;

    for (i = 0; i < 4; i++) {
        slotSetRamSlot(i, pages & 3);
        pages >>= 2;
    }
}

/*
PSG Port A Input
Bit Name   Description
 1  FWD1   Joystick 1, Forward
 2  BACK1  Joystick 1, Back
 3  LEFT1  Joystick 1, Left
 4  RIGHT1 Joystick 1, Right
 5  FWD2   Joystick 2, Forward
 6  BACK2  Joystick 2, Back
 7  LEFT2  Joystick 2, Left
 8  RIGHT2 Joystick 2, Right

PSG Port B Output
Bit Name    Description
1   /CART   Memory bank 11, ROM 0000-7FFF (Cartridge /CCS1, /CCS2)
2   /BK21   Memory bank 21, RAM 0000-7FFF
3   /BK22   Memory bank 22, RAM 8000-FFFF
4   /BK31   Memory bank 31, RAM 0000-7FFF
5   /BK32   Memory bank 32, RAM 8000-7FFF
6   CAPS    Caps-Lock diod
7   /ROMEN0 Memory bank 12, ROM 8000-BFFF* (Cartridge /CCS3)
8   /ROMEN1 Memory bank 12, ROM C000-FFFF* (Cartridge /CCS4)

* The /CART signal must be active for any effect,
  then all banks of RAM are disabled. */

static UInt8 sviPsgReadHandler(void* arg, UInt16 address) 
{
    UInt8 value = 0xff;

    switch (address) {
        case 0:
            value = sviJoyIoRead(joyIO);
            lastJoystickValue = value;
            break;
        case 1:
            value = psgAYReg15;
            break;
    }
    return value;
}

static UInt8 sviPsgPollHandler(void* arg, UInt16 address)
{
    UInt8 value = 0xff;

    switch (address) {
        case 0:
            value = lastJoystickValue;
            break;
        case 1:
            value = psgAYReg15;
            break;
    }
    return value;
}

static void sviPsgWriteHandler(void* arg, UInt16 address, UInt8 value) 
{
    switch (address) {
    case 0:
        break;
    case 1:
        ledSetCapslock(value & 0x20 ? 1 : 0);
        if ((psgAYReg15 & 0xDF) != (value & 0xDF))
            sviMemSetBank(value);
        break;
    }
}

static int sviLoad80Col(Machine* machine, VdpSyncMode vdpSyncMode)
{
    // The 80 column cart has a memory mapping that doesn't fit
    // the blueMSX memory management yet, so it needs special
    // handling. When supported this should be moved to Machine.c
    
    int success = 1;
    int i;

    for (i = 0; i < machine->slotInfoCount; i++) {
        if (!machine->slotInfo[i].error) {
            int size;
            UInt8* buf = romLoad(machine->slotInfo[i].name, machine->slotInfo[i].inZipName, &size);

            if (buf != NULL) {
                if (machine->slotInfo[i].romType == ROM_SVI80COL) {
                    int frameRate = (vdpSyncMode == VDP_SYNC_60HZ) ? 60 : 50;
                    svi80ColEnabled = romMapperSvi80ColCreate(frameRate, buf, size);
                    success &= svi80ColEnabled;
                }
                free(buf);
            }
        }
    }

    return success;
}

static void reset()
{
    UInt32 systemTime = boardSystemTime();

    slotManagerReset();

    if (r800 != NULL) {
        r800Reset(r800, systemTime);
    }
    if (ay8910 != NULL) {
        ay8910Reset(ay8910);
    }

    sviMemSetBank(0xDF);

    ledSetCapslock(0);

    deviceManagerReset();
}

static void destroy()
{
    boardRemoveExternalDevices();
    ay8910SetIoPort(ay8910, NULL, NULL, NULL, NULL);
    ay8910Destroy(ay8910);
    ay8910 = NULL;
    audioKeyClickDestroy(keyClick);
    r800DebugDestroy();
    slotManagerDestroy();
    deviceManagerDestroy();
    r800Destroy(r800);
}

static int getRefreshRate()
{
    return vdpGetRefreshRate();
}

static void saveState()
{
    SaveState* state = saveStateOpenForWrite("svi");

    saveStateSet(state, "svi80ColEnabled", svi80ColEnabled);
    saveStateSet(state, "psgAYReg15", psgAYReg15);

    saveStateClose(state);

    r800SaveState(r800);
    deviceManagerSaveState();
    slotSaveState();
    ay8910SaveState(ay8910);
}

static void loadState()
{
    SaveState* state = saveStateOpenForRead("svi");

    svi80ColEnabled = saveStateGet(state, "svi80ColEnabled", 0);
    psgAYReg15      = (UInt8)saveStateGet(state, "psgAYReg15", 0);

    saveStateClose(state);
    
    r800LoadState(r800);
    boardInit(&r800->systemTime);
    deviceManagerLoadState();
    slotLoadState();
    ay8910LoadState(ay8910);
}

int sviCreate(Machine* machine, 
              VdpSyncMode vdpSyncMode,
              BoardInfo* boardInfo)
{
    int success;
    int i;

    r800 = r800Create(0, sviMemRead, sviMemWrite, ioPortRead, ioPortWrite, PatchZ80, boardTimerCheckTimeout, NULL, NULL, NULL);

    boardInfo->cartridgeCount   = 1;
    boardInfo->diskdriveCount   = 2;
    boardInfo->casetteCount     = 1;
    boardInfo->cpuRef           = r800;

    boardInfo->destroy          = destroy;
    boardInfo->softReset        = reset;
    boardInfo->loadState        = loadState;
    boardInfo->saveState        = saveState;
    boardInfo->getRefreshRate   = getRefreshRate;
    boardInfo->getRamPage       = NULL;

    boardInfo->run              = r800Execute;
    boardInfo->stop             = r800StopExecution;
    boardInfo->setInt           = r800SetInt;
    boardInfo->clearInt         = r800ClearInt;
    boardInfo->setCpuTimeout    = r800SetTimeoutAt;
    boardInfo->setBreakpoint    = r800SetBreakpoint;
    boardInfo->clearBreakpoint  = r800ClearBreakpoint;

    deviceManagerCreate();
    boardInit(&r800->systemTime);
    ioPortReset();
    r800Reset(r800, 0);
    mixerReset(boardGetMixer());

    r800DebugCreate(r800);

    ay8910 = ay8910Create(boardGetMixer(), AY8910_SVI, PSGTYPE_AY8910);
    ay8910SetIoPort(ay8910, sviPsgReadHandler, sviPsgPollHandler, sviPsgWriteHandler, NULL);

    keyClick  = audioKeyClickCreate(boardGetMixer());

    joyIO = sviJoyIoCreate();
    
    sviPPICreate(joyIO);
    slotManagerCreate();

    svi80ColEnabled = 0;

    /* Initialize VDP */
    vdpCreate(VDP_SVI, machine->video.vdpVersion, vdpSyncMode, machine->video.vramSize / 0x4000);

    /* Initialize memory */
    for (i = 0; i < 4; i++) {
        slotSetSubslotted(i, 0);
    }
    for (i = 0; i < 2; i++) {
        cartridgeSetSlotInfo(i, machine->cart[i].slot, 0);
    }

    success = machineInitialize(machine, &sviRam, &sviRamSize);
    success &= sviLoad80Col(machine, vdpSyncMode);

    for (i = 0; i < 8; i++) {
        slotMapRamPage(0, 0, i);
    }

    sviMemSetBank(0xDF);
    ledSetCapslock(0);

    if (success) {
        success = boardInsertExternalDevices();
    }

    memset(KeyboardMap, 0xff, 16);

    r800SetFrequency(r800, CPU_Z80,  machine->cpu.freqZ80);
    r800SetFrequency(r800, CPU_R800, machine->cpu.freqR800);

    diskEnable(0, machine->fdc.count > 0);
    diskEnable(1, machine->fdc.count > 1);

    if (!success) {
        destroy();
    }

    return success;
}

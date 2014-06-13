/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Board/MSX.c,v $
**
** $Revision: 1.62 $
**
** $Date: 2006/06/15 23:26:11 $
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MSX.h"

#include "R800.h"
#include "R800Dasm.h"
#include "R800SaveState.h"
#include "R800Debug.h"

#include "SaveState.h"
#include "MsxPPI.h"
#include "Board.h"
#include "RTC.h"
#include "MsxPsg.h"
#include "VDP.h"
#include "Casette.h"
#include "Disk.h"
#include "MegaromCartridge.h"
#include "IoPort.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "ramMapperIo.h"

void PatchZ80(void* ref, CpuRegs* cpuRegs);

// Hardware
static MsxPsg*         msxPsg;
static R800*           r800;
static RTC*            rtc;
static UInt8*          msxRam;
static UInt32          msxRamSize;
static UInt32          z80Frequency;

void msxSetCpu(int mode)
{
    switch (mode) {
    default:
    case 0:
        r800SetMode(r800, CPU_Z80);
        break;
    case 1:
        r800SetMode(r800, CPU_R800);
        break;
    }
}

void msxEnableCpuFreq_1_5(int enable) {
    if (enable) {
        r800SetFrequency(r800, CPU_Z80, 3 * z80Frequency / 2);
    }
    else {
        r800SetFrequency(r800, CPU_Z80, z80Frequency);
    }
}

static void reset()
{
    UInt32 systemTime = boardSystemTime();

    slotManagerReset();

    if (r800 != NULL) {
        r800Reset(r800, systemTime);
    }
    
    deviceManagerReset();
}

static void destroy() {        
    rtcDestroy(rtc);

    boardRemoveExternalDevices();

    slotManagerDestroy();

    r800DebugDestroy();

    deviceManagerDestroy();

    r800Destroy(r800);
}

int getPC(){return r800->regs.PC.W;}

static int getRefreshRate()
{
    return vdpGetRefreshRate();
}

static UInt8* getRamPage(int page) {
    static UInt8 emptyRam[0x2000];

    if (msxRam == NULL) {
        return emptyRam;
    }

	return msxRam + ((page * 0x2000) & (msxRamSize - 1));
}
    
static void saveState()
{   
    SaveState* state = saveStateOpenForWrite("msx");

    saveStateSet(state, "z80Frequency",    z80Frequency);
    
    saveStateClose(state);

    r800SaveState(r800);
    deviceManagerSaveState();
    slotSaveState();
    rtcSaveState(rtc);
}

static void loadState()
{
    SaveState* state = saveStateOpenForRead("msx");

    z80Frequency = saveStateGet(state, "z80Frequency", 0);

    saveStateClose(state);

    r800LoadState(r800);
    boardInit(&r800->systemTime);

    deviceManagerLoadState();
    slotLoadState();
    rtcLoadState(rtc);
}

int msxCreate(Machine* machine, 
              VdpSyncMode vdpSyncMode,
              BoardInfo* boardInfo)
{
    char cmosName[512];
    int success;
    int i;

    UInt32 cpuFlags = CPU_ENABLE_M1;

    if (machine->board.type == BOARD_MSX_T9769B ||
        machine->board.type == BOARD_MSX_T9769C)
    {
        cpuFlags |= CPU_VDP_IO_DELAY;
    }

    r800 = r800Create(cpuFlags, slotRead, slotWrite, ioPortRead, ioPortWrite, PatchZ80, boardTimerCheckTimeout, NULL, NULL, NULL);

    boardInfo->cartridgeCount   = 2;
    boardInfo->diskdriveCount   = 2;
    boardInfo->casetteCount     = 1;
    boardInfo->cpuRef           = r800;

    boardInfo->destroy          = destroy;
    boardInfo->softReset        = reset;
    boardInfo->loadState        = loadState;
    boardInfo->saveState        = saveState;
    boardInfo->getRefreshRate   = getRefreshRate;
    boardInfo->getRamPage       = getRamPage;

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
    ramMapperIoCreate();

    r800Reset(r800, 0);
    mixerReset(boardGetMixer());

    msxPPICreate();
    slotManagerCreate();

    r800DebugCreate(r800);

    sprintf(cmosName, "%s" DIR_SEPARATOR "%s.cmos", boardGetBaseDirectory(), machine->name);
    rtc = rtcCreate(machine->cmos.enable, machine->cmos.batteryBacked ? cmosName : 0);

    msxRam = NULL;

    vdpCreate(VDP_MSX, machine->video.vdpVersion, vdpSyncMode, machine->video.vramSize / 0x4000);

    for (i = 0; i < 4; i++) {
        slotSetSubslotted(i, machine->slot[i].subslotted);
    }

    for (i = 0; i < 2; i++) {
        cartridgeSetSlotInfo(i, machine->cart[i].slot, machine->cart[i].subslot);
    }

    success = machineInitialize(machine, &msxRam, &msxRamSize);

    msxPsg = msxPsgCreate(machine->board.type == BOARD_MSX ? PSGTYPE_AY8910 : PSGTYPE_YM2149);

    for (i = 0; i < 8; i++) {
        slotMapRamPage(0, 0, i);
    }

    if (success) {
        success = boardInsertExternalDevices();
    }

    z80Frequency = machine->cpu.freqZ80;

    diskEnable(0, machine->fdc.count > 0);
    diskEnable(1, machine->fdc.count > 1);

    r800SetFrequency(r800, CPU_Z80,  machine->cpu.freqZ80);
    r800SetFrequency(r800, CPU_R800, machine->cpu.freqR800);

    if (!success) {
        destroy();
    }

    return success;
}

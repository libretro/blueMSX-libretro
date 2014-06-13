/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/Y8950.c,v $
**
** $Revision: 1.17 $
**
** $Date: 2006/06/15 00:18:16 $
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
#include "Y8950.h"
#include "Fmopl.h"
#include "Board.h"
#include "SaveState.h"
#include "IoPort.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>

#define FREQUENCY        3579545
#define SAMPLERATE       (FREQUENCY / 72)
#define SAMPLERATE_OUT   44100
#define BUFFER_SIZE      10000
#define TIMER_FREQUENCY  (4 * boardFrequency() / SAMPLERATE)


struct Y8950 {
    Mixer* mixer;
    Int32  handle;

    FM_OPL* opl;
    BoardTimer* timer1;
    BoardTimer* timer2;
    UInt32 timerValue1;
    UInt32 timerValue2;
    UInt32 timeout1;
    UInt32 timeout2;
    UInt32 timerRunning1;
    UInt32 timerRunning2;
    UInt8  address;
    // Variables used for resampling
    Int32  off;
    Int32  s1;
    Int32  s2;
    Int32  buffer[BUFFER_SIZE];
};

extern INT32 outd;
extern INT32 ams;
extern INT32 vib;
extern INT32 feedback2;

void y8950TimerStart(void* ptr, int timer, int start);

static void onTimeout1(void* ptr, UInt32 time)
{
    Y8950* y8950 = (Y8950*)ptr;

    y8950->timerRunning1 = 0;
    if (OPLTimerOver(y8950->opl, 0)) {
        y8950TimerStart(y8950, 0, 1);
    }
}

static void onTimeout2(void* ptr, UInt32 time)
{
    Y8950* y8950 = (Y8950*)ptr;

    y8950->timerRunning2 = 0;
    if (OPLTimerOver(y8950->opl, 1)) {
        y8950TimerStart(y8950, 1, 1);
    }
}

void y8950TimerStart(void* ptr, int timer, int start)
{
    Y8950* y8950 = (Y8950*)ptr;

    if (timer == 0) {
        if (start != 0) {
            if (!y8950->timerRunning1) {
                UInt32 systemTime = boardSystemTime();
                UInt32 adjust = systemTime % TIMER_FREQUENCY;
                y8950->timeout1 = systemTime + TIMER_FREQUENCY * y8950->timerValue1 - adjust;
                boardTimerAdd(y8950->timer1, y8950->timeout1);
                y8950->timerRunning1 = 1;
            }
        }
        else {
            if (y8950->timerRunning1) {
                boardTimerRemove(y8950->timer1);
                y8950->timerRunning1 = 0;
            }
        }
    }
    else {
        if (start != 0) {
            if (!y8950->timerRunning2) {
                UInt32 systemTime = boardSystemTime();
                UInt32 adjust = systemTime % (4 * TIMER_FREQUENCY);
                y8950->timeout2 = systemTime + TIMER_FREQUENCY * y8950->timerValue2 - adjust;
                boardTimerAdd(y8950->timer2, y8950->timeout2);
                y8950->timerRunning2 = 1;
            }
        }
        else {
            if (y8950->timerRunning2) {
                boardTimerRemove(y8950->timer2);
                y8950->timerRunning2 = 0;
            }
        }
    }
}

UInt8 y8950Peek(Y8950* y8950, UInt16 ioPort)
{
    if (y8950 != NULL) {
        switch (ioPort & 1) {
        case 0:
            return (UInt8)OPLPeek(y8950->opl, 0);
        case 1:
            return (UInt8)OPLPeek(y8950->opl, 1);
            break;
        }
    }
    return  0xff;
}

UInt8 y8950Read(Y8950* y8950, UInt16 ioPort)
{
    switch (ioPort & 1) {
    case 0:
        return (UInt8)OPLRead(y8950->opl, 0);
    case 1:
        if (y8950->opl->address == 0x14) {
            mixerSync(y8950->mixer);
        }
        return (UInt8)OPLRead(y8950->opl, 1);
        break;
    }
    return  0xff;
}

void y8950Write(Y8950* y8950, UInt16 ioPort, UInt8 value)
{
    switch (ioPort & 1) {
    case 0:
        OPLWrite(y8950->opl, 0, value);
        break;
    case 1:
        mixerSync(y8950->mixer);
        OPLWrite(y8950->opl, 1, value);
        break;
    }
}
static char* regText(int d)
{
    static char text[5];
    sprintf(text, "R%.2x", d);
    return text;
}

static char regsAvailAY8950[] = {
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0, // 0x00
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, // 0x20
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, // 0x40
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, // 0x60
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, // 0x80
    1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0, // 0xa0
    1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0xc0
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1  // 0xe0
};

void y8950GetDebugInfo(Y8950* y8950, DbgDevice* dbgDevice)
{
    DbgRegisterBank* regBank;

    // Add YM2413 registers
    int c = 1;
    int r;

    for (r = 0; r < sizeof(regsAvailAY8950); r++) {
        c += regsAvailAY8950[r];
    }

    regBank = dbgDeviceAddRegisterBank(dbgDevice, langDbgRegsAy8950(), c);
    
    c = 0;

    dbgRegisterBankAddRegister(regBank, c++, "SR", 8, (UInt8)OPLRead(y8950->opl, 0));

    for (r = 0; r < sizeof(regsAvailAY8950); r++) {
        if (regsAvailAY8950[r]) {
            dbgRegisterBankAddRegister(regBank, c++, regText(r), 8, y8950->opl->regs[r]);
        }
    }

    dbgDeviceAddMemoryBlock(dbgDevice, langDbgMemAy8950(), 0, 0, 
                            y8950->opl->deltat->memory_size, 
                            (UInt8*)y8950->opl->deltat->memory);
}
    
static Int32* y8950Sync(void* ref, UInt32 count) 
{
    Y8950* y8950 = (Y8950*)ref;
    UInt32 i;

    for (i = 0; i < count; i++) {
#if SAMPLERATE > SAMPLERATE_OUT
        y8950->off -= SAMPLERATE - SAMPLERATE_OUT;
        y8950->s1 = y8950->s2;
        y8950->s2 = Y8950UpdateOne(y8950->opl);
        if (y8950->off < 0) {
            y8950->off += SAMPLERATE_OUT;
            y8950->s1 = y8950->s2;
            y8950->s2 = Y8950UpdateOne(y8950->opl);
        }
        y8950->buffer[i] = (y8950->s1 * (y8950->off / 256) + y8950->s2 * ((SAMPLERATE - y8950->off) / 256)) / (SAMPLERATE / 256);
#else
        y8950->buffer[i] = Y8950UpdateOne(y8950->opl);
#endif
    }

    return y8950->buffer;
}


void y8950SaveState(Y8950* y8950)
{
    SaveState* state = saveStateOpenForWrite("msxaudio1");

    saveStateSet(state, "address",       y8950->address);
    saveStateSet(state, "timerValue1",   y8950->timerValue1);
    saveStateSet(state, "timerRunning1", y8950->timerRunning1);
    saveStateSet(state, "timeout1",      y8950->timeout1);
    saveStateSet(state, "timerValue2",   y8950->timerValue2);
    saveStateSet(state, "timerRunning2", y8950->timerRunning2);
    saveStateSet(state, "timeout2",      y8950->timeout2);
    saveStateSet(state, "outd",          outd);
    saveStateSet(state, "ams",           ams);
    saveStateSet(state, "vib",           vib);
    saveStateSet(state, "feedback2",     feedback2);

    saveStateClose(state);

    Y8950SaveState(y8950->opl);
    YM_DELTAT_ADPCM_SaveState(y8950->opl->deltat);
}

void y8950LoadState(Y8950* y8950)
{
    SaveState* state = saveStateOpenForRead("msxaudio1");

    y8950->address       = (UInt8)saveStateGet(state, "address",       0);
    y8950->timerValue1   =        saveStateGet(state, "timerValue1",   0);
    y8950->timeout1      =        saveStateGet(state, "timeout1",      0);
    y8950->timerRunning1 =        saveStateGet(state, "timerRunning1", 0);
    y8950->timerValue2   =        saveStateGet(state, "timerValue2",   0);
    y8950->timerRunning2 =        saveStateGet(state, "timerRunning2", 0);
    y8950->timeout2      =        saveStateGet(state, "timeout2",      0);

    outd      = saveStateGet(state, "outd",      0);
    ams       = saveStateGet(state, "ams",       0);
    vib       = saveStateGet(state, "vib",       0);
    feedback2 = saveStateGet(state, "feedback2", 0);

    saveStateClose(state);

    Y8950LoadState(y8950->opl);
    YM_DELTAT_ADPCM_LoadState(y8950->opl->deltat);

    if (y8950->timerRunning1) {
        boardTimerAdd(y8950->timer1, y8950->timeout1);
    }

    if (y8950->timerRunning2) {
        boardTimerAdd(y8950->timer2, y8950->timeout2);
    }
}

void y8950Destroy(Y8950* y8950) 
{
    mixerUnregisterChannel(y8950->mixer, y8950->handle);
    boardTimerDestroy(y8950->timer1);
    boardTimerDestroy(y8950->timer2);
    OPLDestroy(y8950->opl);

    free(y8950);
}

void y8950Reset(Y8950* y8950)
{
    y8950TimerStart(y8950, 0, 0);
    y8950TimerStart(y8950, 1, 0);
    OPLResetChip(y8950->opl);
    y8950->off = 0;
    y8950->s1 = 0;
    y8950->s2 = 0;
}

Y8950* y8950Create(Mixer* mixer)
{
    Y8950* y8950;
    
    y8950 = (Y8950*)calloc(1, sizeof(Y8950));

    y8950->mixer = mixer;
    y8950->timerRunning1 = 0;
    y8950->timerRunning2 = 0;

    y8950->timer1 = boardTimerCreate(onTimeout1, y8950);
    y8950->timer2 = boardTimerCreate(onTimeout2, y8950);

    y8950->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_MSXAUDIO, 0, y8950Sync, y8950);

    y8950->opl = OPLCreate(OPL_TYPE_Y8950, FREQUENCY, SAMPLERATE, 256, y8950);
    OPLSetOversampling(y8950->opl, boardGetY8950Oversampling());
    OPLResetChip(y8950->opl);

    return y8950;
}

void y8950TimerSet(void* ref, int timer, int count)
{
    Y8950* y8950 = (Y8950*)ref;

    if (timer == 0) {
        y8950->timerValue1 = count;
    }
    else {
        y8950->timerValue2 = count;
    }
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/ym2151.c,v $
**
** $Revision: 1.7 $
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
#include "ym2151.h"
#include "MameYM2151.h"
#include "Board.h"
#include "SaveState.h"
#include "IoPort.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include <stdlib.h>
#include <string.h>

#define FREQUENCY        3579545
#define SAMPLERATE       (FREQUENCY / 64)
#define SAMPLERATE_OUT   44100
#define BUFFER_SIZE      10000
#define TIMER_FREQUENCY  (boardFrequency() / FREQUENCY * 64)


struct YM2151 {
    Mixer* mixer;
    Int32  handle;

    MameYm2151* opl;
    BoardTimer* timer1;
    BoardTimer* timer2;
    UInt32 timerValue1;
    UInt32 timerValue2;
    UInt32 timeout1;
    UInt32 timeout2;
    UInt32 timerRunning1;
    UInt32 timerRunning2;
    UInt8  address;
    UInt8  latch;
    // Variables used for resampling
    Int32  off;
    Int32  s1l;
    Int32  s2l;
    Int32  s1r;
    Int32  s2r;
    Int32  buffer[BUFFER_SIZE];
};

void ym2151TimerStart(void* ptr, int timer, int start);

void ym2151Irq(void* ptr, int irq)
{
    if (irq) {
		boardSetInt(0x40);
    }
    else {
		boardClearInt(0x40);
    }
}

void ym2151WritePortCallback(void* ref, UInt32 port, UInt8 value)
{
}

static void onTimeout1(void* ptr, UInt32 time)
{
    YM2151* ym2151 = (YM2151*)ptr;
    ym2151->timerRunning1 = 0;
    YM2151TimerCallback(ym2151->opl, 0);
    ym2151TimerStart(ptr, 0, 1);
}

static void onTimeout2(void* ptr, UInt32 time)
{
    YM2151* ym2151 = (YM2151*)ptr;

    ym2151->timerRunning2 = 0;
    YM2151TimerCallback(ym2151->opl, 1);
    ym2151TimerStart(ptr, 1, 1);
}

void ym2151TimerStart(void* ptr, int timer, int start)
{
    YM2151* ym2151 = (YM2151*)ptr;

    if (timer == 0) {
        if (start != 0) {
            if (!ym2151->timerRunning1) {
                UInt32 systemTime = boardSystemTime();
                UInt32 adjust = systemTime % TIMER_FREQUENCY;
                ym2151->timeout1 = systemTime + TIMER_FREQUENCY * ym2151->timerValue1 - adjust;
                boardTimerAdd(ym2151->timer1, ym2151->timeout1);
                ym2151->timerRunning1 = 1;
            }
        }
        else {
            if (ym2151->timerRunning1) {
                boardTimerRemove(ym2151->timer1);
                ym2151->timerRunning1 = 0;
            }
        }
    }
    else {
        if (start != 0) {
            if (!ym2151->timerRunning2) {
                UInt32 systemTime = boardSystemTime();
                UInt32 adjust = systemTime % (16 * TIMER_FREQUENCY);
                ym2151->timeout2 = systemTime + TIMER_FREQUENCY * ym2151->timerValue2 - adjust;
                boardTimerAdd(ym2151->timer2, ym2151->timeout2);
                ym2151->timerRunning2 = 1;
            }
        }
        else {
            if (ym2151->timerRunning2) {
                boardTimerRemove(ym2151->timer2);
                ym2151->timerRunning2 = 0;
            }
        }
    }
}

UInt8 ym2151Peek(YM2151* ym2151, UInt16 ioPort)
{
    return (UInt8)YM2151ReadStatus(ym2151->opl);
}

UInt8 ym2151Read(YM2151* ym2151, UInt16 ioPort)
{
    return (UInt8)YM2151ReadStatus(ym2151->opl);
}

void ym2151Write(YM2151* ym2151, UInt16 ioPort, UInt8 value)
{
    switch (ioPort & 1) {
    case 0:
        ym2151->latch = value;
        break;
    case 1:
        mixerSync(ym2151->mixer);
        YM2151WriteReg(ym2151->opl, ym2151->latch, value);
        break;
    }
}

void ym2151GetDebugInfo(YM2151* ym2151, DbgDevice* dbgDevice)
{
}
    
static Int32* ym2151Sync(void* ref, UInt32 count) 
{
    YM2151* ym2151 = (YM2151*)ref;
    UInt32 i;

    for (i = 0; i < count; i++) {
        Int16 sl, sr;
        ym2151->off -= SAMPLERATE - SAMPLERATE_OUT;
        ym2151->s1l = ym2151->s2l;
        ym2151->s1r = ym2151->s2r;
        YM2151UpdateOne(ym2151->opl, &sl, &sr, 1);
        ym2151->s2l = sl;
        ym2151->s2r = sr;
        if (ym2151->off < 0) {
            ym2151->off += SAMPLERATE_OUT;
            ym2151->s1l = ym2151->s2l;
            ym2151->s1r = ym2151->s2r;
            YM2151UpdateOne(ym2151->opl, &sl, &sr, 1);
            ym2151->s2l = sl;
            ym2151->s2r = sr;
        }
        ym2151->buffer[2*i+0] = 11*(Int32)((ym2151->s1l * (ym2151->off / 256) + ym2151->s2l * ((SAMPLERATE - ym2151->off) / 256)) / (SAMPLERATE / 256));
        ym2151->buffer[2*i+1] = 11*(Int32)((ym2151->s1r * (ym2151->off / 256) + ym2151->s2r * ((SAMPLERATE - ym2151->off) / 256)) / (SAMPLERATE / 256));
    }

    return ym2151->buffer;
}


void ym2151SaveState(YM2151* ym2151)
{
    SaveState* state = saveStateOpenForWrite("ym2151");

    saveStateSet(state, "address",       ym2151->address);
    saveStateSet(state, "latch",         ym2151->latch);
    saveStateSet(state, "timerValue1",   ym2151->timerValue1);
    saveStateSet(state, "timerRunning1", ym2151->timerRunning1);
    saveStateSet(state, "timeout1",      ym2151->timeout1);
    saveStateSet(state, "timerValue2",   ym2151->timerValue2);
    saveStateSet(state, "timerRunning2", ym2151->timerRunning2);
    saveStateSet(state, "timeout2",      ym2151->timeout2);

    saveStateClose(state);

    YM2151SaveState(ym2151->opl);
}

void ym2151LoadState(YM2151* ym2151)
{
    SaveState* state = saveStateOpenForRead("ym2151");

    ym2151->address       = (UInt8)saveStateGet(state, "address",       0);
    ym2151->latch         = (UInt8)saveStateGet(state, "latch",         0);
    ym2151->timerValue1   =        saveStateGet(state, "timerValue1",   0);
    ym2151->timerRunning1 =        saveStateGet(state, "timerRunning1", 0);
    ym2151->timeout1      =        saveStateGet(state, "timeout1",      0);
    ym2151->timerValue2   =        saveStateGet(state, "timerValue2",   0);
    ym2151->timerRunning2 =        saveStateGet(state, "timerRunning2", 0);
    ym2151->timeout2      =        saveStateGet(state, "timeout2",      0);

    saveStateClose(state);

    YM2151LoadState(ym2151->opl);

    if (ym2151->timerRunning1) {
        boardTimerAdd(ym2151->timer1, ym2151->timeout1);
    }

    if (ym2151->timerRunning2) {
        boardTimerAdd(ym2151->timer2, ym2151->timeout2);
    }
}

void ym2151Destroy(YM2151* ym2151) 
{
    mixerUnregisterChannel(ym2151->mixer, ym2151->handle);
    boardTimerDestroy(ym2151->timer1);
    boardTimerDestroy(ym2151->timer2);
    YM2151Destroy(ym2151->opl);

    free(ym2151);
}

void ym2151Reset(YM2151* ym2151)
{
    ym2151TimerStart(ym2151, 0, ym2151->timerValue1);
    ym2151TimerStart(ym2151, 1, ym2151->timerValue2);
    YM2151ResetChip(ym2151->opl);
    ym2151->off = 0;
    ym2151->s1l = 0;
    ym2151->s2l = 0;
    ym2151->s1r = 0;
    ym2151->s2r = 0;
    ym2151->latch = 0;
}

YM2151* ym2151Create(Mixer* mixer)
{
    YM2151* ym2151;
    
    ym2151 = (YM2151*)calloc(1, sizeof(YM2151));

    ym2151->mixer = mixer;
    ym2151->timerRunning1 = 0;
    ym2151->timerRunning2 = 0;

    ym2151->timer1 = boardTimerCreate(onTimeout1, ym2151);
    ym2151->timer2 = boardTimerCreate(onTimeout2, ym2151);

    ym2151->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_YAMAHA_SFG, 1, ym2151Sync, ym2151);

    ym2151->opl = YM2151Create(ym2151, FREQUENCY, SAMPLERATE);

    return ym2151;
}

void ym2151TimerSet(void* ref, int timer, int count)
{
    YM2151* ym2151 = (YM2151*)ref;

    if (timer == 0) {
        ym2151->timerValue1 = 2 * count;
    }
    else {
        ym2151->timerValue2 = 2 * count;
    }
}

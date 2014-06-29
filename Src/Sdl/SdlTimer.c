/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Sdl/SdlTimer.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2008-03-30 18:38:45 $
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
#include "ArchTimer.h"
#include <stdlib.h>
#include <SDL.h>

#ifdef NO_TIMERS

// Timer callbacks are not needed. When disabled, there is no need for
// archEvents either.

void* archCreateTimer(int period, int (*timerCallback)(void*)) { return NULL; }
void archTimerDestroy(void* timer) {}


// The only timer that is required is a high res timer. The resolution is
// not super important, the higher the better, but one tick every 10ms is
// good enough. The frequency argument is in Hz and is 1000 or less.
UInt32 archGetSystemUpTime(UInt32 frequency) 
{
    return SDL_GetTicks() / (1000 / frequency);
}

// This is just a timer value with a frequency as high as possible. 
// The frequency of the timer is not important.
UInt32 archGetHiresTimer() {
    return SDL_GetTicks();
}

#else 

static void (*timerCb)(void*) = NULL;
static UInt32 timerFreq;
static UInt32 lastTimeout;

Uint32 timerCalback(Uint32 interval)
{
    if (timerCb) {
        UInt32 currentTime = archGetSystemUpTime(timerFreq);

        while (lastTimeout != currentTime) {
            lastTimeout = currentTime;
            timerCb(timerCb);
        }
    }
    return interval;
}

void* archCreateTimer(int period, int (*timerCallback)(void*)) 
{ 
    timerFreq = 1000 / period;
    lastTimeout = archGetSystemUpTime(timerFreq);
    timerCb  = timerCallback;

    SDL_SetTimer(period, timerCalback);

    return timerCallback;
}

void archTimerDestroy(void* timer) 
{
    if (timerCb != timer) {
        return;
    }

    SDL_SetTimer(0, NULL);
    timerCb = NULL;
}

UInt32 archGetSystemUpTime(UInt32 frequency) 
{
    return SDL_GetTicks() / (1000 / frequency);
}

UInt32 archGetHiresTimer() {
    return SDL_GetTicks();
}

#endif
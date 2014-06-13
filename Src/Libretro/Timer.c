/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Sdl/SdlTimer.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2006/06/24 02:27:08 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
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
#include "ArchTimer.h"
#include <stdlib.h>
#include "SDL/SDL.h"

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

void* archCreateTimer(int period, void (*timerCallback)(void*))
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

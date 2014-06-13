/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Linux/blueMSXlite/LinuxTimer.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/06/19 18:20:39 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vikl, Tomas Karlsson
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
#include <sys/time.h>
#include <signal.h>


static void (*timerCb)(void*) = NULL;
static UInt32 timerFreq;
static UInt32 lastTimeout;

static void timerCalback(int arg)
{
    if (timerCb) {
        UInt32 currentTime = archGetSystemUpTime(timerFreq);

        while (lastTimeout != currentTime) {
            lastTimeout++;
            timerCb(timerCb);
        }
    }
}

void* archCreateTimer(int period, void (*timerCallback)(void*)) 
{ 
    struct itimerval timeVal;
    int success;

    timerFreq = 1000 / period;
    lastTimeout = archGetSystemUpTime(timerFreq);
    timerCb  = timerCallback;

    timeVal.it_interval.tv_sec  = 0;
    timeVal.it_value.tv_sec     = 0;
    timeVal.it_interval.tv_usec = timerFreq;
    timeVal.it_value.tv_usec    = timerFreq;
    success = setitimer(ITIMER_REAL, &timeVal, NULL) == 0;
    if (success) {
        signal(SIGALRM, timerCalback);
    }

    return timerCallback;
}

void archTimerDestroy(void* timer) 
{
    if (timerCb != timer) {
        return;
    }

    struct itimerval timeVal;

    timeVal.it_interval.tv_sec  = 0;
    timeVal.it_value.tv_sec     = 0;
    timeVal.it_interval.tv_usec = 0;
    timeVal.it_value.tv_usec    = 0;
    setitimer(ITIMER_REAL, &timeVal, NULL);
    signal(SIGALRM,SIG_DFL);

    timerCb = NULL;
}

UInt32 archGetSystemUpTime(UInt32 frequency) 
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    
    return tv.tv_sec * frequency + tv.tv_usec / (1000000 / frequency);
}

UInt32 archGetHiresTimer() 
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000000 + tv.tv_usec;
}

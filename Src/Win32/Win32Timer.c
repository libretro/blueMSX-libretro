/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Timer.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2005/10/04 23:03:34 $
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
#include "ArchTimer.h"
#include "Win32Common.h"
#include <windows.h>

typedef struct {
    MMRESULT timerId;
} Win32Timer;

static void (*timerCb)(void*) = NULL;
static void* timerId = NULL;

static void syncCallback() {
    if (timerCb) {
        timerCb(timerId);
    }
}

UInt32 archGetHiresTimer() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.LowPart;
}

UInt32 archGetSystemUpTime(UInt32 frequency)
{
    static LONGLONG hfFrequency = 0;
    LARGE_INTEGER li;

    if (!hfFrequency) {
        if (QueryPerformanceFrequency(&li)) {
            hfFrequency = li.QuadPart;
        }
        else {
            return 0;
        }
    }

    QueryPerformanceCounter(&li);

    return (DWORD)(li.QuadPart * frequency / hfFrequency);
}

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    syncCallback();
}

static void CALLBACK timerCallback(unsigned int unused1,
                                   unsigned int unused2,
                                   unsigned long unused3,
                                   unsigned long unused4,
                                   unsigned long unused5)
{
    syncCallback();
}

void* archCreateTimer(int period, void (*callback)(void*)) {
    Win32Timer* win32Timer = (Win32Timer*)malloc(sizeof(Win32Timer));

    win32Timer->timerId = 0;
    
    win32Timer->timerId = timeSetEvent(period, period, timerCallback, 0, TIME_PERIODIC);

    if (win32Timer->timerId == 0) {
        SetTimer(getMainHwnd(), 0, period, TimerProc);
    }

    timerCb = callback;
    timerId = win32Timer;

    return win32Timer;
}

void archTimerDestroy(void* timer) 
{
    Win32Timer* win32Timer = (Win32Timer*)timer;

    if (win32Timer == NULL) {
        return;
    }

    if (win32Timer->timerId != 0) {
        timeKillEvent(win32Timer->timerId);
    }
    else {
        KillTimer(getMainHwnd(), 0);
    }

    free(win32Timer);
}



static unsigned long long last[RDTSC_MAX_TIMERS];

void rdtsc_start_timer (int timer) {
	unsigned int a,b; 
   __asm (
		"rdtsc         \n"
		"mov %0,edx     \n"
		"mov %1,eax     \n"
   :"=r"(a),"=r"(b)
	);
	last[timer]=(((unsigned long long int)a)<<32)+((unsigned long long int)b);
}

static unsigned long long int rdtsc_queue[RDTSC_MAX_TIMERS][30]={
    0,0,0,0, 0,0,0,0, 0,0,
    0,0,0,0, 0,0,0,0, 0,0,
    0,0,0,0, 0,0,0,0, 0,0
};

void rdtsc_end_timer (int timer) {
	unsigned int a,b,i; 
	unsigned long long int c;
   __asm (
		"rdtsc         \n"
		"mov %0,edx     \n"
		"mov %1,eax     \n"
   :"=r"(a),"=r"(b)
	);
	c=((((unsigned long long int)a)<<32)+((unsigned long long int)b))-last[timer];
	for (i=0; i<29; i++)
	  rdtsc_queue[timer][i]=rdtsc_queue[timer][i+1];
    rdtsc_queue[timer][29]=c;
}

unsigned long long int rdtsc_get_timer (int timer) {
  unsigned long long int average=0;
  int i;

  for (i=0; i<30; i++)
	 average+=rdtsc_queue[timer][i];
  return average;
}

/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Timer.c,v $
**
** $Revision: 1.10 $
**
** $Date: 2008-04-05 18:47:11 $
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
#include "Win32Common.h"
#include <windows.h>

typedef struct {
    MMRESULT timerId;
} Win32Timer;

static void (*timerCb)(void*) = NULL;
static void* timerId = NULL;

static LONGLONG uptime_hfFrequency = 0;
static signed long long uptime_offset = 0;

LONGLONG win32timer_get_uptime_freq(void) { return uptime_hfFrequency; }
void win32timer_uptime_offset(int d) { uptime_offset+=d; }

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
    LARGE_INTEGER li;

    if (!uptime_hfFrequency) {
        if (QueryPerformanceFrequency(&li)) {
            uptime_hfFrequency = li.QuadPart;
        }
        else {
            return 0;
        }
    }

    QueryPerformanceCounter(&li);

    return (DWORD)((li.QuadPart + uptime_offset) * frequency / uptime_hfFrequency);
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

void* archCreateTimer(int period, int (*callback)(void*)) 
{
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
#ifdef __GNUC__
	__asm__ __volatile__("rdtsc" : "=A" (last[timer]));
#else
	unsigned int a,b; 
	__asm { 
		rdtsc
		mov a,edx
		mov b,eax
	}
	last[timer]=(((unsigned long long int)a)<<32)+((unsigned long long int)b);
#endif
}

static unsigned long long int rdtsc_queue[RDTSC_MAX_TIMERS][30]={
    0,0,0,0, 0,0,0,0, 0,0,
    0,0,0,0, 0,0,0,0, 0,0,
    0,0,0,0, 0,0,0,0, 0,0
};

void rdtsc_end_timer (int timer) {
	unsigned int a,b,i; 
	unsigned long long int c;

#ifdef __GNUC__
	__asm__ __volatile__("rdtsc" : "=A" (c));
#else
	__asm { 
		rdtsc
		mov a,edx
		mov b,eax
	}
	c=((((unsigned long long int)a)<<32)+((unsigned long long int)b))-last[timer];
#endif

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

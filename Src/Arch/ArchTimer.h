/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Arch/ArchTimer.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/10/04 23:03:33 $
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
#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include "MsxTypes.h"

UInt32 archGetSystemUpTime(UInt32 frequency);
void* archCreateTimer(int period, void (*timerCallback)(void*));
void archTimerDestroy(void* timer);
UInt32 archGetHiresTimer();

#define RDTSC_MAX_TIMERS 5

void rdtsc_start_timer (int timer) ;
void rdtsc_end_timer (int timer);
unsigned long long int rdtsc_get_timer (int timer) ;

#endif

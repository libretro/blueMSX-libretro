/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Event.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2008-03-30 18:38:48 $
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
#include "ArchEvent.h"
#include <windows.h>

void* archEventCreate(int initState) {
    return CreateEvent(NULL, 0, initState, NULL);
}

void archEventDestroy(void* event)
{
    CloseHandle(event);
}

void archEventSet(void* event)
{
    SetEvent(event);
}

void archEventWait(void* event, int timeout)
{
    if (timeout < 0) {
        timeout = INFINITE;
    }

    WaitForSingleObject(event, timeout);
}


void* archSemaphoreCreate(int initCount)
{
    return CreateSemaphore(NULL, initCount, 100, NULL);
}

void archSemaphoreDestroy(void* semaphore)
{
    CloseHandle(semaphore);
}

void archSemaphoreSignal(void* semaphore)
{
    ReleaseSemaphore(semaphore, 1, NULL);
}

void archSemaphoreWait(void* semaphore, int timeout)
{
    archEventWait(semaphore, timeout);
}


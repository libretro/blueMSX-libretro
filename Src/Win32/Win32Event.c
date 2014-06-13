/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Event.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/01/18 10:17:20 $
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


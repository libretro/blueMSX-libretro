/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Sdl/SdlEvent.c,v $
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
#include "ArchEvent.h"
#include <stdlib.h>

#ifdef SINGLE_THREADED

// No semaphores needed in single threaded version
void* archEventCreate(int initState) { return NULL; }
void archEventDestroy(void* event) {}
void archEventSet(void* event) {}
void archEventWait(void* event, int timeout) {}

#else
#error
#endif

#if 1
#include <semaphore.h>

typedef struct {
    sem_t semaphore;
} Semaphore;

void* archSemaphoreCreate(int initCount)
{
    Semaphore* s = (Semaphore*)malloc(sizeof(Semaphore));

    sem_init(&s->semaphore, 0, initCount);

    return s;
}

void archSemaphoreDestroy(void* semaphore)
{
    Semaphore* s = (Semaphore*)semaphore;

    sem_destroy(&s->semaphore);
    free(s);
}

void archSemaphoreSignal(void* semaphore)
{
    Semaphore* s = (Semaphore*)semaphore;

    sem_post(&s->semaphore);
}

void archSemaphoreWait(void* semaphore, int timeout)
{
    Semaphore* s = (Semaphore*)semaphore;

    while (-1 == sem_wait(&s->semaphore));
}

#endif

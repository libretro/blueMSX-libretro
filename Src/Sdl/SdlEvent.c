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
#include <SDL.h>
#include <stdlib.h>

#ifdef SINGLE_THREADED

// No semaphores needed in single threaded version
void* archEventCreate(int initState) { return NULL; }
void archEventDestroy(void* event) {}
void archEventSet(void* event) {}
void archEventWait(void* event, int timeout) {}

#else

typedef struct {
    void* eventSem;
    void* lockSem;
    int   state;
} Event;

void* archEventCreate(int initState) 
{ 
    Event* e = calloc(1, sizeof(Event));
    e->state = initState ? 1 : 0;
    e->lockSem  = archSemaphoreCreate(1);
    e->eventSem  = archSemaphoreCreate(e->state);
    return e; 
}

void archEventDestroy(void* event) 
{
    Event* e = (Event*)event;
    archSemaphoreDestroy(e->lockSem);
    archSemaphoreDestroy(e->eventSem);
    free(e);
}

void archEventSet(void* event) 
{
    Event* e = (Event*)event;
    if (e->state == 0) {
        e->state = 1;
        archSemaphoreSignal(e->eventSem);
    }
}

void archEventWait(void* event, int timeout) 
{
    Event* e = (Event*)event;
    archSemaphoreWait(e->eventSem, timeout);
    e->state = 0;
}

#endif


typedef struct {
    SDL_sem* semaphore;
} Semaphore;

void* archSemaphoreCreate(int initCount) 
{ 
    Semaphore* s = (Semaphore*)malloc(sizeof(Semaphore));

    s->semaphore = SDL_CreateSemaphore(initCount);

    return s;
}

void archSemaphoreDestroy(void* semaphore) 
{
    Semaphore* s = (Semaphore*)semaphore;

    SDL_DestroySemaphore(s->semaphore);
    free(s);
}

void archSemaphoreSignal(void* semaphore) 
{
    Semaphore* s = (Semaphore*)semaphore;

    SDL_SemPost(s->semaphore);
}

void archSemaphoreWait(void* semaphore, int timeout) 
{
    Semaphore* s = (Semaphore*)semaphore;

    while (-1 == SDL_SemWait(s->semaphore));
}

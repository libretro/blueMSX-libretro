/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Sdl/SdlThread.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2007-03-21 22:26:25 $
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
#include "ArchThread.h"
#include <SDL.h>
#include <stdlib.h>

static int threadEntry(void* data) 
{
    void (*entryPoint)() = data;

    entryPoint();
    
    return 0;
}


void* archThreadCreate(void (*entryPoint)(), int priority) { 
    SDL_Thread* sdlThread = SDL_CreateThread(threadEntry, entryPoint);
    return sdlThread;
}

void archThreadJoin(void* thread, int timeout) 
{
    SDL_Thread* sdlThread = (SDL_Thread*)thread;

    SDL_WaitThread(sdlThread, NULL);
}

void  archThreadDestroy(void* thread) 
{
    SDL_Thread* sdlThread = (SDL_Thread*)thread;

    SDL_KillThread(sdlThread);
}

void archThreadSleep(int milliseconds) 
{
    SDL_Delay(milliseconds);
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Sdl/SdlThread.c,v $
**
** $Revision: 1.3 $
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
#include "ArchThread.h"
#include "SDL/SDL.h"
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

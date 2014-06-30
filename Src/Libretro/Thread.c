/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Linux/blueMSXlite/LinuxThread.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/18 07:55:10 $
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
#ifndef PSP

#include "ArchThread.h"
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include "thread.h"


static void* threadEntry(void* data) 
{
    void (*entryPoint)() = data;
#if 0
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif
    entryPoint();
    
    pthread_exit(NULL);

    return NULL;
}


void* archThreadCreate(void (*entryPoint)(), int priority) { 
    int rv;
    pthread_t* thread;
    pthread_attr_t attr;
    static int threadsCreated = 0;
    size_t size;
    rv = pthread_attr_init(&attr);

    pthread_attr_setstacksize(&attr, 65536);

    thread = (pthread_t*)malloc(sizeof(pthread_t));

    // TODO: Fix priorities

    do {
	    rv = pthread_create(thread, &attr , threadEntry, entryPoint);
    } while (rv == EAGAIN);

	if(rv != 0) {
        free(thread);
        thread = NULL;
    }

    pthread_attr_destroy(&attr);

    return thread;
}

void archThreadJoin(void* thread, int timeout) 
{
    int rv;

    do {
        void* threadRet;
        rv = pthread_join(*(pthread_t*)thread, &threadRet);
    } while(rv != 0);
}

void archThreadDestroy(void* thread)
{
    if((pthread_t)thread == pthread_self()) {
        pthread_exit(NULL);
    }
    else {
        pthread_cancel(*(pthread_t*)thread);
    }

    free(thread);
}


void archThreadSleep(int milliseconds)
{
   retro_sleep(milliseconds);
}

#else

#include <pspthreadman.h>

typedef void (*entryPoint_type)();

static int thread_entry(SceSize args, void* argp)
{
   entryPoint_type entryPoint = *((entryPoint_type*)argp);

   entryPoint();

   return 0;
}


void* archThreadCreate(void (*entryPoint)(), int priority) {

   SceUID thread_uid = sceKernelCreateThread ("CPU thread", thread_entry, 0x10, 0x10000, 0, NULL);


   sceKernelStartThread(thread_uid, sizeof(entryPoint_type), &thread_entry);

   return (void*)thread_uid;

}

void archThreadJoin(void* thread, int timeout)
{
   SceUID thread_uid = (SceUID)thread;
   SceUInt sce_timeout = (SceUInt)timeout;

   sceKernelTerminateThread(thread_uid);
   sceKernelWaitThreadEnd(thread_uid, &sce_timeout);
}

void archThreadDestroy(void* thread)
{
   SceUID thread_uid = (SceUID)thread;
   sceKernelDeleteThread((SceUID)thread);
}


void archThreadSleep(int milliseconds)
{
   sceKernelDelayThread(1000 * milliseconds);
}

#endif

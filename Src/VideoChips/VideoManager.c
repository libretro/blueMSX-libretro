/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/VideoManager.c,v $
**
** $Revision: 1.12 $
**
** $Date: 2006/06/16 22:52:10 $
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
#include "VideoManager.h"
#include "ArchNotifications.h"
#include "SaveState.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_DEVICES 64

typedef struct {
    int handle;
    VideoCallbacks callbacks;
    FrameBufferData* frameBufer;
    void* ref;
    VideoMode videoMode;
    VideoMode videoMask;
    char  name[32];
} VideoInfo;

typedef struct {
    VideoInfo di[MAX_DEVICES];
    int count;
    int lastHandle;
} VideoManager;

static VideoManager videoManager;

void videoManagerReset() 
{
    videoManager.count = 0;
    videoManager.lastHandle = 0;
}

int videoManagerGetCount() 
{
    return videoManager.count;
}

int videoManagerIsActive(int index) 
{
    if (index >= videoManager.count) {
        return 0;
    }

    return videoManager.di[index].frameBufer == frameBufferGetActive();
}

int videoManagerGetActive()
{
    int index;
    for (index = 0; index < videoManager.count; index++) {
        if (videoManagerIsActive(index)) {
            return index;
        }
    }
    return -1;
}

void videoManagerSetMode(int index, VideoMode videoMode, VideoMode modeMask)
{
    videoManager.di[index].videoMode = videoMode;
    videoManager.di[index].videoMask = modeMask;
    if (videoManagerIsActive(index)) {
        frameBufferSetMixMode(videoManager.di[index].videoMode, videoManager.di[index].videoMask);
    }
}

void videoManagerSetActive(int index)
{
    int activeIndex;

    if (videoManager.count <= 0) {
        frameBufferSetActive(NULL);
        return;
    }

    if (index >= videoManager.count) {
        return;
    }

    for (activeIndex = 0; activeIndex < videoManager.count; activeIndex++) {
        if (videoManagerIsActive(activeIndex)) {
            if (activeIndex != index && videoManager.di[activeIndex].callbacks.disable != NULL) {
                videoManager.di[activeIndex].callbacks.disable(videoManager.di[activeIndex].ref);
            }
        }
    }

    if (index < 0) {
        frameBufferSetActive(NULL);
    }
    else {
        frameBufferSetActive(videoManager.di[index].frameBufer);
        frameBufferSetMixMode(videoManager.di[index].videoMode, videoManager.di[index].videoMask);
        if (activeIndex != index && videoManager.di[index].callbacks.enable != NULL) {
            videoManager.di[index].callbacks.enable(videoManager.di[index].ref);
        }
    }
}

char* videoManagerGetName(int index)
{
    if (index >= videoManager.count) {
        return index == 0 ? "Default" : NULL;
    }
    return videoManager.di[index].name;
}

int videoManagerRegister(char* name, FrameBufferData* frameBuffer, 
                         VideoCallbacks* callbacks, void* ref)
{
    if (videoManager.count >= MAX_DEVICES) {
        return 0;
    }

    videoManager.di[videoManager.count].handle      = ++videoManager.lastHandle;
    videoManager.di[videoManager.count].frameBufer  = frameBuffer;
    videoManager.di[videoManager.count].callbacks   = *callbacks;
    videoManager.di[videoManager.count].ref         = ref;
    videoManager.di[videoManager.count].videoMode   = VIDEO_INTERNAL;
    videoManager.di[videoManager.count].videoMask   = VIDEO_INTERNAL;

    strcpy(videoManager.di[videoManager.count].name, name);

    videoManager.count++;

    if (videoManager.count == 1) {
        videoManagerSetActive(0);
    }
    
    archVideoOutputChange();

    return videoManager.lastHandle - 1;
}

void videoManagerUnregister(int handle)
{
    int isActive;
    int i;

    if (videoManager.count == 0) {
        return;
    }

    for (i = 0; i < videoManager.count; i++) {
        if (videoManager.di[i].handle == handle + 1) {
            break;
        }
    }

    if (i == videoManager.count) {
        return;
    }

    isActive = videoManagerIsActive(i);

    videoManager.count--;
    while (i < videoManager.count) {
        videoManager.di[i] = videoManager.di[i + 1];
        i++;
    }

    if (isActive || videoManager.count == 0) {
        videoManagerSetActive(0);
    }

    if (videoManager.count == 0) {
        frameBufferClearDeinterlace();
    }

    archVideoOutputChange();
}


void videoManagerLoadState()
{
    SaveState* state = saveStateOpenForRead("VideoManager");
    int index = saveStateGet(state, "ActiveFrameBuffer",  0);
    saveStateClose(state);
    videoManagerSetActive(index);

}

void videoManagerSaveState()
{
    SaveState* state = saveStateOpenForWrite("VideoManager");
    saveStateSet(state, "ActiveFrameBuffer",  videoManagerGetActive());
    saveStateClose(state);
}

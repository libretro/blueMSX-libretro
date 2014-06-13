/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32VideoIn.cpp,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/07/03 19:25:45 $
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
extern "C" {
#include "Win32VideoIn.h"
#include "ArchVideoIn.h"
#include "Language.h"
}

#include "Win32DirectShow.h"

typedef struct {
    UInt16* buffer;
    int     width;
    int     height;

    int inputIndex;
    int inputCount;

    CVideoGrabber* grabber;
} VideoIn;

static VideoIn videoIn;


int archVideoInIsVideoConnected()
{
    return videoIn.inputIndex > 0;
}

UInt16* archVideoInBufferGet(int width, int height)
{
    if (videoIn.inputIndex == 0) {
        return NULL;
    }

    // Allocate new video buffer if size has changed
    if (width != videoIn.width || height != videoIn.height) {
        if (videoIn.buffer != NULL) {
            free(videoIn.buffer);
        }
        videoIn.buffer = (UInt16*)calloc(1, sizeof(UInt16) * width * height);
        videoIn.width  = width;
        videoIn.height = height;
    }

    videoIn.grabber->GrabFrame(videoIn.buffer, width, height);

    return videoIn.buffer;
}

void videoInInitialize(Properties* properties)
{
    videoIn.inputCount = 1;
    videoIn.inputIndex = properties->videoIn.inputIndex;

    if (!properties->videoIn.disabled) {
        videoIn.grabber = new CVideoGrabber;
        
        if (videoIn.grabber->SetupGrabber()) {
            videoIn.inputCount++;

            if (videoIn.inputIndex == 0) {
    //            videoIn.grabber->ShutdownGrabber();
            }
        }
    }

    if (videoIn.inputIndex >= videoIn.inputCount) {
        videoIn.inputIndex = 0;
    }
}

void videoInCleanup(Properties* properties)
{
    if (videoIn.inputCount > 1) {
        videoIn.grabber->ShutdownGrabber();
    }

    if (videoIn.grabber != NULL) {
        delete videoIn.grabber;
    }

    properties->videoIn.inputIndex = videoIn.inputIndex;
}

int  videoInGetCount()
{
    return videoIn.inputCount;
}

int videoInIsActive(int index)
{
    return index == videoIn.inputIndex;
}

int videoInGetActive()
{
    return videoIn.inputIndex;
}

char* videoInGetName(int index)
{
    // Should return a list of input devices
    // First should be 'None', second, 'Use Bitmap File'
    // Others should be Real input devices
    // The list probably needs updating everytime
    // videoInGetInputCount is called (which is
    // from the menu
    switch (index) {
        case 0: return langTextNone();
        case 1: return videoIn.grabber->GetName();
    }

    return langTextUnknown();
}

void videoInSetActive(int index)
{
    if (index >= videoIn.inputCount) {
        return;
    }
    if (videoIn.inputIndex != index) {
//        switch (index) {
//        case 0: videoIn.grabber->ShutdownGrabber(); break;
//        case 1: videoIn.grabber->SetupGrabber(); break;
//        }
    }
    videoIn.inputIndex = index;
}

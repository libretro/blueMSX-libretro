/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32VideoIn.cpp,v $
**
** $Revision: 1.9 $
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
    int disabled;

    CVideoGrabber* grabber;
    
    CVideoGrabber::DeviceNameList devList;
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
    videoIn.inputIndex = 0;

    videoIn.disabled = properties->videoIn.disabled;

    if (videoIn.disabled) {
        return;
    }

    videoIn.grabber = new CVideoGrabber;
    
    videoIn.devList = videoIn.grabber->GetDeviceNames();

    videoIn.inputCount += videoIn.devList.size();

    videoIn.inputIndex = properties->videoIn.inputIndex;

    videoInSetActive(videoIn.inputIndex);
}

void videoInCleanup(Properties* properties)
{
    if (videoIn.disabled) {
        return;
    }

    videoIn.grabber->ShutdownGrabber();
    delete videoIn.grabber;

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

const char* videoInGetName(int index)
{
    if (index == 0) {
        return langTextNone();
    }
    
    if (index < videoIn.inputCount) {
        return videoIn.devList[index - 1].c_str();
    }

    return langTextUnknown();
}

void videoInSetActive(int index)
{
    bool showdialog;

    if (videoIn.disabled) {
        return;
    }

    videoIn.grabber->ShutdownGrabber();

    showdialog = (videoIn.inputIndex != index);
    videoIn.inputIndex = index;

    if (videoIn.inputIndex > videoIn.inputCount) {
        videoIn.inputIndex = 0;
    }

    if (videoIn.inputIndex > 0) {
        if (showdialog) {
            videoIn.grabber->ShowProperties(NULL, videoIn.devList[videoIn.inputIndex - 1]);
        }

        if (!videoIn.grabber->SetupGrabber(videoIn.devList[videoIn.inputIndex - 1])) {
            videoIn.inputIndex = 0;
        }
    }
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/FrameBuffer.h,v $
**
** $Revision: 1.21 $
**
** $Date: 2006/06/16 05:46:46 $
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
#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "MsxTypes.h"

#define FB_MAX_LINE_WIDTH 640
#define FB_MAX_LINES      480

typedef struct {
    int doubleWidth; // 1 when normal, 2 when 2 src pixels per dest pixel
    UInt16 buffer[FB_MAX_LINE_WIDTH];
} LineBuffer;

typedef enum { INTERLACE_NONE, INTERLACE_ODD, INTERLACE_EVEN } InterlaceMode;
typedef struct {
    int age;           // Internal use
    InterlaceMode interlace; 
    int maxWidth;
    int lines;         // Number of lines in frame buffer
    LineBuffer line[FB_MAX_LINES];
} FrameBuffer;

typedef struct FrameBufferData FrameBufferData;

typedef enum {
    MIXMODE_INTERNAL = 1,
    MIXMODE_BOTH     = 2,
    MIXMODE_EXTERNAL = 4,
    MIXMODE_NONE     = 8
} FrameBufferMixMode;

void frameBufferSetFrameCount(int frameCount);

FrameBuffer* frameBufferGetViewFrame();
FrameBuffer* frameBufferGetDrawFrame();
FrameBuffer* frameBufferFlipViewFrame(int mixFrames);
FrameBuffer* frameBufferFlipDrawFrame();

void frameBufferSetScanline(int scanline);
int frameBufferGetScanline();

FrameBuffer* frameBufferGetWhiteNoiseFrame();
FrameBuffer* frameBufferDeinterlace(FrameBuffer* frameBuffer);
void frameBufferClearDeinterlace();

FrameBufferData* frameBufferDataCreate(int maxWidth, int maxHeight, int defaultHorizZoom);
void frameBufferDataDestroy(FrameBufferData* frameData);

void frameBufferSetActive(FrameBufferData* frameData);
void frameBufferSetMixMode(FrameBufferMixMode mode, FrameBufferMixMode mask);

FrameBufferData* frameBufferGetActive();

void frameBufferSetBlendFrames(int blendFrames);

#define videoGetTransparentColor() 0x8000

static UInt16 videoGetColor(int R, int G, int B)
{
    return ((R >> 3) << 10) | ((G >> 3) << 5) | (B >> 3);
}

#endif

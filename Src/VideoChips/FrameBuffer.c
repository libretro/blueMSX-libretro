/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/FrameBuffer.c,v $
**
** $Revision: 1.29 $
**
** $Date: 2006/06/29 05:42:41 $
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
#include "FrameBuffer.h"
#include "ArchEvent.h"
#include "ArchVideoIn.h"
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES_PER_FRAMEBUFFER 4

struct FrameBufferData {
    int viewFrame;
    int drawFrame;
    int currentAge;
    int currentBlendFrame;
    FrameBuffer frame[MAX_FRAMES_PER_FRAMEBUFFER];
    FrameBuffer blendFrame[2];
};

static int curScanline = 0;
static void* semaphore = NULL;
static FrameBuffer* deintBuffer = NULL;
static int confBlendFrames = 0;

static FrameBuffer* mixFrame(FrameBuffer* d, FrameBuffer* a, FrameBuffer* b, int pct);
static FrameBuffer* mixFrameInterlace(FrameBuffer* d, FrameBuffer* a, FrameBuffer* b, int pct);
static void frameBufferSuperimpose(FrameBuffer* a);
static void frameBufferExternal(FrameBuffer* a);
static void frameBufferBlack(FrameBuffer* a);
extern int getScreenCompletePercent();

static void waitSem() {
    if (semaphore == NULL) {
        semaphore = archSemaphoreCreate(1);
    }
    archSemaphoreWait(semaphore, -1);
}

static void signalSem() {
    archSemaphoreSignal(semaphore);
}



static FrameBufferData* currentBuffer = NULL;
static FrameBufferMixMode mixMode = MIXMODE_INTERNAL;
static FrameBufferMixMode mixMask = MIXMODE_INTERNAL;
static int frameBufferCount = MAX_FRAMES_PER_FRAMEBUFFER;


static FrameBuffer* frameBufferFlipViewFrame1(int mixFrames) 
{
    if (currentBuffer == NULL) {
        return NULL;
    }
    currentBuffer->frame->age = ++currentBuffer->currentAge;
    return currentBuffer->frame;
}

static FrameBuffer* frameBufferFlipViewFrame3(int mixFrames) 
{
    int index;

    if (currentBuffer == NULL) {
        return NULL;
    }
    waitSem();
    switch (currentBuffer->viewFrame) {
    case 0: index = currentBuffer->drawFrame == 1 ? 2 : 1; break;
    case 1: index = currentBuffer->drawFrame == 2 ? 0 : 2; break;
    case 2: index = currentBuffer->drawFrame == 0 ? 1 : 0; break;
    }
    if (currentBuffer->frame[index].age > currentBuffer->frame[currentBuffer->viewFrame].age) {
        currentBuffer->viewFrame = index;
    }
    signalSem();
    return currentBuffer->frame + currentBuffer->viewFrame;
}

static FrameBuffer* frameBufferFlipViewFrame4(int mixFrames) 
{
    int i;

    if (currentBuffer == NULL) {
        return NULL;
    }
    waitSem();
    for (i = 0; i < 4; i++) {
        if (i == currentBuffer->drawFrame) continue;
        if (currentBuffer->frame[i].age > currentBuffer->frame[currentBuffer->viewFrame].age) {
            currentBuffer->viewFrame = i;
        }
    }
        
    if (mixFrames) {
        int secondFrame = 0;
        int viewAge = 0;
        for (i = 0; i < 4; i++) {
            if (i == currentBuffer->drawFrame) continue;
            if (i == currentBuffer->viewFrame) continue;
            if (currentBuffer->frame[i].age > viewAge) {
                viewAge = currentBuffer->frame[i].age;
                secondFrame = i;
            }
        }
        signalSem();

        return mixFrame(NULL, currentBuffer->frame + currentBuffer->viewFrame, currentBuffer->frame + secondFrame, getScreenCompletePercent());
    }

    signalSem();
    return currentBuffer->frame + currentBuffer->viewFrame;
}


static FrameBuffer* frameBufferFlipDrawFrame1()
{
    if (currentBuffer == NULL) {
        return NULL;
    }
    return currentBuffer->frame; 
}

static FrameBuffer* frameBufferFlipDrawFrame3()
{
    FrameBuffer* frame;

    if (currentBuffer == NULL) {
        return NULL;
    }
    waitSem();
    switch (currentBuffer->drawFrame) {
    case 0: currentBuffer->drawFrame = currentBuffer->viewFrame == 1 ? 2 : 1; break;
    case 1: currentBuffer->drawFrame = currentBuffer->viewFrame == 2 ? 0 : 2; break;
    case 2: currentBuffer->drawFrame = currentBuffer->viewFrame == 0 ? 1 : 0; break;
    }
    frame = currentBuffer->frame + currentBuffer->drawFrame;
    frame->age = ++currentBuffer->currentAge;
    signalSem();
    return frame;
}

static FrameBuffer* frameBufferFlipDrawFrame4()
{
    FrameBuffer* frame;
    int drawFrame = currentBuffer->drawFrame;
    int drawAge = 0x7fffffff;
    int i;

    if (currentBuffer == NULL) {
        return NULL;
    }

    waitSem();

    for (i = 0; i < 4; i++) {
        if (i == currentBuffer->viewFrame) continue;
        if (currentBuffer->frame[i].age < drawAge) {
            drawAge = currentBuffer->frame[i].age;
            currentBuffer->drawFrame = i;
        }
    }

    signalSem();
    frame = currentBuffer->frame + currentBuffer->drawFrame;
    frame->age = ++currentBuffer->currentAge;
    return frame;
}



FrameBuffer* frameBufferGetViewFrame()
{
    return currentBuffer ? currentBuffer->frame + currentBuffer->viewFrame : NULL;
}

void frameBufferSetScanline(int scanline)
{
    curScanline = scanline;
}

int frameBufferGetScanline()
{
    return curScanline;
}

FrameBuffer* frameBufferGetDrawFrame()
{
    FrameBuffer* frameBuffer;

    if (currentBuffer == NULL) {
        return NULL;
    }
    
    if (confBlendFrames) {
        frameBuffer = currentBuffer->blendFrame + currentBuffer->currentBlendFrame;
    }
    else {
        frameBuffer = currentBuffer->frame + currentBuffer->drawFrame;
    }

    return frameBuffer;
}

void frameBufferSetFrameCount(int frameCount)
{
    waitSem();
    frameBufferCount = frameCount;
    if (currentBuffer != NULL) {
        int i;
        currentBuffer->viewFrame = 0;
        currentBuffer->drawFrame = 0;

        for (i = 0; i < MAX_FRAMES_PER_FRAMEBUFFER; i++) {
            currentBuffer->frame[i].age = 0;
        }
    }
    signalSem();
}

FrameBuffer* frameBufferFlipViewFrame(int mixFrames) 
{
    FrameBuffer* frameBuffer;

    if (currentBuffer == NULL) {
        return NULL;
    }

    switch (frameBufferCount) {
    case 3:
        frameBuffer = frameBufferFlipViewFrame3(mixFrames);
        break;
    case 4:
        frameBuffer = frameBufferFlipViewFrame4(mixFrames);
        break;
    default:
        frameBuffer = frameBufferFlipViewFrame1(mixFrames);
        break;
    }
    return frameBuffer;
}

FrameBuffer* frameBufferFlipDrawFrame() 
{
    FrameBuffer* frameBuffer;

    if (currentBuffer == NULL) {
        return NULL;
    }

    if (confBlendFrames) {
        mixFrame(currentBuffer->frame + currentBuffer->drawFrame, 
                 &currentBuffer->blendFrame[0], &currentBuffer->blendFrame[1], 50);
    }

    curScanline = 0;

    if (mixMode == MIXMODE_EXTERNAL) {
        frameBufferExternal(currentBuffer->frame + currentBuffer->drawFrame);
    }
    else if (mixMode == MIXMODE_BOTH) {
        frameBufferSuperimpose(currentBuffer->frame + currentBuffer->drawFrame);
    }
    else if (mixMode == MIXMODE_NONE) {
        frameBufferBlack(currentBuffer->frame + currentBuffer->drawFrame);
    }

    switch (frameBufferCount) {
    case 3:
        frameBuffer = frameBufferFlipDrawFrame3();
        break;
    case 4:
        frameBuffer = frameBufferFlipDrawFrame4();
        break;
    default:
        frameBuffer = frameBufferFlipDrawFrame1();
        break;
    }
    
    if (confBlendFrames) {
        currentBuffer->currentBlendFrame ^= 1;
        frameBuffer = currentBuffer->blendFrame + currentBuffer->currentBlendFrame;
    }

    return frameBuffer;
}

void frameBufferSetBlendFrames(int blendFrames)
{
    confBlendFrames = blendFrames;
}

FrameBufferData* frameBufferDataCreate(int maxWidth, int maxHeight, int defaultHorizZoom)
{
    int i;
    FrameBufferData* frameData = calloc(1, sizeof(FrameBufferData));
    frameData->drawFrame = frameBufferCount > 1 ? 1 : 0;

    for (i = 0; i < MAX_FRAMES_PER_FRAMEBUFFER; i++) {
        int j;

        frameData->frame[i].maxWidth = maxWidth;
        frameData->frame[i].lines = maxHeight;
        for (j = 0; j < FB_MAX_LINES; j++) {
            frameData->frame[i].line[j].doubleWidth = defaultHorizZoom - 1;
        }
    }

    for (i = 0; i < 2; i++) {
        int j;

        frameData->blendFrame[i].maxWidth = maxWidth;
        frameData->blendFrame[i].lines = maxHeight;
        for (j = 0; j < FB_MAX_LINES; j++) {
            frameData->blendFrame[i].line[j].doubleWidth = defaultHorizZoom - 1;
        }
    }

    return frameData;
}

void frameBufferDataDestroy(FrameBufferData* frameData)
{
    if (frameData == currentBuffer) {
        currentBuffer = NULL;
    }
    free(frameData);
}

void frameBufferSetActive(FrameBufferData* frameData)
{
    currentBuffer = frameData;
    if (frameData == NULL) {
        mixMode = MIXMODE_INTERNAL;
    }
}

void frameBufferSetMixMode(FrameBufferMixMode mode,  FrameBufferMixMode mask)
{
    mixMode = mode;
    mixMask = mask;
}

FrameBufferData* frameBufferGetActive() 
{
    return currentBuffer;
}

FrameBuffer* frameBufferGetWhiteNoiseFrame() 
{
    static FrameBuffer* frameBuffer = NULL;
    UInt16 colors[32];
    static UInt32 r = 13;
    int y;

    for (y = 0; y < 32; y++) {
        colors[y] = videoGetColor(y << 3, y << 3, y << 3);
    }

    if (frameBuffer == NULL) {
        frameBuffer = calloc(1, sizeof(FrameBuffer));
        frameBuffer->lines = 240;
        frameBuffer->maxWidth = 320;
    }

    for (y = 0; y < 240; y++) {
        int x;
        UInt16* buffer = frameBuffer->line[y].buffer;
        frameBuffer->line[y].doubleWidth = 0;
        for (x = 0; x < 320; x++) {
            buffer[x] = colors[r >> 27];
            r *= 7;
        }
    }

    return frameBuffer;
}

void frameBufferClearDeinterlace()
{
    if (deintBuffer != NULL) {
        void* buf = deintBuffer;
        deintBuffer = NULL;
        free(buf);
    }
}

FrameBuffer* frameBufferDeinterlace(FrameBuffer* frameBuffer)
{
    int y;

    if (deintBuffer == NULL) {
        deintBuffer = calloc(1, sizeof(FrameBuffer));
    }
    deintBuffer->lines = frameBuffer->lines < FB_MAX_LINES / 2 ? 2 * frameBuffer->lines : FB_MAX_LINES;
    deintBuffer->maxWidth = frameBuffer->maxWidth;

    for (y = frameBuffer->interlace == INTERLACE_ODD ? 1 : 0; y < FB_MAX_LINES; y += 2) {
        deintBuffer->line[y] = frameBuffer->line[y / 2];
    }
    return deintBuffer;
}


#define M1 0x3E07C1F
#define M2 0x3E0F81F

static FrameBuffer* mixFrame(FrameBuffer* d, FrameBuffer* a, FrameBuffer* b, int pct)
{
    static FrameBuffer* dst = NULL;
    int p = 0x20 * pct / 100;
    int n = 0x20 - p;
    int x;
    int y;

    if (d == NULL) {
        if (dst == NULL) {
            dst = (FrameBuffer*)malloc(sizeof(FrameBuffer)); 
        }
        d = dst;
    }

    if (a->interlace != INTERLACE_NONE) {
        return mixFrameInterlace(d, a, b, pct);
    }

    if (p == 0x20) {
        return a;
    }
    if (n == 0x20) {
        return b;
    }

    d->lines = a->lines;
    d->interlace = a->interlace;
    d->maxWidth = a->maxWidth;

    for (y = 0; y < a->lines; y++) {
        int width = a->line[y].doubleWidth ? a->maxWidth: a->maxWidth / 2;
        UInt32* ap = (UInt32*)a->line[y].buffer;
        UInt32* bp = (UInt32*)b->line[y].buffer;
        UInt32* dp = (UInt32*)d->line[y].buffer;

        d->line[y].doubleWidth = a->line[y].doubleWidth;
        for (x = 0; x < width; x ++) {
            UInt32 av = ap[x];
            UInt32 bv = bp[x];
            dp[x] = ((((av & M1) * p + (bv & M1) * n) >> 5) & M1) |
                    ((((((av >> 5) & M2) * p + ((bv >> 5) & M2) * n) >> 5) & M2) << 5) |
                    (av & 0x80008000);
        }
    }

    return d;
}


static FrameBuffer* mixFrameInterlace(FrameBuffer* d, FrameBuffer* a, FrameBuffer* b, int pct)
{
    static FrameBuffer* dst = NULL;
    int p = 0x20 * pct / 100;
    int n = 0x20 - p;
    int x;
    int y;

    if (d == NULL) {
        if (dst == NULL) {
            dst = (FrameBuffer*)malloc(sizeof(FrameBuffer)); 
        }
        d = dst;
    }

    d->lines     = a->lines * 2;
    d->interlace = INTERLACE_NONE;
    d->maxWidth  = a->maxWidth;

    if (p == 0x20) { p = 0x1f; n = 1; }
    if (n == 0x20) { n = 0x1f; p = 1; }

    if (a->interlace == INTERLACE_ODD) {
        FrameBuffer* t;
        int t2;

        t = a;
        a = b;
        b = t;

        t2 = n;
        n = p;
        p = t2;
    }

    for (y = 0; y < a->lines * 2; y++) {
        int     width = a->line[y / 2].doubleWidth ? a->maxWidth: a->maxWidth / 2;
        UInt32* ap;
        UInt32* bp;
        UInt32* dp;
        
        if (y & 1) {
            ap = (UInt32*)a->line[y / 2].buffer;
            bp = (UInt32*)b->line[y / 2].buffer;
        }
        else {
            ap = (UInt32*)a->line[y / 2].buffer;
            if (y == 0) {
                bp = (UInt32*)b->line[a->lines - 1].buffer;
            }
            else {
                bp = (UInt32*)b->line[y / 2 - 1].buffer;
            }
        }
        dp = (UInt32*)d->line[y].buffer;
        d->line[y].doubleWidth = a->line[y / 2].doubleWidth;

        for (x = 0; x < width; x++) {
            UInt32 av = ap[x];
            UInt32 bv = bp[x];
            dp[x] = ((((av & M1) * p + (bv & M1) * n) >> 5) & M1) |
                    ((((((av >> 5) & M2) * p + ((bv >> 5) & M2) * n) >> 5) & M2) << 5) |
                    (av & 0x80008000);
        }
    }

    return d;
}

static UInt16* getBlackImage()
{
    static UInt16* blackImage = NULL;

    if (blackImage == NULL) {
        blackImage = calloc(sizeof(UInt16), FB_MAX_LINE_WIDTH * FB_MAX_LINES);
    }
    return blackImage;
}

static void frameBufferBlack(FrameBuffer* a)
{
    UInt16* pImage = getBlackImage();
    int y;

    for (y = 0; y < a->lines; y++) {
        memcpy(a->line[y].buffer, pImage, a->maxWidth * sizeof(UInt16));
        a->line[y].doubleWidth = 0;
    }
}

static void frameBufferExternal(FrameBuffer* a)
{
    int y;
    UInt16* pImage = NULL;
    int scaleHeight = 0;
    int scaleWidth = 0;
    int imageHeight = a->lines;
    int imageWidth  = a->maxWidth;

    if (a->maxWidth <= FB_MAX_LINE_WIDTH / 2) {
        scaleWidth = 1;
        imageWidth *= 2;
    }

    if (a->lines <= FB_MAX_LINES / 4) {
        scaleHeight = 1;
        imageHeight *= 2;
    }
    else if (a->lines <= FB_MAX_LINES / 2) {
        if (a->interlace == INTERLACE_NONE) {
            scaleHeight = 1;
        }
        imageHeight *= 2;
    }

    if (mixMode & mixMask) {
        pImage = archVideoInBufferGet(imageWidth, imageHeight);
    }
    if (pImage == NULL) {
        pImage = getBlackImage();
    }
    
    if (scaleHeight) {
        a->lines *= 2;
        
        for (y = 0; y < a->lines; y++) {
            memcpy(a->line[y].buffer, pImage + y * imageWidth, imageWidth * sizeof(UInt16));
            if (scaleWidth) {
                a->line[y].doubleWidth = 1;
            }
        }
    }
}

static void frameBufferSuperimpose(FrameBuffer* a)
{
    int x, y;
    UInt16* pImage = NULL;
    int scaleHeight = 0;
    int scaleWidth = 0;
    int imageHeight = a->lines;
    int imageWidth  = a->maxWidth;

    if (a->maxWidth <= FB_MAX_LINE_WIDTH / 2) {
        scaleWidth = 1;
        imageWidth *= 2;
    }

    if (a->lines <= FB_MAX_LINES / 4) {
        scaleHeight = 1;
        imageHeight *= 2;
    }
    else if (a->lines <= FB_MAX_LINES / 2) {
        if (a->interlace == INTERLACE_NONE) {
            scaleHeight = 1;
        }
        imageHeight *= 2;
    }

    if (mixMode & mixMask) {
        pImage = archVideoInBufferGet(imageWidth, imageHeight);
    }
    if (pImage == NULL) {
        pImage = getBlackImage();
    }

    if (scaleHeight) {
        for (y = a->lines - 1; y >= 0; y--) {
            UInt16* pSrc = a->line[y].buffer;
            UInt16* pDst1 = a->line[2*y+0].buffer;
            UInt16* pDst2 = a->line[2*y+1].buffer;
            UInt16* pImg1 = pImage+(2*y+0) * imageWidth;
            UInt16* pImg2 = pImage+(2*y+1) * imageWidth;

            if (scaleWidth && a->line[y].doubleWidth) {
                for (x = imageWidth - 1; x >= 0; x--) {
                    UInt16 val = pSrc[x];
                    if (val & 0x8000) {
                        pDst1[x] = pImg1[x];
                        pDst2[x] = pImg2[x];
                    }
                    else {
                        pDst1[x] = val;
                        pDst2[x] = val;
                    }
                }
            }
            else {
                for (x = imageWidth - 1; x >= 0; x--) {
                    UInt16 val = pSrc[x / 2];
                    if (val & 0x8000) {
                        pDst1[x] = pImg1[x];
                        pDst2[x] = pImg2[x];
                        x--;
                        pDst1[x] = pImg1[x];
                        pDst2[x] = pImg2[x];
                    }
                    else {
                        pDst1[x] = val;
                        pDst2[x] = val;
                        x--;
                        pDst1[x] = val;
                        pDst2[x] = val;
                    }
                }
            }
            
            if (scaleWidth) {
                a->line[2*y+0].doubleWidth = 1;
                a->line[2*y+1].doubleWidth = 1;
            }
        }
        
        a->lines *= 2;
    }
    else {
        if (a->interlace == INTERLACE_ODD) {
            pImage += imageWidth;
        }

        for (y = a->lines - 1; y >= 0; y--) {
            UInt16* pSrc = a->line[y].buffer;
            UInt16* pDst = a->line[y].buffer;
            UInt16* pImg = pImage + y * imageWidth;

            if (scaleWidth && a->line[y].doubleWidth) {
                for (x = imageWidth - 1; x >= 0; x--) {
                    UInt16 val = pSrc[x];
                    if (val & 0x8000) {
                        pDst[x] = pImg[x];
                    }
                    else {
                        pDst[x] = val;
                    }
                }
            }
            else {
                for (x = imageWidth - 1; x >= 0; x--) {
                    UInt16 val = pSrc[x / 2];
                    if (val & 0x8000) {
                        pDst[x] = pImg[x];
                        x--;
                        pDst[x] = pImg[x];
                    }
                    else {
                        pDst[x] = val;
                        x--;
                        pDst[x] = val;
                    }
                }
            }
            if (scaleWidth) {
                a->line[y].doubleWidth = 1;
            }
        }
    }
}

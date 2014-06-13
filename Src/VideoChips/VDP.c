/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/VDP.c,v $
**
** $Revision: 1.74 $
**
** $Date: 2006/07/03 23:12:08 $
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
#include "VDP.h"
#include "V9938.h"
#include "Board.h"
#include "Board.h"
#include "IoPort.h"
#include "SaveState.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "FrameBuffer.h"
#include "ArchVideoIn.h"
#include "Language.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#define ENABLE_VRAM_DECAY
// Global configuration (not device specific)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static int spritesEnable = 1;
static int displayEnable = 1;
static int refreshRate   = 0;

void vdpSetSpritesEnable(int enable) {
    spritesEnable = enable ? 1 : 0;
}

int vdpGetSpritesEnable() {
    return spritesEnable;
}

void vdpSetDisplayEnable(int enable) {
    displayEnable = enable ? 1 : 0;
}

int vdpGetDisplayEnable() {
    return displayEnable;
}

int vdpGetRefreshRate() 
{
    return refreshRate;
}


// Video DA management

typedef struct {
    int handle;
    VideoMode videoModeMask;
    VdpDaCallbacks callbacks;
    void* ref;
} VdpDaDevice;

static void  daStart(void* ref, int oddPage) {}
static void  daEnd(void* ref) {}
static UInt8 daRead(void* ref, int screenMode, int x, int y, UInt16* palette, int count) { return 0x00; }

static VdpDaDevice vdpDaDevice = {
    0,
    VIDEO_INTERNAL,
    { daStart, daEnd, daRead },
    NULL
};

int vdpRegisterDaConverter(VdpDaCallbacks* callbacks, void* ref, VideoMode videoModeMask)
{
    vdpDaDevice.videoModeMask     = videoModeMask;
    vdpDaDevice.callbacks.daStart = callbacks->daStart ? callbacks->daStart : daStart;
    vdpDaDevice.callbacks.daEnd   = callbacks->daEnd   ? callbacks->daEnd   : daEnd;
    vdpDaDevice.callbacks.daRead  = callbacks->daRead  ? callbacks->daRead  : daRead;
    vdpDaDevice.ref               = ref;
    vdpDaDevice.handle++;

    return vdpDaDevice.handle;
}

void vdpUnregisterDaConverter(int vdpDaHandle)
{
    if (vdpDaHandle == vdpDaDevice.handle) {
        vdpDaDevice.videoModeMask       = VIDEO_INTERNAL;
        vdpDaDevice.callbacks.daStart   = daStart;
        vdpDaDevice.callbacks.daEnd     = daEnd;
        vdpDaDevice.callbacks.daRead    = daRead;
    }
}



// VDP emulation

#define HPERIOD      1368

#define INT_IE0     0x01
#define INT_IE1     0x02

#define VRAM_SIZE (192 * 1024)

static int vramAddr;
#define MAP_VRAM(vdp, addr) ((vdp)->vramPtr + ((vramAddr = addr, (vdp)->screenMode >= 7 && (vdp)->screenMode <= 12 ? (vramAddr >> 1 | ((vramAddr & 1) << 16)) : vramAddr) & (vdp)->vramAccMask))

#define vdpIsSpritesBig(regs)        (regs[1]  & 0x01)
#define vdpIsSprites16x16(regs)      (regs[1]  & 0x02)
#define vdpIsSpritesOff(regs)        (regs[8]  & 0x02)
#define vdpIsColor0Solid(regs)       (regs[8]  & 0x20)
#define vdpIsVideoPal(vdp)          (((vdp)->vdpRegs[9]  & (vdp)->palMask & 0x02) | (vdp)->palValue)
#define vdpIsOddPage(vdp)           (((~(vdp)->vdpStatus[2] & 0x02) << 7) & (((vdp)->vdpRegs[9]  & 0x04) << 6))
#define vdpIsInterlaceOn(regs)       (regs[9]  & 0x08)
#define vdpIsScanLines212(regs)      (regs[9]  & 0x80)
#define vdpIsEdgeMasked(regs)        (regs[25] & 0x02)
#define vdpIsModeYJK(regs)           (regs[25] & 0x08)
#define vdpIsModeYAE(regs)           (regs[25] & 0x10)
#define vdpVScroll(vdp)              ((vdp)->vdpRegs[23])
#define vdpHScroll(vdp)       ((((int)((vdp)->vdpRegs[26]&0x3F)<<3)-(int)((vdp)->vdpRegs[27]&0x07))&~(~(int)vdpHScroll512(vdp)<<8))
#define vdpHScroll512(vdp)    ((vdp)->vdpRegs[25]&((vdp)->vdpRegs[2]>>5)&0x1)


static const UInt8 registerValueMaskMSX1[8] = {
	0x03, 0xfb, 0x0f, 0xff, 0x07, 0x7f, 0x07, 0xff
};

static const UInt8 registerValueMaskMSX2[64] = {
	0x7e, 0x7b, 0x7f, 0xff, 0x3f, 0xff, 0x3f, 0xff,
	0xfb, 0xbf, 0x07, 0x03, 0xff, 0xff, 0x07, 0x0f,
	0x0f, 0xbf, 0xff, 0xff, 0x3f, 0x3f, 0x3f, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const UInt8 registerValueMaskMSX2p[64] = {
	0x7e, 0x7b, 0x7f, 0xff, 0x3f, 0xff, 0x3f, 0xff,
	0xfb, 0xbf, 0x07, 0x03, 0xff, 0xff, 0x07, 0x0f,
	0x0f, 0xbf, 0xff, 0xff, 0x3f, 0x3f, 0x3f, 0xff,
    0x00, 0x7f, 0x3f, 0x07, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static struct {
    int r;
    int g;
    int b;
} msx1Palette[16] = {
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 }, 
    { 0x21, 0xc8, 0x42 }, 
    { 0x5e, 0xdc, 0x78 }, 
    { 0x54, 0x55, 0xed }, 
    { 0x7d, 0x76, 0xfc }, 
    { 0xd4, 0x52, 0x4d },
    { 0x42, 0xeb, 0xf5 },
    { 0xfc, 0x55, 0x54 }, 
    { 0xff, 0x79, 0x78 }, 
    { 0xd4, 0xc1, 0x54 }, 
    { 0xe6, 0xce, 0x80 }, 
    { 0x21, 0xb0, 0x3b }, 
    { 0xc9, 0x5b, 0xba }, 
    { 0xcc, 0xcc, 0xcc }, 
    { 0xff, 0xff, 0xff }
};

static struct {
    int r;
    int g;
    int b;
} msx2Palette[16] = {
    { 0x00, 0x00, 0x00 }, 
    { 0x00, 0x00, 0x00 }, 
    { 0x24, 0xda, 0x24 }, 
    { 0x68, 0xff, 0x68 }, 
    { 0x24, 0x24, 0xff }, 
    { 0x48, 0x68, 0xff },
    { 0xb6, 0x24, 0x24 },
    { 0x48, 0xda, 0xff },
    { 0xff, 0x24, 0x24 }, 
    { 0xff, 0x68, 0x68 },
    { 0xda, 0xda, 0x24 }, 
    { 0xda, 0xda, 0x91 }, 
    { 0x24, 0x91, 0x24 }, 
    { 0xda, 0x48, 0xb6 }, 
    { 0xb6, 0xb6, 0xb6 }, 
    { 0xff, 0xff, 0xff }
};

static UInt16 defaultPaletteRegs[16] = {
    0x0000, 0x0000, 0x0611, 0x0733, 0x0117, 0x0327, 0x0151, 0x0627,
    0x0171, 0x0373, 0x0661, 0x0664, 0x0411, 0x0265, 0x0555, 0x0777
};

typedef struct VDP VDP;

static VDP* theVdp = NULL;

static void RefreshLineTx80(VDP*, int, int, int);
static void RefreshLine0(VDP*, int, int, int);
static void RefreshLine0Plus(VDP*, int, int, int);
static void RefreshLine1(VDP*, int, int, int);
static void RefreshLine2(VDP*, int, int, int);
static void RefreshLine3(VDP*, int, int, int);
static void RefreshLine4(VDP*, int, int, int);
static void RefreshLine5(VDP*, int, int, int);
static void RefreshLine6(VDP*, int, int, int);
static void RefreshLine7(VDP*, int, int, int);
static void RefreshLine8(VDP*, int, int, int);
static void RefreshLine10(VDP*, int, int, int);
static void RefreshLine12(VDP*, int, int, int);

static void sync(VDP*, UInt32);

struct VDP {
    VdpCmdState* cmdEngine;
    void (*RefreshLine)(VDP*, int, int, int);
    int    vdpConnector;
    int    vdpVersion;

    const UInt8* registerValueMask;
    UInt8  registerMask;

    UInt8  palMask;
    UInt8  palValue;
    int    scr0splitLine;
    int    vramPage;
    int    sprGenBase;
    int    sprTabBase;
    int    colTabBase;
    int    chrTabBase;
    int    chrGenBase;
    UInt8  FGColor;
    UInt8  BGColor;
    UInt8  XFGColor;
    UInt8  XBGColor;
    int    blinkFlag;
    int    blinkCnt;
    int    drawArea;
    UInt16 paletteReg[16];
    int    vramSize;
    int    vramPages;
    int    vram128;
    int    vram192;
    int    vram16;
    int    vramEnable;
    int    vramMask;
    int    lineOffset;
    int    firstLine;
    int    lastLine;
    int    displayOffest;
    int    leftBorder;
    UInt32 displayArea;

    int    curLine;

    int    screenOn;
    int    VAdjust;
    int    HAdjust;

    int screenMode;
    UInt8  vdpRegs[64];
    UInt8  vdpStatus[16];

    int    palKey;
    int    vdpKey;
    UInt8  vdpData;
    UInt8  vdpDataLatch;
    UInt16 vramAddress;

    BoardTimer* timerDisplay;
    BoardTimer* timerDrawAreaStart;
    BoardTimer* timerVStart;
    BoardTimer* timerScrModeChange;
    BoardTimer* timerHint;
    BoardTimer* timerVint;
    BoardTimer* timerTmsVint;

    UInt32 frameStartTime;
    UInt32 timeScrMode;
    UInt32 timeHint;
    UInt32 timeVint;
    UInt32 timeTmsVint;
    UInt32 timeDrawAreaStart;
    UInt32 timeVStart;
    UInt32 timeDisplay;

    UInt32 screenOffTime;
    
    UInt16 paletteFixed[256];
    UInt16 paletteSprite8[16];
    UInt16 palette0;
    UInt16 palette[16];
    UInt16 yjkColor[32][64][64];

    UInt8* vramPtr;
    int    vramAccMask;
    int vramOffsets[2];
    int vramMasks[2];
    UInt8  vram[VRAM_SIZE];
    
    int deviceHandle;
    int debugHandle;
    int videoHandle;
    int videoEnabled;

    FrameBufferData* frameBuffer;
};


#include "Common.h"


static void digitize(VDP* vdp);
static void updateOutputMode(VDP* vdp);


#include "SpriteLine.h"

static void vdpBlink(VDP* vdp)
{
    if(vdp->blinkCnt) {
        vdp->blinkCnt--;
    }
    else {
        vdp->blinkFlag = !vdp->blinkFlag;
        if (!vdp->vdpRegs[13]) { 
            vdp->XFGColor = vdp->FGColor;
            vdp->XBGColor = vdp->BGColor; 
        }
        else {
            vdp->blinkCnt = (vdp->blinkFlag ? vdp->vdpRegs[13] & 0x0f : vdp->vdpRegs[13] >> 4) * 10;
            if(vdp->blinkCnt) {
                if (vdp->blinkFlag) { 
                    vdp->XFGColor = vdp->FGColor;
                    vdp->XBGColor = vdp->BGColor; 
                }
                else { 
                    vdp->XFGColor = vdp->vdpRegs[12] >> 4; 
                    vdp->XBGColor = vdp->vdpRegs[12] & 0x0f; 
                }
            }
        }
    }
}

static void scheduleScrModeChange(VDP* vdp)
{
    vdp->timeScrMode = vdp->frameStartTime + HPERIOD * (1 + (boardSystemTime() - vdp->frameStartTime) / HPERIOD);
    boardTimerAdd(vdp->timerScrModeChange, vdp->timeScrMode);
}

static void scheduleHint(VDP* vdp)
{
    vdp->timeHint = vdp->frameStartTime + 
        (vdp->firstLine + ((vdp->vdpRegs[19] - vdp->vdpRegs[23]) & 0xff)) * HPERIOD + 
        vdp->leftBorder + vdp->displayArea;
    boardTimerAdd(vdp->timerHint, vdp->timeHint + 20);
}

static void scheduleVint(VDP* vdp)
{
    vdp->timeVint = vdp->frameStartTime + 
                    (vdp->firstLine + ((vdp->vdpRegs[9] & 0x80) ? 212 : 192)) * HPERIOD + 
                    vdp->leftBorder - 10;
    boardTimerAdd(vdp->timerVint, vdp->timeVint);
    if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
        vdp->timeTmsVint = vdp->timeVint + 1176;
        boardTimerAdd(vdp->timerTmsVint, vdp->timeTmsVint);
    }
}

static void scheduleDrawAreaStart(VDP* vdp)
{
    vdp->timeDrawAreaStart = vdp->frameStartTime + ((vdp->drawArea ? 3 + 13 : vdp->firstLine) - 1) * HPERIOD + vdp->leftBorder + vdp->displayArea;
    boardTimerAdd(vdp->timerDrawAreaStart, vdp->timeDrawAreaStart);

    vdp->timeVStart = vdp->frameStartTime + (vdp->firstLine - 1) * HPERIOD + vdp->leftBorder - 10;
    boardTimerAdd(vdp->timerVStart, vdp->timeVStart);
}

static void onHint(VDP* vdp, UInt32 time)
{
    sync(vdp, time);

    if (vdp->vdpRegs[0] & 0x10) {
        boardSetInt(INT_IE1);
    }
}

static void onVint(VDP* vdp, UInt32 time)
{
    sync(vdp, time);

    vdp->lineOffset = -1;
    vdp->vdpStatus[0] |= 0x80;
    vdp->vdpStatus[2] |= 0x40;
    if (vdp->vdpVersion != VDP_TMS9929A && vdp->vdpVersion != VDP_TMS99x8A) {
        if (vdp->vdpRegs[1] & 0x20) {
            boardSetInt(INT_IE0);
        }
    }
    vdp->drawArea = 0;
    vdpSetTimingMode(vdp->cmdEngine, vdp->vdpRegs[8] & 2);
}

static void onTmsVint(VDP* vdp, UInt32 time)
{
    if (vdp->vdpRegs[1] & 0x20) {
        boardSetInt(INT_IE0);
    }
}

static void onVStart(VDP* vdp, UInt32 time)
{
    sync(vdp, time);
    vdp->lineOffset = -1;
    vdp->vdpStatus[2] &= ~0x40;
}

static void onDrawAreaStart(VDP* vdp, UInt32 time)
{
    sync(vdp, time);

    vdp->drawArea = 1;
    vdp->vdpStatus[2] &= ~0x40;
    vdpSetTimingMode(vdp->cmdEngine, ((vdp->vdpRegs[1] >> 6) & vdp->drawArea) | (vdp->vdpRegs[8] & 2));
}

static UInt32 frameStartTime;
static int    timeDisplay;

int getScreenCompletePercent() 
{
    return 100 * (boardSystemTime() - frameStartTime) / timeDisplay;
}

static void onDisplay(VDP* vdp, UInt32 time)
{
    int isPal = vdpIsVideoPal(vdp); 
    
    sync(vdp, time);

    if (vdp->videoEnabled) {
        FrameBuffer* frameBuffer;
        frameBuffer = frameBufferFlipDrawFrame();
        frameBuffer->lines = 240;
        if (vdpIsInterlaceOn(vdp->vdpRegs)) {
            frameBuffer->interlace = 
                (vdp->vdpStatus[2] & 0x02) && (vdp->vdpRegs[9]  & 0x04) && vdp->vram128 ? INTERLACE_EVEN : INTERLACE_ODD;
        }
        else {
            frameBuffer->interlace = INTERLACE_NONE;
        }
    }

    refreshRate = isPal ? 50 : 60; // Update global refresh rate

    vdp->scr0splitLine = 0;
    vdp->curLine = 0;
    vdp->VAdjust = (-((Int8)(vdp->vdpRegs[18]) >> 4));

    vdp->lastLine = isPal ? 313 : 262;
    vdp->displayOffest = isPal ? 27 : 0;
    vdp->firstLine = vdp->displayOffest + (vdpIsScanLines212(vdp->vdpRegs) ? 14 : 24) + vdp->VAdjust;
    if (!(vdp->vdpRegs[0] & 0x10)) {
        boardClearInt(INT_IE1);
    }
    vdp->vdpStatus[2] ^= 0x02;
    RefreshScreen(vdp->screenMode);

    vdpBlink(vdp);

    vdp->frameStartTime = vdp->timeDisplay;
    vdp->timeDisplay += HPERIOD * vdp->lastLine;
    boardTimerAdd(vdp->timerDisplay, vdp->timeDisplay);

    scheduleDrawAreaStart(vdp);
    scheduleHint(vdp);
    scheduleVint(vdp);

    frameStartTime = vdp->frameStartTime;
    timeDisplay    = HPERIOD * vdp->lastLine;

    if (vdp->vdpRegs[0] & 0x40) {
        digitize(vdp);
    }
}

static void simulateVramDecay(VDP* vdp) 
{
    int time = (boardSystemTime() - vdp->screenOffTime) / 1350000;
    int i;
    if (time >= 24) {
        for (i = 0x0000; i < 0x3000; i += 2) {
            vdp->vramPtr[i]     = 0x55;
            vdp->vramPtr[i + 1] = 0xaa;
        }
    }
    else if (time >= 8) {
        static UInt32 rnd = 0xfffffff3;
        static UInt8 v[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
        UInt8* vPtr = v + time - 8;
        time -= 8;
        for (i = 0x0000; i < 0x3000; i += 2) {
            int j = 8;
            UInt8 val = 0;
            while (j--) {
                val <<= 1;
                val |= vPtr[(rnd *= 17) >> 28];
            }
            vdp->vramPtr[i] |= val & 0x55;
            vdp->vramPtr[i] &= ~(val & 0xaa);
            vdp->vramPtr[i + 1] |= val & 0xaa;
            vdp->vramPtr[i + 1] &= ~(val & 0x55);
        }
    }
}

static void onScrModeChange(VDP* vdp, UInt32 time)
{
    int scanLine = (boardSystemTime() - vdp->frameStartTime) / HPERIOD;
    int screenMode = vdp->screenMode;
    sync(vdp, time);

#if 0
    switch (((vdp->vdpRegs[0] & 0x0e) << 1) | 
            ((vdp->vdpRegs[1] & 0x08) >> 2) | 
            ((vdp->vdpRegs[1] & 0x10) >> 4)) 
    {
    case 0x01: vdp->screenMode = 0; break;
    case 0x00: vdp->screenMode = 1; break;
    case 0x04: vdp->screenMode = 2; break;
    case 0x02: vdp->screenMode = 3; break;
    case 0x08: vdp->screenMode = 4; break;
    case 0x0c: vdp->screenMode = 5; break;
    case 0x10: vdp->screenMode = 6; break;
    case 0x14: vdp->screenMode = 7; break;
    case 0x1c: vdp->screenMode = 8; break;
    case 0x09: vdp->screenMode = 13; break;
    }
#else
    switch (((vdp->vdpRegs[0] & 0x0e) >> 1) | (vdp->vdpRegs[1] & 0x18)) {
    case 0x10: vdp->screenMode = 0; break;
    case 0x00: vdp->screenMode = 1; break;
    case 0x01: vdp->screenMode = 2; break;
    case 0x08: vdp->screenMode = 3; break;
    case 0x02: vdp->screenMode = 4; break;
    case 0x03: vdp->screenMode = 5; break;
    case 0x04: vdp->screenMode = 6; break;
    case 0x05: vdp->screenMode = 7; break;
    case 0x07: vdp->screenMode = 8; break;
    case 0x12: vdp->screenMode = 13; break;
    case 0x11: 
        if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
            vdp->screenMode = 16; 
        }
        break;
    }
#endif
    vdp->chrTabBase = ((((int)vdp->vdpRegs[2] << 10) & ~((int)(vdp->vdpRegs[25] & 1) << 15)) | ~(-1 << 10)) & vdp->vramMask;
    vdp->chrGenBase = (((int)vdp->vdpRegs[4] << 11) | ~(-1 << 11)) & vdp->vramMask;
    vdp->colTabBase = (((int)vdp->vdpRegs[10] << 14) | ((int)vdp->vdpRegs[3] << 6) | ~(-1 << 6)) & vdp->vramMask;

    vdp->sprTabBase = (((int)vdp->vdpRegs[11] << 15) | ((int)vdp->vdpRegs[5] << 7) | ~(-1 << 7)) & vdp->vramMask;
    vdp->sprGenBase = (((int)vdp->vdpRegs[6] << 11) | ~(-1 << 11)) & vdp->vramMask;

#ifdef ENABLE_VRAM_DECAY
    if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
        if ((vdp->screenOn ^ vdp->vdpRegs[1]) & 0x40) {
            if ((~vdp->vdpRegs[1] & 0x80) && !vdp->screenOn) {
               simulateVramDecay(vdp);
            }
            vdp->screenOffTime = boardSystemTime();
        }
    }
#endif
    vdp->screenOn = vdp->vdpRegs[1] & 0x40;
    
    vdpSetScreenMode(vdp->cmdEngine, vdp->screenMode & 0x0f, vdp->vdpRegs[25] & 0x40);

    switch (vdp->screenMode) {
    case 0: vdp->RefreshLine = RefreshLine0; break;
    case 1: vdp->RefreshLine = RefreshLine1; break;
    case 2: vdp->RefreshLine = RefreshLine2; break;
    case 3: vdp->RefreshLine = RefreshLine3; break;
    case 4: vdp->RefreshLine = RefreshLine4; break;
    case 5: vdp->RefreshLine = RefreshLine5; break;
    case 6: vdp->RefreshLine = RefreshLine6; break;
    case 7: 
        if(!vdpIsModeYJK(vdp->vdpRegs)) {
            vdp->RefreshLine = RefreshLine7; 
            break;
        }
    case 8: 
        if(vdp->screenMode == 8 && !vdpIsModeYJK(vdp->vdpRegs)) {
            vdp->RefreshLine = RefreshLine8; 
        }
        else if (vdpIsModeYAE(vdp->vdpRegs)) {
            vdp->RefreshLine = RefreshLine10; 
            vdp->screenMode = 10;
        }
        else {
            vdp->RefreshLine = RefreshLine12; 
            vdp->screenMode = 12;
        }
        break;
    case 16:
        vdp->screenMode = 0;
        vdp->RefreshLine = RefreshLine0Plus; 
        break;
    default:
        vdp->RefreshLine = RefreshLineTx80; break;
        vdp->screenMode = 13;
        break;
    }
    
    if (screenMode != vdp->screenMode) {
        vdp->scr0splitLine = (scanLine - vdp->firstLine) & ~7;
    }

    if (vdp->screenMode == 0 || vdp->screenMode == 13) {
        vdp->displayArea = 960;
        vdp->leftBorder = 102 + 92;
    }
    else {
        vdp->displayArea = 1024;
        vdp->leftBorder = 102 + 56;
    }

    vdp->HAdjust = (-((Int8)(vdp->vdpRegs[18] << 4) >> 4));
	if (vdp->vdpRegs[25] & 0x08) {
		vdp->HAdjust += 4;
	}
    vdp->leftBorder += vdp->HAdjust;
}

static void vdpUpdateRegisters(VDP* vdp, UInt8 reg, UInt8 value)
{ 
    UInt8 change;

    reg   &= vdp->registerMask;
    value &= vdp->registerValueMask[reg];
    sync(vdp, boardSystemTime());

#if 0
    if (reg == 0 || reg == 8 || reg == 9)
        printf("W %.2x: 0x%.2x\n", reg, value);
#endif
    change = vdp->vdpRegs[reg] ^ value;
    vdp->vdpRegs[reg] = value;

    if (reg >= 0x20) {
        if (reg == 0x2d && (change & 0x40)) {
            vdp->vramPtr      = vdp->vram + vdp->vramOffsets[(value >> 6) & 1];
            vdp->vramAccMask  = vdp->vramMasks[(value >> 6) & 1];
            vdp->vramEnable   = vdp->vram192 || !((value >> 6) & 1);
        }
        vdpCmdWrite(vdp->cmdEngine, reg - 0x20, value, boardSystemTime());
        return;
    }

    switch (reg) {
    case 0: 
        if (!(value & 0x10)) {
            boardClearInt(INT_IE1);
        }

        if (change & 0x0e) {
            scheduleScrModeChange(vdp);
        }
        
        if (change & 0x40) {
             updateOutputMode(vdp);
        }

        break;

    case 1: 
        if (vdp->vdpStatus[0] & 0x80) {
			if (value & 0x20) {
				boardSetInt(INT_IE0);
			}
			else {
				boardClearInt(INT_IE0);
			}
        }

        if (change & 0x58) {
            scheduleScrModeChange(vdp);
        }
        
        vdpSetTimingMode(vdp->cmdEngine, ((value >> 6) & vdp->drawArea) | (vdp->vdpRegs[8] & 2));
        break;

    case 2: 
        vdp->chrTabBase = ((((int)vdp->vdpRegs[2] << 10) & ~((int)(vdp->vdpRegs[25] & 1) << 15)) | ~(-1 << 10)) & vdp->vramMask;
        break;

    case 3: 
        vdp->colTabBase = (((int)vdp->vdpRegs[10] << 14) | ((int)value << 6) | ~(-1 << 6)) & vdp->vramMask;
        break;

    case 4: 
        vdp->chrGenBase = (((int)value << 11) | ~(-1 << 11)) & vdp->vramMask;
        break;

    case 5: 
        vdp->sprTabBase = ((vdp->vdpRegs[11] << 15) | (value << 7) | ~(-1 << 7)) & ((vdp->vramPages << 14) - 1);
        break;

    case 6: 
    	vdp->sprGenBase = ((value << 11) | ~(-1 << 11)) & ((vdp->vramPages << 14) - 1);
        break;

    case 7: 
        vdp->FGColor = value >> 4;
        vdp->BGColor = value & 0x0F;
        updateOutputMode(vdp);
        break;

    case 8:
        vdpSetTimingMode(vdp->cmdEngine, ((vdp->vdpRegs[1] >> 6) & vdp->drawArea) | (value & 2));
        if (change & 0xb0) {
            updateOutputMode(vdp);
        }
        break;

    case 9:
        value = (value & vdp->palMask) | vdp->palValue;
        if (change & 0x80) {
            scheduleVint(vdp);
        }
        if (change & 0x30) {
            updateOutputMode(vdp);
        }
        break;

    case 10: 
        vdp->colTabBase = (((int)value << 14) | ((int)vdp->vdpRegs[3] << 6) | ~(-1 << 6)) & vdp->vramMask;
        break;

    case 11: 
        vdp->sprTabBase = ((value << 15) | (vdp->vdpRegs[5] << 7) | ~(-1 << 7)) & ((vdp->vramPages << 14) - 1);
        break;

    case 14:
        value &= vdp->vramPages - 1;
        vdp->vramPage = (int)value << 14; 
        if (vdp->vram16) {
            vdp->vramEnable = value == 0;
        }
        break;

    case 16: 
        vdp->palKey = 0;
        break;

    case 18:
        if (change) {
            scheduleScrModeChange(vdp);
        }
        break;

    case 19:
        boardClearInt(INT_IE1);
        if (change) {
            scheduleHint(vdp);
        }
        break;

    case 23:
        if (change) {
            scheduleHint(vdp);
            spriteLineInvalidate(vdp, (boardSystemTime() - vdp->frameStartTime) / HPERIOD);
        }
        if (!(vdp->vdpRegs[0] & 0x10)) {
            boardClearInt(INT_IE1);
        }
        break;

    case 25: 
        if (change) {
            scheduleScrModeChange(vdp);
        }
        break;

    default:
        break;
    }
} 

static UInt8 peek(VDP* vdp, UInt16 ioPort) 
{
    if (vdp->vdpVersion == VDP_V9938 || vdp->vdpVersion == VDP_V9958) {
        vdpCmdExecute(vdp->cmdEngine, boardSystemTime());
    }

	return vdp->vdpData;
}

static UInt8 read(VDP* vdp, UInt16 ioPort) 
{
    UInt8 value;

    if (vdp->vdpVersion == VDP_V9938 || vdp->vdpVersion == VDP_V9958) {
        vdpCmdExecute(vdp->cmdEngine, boardSystemTime());
    }

	value = vdp->vdpData;
    vdp->vdpData = vdp->vramEnable ? *MAP_VRAM(vdp, (vdp->vdpRegs[14] << 14) | vdp->vramAddress) : 0xff;
	vdp->vramAddress = (vdp->vramAddress + 1) & 0x3fff;
    if (vdp->vramAddress == 0 && vdp->screenMode > 3) {
        vdp->vdpRegs[14] = (vdp->vdpRegs[14] + 1) & (vdp->vramPages - 1);
    }
	vdp->vdpKey = 0;

    return value;
}

static UInt8 peekStatus(VDP* vdp, UInt16 ioPort)
{
    UInt8 vdpStatus;

    sync(vdp, boardSystemTime());
    if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
        return vdp->vdpStatus[0];
    }
    vdpStatus = vdp->vdpStatus[vdp->vdpRegs[15]];

    switch(vdp->vdpRegs[15]) {
    case 1: 
        if (vdp->vdpRegs[0] & 0x10) {
            if (boardGetInt(INT_IE1)) {
                vdpStatus |=  0x01;
            }
        }
        else {
            if (boardSystemTime() - vdp->timeHint < HPERIOD - vdp->displayArea) {
                vdpStatus |= 0x01;
            }
        }
        break;

    case 2:
        {
            UInt32 frameTime = boardSystemTime() - vdp->frameStartTime;
            vdpStatus |= 0x40 | 0x20 | vdpGetStatus(vdp->cmdEngine);
            if (vdp->drawArea || (frameTime - ((vdp->firstLine - 1) * HPERIOD + vdp->leftBorder - 10) < 4 * HPERIOD)) {
                vdpStatus &= ~0x40;
            }
            if (frameTime % HPERIOD - vdp->leftBorder - 30 < (UInt32)vdp->displayArea + 30) {
                vdpStatus &= ~0x20;
            }
        }
        break;

    case 7: 
        vdpStatus = vdpGetColor(vdp->cmdEngine);
        break;

    case 8: 
        vdpStatus = (UInt8)vdpGetBorderX(vdp->cmdEngine);
        break;

    case 9: 
        vdpStatus = (UInt8)(vdpGetBorderX(vdp->cmdEngine) >> 8);
        break;
    }

    return vdpStatus;
}

static UInt8 readStatus(VDP* vdp, UInt16 ioPort)
{
    UInt8 vdpStatus;

    sync(vdp, boardSystemTime());

    vdp->vdpKey = 0;

    if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
        vdpStatus = vdp->vdpStatus[0];
        vdp->vdpStatus[0] &= 0x1f;
        boardClearInt(INT_IE0);
        return vdpStatus;
    }
    
    vdpStatus = vdp->vdpStatus[vdp->vdpRegs[15]];
   
    switch(vdp->vdpRegs[15]) {
    case 0: 
        vdp->vdpStatus[0] &= 0x1f;
        boardClearInt(INT_IE0);
        break;

    case 1: 
        if (vdp->vdpRegs[0] & 0x10) {
            if (boardGetInt(INT_IE1)) {
                boardClearInt(INT_IE1);
                vdpStatus |=  0x01;
            }
        }
        else {
            if (boardSystemTime() - vdp->timeHint < HPERIOD - vdp->displayArea) {
                vdpStatus |= 0x01;
            }
        }
        break;

    case 2:
        {
            UInt32 frameTime = boardSystemTime() - vdp->frameStartTime;
            vdpStatus |= 0x40 | 0x20 | vdpGetStatus(vdp->cmdEngine);
            if (vdp->drawArea || (frameTime - ((vdp->firstLine - 1) * HPERIOD + vdp->leftBorder - 10) < 4 * HPERIOD)) {
                vdpStatus &= ~0x40;
            }
            if (frameTime % HPERIOD - vdp->leftBorder - 30 < (UInt32)vdp->displayArea + 30) {
                vdpStatus &= ~0x20;
            }
        }
        break;

    case 7: 
        vdpStatus = vdpGetColor(vdp->cmdEngine);
        break;

    case 8: 
        vdpStatus = (UInt8)vdpGetBorderX(vdp->cmdEngine);
        break;

    case 9: 
        vdpStatus = (UInt8)(vdpGetBorderX(vdp->cmdEngine) >> 8);
        break;
    }

    return vdpStatus;
}

static void write(VDP* vdp, UInt16 ioPort, UInt8 value)
{
    sync(vdp, boardSystemTime());

    if (vdp->vramEnable) {
        *MAP_VRAM(vdp, (vdp->vdpRegs[14] << 14) | vdp->vramAddress) = value;
    }
	vdp->vdpData = value;
	vdp->vdpKey = 0;
    vdp->vramAddress = (vdp->vramAddress + 1) & 0x3fff;
    if (vdp->vramAddress == 0 && vdp->screenMode > 3) {
        vdp->vdpRegs[14] = (vdp->vdpRegs[14] + 1 )& (vdp->vramPages - 1);
    }
    if (!vdp->videoEnabled && boardGetVideoAutodetect() && videoManagerGetCount() > 1) {
        videoManagerSetActive(vdp->videoHandle);
    }
}

static void writeLatch(VDP* vdp, UInt16 ioPort, UInt8 value)
{
    if (vdp->vdpKey) {
		if (value & 0x80) {
			if (!(value & 0x40)) {
                vdpUpdateRegisters(vdp, value, vdp->vdpDataLatch);
			}
		} 
        else {
			vdp->vramAddress = ((UInt16)value << 8 | vdp->vdpDataLatch) & 0x3fff;
			if (!(value & 0x40)) {
				read(vdp, ioPort);
			}
		}
		vdp->vdpKey = 0;
	} else {
		vdp->vdpDataLatch = value;
		vdp->vdpKey = 1;
	}
}

static void initPalette(VDP* vdp)
{
    int i;
    int y;
    int J;
    int K;

    for (y = 0; y < 32; y++) {
        for (J = 0; J < 64; J++) {
            for (K = 0; K < 64; K++) {
		        int j = (J & 0x1f) - (J & 0x20);
		        int k = (K & 0x1f) - (K & 0x20);
			    int r = 255 * (y + j) / 31;
			    int g = 255 * (y + k) / 31;
			    int b = 255 * ((5 * y - 2 * j - k) / 4) / 31;

                r = MIN(255, MAX(0, r));
                g = MIN(255, MAX(0, g));
                b = MIN(255, MAX(0, b));
                vdp->yjkColor[y][J][K] = videoGetColor(r, g, b);
            }
        }
    }

    for (i = 0; i < 256; i++) {
        vdp->paletteFixed[i] = videoGetColor(255 * ((i >> 2) & 7) / 7, 
                                           255 * ((i >> 5) & 7) / 7, 
                                           255 * ((i & 3) == 3 ? 7 : 2 * (i & 3)) / 7);
    }

    vdp->paletteSprite8[0]  = videoGetColor(0 * 255 / 7, 0 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[1]  = videoGetColor(0 * 255 / 7, 0 * 255 / 7, 2 * 255 / 7);
    vdp->paletteSprite8[2]  = videoGetColor(3 * 255 / 7, 0 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[3]  = videoGetColor(3 * 255 / 7, 0 * 255 / 7, 2 * 255 / 7);
    vdp->paletteSprite8[4]  = videoGetColor(0 * 255 / 7, 3 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[5]  = videoGetColor(0 * 255 / 7, 3 * 255 / 7, 2 * 255 / 7);
    vdp->paletteSprite8[6]  = videoGetColor(3 * 255 / 7, 3 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[7]  = videoGetColor(3 * 255 / 7, 3 * 255 / 7, 2 * 255 / 7);
    vdp->paletteSprite8[8]  = videoGetColor(7 * 255 / 7, 4 * 255 / 7, 2 * 255 / 7);
    vdp->paletteSprite8[9]  = videoGetColor(0 * 255 / 7, 0 * 255 / 7, 7 * 255 / 7);
    vdp->paletteSprite8[10] = videoGetColor(7 * 255 / 7, 0 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[11] = videoGetColor(7 * 255 / 7, 0 * 255 / 7, 7 * 255 / 7);
    vdp->paletteSprite8[12] = videoGetColor(0 * 255 / 7, 7 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[13] = videoGetColor(0 * 255 / 7, 7 * 255 / 7, 7 * 255 / 7);
    vdp->paletteSprite8[14] = videoGetColor(7 * 255 / 7, 7 * 255 / 7, 0 * 255 / 7);
    vdp->paletteSprite8[15] = videoGetColor(7 * 255 / 7, 7 * 255 / 7, 7 * 255 / 7);
}

static void digitize(VDP* vdp)
{
    UInt8 colorMask = vdp->vdpRegs[7];
    int yDelta = 14 + vdp->VAdjust;
    int x, y;

    vdpDaDevice.callbacks.daStart(vdpDaDevice.ref, vdpIsOddPage(vdp));

#define videoDaGet(sm, x, y, pal, cnt) vdpDaDevice.callbacks.daRead(vdpDaDevice.ref, sm, x, y, pal, cnt)

    for (y = 0; y < 212; y++) {
        UInt8* charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((y + vdpVScroll(vdp)) << 7)));

        switch (vdp->screenMode) {
        case 5:
            for (x = 0; x < 128; x++) {
                charTable[x] = ((videoDaGet(vdp->screenMode, 4 * x + 0, y + yDelta, vdp->palette, 16) & colorMask) << 4) |
                               ((videoDaGet(vdp->screenMode, 4 * x + 2, y + yDelta, vdp->palette, 16) & colorMask) << 0);
            }
            break;
        case 6:
            for (x = 0; x < 128; x++) {
                charTable[x] = ((((videoDaGet(vdp->screenMode, 4 * x + 0, y + yDelta, vdp->palette, 4) << 2) | 
                                  (videoDaGet(vdp->screenMode, 4 * x + 1, y + yDelta, vdp->palette, 4) << 0)) & colorMask) << 4) |
                               ((((videoDaGet(vdp->screenMode, 4 * x + 2, y + yDelta, vdp->palette, 4) << 2) | 
                                  (videoDaGet(vdp->screenMode, 4 * x + 3, y + yDelta, vdp->palette, 4) << 0)) & colorMask) << 0);
            }
            break;
        case 7:
            for (x = 0; x < 128; x++) {
                charTable[x] =                ((videoDaGet(vdp->screenMode, 4 * x + 0, y + yDelta, vdp->palette, 16) & colorMask) << 4) |
                                              ((videoDaGet(vdp->screenMode, 4 * x + 1, y + yDelta, vdp->palette, 16) & colorMask) << 0);
                charTable[x + vdp->vram128] = ((videoDaGet(vdp->screenMode, 4 * x + 2, y + yDelta, vdp->palette, 16) & colorMask) << 4) |
                                              ((videoDaGet(vdp->screenMode, 4 * x + 3, y + yDelta, vdp->palette, 16) & colorMask) << 0);
            }
            break;
        case 8:
        case 10:
        case 11:
        case 12:
            for (x = 0; x < 128; x++) {
                charTable[x]                = videoDaGet(vdp->screenMode, 4 * x + 0, y + yDelta, NULL, 0) & colorMask;
                charTable[x + vdp->vram128] = videoDaGet(vdp->screenMode, 4 * x + 2, y + yDelta, NULL, 0) & colorMask;
            }
        }
    }
    
    vdpDaDevice.callbacks.daEnd(vdpDaDevice.ref);
}

static void updateOutputMode(VDP* vdp)
{
    int mode = (vdp->vdpRegs[9] >> 4) & 3;
    int transparency = (vdp->screenMode < 8 || vdp->screenMode > 12) && (vdp->vdpRegs[8] & 0x20) == 0;

    if (mode == 2 || 
        (!(vdp->vdpRegs[8] & 0x80) && (vdp->vdpRegs[8] & 0x10) || (vdp->vdpRegs[0] & 0x40))) {
        videoManagerSetMode(vdp->videoHandle, VIDEO_EXTERNAL, vdpDaDevice.videoModeMask);
    }
    else if (mode == 1 && transparency) {
        vdp->palette[0] = videoGetTransparentColor();
        videoManagerSetMode(vdp->videoHandle, VIDEO_MIX, vdpDaDevice.videoModeMask);
    }
    else {
        if (vdp->BGColor == 0 || !transparency) {
            vdp->palette[0] = vdp->palette0;
        }
        else {
            vdp->palette[0] = vdp->palette[vdp->BGColor];
        }
        videoManagerSetMode(vdp->videoHandle, VIDEO_INTERNAL, vdpDaDevice.videoModeMask);
    }
}

static void updatePalette(VDP* vdp, int palEntry, int r, int g, int b)
{
    UInt16 color = videoGetColor(r, g, b);
    if (palEntry == 0) {
        vdp->palette0 = color;
        updateOutputMode(vdp);
    }
    else {
        vdp->palette[palEntry] = color;
        if (palEntry == vdp->BGColor) {
            updateOutputMode(vdp);
        }
    }
}

static void writePaletteLatch(VDP* vdp, UInt16 ioPort, UInt8 value)
{
    if (vdp->palKey) {
		int palEntry = vdp->vdpRegs[16];
        sync(vdp, boardSystemTime());
        vdp->paletteReg[palEntry] = 256 * (value & 0x07) | (vdp->vdpDataLatch & 0x77);
        updatePalette(vdp, palEntry, (vdp->vdpDataLatch & 0x70) * 255 / 112, 
                                     (value & 0x07) * 255 / 7,
                                     (vdp->vdpDataLatch & 0x07) * 255 / 7);

        vdp->vdpRegs[16] = (palEntry + 1) & 0x0f;
		vdp->palKey = 0;
	} 
    else {
		vdp->vdpDataLatch = value;
		vdp->palKey = 1;
	}
}

static void writeRegister(VDP* vdp, UInt16 ioPort, UInt8 value)
{
    int reg;

	vdp->vdpDataLatch = value;
    reg = vdp->vdpRegs[17];
    if((reg & 0x3f) != 17) {
        vdpUpdateRegisters(vdp, reg, value);
    }
    if(~reg & 0x80) {
        vdp->vdpRegs[17] = (reg + 1) & 0x3f;
    }
}

void vdpForceSync()
{
    if (theVdp != NULL) {
        sync(theVdp, boardSystemTime());
    }
}

static void sync(VDP* vdp, UInt32 systemTime) 
{
    int frameTime = systemTime - vdp->frameStartTime;
    int scanLine = frameTime / HPERIOD;
    int lineTime = frameTime % HPERIOD - vdp->leftBorder + 20;
    int curLineOffset;

    if (vdp->vdpVersion == VDP_V9938 || vdp->vdpVersion == VDP_V9958) {
        vdpCmdExecute(vdp->cmdEngine, boardSystemTime());
    }

    if (!vdp->videoEnabled || !displayEnable || frameBufferGetDrawFrame() == NULL) {
        return;
    }

    if (vdp->curLine < scanLine) {
        if (vdp->lineOffset <= 32) {
            if (vdp->curLine >= vdp->displayOffest && vdp->curLine < vdp->displayOffest + SCREEN_HEIGHT) {
                vdp->RefreshLine(vdp, vdp->curLine, vdp->lineOffset, 33);
            }
        }
        vdp->lineOffset = -1;
        vdp->curLine++;
        while (vdp->curLine < scanLine) {
            if (vdp->curLine >= vdp->displayOffest && vdp->curLine < vdp->displayOffest + SCREEN_HEIGHT) {
                vdp->RefreshLine(vdp, vdp->curLine, -1, 33);
            }
            vdp->curLine++;
        }
    }

    if (vdp->lineOffset > 32 || lineTime < -1) {
        return;
    }

    curLineOffset = (lineTime + 32) / 32 - 1;
    if (curLineOffset > 33) {
        curLineOffset = 33;
    }

    if (vdp->lineOffset < curLineOffset) {
        if (vdp->curLine >= vdp->displayOffest && vdp->curLine < vdp->displayOffest + SCREEN_HEIGHT) {
            vdp->RefreshLine(vdp, vdp->curLine, vdp->lineOffset, curLineOffset);
        }
        vdp->lineOffset = curLineOffset;
    }
}

static void saveState(VDP* vdp)
{
    SaveState* state = saveStateOpenForWrite("vdp");
    char tag[32];
    int index;

    saveStateGet(state, "frameStartTime",    vdp->frameStartTime);
    saveStateGet(state, "timeScrMode",       vdp->timeScrMode);
    saveStateGet(state, "timeHint",          vdp->timeHint);
    saveStateGet(state, "timeVint",          vdp->timeVint);
    saveStateGet(state, "timeTmsVint",       vdp->timeTmsVint);
    saveStateGet(state, "timeDrawAreaStart", vdp->timeDrawAreaStart);
    saveStateGet(state, "timeVStart",        vdp->timeVStart);
    saveStateGet(state, "timeDisplay",       vdp->timeDisplay);

    saveStateSet(state, "palKey",          vdp->palKey);
    saveStateSet(state, "vdpKey",          vdp->vdpKey);
    saveStateSet(state, "vramAddress",     vdp->vramAddress);
    saveStateSet(state, "vdpData",         vdp->vdpData);
    saveStateSet(state, "vdpDataLatch",    vdp->vdpDataLatch);
    saveStateSet(state, "xfgColor",        vdp->XFGColor);
    saveStateSet(state, "xbgColor",        vdp->XBGColor);
    saveStateSet(state, "firstLine",       vdp->firstLine);
    saveStateSet(state, "blinkFlag",       vdp->blinkFlag);
    saveStateSet(state, "blinkCnt",        vdp->blinkCnt);
    saveStateSet(state, "drawArea",        vdp->drawArea);
    saveStateSet(state, "vramPages",       vdp->vramPages);
    saveStateSet(state, "vdpConnector",    vdp->vdpConnector);
    saveStateSet(state, "vdpVersion",      vdp->vdpVersion);
    saveStateGet(state, "leftBorder",      vdp->leftBorder);
    saveStateGet(state, "hRefresh",        vdp->displayArea);

    saveStateSetBuffer(state, "regs", vdp->vdpRegs, sizeof(vdp->vdpRegs));
    saveStateSetBuffer(state, "status", vdp->vdpStatus, sizeof(vdp->vdpStatus));

    for (index = 0; index < sizeof(vdp->palette) / sizeof(vdp->palette[0]); index++) {
        sprintf(tag, "vdp->palette%d", index);
        saveStateSet(state, tag, vdp->palette[index]);
    }
    
    saveStateSetBuffer(state, "vram", vdp->vram, 0x4000 * vdp->vramPages);

    saveStateClose(state);

    vdpCmdSaveState(vdp->cmdEngine);
}

static void loadState(VDP* vdp)
{
    SaveState* state = saveStateOpenForRead("vdp");
    UInt32 systemTime = boardSystemTime() + 100;
    char tag[32];
    int index;

    vdp->frameStartTime    =      saveStateGet(state, "frameStartTime",    systemTime);
    vdp->timeScrMode       =      saveStateGet(state, "timeScrMode",       systemTime);
    vdp->timeHint          =      saveStateGet(state, "timeHint",          systemTime);
    vdp->timeVint          =      saveStateGet(state, "timeVint",          systemTime);
    vdp->timeTmsVint       =      saveStateGet(state, "timeTmsVint",       systemTime);
    vdp->timeDrawAreaStart =      saveStateGet(state, "timeDrawAreaStart", systemTime);
    vdp->timeVStart        =      saveStateGet(state, "timeVStart",        systemTime);
    vdp->timeDisplay       =      saveStateGet(state, "timeDisplay",       systemTime);

    vdp->palKey         =         saveStateGet(state, "palKey",          0);
    vdp->vdpKey         =         saveStateGet(state, "vdpKey",          0);
    vdp->vramAddress    = (UInt16)saveStateGet(state, "vramAddress",     0);
    vdp->vdpData        = (UInt8) saveStateGet(state, "vdpData",         0);
    vdp->vdpDataLatch   = (UInt8) saveStateGet(state, "vdpDataLatch",    0);
    vdp->XFGColor       = (UInt8) saveStateGet(state, "xfgColor",        0);
    vdp->XBGColor       = (UInt8) saveStateGet(state, "xbgColor",        0);
    vdp->firstLine      =         saveStateGet(state, "firstLine",       1);
    vdp->blinkFlag      =         saveStateGet(state, "blinkFlag",       0);
    vdp->blinkCnt       =         saveStateGet(state, "blinkCnt",        0);
    vdp->drawArea       =         saveStateGet(state, "drawArea",        0);
    vdp->vramPages      =         saveStateGet(state, "vramPages",       8);
    vdp->vdpConnector   =         saveStateGet(state, "vdpConnector",    VDP_MSX);
    vdp->vdpVersion     =         saveStateGet(state, "vdpVersion",      0);
    vdp->leftBorder     =         saveStateGet(state, "leftBorder",      200);
    vdp->displayArea       =         saveStateGet(state, "hRefresh",        1024);

    vdp->screenOffTime = boardSystemTime();

    saveStateGetBuffer(state, "regs", vdp->vdpRegs, sizeof(vdp->vdpRegs));
    saveStateGetBuffer(state, "status", vdp->vdpStatus, sizeof(vdp->vdpStatus));

    for (index = 0; index < sizeof(vdp->palette) / sizeof(vdp->palette[0]); index++) {
        sprintf(tag, "vdp->palette%d", index);
        vdp->palette[index] = (UInt16)saveStateGet(state, tag, 0);
    }

    saveStateGetBuffer(state, "vram", vdp->vram, 0x4000 * vdp->vramPages);

    saveStateClose(state);

    vdp->vramMask        = (vdp->vramPages << 14) - 1;
    vdp->vram128         = vdp->vramPages >= 8 ? 0x10000 : 0;
    vdp->vramPage        = ((int)vdp->vdpRegs[14] << 14) & (vdp->vramPages - 1);
    vdp->lastLine        = vdpIsVideoPal(vdp) ? 313 : 262;    
    vdp->displayOffest   = vdpIsVideoPal(vdp) ? 27 : 0;  
    vdp->FGColor         = vdp->vdpRegs[7] >> 4;
    vdp->BGColor         = vdp->vdpRegs[7] & 0x0F;
    vdp->lineOffset      = -1;

    switch (vdp->vdpVersion) {
    case VDP_TMS9929A:
    case VDP_TMS99x8A:
        vdp->registerValueMask = registerValueMaskMSX1;
        vdp->registerMask      = 0x07;
        break;
    case VDP_V9938:
        vdp->registerValueMask = registerValueMaskMSX2;
        vdp->registerMask      = 0x3f;
        break;
    case VDP_V9958:
        vdp->registerValueMask = registerValueMaskMSX2p;
        vdp->registerMask      = 0x3f;
        break;
    }
    
    vdp->vramPtr        = vdp->vram + vdp->vramOffsets[(vdp->vdpRegs[0x2d] >> 6) & 1];
    vdp->vramAccMask    = vdp->vramMasks[(vdp->vdpRegs[0x2d] >> 6) & 1];
    vdp->vramEnable     = vdp->vram192 || !((vdp->vdpRegs[0x2d] >> 6) & 1);
    vdp->vramEnable    &= !vdp->vram16 || vdp->vdpRegs[14] == 0;

    vdpCmdLoadState(vdp->cmdEngine);

    onScrModeChange(vdp, boardSystemTime());

    boardTimerAdd(vdp->timerScrModeChange, vdp->timeScrMode);
    boardTimerAdd(vdp->timerHint, vdp->timeHint);
    boardTimerAdd(vdp->timerVint, vdp->timeVint);
    if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
        boardTimerAdd(vdp->timerTmsVint, vdp->timeTmsVint);
    }
    boardTimerAdd(vdp->timerDrawAreaStart, vdp->timeDrawAreaStart);
    boardTimerAdd(vdp->timerVStart, vdp->timeVStart);
    boardTimerAdd(vdp->timerDisplay, vdp->timeDisplay);
}

static void getDebugInfo(VDP* vdp, DbgDevice* dbgDevice)
{
    DbgRegisterBank* regBank;
    DbgIoPorts* ioPorts;
    int cmdRegCount;
    int regCount;
    int i;
    int extraRegCount = 3;
    int statusRegCount;
    int paletteCount;
    int scanLine;
    int lineTime;
    int frameTime;
    int regOffset = 0;
    char* vdpVersionName;

    switch (vdp->vdpVersion) {
    case VDP_TMS9929A:
        vdpVersionName = "TMS9929A";
        break;
    case VDP_TMS99x8A:
        vdpVersionName = "TMS99x8A";
        break;
    case VDP_V9938:
        vdpVersionName = "V9938";
        break;
    case VDP_V9958:
        vdpVersionName = "V9958";
        break;
    default:
        vdpVersionName = "VDP";
        break;
    }

    sync(vdp, boardSystemTime());

    dbgDeviceAddMemoryBlock(dbgDevice, langDbgMemVram(), 0, 0, vdp->vramSize, vdp->vram);

    if (vdp->vdpVersion == VDP_V9938) {
        regCount = 24;
        statusRegCount = 9;
        paletteCount = 16;
        cmdRegCount = 15;
    }
    else if (vdp->vdpVersion == VDP_V9958) {
        regCount = 32;
        statusRegCount = 9;
        paletteCount = 16;
        cmdRegCount = 15;
    }
    else {
        regCount = 8;
        statusRegCount = 1;
        paletteCount = 0;
        cmdRegCount = 0;
    }

    regBank = dbgDeviceAddRegisterBank(dbgDevice, langDbgRegs(), 
                                       regCount + 
                                       cmdRegCount + 
                                       paletteCount +
                                       statusRegCount + 
                                       extraRegCount);

    for (i = 0; i < regCount; i++) {
        char reg[4];
        sprintf(reg, "R%d", i);
        dbgRegisterBankAddRegister(regBank, regOffset++, reg,  8, vdp->vdpRegs[i]);
    }

    for (i = 0; i < cmdRegCount; i++) {
        char reg[4];
        sprintf(reg, "R%d", 32 + i);
        dbgRegisterBankAddRegister(regBank, regOffset++, reg, 8, vdpCmdPeek(vdp->cmdEngine, i, boardSystemTime()));
    }

    for (i = 0; i < paletteCount; i++) {
        char reg[4];
        sprintf(reg, "P%d", i);
        dbgRegisterBankAddRegister(regBank, regOffset++, reg, 16, vdp->paletteReg[i]);
    }

    for (i = 0; i < statusRegCount; i++) {
        char reg[4];
        sprintf(reg, "S%d", i);
        dbgRegisterBankAddRegister(regBank, regOffset++, reg, 8, vdp->vdpStatus[i]);
    }

    frameTime = boardSystemTime() - vdp->frameStartTime;
    scanLine = frameTime / HPERIOD;
    lineTime = frameTime % HPERIOD;
    if (scanLine < 0) scanLine += vdpIsVideoPal(vdp) ? 313 : 262;
    dbgRegisterBankAddRegister(regBank, regOffset++, "VRMP", 16, vdp->vramAddress);
    dbgRegisterBankAddRegister(regBank, regOffset++, "SCAN", 8, scanLine);
    dbgRegisterBankAddRegister(regBank, regOffset++, "LNTM", 16, lineTime);

    // Add IO Ports
    switch (vdp->vdpConnector) {
    case VDP_MSX:
        if (vdp->vdpVersion == VDP_V9938 || vdp->vdpVersion == VDP_V9958) {
            ioPorts = dbgDeviceAddIoPorts(dbgDevice, vdpVersionName, 4);
            dbgIoPortsAddPort(ioPorts, 0, 0x98, DBG_IO_READWRITE,  peek(vdp, 0x98));
            dbgIoPortsAddPort(ioPorts, 1, 0x99, DBG_IO_READWRITE,  peekStatus(vdp, 0x99));
            dbgIoPortsAddPort(ioPorts, 2, 0x9a, DBG_IO_WRITE, 0);
            dbgIoPortsAddPort(ioPorts, 3, 0x9b, DBG_IO_WRITE, 0);
        }
        else {
            ioPorts = dbgDeviceAddIoPorts(dbgDevice, vdpVersionName, 2);
            dbgIoPortsAddPort(ioPorts, 0, 0x98, DBG_IO_READWRITE,  peek(vdp, 0x98));
            dbgIoPortsAddPort(ioPorts, 1, 0x99, DBG_IO_READWRITE,  peekStatus(vdp, 0x99));
        }
        break;

    case VDP_SVI:
        ioPorts = dbgDeviceAddIoPorts(dbgDevice, vdpVersionName, 4);
        dbgIoPortsAddPort(ioPorts, 0, 0x80, DBG_IO_WRITE, 0);
        dbgIoPortsAddPort(ioPorts, 1, 0x81, DBG_IO_WRITE, 0);
        dbgIoPortsAddPort(ioPorts, 2, 0x84, DBG_IO_READ,  peek(vdp, 0x84));
        dbgIoPortsAddPort(ioPorts, 3, 0x85, DBG_IO_READ,  peekStatus(vdp, 0x85));
        break;

    case VDP_COLECO:
        ioPorts = dbgDeviceAddIoPorts(dbgDevice, vdpVersionName, 32);
        for (i = 0; i < 32; i += 2) {
            dbgIoPortsAddPort(ioPorts, 0,     i + 0xa0, DBG_IO_READWRITE, peek(vdp, i + 0xa0));
            dbgIoPortsAddPort(ioPorts, i + 1, i + 0xa1, DBG_IO_READWRITE, peekStatus(vdp, i + 0xa1));
        }
        break;

    case VDP_SG1000:
        ioPorts = dbgDeviceAddIoPorts(dbgDevice, vdpVersionName, 64);
		for (i=0; i<64; i+=2) {
			dbgIoPortsAddPort(ioPorts, 0, i+0x80, DBG_IO_READWRITE,  peek(vdp, i+0x80));
			dbgIoPortsAddPort(ioPorts, 1, i+0x81, DBG_IO_READWRITE,  peekStatus(vdp, i+0x81));
		}
        break;
    }
}

static int dbgWriteMemory(VDP* vdp, char* name, void* data, int start, int size)
{
    if (strcmp(name, "VRAM") || start + size > vdp->vramSize) {
        return 0;
    }

    memcpy(vdp->vram + start, data, size);

    return 1;
}

static int dbgWriteRegister(VDP* vdp, char* name, int regIndex, UInt32 value)
{
    int regCount;
    int cmdRegCount;
    int paletteCount;

    if (vdp->vdpVersion == VDP_V9938) {
        regCount = 24;
        paletteCount = 16;
        cmdRegCount = 15;
    }
    else if (vdp->vdpVersion == VDP_V9958) {
        regCount = 32;
        paletteCount = 16;
        cmdRegCount = 15;
    }
    else {
        regCount = 8;
        paletteCount = 0;
        cmdRegCount = 0;
    }

    if (regIndex < 0) {
        return 0;
    }

    if (regIndex < regCount) {
        vdpUpdateRegisters(vdp, (UInt8)regIndex, (UInt8)value);
        return 1;
    }

    regIndex -= regCount;

    if (regIndex < cmdRegCount) {
        return 0;
    }

    regIndex -= cmdRegCount;

    if (regIndex < paletteCount) {
        value &= 0x0777;
        vdp->paletteReg[regIndex] = (UInt16)value;
        
        updatePalette(vdp, regIndex, (value & 0x70) * 255 / 112, 
                                     (value & 0x07) * 255 / 7,
                                     (value & 0x07) * 255 / 7);
        return 1;
    }

    regIndex -= paletteCount;

    if (regIndex == 0) { // VRMP
        vdp->vramAddress = (UInt16)value & 0x3fff;
    }

    return 0;
}

static void reset(VDP* vdp)
{
    int i;

    vdp->frameStartTime  = boardSystemTime();
    vdp->timeDisplay     = boardSystemTime();
    vdp->scr0splitLine   = 0;
    vdp->vramPage        = 0;
    vdp->sprGenBase      = 0;
    vdp->sprTabBase      = 0;
    vdp->colTabBase      = 0;
    vdp->chrTabBase      = 0;
    vdp->chrGenBase      = 0;
    vdp->palKey          = 0;
    vdp->vdpKey          = 0;
	vdp->vramAddress     = 0;
	vdp->vdpData         = 0;
	vdp->vdpDataLatch    = 0;
    vdp->FGColor         = 0;
    vdp->BGColor         = 0;
    vdp->XFGColor        = 0;
    vdp->XBGColor        = 0;
    vdp->blinkFlag       = 0;
    vdp->blinkCnt        = 0;
    vdp->drawArea        = 0;
    vdp->lastLine        = 0;
    vdp->displayOffest   = 0;
    vdp->screenOn        = 0;
    vdp->VAdjust         = 0;
    vdp->HAdjust         = 0;
    vdp->displayArea        = 0;

    vdp->leftBorder      = 200;
    vdp->screenMode      = 1;
    vdp->lineOffset      = 256;
    vdp->firstLine       = 1;
    vdp->lastLine        = -1;
    vdp->screenOffTime = boardSystemTime();

    memset(vdp->vdpStatus, 0, sizeof(vdp->vdpStatus));
    memset(vdp->vdpRegs, 0, sizeof(vdp->vdpRegs));

    vdp->vdpStatus[0] = 0x9f;
    vdp->vdpStatus[1] = vdp->vdpVersion == VDP_V9958 ? 0x04 : 0;
    vdp->vdpStatus[2] = 0x6c;
        
    vdp->vdpRegs[1] = 0x10;
    vdp->vdpRegs[2] = 0xff;
    vdp->vdpRegs[3] = 0xff;
    vdp->vdpRegs[4] = 0xff;
    vdp->vdpRegs[5] = 0xff;
    vdp->vdpRegs[9] = (0x02 & vdp->palMask) | vdp->palValue;

    if (vdp->vdpVersion == VDP_TMS9929A || vdp->vdpVersion == VDP_TMS99x8A) {
        for (i = 0; i < 16; i++) {
            updatePalette(vdp, i, msx1Palette[i].r, msx1Palette[i].g, msx1Palette[i].b);
        }
    }
    else {
        for (i = 0; i < 16; i++) {
            updatePalette(vdp, i, msx2Palette[i].r, msx2Palette[i].g, msx2Palette[i].b);
        }
    }

    memcpy(vdp->paletteReg, defaultPaletteRegs, sizeof(vdp->paletteReg));

    onScrModeChange(vdp, boardSystemTime());

    onDisplay(vdp, boardSystemTime());
}

static void destroy(VDP* vdp) 
{
    int i;

    theVdp = NULL;

    debugDeviceUnregister(vdp->debugHandle);
    deviceManagerUnregister(vdp->deviceHandle);
    videoManagerUnregister(vdp->videoHandle);

    switch (vdp->vdpConnector) {
    case VDP_MSX:
        ioPortUnregister(0x98);
        ioPortUnregister(0x99);
        ioPortUnregister(0x9a);
        ioPortUnregister(0x9b);
        break;

    case VDP_SVI:
        ioPortUnregister(0x80);
        ioPortUnregister(0x81);
        ioPortUnregister(0x84);
        ioPortUnregister(0x85);
        break;

    case VDP_COLECO:
        for (i = 0xa0; i < 0xc0; i++) {
            ioPortUnregister(i);
        }
        break;

    case VDP_SG1000:
        for (i = 0x80; i < 0xc0; i++) {
            ioPortUnregister(i);
        }
        break;
    }

    boardTimerDestroy(vdp->timerDisplay);
    boardTimerDestroy(vdp->timerDrawAreaStart);
    boardTimerDestroy(vdp->timerVStart);
    boardTimerDestroy(vdp->timerScrModeChange);
    boardTimerDestroy(vdp->timerHint);
    boardTimerDestroy(vdp->timerVint);
    boardTimerDestroy(vdp->timerTmsVint);

    vdpCmdDestroy(vdp->cmdEngine);

    frameBufferDataDestroy(vdp->frameBuffer);

    free(vdp);
}

static void videoEnable(VDP* vdp)
{
    vdp->videoEnabled = 1;
}

static void videoDisable(VDP* vdp)
{
    vdp->videoEnabled = 0;
}

void vdpCreate(VdpConnector connector, VdpVersion version, VdpSyncMode sync, int vramPages)
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, dbgWriteMemory, dbgWriteRegister, NULL };
    VideoCallbacks videoCallbacks = { videoEnable, videoDisable };
    char* vdpVersionString;
    int vramSize;
    int i;

    VDP* vdp = (VDP*)calloc(1, sizeof(VDP));

    theVdp = vdp;

    initPalette(vdp);

    vdp->deviceHandle = deviceManagerRegister(ROM_V9958, &callbacks, vdp);

    vdp->timerDisplay       = boardTimerCreate(onDisplay, vdp);
    vdp->timerDrawAreaStart = boardTimerCreate(onDrawAreaStart, vdp);
    vdp->timerVStart        = boardTimerCreate(onVStart, vdp);
    vdp->timerScrModeChange = boardTimerCreate(onScrModeChange, vdp);
    vdp->timerHint          = boardTimerCreate(onHint, vdp);
    vdp->timerVint          = boardTimerCreate(onVint, vdp);
    vdp->timerTmsVint       = boardTimerCreate(onTmsVint, vdp);

    vdp->RefreshLine = RefreshLine0; 

    vdp->vramSize       = vramPages << 14;

    vdp->vram192        = vramPages == 12;
    vdp->vram16         = vramPages == 1;
    
    if (vramPages < 4) {
        vramPages = 4;
    }

    vramSize            = vramPages << 14;
    vdp->vramOffsets[0] = 0;
    vdp->vramOffsets[1] = vramSize > 0x20000 ? 0x20000 : 0;
    vdp->vramMasks[0]   = vramSize > 0x20000 ? 0x1ffff : vramSize - 1;
    vdp->vramMasks[1]   = vramSize > 0x20000 ? 0xffff  : vramSize - 1;
    vdp->vramPtr        = vdp->vram + vdp->vramOffsets[0];
    vdp->vramAccMask    = vdp->vramMasks[0];
    vdp->vramEnable     = 1;

    if (vramPages > 8) {
        vramPages = 8;
    }

    vdp->vramPages     = vramPages;
    vdp->vram128       = vramPages >= 8 ? 0x10000 : 0;
    vdp->vramMask      = (vramPages << 14) - 1;
    vdp->vdpVersion    = version;
    vdp->vdpConnector  = connector;

    vdp->frameBuffer   = frameBufferDataCreate(8 + 256 + 8, 240, 1);
    vdp->videoHandle   = videoManagerRegister(VdpNames[version], vdp->frameBuffer, &videoCallbacks, vdp);

    if (sync == VDP_SYNC_AUTO) {
        vdp->palMask  = ~0;
        vdp->palValue = 0;
    }
    else if (sync == VDP_SYNC_50HZ) {
        vdp->palMask  = ~0x02;
        vdp->palValue = 0x02;
    }
    else if (sync == VDP_SYNC_60HZ) {
        vdp->palMask  = ~0x02;
        vdp->palValue = 0x00;
    }

    memset(vdp->vram, 0, VRAM_SIZE);
    vdp->cmdEngine = vdpCmdCreate(vramSize, vdp->vram, boardSystemTime());

    reset(vdp);

    switch (vdp->vdpVersion) {
    case VDP_TMS9929A:
        vdp->registerValueMask = registerValueMaskMSX1;
        vdp->registerMask      = 0x07;
        vdpVersionString       = langDbgDevTms9929A();
        break;
    case VDP_TMS99x8A:
        vdp->registerValueMask = registerValueMaskMSX1;
        vdp->registerMask      = 0x07;
        vdp->vdpRegs[9]          &= ~0x02;
        vdpVersionString       = langDbgDevTms99x8A();
        break;
    case VDP_V9938:
        vdp->registerValueMask = registerValueMaskMSX2;
        vdp->registerMask      = 0x3f;
        vdpVersionString       = langDbgDevV9938();
        break;
    case VDP_V9958:
        vdp->registerValueMask = registerValueMaskMSX2p;
        vdp->registerMask      = 0x3f;
        vdpVersionString       = langDbgDevV9958();
        break;
    }
    
    vdp->debugHandle = debugDeviceRegister(DBGTYPE_VIDEO, vdpVersionString, &dbgCallbacks, vdp);

    switch (vdp->vdpConnector) {
    case VDP_MSX:
        ioPortRegister(0x98, read,       write,      vdp);
        ioPortRegister(0x99, readStatus, writeLatch, vdp);
        if (vdp->vdpVersion == VDP_V9938 || vdp->vdpVersion == VDP_V9958) {
            ioPortRegister(0x9a, NULL, writePaletteLatch, vdp);
            ioPortRegister(0x9b, NULL, writeRegister,     vdp);
        }
        break;

    case VDP_SVI:
        ioPortRegister(0x80, NULL,       write,      vdp); // vdp->vdpRegs vdp->vram Write
        ioPortRegister(0x81, NULL,       writeLatch, vdp); // vdp->vdpRegs Address Latch
        ioPortRegister(0x84, read,       NULL,       vdp); // vdp->vdpRegs vdp->vram Read
        ioPortRegister(0x85, readStatus, NULL,       vdp); // vdp->vdpRegs Status Read
        break;

    case VDP_COLECO:
        for (i = 0xa0; i < 0xc0; i += 2) {
            ioPortRegister(i,     read,       write,      vdp);
            ioPortRegister(i + 1, readStatus, writeLatch, vdp);
        }
        break;
        
    case VDP_SG1000:
        ioPortRegister(0xbe, read,       write,      vdp);
        ioPortRegister(0xbf, readStatus, writeLatch, vdp);
        break;
    }
}

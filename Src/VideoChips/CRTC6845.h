/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/CRTC6845.h,v $
**
** $Revision: 1.11 $
**
** $Date: 2006/06/13 17:13:28 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Tomas Karlsson
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
#ifndef CRTC_H
#define CRTC_H

#include "MsxTypes.h"
#include "Board.h"
#include "VideoManager.h"

typedef struct
{
    int     mode;
    UInt8   rasterStart;
    UInt8   rasterEnd;
    UInt16  addressStart;
    int     blinkrate;
    UInt32  blinkstart;
} Crtc6845Cursor;

typedef struct
{
    UInt8    address;  // AR
    UInt8    reg[18];  // R0-R17
} Crtc6845Register;

typedef struct
{
    Crtc6845Cursor   cursor;
    Crtc6845Register registers;
    UInt32           frameCounter;
    int              frameRate;
    int              deviceHandle;
    int              debugHandle;
    int              videoHandle;
    int              videoEnabled;
    BoardTimer*      timerDisplay;   
    UInt32           timeDisplay;
    FrameBufferData* frameBufferData;
    UInt8*           vram;
    UInt32           vramMask;
    UInt8*           romData;
    UInt32           romMask;
    int              charWidth;
    int              charSpace;
    int              charsPerLine;
    int              displayWidth;
} CRTC6845;

UInt8 crtcRead(CRTC6845* crtc);
void crtcWrite(CRTC6845* crtc, UInt8 value);
void crtcWriteLatch(CRTC6845* crtc, UInt8 value);

void crtcMemWrite(CRTC6845* crtc, UInt16 address, UInt8 value);
UInt8 crtcMemRead(CRTC6845* crtc, UInt16 address);

CRTC6845* crtc6845Create(int frameRate, UInt8* romData, int size, int vramSize, 
                         int charWidth, int charSpace, int charsPerLine, int borderChars);

#endif

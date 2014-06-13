/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/Common.h,v $
**
** $Revision: 1.26 $
**
** $Date: 2006/07/04 07:49:04 $
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
#include "SpriteLine.h"

#define BORDER_WIDTH   8
#define DISPLAY_WIDTH  256
#define SCREEN_WIDTH   (2 * BORDER_WIDTH + DISPLAY_WIDTH)
#define SCREEN_HEIGHT  240

static UInt8 emptylineBuf[512];

UInt16 *RefreshBorder(VDP* vdp, int Y, UInt16 bgColor, int line512, int borderExtra)
{
    FrameBuffer* frameBuffer = frameBufferGetDrawFrame();
    int lineSize = line512 ? 2 : 1;
    UInt16 *linePtr;
    int offset;

    if (frameBuffer == NULL) {
        return NULL;
    }

    Y -= vdp->displayOffest;

    frameBufferSetScanline(Y);

    linePtr = frameBuffer->line[Y].buffer;

    if (frameBuffer->line[Y].doubleWidth != line512 && line512 == 0) {
        for(offset = 256 + 16; offset < 512 + 16; offset++) {
            linePtr[offset] = 0;
        }
    }

    frameBuffer->line[Y].doubleWidth = line512;

    for (offset = lineSize * (BORDER_WIDTH + vdp->HAdjust + borderExtra) - 1; offset >= 0; offset--) {
        *linePtr++ = bgColor;
    }

    return linePtr;
}


UInt16 *RefreshBorder6(VDP* vdp, int Y, UInt16 bgColor1, UInt16 bgColor2, int line512, int borderExtra)
{
    FrameBuffer* frameBuffer = frameBufferGetDrawFrame();
    int lineSize = line512 ? 2 : 1;
    UInt16 *linePtr;
    int offset;

    if (frameBuffer == NULL) {
        return NULL;
    }

    Y -= vdp->displayOffest;

    frameBufferSetScanline(Y);

    linePtr = frameBuffer->line[Y].buffer;

    if (frameBuffer->line[Y].doubleWidth != line512 && line512 == 0) {
        for(offset = 256 + 16; offset < 512 + 16; offset++) {
            linePtr[offset] = 0;
        }
    }

    frameBuffer->line[Y].doubleWidth = line512;

    for (offset = lineSize * (BORDER_WIDTH + vdp->HAdjust + borderExtra) - 1; offset >= 0; offset -= 2) {
        *linePtr++ = bgColor1;
        *linePtr++ = bgColor2;
    }

    return linePtr;
}

static void RefreshRightBorder(VDP* vdp, int Y, UInt16 bgColor, int line512, int borderExtra) {
    FrameBuffer* frameBuffer = frameBufferGetDrawFrame();
    int lineSize = line512 ? 2 : 1;
    UInt16 *linePtr;
    int offset;

    if (frameBuffer == NULL) {
        return;
    }

    Y -= vdp->displayOffest;

    if (!displayEnable) {
        return;
    }
    
    linePtr = frameBuffer->line[Y].buffer;

    for(offset = lineSize * (BORDER_WIDTH - vdp->HAdjust + borderExtra + 1) - 1; offset > 0; offset--) {
        linePtr[lineSize * SCREEN_WIDTH - offset] = bgColor;
    }
}

static void RefreshRightBorder6(VDP* vdp, int Y, UInt16 bgColor1, UInt16 bgColor2, int line512, int borderExtra) {
    FrameBuffer* frameBuffer = frameBufferGetDrawFrame();
    int lineSize = line512 ? 2 : 1;
    UInt16 *linePtr;
    int offset;

    if (frameBuffer == NULL) {
        return;
    }

    Y -= vdp->displayOffest;

    if (!displayEnable) {
        return;
    }
    
    linePtr = frameBuffer->line[Y].buffer;

    for(offset = lineSize * (BORDER_WIDTH - vdp->HAdjust + borderExtra + 1) - 1; offset > 0; offset-= 2) {
        linePtr[lineSize * SCREEN_WIDTH - offset - 1] = bgColor1;
        linePtr[lineSize * SCREEN_WIDTH - offset] = bgColor2;
    }
}

#if 0
static void RefreshLine0(VDP* vdp, int Y, int X, int X2)
{
    static UInt16* linePtr = NULL;
    static int     border = 0;
    static UInt8*  sprLine = emptylineBuf;
    static UInt8*  charTable;
    static int     patternBase;
    UInt16 color[2];
    int    y;
    int    rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 9);
        border = !vdp->screenOn || !vdp->drawArea;

        if (!border) {
            y = Y - vdp->firstLine + vdpVScroll(vdp) - vdp->scr0splitLine;
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }
    
    X = 40  * (X - 2) / 28;
    X2 = 40 * (X2 - 2) / 28;
    X = X < 0 ? 0 : X; X = X > 40 ? 40 : X;
    X2 = X2 < 0 ? 0 : X2; X2 = X2 > 40 ? 40 : X2;

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr += 6; 
            X++;
        }
    }
    else {
        color[0] = vdp->palette[vdp->BGColor];
        color[1] = vdp->palette[vdp->FGColor];

        charTable = vdp->vram + (vdp->chrTabBase & ((-1 << 12) | (0xc00 + 40 * (y / 8))));
        patternBase = vdp->chrGenBase & ((-1 << 11) | (y & 7));

        while (X < X2) {
            int pattern = vdp->vram[patternBase | ((int)*charTable * 8)];

            linePtr[0] = color[(pattern >> 7) & 1];
            linePtr[1] = color[(pattern >> 6) & 1];
            linePtr[2] = color[(pattern >> 5) & 1];
            linePtr[3] = color[(pattern >> 4) & 1];
            linePtr[4] = color[(pattern >> 3) & 1];
            linePtr[5] = color[(pattern >> 2) & 1];
            charTable++;
            linePtr += 6;
            X++;
        }
    }
    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 7);
    }
}

#else
static void RefreshLine0(VDP* vdp, int Y, int X, int X2)
{
    UInt16* linePtr;
    UInt8*  charTable;
    int     patternBase;
    UInt16  color[2];

    if (X2 >= 32) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 7);
    }

    if (X > 6 || X2 <= 6) {
        return;
    }

    linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 9);

    if (linePtr == NULL) {
        return;
    }

    if (!vdp->screenOn || !vdp->drawArea) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        for (X = 0; X < 40; X++) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr += 6; 
        }
    }
    else {
        Y -= vdp->firstLine;
        Y = Y + vdpVScroll(vdp) - vdp->scr0splitLine;
        charTable = vdp->vram + (vdp->chrTabBase & ((-1 << 12) | (0xc00 + 40 * (Y / 8))));
        patternBase = vdp->chrGenBase & ((-1 << 11) | (Y & 7));
        color[0] = vdp->palette[vdp->BGColor];
        color[1] = vdp->palette[vdp->FGColor];

        for (X = 0; X < 40; X++) {
            int pattern = vdp->vram[patternBase | ((int)*charTable * 8)];
            linePtr[0] = color[(pattern >> 7) & 1];
            linePtr[1] = color[(pattern >> 6) & 1];
            linePtr[2] = color[(pattern >> 5) & 1];
            linePtr[3] = color[(pattern >> 4) & 1];
            linePtr[4] = color[(pattern >> 3) & 1];
            linePtr[5] = color[(pattern >> 2) & 1];
            charTable++;
            linePtr += 6;
        }
    }
}
#endif

static void RefreshLine0Plus(VDP* vdp, int Y, int X, int X2)
{
    UInt16* linePtr;
    UInt8*  charTable;
    int     patternBase;
    UInt16  color[2];

    if (X2 >= 32) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 7);
    }

    if (X > 6 || X2 <= 6) {
        return;
    }

    linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 9);

    if (linePtr == NULL) {
        return;
    }

    if (!vdp->screenOn || !vdp->drawArea) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        for (X = 0; X < 40; X++) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr += 6; 
        }
    }
    else {
        Y -= vdp->firstLine;
        Y = Y + vdpVScroll(vdp) - vdp->scr0splitLine;
        charTable = vdp->vram + (vdp->chrTabBase & ((-1 << 12) | (0xc00 + 40 * (Y / 8))));
        patternBase =  (-1 << 13) | ((Y & 0xc0) << 5) | (Y & 7);
        color[0] = vdp->palette[vdp->BGColor];
        color[1] = vdp->palette[vdp->FGColor];

        for (X = 0; X < 40; X++) {
            int pattern = vdp->vram[vdp->chrGenBase & (patternBase | ((int)*charTable * 8))];
            linePtr[0] = color[(pattern >> 7) & 1];
            linePtr[1] = color[(pattern >> 6) & 1];
            linePtr[2] = color[(pattern >> 5) & 1];
            linePtr[3] = color[(pattern >> 4) & 1];
            linePtr[4] = color[(pattern >> 3) & 1];
            linePtr[5] = color[(pattern >> 2) & 1];
            charTable++;
            linePtr += 6;
        }
    }
}


static void RefreshLineTx80(VDP* vdp, int Y, int X, int X2)
{
    UInt16* linePtr;
    UInt8   colPattern;
    UInt8*  charTable;
    int     patternBase;
    UInt8*  colTable;
    UInt16  colors[4];

    if (X2 >= 32) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 1, 7);
    }

    if (X > 6 || X2 <= 6) {
        return;
    }

    linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 1, 9);
    
    if (linePtr == NULL) {
        return;
    }

    if (!vdp->screenOn || !vdp->drawArea) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        for (X = 0; X < 80; X++) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr += 6; 
        }
    }
    else {
        Y -= vdp->firstLine;
        Y = Y + vdpVScroll(vdp) - vdp->scr0splitLine;

        charTable   = vdp->vram + (vdp->chrTabBase & ((-1 << 12) | (80 * (Y / 8))));
        patternBase = vdp->chrGenBase & ((-1 << 11) | (Y & 7));
        colTable    = vdp->vram + (vdp->colTabBase & ((-1 << 9) | (10 * (Y / 8))));
        colors[0]   = vdp->palette[vdp->BGColor];
        colors[1]   = vdp->palette[vdp->FGColor];
        colors[2]   = vdp->palette[vdp->XBGColor];
        colors[3]   = vdp->palette[vdp->XFGColor];

        for(X = 0; X < 80; X++) {
            int charPattern = vdp->vram[patternBase | ((int)*charTable * 8)];
            UInt16* color;
            if ((X & 0x07) == 0) {
                colPattern = *colTable++;
            }
            color = colors + ((colPattern >> 6) & 2);
            colPattern <<= 1;

            linePtr[0] = color[(charPattern >> 7) & 1];
            linePtr[1] = color[(charPattern >> 6) & 1];
            linePtr[2] = color[(charPattern >> 5) & 1];
            linePtr[3] = color[(charPattern >> 4) & 1];
            linePtr[4] = color[(charPattern >> 3) & 1];
            linePtr[5] = color[(charPattern >> 2) & 1];
            charTable++;
            linePtr += 6;
        }
    }
}

static void RefreshLine1(VDP* vdp, int Y, int X, int X2)
{
    static UInt16* linePtr = NULL;
    static int     border = 0;
    static UInt8*  sprLine = emptylineBuf;
    static UInt8*  charTable;
    static int     patternBase;
    UInt8  charPattern;
    UInt8  colPattern;
    UInt8  col;
    UInt16 color[2];
    int    y;
    int    rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = spritesLine(vdp, Y);

        if (!border) {
            y = Y - vdp->firstLine + vdpVScroll(vdp);
            charTable   = vdp->vram + (vdp->chrTabBase & ((-1 << 10) | (32 * (y / 8))));
            patternBase = vdp->chrGenBase & ((-1 << 11) | (y & 7));
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        while (X < X2) {
            colPattern = vdp->vram[vdp->colTabBase & ((*charTable / 8) | (-1 << 6))];
            color[0] = vdp->palette[colPattern & 0x0f];
            color[1] = vdp->palette[colPattern >> 4];
            charPattern = vdp->vram[patternBase | ((int)*charTable * 8)];

            if (sprLine != NULL) {
                col = sprLine[0]; linePtr[0] = col ? vdp->palette[col] : color[(charPattern >> 7) & 1]; 
                col = sprLine[1]; linePtr[1] = col ? vdp->palette[col] : color[(charPattern >> 6) & 1];
                col = sprLine[2]; linePtr[2] = col ? vdp->palette[col] : color[(charPattern >> 5) & 1]; 
                col = sprLine[3]; linePtr[3] = col ? vdp->palette[col] : color[(charPattern >> 4) & 1];
                col = sprLine[4]; linePtr[4] = col ? vdp->palette[col] : color[(charPattern >> 3) & 1]; 
                col = sprLine[5]; linePtr[5] = col ? vdp->palette[col] : color[(charPattern >> 2) & 1];
                col = sprLine[6]; linePtr[6] = col ? vdp->palette[col] : color[(charPattern >> 1) & 1]; 
                col = sprLine[7]; linePtr[7] = col ? vdp->palette[col] : color[(charPattern >> 0) & 1];
                sprLine += 8;
            }
            else {
                linePtr[0] = color[(charPattern >> 7) & 1];
                linePtr[1] = color[(charPattern >> 6) & 1];
                linePtr[2] = color[(charPattern >> 5) & 1];
                linePtr[3] = color[(charPattern >> 4) & 1];
                linePtr[4] = color[(charPattern >> 3) & 1];
                linePtr[5] = color[(charPattern >> 2) & 1];
                linePtr[6] = color[(charPattern >> 1) & 1];
                linePtr[7] = color[(charPattern >> 0) & 1];
            }
            charTable++; 
            linePtr += 8; 
            X++;
        }
    }
    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
    }
}


static void RefreshLine2(VDP* vdp, int Y, int X, int X2)
{
    static UInt16* linePtr = NULL;
    static int     border = 0;
    static UInt8*  sprLine = emptylineBuf;
    static UInt8*  charTable = NULL;
    static int     patternBase = 0;
    static int     base = 0;
    UInt8  charPattern;
    UInt8  colPattern;
    UInt8  col;
    UInt16 color[2];
    int    index;
    int    y;
    int    rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y,  vdp->palette[vdp->BGColor], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = spritesLine(vdp, Y);
        
        if (!border) {
            y = Y - vdp->firstLine + vdpVScroll(vdp);
            charTable   = vdp->vram + (vdp->chrTabBase & ((-1 << 10) | (32 * (y / 8))));
            base        = (-1 << 13) | ((y & 0xc0) << 5) | (y & 7);
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        while (X < X2) {
            index      = base | ((int)*charTable * 8);
            colPattern = vdp->vram[vdp->colTabBase & index];
            color[0]   = vdp->palette[colPattern & 0x0f];
            color[1]   = vdp->palette[colPattern >> 4];
            charPattern = vdp->vram[vdp->chrGenBase & index];

            if (sprLine != NULL) {
                col = sprLine[0]; linePtr[0] = col ? vdp->palette[col] : color[(charPattern >> 7) & 1]; 
                col = sprLine[1]; linePtr[1] = col ? vdp->palette[col] : color[(charPattern >> 6) & 1];
                col = sprLine[2]; linePtr[2] = col ? vdp->palette[col] : color[(charPattern >> 5) & 1];
                col = sprLine[3]; linePtr[3] = col ? vdp->palette[col] : color[(charPattern >> 4) & 1];
                col = sprLine[4]; linePtr[4] = col ? vdp->palette[col] : color[(charPattern >> 3) & 1];
                col = sprLine[5]; linePtr[5] = col ? vdp->palette[col] : color[(charPattern >> 2) & 1];
                col = sprLine[6]; linePtr[6] = col ? vdp->palette[col] : color[(charPattern >> 1) & 1];
                col = sprLine[7]; linePtr[7] = col ? vdp->palette[col] : color[(charPattern >> 0) & 1];
                sprLine += 8;
            }
            else {
                linePtr[0] = color[(charPattern >> 7) & 1];
                linePtr[1] = color[(charPattern >> 6) & 1];
                linePtr[2] = color[(charPattern >> 5) & 1];
                linePtr[3] = color[(charPattern >> 4) & 1];
                linePtr[4] = color[(charPattern >> 3) & 1];
                linePtr[5] = color[(charPattern >> 2) & 1];
                linePtr[6] = color[(charPattern >> 1) & 1];
                linePtr[7] = color[(charPattern >> 0) & 1];
            }

            charTable++;
            linePtr += 8; 
            X++;
        }
    }

    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
    }
}

static void RefreshLine3(VDP* vdp, int Y, int X, int X2)
{
    static UInt16* linePtr = NULL;
    static int     border = 0;
    static UInt8*  sprLine = emptylineBuf;
    static UInt8*  charTable;
    static int     patternBase;
    UInt8  colPattern;
    UInt16 fc;
    UInt16 bc;
    UInt8 col;
    int   y;
    int    rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = spritesLine(vdp, Y);

        if (!border) {
            y = Y - vdp->firstLine + vdpVScroll(vdp);
            charTable   = vdp->vram + (vdp->chrTabBase & ((-1 << 10) | (32 * (y / 8))));
            patternBase = vdp->chrGenBase & ((-1 << 11) | ((y >> 2) & 7));
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }


    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        while (X < X2) {
            colPattern = vdp->vram[patternBase | ((int)*charTable * 8)];
            fc = vdp->palette[colPattern >> 4];
            bc = vdp->palette[colPattern & 0x0f];

            if (sprLine != NULL) {
                col = sprLine[0]; linePtr[0] = col ? vdp->palette[col] : fc; 
                col = sprLine[1]; linePtr[1] = col ? vdp->palette[col] : fc;
                col = sprLine[2]; linePtr[2] = col ? vdp->palette[col] : fc; 
                col = sprLine[3]; linePtr[3] = col ? vdp->palette[col] : fc;
                col = sprLine[4]; linePtr[4] = col ? vdp->palette[col] : bc; 
                col = sprLine[5]; linePtr[5] = col ? vdp->palette[col] : bc;
                col = sprLine[6]; linePtr[6] = col ? vdp->palette[col] : bc; 
                col = sprLine[7]; linePtr[7] = col ? vdp->palette[col] : bc;
                sprLine += 8;
            }
            else {
                linePtr[0] = fc; 
                linePtr[1] = fc;
                linePtr[2] = fc; 
                linePtr[3] = fc;
                linePtr[4] = bc; 
                linePtr[5] = bc;
                linePtr[6] = bc; 
                linePtr[7] = bc;
            }

            charTable++; 
            linePtr += 8; 
            X++;
        }
    }
    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
    }
}


static void RefreshLine4(VDP* vdp, int Y, int X, int X2)
{
    static int jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static UInt16* linePtr = NULL;
    static int     border = 0;
    static UInt8*  sprLine = emptylineBuf;
    static UInt8*  charTable;
    static int     base = 0;
    static int     hScroll;
    static int     hScroll512;
    static int*    jump;
    static int     page;
    static int     scroll;
    UInt8  charPattern;
    UInt8  colPattern;
    UInt8  col;
    UInt16 color[2];
    int    index;
    int    rightBorder;

    if (X == -1) {
        int y;

        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = colorSpritesLine(vdp, Y);

        if (linePtr == NULL) {
            return;
        }

        if (!border) {
            hScroll    =  ((((int)(vdp->vdpRegs[26] & 0x3F & ~(~vdpHScroll512(vdp) << 5))) << 3) - (int)(vdp->vdpRegs[27] & 0x07) & 0xffffffff);
            hScroll512 = vdpHScroll512(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page       = (vdp->chrTabBase / 0x8000) & 1;
            scroll     = hScroll >> 3;

            y = Y - vdp->firstLine + vdpVScroll(vdp);
            charTable   = vdp->vram + (vdp->chrTabBase & ((-1 << 10) | (32 * (y / 8)))) + scroll;
            base        = (-1 << 13) | ((y & 0xc0) << 5) | (y & 7);

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->palette[vdp->BGColor];
                linePtr[0] = bgColor;
                linePtr[1] = bgColor;
                linePtr[2] = bgColor;
                linePtr[3] = bgColor;
                linePtr[4] = bgColor;
                linePtr[5] = bgColor;
                linePtr[6] = bgColor;
                linePtr[7] = bgColor;
                charTable++; 
                X++;
                sprLine += sprLine != NULL ? 8 : 0;
                linePtr += 8;
            }

            if (hScroll512) {
                if (scroll & 0x20) {
                    charTable -= 32;
                }
            }

            index       = base | ((int)*charTable * 8);
            colPattern = vdp->vram[vdp->colTabBase & index];
            color[0]   = vdp->palette[colPattern & 0x0f];
            color[1]   = vdp->palette[colPattern >> 4];
            charPattern = vdp->vram[vdp->chrGenBase & index];

            if (sprLine != NULL) {
                switch (hScroll & 7) {
                    case 1: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 6) & 1]; 
                    case 2: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 5) & 1]; 
                    case 3: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 4) & 1]; 
                    case 4: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 3) & 1]; 
                    case 5: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 2) & 1]; 
                    case 6: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 1) & 1]; 
                    case 7: col = *sprLine++; *linePtr++ = col ? vdp->palette[col >> 1] : color[(charPattern >> 0) & 1]; charTable++;
                }
            }
            else {
                switch (hScroll & 7) {
                    case 1: *linePtr++ = color[(charPattern >> 6) & 1]; 
                    case 2: *linePtr++ = color[(charPattern >> 5) & 1]; 
                    case 3: *linePtr++ = color[(charPattern >> 4) & 1]; 
                    case 4: *linePtr++ = color[(charPattern >> 3) & 1]; 
                    case 5: *linePtr++ = color[(charPattern >> 2) & 1]; 
                    case 6: *linePtr++ = color[(charPattern >> 1) & 1]; 
                    case 7: *linePtr++ = color[(charPattern >> 0) & 1];  charTable++;
                }
            }
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        while (X < X2) {
            index       = base | ((int)*charTable * 8);
            colPattern = vdp->vram[vdp->colTabBase & index];
            color[0]   = vdp->palette[colPattern & 0x0f];
            color[1]   = vdp->palette[colPattern >> 4];
            charPattern = vdp->vram[vdp->chrGenBase & index];

            if (sprLine != NULL) {
                col = sprLine[0]; linePtr[0] = col ? vdp->palette[col >> 1] : color[(charPattern >> 7) & 1]; 
                col = sprLine[1]; linePtr[1] = col ? vdp->palette[col >> 1] : color[(charPattern >> 6) & 1];
                col = sprLine[2]; linePtr[2] = col ? vdp->palette[col >> 1] : color[(charPattern >> 5) & 1];
                col = sprLine[3]; linePtr[3] = col ? vdp->palette[col >> 1] : color[(charPattern >> 4) & 1];
                col = sprLine[4]; linePtr[4] = col ? vdp->palette[col >> 1] : color[(charPattern >> 3) & 1];
                col = sprLine[5]; linePtr[5] = col ? vdp->palette[col >> 1] : color[(charPattern >> 2) & 1];
                col = sprLine[6]; linePtr[6] = col ? vdp->palette[col >> 1] : color[(charPattern >> 1) & 1];
                col = sprLine[7]; linePtr[7] = col ? vdp->palette[col >> 1] : color[(charPattern >> 0) & 1];
                sprLine += 8;
            }
            else {
                linePtr[0] = color[(charPattern >> 7) & 1];
                linePtr[1] = color[(charPattern >> 6) & 1];
                linePtr[2] = color[(charPattern >> 5) & 1];
                linePtr[3] = color[(charPattern >> 4) & 1];
                linePtr[4] = color[(charPattern >> 3) & 1];
                linePtr[5] = color[(charPattern >> 2) & 1];
                linePtr[6] = color[(charPattern >> 1) & 1];
                linePtr[7] = color[(charPattern >> 0) & 1];
            }

            charTable++;
            linePtr += 8; 
            X++;
        }
    }
    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
    }
}

static void RefreshLine5(VDP* vdp, int Y, int X, int X2)
{
    static int     jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static int     border = 0;
    static UInt16* linePtr = NULL;
    static UInt8*  charTable;
    static UInt8*  sprLine = emptylineBuf;
    static int     addrSwitch;
    static int     hScroll512;
    static int*    jump;
    static int     page;
    static int     scroll;
    static int     hScroll;
    static int     vscroll;
    static int     chrTabO;
    int col;
    int rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y,  vdp->palette[vdp->BGColor], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine   = colorSpritesLine(vdp, Y);

        if (linePtr == NULL) {
            return;
        }

        if (!border) {
            hScroll512 = vdpHScroll512(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page       = (vdp->chrTabBase / 0x8000) & 1;
            hScroll    = vdpHScroll(vdp);
            vscroll    = vdpVScroll(vdp);
            chrTabO    = vdp->chrTabBase;
            scroll     = hScroll / 2;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll;

            if (hScroll512) {
                if (scroll & 0x80) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }

    #define UPDATE_T() if ((++scroll & 0x7f) == 0) charTable += jump[page ^= 1];

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->palette[vdp->BGColor];
                linePtr[0] = bgColor;
                linePtr[1] = bgColor;
                linePtr[2] = bgColor;
                linePtr[3] = bgColor;
                linePtr[4] = bgColor;
                linePtr[5] = bgColor;
                linePtr[6] = bgColor;
                linePtr[7] = bgColor;

                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                sprLine   += sprLine != NULL ? 8 : 0;
                linePtr += 8;
                charTable += 4;
                X++;
            }
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        // Update vscroll if needed
        if (vscroll != vdpVScroll(vdp) || chrTabO != vdp->chrTabBase) {
            jump       = jumpTable + hScroll512 * 2;
            page       = (vdp->chrTabBase / 0x8000) & 1;
            hScroll    = vdpHScroll(vdp) + X * 8;
            scroll     = hScroll / 2;
            vscroll    = vdpVScroll(vdp);
            chrTabO    = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll;

            if (hScroll512) {
                if (scroll & 0x80) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }
        }
        while (X < X2) {
            if (sprLine != NULL) {
                if (hScroll & 1) {
                    col = sprLine[0]; linePtr[0] = vdp->palette[col? col >> 1 : charTable[0] & 0x0f]; UPDATE_T();
                    col = sprLine[1]; linePtr[1] = vdp->palette[col? col >> 1 : charTable[1] >> 4];
                    col = sprLine[2]; linePtr[2] = vdp->palette[col? col >> 1 : charTable[1] & 0x0f]; UPDATE_T();
                    col = sprLine[3]; linePtr[3] = vdp->palette[col? col >> 1 : charTable[2] >> 4];
                    col = sprLine[4]; linePtr[4] = vdp->palette[col? col >> 1 : charTable[2] & 0x0f]; UPDATE_T();
                    col = sprLine[5]; linePtr[5] = vdp->palette[col? col >> 1 : charTable[3] >> 4];
                    col = sprLine[6]; linePtr[6] = vdp->palette[col? col >> 1 : charTable[3] & 0x0f]; UPDATE_T();
                    col = sprLine[7]; linePtr[7] = vdp->palette[col? col >> 1 : charTable[4] >> 4];
                }
                else {     
                    col = sprLine[0]; linePtr[0] = vdp->palette[col? col >> 1 : charTable[0] >> 4];
                    col = sprLine[1]; linePtr[1] = vdp->palette[col? col >> 1 : charTable[0] & 0x0f]; UPDATE_T();
                    col = sprLine[2]; linePtr[2] = vdp->palette[col? col >> 1 : charTable[1] >> 4];
                    col = sprLine[3]; linePtr[3] = vdp->palette[col? col >> 1 : charTable[1] & 0x0f]; UPDATE_T();
                    col = sprLine[4]; linePtr[4] = vdp->palette[col? col >> 1 : charTable[2] >> 4];
                    col = sprLine[5]; linePtr[5] = vdp->palette[col? col >> 1 : charTable[2] & 0x0f]; UPDATE_T();
                    col = sprLine[6]; linePtr[6] = vdp->palette[col? col >> 1 : charTable[3] >> 4];
                    col = sprLine[7]; linePtr[7] = vdp->palette[col? col >> 1 : charTable[3] & 0x0f]; UPDATE_T();
                }
                sprLine += 8;
            }
            else {
                if (hScroll & 1) {
                    linePtr[0] = vdp->palette[charTable[0] & 0x0f]; UPDATE_T();
                    linePtr[1] = vdp->palette[charTable[1] >> 4];
                    linePtr[2] = vdp->palette[charTable[1] & 0x0f]; UPDATE_T();
                    linePtr[3] = vdp->palette[charTable[2] >> 4];
                    linePtr[4] = vdp->palette[charTable[2] & 0x0f]; UPDATE_T();
                    linePtr[5] = vdp->palette[charTable[3] >> 4];
                    linePtr[6] = vdp->palette[charTable[3] & 0x0f]; UPDATE_T();
                    linePtr[7] = vdp->palette[charTable[4] >> 4];
                }
                else {     
                    linePtr[0] = vdp->palette[charTable[0] >> 4];
                    linePtr[1] = vdp->palette[charTable[0] & 0x0f]; UPDATE_T();
                    linePtr[2] = vdp->palette[charTable[1] >> 4];
                    linePtr[3] = vdp->palette[charTable[1] & 0x0f]; UPDATE_T();
                    linePtr[4] = vdp->palette[charTable[2] >> 4];
                    linePtr[5] = vdp->palette[charTable[2] & 0x0f]; UPDATE_T();
                    linePtr[6] = vdp->palette[charTable[3] >> 4];
                    linePtr[7] = vdp->palette[charTable[3] & 0x0f]; UPDATE_T();
                }
            }
        #undef UPDATE_T

            linePtr += 8; 
            charTable += 4;
            X++;
        }
    }

    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 0, 0);
    }
}

static void RefreshLine6(VDP* vdp, int Y, int X, int X2)
{
    static int     jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static int     border = 0;
    static int*    jump;
    static int     hScroll512;
    static int     scroll;
    static int     page;
    static UInt16* linePtr = NULL;
    static UInt8*  charTable;
    static UInt8*  sprLine = emptylineBuf;
    int col;
    int rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder6(vdp, Y, vdp->palette[(vdp->BGColor >> 2) & 0x03], vdp->palette[vdp->BGColor & 0x03], 1, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine   = colorSpritesLine(vdp, Y);

        if (linePtr == NULL) {
            return;
        }

        if (!border) {
            hScroll512 = vdpHScroll512(vdp);
            scroll     = vdpHScroll(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page    = (vdp->chrTabBase / 0x8000) & 1;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }

    #define UPDATE_T() if ((++scroll & 0xff) == 0) charTable += jump[page ^= 1];

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->palette[vdp->BGColor & 0x03];
                linePtr[0]  = linePtr[1]  = bgColor; 
                linePtr[2]  = linePtr[3]  = bgColor; 
                linePtr[4]  = linePtr[5]  = bgColor; 
                linePtr[6]  = linePtr[7]  = bgColor; 
                linePtr[8]  = linePtr[9]  = bgColor; 
                linePtr[10] = linePtr[11] = bgColor; 
                linePtr[12] = linePtr[13] = bgColor; 
                linePtr[14] = linePtr[15] = bgColor; 
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                sprLine += sprLine != NULL ? 8 : 0; 
                linePtr += 16; 
                charTable += 4;
                X++;
            }
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor1 = vdp->palette[(vdp->BGColor >> 2) & 0x03];
        UInt16 bgColor2 = vdp->palette[vdp->BGColor & 0x03];
        while (X < X2) {
            linePtr[0] = bgColor1;
            linePtr[1] = bgColor2;
            linePtr[2] = bgColor1;
            linePtr[3] = bgColor2;
            linePtr[4] = bgColor1;
            linePtr[5] = bgColor2;
            linePtr[6] = bgColor1;
            linePtr[7] = bgColor2;
            linePtr[8] = bgColor1;
            linePtr[9] = bgColor2;
            linePtr[10] = bgColor1;
            linePtr[11] = bgColor2;
            linePtr[12] = bgColor1;
            linePtr[13] = bgColor2;
            linePtr[14] = bgColor1;
            linePtr[15] = bgColor2;
            linePtr += 16; 
            X++;
        }
    }
    else {
        while (X < X2) {
            if (sprLine != NULL) {
                if (scroll & 1) {
                    (col = sprLine[0]) ? linePtr[0]  = linePtr[1]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[0],
                        linePtr[0]  = vdp->palette[(col >> 2) & 3],
                        linePtr[1]  = vdp->palette[(col >> 0) & 3]);
                    UPDATE_T();
                    (col = sprLine[1]) ? linePtr[2]  = linePtr[3]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[1],
                        linePtr[2]  = vdp->palette[(col >> 6) & 3], 
                        linePtr[3]  = vdp->palette[(col >> 4) & 3]);
                    UPDATE_T();
                    (col = sprLine[2]) ? linePtr[4]  = linePtr[5]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[1], 
                        linePtr[4]  = vdp->palette[(col >> 2) & 3], 
                        linePtr[5]  = vdp->palette[(col >> 0) & 3]);
                    UPDATE_T();
                    (col = sprLine[3]) ? linePtr[6]  = linePtr[7]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[2],
                        linePtr[6]  = vdp->palette[(col >> 6) & 3],
                        linePtr[7]  = vdp->palette[(col >> 4) & 3]);
                    UPDATE_T();
                    (col = sprLine[4]) ? linePtr[8]  = linePtr[9]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[2],
                        linePtr[8]  = vdp->palette[(col >> 2) & 3],
                        linePtr[9]  = vdp->palette[(col >> 0) & 3]);
                    UPDATE_T();
                    (col = sprLine[5]) ? linePtr[10] = linePtr[11] = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[3], 
                        linePtr[10] = vdp->palette[(col >> 6) & 3], 
                        linePtr[11] = vdp->palette[(col >> 4) & 3]); 
                    UPDATE_T();
                    (col = sprLine[6]) ? linePtr[12] = linePtr[13] = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[3],
                        linePtr[12] = vdp->palette[(col >> 2) & 3], 
                        linePtr[13] = vdp->palette[(col >> 0) & 3]); 
                    UPDATE_T();
                    (col = sprLine[7]) ? linePtr[14] = linePtr[15] = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[4],
                        linePtr[14] = vdp->palette[(col >> 6) & 3],
                        linePtr[15] = vdp->palette[(col >> 4) & 3]);
                    UPDATE_T();
                }
                else {
                    (col = sprLine[0]) ? linePtr[0]  = linePtr[1]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[0], 
                        linePtr[0]  = vdp->palette[(col >> 6) & 3], 
                        linePtr[1]  = vdp->palette[(col >> 4) & 3]);
                    UPDATE_T();
                    (col = sprLine[1]) ? linePtr[2]  = linePtr[3]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[0], 
                        linePtr[2]  = vdp->palette[(col >> 2) & 3],
                        linePtr[3]  = vdp->palette[(col >> 0) & 3]); 
                    UPDATE_T();
                    (col = sprLine[2]) ? linePtr[4]  = linePtr[5]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[1], 
                        linePtr[4]  = vdp->palette[(col >> 6) & 3], 
                        linePtr[5]  = vdp->palette[(col >> 4) & 3]);
                    UPDATE_T();
                    (col = sprLine[3]) ? linePtr[6]  = linePtr[7]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[1],
                        linePtr[6]  = vdp->palette[(col >> 2) & 3], 
                        linePtr[7]  = vdp->palette[(col >> 0) & 3]);
                    UPDATE_T();
                    (col = sprLine[4]) ? linePtr[8]  = linePtr[9]  = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[2], 
                        linePtr[8]  = vdp->palette[(col >> 6) & 3],
                        linePtr[9]  = vdp->palette[(col >> 4) & 3]);
                    UPDATE_T();
                    (col = sprLine[5]) ? linePtr[10] = linePtr[11] = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[2], 
                        linePtr[10] = vdp->palette[(col >> 2) & 3],
                        linePtr[11] = vdp->palette[(col >> 0) & 3]); 
                    UPDATE_T();
                    (col = sprLine[6]) ? linePtr[12] = linePtr[13] = vdp->palette[(col >> 1) & 3] : 
                        (col = charTable[3],
                        linePtr[12] = vdp->palette[(col >> 6) & 3], 
                        linePtr[13] = vdp->palette[(col >> 4) & 3]); 
                    UPDATE_T();
                    (col = sprLine[7]) ? linePtr[14] = linePtr[15] = vdp->palette[(col >> 1) & 3] :
                        (col = charTable[3],
                        linePtr[14] = vdp->palette[(col >> 2) & 3], 
                        linePtr[15] = vdp->palette[(col >> 0) & 3]);
                    UPDATE_T();
                }
                sprLine += 8; 
            }
            else {
                if (scroll & 1) {
                    col = charTable[0]; 
                    linePtr[0]  = vdp->palette[(col >> 2) & 3]; 
                    linePtr[1]  = vdp->palette[(col >> 0) & 3]; 
                    UPDATE_T();
                    col = charTable[1]; 
                    linePtr[2]  = vdp->palette[(col >> 6) & 3]; 
                    linePtr[3]  = vdp->palette[(col >> 4) & 3];
                    UPDATE_T();
                    col = charTable[1]; 
                    linePtr[4]  = vdp->palette[(col >> 2) & 3];
                    linePtr[5]  = vdp->palette[(col >> 0) & 3];
                    UPDATE_T();
                    col = charTable[2]; 
                    linePtr[6]  = vdp->palette[(col >> 6) & 3]; 
                    linePtr[7]  = vdp->palette[(col >> 4) & 3];
                    UPDATE_T();
                    col = charTable[2]; 
                    linePtr[8]  = vdp->palette[(col >> 2) & 3]; 
                    linePtr[9]  = vdp->palette[(col >> 0) & 3];
                    UPDATE_T();
                    col = charTable[3]; 
                    linePtr[10] = vdp->palette[(col >> 6) & 3]; 
                    linePtr[11] = vdp->palette[(col >> 4) & 3]; 
                    UPDATE_T();
                    col = charTable[3]; 
                    linePtr[12] = vdp->palette[(col >> 2) & 3]; 
                    linePtr[13] = vdp->palette[(col >> 0) & 3];
                    UPDATE_T();
                    col = charTable[4]; 
                    linePtr[14] = vdp->palette[(col >> 6) & 3]; 
                    linePtr[15] = vdp->palette[(col >> 4) & 3]; 
                    UPDATE_T();
                }
                else {
                    col = charTable[0];
                    linePtr[0]  = vdp->palette[(col >> 6) & 3];
                    linePtr[1]  = vdp->palette[(col >> 4) & 3];
                    UPDATE_T();
                    col = charTable[0];
                    linePtr[2]  = vdp->palette[(col >> 2) & 3]; 
                    linePtr[3]  = vdp->palette[(col >> 0) & 3]; 
                    UPDATE_T();
                    col = charTable[1];
                    linePtr[4]  = vdp->palette[(col >> 6) & 3]; 
                    linePtr[5]  = vdp->palette[(col >> 4) & 3]; 
                    UPDATE_T();
                    col = charTable[1]; 
                    linePtr[6]  = vdp->palette[(col >> 2) & 3];
                    linePtr[7]  = vdp->palette[(col >> 0) & 3]; 
                    UPDATE_T();
                    col = charTable[2]; 
                    linePtr[8]  = vdp->palette[(col >> 6) & 3];
                    linePtr[9]  = vdp->palette[(col >> 4) & 3]; 
                    UPDATE_T();
                    col = charTable[2]; 
                    linePtr[10] = vdp->palette[(col >> 2) & 3];
                    linePtr[11] = vdp->palette[(col >> 0) & 3]; 
                    UPDATE_T();
                    col = charTable[3]; 
                    linePtr[12] = vdp->palette[(col >> 6) & 3]; 
                    linePtr[13] = vdp->palette[(col >> 4) & 3]; 
                    UPDATE_T();
                    col = charTable[3]; 
                    linePtr[14] = vdp->palette[(col >> 2) & 3];
                    linePtr[15] = vdp->palette[(col >> 0) & 3]; 
                    UPDATE_T();
                }
            }
        #undef UPDATE_T

            linePtr += 16; 
            charTable += 4;
            X++;
        }
    }
    if (rightBorder) {
        RefreshRightBorder6(vdp, Y, vdp->palette[(vdp->BGColor >> 2) & 0x03], vdp->palette[vdp->BGColor & 0x03], 1, 0);
    }
}

static void RefreshLine7(VDP* vdp, int Y, int X, int X2)
{
    static int     jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static int     border = 0;
    static UInt16* linePtr = NULL;
    static UInt8*  charTable;
    static UInt8*  sprLine = emptylineBuf;
    static int     hScroll512;
    static int     page;
    static int*    jump;
    static int     scroll;
    static int     vscroll;
    static int     chrTabO;
    int col;
    int rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->palette[vdp->BGColor], 1, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = colorSpritesLine(vdp, Y);
    
        if (linePtr == NULL) {
            return;
        }

        if (!border) {
            hScroll512 = vdpHScroll512(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page    = (vdp->chrTabBase / 0x8000) & 1;
            scroll     = vdpHScroll(vdp);
            vscroll    = vdpVScroll(vdp);
            chrTabO    = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

    #define UPDATE_T() if ((++scroll & 0xff) == 0) charTable += jump[page ^= 1];

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->palette[vdp->BGColor];
                linePtr[0]  = linePtr[1]  = bgColor; 
                linePtr[2]  = linePtr[3]  = bgColor; 
                linePtr[4]  = linePtr[5]  = bgColor; 
                linePtr[6]  = linePtr[7]  = bgColor; 
                linePtr[8]  = linePtr[9]  = bgColor; 
                linePtr[10] = linePtr[11] = bgColor; 
                linePtr[12] = linePtr[13] = bgColor; 
                linePtr[14] = linePtr[15] = bgColor; 
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                sprLine   += sprLine != NULL ? 8 : 0;
                linePtr += 16; 
                charTable += 4;
                X++; 
            }
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr[8] = bgColor;
            linePtr[9] = bgColor;
            linePtr[10] = bgColor;
            linePtr[11] = bgColor;
            linePtr[12] = bgColor;
            linePtr[13] = bgColor;
            linePtr[14] = bgColor;
            linePtr[15] = bgColor;
            linePtr += 16; 
            X++;
        }
    }
    else {
        // Update vscroll if needed
        if (vscroll != vdpVScroll(vdp) || chrTabO != vdp->chrTabBase) {
            scroll  = vdpHScroll(vdp) + X * 8;
            page = (vdp->chrTabBase / 0x8000) & 1;
            jump    = jumpTable + hScroll512 * 2;
            vscroll = vdpVScroll(vdp);
            chrTabO  = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }
        }

        while (X < X2) {

            if (sprLine != NULL) {
                if (scroll & 1) {
                    (col = sprLine[0]) ? linePtr[0]  = linePtr[1]  = vdp->palette[col >> 1] : 
                    (col = charTable[vdp->vram128], 
                    linePtr[0]  = vdp->palette[col >> 4],
                    linePtr[1]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[1]) ? linePtr[2]  = linePtr[3]  = vdp->palette[col >> 1] :
                    (col = charTable[1],  
                    linePtr[2]  = vdp->palette[col >> 4],
                    linePtr[3]  = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                    (col = sprLine[2]) ? linePtr[4]  = linePtr[5]  = vdp->palette[col >> 1] : 
                    (col = charTable[vdp->vram128|1],
                    linePtr[4]  = vdp->palette[col >> 4],
                    linePtr[5]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[3]) ? linePtr[6]  = linePtr[7]  = vdp->palette[col >> 1] : 
                    (col = charTable[2],   
                    linePtr[6]  = vdp->palette[col >> 4],
                    linePtr[7]  = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                    (col = sprLine[4]) ? linePtr[8]  = linePtr[9]  = vdp->palette[col >> 1] :
                    (col = charTable[vdp->vram128|2], 
                    linePtr[8]  = vdp->palette[col >> 4], 
                    linePtr[9]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[5]) ? linePtr[10] = linePtr[11] = vdp->palette[col >> 1] : 
                    (col = charTable[3],      
                    linePtr[10] = vdp->palette[col >> 4],
                    linePtr[11] = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                    (col = sprLine[6]) ? linePtr[12] = linePtr[13] = vdp->palette[col >> 1] : 
                    (col = charTable[vdp->vram128|3], 
                    linePtr[12] = vdp->palette[col >> 4], 
                    linePtr[13] = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[7]) ? linePtr[14] = linePtr[15] = vdp->palette[col >> 1] : 
                    (col = charTable[4],  
                    linePtr[14] = vdp->palette[col >> 4], 
                    linePtr[15] = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                }
                else {
                    (col = sprLine[0]) ? linePtr[0]  = linePtr[1]  = vdp->palette[col >> 1] : 
                    (col = charTable[0],      
                    linePtr[0]  = vdp->palette[col >> 4],
                    linePtr[1]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[1]) ? linePtr[2]  = linePtr[3]  = vdp->palette[col >> 1] : 
                    (col = charTable[vdp->vram128], 
                    linePtr[2]  = vdp->palette[col >> 4],
                    linePtr[3]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[2]) ? linePtr[4]  = linePtr[5]  = vdp->palette[col >> 1] :
                    (col = charTable[1],    
                    linePtr[4]  = vdp->palette[col >> 4], 
                    linePtr[5]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[3]) ? linePtr[6]  = linePtr[7]  = vdp->palette[col >> 1] :
                    (col = charTable[vdp->vram128|1],
                    linePtr[6]  = vdp->palette[col >> 4], 
                    linePtr[7]  = vdp->palette[col & 0xf]);
                    UPDATE_T();
                    (col = sprLine[4]) ? linePtr[8]  = linePtr[9]  = vdp->palette[col >> 1] : 
                    (col = charTable[2],      
                    linePtr[8]  = vdp->palette[col >> 4], 
                    linePtr[9]  = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                    (col = sprLine[5]) ? linePtr[10] = linePtr[11] = vdp->palette[col >> 1] :
                    (col = charTable[vdp->vram128|2], 
                    linePtr[10] = vdp->palette[col >> 4], 
                    linePtr[11] = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                    (col = sprLine[6]) ? linePtr[12] = linePtr[13] = vdp->palette[col >> 1] : 
                    (col = charTable[3],   
                    linePtr[12] = vdp->palette[col >> 4], 
                    linePtr[13] = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                    (col = sprLine[7]) ? linePtr[14] = linePtr[15] = vdp->palette[col >> 1] : 
                    (col = charTable[vdp->vram128|3], 
                    linePtr[14] = vdp->palette[col >> 4], 
                    linePtr[15] = vdp->palette[col & 0xf]); 
                    UPDATE_T();
                }
                sprLine += 8; 
            }
            else {
                if (scroll & 1) {
                    col = charTable[vdp->vram128];  
                    linePtr[0]  = vdp->palette[col >> 4]; 
                    linePtr[1]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[1];
                    linePtr[2]  = vdp->palette[col >> 4]; 
                    linePtr[3]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[vdp->vram128|1];
                    linePtr[4]  = vdp->palette[col >> 4];
                    linePtr[5]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[2];
                    linePtr[6]  = vdp->palette[col >> 4];
                    linePtr[7]  = vdp->palette[col & 0xf]; 
                    UPDATE_T();
                    col = charTable[vdp->vram128|2];
                    linePtr[8]  = vdp->palette[col >> 4]; 
                    linePtr[9]  = vdp->palette[col & 0xf]; 
                    UPDATE_T();
                    col = charTable[3];
                    linePtr[10] = vdp->palette[col >> 4]; 
                    linePtr[11] = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[vdp->vram128|3];
                    linePtr[12] = vdp->palette[col >> 4]; 
                    linePtr[13] = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[4];
                    linePtr[14] = vdp->palette[col >> 4]; 
                    linePtr[15] = vdp->palette[col & 0xf];
                    UPDATE_T();
                }
                else {
                    col = charTable[0];
                    linePtr[0]  = vdp->palette[col >> 4];
                    linePtr[1]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[vdp->vram128];
                    linePtr[2]  = vdp->palette[col >> 4];
                    linePtr[3]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[1];
                    linePtr[4]  = vdp->palette[col >> 4];
                    linePtr[5]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[vdp->vram128|1];
                    linePtr[6]  = vdp->palette[col >> 4];
                    linePtr[7]  = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[2];
                    linePtr[8]  = vdp->palette[col >> 4];
                    linePtr[9]  = vdp->palette[col & 0xf]; 
                    UPDATE_T();
                    col = charTable[vdp->vram128|2];
                    linePtr[10] = vdp->palette[col >> 4];
                    linePtr[11] = vdp->palette[col & 0xf]; 
                    UPDATE_T();
                    col = charTable[3];
                    linePtr[12] = vdp->palette[col >> 4];
                    linePtr[13] = vdp->palette[col & 0xf];
                    UPDATE_T();
                    col = charTable[vdp->vram128|3];
                    linePtr[14] = vdp->palette[col >> 4];
                    linePtr[15] = vdp->palette[col & 0xf];
                    UPDATE_T();
                }
            }
        #undef UPDATE_T

            linePtr += 16; 
            charTable += 4;
            X++;
        }
    }    
    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->palette[vdp->BGColor], 1, 0);
    }
}

static void RefreshLine8(VDP* vdp, int Y, int X, int X2)
{
    static UInt8   SprToScr[16] = { 
        0x00, 0x02, 0x10, 0x12, 0x80, 0x82, 0x90, 0x92, 
        0x49, 0x4B, 0x59, 0x5B, 0xC9, 0xCB, 0xD9, 0xDB 
    };
    static int     jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static int     border = 0;
    static UInt16* linePtr = NULL;
    static UInt8*  charTable;
    static UInt8*  sprLine = emptylineBuf;
    static int     hScroll;
    static int     hScroll512;
    static int*    jump;
    static int     page;
    static int     scroll;
    static int     vscroll;
    static int     chrTabO;
    int col;
    int rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->paletteFixed[vdp->vdpRegs[7]], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = colorSpritesLine(vdp, Y);

        if (linePtr == NULL) {
            return;
        }

        if (!border) {        
            hScroll    = vdpHScroll(vdp);
            hScroll512 = vdpHScroll512(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page    = (vdp->chrTabBase / 0x8000) & 1;
            scroll     = hScroll;
            vscroll    = vdpVScroll(vdp);
            chrTabO    = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

    #define UPDATE_T() if ((++scroll & 0xff) == 0) charTable += jump[page ^= 1];

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->paletteFixed[vdp->vdpRegs[7]];
                linePtr[0] = bgColor;
                linePtr[1] = bgColor;
                linePtr[2] = bgColor;
                linePtr[3] = bgColor;
                linePtr[4] = bgColor;
                linePtr[5] = bgColor;
                linePtr[6] = bgColor;
                linePtr[7] = bgColor;
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                sprLine   += sprLine != NULL ? 8 : 0; 
                charTable += 4;
                linePtr += 8; 
                X++; 
            }
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->paletteFixed[vdp->vdpRegs[7]];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        // Update vscroll if needed
        if (vscroll != vdpVScroll(vdp) || chrTabO != vdp->chrTabBase) {
            scroll = vdpHScroll(vdp) + X * 8;
            jump   = jumpTable + hScroll512 * 2;
            vscroll = vdpVScroll(vdp);
            chrTabO = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }
        }

        while (X < X2) {
            if (sprLine != NULL) {
                if (scroll & 1) {
                    col = sprLine[0]; linePtr[0] = col ? vdp->paletteSprite8[col >> 1] : 
                vdp->paletteFixed[charTable[vdp->vram128]]; UPDATE_T();
                    col = sprLine[1]; linePtr[1] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[1]]; UPDATE_T();
                    col = sprLine[2]; linePtr[2] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128|1]]; UPDATE_T();
                    col = sprLine[3]; linePtr[3] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[2]]; UPDATE_T();
                    col = sprLine[4]; linePtr[4] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128|2]]; UPDATE_T();
                    col = sprLine[5]; linePtr[5] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[3]]; UPDATE_T();
                    col = sprLine[6]; linePtr[6] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128|3]]; UPDATE_T();
                    col = sprLine[7]; linePtr[7] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[4]]; UPDATE_T();
                }
                else {
                    col = sprLine[0]; linePtr[0] = col ? vdp->paletteSprite8[col >> 1] : 
                vdp->paletteFixed[charTable[0]]; UPDATE_T();
                    col = sprLine[1]; linePtr[1] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128]]; UPDATE_T();
                    col = sprLine[2]; linePtr[2] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[1]]; UPDATE_T();
                    col = sprLine[3]; linePtr[3] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128|1]]; UPDATE_T();
                    col = sprLine[4]; linePtr[4] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[2]]; UPDATE_T();
                    col = sprLine[5]; linePtr[5] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128|2]]; UPDATE_T();
                    col = sprLine[6]; linePtr[6] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[3]]; UPDATE_T();
                    col = sprLine[7]; linePtr[7] = col ? vdp->paletteSprite8[col >> 1] : 
                    vdp->paletteFixed[charTable[vdp->vram128|3]]; UPDATE_T();
                }
                sprLine += 8; 
            }
            else {
                if (scroll & 1) {
                    linePtr[0] = vdp->paletteFixed[charTable[vdp->vram128]];   UPDATE_T();
                    linePtr[1] = vdp->paletteFixed[charTable[1]];         UPDATE_T();
                    linePtr[2] = vdp->paletteFixed[charTable[vdp->vram128|1]]; UPDATE_T();
                    linePtr[3] = vdp->paletteFixed[charTable[2]];         UPDATE_T();
                    linePtr[4] = vdp->paletteFixed[charTable[vdp->vram128|2]]; UPDATE_T();
                    linePtr[5] = vdp->paletteFixed[charTable[3]];         UPDATE_T();
                    linePtr[6] = vdp->paletteFixed[charTable[vdp->vram128|3]]; UPDATE_T();
                    linePtr[7] = vdp->paletteFixed[charTable[4]];         UPDATE_T();
                }
                else {
                    linePtr[0] = vdp->paletteFixed[charTable[0]];         UPDATE_T();
                    linePtr[1] = vdp->paletteFixed[charTable[vdp->vram128]];   UPDATE_T();
                    linePtr[2] = vdp->paletteFixed[charTable[1]];         UPDATE_T();
                    linePtr[3] = vdp->paletteFixed[charTable[vdp->vram128|1]]; UPDATE_T();
                    linePtr[4] = vdp->paletteFixed[charTable[2]];         UPDATE_T();
                    linePtr[5] = vdp->paletteFixed[charTable[vdp->vram128|2]]; UPDATE_T();
                    linePtr[6] = vdp->paletteFixed[charTable[3]];         UPDATE_T();
                    linePtr[7] = vdp->paletteFixed[charTable[vdp->vram128|3]]; UPDATE_T();
                }
            }
        #undef UPDATE_T

            charTable += 4; linePtr += 8; X++;
        }
    }

    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->paletteFixed[vdp->vdpRegs[7]], 0, 0);
    }
}

static void RefreshLine10(VDP* vdp, int Y, int X, int X2)
{
    static int jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static int     border = 0;
    static UInt16* linePtr = NULL;
    static UInt8* charTable;
    static UInt8* sprLine = emptylineBuf;
    static int hScroll512;
    static int* jump;
    static int page;
    static int scroll;
    static int vscroll;
    static int hscroll;
    static int chrTabO;
    int col;
    UInt8 t0, t1, t2, t3;
    int y, J, K;
    int rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->paletteFixed[vdp->vdpRegs[7]], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = colorSpritesLine(vdp, Y);

        if (linePtr == NULL) {
            return;
        }

        if (!border) {        
            hScroll512 = vdpHScroll512(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page    = (vdp->chrTabBase / 0x8000) & 1;
            hscroll    = vdpHScroll(vdp);
            scroll     = hscroll & ~3;
            vscroll    = vdpVScroll(vdp);
            chrTabO    = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

    #define UPDATE_T() if ((++scroll & 0xff) == 0) charTable += jump[page ^= 1];

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->paletteFixed[vdp->vdpRegs[7]];
                linePtr[0] = bgColor;
                linePtr[1] = bgColor;
                linePtr[2] = bgColor;
                linePtr[3] = bgColor;
                linePtr[4] = bgColor;
                linePtr[5] = bgColor;
                linePtr[6] = bgColor;
                linePtr[7] = bgColor;
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                sprLine   += sprLine != NULL ? 8 : 0; 
                charTable += 4; 
                linePtr += 8;
                X++; 
            }

            t0 = charTable[0];        UPDATE_T();
            t1 = charTable[vdp->vram128];  UPDATE_T();
            t2 = charTable[1];        UPDATE_T();
            t3 = charTable[vdp->vram128|1];  UPDATE_T();


            K=(t0 & 0x07) | ((t1 & 0x07) << 3);
            J=(t2 & 0x07) | ((t3 & 0x07) << 3);

            if (sprLine != NULL) {
                switch (hscroll & 3) {
                case 0:
                    col = sprLine[0]; y = t0 >> 3; *linePtr++ = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                case 1:
                    col = sprLine[1]; y = t1 >> 3; *linePtr++ = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                case 2:
                    col = sprLine[2]; y = t2 >> 3; *linePtr++ = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                case 3:
                    col = sprLine[3]; y = t3 >> 3; *linePtr++ = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                }
                sprLine += 4;
            }
            else {
                switch (hscroll & 3) {
                case 0:
                    y = t0 >> 3; *linePtr++ = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                case 1:
                    y = t1 >> 3; *linePtr++ = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                case 2:
                    y = t2 >> 3; *linePtr++ = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                case 3:
                    y = t3 >> 3; *linePtr++ = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                }
            }
            charTable += 2;
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->palette[vdp->BGColor];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        // Update vscroll if needed
        if (vscroll != vdpVScroll(vdp) || chrTabO != vdp->chrTabBase) {
            hscroll = vdpHScroll(vdp);
            scroll = (hscroll & ~3) + X * 8;
            jump   = jumpTable + hScroll512 * 2;
            vscroll = vdpVScroll(vdp);
            chrTabO  = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;
            charTable += 2; 

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }
        }

        while (X < X2) {
            t0 = charTable[0];         UPDATE_T();
            t1 = charTable[vdp->vram128];   UPDATE_T();
            t2 = charTable[1];         UPDATE_T();
            t3 = charTable[vdp->vram128|1]; UPDATE_T();

            K=(t0 & 0x07) | ((t1 & 0x07) << 3);
            J=(t2 & 0x07) | ((t3 & 0x07) << 3);

            if (sprLine != NULL) {
                col = sprLine[0]; y = t0 >> 3; linePtr[0] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                col = sprLine[1]; y = t1 >> 3; linePtr[1] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                col = sprLine[2]; y = t2 >> 3; linePtr[2] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                col = sprLine[3]; y = t3 >> 3; linePtr[3] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
            }
            else {
                y = t0 >> 3; linePtr[0] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                y = t1 >> 3; linePtr[1] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                y = t2 >> 3; linePtr[2] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                y = t3 >> 3; linePtr[3] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
            }

            t0 = charTable[2];        UPDATE_T();
            t1 = charTable[vdp->vram128|2];  UPDATE_T();
            t2 = charTable[3];        UPDATE_T();
            t3 = charTable[vdp->vram128|3];  UPDATE_T();

            K=(t0 & 0x07) | ((t1 & 0x07) << 3);
            J=(t2 & 0x07) | ((t3 & 0x07) << 3);

            if (X == 31) {
                if (sprLine != NULL) {
                    switch (hscroll & 3) {
                    case 1:
                        y = t2 >> 3; col = sprLine[6]; linePtr[6] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    case 2:
                        y = t1 >> 3; col = sprLine[5]; linePtr[5] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    case 3:
                        y = t0 >> 3; col = sprLine[4]; linePtr[4] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    }
                }
                else {
                    switch (hscroll & 3) {
                    case 1:
                        y = t2 >> 3; linePtr[6] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    case 2:
                        y = t1 >> 3; linePtr[5] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    case 3:
                        y = t0 >> 3; linePtr[4] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    }
                }
            }
            else {
                if (sprLine != NULL) {
                    col = sprLine[4]; y = t0 >> 3; linePtr[4] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    col = sprLine[5]; y = t1 >> 3; linePtr[5] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    col = sprLine[6]; y = t2 >> 3; linePtr[6] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    col = sprLine[7]; y = t3 >> 3; linePtr[7] = col ? vdp->palette[col >> 1] : y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    sprLine += 8; 
                }
                else {
                    y = t0 >> 3; linePtr[4] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    y = t1 >> 3; linePtr[5] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    y = t2 >> 3; linePtr[6] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                    y = t3 >> 3; linePtr[7] = y & 1 ? vdp->palette[y >> 1] : vdp->yjkColor[y][J][K];
                }
            }
        #undef UPDATE_T

            charTable += 4; linePtr += 8;
            X++;
        }
    }

    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->paletteFixed[vdp->vdpRegs[7]], 0, 0);
    }
}

static void RefreshLine12(VDP* vdp, int Y, int X, int X2)
{
    static int jumpTable[] = { -128, -128, -0x8080, 0x7f80 };
    static int     border = 0;
    static UInt16* linePtr = NULL;
    static UInt8* charTable;
    static UInt8* sprLine = emptylineBuf;
    static int hScroll512;
    static int* jump;
    static int page;
    static int scroll;
    static int vscroll;
    static int hscroll;
    static int chrTabO;
    int col;
    UInt8 t0, t1, t2, t3;
    int J, K;
    int rightBorder;

    if (X == -1) {
        X++;
        linePtr = RefreshBorder(vdp, Y, vdp->paletteFixed[vdp->vdpRegs[7]], 0, 0);
        border = !vdp->screenOn || !vdp->drawArea;
        sprLine = colorSpritesLine(vdp, Y);

        if (linePtr == NULL) {
            return;
        }

        if (!border) {
            hScroll512 = vdpHScroll512(vdp);
            jump       = jumpTable + hScroll512 * 2;
            page    = (vdp->chrTabBase / 0x8000) & 1;
            hscroll    = vdpHScroll(vdp);
            scroll     = hscroll & ~3;
            vscroll    = vdpVScroll(vdp);
            chrTabO    = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;

    #define UPDATE_T() if ((++scroll & 0xff) == 0) charTable += jump[page ^= 1];

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }

            if (vdpIsEdgeMasked(vdp->vdpRegs)) {
                UInt16 bgColor = vdp->paletteFixed[vdp->vdpRegs[7]];
                linePtr[0] = bgColor;
                linePtr[1] = bgColor;
                linePtr[2] = bgColor;
                linePtr[3] = bgColor;
                linePtr[4] = bgColor;
                linePtr[5] = bgColor;
                linePtr[6] = bgColor;
                linePtr[7] = bgColor;
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                UPDATE_T(); UPDATE_T(); UPDATE_T(); UPDATE_T();
                sprLine   += sprLine != NULL ? 8 : 0; 
                charTable += 4; 
                linePtr += 8;
                X++; 
            }

            t0 = charTable[0];        UPDATE_T();
            t1 = charTable[vdp->vram128];  UPDATE_T();
            t2 = charTable[1];        UPDATE_T();
            t3 = charTable[vdp->vram128|1];  UPDATE_T();

            K=(t0 & 0x07) | ((t1 & 0x07) << 3);
            J=(t2 & 0x07) | ((t3 & 0x07) << 3);

            if (sprLine != NULL) {
                switch (hscroll & 3) {
                case 0:
                    col = sprLine[0]; *linePtr++ = col ? vdp->palette[col >> 1] : vdp->yjkColor[t0 >> 3][J][K];
                case 1:
                    col = sprLine[1]; *linePtr++ = col ? vdp->palette[col >> 1] : vdp->yjkColor[t1 >> 3][J][K];
                case 2:
                    col = sprLine[2]; *linePtr++ = col ? vdp->palette[col >> 1] : vdp->yjkColor[t2 >> 3][J][K];
                case 3:
                    col = sprLine[3]; *linePtr++ = col ? vdp->palette[col >> 1] : vdp->yjkColor[t3 >> 3][J][K];
                }
                sprLine += 4;
            }
            else {
                switch (hscroll & 3) {
                case 0:
                    *linePtr++ = vdp->yjkColor[t0 >> 3][J][K];
                case 1:
                    *linePtr++ = vdp->yjkColor[t1 >> 3][J][K];
                case 2:
                    *linePtr++ = vdp->yjkColor[t2 >> 3][J][K];
                case 3:
                    *linePtr++ = vdp->yjkColor[t3 >> 3][J][K];
                }
            }
            charTable += 2;
        }
    }

    if (linePtr == NULL) {
        return;
    }

    rightBorder = X2 == 33;
    if (rightBorder) {
        X2--;
    }

    if (border) {
        UInt16 bgColor = vdp->paletteFixed[vdp->vdpRegs[7]];
        while (X < X2) {
            linePtr[0] = bgColor;
            linePtr[1] = bgColor;
            linePtr[2] = bgColor;
            linePtr[3] = bgColor;
            linePtr[4] = bgColor;
            linePtr[5] = bgColor;
            linePtr[6] = bgColor;
            linePtr[7] = bgColor;
            linePtr += 8; 
            X++;
        }
    }
    else {
        // Update vscroll if needed
        if (vscroll != vdpVScroll(vdp) || chrTabO != vdp->chrTabBase) {
            hscroll = vdpHScroll(vdp);
            scroll = (hscroll & ~3) + X * 8;
            jump   = jumpTable + hScroll512 * 2;
            vscroll = vdpVScroll(vdp);
            chrTabO  = vdp->chrTabBase;

            charTable = vdp->vram + (vdp->chrTabBase & (~vdpIsOddPage(vdp) << 7) & ((-1 << 15) | ((Y - vdp->firstLine + vdpVScroll(vdp)) << 7))) + scroll / 2;
            charTable += 2; 

            if (hScroll512) {
                if (scroll & 0x100) charTable += jump[page ^= 1];
                if (vdp->chrTabBase & (1 << 15)) charTable += jump[page ^= 1] + 128;
            }
        }

        while (X < X2) {
            t0 = charTable[0];         UPDATE_T();
            t1 = charTable[vdp->vram128];   UPDATE_T();
            t2 = charTable[1];         UPDATE_T();
            t3 = charTable[vdp->vram128|1]; UPDATE_T();

            K=(t0 & 0x07) | ((t1 & 0x07) << 3);
            J=(t2 & 0x07) | ((t3 & 0x07) << 3);

            if (sprLine != NULL) {
                col = sprLine[0]; linePtr[0] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t0 >> 3][J][K];
                col = sprLine[1]; linePtr[1] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t1 >> 3][J][K];
                col = sprLine[2]; linePtr[2] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t2 >> 3][J][K];
                col = sprLine[3]; linePtr[3] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t3 >> 3][J][K];
            }
            else {
                linePtr[0] = vdp->yjkColor[t0 >> 3][J][K];
                linePtr[1] = vdp->yjkColor[t1 >> 3][J][K];
                linePtr[2] = vdp->yjkColor[t2 >> 3][J][K];
                linePtr[3] = vdp->yjkColor[t3 >> 3][J][K];
            }

            t0 = charTable[2];        UPDATE_T();
            t1 = charTable[vdp->vram128|2];  UPDATE_T();
            t2 = charTable[3];        UPDATE_T();
            t3 = charTable[vdp->vram128|3];  UPDATE_T();

            K=(t0 & 0x07) | ((t1 & 0x07) << 3);
            J=(t2 & 0x07) | ((t3 & 0x07) << 3);

            if (X == 31) {
                if (sprLine != NULL) {
                    switch (hscroll & 3) {
                    case 1:
                        col = sprLine[6]; linePtr[6] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t2 >> 3][J][K];
                    case 2:
                        col = sprLine[5]; linePtr[5] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t1 >> 3][J][K];
                    case 3:
                        col = sprLine[4]; linePtr[4] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t0 >> 3][J][K];
                    }
                }
                else {
                    switch (hscroll & 3) {
                    case 1:
                        linePtr[6] = vdp->yjkColor[t2 >> 3][J][K];
                    case 2:
                        linePtr[5] = vdp->yjkColor[t1 >> 3][J][K];
                    case 3:
                        linePtr[4] = vdp->yjkColor[t0 >> 3][J][K];
                    }
                }
            }
            else {
                if (sprLine != NULL) {
                    col = sprLine[4]; linePtr[4] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t0 >> 3][J][K];
                    col = sprLine[5]; linePtr[5] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t1 >> 3][J][K];
                    col = sprLine[6]; linePtr[6] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t2 >> 3][J][K];
                    col = sprLine[7]; linePtr[7] = col ? vdp->palette[col >> 1] : vdp->yjkColor[t3 >> 3][J][K];
                    sprLine += 8; 
                }
                else {
                    linePtr[4] = vdp->yjkColor[t0 >> 3][J][K];
                    linePtr[5] = vdp->yjkColor[t1 >> 3][J][K];
                    linePtr[6] = vdp->yjkColor[t2 >> 3][J][K];
                    linePtr[7] = vdp->yjkColor[t3 >> 3][J][K];
                }
            }
        #undef UPDATE_T

            charTable += 4; linePtr += 8;
            X++;
        }
    }

    if (rightBorder) {
        RefreshRightBorder(vdp, Y, vdp->paletteFixed[vdp->vdpRegs[7]], 0, 0);
    }
}

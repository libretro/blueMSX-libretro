/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoChips/SpriteLine.h,v $
**
** $Revision: 1.16 $
**
** $Date: 2006/05/13 17:29:06 $
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
#ifndef SPRITE_LINE_H
#define SPRITE_LINE_H



typedef struct {
    int horizontalPos;
    int color;
    UInt16 pattern;
} SpriteAttribute;

static UInt8 colChckBuf[384] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


static UInt8 lineBuffer[2][384];
static UInt8* lineBufs[2] = { NULL, NULL };
static UInt8* lineBuf = NULL;
static int nonVisibleLine = -1;


UInt8* spritesLine(VDP* vdp, int line) {
    int bufIndex;
    UInt8 collisionBuf[384];
    UInt8* attrib;
    UInt8* attribTable[4];
    UInt8 patternMask;
    int idx;
    int size;
    int scale;
    int visibleCnt;
    int collision;

    idx = line;

    line -= vdp->firstLine;

    bufIndex = line & 1;

//    vdp->vdpStatus[0] &= 0xbf;

    if (idx == 0) {
        lineBufs[bufIndex] = NULL;
        return NULL;
    }

    if (!vdp->screenOn || (vdp->vdpStatus[2] & 0x40) ||vdpIsSpritesOff(vdp->vdpRegs)) {
        lineBufs[bufIndex] = NULL;
        return lineBufs[bufIndex ^ 1];
    }

    attrib = &vdp->vram[vdp->sprTabBase & (-1 << 7)];
    size   = vdpIsSprites16x16(vdp->vdpRegs) ? 16 : 8;
    scale  = vdpIsSpritesBig(vdp->vdpRegs) ? 2 : 1;
    line   = (line + vdpVScroll(vdp)) & 0xff;
    
	patternMask = vdpIsSprites16x16(vdp->vdpRegs) ? 0xfc : 0xff;
    visibleCnt = 0;
    collision = 0;
    /* Find visible sprites on current line */
    for (idx = 0; idx < 32; idx++, attrib += 4) {
        int spriteLine = attrib[0];
        if (spriteLine == 208) {
            break;
        }
       
        spriteLine = ((line - spriteLine) & 0xff) / scale;
		if (spriteLine >= size) {
            continue;
        }
        
        if (visibleCnt == 4) {
			if (~vdp->vdpStatus[0] & 0x40) {
				vdp->vdpStatus[0] = (vdp->vdpStatus[0] & 0xe0) | 0x40 | idx;
			}
            break;
        }

        attribTable[visibleCnt++] = attrib;
    }

    if (visibleCnt == 0) {
        lineBufs[bufIndex] = NULL;
        return lineBufs[bufIndex ^ 1];
    }

	if (~vdp->vdpStatus[0] & 0x40) {
		vdp->vdpStatus[0] = (vdp->vdpStatus[0] & 0xe0) | (idx < 32 ? idx : 31);
	}
    
    lineBuf = lineBuffer[bufIndex];
    memset(lineBuf, 0, 384);
    memset(collisionBuf, 0, 384);

    collision = 0;
    
    while (visibleCnt--) {
        int    spriteLine;
        UInt8  color;
        UInt8* patternPtr;
        UInt8  pattern;
        UInt8* linePtr;
        UInt8* colPtr;
        UInt8* colChck;
        int    colOffset;

        attrib     = attribTable[visibleCnt];
        spriteLine = ((line - attrib[0]) & 0xff) / scale;

        colOffset = ((int)attrib[1] + 32 - ((attrib[3] >> 2) & 0x20));
        colPtr     = collisionBuf + colOffset;
        colChck    = colChckBuf   + colOffset;
        linePtr    = lineBuf + ((int)attrib[1] + 32 - ((attrib[3] >> 2) & 0x20));
        color      = attrib[3] & 0x0f;
        patternPtr = &vdp->vram[(vdp->sprGenBase & (-1 << 11)) + ((int)(attrib[2] & patternMask) << 3) + spriteLine];

        if (!vdpIsColor0Solid(vdp->vdpRegs) && color == 0) {
            continue;
        }
        if (scale == 1) {
            pattern = patternPtr[0]; 
            if (pattern & 0x80) { linePtr[0] = color; collision |= colPtr[0]; colPtr[0] = colChck[0]; }
            if (pattern & 0x40) { linePtr[1] = color; collision |= colPtr[1]; colPtr[1] = colChck[1]; }
            if (pattern & 0x20) { linePtr[2] = color; collision |= colPtr[2]; colPtr[2] = colChck[2]; }
            if (pattern & 0x10) { linePtr[3] = color; collision |= colPtr[3]; colPtr[3] = colChck[3]; }
            if (pattern & 0x08) { linePtr[4] = color; collision |= colPtr[4]; colPtr[4] = colChck[4]; }
            if (pattern & 0x04) { linePtr[5] = color; collision |= colPtr[5]; colPtr[5] = colChck[5]; }
            if (pattern & 0x02) { linePtr[6] = color; collision |= colPtr[6]; colPtr[6] = colChck[6]; }
            if (pattern & 0x01) { linePtr[7] = color; collision |= colPtr[7]; colPtr[7] = colChck[7]; }

            if (vdpIsSprites16x16(vdp->vdpRegs)) {
                pattern = patternPtr[16];

                if (pattern & 0x80) { linePtr[8]  = color; collision |= colPtr[8];  colPtr[8]  = colChck[8]; }
                if (pattern & 0x40) { linePtr[9]  = color; collision |= colPtr[9];  colPtr[9]  = colChck[9]; }
                if (pattern & 0x20) { linePtr[10] = color; collision |= colPtr[10]; colPtr[10] = colChck[10]; }
                if (pattern & 0x10) { linePtr[11] = color; collision |= colPtr[11]; colPtr[11] = colChck[11]; }
                if (pattern & 0x08) { linePtr[12] = color; collision |= colPtr[12]; colPtr[12] = colChck[12]; }
                if (pattern & 0x04) { linePtr[13] = color; collision |= colPtr[13]; colPtr[13] = colChck[13]; }
                if (pattern & 0x02) { linePtr[14] = color; collision |= colPtr[14]; colPtr[14] = colChck[14]; }
                if (pattern & 0x01) { linePtr[15] = color; collision |= colPtr[15]; colPtr[15] = colChck[15]; }
            }
        }
        else {
            pattern = patternPtr[0];
            if (pattern & 0x80) { linePtr[0]  = linePtr[1]  = color; collision |= colPtr[0];  colPtr[0]  = colChck[0];  collision |= colPtr[1];  colPtr[1]  = colChck[1]; }
            if (pattern & 0x40) { linePtr[2]  = linePtr[3]  = color; collision |= colPtr[2];  colPtr[2]  = colChck[2];  collision |= colPtr[3];  colPtr[3]  = colChck[3]; }
            if (pattern & 0x20) { linePtr[4]  = linePtr[5]  = color; collision |= colPtr[4];  colPtr[4]  = colChck[4];  collision |= colPtr[5];  colPtr[5]  = colChck[5]; }
            if (pattern & 0x10) { linePtr[6]  = linePtr[7]  = color; collision |= colPtr[6];  colPtr[6]  = colChck[6];  collision |= colPtr[7];  colPtr[7]  = colChck[7]; }
            if (pattern & 0x08) { linePtr[8]  = linePtr[9]  = color; collision |= colPtr[8];  colPtr[8]  = colChck[8];  collision |= colPtr[9];  colPtr[9]  = colChck[9]; }
            if (pattern & 0x04) { linePtr[10] = linePtr[11] = color; collision |= colPtr[10]; colPtr[10] = colChck[10]; collision |= colPtr[11]; colPtr[11] = colChck[11]; }
            if (pattern & 0x02) { linePtr[12] = linePtr[13] = color; collision |= colPtr[12]; colPtr[12] = colChck[12]; collision |= colPtr[13]; colPtr[13] = colChck[13]; }
            if (pattern & 0x01) { linePtr[14] = linePtr[15] = color; collision |= colPtr[14]; colPtr[14] = colChck[14]; collision |= colPtr[15]; colPtr[15] = colChck[15]; }

            if (vdpIsSprites16x16(vdp->vdpRegs)) {
                pattern = patternPtr[16];

                if (pattern & 0x80) { linePtr[16] = linePtr[17] = color; collision |= colPtr[16]; colPtr[16] = colChck[16]; collision |= colPtr[17]; colPtr[17] = colChck[17]; }
                if (pattern & 0x40) { linePtr[18] = linePtr[19] = color; collision |= colPtr[18]; colPtr[18] = colChck[18]; collision |= colPtr[19]; colPtr[19] = colChck[19]; }
                if (pattern & 0x20) { linePtr[20] = linePtr[21] = color; collision |= colPtr[20]; colPtr[20] = colChck[20]; collision |= colPtr[21]; colPtr[21] = colChck[21]; }
                if (pattern & 0x10) { linePtr[22] = linePtr[23] = color; collision |= colPtr[22]; colPtr[22] = colChck[22]; collision |= colPtr[23]; colPtr[23] = colChck[23]; }
                if (pattern & 0x08) { linePtr[24] = linePtr[25] = color; collision |= colPtr[24]; colPtr[24] = colChck[24]; collision |= colPtr[25]; colPtr[25] = colChck[25]; }
                if (pattern & 0x04) { linePtr[26] = linePtr[27] = color; collision |= colPtr[26]; colPtr[26] = colChck[26]; collision |= colPtr[27]; colPtr[27] = colChck[27]; }
                if (pattern & 0x02) { linePtr[28] = linePtr[29] = color; collision |= colPtr[28]; colPtr[28] = colChck[28]; collision |= colPtr[29]; colPtr[29] = colChck[29]; }
                if (pattern & 0x01) { linePtr[30] = linePtr[31] = color; collision |= colPtr[30]; colPtr[30] = colChck[30]; collision |= colPtr[31]; colPtr[31] = colChck[31]; }
            }
        }
    }

    if (collision) {
        vdp->vdpStatus[0] |= 0x20;
    }

    lineBufs[bufIndex] = lineBuf + 32;

    if (!spritesEnable) {
        return NULL;
    }

    return lineBufs[bufIndex ^ 1];
}


void spriteLineInvalidate(VDP* vdp, int line) {
    nonVisibleLine = line - vdp->firstLine;
}

UInt8* colorSpritesLine(VDP* vdp, int line) {
    int solidColor;
    int bufIndex;
    UInt8 collisionBuf[384];
    SpriteAttribute attribTable[8];
    UInt8 patternMask;
    int   attribOffset;
    int   sprite;
    int   size;
    int   scale;
    int   visibleCnt;
    int   collision;
    int   idx;
    static UInt8 ccColorMask;
    static UInt8 ccColorCheckMask;

    idx = line;

    line -= vdp->firstLine;

    bufIndex = line & 1;

//    vdp->vdpStatus[0] &= 0x80;

    if (line == 0) {
        nonVisibleLine = -1000;
        // This is an not 100% correct optimization. CC sprites should be shown only when
        // they collide with a non CC sprite. However very few games/demos uses this and
        // it is safe to disable the CC sprites if no non CC sprites are visible.
        ccColorMask = ccColorCheckMask;
        ccColorCheckMask = 0xf0;
    }

    if (idx == 0 || nonVisibleLine == line) {
        lineBufs[bufIndex] = NULL;
        return NULL;
    }

    if (!vdp->screenOn || (vdp->vdpStatus[2] & 0x40) ||vdpIsSpritesOff(vdp->vdpRegs)) {
        lineBufs[bufIndex] = NULL;
        return lineBufs[bufIndex ^ 1];
    }

    solidColor   = vdpIsColor0Solid(vdp->vdpRegs) ? 1 : 0;
    attribOffset = vdp->sprTabBase & 0x1fe00;
    size         = vdpIsSprites16x16(vdp->vdpRegs) ? 16 : 8;
    scale        = vdpIsSpritesBig(vdp->vdpRegs) ? 2 : 1;
	patternMask  = vdpIsSprites16x16(vdp->vdpRegs) ? 0xfc : 0xff;
    visibleCnt   = 0;
    collision    = 0;
    line         = (line + vdpVScroll(vdp)) & 0xff;

    /* Find visible sprites on current line */
    for (sprite = 0; sprite < 32; sprite++, attribOffset += 4) {
        int spriteLine;
        int offset;
        int color;

        spriteLine = *MAP_VRAM(vdp, attribOffset);
        if (spriteLine == 216) {
            break;
        }
       
        spriteLine = ((line - spriteLine) & 0xff) / scale;
		if (spriteLine >= size) {
            continue;
        }

        if (visibleCnt == 8) {
			if (~vdp->vdpStatus[0] & 0x40) {
				vdp->vdpStatus[0] |= 0x40 | sprite;
			}
            break;
        }

        offset = (vdp->sprGenBase & 0x1f800) + ((int)(*MAP_VRAM(vdp, attribOffset + 2) & patternMask) << 3) + spriteLine;
        color  = *MAP_VRAM(vdp, vdp->sprTabBase & ((-1 << 10) | (sprite * 16 + spriteLine)));

        if (color & 0x40) {
            color &= ccColorMask;
        }
        else if ((color & 0x0f) || solidColor) {
            ccColorCheckMask = 0xff;
        }

        attribTable[visibleCnt].color         = color;
        attribTable[visibleCnt].horizontalPos = (int)*MAP_VRAM(vdp, attribOffset + 1) + 24 - ((attribTable[visibleCnt].color >> 2) & 0x20);
        attribTable[visibleCnt].pattern       = *MAP_VRAM(vdp, offset);
        if (vdpIsSprites16x16(vdp->vdpRegs)) {
            attribTable[visibleCnt].pattern = (attribTable[visibleCnt].pattern << 8) | *MAP_VRAM(vdp, offset + 16);
            attribTable[visibleCnt].horizontalPos += 8;
        }
        visibleCnt++;
    }

    if (visibleCnt == 0) {
        lineBufs[bufIndex] = NULL;
        return lineBufs[bufIndex ^ 1];
    }

    if (~vdp->vdpStatus[0] & 0x40) {
		vdp->vdpStatus[0] |= sprite < 32 ? sprite : 31;
	}
    
    lineBuf = lineBuffer[bufIndex];
    memset(lineBuf, 0, 384);
    memset(collisionBuf, 0, 384);

    /* Draw the visible sprites */
    for (idx = visibleCnt - 1; idx >= 0; idx--) {
        SpriteAttribute* attrib = &attribTable[idx];
        UInt8* linePtr;
        UInt8* colPtr;
        UInt8* colChck;
        UInt16 pattern;
        UInt8 color;
        int offset;
        int idx2;

        color = ((attrib->color & 0x0f) << 1) | solidColor;
        if (color == 0) {
            continue;
        }

        colPtr  = collisionBuf + attrib->horizontalPos;
        colChck = colChckBuf   + attrib->horizontalPos;

        linePtr = lineBuf + attrib->horizontalPos;
        pattern = attrib->pattern;
        offset  = scale * 15;

        if (attrib->color & 0x60) {
            if (scale == 2) {
                while (pattern) {
                    if (pattern & 1) {
                        linePtr[offset] = color;
                        linePtr[offset + 1] = color;
                    }
                    offset -= 2;
                    pattern >>= 1;
                }
            }
            else {
                while (pattern) {
                    if (pattern & 1) {
                        linePtr[offset] = color;
                    }
                    offset--;
                    pattern >>= 1;
                }
            }
        }
        else {
            if (scale == 2) {
                while (pattern) {
                    if (pattern & 1) {
                        linePtr[offset] = color;
                        linePtr[offset + 1] = color;
                        collision |= colPtr[offset]; 
                        colPtr[offset] = colChck[offset];
                        collision |= colPtr[offset + 1]; 
                        colPtr[offset + 1] = colChck[offset + 1];
                    }
                    offset -= 2;
                    pattern >>= 1;
                }
            }
            else {
                while (pattern) {
                    if (pattern & 1) {
                        linePtr[offset] = color;
                        collision |= colPtr[offset]; 
                        colPtr[offset] = colChck[offset];
                    }
                    offset--;
                    pattern >>= 1;
                }
            }
        }

        /* Skip CC sprites for now */
        if (attrib->color & 0x40) {
            continue;
        }

        /* Draw CC sprites */
        for (idx2 = idx + 1; idx2 < visibleCnt; idx2++) {
            SpriteAttribute* attrib = &attribTable[idx2];

            if (!(attrib->color & 0x40)) {
                break;
            }
            
            color   = ((attrib->color & 0x0f) << 1) | solidColor;
            linePtr = lineBuf + attrib->horizontalPos;
            pattern = attrib->pattern;
            offset  = scale * 15;
            
            if (scale == 2) {
                while (pattern) {
                    if (pattern & 1) {
                        linePtr[offset] |= color;
                        linePtr[offset + 1] |= color;
                    }
                    offset -= 2;
                    pattern >>= 1;
                }
            }
            else {
                while (pattern) {
                    if (pattern & 1) {
                        linePtr[offset] |= color;
                    }
                    offset--;
                    pattern >>= 1;
                }
            }
        }
    }

    if (collision) {
        vdp->vdpStatus[0] |= 0x20;
    }

    lineBufs[bufIndex] = lineBuf + 32;

    if (!spritesEnable) {
        return NULL;
    }

    return lineBufs[bufIndex ^ 1];
}


#endif

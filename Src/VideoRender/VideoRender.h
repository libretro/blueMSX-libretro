/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/VideoRender/VideoRender.h,v $
**
** $Revision: 1.16 $
**
** $Date: 2006/06/20 23:47:33 $
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
#ifndef VIDEO_RENDER_H
#define VIDEO_RENDER_H

#include "MsxTypes.h"
#include "FrameBuffer.h"

struct Properties;

typedef enum { 
    VIDEO_GREEN, 
    VIDEO_COLOR, 
    VIDEO_BLACKWHITE,
    VIDEO_AMBER
} VideoColorMode;

typedef enum { 
    VIDEO_PAL_FAST,
    VIDEO_PAL_MONITOR,
    VIDEO_PAL_SHARP,
    VIDEO_PAL_SHARP_NOISE,  
    VIDEO_PAL_BLUR, 
    VIDEO_PAL_BLUR_NOISE,
	VIDEO_PAL_SCALE2X,
    VIDEO_PAL_HQ2X,
} VideoPalMode;

typedef struct Video Video;

struct Video {
    UInt16* pRgbTable16;
    UInt32* pRgbTable32;
    VideoPalMode palMode;
    int scanLinesEnable;
    int scanLinesPct;
    int colorSaturationEnable;
    int colorSaturationWidth;
    double gamma;
    double saturation;
    double brightness;
    double contrast;
    int deInterlace;
    int invertRGB;
};

Video* videoCreate();

void videoDestroy(Video* video);

void videoSetDeInterlace(Video* video, int deInterlace);
void videoSetBlendFrames(Video* video, int blendFrames);

void videoSetColorMode(Video* video, VideoColorMode colorMode);
void videoSetRgbMode(Video* video, int inverted);

void videoSetPalMode(Video* video, VideoPalMode palMode);

int videoRender(Video* video, FrameBuffer* frameBuffer, int bitDepth, int zoom, void* pDst, int dstOffset, int dstPitch, int canChangeZoom);

void videoSetColors(Video* video, int saturation, int brightness, int contrast, int gamma);

void videoSetScanLines(Video* video, int enable, int scanLinesPct);
void videoSetColorSaturation(Video* video, int enable, int width);

void videoUpdateAll(Video* video, struct Properties* properties); 

#endif

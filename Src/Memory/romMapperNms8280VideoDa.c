/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperNms8280VideoDa.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2006/01/18 22:27:45 $
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
#include "romMapperNms8280VideoDa.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "VideoManager.h"
#include "ArchVideoIn.h"
#include "VDP.h"

#include <stdlib.h>

typedef struct {
    int deviceHandle;
    int vdpDaHandle;
    UInt16* videoBuffer;
} RomMapperNms8280VideoDa;

static void daStart(RomMapperNms8280VideoDa* rm, int oddPage)
{
    rm->videoBuffer = 16 + archVideoInBufferGet(VDP_VIDEODA_WIDTH, VDP_VIDEODA_HEIGHT * 2);
    if (oddPage) {
        rm->videoBuffer += VDP_VIDEODA_WIDTH;
    }
}

static UInt8 daRead(RomMapperNms8280VideoDa* rm, int screenMode, int x, int y, UInt16* palette, int paletteCount)
{
    UInt16 color;
    int bestDiff;
    UInt8 match;
    int i;
    
    color = rm->videoBuffer[x + y * VDP_VIDEODA_WIDTH * 2];

    // If palette is NULL we do 8 bit RGB conversion
    if (palette == NULL) {
        return ((color >> 10) & 0x1c) | ((color >> 2) & 0xe0) | ((color >> 3) & 0x03);
    }

    bestDiff = 0x1000000;
    match = 0;
    
    for (i = 0; i < paletteCount; i++) {
        int dR = ((palette[i] >> 10) & 0x1f) - ((color >> 10) & 0x1f);
        int dG = ((palette[i] >>  5) & 0x1f) - ((color >>  5) & 0x1f);
        int dB = ((palette[i] >>  0) & 0x1f) - ((color >>  0) & 0x1f);
        int test = dR * dR + dG * dG + dB * dB;
        if (test < bestDiff) {
            bestDiff = test;
            match = (UInt8)i;
        }
    }
    return match;
}

static void destroy(RomMapperNms8280VideoDa* rm)
{
    deviceManagerUnregister(rm->deviceHandle);
    vdpUnregisterDaConverter(rm->vdpDaHandle);

    free(rm);
}

int romMapperNms8280VideoDaCreate() 
{
    DeviceCallbacks callbacks   = { destroy, NULL, NULL, NULL };
    VdpDaCallbacks  daCallbacks = { daStart, NULL, daRead };
    RomMapperNms8280VideoDa* rm = malloc(sizeof(RomMapperNms8280VideoDa));

    rm->deviceHandle = deviceManagerRegister(ROM_NMS8280DIGI, &callbacks, rm);
    rm->vdpDaHandle    = vdpRegisterDaConverter(&daCallbacks, rm, VIDEO_MASK_ALL);

    return 1;
}

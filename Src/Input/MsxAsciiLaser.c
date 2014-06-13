/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/MsxAsciiLaser.c,v $
**
** $Revision: 1.10 $
**
** $Date: 2006/06/23 19:09:49 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, NYYRIKKI
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


// TEST: 10 COLOR,15+STRIG(1):GOTO10

// R,G,B (MSX 512 colors, 0-7)
// 7,0,0 is not enough
// 0,4,0 is just enough
// 0,0,6 is just enough
// 7,3,5 is just enough

// 31, 0,  0
// 0,  17, 0
// 0,  0,  26
// 31, 13, 22

// r * 15 + g * 26 + b * 17 >= 442



#include "MsxAsciiLaser.h"
#include "InputEvent.h"
#include "ArchInput.h"
#include "FrameBuffer.h"
#include "Board.h"
#include "VDP.h"
#include <stdlib.h>


#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

struct MsxAsciiLaser {
    MsxJoystickDevice joyDevice;
    int scanlines;
    UInt32 lastTrigger;
};

#define RADIUS     5
#define DELAY      1
#define HOLD       9
#define AIMADJUST  (-3)
#define TRESHOLD   128

static UInt8 read(MsxAsciiLaser* joystick) {
    FrameBuffer* frameBuffer;
    UInt8 state = (~archMouseGetButtonState(0) & 1) << 5;
    int mx, my;

    vdpForceSync();

    archMouseGetState(&mx, &my);

    my = my * joystick->scanlines / 0x10000;
    
    frameBuffer = frameBufferGetDrawFrame(my);
    if (frameBuffer != NULL) {
        int scanline = frameBufferGetScanline();
        int myLow  = MAX(scanline - DELAY - HOLD - RADIUS, my + AIMADJUST - RADIUS);
        int myHigh = MIN(scanline - DELAY, my + AIMADJUST + RADIUS + HOLD);
        int y;

        joystick->scanlines = frameBuffer->lines;

        myLow  = MAX(myLow, 0);
        myHigh = MIN(myHigh, frameBuffer->lines);

        for (y = myLow; y < myHigh; y++) {
            int x = mx * (frameBuffer->line[y].doubleWidth ? 2 : 1) * frameBuffer->maxWidth / 0x10000;
            UInt16 rgb = frameBuffer->line[y].buffer[x];
            int R = 8 * ((rgb >> 10) & 0x01f);
            int G = 8 * ((rgb >> 5) & 0x01f);
            int B = 8 * ((rgb >> 0) & 0x01f);
            int Y = (int)(0.2989*R + 0.5866*G + 0.1145*B);
        
            if (Y > TRESHOLD) {
                state |= 1 << 4;
                break;
            }
        }
    }

    return state;
}

MsxJoystickDevice* msxAsciiLaserCreate()
{
    MsxAsciiLaser* joystick = (MsxAsciiLaser*)calloc(1, sizeof(MsxAsciiLaser));
    joystick->joyDevice.read   = read;
    
    return (MsxJoystickDevice*)joystick;
}
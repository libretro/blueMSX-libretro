/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/MsxMouse.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/11/01 21:19:31 $
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
#include "MsxMouse.h"
#include "InputEvent.h"
#include "ArchInput.h"
#include "Board.h"
#include "SaveState.h"

#include <stdlib.h>

struct MsxMouse {
    MsxJoystickDevice joyDevice;
    int dx;
    int dy;
    int count;
    int mouseAsJoystick;
    UInt8 oldValue;
    UInt32 clock;
};

static void saveState(MsxMouse* mouse)
{
    SaveState* state = saveStateOpenForWrite("msxMouse");
    
    saveStateSet(state, "dx",               mouse->dx);
    saveStateSet(state, "dy",               mouse->dy);
    saveStateSet(state, "count",            mouse->count);
    saveStateSet(state, "mouseAsJoystick",  mouse->mouseAsJoystick);
    saveStateSet(state, "oldValue",         mouse->oldValue);
    saveStateSet(state, "clock",            mouse->clock);

    saveStateClose(state);
}

static void loadState(MsxMouse* mouse) 
{
    SaveState* state = saveStateOpenForRead("msxMouse");

    mouse->dx              =        saveStateGet(state, "dx",              0);
    mouse->dy              =        saveStateGet(state, "dy",              0);
    mouse->count           =        saveStateGet(state, "count",           0);
    mouse->mouseAsJoystick =        saveStateGet(state, "mouseAsJoystick", 0);
    mouse->oldValue        = (UInt8)saveStateGet(state, "oldValue",        0);
    mouse->clock           =        saveStateGet(state, "clock",           0);

    saveStateClose(state);
}

static UInt8 read(MsxMouse* mouse) 
{
    UInt8 state = 0x3f;
    UInt32 systemTime = boardSystemTime();

    if (mouse->mouseAsJoystick) {
        if (systemTime - mouse->clock > boardFrequency() / 120) {
            int dx;
            int dy;

            archMouseGetState(&dx, &dy);
            mouse->clock = systemTime;

            mouse->dx = (dx > 127 ? 127 : (dx < -127 ? -127 : dx));
            mouse->dy = (dy > 127 ? 127 : (dy < -127 ? -127 : dy));
        }

        if ((mouse->oldValue & 0x04) == 0) {
            state = ((mouse->dx / 3) ? ((mouse->dx > 0) ? 0x08 : 0x04) : 0x0c) |
                    ((mouse->dy / 3) ? ((mouse->dy > 0) ? 0x02 : 0x01) : 0x03);
        }
    }
    else {
        switch (mouse->count) {
        case 0:
            state = (mouse->dx >> 4) & 0x0f;
            break;
        case 1:
            state = mouse->dx & 0x0f;
            break;
        case 2: 
            state =(mouse->dy >> 4) & 0x0f;
            break;
        case 3: 
            state = mouse->dy & 0x0f;
            break;
        }
    }

    state |= (~archMouseGetButtonState(0) << 4) & 0x30;

    return state;
}

static void write(MsxMouse* mouse, UInt8 value) 
{
    UInt32 systemTime = boardSystemTime();

    if (mouse->mouseAsJoystick) {
        return;
    }

    if ((value ^ mouse->oldValue) & 0x04) {
        if (systemTime - mouse->clock > boardFrequency() / 2500) {
            mouse->count = 0;
        }
        else {
            mouse->count = (mouse->count + 1) & 3;
        }

        mouse->clock = systemTime;
        
        if (mouse->count == 0) {
            int dx;
            int dy;
            archMouseGetState(&dx, &dy);
            mouse->clock = systemTime;
            mouse->dx = (dx > 127 ? 127 : (dx < -127 ? -127 : dx));
            mouse->dy = (dy > 127 ? 127 : (dy < -127 ? -127 : dy));
        }
    }
    mouse->oldValue = value;
}

static void reset(MsxMouse* mouse) {
    mouse->dx       = 0;
    mouse->dy       = 0;
    mouse->count    = 0;
    mouse->clock    = 0;
    mouse->oldValue = 0;
}

MsxJoystickDevice* msxMouseCreate()
{
    MsxMouse* mouse = (MsxMouse*)calloc(1, sizeof(MsxMouse));
    mouse->joyDevice.read       = read;
    mouse->joyDevice.write      = write;
    mouse->joyDevice.reset      = reset;
    mouse->joyDevice.loadState  = loadState;
    mouse->joyDevice.saveState  = saveState;

    reset(mouse);
    
    return (MsxJoystickDevice*)mouse;
}
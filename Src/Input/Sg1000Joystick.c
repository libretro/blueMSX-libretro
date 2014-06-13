/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/Sg1000Joystick.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2005/11/02 06:58:20 $
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
#include "Sg1000Joystick.h"
#include "InputEvent.h"

#include <stdlib.h>

struct Sg1000Joystick {
    Sg1000JoystickDevice joyDevice;
};

static UInt8 read(Sg1000Joystick* joystick) {
    UInt8 state = (inputEventGetState(EC_JOY1_UP)      << 0) |
                  (inputEventGetState(EC_JOY1_DOWN)    << 1) |
                  (inputEventGetState(EC_JOY1_LEFT)    << 2) |
                  (inputEventGetState(EC_JOY1_RIGHT)   << 3) |
                  (inputEventGetState(EC_JOY1_BUTTON1) << 4) |
                  (inputEventGetState(EC_JOY1_BUTTON2) << 5);

    return ~state & 0x3f;
}

Sg1000JoystickDevice* sg1000JoystickCreate()
{
    Sg1000Joystick* joystick = (Sg1000Joystick*)calloc(1, sizeof(Sg1000Joystick));
    joystick->joyDevice.read   = read;
    
    return (Sg1000JoystickDevice*)joystick;
}
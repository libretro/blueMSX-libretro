/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/JoystickPort.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/22 23:51:17 $
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
#include "JoystickPort.h"
#include "ArchInput.h"
#include "Language.h"
#include <stdlib.h>


static JoystickPortUpdateHandler updateHandler = NULL;
static void* updateHandlerRef;
static JoystickPortType inputType[JOYSTICK_MAX_PORTS];


void joystickPortSetType(int port, JoystickPortType type) 
{
    AmEnableMode mode;
    if (updateHandler != NULL && inputType[port] != type) {
        updateHandler(updateHandlerRef, port, type);
    }

    inputType[port] = type;

    mode = AM_DISABLE;

    if (inputType[0] == JOYSTICK_PORT_MOUSE || 
        inputType[1] == JOYSTICK_PORT_MOUSE)
    {
        mode = AM_ENABLE_MOUSE;
    }

    if (inputType[0] == JOYSTICK_PORT_GUNSTICK || 
        inputType[0] == JOYSTICK_PORT_ASCIILASER || 
        inputType[1] == JOYSTICK_PORT_GUNSTICK || 
        inputType[1] == JOYSTICK_PORT_ASCIILASER)
    {
        mode = AM_ENABLE_LASER;
    }

    archMouseEmuEnable(mode);
}

JoystickPortType joystickPortGetType(int port)
{
    return inputType[port];
}

void joystickPortUpdateHandlerRegister(JoystickPortUpdateHandler fn, void* ref)
{
    int port;
    updateHandler = fn;
    updateHandlerRef = ref;

    for (port = 0; port < JOYSTICK_MAX_PORTS; port++) {
        updateHandler(updateHandlerRef, port, inputType[port]);
    }
}

void joystickPortUpdateHandlerUnregister()
{
    updateHandler = NULL;
}

char* joystickPortTypeToName(int port, JoystickPortType type, int translate)
{
    if (translate) {
        switch(inputType[port]) {
        default:
        case JOYSTICK_PORT_NONE:            return langEnumControlsJoyNone();
        case JOYSTICK_PORT_JOYSTICK:        return langEnumControlsJoy2Button();
        case JOYSTICK_PORT_MOUSE:           return langEnumControlsJoyMouse();
        case JOYSTICK_PORT_TETRIS2DONGLE:   return langEnumControlsJoyTetrisDongle();
        case JOYSTICK_PORT_GUNSTICK:        return langEnumControlsJoyGunStick();
        case JOYSTICK_PORT_COLECOJOYSTICK:  return langEnumControlsJoyColeco();
        case JOYSTICK_PORT_MAGICKEYDONGLE:  return langEnumControlsJoyMagicKeyDongle();
        case JOYSTICK_PORT_ASCIILASER:      return langEnumControlsJoyAsciiLaser();
        }

        return langTextUnknown();
    }

    switch(inputType[port]) {
    default:
    case JOYSTICK_PORT_NONE:            return "none";
    case JOYSTICK_PORT_JOYSTICK:        return "joystick";
    case JOYSTICK_PORT_MOUSE:           return "mouse";
    case JOYSTICK_PORT_TETRIS2DONGLE:   return "tetris2 dongle";
    case JOYSTICK_PORT_GUNSTICK:        return "gunstick";
    case JOYSTICK_PORT_COLECOJOYSTICK:  return "coleco joystick";
    case JOYSTICK_PORT_MAGICKEYDONGLE:  return "magic key dongle";
    case JOYSTICK_PORT_ASCIILASER:      return "ascii laser";
    }

    return "unknown";
}

JoystickPortType joystickPortNameToType(int port, char* name, int translate)
{
    if (translate) {
        if (0 == strcmp(name, langEnumControlsJoy2Button())) return JOYSTICK_PORT_JOYSTICK;
        if (0 == strcmp(name, langEnumControlsJoyMouse())) return JOYSTICK_PORT_MOUSE;
        if (0 == strcmp(name, langEnumControlsJoyTetrisDongle())) return JOYSTICK_PORT_TETRIS2DONGLE;
        if (0 == strcmp(name, langEnumControlsJoyGunStick())) return JOYSTICK_PORT_GUNSTICK;
        if (0 == strcmp(name, langEnumControlsJoyColeco())) return JOYSTICK_PORT_COLECOJOYSTICK;
        if (0 == strcmp(name, langEnumControlsJoyMagicKeyDongle())) return JOYSTICK_PORT_MAGICKEYDONGLE;
        if (0 == strcmp(name, langEnumControlsJoyAsciiLaser())) return JOYSTICK_PORT_ASCIILASER;

        return JOYSTICK_PORT_NONE;
    }

    if (0 == strcmp(name, "joystick")) return JOYSTICK_PORT_JOYSTICK;
    if (0 == strcmp(name, "mouse")) return JOYSTICK_PORT_MOUSE;
    if (0 == strcmp(name, "tetris2 dongle")) return JOYSTICK_PORT_TETRIS2DONGLE;
    if (0 == strcmp(name, "gunstick")) return JOYSTICK_PORT_GUNSTICK;
    if (0 == strcmp(name, "coleco joystick")) return JOYSTICK_PORT_COLECOJOYSTICK;
    if (0 == strcmp(name, "magic key dongle")) return JOYSTICK_PORT_MAGICKEYDONGLE;
    if (0 == strcmp(name, "ascii laser")) return JOYSTICK_PORT_ASCIILASER;

    return JOYSTICK_PORT_NONE;
}

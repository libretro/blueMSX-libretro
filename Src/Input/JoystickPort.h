/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/JoystickPort.h,v $
**
** $Revision: 1.5 $
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
#ifndef JOYSTICK_PORT_H
#define JOYSTICK_PORT_H

#include "MsxTypes.h"

#define JOYSTICK_MAX_PORTS 2

typedef enum {
    JOYSTICK_PORT_NONE,
    JOYSTICK_PORT_JOYSTICK,
    JOYSTICK_PORT_MOUSE,
    JOYSTICK_PORT_TETRIS2DONGLE,
    JOYSTICK_PORT_GUNSTICK,
    JOYSTICK_PORT_COLECOJOYSTICK,
    JOYSTICK_PORT_MAGICKEYDONGLE,
    JOYSTICK_PORT_ASCIILASER
} JoystickPortType;

typedef void (*JoystickPortUpdateHandler)(void*, int, JoystickPortType);

void joystickPortSetType(int port, JoystickPortType type);
JoystickPortType joystickPortGetType(int port);

JoystickPortType joystickPortNameToType(int port, char* name, int translate);
char* joystickPortTypeToName(int port, JoystickPortType type, int translate);

void joystickPortUpdateHandlerRegister(JoystickPortUpdateHandler fn, void* ref);
void joystickPortUpdateHandlerUnregister();

#endif // JOYSTICK_PORT_H

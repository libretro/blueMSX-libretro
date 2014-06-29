/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32joystick.h,v $
**
** $Revision: 1.3 $
**
** $Date: 2008-03-30 18:38:48 $
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
#ifndef WIN32_JOYSTICK_H
#define WIN32_JOYSTICK_H

#include <windows.h>

typedef enum { JOY_NONE, JOY_NUMPAD, JOY_HW, JOY_KEYSET } JoyType;
typedef enum { JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT, JOY_BT1, JOY_BT2 } JoyAction;

void JoystickSetKeyStateKey(int keyset, JoyAction key, int value);
void JoystickSetType(int joyId, JoyType joyType, int subtype);
void JoystickSetHwButtons(int joyId, int buttonA, int buttonB);
char** JoystickGetDevList();
void JoystickInit(HWND hwnd);

#endif

/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32joystick.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2008-05-15 08:52:53 $
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
#include "Win32joystick.h"
#include "Win32keyboard.h"
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>
 
static JoyType joyType[2]    = { JOY_NONE, JOY_NONE };
static int     joySubType[2] = { 0, 0 };
static DWORD autofire[2] = { 0, 0 };
int keySet0[6] = { -1, -1, -1, -1, -1, -1 };
int keySet1[6] = { -1, -1, -1, -1, -1, -1 };

static BYTE joystickNumpad() {
    BYTE value = 0;
    if (GetKeyState(VK_NUMPAD8) > 1UL) value |= 0x01;
    if (GetKeyState(VK_NUMPAD2) > 1UL) value |= 0x02;
    if (GetKeyState(VK_NUMPAD4) > 1UL) value |= 0x04;
    if (GetKeyState(VK_NUMPAD6) > 1UL) value |= 0x08;
    if (GetKeyState(VK_NUMPAD0) > 1UL) value |= 0x10;
    if (GetKeyState(VK_DECIMAL) > 1UL) value |= 0x20;

    return value;
}

static BYTE joystickKeyset(int keyset) {
    int* pKeyset = keyset == 1 ? keySet0 : keySet1;
    BYTE value = 0;

    if (pKeyset[0] != 0xff && GetAsyncKeyState(pKeyset[0]) > 1UL) value |= 0x01;
    if (pKeyset[1] != 0xff && GetAsyncKeyState(pKeyset[1]) > 1UL) value |= 0x02;
    if (pKeyset[2] != 0xff && GetAsyncKeyState(pKeyset[2]) > 1UL) value |= 0x04;
    if (pKeyset[3] != 0xff && GetAsyncKeyState(pKeyset[3]) > 1UL) value |= 0x08;
    if (pKeyset[4] != 0xff && GetAsyncKeyState(pKeyset[4]) > 1UL) value |= 0x10;
    if (pKeyset[5] != 0xff && GetAsyncKeyState(pKeyset[5]) > 1UL) value |= 0x20;

    return value;
}

void JoystickSetKeyStateKey(int keyset, JoyAction key, int value) {
    int* pKeyset = keyset == 1 ? keySet0 : keySet1;
    pKeyset[key] = value;
}

void JoystickSetType(int joyId, JoyType type, int subtype) {
    joyType[joyId]    = type;
    joySubType[joyId] = subtype;
}

void JoystickSetHwButtons(int joyId, int buttonA, int buttonB)
{
    joystickSetButtons(joySubType[joyId], buttonA, buttonB);
}


/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32keyboard.h,v $
**
** $Revision: 1.15 $
**
** $Date: 2005/11/11 06:26:10 $
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
#ifndef WIN32_KEYBOARD_H
#define WIN32_KEYBOARD_H
 
#include "ArchInput.h"

#include <windows.h>

#define KBD_LSHIFT 0x01
#define KBD_RSHIFT 0x02
#define KBD_LCTRL  0x04
#define KBD_RCTRL  0x08
#define KBD_LALT   0x10
#define KBD_RALT   0x20
#define KBD_LWIN   0x40
#define KBD_RWIN   0x80

void inputInit();
int inputReset(HWND hwnd);
void inputDestroy(void);

void keyboardSetDirectory(char* directory);

int keyboardLoadConfig(char* configName);
void keyboardSaveConfig(char* configName);
char* keyboardGetCurrentConfig();
int   keyboardIsCurrentConfigDefault();
char** keyboardGetConfigs();

// For configuration
void keyboardStartConfig();
void keyboardCancelConfig();
int  keyboardConfigIsModified();
void keybardEnableEdit(int enable);
int archKeyboardIsKeySelected(int msxKeyCode);
int archKeyboardIsKeyConfigured(int msxKeyCode);
void archKeyboardSetSelectedKey(int msxKeyCode);

void keyboardEnable(int enable);
void keyboardUpdate();

int keyboardGetModifiers();

void joystickUpdate();
DWORD joystickGetButtonState();
void keyboardSetFocus(int handle, int focus);
int joystickNumButtons(int index);
void joystickSetButtons(int index, int buttonA, int buttonB);

#endif


/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32keyboard.h,v $
**
** $Revision: 1.17 $
**
** $Date: 2008-03-30 18:38:48 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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


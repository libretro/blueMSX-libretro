/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Sdl/SdlShortcuts.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2008-03-30 18:38:45 $
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
#ifndef SDL_SHORTCUTS_H
#define SDL_SHORTCUTS_H


#define HOTKEY_TYPE_NONE     0
#define HOTKEY_TYPE_KEYBOARD 1
#define HOTKEY_TYPE_JOYSTICK 2


typedef struct Shortcuts Shortcuts;

void shortcutsSetDirectory(char* directory);

Shortcuts* shortcutsCreate();
void shortcutsDestroy(Shortcuts* shortcuts);
void shortcutCheckDown(Shortcuts* s, int type, int mods, int keySym);
void shortcutCheckUp(Shortcuts* s, int type, int mods, int keySym);

#endif

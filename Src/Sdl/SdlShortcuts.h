/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Sdl/SdlShortcuts.h,v $
**
** $Revision: 1.2 $
**
** $Date: 2006/06/24 02:27:08 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
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

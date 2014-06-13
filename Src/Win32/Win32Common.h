/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Common.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/01/30 09:09:43 $
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
#ifndef WIN32_COMMON_H
#define WIN32_COMMON_H

#include <windows.h>

#include "StrcmpNoCase.h"
#include "IsFileExtension.h"

typedef enum {
    DLG_ID_PROPERTIES = 1,
    DLG_ID_JOYKEYS = 2,
    DLG_ID_ABOUT = 3,
    DLG_ID_OPEN = 4,
    DLG_ID_MACHINECONFIG = 6,
    DLG_ID_ZIPOPEN = 7,
    DLG_ID_TAPEPOS = 8,
    DLG_ID_OPENSTATE = 9,
} DialogIds;

void centerDialog(HWND hDlg, int noActivate);
void updateDialogPos(HWND hwnd, int dialogID, int noMove, int noSize);
void saveDialogPos(HWND hwnd, int dialogID);

HWND getMainHwnd();


void enterDialogShow();
void exitDialogShow();

#endif

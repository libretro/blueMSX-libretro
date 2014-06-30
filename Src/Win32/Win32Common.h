/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Common.h,v $
**
** $Revision: 1.6 $
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

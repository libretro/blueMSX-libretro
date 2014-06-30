/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Dir.cpp,v $
**
** $Revision: 1.9 $
**
** $Date: 2008-06-06 23:55:57 $
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
#ifndef __WINCRYPT_H__
#define __WINCRYPT_H__
#endif

#include <windows.h> 
#include <shlobj.h> 
#include <objbase.h> 


static bool initialized = false;
static char* defaultDirectory;

static int CALLBACK browseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    switch (uMsg) {
    case BFFM_INITIALIZED:
#if 0
        HWND cbohWnd = CreateWindow("COMBOBOX", NULL, CBS_DROPDOWNLIST|WS_VSCROLL|CBS_AUTOHSCROLL|WS_CHILD|WS_VISIBLE,
            17, 30, 286, 150, hwnd, (HMENU)1005, (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), NULL); 

        SendMessage(cbohWnd, CB_RESETCONTENT, 0, 0);
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"MSX 3.5\" DSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"MSX2 CP/M 3.0 DSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"MSX2 CP/M 3.0 SSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"SVI-328 CP/M 2.24 DSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"SVI-328 CP/M 2.24 SSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"SVI-328 Disk Basic DSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"SVI-328 Disk Basic SSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"SVI-328 Z-CPR3 DSDD");
        SendMessage(cbohWnd, CB_ADDSTRING, 0, (LPARAM)"SVI-738 CP/M 2.28 SSDD");
        SendMessage(cbohWnd, CB_SETCURSEL, 0, 0);

#endif
        if (*defaultDirectory) {
            SendMessage(hwnd, BFFM_SETSELECTION, 1, (LPARAM)defaultDirectory);
        }
        break;
    }
    return 0;
}


extern "C" char* openDir(HWND hwnd, char* pTitle, char* defDir) { 
    static char pFileName[MAX_PATH];
    LPMALLOC pMalloc; 
    BROWSEINFO bi; 
    char* pBuffer = pFileName; 
    LPITEMIDLIST pidl; 

    defaultDirectory = defDir;

	if (!initialized) {
		initialized = true;
	}

    /* Gets the Shell's default allocator */ 
    if (SHGetMalloc(&pMalloc) != NOERROR) { 
        return NULL; 
    } 

    bi.hwndOwner = hwnd; 
    bi.pidlRoot = NULL; 
    bi.pszDisplayName = pBuffer; 
    bi.lpszTitle = pTitle; 
    bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE; 
    bi.lpfn = browseCallbackProc; 
    bi.lParam = 0; 
    
    if ((pidl = SHBrowseForFolder(&bi)) != NULL) { 
        if (!SHGetPathFromIDList(pidl, pBuffer)) { 
            pBuffer = NULL; 
        } 
        pMalloc->Free(pidl); 
    } 

    pMalloc->Release(); 

    return pBuffer; 
} 

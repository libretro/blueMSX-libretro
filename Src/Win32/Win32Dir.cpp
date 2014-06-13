/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Dir.cpp,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/07/03 19:25:45 $
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

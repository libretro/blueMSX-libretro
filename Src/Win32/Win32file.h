/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32file.h,v $
**
** $Revision: 1.11 $
**
** $Date: 2006/06/14 18:15:42 $
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
#ifndef WIN32_FILE_H
#define WIN32_FILE_H

#include <windows.h>
#include "MediaDb.h"

char* openFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
               int newFileSize, char* defExt, int* filterIndex);
char* openStateFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
                    int newFileSize, char* defExt, int* filterIndex, int* showPreview);
char* openRomFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
                  int mustExist, char* defExt, int* filterIndex, RomType* romType);
char* openNewHdFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
                    char* defExt, int* filterIndex);
char* openNewDskFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, char* defExt, int* filterIndex);
char* saveFile(HWND hwndOwner, char* pTitle, char* pFilter, int* pFilterIndex, char* pDir);
char* saveStateFile(HWND hwndOwner, char* pTitle, char* pFilter, int* pFilterIndex, char* pDir, int* showPreview);
char* openConfigFile(HWND parent, char* title, char* description,
                     char** itemList, char* defaultName);
#endif

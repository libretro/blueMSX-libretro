/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32FileTypes.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/01/16 19:34:05 $
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
#include "Win32FileTypes.h"
#include <stdio.h>

BOOL registerFileType(char* extension, char* appName, char* description, int iconIndex) 
{
    char path[MAX_PATH];
    char fileName[MAX_PATH];
    char buffer[MAX_PATH + 32];
    HKEY hKey;
    DWORD exist;
    DWORD rv;

    GetModuleFileName(GetModuleHandle(NULL), fileName, MAX_PATH);

    rv = RegCreateKeyEx(HKEY_CLASSES_ROOT, appName, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &exist);
    rv = RegSetValueEx(hKey, "", 0, REG_SZ, description, strlen(description) + 1);
    RegCloseKey(hKey);

    rv = RegCreateKeyEx(HKEY_CLASSES_ROOT, extension, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &exist);
    rv = RegSetValueEx(hKey, "", 0, REG_SZ, appName, strlen(appName) + 1);
    RegCloseKey(hKey);

    sprintf(buffer, "%s /onearg %%1", fileName);
    sprintf(path, "%s\\Shell\\Open\\command", appName);
    rv = RegCreateKeyEx(HKEY_CLASSES_ROOT, path, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &exist);
    rv = RegSetValueEx(hKey, "", 0, REG_SZ, buffer, strlen(buffer) + 1);
    RegCloseKey(hKey);

    sprintf(path, "%s\\DefaultIcon", appName);
    sprintf(buffer, "%s,%d", fileName, iconIndex);
    rv = RegCreateKeyEx(HKEY_CLASSES_ROOT, path, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &exist);
    rv = RegSetValueEx(hKey, "", 0, REG_SZ, buffer, strlen(buffer) + 1);
    RegCloseKey(hKey);

    return TRUE;
}

BOOL unregisterFileType(char* extension, char* appName, char* description, int iconIndex) 
{
    char path[MAX_PATH];
    HKEY hKey;
    DWORD rv;

    rv = RegOpenKeyEx(HKEY_CLASSES_ROOT, extension, 0, KEY_ALL_ACCESS, &hKey);
    if (rv == ERROR_SUCCESS) {
        char buffer[MAX_PATH + 32];
        DWORD length;
        DWORD type;
        rv = RegQueryValueEx(hKey, "", 0, &type, buffer, &length);
        RegCloseKey(hKey);
        if (type == REG_SZ) {
            if (strcmp(buffer, appName) == 0) {
                RegDeleteKey(HKEY_CLASSES_ROOT, extension);
            }

            sprintf(path, "%s\\DefaultIcon", appName);
            rv = RegOpenKeyEx(HKEY_CLASSES_ROOT, path, 0, KEY_ALL_ACCESS, &hKey);
            if (rv == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                RegDeleteKey(HKEY_CLASSES_ROOT, path);
            }
        }
    }

    return TRUE;
}

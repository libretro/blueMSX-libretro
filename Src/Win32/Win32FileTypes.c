/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32FileTypes.c,v $
**
** $Revision: 1.3 $
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

/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Emulator/AppConfig.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2008-04-03 02:31:52 $
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AppConfig.h"

typedef struct
{
    char key[32];
    char value[32];
    int  intVal;
} AppEntry;

#define MAX_APP_ENTRIES 128

static AppEntry appEntries[MAX_APP_ENTRIES];
static int      appEntryCnt = 0;

const char* appConfigGetString(const char* key, const char* defVal)
{
    int i;
    for (i = 0; i < appEntryCnt; i++) {
        if (strcmp(key, appEntries[i].key) == 0) {
            return appEntries[i].value;
        }
    }
    return defVal;
}

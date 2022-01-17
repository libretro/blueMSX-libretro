/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Sdl/SdlGlob.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2008-03-31 19:42:23 $
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
#if defined(_WIN32) && !defined(_XBOX)

#include "ArchGlob.h"
#include <windows.h>
#include <stdlib.h>

// This glob only support very basic globbing, dirs in the patterns are only
// supported without any wildcards

ArchGlob* archGlob(const char* pattern, int flags)
{
    char oldPath[MAX_PATH];
    const char* filePattern;
    ArchGlob* glob;
    WIN32_FIND_DATA wfd;
    HANDLE handle;

    GetCurrentDirectory(MAX_PATH, oldPath);

    filePattern = strrchr(pattern, '/');
    if (filePattern == NULL) {
        filePattern = pattern;
    }
    else {
        char relPath[MAX_PATH];
        strcpy(relPath, pattern);
        relPath[filePattern - pattern] = '\0';
        pattern = filePattern + 1;
        SetCurrentDirectory(relPath);
    }

    handle = FindFirstFile(pattern, &wfd);
    if (handle == INVALID_HANDLE_VALUE) {
        SetCurrentDirectory(oldPath);
        return NULL;
    }

    glob = (ArchGlob*)calloc(1, sizeof(ArchGlob));

    do {
        DWORD fa;
        if (0 == strcmp(wfd.cFileName, ".") || 0 == strcmp(wfd.cFileName, "..")) {
            continue;
        }
      fa = GetFileAttributes(wfd.cFileName);
        if (((flags & ARCH_GLOB_DIRS) && (fa & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
            ((flags & ARCH_GLOB_FILES) && (fa & FILE_ATTRIBUTE_DIRECTORY) == 0))
        {
            char* path = (char*)malloc(MAX_PATH);
            GetCurrentDirectory(MAX_PATH, path);
            strcat(path, "\\");
            strcat(path, wfd.cFileName);

            glob->count++;
            glob->pathVector = realloc(glob->pathVector, sizeof(char*) * glob->count);
            glob->pathVector[glob->count - 1] = path;
        }
    } while (FindNextFile(handle, &wfd));

    FindClose(handle);

    SetCurrentDirectory(oldPath);

    return glob;
}

void archGlobFree(ArchGlob* globHandle)
{
    int i;

    if (globHandle == NULL) {
        return;
    }

    for (i = 0; i < globHandle->count; i++) {
        free(globHandle->pathVector[i]);
    }
    free(globHandle->pathVector);
    free(globHandle);
}


#else
#include "ArchGlob.h"
#if defined(PSP) || defined(__PS3__) || defined(ANDROID) || defined(WIIU) || defined(_XBOX) || defined(VITA) || defined(PS2) || defined(_3DS) || defined(__SWITCH__)
/* TODO/FIXME - might want to turn this into more generic define that we
 * flick on for consoles/portables */
#include "psp/diet-glob.h"
#else
#include <glob.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ArchGlob* archGlob(const char* pattern, int flags)
{
    glob_t g;
    ArchGlob* globHandle = NULL;
    int rv;
    int i;

    rv = glob(pattern, GLOB_MARK, NULL, &g);
    if (rv != 0) {
        return NULL;
    }

    globHandle = (ArchGlob*)calloc(1, sizeof(ArchGlob));

    for (i = 0; i < g.gl_pathc; i++) {
        char* path = g.gl_pathv[i];
        int len = strlen(path);

        if ((flags & ARCH_GLOB_DIRS) && path[len - 1] == '/') {
            char* storePath = calloc(1, len);
            memcpy(storePath, path, len - 1);
            globHandle->count++;
            globHandle->pathVector = realloc(globHandle->pathVector, sizeof(char*) * globHandle->count);
            globHandle->pathVector[globHandle->count - 1] = storePath;
        }

        if ((flags & ARCH_GLOB_FILES) && path[len - 1] != '/') {
            char* storePath = calloc(1, len + 1);
            memcpy(storePath, path, len);
            globHandle->count++;
            globHandle->pathVector = realloc(globHandle->pathVector, sizeof(char*) * globHandle->count);
            globHandle->pathVector[globHandle->count - 1] = storePath;
        }
    }

    globfree(&g);

    return globHandle;
}

void archGlobFree(ArchGlob* globHandle)
{
    int i;

    if (globHandle == NULL) {
        return;
    }

    for (i = 0; i < globHandle->count; i++) {
        free(globHandle->pathVector[i]);
    }
    if (globHandle->pathVector != NULL) {
        free(globHandle->pathVector);
    }
    free(globHandle);
}

#endif

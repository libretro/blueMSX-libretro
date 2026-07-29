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
#include "ArchGlob.h"

#include <stdlib.h>
#include <string.h>

#include <retro_dirent.h>

/* Match a directory entry name against a glob pattern supporting the
 * '*' and '?' wildcards. Iterative with single-star backtracking. */
static int globMatch(const char* pattern, const char* name)
{
    const char* p = pattern;
    const char* n = name;
    const char* starP = NULL;
    const char* starN = NULL;

    while (*n) {
        if (*p == '*') {
            starP = ++p;
            starN = n;
        }
        else if (*p == '?' || *p == *n) {
            p++;
            n++;
        }
        else if (starP) {
            p = starP;
            n = ++starN;
        }
        else {
            return 0;
        }
    }
    while (*p == '*') p++;
    return *p == 0;
}

static int globCompare(const void* a, const void* b)
{
    return strcmp(*(char* const*)a, *(char* const*)b);
}

ArchGlob* archGlob(const char* pattern, int flags)
{
    char dirName[512];
    const char* filePattern;
    const char* sep;
    const char* sep2;
    size_t dirLen;
    struct RDIR* rdir;
    ArchGlob* globHandle;

    /* Split the pattern into a directory part and a basename pattern.
     * Wildcards are only supported in the basename. */
    sep = strrchr(pattern, '/');
    sep2 = strrchr(pattern, '\\');
    if (sep2 > sep) sep = sep2;

    if (sep == NULL) {
        strcpy(dirName, ".");
        dirLen = 0;
        filePattern = pattern;
    }
    else {
        dirLen = (size_t)(sep - pattern);
        if (dirLen == 0 || dirLen >= sizeof(dirName)) {
            return NULL;
        }
        memcpy(dirName, pattern, dirLen);
        dirName[dirLen] = 0;
        filePattern = sep + 1;
        dirLen++; /* include the separator when building result paths */
    }

    rdir = retro_opendir(dirName);
    if (rdir == NULL) {
        return NULL;
    }

    globHandle = (ArchGlob*)calloc(1, sizeof(ArchGlob));

    while (retro_readdir(rdir)) {
        const char* name = retro_dirent_get_name(rdir);
        int isDir;
        char* storePath;
        size_t nameLen;

        if (name == NULL || name[0] == 0) {
            continue;
        }
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        isDir = retro_dirent_is_dir(rdir, NULL);
        if (isDir && !(flags & ARCH_GLOB_DIRS)) {
            continue;
        }
        if (!isDir && !(flags & ARCH_GLOB_FILES)) {
            continue;
        }
        if (!globMatch(filePattern, name)) {
            continue;
        }

        nameLen = strlen(name);
        storePath = (char*)malloc(dirLen + nameLen + 1);
        if (dirLen > 0) {
            memcpy(storePath, pattern, dirLen);
        }
        memcpy(storePath + dirLen, name, nameLen + 1);

        globHandle->count++;
        globHandle->pathVector = (char**)realloc(globHandle->pathVector, sizeof(char*) * globHandle->count);
        globHandle->pathVector[globHandle->count - 1] = storePath;
    }

    retro_closedir(rdir);

    if (globHandle->count == 0) {
        free(globHandle);
        return NULL;
    }

    /* POSIX glob() returns sorted results; callers rely on this. */
    qsort(globHandle->pathVector, globHandle->count, sizeof(char*), globCompare);

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

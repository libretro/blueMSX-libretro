/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Emulator/FileHistory.c,v $
**
** $Revision: 1.39 $
**
** $Date: 2008/10/26 19:48:18 $
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
#include "FileHistory.h"
#include "Properties.h"
#include "ziphelper.h"
#include "RomLoader.h"
#include "MsxTypes.h"
#include "ArchNotifications.h"
#include "Disk.h"
#include "AppConfig.h"
#include "ArchFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char extendedName[PROP_MAX_CARTS][256];
static char extendedDiskName[PROP_MAX_DISKS][256];
static char extendedCasName[PROP_MAX_TAPES][256];

const char* stripPathExt(const char* filename) {
    static char buffer[128];
    size_t buffer_len;
    strcpy(buffer, stripPath(filename));
    buffer_len = strlen(buffer);

    if (buffer[buffer_len - 4] == '.')
        buffer[buffer_len - 4] = 0;

    return buffer;
}

const char* stripPath(const char* filename) {
    const char* ptr = filename + strlen(filename) - 1;

    while (--ptr >= filename)
    {
        if (*ptr == '/' || *ptr == '\\')
            return ptr + 1;
    }

    return filename;
}

static int fileExist(char* fileName, char* zipFile) {
    if (fileName == NULL || *fileName == 0)
        return 0;

    if (zipFile == NULL || *zipFile == 0) {
        return archFileExists(fileName);
        return 0;
    }
    else {
        if (archFileExists(zipFile)) {
            if( zipFileExists(zipFile, fileName) ) {
                return 1;
            }
        }
        return 0;
    }

    return 0;
}

char* fileGetNext(char* filename, char* zipFile) {
    static char name[512];
    static int pos = -1;
    int c;
    int j;

    strcpy(name, filename);

    pos = strlen(name) - 5;

    if (pos < 0)
        return name;

    while (pos >= 0) {
        c = name[pos];

        if (c >= '0' && c <= '9') {
            if (c < '9') {
                name[pos] = c + 1;
                if (fileExist(name, zipFile))
                    return name;
            }

            for (j = '0'; j < c; j++) {
                name[pos] = j;
                if (fileExist(name, zipFile))
                    return name;
            }
            name[pos] = c;
        }
        pos--;
    }

    pos = strlen(name) - 5;
    c = name[pos];

    if (c >= 'A' && c <= 'Z') {
        if (c < 'Z') {
            name[pos] = c + 1;
            if (fileExist(name, zipFile)) {
                pos = -1;
                return name;
            }
        }

        for (j = 'A'; j <= c; j++) {
            name[pos] = j;
            if (fileExist(name, zipFile)) {
                pos = -1;
                return name;
            }
        }
    }

    if (c >= 'a' && c <= 'z') {
        if (c < 'z') {
            name[pos] = c + 1;
            if (fileExist(name, zipFile)) {
                pos = -1;
                return name;
            }
        }

        for (j = 'a'; j <= c; j++) {
            name[pos] = j;
            if (fileExist(name, zipFile)) {
                pos = -1;
                return name;
            }
        }
    }

    return name;
}

void updateExtendedRomName(int drive, char* filename, char* zipFile) {
    int size;
    char* buf = romLoad(filename, zipFile[0] ? zipFile : NULL, &size);

    if (buf)
    {
        strcpy(extendedName[drive], mediaDbGetPrettyString(mediaDbLookupRom(buf, size)));
        free(buf);
        if (extendedName[drive][0] == 0)
            strcpy(extendedName[drive], stripPathExt(zipFile[0] ? zipFile : filename));
    }
}

void updateExtendedDiskName(int drive, char* filename, char* zipFile) {
    int size;
    char* buf;
    char* name;

    extendedDiskName[drive][0] = 0;

#ifndef WII
    if (drive < MAX_FDC_COUNT) {
        buf = romLoad(filename, zipFile[0] ? zipFile : NULL, &size);
        if (buf != NULL) {
            strcpy(extendedDiskName[drive], mediaDbGetPrettyString(mediaDbLookupDisk(buf, size)));
            free(buf);
            if (extendedDiskName[drive][0] == 0) {
                strcpy(extendedDiskName[drive], stripPathExt(zipFile[0] ? zipFile : filename));
            }
        }
    } else {
#else
    {
#endif
        name = zipFile[0] ? zipFile : filename;
        if ((name != NULL) && name[0]) {
            archFileExists(name);
            strcpy(extendedDiskName[drive], stripPathExt(name));
        }
    }
/*
    int size;
    char* buf = romLoad(filename, zipFile[0] ? zipFile : NULL, &size);

    extendedDiskName[drive][0] = 0;
    if (buf != NULL) {
        strcpy(extendedDiskName[drive], mediaDbGetPrettyString(mediaDbLookupDisk(buf, size)));
        free(buf);
        if (extendedDiskName[drive][0] == 0) {
            strcpy(extendedDiskName[drive], stripPathExt(zipFile[0] ? zipFile : filename));
        }
    }
*/
}

void updateExtendedCasName(int drive, char* filename, char* zipFile) {
    int size;
    char* buf = romLoad(filename, zipFile[0] ? zipFile : NULL, &size);

    extendedCasName[drive][0] = 0;
    if (buf != NULL) {
        strcpy(extendedCasName[drive], mediaDbGetPrettyString(mediaDbLookupCas(buf, size)));
        free(buf);
        if (extendedCasName[drive][0] == 0) {
            strcpy(extendedCasName[drive], stripPathExt(zipFile[0] ? zipFile : filename));
        }
    }
}

void setExtendedRomName(int drive, const char* name) {
    strcpy(extendedName[drive], name);
}


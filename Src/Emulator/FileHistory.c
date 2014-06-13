/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Emulator/FileHistory.c,v $
**
** $Revision: 1.25 $
**
** $Date: 2006/06/26 19:35:54 $
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
#define USE_ARCH_GLOB
#include "FileHistory.h"
#include "Properties.h"
#include "ziphelper.h"
#include "RomLoader.h"
#include "MsxTypes.h"
#include "ArchNotifications.h"
#ifdef USE_ARCH_GLOB
#include "ArchGlob.h"
#include "ArchFile.h"
#endif
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

static char extendedName[PROP_MAX_CARTS][256];
static char extendedDiskName[PROP_MAX_DISKS][256];
static char extendedCasName[PROP_MAX_TAPES][256];

const char* stripPathExt(const char* filename) {
    static char buffer[128];
    
    strcpy(buffer, stripPath(filename));

    if (buffer[strlen(buffer) - 4] == '.') {
        buffer[strlen(buffer) - 4] = 0;
    }

    return buffer;
}

const char* stripPath(const char* filename) {
    const char* ptr = filename + strlen(filename) - 1;

    while (--ptr >= filename) {
        if (*ptr == '/' || *ptr == '\\') {
            return ptr + 1;
        }
    }

    return filename;
}

void updateFileHistory(char* history, RomType* historyType, char* filename, RomType romType) {
    char fname[PROP_MAXPATH];
    int i = 0;

    if (strcmp(filename, CARTNAME_GAMEREADER) == 0) {
        return;
    }

    strcpy(fname, filename);

    for (i = 0; i < MAX_HISTORY - 1; i++) {
        if (*(history + PROP_MAXPATH * i) == 0 || 0 == strcmp(history + PROP_MAXPATH * i, fname)) {
            break;
        }
    }
    while (i > 0) {
        strcpy(history + PROP_MAXPATH * i, history + PROP_MAXPATH * (i - 1));
        if (historyType) historyType[i] = historyType[i - 1];
        i--;
    }
    strcpy(history, fname);
    if (historyType) historyType[0] = romType;
}

void verifyFileHistory(char* history, RomType* historyType) {
    int i, j;

    for (i = 0; i < MAX_HISTORY; i++) {
        char *fname = history + PROP_MAXPATH * i;
        if (fname[0] != '\0' && 
            strcmp(fname, CARTNAME_SNATCHER)    && 
            strcmp(fname, CARTNAME_SDSNATCHER)  && 
            strcmp(fname, CARTNAME_SCCMIRRORED) && 
            strcmp(fname, CARTNAME_SCCEXPANDED) && 
            strcmp(fname, CARTNAME_SCC)         && 
            strcmp(fname, CARTNAME_SCCPLUS)     && 
            strcmp(fname, CARTNAME_FMPAC)       && 
            strcmp(fname, CARTNAME_PAC)         && 
            strcmp(fname, CARTNAME_GAMEREADER)  && 
            strcmp(fname, CARTNAME_SUNRISEIDE)  &&
            strcmp(fname, CARTNAME_BEERIDE)     &&
            strcmp(fname, CARTNAME_GIDE)        &&
            strcmp(fname, CARTNAME_SONYHBI55)   && 
            strcmp(fname, CARTNAME_EXTRAM512KB) && 
            strcmp(fname, CARTNAME_EXTRAM1MB)   &&
            strcmp(fname, CARTNAME_EXTRAM2MB)   &&
            strcmp(fname, CARTNAME_EXTRAM4MB)   &&
            strcmp(fname, CARTNAME_MEGARAM128)  && 
            strcmp(fname, CARTNAME_MEGARAM256)  && 
            strcmp(fname, CARTNAME_MEGARAM512)  && 
            strcmp(fname, CARTNAME_MEGARAM768)  && 
            strcmp(fname, CARTNAME_MEGARAM2M)) 
        {
            struct stat s;
            int rv = archFileExists(fname);
            if (rv) {
                rv = stat(fname, &s) == 0;
            }
            if (rv && (s.st_mode & S_IFDIR)) {
            }
            else {
                if (!archFileExists(fname)) {
                    if (i == MAX_HISTORY - 1) {
                        *(history + PROP_MAXPATH * i) = 0;
                    }
                    else {
                        for (j = i + 1; j < MAX_HISTORY; j++) {
                            strcpy(history + PROP_MAXPATH * (j - 1), history + PROP_MAXPATH * j);
                            if (historyType) historyType[j - 1] = historyType[j];
                            *(history + PROP_MAXPATH * j) = 0;
                        }
                        i--;
                    }
                }
            }
        }
    }
}

int fileExist(char* fileName, char* zipFile) {
    if (fileName == NULL || *fileName == 0) {
        return 0;
    }

    if (zipFile == NULL || *zipFile == 0) {
        return archFileExists(fileName);
        return 0;
    }
    else {
        if (archFileExists(fileName)) {
            int size;
            char* buf = zipLoadFile(zipFile, fileName, &size);
            if (buf != NULL) {
                free(buf);
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

    if (pos < 0) {
        return name;
    }

    while (pos >= 0) {
        c = name[pos];
    
        if (c >= '0' && c <= '9') {
            if (c < '9') {
                name[pos] = c + 1;
                if (fileExist(name, zipFile)) {
                    return name;
                }
            }

            for (j = 0; j < c; j++) {
                name[pos] = j;
                if (fileExist(name, zipFile)) {
                    return name;
                }
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

    if (buf != NULL) {
        strcpy(extendedName[drive], mediaDbGetPrettyString(mediaDbLookupRom(buf, size)));
        free(buf);
        if (extendedName[drive][0] == 0) {
            strcpy(extendedName[drive], stripPathExt(zipFile[0] ? zipFile : filename));
        }
    }
}

void updateExtendedDiskName(int drive, char* filename, char* zipFile) {
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

void setExtendedRomName(int drive, char* name) {
    strcpy(extendedName[drive], name);
}

char* createSaveFileBaseName(Properties* properties, int useExtendedName)
{
    static char fileBase[128] = { 0 };
    int done = 0;
    int i;

    for (i = 0; !done && i < PROP_MAX_CARTS; i++) {
        if (properties->media.carts[i].fileName[0]) {
            if (useExtendedName && extendedName[i][0]) {
                strcpy(fileBase, extendedName[i]);
            }
            else if (*properties->media.carts[i].fileNameInZip) {
                strcpy(fileBase, stripPathExt(properties->media.carts[i].fileNameInZip));
            }
            else {
                strcpy(fileBase, stripPathExt(properties->media.carts[i].fileName));
            }

            if (strcmp(properties->media.carts[i].fileName, CARTNAME_SNATCHER)     &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SDSNATCHER)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SCCMIRRORED)  &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SCCEXPANDED)  &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SCC)          &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SCCPLUS)      &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_FMPAC)        &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_PAC)          &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_GAMEREADER)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SUNRISEIDE)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_BEERIDE)      &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_GIDE)         &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_SONYHBI55)    &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_EXTRAM512KB)  &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_EXTRAM1MB)    &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_EXTRAM2MB)    &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_EXTRAM4MB)    &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_MEGARAM128)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_MEGARAM256)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_MEGARAM512)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_MEGARAM768)   &&
                strcmp(properties->media.carts[i].fileName, CARTNAME_MEGARAM2M)    &&
                properties->media.carts[i].type != ROM_FMPAC               &&
                properties->media.carts[i].type != ROM_PAC                 &&
                properties->media.carts[i].type != ROM_GAMEREADER          &&
                properties->media.carts[i].type != ROM_SUNRISEIDE          &&
                properties->media.carts[i].type != ROM_BEERIDE             &&
                properties->media.carts[i].type != ROM_GIDE                &&
                properties->media.carts[i].type != ROM_MSXAUDIO            &&
                properties->media.carts[i].type != ROM_MOONSOUND           &&
                properties->media.carts[i].type != ROM_SNATCHER            &&
                properties->media.carts[i].type != ROM_SDSNATCHER          &&
                properties->media.carts[i].type != ROM_SCCMIRRORED         &&
                properties->media.carts[i].type != ROM_SCC                 &&
                properties->media.carts[i].type != ROM_SCCPLUS             &&
                properties->media.carts[i].type != ROM_SONYHBI55           &&
                properties->media.carts[i].type != ROM_YAMAHASFG01         &&
                properties->media.carts[i].type != ROM_YAMAHASFG05)
            {
                done = 1;
            }
        }
    }

    for (i = 0; !done && i < PROP_MAX_DISKS; i++) {
        if (properties->media.disks[i].fileName[0]) {
            if (useExtendedName && extendedDiskName[i][0]) {
                strcpy(fileBase, extendedDiskName[i]);
            }
            else if (*properties->media.disks[i].fileNameInZip) {
                strcpy(fileBase, stripPathExt(properties->media.disks[i].fileNameInZip));
            }
            else {
                strcpy(fileBase, stripPathExt(properties->media.disks[i].fileName));
            }
            done = 1;
        }
    }

    for (i = 0; !done && i < PROP_MAX_TAPES; i++) {
        if (properties->media.tapes[0].fileName[0]) {
            if (useExtendedName && extendedCasName[i][0]) {
                strcpy(fileBase, extendedCasName[i]);
            }
            else if (*properties->media.tapes[i].fileNameInZip) {
                strcpy(fileBase, stripPathExt(properties->media.tapes[i].fileNameInZip));
            }
            else {
                strcpy(fileBase, stripPathExt(properties->media.tapes[i].fileName));
            }
            done = 1;
        }
    }

    if (fileBase[0] == 0) {
        strcpy(fileBase, "unknown");
    }

    return fileBase;
}



#ifdef USE_ARCH_GLOB
static UInt32 fileWriteTime(const char* filename)
{
  struct stat s;
  int rv;
  
  rv = stat(filename, &s);

  return rv < 0 ? 0 : s.st_mtime;
}

char* generateSaveFilename(Properties* properties, char* directory, char* prefix, char* extension, int digits)
{
    ArchGlob* glob;
    static char filename[512];
    char baseName[128];
    int fileIndex = 0;
    int extensionLen = strlen(extension);
    int i;
    int numMod = 1;
    char filenameFormat[32] = "%s/%s%s_";
    char destfileFormat[32];

    for (i = 0; i < digits; i++) {
        strcat(filenameFormat, "?");
        numMod *= 10;
    }
    strcat(filenameFormat, "%s");
    sprintf(destfileFormat, "%%s/%%s%%s_%%0%di%%s", digits);
    
    strcpy(baseName, createSaveFileBaseName(properties, 0));

	archCreateDirectory(directory);

    sprintf(filename, filenameFormat, directory, prefix, baseName, extension);

    glob = archGlob(filename, ARCH_GLOB_FILES);

    if (glob) {
        if (glob->count > 0) {
            UInt32 writeTime = fileWriteTime(glob->pathVector[0]);
	        char lastfile[512];
            int filenameLen;
		    strcpy(lastfile, glob->pathVector[0]);

            for (i = 1; i < glob->count; i++) {
                UInt32 thisWriteTime = fileWriteTime(glob->pathVector[i]);
                if (thisWriteTime > 0 && thisWriteTime < writeTime) {
                    break;
                }
                writeTime = thisWriteTime;
		        strcpy(lastfile, glob->pathVector[i]);
            }
            
            filenameLen = strlen(lastfile);

            if (filenameLen > extensionLen + digits) {
                lastfile[filenameLen - extensionLen] = 0;
                fileIndex = (atoi(&lastfile[filenameLen - extensionLen - digits]) + 1) % numMod;
            }
        }
        archGlobFree(glob);
    }

    sprintf(filename, destfileFormat, directory, prefix, baseName, fileIndex, extension);

    return filename;
}
#else
char* generateSaveFilename(Properties* properties, char* directory, char* prefix, char* extension, int digits)
{
	WIN32_FIND_DATA fileData;
    FILETIME writeTime;
    HANDLE hFile;
	char lastfile[512];
    static char filename[512];
    char baseName[128];
    int fileIndex = 0;
    int extensionLen = strlen(extension);
    int i;
    int numMod = 1;
    char filenameFormat[32] = "%s" DIR_SEPARATOR "%s%s_";
    char destfileFormat[32];

    for (i = 0; i < digits; i++) {
        strcat(filenameFormat, "?");
        numMod *= 10;
    }
    strcat(filenameFormat, "%s");
    sprintf(destfileFormat, "%%s" DIR_SEPARATOR "%%s%%s_%%0%di%%s", digits);
    
    strcpy(baseName, createSaveFileBaseName(properties, 0));

	archCreateDirectory(directory);

    sprintf(filename, filenameFormat, directory, prefix, baseName, extension);

    hFile = FindFirstFile(filename,&fileData);
    if (hFile != INVALID_HANDLE_VALUE) {
        int filenameLen;

        writeTime = fileData.ftLastWriteTime;
		strcpy(lastfile, fileData.cFileName);

	    while (FindNextFile(hFile, &fileData ) != 0) {
            if (CompareFileTime(&fileData.ftLastWriteTime, &writeTime) < 0) {
                break;
            }
            writeTime = fileData.ftLastWriteTime;
            strcpy(lastfile, fileData.cFileName);
		}

        filenameLen = strlen(lastfile);

        if (filenameLen > extensionLen + digits) {
            lastfile[filenameLen - extensionLen] = 0;
            fileIndex = (atoi(&lastfile[filenameLen - extensionLen - digits]) + 1) % numMod;
        }
        FindClose(hFile);
    }

    sprintf(filename, destfileFormat, directory, prefix, baseName, fileIndex, extension);

    return filename;
}
#endif

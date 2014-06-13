/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Utils/ziphelper.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2006/05/30 04:10:19 $
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
#include "zip.h"
#include "unzip.h"
#include <stdlib.h>
#include <string.h>

static void toLower(char* str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}
 
/******************************************************************************
*** Description
***     Load a file in a zip file into memory.
***
*** Arguments
***     zipName     - Name of zip file
***     fileName    - Name of file insize zipfile to load
***     size        - Output of size of file
***
*** Return
***     Pointer to allocate memory buffer with file content or NULL on
***     failure.
***
*******************************************************************************
*/
void* zipLoadFile(const char* zipName, const char* fileName, int* size)
{
    void* buf;
    char name[256];
    unzFile zip;
    unz_file_info info;

    *size = 0;

    if (fileName[0] == '*') {
        strcpy(name, zipName);
        name[strlen(zipName) - 3] = fileName[strlen(fileName) - 3];   
        name[strlen(zipName) - 2] = fileName[strlen(fileName) - 2];
        name[strlen(zipName) - 1] = fileName[strlen(fileName) - 1];
    }
    else {
        strcpy(name, fileName);
    }

    zip = unzOpen(zipName);
    if (!zip) {
        return NULL;
    }

    if (unzLocateFile(zip, name, 1) == UNZ_END_OF_LIST_OF_FILE) {
        unzClose(zip);
        return NULL;
    }

    if (unzOpenCurrentFile(zip) != UNZ_OK) {
        return NULL;
    }

    unzGetCurrentFileInfo(zip,&info,NULL,0,NULL,0,NULL,0);

    buf = malloc(info.uncompressed_size);
    *size = info.uncompressed_size;

    if (!buf) {
        unzCloseCurrentFile(zip);
        unzClose(zip);
        return NULL;
    }

    unzReadCurrentFile(zip, buf, info.uncompressed_size);
    unzCloseCurrentFile(zip);
    unzClose(zip);

    return buf;
}

/******************************************************************************
*** Description
***     Load a file in a zip file into memory.
***
*** Arguments
***     zipName     - Name of zip file
***     fileName    - Name of file insize zipfile to save
***     buffer      - Buffer to save
***     size        - Size of buffer to save
***
*******************************************************************************
*/
int zipSaveFile(const char* zipName, char* fileName, int append, void* buffer, int size)
{
    zipFile zip;
    zip_fileinfo zi;
    int err;
    
    zip = zipOpen(zipName, append ? 2 : 0);
    if (zip == NULL) {
        return 0;
    }

    memset(&zi, 0, sizeof(zi));

    err = zipOpenNewFileInZip(zip, fileName, &zi,
                              NULL, 0, NULL, 0, NULL,
                              Z_DEFLATED, Z_DEFAULT_COMPRESSION);
    if (err == ZIP_OK) {
        err = zipWriteInFileInZip(zip, buffer, size);
    }

    zipClose(zip, NULL);

    return err >= 0;
}

int zipHasFileType(char* zipName, char* ext) {
    char tempName[256];
    char extension[8];
    unzFile zip;
    unz_file_info info;
    int found = 0;
    int status;

    zip = unzOpen(zipName);
    if (!zip) {
        return 0;
    }

    strcpy(extension, ext);
    toLower(extension);

    status = unzGoToFirstFile(zip);
    unzGetCurrentFileInfo(zip,&info,tempName,256,NULL,0,NULL,0);

    while (status == UNZ_OK) {
        unzGetCurrentFileInfo(zip, &info, tempName, 256, NULL, 0, NULL, 0);

        toLower(tempName);
        if (strstr(tempName, extension) != NULL) {
            found = 1;
            break;
        }
        
        status = unzGoToNextFile(zip);
    }

    unzClose(zip);

    return found;
}



/******************************************************************************
*** Description
***     Creates a list of file names inside a zip that matches a given
***     extension.
***
*** Arguments
***     zipName     - Name of zip file
***     ext         - Extension to check
***     count       - Output for number of matching files in zip file.
***
*** Return
***     1 if files with the given extension exists in the zip file,
***     0 otherwise.
***
*******************************************************************************
*/
char* zipGetFileList(char* zipName, char* ext, int* count) {
    char tempName[256];
    char extension[8];
    unzFile zip;
    unz_file_info info;
    char* fileArray = NULL;
    int totalLen = 0;
    int status;

    *count = 0;

    zip = unzOpen(zipName);
    if (!zip) {
        return 0;
    }

    strcpy(extension, ext);
    toLower(extension);

    status = unzGoToFirstFile(zip);
    unzGetCurrentFileInfo(zip,&info,tempName,256,NULL,0,NULL,0);

    while (status == UNZ_OK) {
        char tmp[256];
        
        unzGetCurrentFileInfo(zip, &info, tempName, 256, NULL, 0, NULL, 0);

        strcpy(tmp, tempName);

        toLower(tmp);
        if (strstr(tmp, extension) != NULL) {
            int entryLen = strlen(tempName) + 1;
            fileArray = realloc(fileArray, totalLen +  entryLen + 1);
            strcpy(fileArray + totalLen, tempName);
            totalLen += entryLen;
            fileArray[totalLen] = '\0'; // double null termination at end

            *count = *count + 1;
        }
        
        status = unzGoToNextFile(zip);
    }

    unzClose(zip);

    return fileArray;
}

void* zipCompress(void* buffer, int size, int* retSize)
{
    void* retBuf;

    *retSize = (size * 1001) / 1000 + 12;
    retBuf = malloc(*retSize);

    if (compress(retBuf, retSize, buffer, size) != Z_OK) {
        free(retBuf);
        retBuf = NULL;
    }

    return retBuf;
}
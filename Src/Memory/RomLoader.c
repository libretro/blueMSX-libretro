/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/RomLoader.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2004/12/06 07:47:11 $
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
#include "RomLoader.h"
#include "ziphelper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

UInt8* romLoad(const char *fileName, const char *fileInZipFile, int* size)
{
    UInt8* buf = NULL;
    FILE *file;

    if (fileName == NULL || strlen(fileName) == 0) {
        return NULL;
    }

    if (fileInZipFile != NULL && strlen(fileInZipFile) == 0) {
        fileInZipFile = NULL;
    }

    if (fileInZipFile != NULL) {
        return zipLoadFile(fileName, fileInZipFile, size);
    }

    file = fopen(fileName, "rb");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    if (*size == 0) {
        fclose(file);
        return malloc(1);
    }

    fseek(file, 0, SEEK_SET);

    buf = malloc(*size);
    
    *size = fread(buf, 1, *size, file);
    fclose(file);

    return buf;
}


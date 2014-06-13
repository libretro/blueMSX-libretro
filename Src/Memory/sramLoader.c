/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/sramLoader.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/06/15 22:35:59 $
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
#include "sramLoader.h"
#include "Board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



char* sramCreateFilenameWithSuffix(char* romFilename, char* suffix)
{
    static char SRAMfileName[512];
    char fileName[512];
    char* dst = fileName + 512;
    char* src;

    *--dst = '\0';
    *--dst = 'm';
    *--dst = 'a';
    *--dst = 'r';
    *--dst = 's';
    *--dst = '.';

    dst -= strlen(suffix);
    memcpy(dst, suffix, strlen(suffix));
    
    src = romFilename + strlen(romFilename);

    while (*src != '.' && src > romFilename) {
        src--;
    }
    src--;

    while (*src != '/' && *src != '\\' && src >= romFilename) {
        *--dst = *src--;
    }

    sprintf(SRAMfileName, "%s" DIR_SEPARATOR "%s", boardGetBaseDirectory(), dst);

    return SRAMfileName;
}


char* sramCreateFilename(char* romFilename) {
    return sramCreateFilenameWithSuffix(romFilename, "");
}
void sramLoad(char* filename, UInt8* sram, int length, void* header, int headerLength) {
    FILE* file;

    file = fopen(filename, "rb");
    if (file != NULL) {
        if (headerLength > 0) {
            char* readHeader[256];

            fread(readHeader, 1, headerLength, file);
            if (memcmp(readHeader, header, headerLength)) {
                fclose(file);
                return;
            }
        }
        fread(sram, 1, length, file);
        fclose(file);
    }
}

void sramSave(char* filename, UInt8* sram, int length, void* header, int headerLength) {
    FILE* file;

    file = fopen(filename, "wb");
    if (file != NULL) {
        if (headerLength > 0) {
            fwrite(header, 1, headerLength, file);
        }
        fwrite(sram, 1, length, file);
        fclose(file);
    }
}


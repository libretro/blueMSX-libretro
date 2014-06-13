/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/Casette.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/06/13 17:13:27 $
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
#ifndef CASSETTE_H
#define CASSETTE_H

#include "MsxTypes.h"

typedef enum { TAPE_ASCII = 0, TAPE_BINARY, TAPE_BASIC, TAPE_CUSTOM } TapeContentType;

typedef enum { TAPE_UNKNOWN = 0, TAPE_FMSXDOS, TAPE_FMSX98AT, TAPE_SVICAS } TapeFormat;

typedef struct {
    int             pos;
    TapeContentType type;
    char            fileName[8];
} TapeContent;

void   tapeSetDirectory(char* baseDir, char* prefix);
int    tapeInsert(char *name, const char *fileInZipFile);
int    tapeIsInserted();
int    tapeSave(char *name, TapeFormat format);
void tapeLoadState();
void tapeSaveState();
UInt32 tapeGetLength();
UInt32 tapeGetCurrentPos();
void   tapeSetCurrentPos(int pos);
TapeContent* tapeGetContent(int* count);
TapeFormat   tapeGetFormat();
void tapeSetReadOnly(int readOnly);
int tapeIsBusy();
void tapeSetBusy(int busy);

UInt8 tapeWrite(UInt8 value);
UInt8 tapeRead(UInt8* value);
UInt8 tapeReadHeader();
UInt8 tapeWriteHeader();

#endif


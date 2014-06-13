/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Emulator/FileHistory.h,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/11 21:17:03 $
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
#ifndef FILE_HISTORY_H
#define FILE_HISTORY_H

#include "MediaDb.h"
#include "Properties.h"

const char* stripPath(const char* filename);
const char* stripPathExt(const char* filename);
void verifyFileHistory(char* history, RomType* historyType);
void updateFileHistory(char* history, RomType* historyType, char* filename, RomType romType);
int tempStateExists();
int fileExist(char* filename, char* zipFile);
char* fileGetNext(char* filename, char* zipFile);
void setExtendedRomName(int drive, char* name);
void updateExtendedRomName(int drive, char* filename, char* zipFile);
void updateExtendedDiskName(int drive, char* filename, char* zipFile);
void updateExtendedCasName(int drive, char* filename, char* zipFile);

char* createSaveFileBaseName(Properties* properties, int useExtendedName);
char* generateSaveFilename(Properties* properties, char* directory, char* prefix, char* extension, int digits);

#endif



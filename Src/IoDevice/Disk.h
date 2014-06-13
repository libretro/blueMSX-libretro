/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/Disk.h,v $
**
** $Revision: 1.13 $
**
** $Date: 2006/06/24 17:15:57 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Tomas Karlsson
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
#ifndef DISK_H
#define DISK_H

#include "MsxTypes.h"

#define MAX_FDC_COUNT            2

#define MAX_HD_COUNT             4
#define FIRST_INTERNAL_HD_INDEX  2

#define MAX_DRIVES_PER_HD        8

#define MAXDRIVES (MAX_FDC_COUNT + MAX_DRIVES_PER_HD * MAX_HD_COUNT)

UInt8 diskChange(int driveId, char* fileName, const char* fileInZipFile);
void  diskEnable(int driveId, int enable);
UInt8 diskEnabled(int driveId);
UInt8 diskReadOnly(int driveId);
UInt8 diskPresent(int driveId);
UInt8 diskRead(int driveId, UInt8* buffer, int sector);
UInt8 diskReadSector(int driveId, UInt8* buffer, int sector, int side, int track, int density, int *sectorSize);
UInt8 diskWrite(int driveId, UInt8* buffer, int sector);
UInt8 diskWriteSector(int driveId, UInt8 *buffer, int sector, int side, int track, int density);
int   diskGetSectorsPerTrack(int driveId);
int   diskGetSectorSize(int driveId, int side, int track, int density);
int   diskGetSides(int driveId);
int   diskChanged(int driveId);

static int diskGetHdDriveId(int hdId, int driveNo) {
    return MAX_FDC_COUNT + MAX_DRIVES_PER_HD * hdId + driveNo;
}

#endif


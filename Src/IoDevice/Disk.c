/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/Disk.c,v $
**
** $Revision: 1.14 $
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
#include "Disk.h"
#include "DirAsDisk.h"
#include "ziphelper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#define MAXSECTOR (2 * 9 * 81)

static int   drivesEnabled[MAXDRIVES] = { 1, 1 };
static FILE* drives[MAXDRIVES];
static int   RdOnly[MAXDRIVES];
static char* ramImageBuffer[MAXDRIVES];
static int   ramImageSize[MAXDRIVES];
static int   sectorsPerTrack[MAXDRIVES];
static int   fileSize[MAXDRIVES];
static int   sides[MAXDRIVES];
static int   tracks[MAXDRIVES];
static int   changed[MAXDRIVES];
static int   diskType[MAXDRIVES];
static int   maxSector[MAXDRIVES];

enum { MSX_DISK, SVI328_DISK, IDEHD_DISK } diskTypes;

static const UInt8 hdIdentifyBlock[512] = {
    0x5a,0x0c,0xba,0x09,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x31,0x20,0x00,0x00,0x00,0x01,0x04,0x00,0x31,0x56,
    0x30,0x2e,0x20,0x20,0x20,0x20,0x6c,0x62,0x65,0x75,0x53,0x4d,0x00,0x58,0x48,0x20,
    0x52,0x41,0x20,0x44,0x49,0x44,0x4b,0x53,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x80,
    0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x02,0x00,0x02,0x03,0x00,0xba,0x09,0x10,0x00,
    0x3f,0x00,0x60,0x4c,0x26,0x00,0x00,0x00,0xe0,0x53,0x26,0x00,0x07,0x00,0x07,0x04,
    0x03,0x00,0x78,0x00,0x78,0x00,0xf0,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static void diskReadHdIdentifySector(int driveId, UInt8* buffer)
{
    UInt32 totalSectors = fileSize[driveId] / 512;
    UInt16 heads = 16;
    UInt16 sectors = 32;
    UInt16 cylinders = (UInt16)(totalSectors / (heads * sectors));
    
    memcpy(buffer, hdIdentifyBlock, 512);

    buffer[0x02] = cylinders & 0xff;
    buffer[0x03] = cylinders >> 8;
    buffer[0x06] = heads & 0xff;
    buffer[0x07] = heads >> 8;
    buffer[0x0c] = sectors & 0xff;
    buffer[0x0d] = sectors >> 8;
    buffer[0x78] = (UInt8)((totalSectors & 0x000000ff) >>  0);
    buffer[0x79] = (UInt8)((totalSectors & 0x0000ff00) >>  8);
    buffer[0x7a] = (UInt8)((totalSectors & 0x00ff0000) >> 16);
    buffer[0x7b] = (UInt8)((totalSectors & 0xff000000) >> 24);
}


UInt8 diskEnabled(int driveId)
{
    return driveId >= 0 && driveId < MAXDRIVES && drivesEnabled[driveId];
}

UInt8 diskReadOnly(int driveId)
{
    if (!diskPresent(driveId)) {
        return 0;
    }
    return RdOnly[driveId];
}

void  diskEnable(int driveId, int enable)
{
    if (driveId >= 0 && driveId < MAXDRIVES)
        drivesEnabled[driveId] = enable;
}

UInt8 diskPresent(int driveId)
{
    return driveId >= 0 && driveId < MAXDRIVES && 
        (drives[driveId] != NULL || ramImageBuffer[driveId] != NULL);
}

int diskGetSectorsPerTrack(int driveId)
{
    if (driveId < MAXDRIVES)
        return sectorsPerTrack[driveId];

    return 0;
}

int diskGetSides(int driveId)
{
    if (driveId < MAXDRIVES)
        return sides[driveId];

    return 0;
}

int diskGetSectorSize(int driveId, int side, int track, int density)
{
    int secSize;

    if (driveId >= MAXDRIVES)
        return 0;

//    if (boardGetType() == BOARD_SVI) {
    if (diskType[driveId] == SVI328_DISK) {
        secSize = (track==0 && side==0 && density==1) ? 128 : 256;
    }
    else {
        secSize = 512;
    }

    return secSize;
}

static int diskGetSectorOffset(int driveId, int sector, int side, int track, int density)
{
    int offset;
    int secSize;

    if (driveId >= MAXDRIVES)
        return 0;

    secSize = diskGetSectorSize(driveId, side, track, density);

    if (diskType[driveId] == SVI328_DISK) {
        if (track==0 && side==0 && density==1)
            offset = (sector-1)*128; 
        else
            offset = ((track*sides[driveId]+side)*17+sector-1)*256-2048;
    }
    else {
        offset =  sector - 1 + diskGetSectorsPerTrack(driveId) * (track * diskGetSides(driveId) + side);
        offset *= secSize;
    }
    return offset;
}

int diskChanged(int driveId)
{
    if (driveId < MAXDRIVES) {
        int isChanged = changed[driveId];
        changed[driveId] = 0;

        return isChanged;
    }

    return 0;
}

UInt8 diskRead(int driveId, UInt8* buffer, int sector)
{
    if (!diskPresent(driveId))
        return 0;

    if (ramImageBuffer[driveId] != NULL) {
        int offset = sector * 512;

        if (ramImageSize[driveId] < offset + 512) {
            return 0;
        }

        memcpy(buffer, ramImageBuffer[driveId] + offset, 512);
        return 1;
    }
    else {
        if ((drives[driveId] != NULL)) {
            if (0 == fseek(drives[driveId], sector * 512, SEEK_SET)) {
                UInt8 success = fread(buffer, 1, 512, drives[driveId]) == 512;
                return success;
            }
        }
    }
    return 0;
}

UInt8 diskReadSector(int driveId, UInt8* buffer, int sector, int side, int track, int density, int *sectorSize)
{
    int secSize;
    int offset;

    if (!diskPresent(driveId))
        return 0;

    if (diskType[driveId] == IDEHD_DISK && sector == -1) {
        diskReadHdIdentifySector(driveId, buffer);
        return 1;
    }

    offset = diskGetSectorOffset(driveId, sector, side, track, density);
    secSize = diskGetSectorSize(driveId, side, track, density);

    if (sectorSize != NULL) {
        *sectorSize = secSize;
    }

    if (ramImageBuffer[driveId] != NULL) {
        if (ramImageSize[driveId] < offset + secSize) {
            return 0;
        }

        memcpy(buffer, ramImageBuffer[driveId] + offset, secSize);
        return 1;
    }
    else {
        if ((drives[driveId] != NULL)) {
            if (0 == fseek(drives[driveId], offset, SEEK_SET)) {
                UInt8 success = fread(buffer, 1, secSize, drives[driveId]) == secSize;
                return success;
            }
        }
    }

    return 0;
}

static void diskUpdateInfo(int driveId) 
{
	UInt8 buf[512];
    int sectorSize;
    int rv;

    sectorsPerTrack[driveId] = 9;
    sides[driveId]           = 2;
    tracks[driveId]          = 80;
    changed[driveId]         = 1;
    diskType[driveId]        = MSX_DISK;
    maxSector[driveId]       = MAXSECTOR;

    if (fileSize[driveId] > 2 * 1024 * 1024) {
        // HD image
        sectorsPerTrack[driveId] = fileSize[driveId] / 512;
        tracks[driveId]          = 1;
        changed[driveId]         = 1;
        sides[driveId]           = 1;
        diskType[driveId]        = IDEHD_DISK;
        maxSector[driveId]       = 99999999;
        return;
    }

    if (fileSize[driveId] / 512 == 1440) {
        return;
    }

    switch (fileSize[driveId]) {
        case 172032:	/* Single sided */
            sides[driveId] = 1;
            tracks[driveId] = 40;
            sectorsPerTrack[driveId] = 17;
            diskType[driveId] = SVI328_DISK;
            return;
        case 346112:	/* Double sided */
            sides[driveId] = 2;
            tracks[driveId] = 40;
            sectorsPerTrack[driveId] = 17;
            diskType[driveId] = SVI328_DISK;
            return;
	}

    rv = diskReadSector(driveId, buf, 1, 0, 0, 512, &sectorSize);
    if (!rv) {
        return;
    }

    if (buf[0] ==0xeb) {
        switch (buf[0x15]) {
        case 0xf8:
	        sides[driveId]           = 1;
            tracks[driveId]          = 80;
	        sectorsPerTrack[driveId] = 9;
            return;
        case 0xf9:
	        sides[driveId]           = 2;
            tracks[driveId]          = 80;
	        sectorsPerTrack[driveId] = 9;
            // This check is needed to get the SVI-738 MSX-DOS disks to work
            // Maybe it should be applied to other cases as well
            rv = diskReadSector(driveId, buf, 2, 0, 0, 512, &sectorSize);
            if (rv && buf[0] == 0xf8) {
	            sides[driveId] = 1;
            }
            return;
        case 0xfa:
	        sides[driveId]           = 1;
            tracks[driveId]          = 80;
	        sectorsPerTrack[driveId] = 8;
            return;
        case 0xfb:
	        sides[driveId]           = 2;
            tracks[driveId]          = 80;
	        sectorsPerTrack[driveId] = 8;
            return;
        case 0xfc:
	        sides[driveId]           = 1;
            tracks[driveId]          = 40;
	        sectorsPerTrack[driveId] = 9;
            return;
        case 0xfd:
	        sides[driveId]           = 2;
            tracks[driveId]          = 40;
	        sectorsPerTrack[driveId] = 9;
            return;
        case 0xfe:
	        sides[driveId]           = 1;
            tracks[driveId]          = 40;
	        sectorsPerTrack[driveId] = 8;
            return;
        case 0xff:
	        sides[driveId]           = 2;
            tracks[driveId]          = 40;
	        sectorsPerTrack[driveId] = 8;
            return;
        }
    }
    
    if ((buf[0] == 0xe9) || (buf[0] ==0xeb)) {
	    sectorsPerTrack[driveId] = buf[0x18] + 256 * buf[0x19];
	    sides[driveId]           = buf[0x1a] + 256 * buf[0x1b];
    }
    else {
        rv = diskReadSector(driveId, buf, 2, 0, 0, 512, &sectorSize);
        if (!rv) {
            return;
        }
		if (buf[0] >= 0xF8) {
			sectorsPerTrack[driveId] = (buf[0] & 2) ? 8 : 9;
			sides[driveId]           = (buf[0] & 1) ? 2 : 1;
		}
    }

    if (sectorsPerTrack[driveId] == 0  || sides[driveId] == 0 || 
        sectorsPerTrack[driveId] > 255 || sides[driveId] > 2) 
    {
    	switch (fileSize[driveId]) {
        case 327680:  /* 80 tracks, 1 side, 8 sectors/track */
	        sectorsPerTrack[driveId] = 8;
	        sides[driveId] = 1;
            break;
        case 368640:  /* 80 tracks, 1 side, 9 sectors/track */
	        sectorsPerTrack[driveId] = 9;
	        sides[driveId] = 1;
            break;
        case 655360:  /* 80 tracks, 2 side, 8 sectors/track */
	        sectorsPerTrack[driveId] = 8;
	        sides[driveId] = 2;
            break;
        default:
            sectorsPerTrack[driveId] = 9;
            sides[driveId]           = 2;
        }
    }
}

UInt8 diskWrite(int driveId, UInt8 *buffer, int sector)
{
    if (!diskPresent(driveId)) {
        return 0;
    }

    if (sector >= maxSector[driveId]) {
        return 0;
    }

    if (ramImageBuffer[driveId] != NULL) {
        int offset = sector * 512;

        if (ramImageSize[driveId] < offset + 512) {
            return 0;
        }

        memcpy(ramImageBuffer[driveId] + offset, buffer, 512);
        return 1;
    }
    else {
        if (drives[driveId] != NULL && !RdOnly[driveId]) {
            if (0 == fseek(drives[driveId], sector * 512, SEEK_SET)) {
                UInt8 success = fwrite(buffer, 1, 512, drives[driveId]) == 512;
                if (success && sector == 0) {
                    diskUpdateInfo(driveId);
                }
                return success;
            }
        }
    }
    return 0;
}

UInt8 diskWriteSector(int driveId, UInt8 *buffer, int sector, int side, int track, int density)
{
    int secSize;
    int offset;

    if (!diskPresent(driveId))
        return 0;

    if (sector >= maxSector[driveId])
        return 0;

    if (density == 0) {
        density = 512;
    }

    offset = diskGetSectorOffset(driveId, sector, side, track, density);
    secSize = diskGetSectorSize(driveId, side, track, density);

    if (ramImageBuffer[driveId] != NULL) {
        if (ramImageSize[driveId] < offset + secSize) {
            return 0;
        }

        memcpy(ramImageBuffer[driveId] + offset, buffer, secSize);
        return 1;
    }
    else {
        if (drives[driveId] != NULL && !RdOnly[driveId]) {
            if (0 == fseek(drives[driveId], offset, SEEK_SET)) {
                UInt8 success = fwrite(buffer, 1, secSize, drives[driveId]) == secSize;
                return success;
            }
        }
    }
    return 0;
}

UInt8 diskChange(int driveId, char* fileName, const char* fileInZipFile)
{
    struct stat s;
    int rv;

    if (driveId >= MAXDRIVES)
        return 0;

    /* Close previous disk image */
    if(drives[driveId] != NULL) { 
        fclose(drives[driveId]);
        drives[driveId] = NULL; 
    }

    if (ramImageBuffer[driveId] != NULL) {
        // Flush to file??
        free(ramImageBuffer[driveId]);
        ramImageBuffer[driveId] = NULL;
    }

    if(!fileName) {
        return 1;
    }

    rv = stat(fileName, &s);
    if (rv == 0) {
        if (s.st_mode & S_IFDIR) {
            ramImageBuffer[driveId] = dirLoadFile(fileName, &ramImageSize[driveId]);
            fileSize[driveId] = ramImageSize[driveId];
            diskUpdateInfo(driveId);
            return ramImageBuffer[driveId] != NULL;
        }
    }

    if (fileInZipFile != NULL) {
        ramImageBuffer[driveId] = zipLoadFile(fileName, fileInZipFile, &ramImageSize[driveId]);
        fileSize[driveId] = ramImageSize[driveId];
        diskUpdateInfo(driveId);
        return ramImageBuffer[driveId] != NULL;
    }

    drives[driveId] = fopen(fileName, "r+b");
    RdOnly[driveId] = 0;

    if (drives[driveId] == NULL) {
        drives[driveId] = fopen(fileName, "rb");
        RdOnly[driveId] = 1;
    }

    if (drives[driveId] == NULL) {
        return 0;
    }

    fseek(drives[driveId],0,SEEK_END);
    fileSize[driveId] = ftell(drives[driveId]);

    diskUpdateInfo(driveId);

    return 1;
}

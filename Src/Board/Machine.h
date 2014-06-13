/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Board/Machine.h,v $
**
** $Revision: 1.11 $
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
#ifndef MACHINE_H
#define MACHINE_H
 
#include "MsxTypes.h"
#include "MediaDb.h"
#include "VDP.h"
#include "AY8910.h"
#include <stdio.h>


typedef enum { 
    BOARD_UNKNOWN       = -1, 
    BOARD_MSX           = 0x0100 + 0x00, 
    BOARD_MSX_S3527     = 0x0100 + 0x01,
    BOARD_MSX_S1985     = 0x0100 + 0x02,
    BOARD_MSX_T9769B    = 0x0100 + 0x03,
    BOARD_MSX_T9769C    = 0x0100 + 0x04,
    BOARD_SVI           = 0x0200 + 0x00,
    BOARD_COLECO        = 0x0300 + 0x00,
    BOARD_COLECOADAM    = 0x3000 + 0x01,
    BOARD_SG1000        = 0x0400 + 0x00,
    BOARD_MASK          = 0xff00
} BoardType;

typedef struct {
    RomType romType;
    char name[512];
    char inZipName[128];
    int slot;
    int subslot;
    int startPage;
    int pageCount;
    int error;
} SlotInfo;

typedef struct {
    char name[64];
    struct {
        BoardType type;
    } board;
    struct {
        int subslotted;
    } slot[4];
    struct {
        int slot;
        int subslot;
    } cart[2];
    struct {
        VdpVersion vdpVersion;
        int vramSize;
    } video;
    struct {
        int enable;
        int batteryBacked;
    } cmos;
    struct {
        int    hasR800;
        UInt32 freqZ80;
        UInt32 freqR800;
    } cpu;
    struct {
        int enabled;
        int count;
    } fdc;
    int slotInfoCount;
    SlotInfo slotInfo[32];
} Machine;


Machine* machineCreate(const char* machineName);
void machineDestroy(Machine* machine);

char** machineGetAvailable(int checkRoms);

int machineIsValid(const char* machineName, int checkRoms);

void machineUpdate(Machine* machine);

void machineSave(Machine* machine);

int machineInitialize(Machine* machine, UInt8** mainRam, UInt32* mainRamSize);

void machineLoadState(Machine* machine);
void machineSaveState(Machine* machine);

#endif


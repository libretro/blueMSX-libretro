/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/sramMapperMatsuchita.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/23 19:09:50 $
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
#include "sramMapperMatsuchita.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SaveState.h"
#include "IoPort.h"
#include "sramLoader.h"
#include "Switches.h"
#include "Language.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern void msxEnableCpuFreq_1_5(int enable);

typedef struct {
    int    deviceHandle;
    int    debugHandle;
    UInt8  sram[0x800];
    UInt32 address;
	UInt8  color1;
    UInt8  color2;
	UInt8  pattern;
    int    cpu15;
} SramMapperMatsushita;

static void saveState(SramMapperMatsushita* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperMatsushita");

    saveStateSet(state, "address", rm->address);
    saveStateSet(state, "color1",  rm->color1);
    saveStateSet(state, "color2",  rm->color2);
    saveStateSet(state, "pattern", rm->pattern);
    saveStateSet(state, "cpu15",   rm->cpu15);
    
    saveStateClose(state);
}

static void loadState(SramMapperMatsushita* rm)
{
    SaveState* state = saveStateOpenForRead("mapperMatsushita");

    rm->address =        saveStateGet(state, "address", 0);
    rm->color1  = (UInt8)saveStateGet(state, "color1",  0);
    rm->color2  = (UInt8)saveStateGet(state, "color2",  0);
    rm->pattern = (UInt8)saveStateGet(state, "pattern", 0);
    rm->cpu15   = (UInt8)saveStateGet(state, "cpu15",   0);

    saveStateClose(state);

    msxEnableCpuFreq_1_5(rm->cpu15);
}

static void destroy(SramMapperMatsushita* rm)
{
    sramSave(sramCreateFilename("Matsushita.SRAM"), rm->sram, 0x800, NULL, 0);

    deviceManagerUnregister(rm->deviceHandle);
    debugDeviceUnregister(rm->debugHandle);

    ioPortUnregisterSub(0x08);

    free(rm);
}

static UInt8 peek(SramMapperMatsushita* rm, UInt16 ioPort)
{
	UInt8 result;
	switch (ioPort & 0x0f) {
	case 0:
		result = ~0x08;
		break;
	case 1:
        result = switchGetFront() ? 0x7f : 0xff;
		break;
	case 3:
		result = (((rm->pattern & 0x80) ? rm->color2 : rm->color1) << 4)
		        | ((rm->pattern & 0x40) ? rm->color2 : rm->color1);
		break;
	case 9:
		if (rm->address < 0x800) {
			result = rm->sram[rm->address];
		} else {
			result = 0xff;
		}
		break;
	default:
		result = 0xff;
	}
	return result;
}

static UInt8 read(SramMapperMatsushita* rm, UInt16 ioPort)
{
	UInt8 result;
	switch (ioPort & 0x0f) {
	case 0:
		result = ~0x08;
		break;
	case 1:
        result = switchGetFront() ? 0x7f : 0xff;
		break;
	case 3:
		result = (((rm->pattern & 0x80) ? rm->color2 : rm->color1) << 4)
		        | ((rm->pattern & 0x40) ? rm->color2 : rm->color1);
		rm->pattern = (rm->pattern << 2) | (rm->pattern >> 6);
		break;
	case 9:
		if (rm->address < 0x800) {
			result = rm->sram[rm->address];
		} else {
			result = 0xff;
		}
		rm->address = (rm->address + 1) & 0x1fff;
		break;
	default:
		result = 0xff;
	}
	return result;
}

static void write(SramMapperMatsushita* rm, UInt16 ioPort, UInt8 value)
{
	switch (ioPort & 0x0f) {
    case 1:
        rm->cpu15 = (value & 1) == 1;
        msxEnableCpuFreq_1_5(rm->cpu15);
        break;
	case 3:
		rm->color2 = value >> 4;
		rm->color1 = value & 0x0f;
		break;
	case 4:
		rm->pattern = value;
		break;
	case 7:
		rm->address = (rm->address & 0xff00) | value;
		break;
	case 8:
		rm->address = (rm->address & 0x00ff) | ((value & 0x1f) << 8);
		break;
	case 9:
		if (rm->address < 0x800) {
			rm->sram[rm->address] = value;
		}
		rm->address = (rm->address + 1) & 0x1fff;
		break;
	}	
}

static void getDebugInfo(SramMapperMatsushita* rm, DbgDevice* dbgDevice)
{
    if (ioPortCheckSub(0x08)) {
        DbgIoPorts* ioPorts;
        int i;

        ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevKanji12(), 2);

        for (i = 0; i < 16; i++) {
            dbgIoPortsAddPort(ioPorts, i, 0x40 + i, DBG_IO_READWRITE, peek(rm, i));
        }
    }
}

int sramMapperMatsushitaCreate() 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    SramMapperMatsushita* rm;

    rm = malloc(sizeof(SramMapperMatsushita));

    rm->deviceHandle = deviceManagerRegister(SRAM_MATSUCHITA, &callbacks, rm);
    rm->debugHandle = debugDeviceRegister(DBGTYPE_BIOS, langDbgDevMatsushita(), &dbgCallbacks, rm);

    memset(rm->sram, 0xff, 0x800);
    rm->address = 0;

    sramLoad(sramCreateFilename("Matsushita.SRAM"), rm->sram, 0x800, NULL, 0);

    ioPortRegisterSub(0x08, read, write, rm);

    return 1;
}


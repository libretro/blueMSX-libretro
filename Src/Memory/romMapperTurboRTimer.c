/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperTurboRTimer.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/02/11 04:38:35 $
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
#include "romMapperTurboRTimer.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "Board.h"
#include "IoPort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt32 counter;
    UInt32 refTime;
    UInt32 refFrag;
} RomMapperTurboRTimer;

static RomMapperTurboRTimer* theTimer = NULL;

static void saveState(RomMapperTurboRTimer* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperTurboRTimer");

    saveStateSet(state, "counter", rm->counter);
    saveStateSet(state, "refTime", rm->refTime);
    saveStateSet(state, "refFrag", rm->refFrag);
    
    saveStateClose(state);
}

static void loadState(RomMapperTurboRTimer* rm)
{
    SaveState* state = saveStateOpenForRead("mapperTurboRTimer");

    rm->counter = saveStateGet(state, "counter", 0);
    rm->refTime = saveStateGet(state, "refTime", boardSystemTime());
    rm->refFrag = saveStateGet(state, "refFrag", 0);

    saveStateClose(state);
}

static void destroy(RomMapperTurboRTimer* rm)
{
    deviceManagerUnregister(rm->deviceHandle);

    ioPortUnregister(0xe6);
    ioPortUnregister(0xe7);

    free(rm);

    theTimer = NULL;
}

void romMapperTurboRTimerSync()
{
    if (theTimer != NULL) {
        UInt32 systemTime = boardSystemTime();
        UInt64 elapsed = 255682 * (UInt64)(systemTime - theTimer->refTime) + theTimer->refFrag;
        theTimer->refTime = systemTime;
        theTimer->refFrag = (UInt32)(elapsed % boardFrequency());
        theTimer->counter += (UInt32)(elapsed / boardFrequency());
    }
}

static UInt8 read(RomMapperTurboRTimer* rm, UInt16 ioPort)
{
    romMapperTurboRTimerSync();
    
    switch (ioPort & 0x01) {
	case 0:
		return (UInt8)(rm->counter & 0xff);
	case 1:
		return (UInt8)((rm->counter >> 8) & 0xff);
    }

    return 0xff;
}

static void write(RomMapperTurboRTimer* rm, UInt16 ioPort, UInt8 value)
{
    theTimer->counter = 0;
    rm->refTime = boardSystemTime();
    rm->refFrag = 0;
}

int romMapperTurboRTimerCreate() 
{
    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };
    RomMapperTurboRTimer* rm = malloc(sizeof(RomMapperTurboRTimer));

    rm->deviceHandle = deviceManagerRegister(ROM_TURBORTIMER, &callbacks, rm);

    rm->refTime = boardSystemTime();
    rm->refFrag = 0;
    rm->counter = 0;

    ioPortRegister(0xe6, read, write, rm);
    ioPortRegister(0xe7, read, write, rm);

    theTimer = rm;

    return 1;
}


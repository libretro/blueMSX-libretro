/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/MsxTetrisDongle.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/11/01 21:19:31 $
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
#include "MsxTetrisDongle.h"
#include <stdlib.h>
#include "SaveState.h"

struct MsxTetrisDongle {
    MsxJoystickDevice joyDevice;
    UInt8 state;
};

static UInt8 read(MsxTetrisDongle* tetrisDongle) {
    return tetrisDongle->state;
}

static void write(MsxTetrisDongle* tetrisDongle, UInt8 value) {
    if (value & 0x02) {
        tetrisDongle->state |= 0x08;
    }
    else {
        tetrisDongle->state &= ~0x08;
    }
}

static void saveState(MsxTetrisDongle* tetrisDongle)
{
    SaveState* state = saveStateOpenForWrite("tetrisDongle");
    
    saveStateSet(state, "state", tetrisDongle->state);

    saveStateClose(state);
}

static void loadState(MsxTetrisDongle* tetrisDongle) 
{
    SaveState* state = saveStateOpenForRead("tetrisDongle");

    tetrisDongle->state = (UInt8)saveStateGet(state, "state", 0x3f);

    saveStateClose(state);
}

static void reset(MsxTetrisDongle* tetrisDongle) {
    tetrisDongle->state = 0x3f;
}

MsxJoystickDevice* msxTetrisDongleCreate()
{
    MsxTetrisDongle* tetrisDongle = (MsxTetrisDongle*)calloc(1, sizeof(MsxTetrisDongle));
    tetrisDongle->joyDevice.read      = read;
    tetrisDongle->joyDevice.write     = write;
    tetrisDongle->joyDevice.reset     = reset;
    tetrisDongle->joyDevice.loadState = loadState;
    tetrisDongle->joyDevice.saveState = saveState;

    reset(tetrisDongle);
    
    return (MsxJoystickDevice*)tetrisDongle;
}
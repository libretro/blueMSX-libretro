/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/MsxPsg.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/13 06:24:20 $
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
#include "MsxPsg.h"
#include "Board.h"
#include "AY8910.h"
#include "JoystickPort.h"
#include "DeviceManager.h"
#include "Led.h"
#include "Switches.h"

#include "MsxJoystickDevice.h"
#include "MsxJoystick.h"
#include "MsxGunstick.h"
#include "MsxAsciiLaser.h"
#include "MsxMouse.h"
#include "MsxTetrisDongle.h"
#include "MagicKeyDongle.h"

#include <stdlib.h>

struct MsxPsg {
    int deviceHandle;
    AY8910* ay8910;
    int currentPort;
    UInt8 registers[2];
    UInt8 readValue[2];
    MsxJoystickDevice* devFun[2];
};

static void joystickPortHandler(MsxPsg* msxPsg, int port, JoystickPortType type)
{
    if (port >= 2) {
        return;
    }

    if (msxPsg->devFun[port] != NULL && msxPsg->devFun[port]->destroy != NULL) {
        msxPsg->devFun[port]->destroy(msxPsg->devFun[port]);
    }

    switch (type) {
    default:
    case JOYSTICK_PORT_NONE:
        msxPsg->devFun[port] = NULL;
        break;
    case JOYSTICK_PORT_GUNSTICK:
        msxPsg->devFun[port] = msxGunstickCreate();
        break;
    case JOYSTICK_PORT_ASCIILASER:
        msxPsg->devFun[port] = msxAsciiLaserCreate();
        break;
    case JOYSTICK_PORT_JOYSTICK:
        msxPsg->devFun[port] = msxJoystickCreate(port);
        break;
    case JOYSTICK_PORT_MOUSE:
        msxPsg->devFun[port] = msxMouseCreate();
        break;
    case JOYSTICK_PORT_TETRIS2DONGLE:
        msxPsg->devFun[port] = msxTetrisDongleCreate();
        break;
    case JOYSTICK_PORT_MAGICKEYDONGLE:
        msxPsg->devFun[port] = magicKeyDongleCreate();
        break;
    }
}

static UInt8 peek(MsxPsg* msxPsg, UInt16 address)
{
    if (address & 1) {
        return msxPsg->registers[1] & 0xf0;
    }
    return msxPsg->readValue[address & 1];
}

static UInt8 read(MsxPsg* msxPsg, UInt16 address)
{
    if (address & 1) {
        return msxPsg->registers[1] & 0xf0;
    }
    else {
        int renshaSpeed = switchGetRensha();
	    UInt8 state = 0x3f;
        if (msxPsg->devFun[msxPsg->currentPort] != NULL &&
            msxPsg->devFun[msxPsg->currentPort]->read != NULL) {
            state = msxPsg->devFun[msxPsg->currentPort]->read(msxPsg->devFun[msxPsg->currentPort]);
        }
        if (renshaSpeed) {
            state &= ~((((UInt64)renshaSpeed * boardSystemTime() / boardFrequency()) & 1) << 4);
        }
        state |= 0x40;

        msxPsg->readValue[address & 1] = state;

        return state;
    }
}

static void write(MsxPsg* msxPsg, UInt16 address, UInt8 value)
{
    if (address & 1) {
        if (msxPsg->devFun[0] != NULL && msxPsg->devFun[0]->write != NULL) {
	        UInt8 val = ((value >> 0) & 0x03) | ((value >> 2) & 0x04);
	        msxPsg->devFun[0]->write(msxPsg->devFun[0], val);
        }
        if (msxPsg->devFun[1] != NULL && msxPsg->devFun[1]->write != NULL) {
	        UInt8 val = ((value >> 2) & 0x03) | ((value >> 3) & 0x04);
	        msxPsg->devFun[1]->write(msxPsg->devFun[1], val);
        }

	    msxPsg->currentPort = (value >> 6) & 0x01;

        ledSetKana(0 == (value & 0x80));
    }
    msxPsg->registers[address & 1] = value;
}

static void saveState(MsxPsg* msxPsg)
{
    if (msxPsg->devFun[0] != NULL && msxPsg->devFun[0]->saveState != NULL) {
	    msxPsg->devFun[0]->saveState(msxPsg->devFun[0]);
    }
    if (msxPsg->devFun[1] != NULL && msxPsg->devFun[1]->saveState != NULL) {
	    msxPsg->devFun[1]->saveState(msxPsg->devFun[1]);
    }

    ay8910SaveState(msxPsg->ay8910);
}

static void loadState(MsxPsg* msxPsg)
{
    if (msxPsg->devFun[0] != NULL && msxPsg->devFun[0]->loadState != NULL) {
	    msxPsg->devFun[0]->loadState(msxPsg->devFun[0]);
    }
    if (msxPsg->devFun[1] != NULL && msxPsg->devFun[1]->loadState != NULL) {
	    msxPsg->devFun[1]->loadState(msxPsg->devFun[1]);
    }
    
    ay8910LoadState(msxPsg->ay8910);
}

static void reset(MsxPsg* msxPsg)
{
    msxPsg->currentPort  = 0;
    msxPsg->registers[0] = 0;
    msxPsg->registers[1] = 0;
    msxPsg->readValue[0] = 0;
    msxPsg->readValue[1] = 0;

    if (msxPsg->devFun[0] != NULL && msxPsg->devFun[0]->reset != NULL) {
	    msxPsg->devFun[0]->reset(msxPsg->devFun[0]);
    }
    if (msxPsg->devFun[1] != NULL && msxPsg->devFun[1]->reset != NULL) {
	    msxPsg->devFun[1]->reset(msxPsg->devFun[1]);
    }

    ay8910Reset(msxPsg->ay8910);
}

static void destroy(MsxPsg* msxPsg) 
{
    ay8910SetIoPort(msxPsg->ay8910, NULL, NULL, NULL, NULL);
    ay8910Destroy(msxPsg->ay8910);
    joystickPortUpdateHandlerUnregister();
    deviceManagerUnregister(msxPsg->deviceHandle);
}

MsxPsg* msxPsgCreate(PsgType type)
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    MsxPsg* msxPsg = (MsxPsg*)calloc(1, sizeof(MsxPsg));

    msxPsg->ay8910 = ay8910Create(boardGetMixer(), AY8910_MSX, type);
    
    ay8910SetIoPort(msxPsg->ay8910, read, peek, write, msxPsg);

    joystickPortUpdateHandlerRegister(joystickPortHandler, msxPsg);

    msxPsg->deviceHandle = deviceManagerRegister(ROM_UNKNOWN, &callbacks, msxPsg);

    return msxPsg;
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Input/SviJoyIo.c,v $
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
#include "SviJoyIo.h"
#include "JoystickPort.h"
#include "InputEvent.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "MediaDb.h"

#include "SviJoystick.h"

#include <stdlib.h>

struct SviJoyIo {
    SviJoystickDevice* joyDevice[2];
    int joyDeviceHandle;
    UInt8 lastReadValue;
    int deviceHandle;
};

static void joyIoHandler(SviJoyIo* joyIo, int port, JoystickPortType type)
{
    if (port >= 2) {
        return;
    }

    if (joyIo->joyDevice[port] != NULL && joyIo->joyDevice[port]->destroy != NULL) {
        joyIo->joyDevice[port]->destroy(joyIo->joyDevice[port]);
    }
    
    switch (type) {
    default:
    case JOYSTICK_PORT_NONE:
        joyIo->joyDevice[port] = NULL;
        break;
    case JOYSTICK_PORT_JOYSTICK:
        joyIo->joyDevice[port] = sviJoystickCreate(port);
        break;
    }
}

static void loadState(SviJoyIo* joyIo)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (joyIo->joyDevice[i] != NULL && joyIo->joyDevice[i]->loadState != NULL) {
            joyIo->joyDevice[i]->loadState(joyIo->joyDevice[i]);
        }
    }
}

static void saveState(SviJoyIo* joyIo)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (joyIo->joyDevice[i] != NULL && joyIo->joyDevice[i]->saveState != NULL) {
            joyIo->joyDevice[i]->saveState(joyIo->joyDevice[i]);
        }
    }
}

static void reset(SviJoyIo* joyIo)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (joyIo->joyDevice[i] != NULL && joyIo->joyDevice[i]->reset != NULL) {
            joyIo->joyDevice[i]->reset(joyIo->joyDevice[i]);
        }
    }
}

static void destroy(SviJoyIo* joyIo)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (joyIo->joyDevice[i] != NULL && joyIo->joyDevice[i]->destroy != NULL) {
            joyIo->joyDevice[i]->destroy(joyIo->joyDevice[i]);
        }
    }

    joystickPortUpdateHandlerUnregister();

    deviceManagerUnregister(joyIo->deviceHandle);
}

UInt8 sviJoyIoRead(SviJoyIo* joyIo) {
    UInt8 value = 0xff;
    if (joyIo->joyDevice[0] != NULL && joyIo->joyDevice[0]->read != NULL) {
        value = (value & 0xf0) | (joyIo->joyDevice[0]->read(joyIo->joyDevice[0]) << 0);
    }
    if (joyIo->joyDevice[1] != NULL && joyIo->joyDevice[1]->read != NULL) {
        value = (value & 0x0f) | (joyIo->joyDevice[1]->read(joyIo->joyDevice[1]) << 4);
    }
    joyIo->lastReadValue = value;
    return value;
}

UInt8 sviJoyIoReadTrigger(SviJoyIo* joyIo) {
    UInt8 value = 0;
    if (joyIo->joyDevice[0] != NULL && joyIo->joyDevice[0]->readTrigger != NULL) {
        value |= (joyIo->joyDevice[0]->readTrigger(joyIo->joyDevice[0]) << 0);
    }
    if (joyIo->joyDevice[1] != NULL && joyIo->joyDevice[1]->readTrigger != NULL) {
        value |= (joyIo->joyDevice[1]->readTrigger(joyIo->joyDevice[1]) << 1);
    }
    return value;
}

SviJoyIo* sviJoyIoCreate() 
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    SviJoyIo* sviJoyIo = (SviJoyIo*)calloc(1, sizeof(SviJoyIo));

    joystickPortUpdateHandlerRegister(joyIoHandler, sviJoyIo);

    sviJoyIo->deviceHandle = deviceManagerRegister(ROM_UNKNOWN, &callbacks, sviJoyIo);

    return sviJoyIo;
}


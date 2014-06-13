/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/DeviceManager.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2004/12/06 07:47:11 $
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
#include "DeviceManager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_DEVICES 64

typedef struct {
    int handle;
    DeviceCallbacks callbacks;
    void* ref;
    int type;
} DeviceInfo;

typedef struct {
    DeviceInfo di[MAX_DEVICES];
    int count;
    int lastHandle;
    int shutDown;
} DeviceManager;

static DeviceManager deviceManager;

void deviceManagerCreate() {
    deviceManager.count = 0;
    deviceManager.lastHandle = 0;
    deviceManager.shutDown = 0;
}

void deviceManagerDestroy()
{
    int i;

    deviceManager.shutDown = 1;
    
    for (i = 0; i < deviceManager.count; i++) {
        if (deviceManager.di[i].callbacks.destroy != NULL) {
            deviceManager.di[i].callbacks.destroy(deviceManager.di[i].ref);
        }
    }
}

void deviceManagerReset()
{
    int i;
    for (i = 0; i < deviceManager.count; i++) {
        if (deviceManager.di[i].callbacks.reset != NULL) {
            deviceManager.di[i].callbacks.reset(deviceManager.di[i].ref);
        }
    }
}

void deviceManagerLoadState()
{
    int i;
    for (i = 0; i < deviceManager.count; i++) {
        if (deviceManager.di[i].callbacks.loadState != NULL) {
            deviceManager.di[i].callbacks.loadState(deviceManager.di[i].ref);
        }
    }
}

void deviceManagerSaveState()
{
    int i;
    for (i = 0; i < deviceManager.count; i++) {
        if (deviceManager.di[i].callbacks.saveState != NULL) {
            deviceManager.di[i].callbacks.saveState(deviceManager.di[i].ref);
        }
    }
}

int deviceManagerRegister(int type, DeviceCallbacks* callbacks, void* ref)
{
    if (deviceManager.count >= MAX_DEVICES) {
        return 0;
    }

    deviceManager.di[deviceManager.count].handle    = ++deviceManager.lastHandle;
    deviceManager.di[deviceManager.count].type      = type;
    deviceManager.di[deviceManager.count].callbacks = *callbacks;
    deviceManager.di[deviceManager.count].ref       = ref;

    deviceManager.count++;

    return deviceManager.lastHandle;
}

void deviceManagerUnregister(int handle)
{
    int i;

    if (deviceManager.count == 0 || deviceManager.shutDown) {
        return;
    }

    for (i = 0; i < deviceManager.count; i++) {
        if (deviceManager.di[i].handle == handle) {
            break;
        }
    }

    if (i == deviceManager.count) {
        return;
    }

    deviceManager.count--;
    while (i < deviceManager.count) {
        deviceManager.di[i] = deviceManager.di[i + 1];
        i++;
    }
}

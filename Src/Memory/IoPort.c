/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/IoPort.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/28 20:42:35 $
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
#include "IoPort.h"
#include "Board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct IoPortInfo {
    IoPortRead  read;
    IoPortWrite write;
    void*       ref;
} IoPortInfo;

static IoPortInfo ioTable[256];
static IoPortInfo ioSubTable[256];
static IoPortInfo ioUnused[2];
static int currentSubport;

void ioPortReset()
{
    memset(ioTable, 0, sizeof(ioTable));
    memset(ioSubTable, 0, sizeof(ioSubTable));

    currentSubport = 0;
}

void ioPortRegister(int port, IoPortRead read, IoPortWrite write, void* ref)
{
    if (ioTable[port].read  == NULL && 
        ioTable[port].write == NULL && 
        ioTable[port].ref   == NULL)
    {
        ioTable[port].read  = read;
        ioTable[port].write = write;
        ioTable[port].ref   = ref;
    }
}


void ioPortUnregister(int port)
{
    ioTable[port].read  = NULL;
    ioTable[port].write = NULL;
    ioTable[port].ref   = NULL;
}

void ioPortRegisterUnused(int idx, IoPortRead read, IoPortWrite write, void* ref)
{
    ioUnused[idx].read  = read;
    ioUnused[idx].write = write;
    ioUnused[idx].ref   = ref;
}

void ioPortUnregisterUnused(int idx)
{
    ioUnused[idx].read  = NULL;
    ioUnused[idx].write = NULL;
    ioUnused[idx].ref   = NULL;
}

void ioPortRegisterSub(int subport, IoPortRead read, IoPortWrite write, void* ref)
{
    ioSubTable[subport].read  = read;
    ioSubTable[subport].write = write;
    ioSubTable[subport].ref   = ref;
}


void ioPortUnregisterSub(int subport)
{
    ioSubTable[subport].read  = NULL;
    ioSubTable[subport].write = NULL;
    ioSubTable[subport].ref   = NULL;
}

int ioPortCheckSub(int subport)
{
    return currentSubport = subport;
}

UInt8 ioPortRead(void* ref, UInt16 port)
{
    port &= 0xff;

    if (boardGetType() == BOARD_MSX && port >= 0x40 && port < 0x50) {
        if (ioSubTable[currentSubport].read == NULL) {
            return 0xff;
        }

        return ioSubTable[currentSubport].read(ioSubTable[currentSubport].ref, port);
    }

    if (ioTable[port].read == NULL) {
        if (ioUnused[0].read != NULL) {
            return ioUnused[0].read(ioUnused[0].ref, port);
        }
        if (ioUnused[1].read != NULL) {
            return ioUnused[1].read(ioUnused[1].ref, port);
        }
        return 0xff;
    }

    return ioTable[port].read(ioTable[port].ref, port);
}

void  ioPortWrite(void* ref, UInt16 port, UInt8 value)
{
    port &= 0xff;

    if (boardGetType() == BOARD_MSX && port >= 0x40 && port < 0x50) {
        if (port == 0x40) {
            currentSubport = value;
            return;
        }
        
        if (ioSubTable[currentSubport].write != NULL) {
            ioSubTable[currentSubport].write(ioSubTable[currentSubport].ref, port, value);
        }
        return;
    }

    if (ioTable[port].write != NULL) {
        ioTable[port].write(ioTable[port].ref, port, value);
    }
    else if (ioUnused[0].write != NULL) {
        ioUnused[0].write(ioUnused[0].ref, port, value);
    }
    else if (ioUnused[1].write != NULL) {
        ioUnused[1].write(ioUnused[1].ref, port, value);
    }
}



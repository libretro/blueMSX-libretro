/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/I8255.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2005/08/18 05:21:51 $
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
#include "I8255.h"
#include "SaveState.h"
#include <stdlib.h>

struct I8255
{
    I8255Read peekA;
    I8255Read readA;
    I8255Write writeA;
    I8255Read peekB;
    I8255Read readB;
    I8255Write writeB;
    I8255Read peekCLo;
    I8255Read readCLo;
    I8255Write writeCLo;
    I8255Read peekCHi;
    I8255Read readCHi;
    I8255Write writeCHi;
    void* ref;

    UInt8 reg[4];
};

static UInt8 readDummy(void* ref)
{
    return 0xff;
}

static void writeDummy(void* ref, UInt8 value)
{
}

I8255* i8255Create(I8255Read peekA,   I8255Read readA,   I8255Write writeA, 
                   I8255Read peekB,   I8255Read readB,   I8255Write writeB,
                   I8255Read peekCLo, I8255Read readCLo, I8255Write writeCLo,
                   I8255Read peekCHi, I8255Read readCHi, I8255Write writeCHi,
                   void* ref)
{
    I8255* i8255 = calloc(1, sizeof(I8255));

    i8255->peekA    = peekA    ? peekA    : readDummy;
    i8255->readA    = readA    ? readA    : readDummy;
    i8255->writeA   = writeA   ? writeA   : writeDummy;
    i8255->peekB    = peekB    ? peekB    : readDummy;
    i8255->readB    = readB    ? readB    : readDummy;
    i8255->writeB   = writeB   ? writeB   : writeDummy;
    i8255->peekCLo  = peekCLo  ? peekCLo  : readDummy;
    i8255->readCLo  = readCLo  ? readCLo  : readDummy;
    i8255->writeCLo = writeCLo ? writeCLo : writeDummy;
    i8255->peekCHi  = peekCHi  ? peekCHi  : readDummy;
    i8255->readCHi  = readCHi  ? readCHi  : readDummy;
    i8255->writeCHi = writeCHi ? writeCHi : writeDummy;
    i8255->ref      = ref;

    return i8255;
}

void i8255Reset(I8255* i8255)
{
    i8255->reg[3] = 0x9b;

    i8255Write(i8255, 0, 0);
    i8255Write(i8255, 1, 0);
    i8255Write(i8255, 2, 0);
}

void i8255Destroy(I8255* i8255) 
{
    free(i8255);
}

void i8255LoadState(I8255* i8255)
{
    SaveState* state = saveStateOpenForRead("i8255");

    i8255->reg[0] = (UInt8)saveStateGet(state, "reg00", 0);
    i8255->reg[1] = (UInt8)saveStateGet(state, "reg01", 0);
    i8255->reg[2] = (UInt8)saveStateGet(state, "reg02", 0);
    i8255->reg[3] = (UInt8)saveStateGet(state, "reg03", 0);

    saveStateClose(state);
}

void i8255SaveState(I8255* i8255)
{
    SaveState* state = saveStateOpenForWrite("i8255");
    
    saveStateSet(state, "reg00", i8255->reg[0]);
    saveStateSet(state, "reg01", i8255->reg[1]);
    saveStateSet(state, "reg02", i8255->reg[2]);
    saveStateSet(state, "reg03", i8255->reg[3]);

    saveStateClose(state);
}

UInt8 i8255Peek(I8255* i8255, UInt16 port)
{
    UInt8 value;

    port &= 0x03;

    switch (port) {
    case 0:
        switch (i8255->reg[3] & 0x60) {
        case 0x00: // MODE 0
            if (i8255->reg[3] & 0x10) {
                return i8255->peekA(i8255->ref);
            }
            return i8255->reg[0];

        case 0x20: // MODE 1
            return 0xff;

        default: // MODE 2
            return 0xff;
        }
        break;

    case 1:
        switch (i8255->reg[3] & 0x04) {
        case 0x00: // MODE 0
            if (i8255->reg[3] & 0x02) {
                return i8255->peekA(i8255->ref);
            }
            return i8255->reg[1];

        default: // MODE 1
            return 0xff;
        }
        break;

    case 2:
        value = i8255->reg[2];

        if (i8255->reg[3] & 0x01) {
            value = (value & 0xf0) | (i8255->peekCLo(i8255->ref) & 0x0f);
        }
        if (i8255->reg[3] & 0x08) {
            value = (value & 0x0f) | (i8255->peekCHi(i8255->ref) << 4);
        }
        return value;

    case 3:
        return i8255->reg[3];
    }

    return 0xff;
}

UInt8 i8255Read(I8255* i8255, UInt16 port)
{
    UInt8 value;

    port &= 0x03;

    switch (port) {
    case 0:
        switch (i8255->reg[3] & 0x60) {
        case 0x00: // MODE 0
            if (i8255->reg[3] & 0x10) {
                return i8255->readA(i8255->ref);
            }
            return i8255->reg[0];

        case 0x20: // MODE 1
            return 0xff;

        default: // MODE 2
            return 0xff;
        }
        break;

    case 1:
        switch (i8255->reg[3] & 0x04) {
        case 0x00: // MODE 0
            if (i8255->reg[3] & 0x02) {
                return i8255->readB(i8255->ref);
            }
            return i8255->reg[1];

        default: // MODE 1
            return 0xff;
        }
        break;

    case 2:
        // FIXME: Check mode

        value = i8255->reg[2];

        if (i8255->reg[3] & 0x01) {
            value = (value & 0xf0) | (i8255->readCLo(i8255->ref) & 0x0f);
        }
        if (i8255->reg[3] & 0x08) {
            value = (value & 0x0f) | (i8255->readCHi(i8255->ref) << 4);
        }
        return value;

    case 3:
        return i8255->reg[3];
    }

    return 0xff;
}

void i8255Write(I8255* i8255, UInt16 port, UInt8 value)
{
    port &= 0x03;

    switch (port) {
    case 0:
        switch (i8255->reg[3] & 0x60) {
        case 0x00: // MODE 0
            break;
        case 0x20: // MODE 1
            break;
        default: // MODE 2
            break;
        }

        i8255->reg[0] = value;
        
        if (!(i8255->reg[3] & 0x10)) {
            i8255->writeA(i8255->ref, value);
        }
        return;

    case 1:
        switch (i8255->reg[3] & 0x04) {
        case 0x00: // MODE 0
            break;
        default: // MODE 1
            break;
        }

        i8255->reg[1] = value;
        
        if (!(i8255->reg[3] & 0x02)) {
            i8255->writeB(i8255->ref, value);
        }
        return;
        
    case 2:
        i8255->reg[2] = value;

        // FIXME: Check mode

        if (!(i8255->reg[3] & 0x01)) {
            i8255->writeCLo(i8255->ref, value & 0x0f);
        }
        if (!(i8255->reg[3] & 0x08)) {
            i8255->writeCHi(i8255->ref, value >> 4);
        }
        return;

    case 3:
        if (value & 0x80) {
            i8255->reg[3] = value;
            i8255Write(i8255, 0, i8255->reg[0]);
            i8255Write(i8255, 1, i8255->reg[1]);
            i8255Write(i8255, 2, i8255->reg[2]);
        }
        else {
            UInt8 mask = 1 << ((value >> 1) & 0x07);
            if (value & 0x01) {
                i8255Write(i8255, 2, i8255->reg[2] | mask);
            }
            else {
                i8255Write(i8255, 2, i8255->reg[2] & ~mask);
            }
        }
        return;
    }
}

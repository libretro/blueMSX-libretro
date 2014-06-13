/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Z80/R800SaveState.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/09/15 03:33:14 $
**
** Author: Daniel Vik
**
** Description: Save/load state of an R800 object
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2004 Daniel Vik
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
**
******************************************************************************
*/
#include "R800SaveState.h"
#include "SaveState.h"
#include <stdio.h>


#define r800LoadRegisterState(state, regs, index) {             \
    regs.AF.W  = (UInt16)saveStateGet(state, "AF"   #index, 0); \
    regs.BC.W  = (UInt16)saveStateGet(state, "BC"   #index, 0); \
    regs.DE.W  = (UInt16)saveStateGet(state, "DE"   #index, 0); \
    regs.HL.W  = (UInt16)saveStateGet(state, "HL"   #index, 0); \
    regs.IX.W  = (UInt16)saveStateGet(state, "IX"   #index, 0); \
    regs.IY.W  = (UInt16)saveStateGet(state, "IY"   #index, 0); \
    regs.PC.W  = (UInt16)saveStateGet(state, "PC"   #index, 0); \
    regs.SP.W  = (UInt16)saveStateGet(state, "SP"   #index, 0); \
    regs.AF1.W = (UInt16)saveStateGet(state, "AF1"  #index, 0); \
    regs.BC1.W = (UInt16)saveStateGet(state, "BC1"  #index, 0); \
    regs.DE1.W = (UInt16)saveStateGet(state, "DE1"  #index, 0); \
    regs.HL1.W = (UInt16)saveStateGet(state, "HL1"  #index, 0); \
    regs.SH.W  = (UInt16)saveStateGet(state, "SH"   #index, 0); \
    regs.I     = (UInt8) saveStateGet(state, "I"    #index, 0); \
    regs.R     = (UInt8) saveStateGet(state, "R"    #index, 0); \
    regs.R2    = (UInt8) saveStateGet(state, "R2"   #index, 0); \
    regs.iff1  = (UInt8) saveStateGet(state, "iff1" #index, 0); \
    regs.iff2  = (UInt8) saveStateGet(state, "iff2" #index, 0); \
    regs.im    = (UInt8) saveStateGet(state, "im"   #index, 0); \
    regs.halt  = (UInt8) saveStateGet(state, "halt" #index, 0); \
    regs.ei_mode = (UInt8) saveStateGet(state, "ei_mode" #index, 0); \
}

#define r800SaveRegisterState(state, regs, index) {             \
    saveStateSet(state, "AF"   #index, regs.AF.W);              \
    saveStateSet(state, "BC"   #index, regs.BC.W);              \
    saveStateSet(state, "DE"   #index, regs.DE.W);              \
    saveStateSet(state, "HL"   #index, regs.HL.W);              \
    saveStateSet(state, "IX"   #index, regs.IX.W);              \
    saveStateSet(state, "IY"   #index, regs.IY.W);              \
    saveStateSet(state, "PC"   #index, regs.PC.W);              \
    saveStateSet(state, "SP"   #index, regs.SP.W);              \
    saveStateSet(state, "AF1"  #index, regs.AF1.W);             \
    saveStateSet(state, "BC1"  #index, regs.BC1.W);             \
    saveStateSet(state, "DE1"  #index, regs.DE1.W);             \
    saveStateSet(state, "HL1"  #index, regs.HL1.W);             \
    saveStateSet(state, "SH"   #index, regs.SH.W);              \
    saveStateSet(state, "I"    #index, regs.I);                 \
    saveStateSet(state, "R"    #index, regs.R);                 \
    saveStateSet(state, "R2"   #index, regs.R2);                \
    saveStateSet(state, "iff1" #index, regs.iff1);              \
    saveStateSet(state, "iff2" #index, regs.iff2);              \
    saveStateSet(state, "im"   #index, regs.im);                \
    saveStateSet(state, "halt" #index, regs.halt);              \
    saveStateSet(state, "ei_mode" #index, regs.ei_mode);        \
}

void r800LoadState(R800* r800)
{
    SaveState* state = saveStateOpenForRead("r800");
    char tag[32];
    int i;

    r800->systemTime =         saveStateGet(state, "systemTime", 0);
    r800->vdpTime    =         saveStateGet(state, "vdpTime",    0);
    r800->cachePage  = (UInt16)saveStateGet(state, "cachePage",  0);
    r800->dataBus    = (UInt8) saveStateGet(state, "dataBus",    0);
    r800->intState   =         saveStateGet(state, "intState",   0);
    r800->nmiState   =         saveStateGet(state, "nmiState",   0);
    r800->cpuMode    =         saveStateGet(state, "cpuMode",    0);
    r800->oldCpuMode =         saveStateGet(state, "oldCpuMode", 0);
    
    for (i = 0; i < sizeof(r800->delay) / sizeof(r800->delay[0]); i++) {
        sprintf(tag, "delay%d", i);
        r800->delay[i] = saveStateGet(state, tag, 0);
    }

    r800LoadRegisterState(state, r800->regs,        00);
    r800LoadRegisterState(state, r800->regBanks[0], 01);
    r800LoadRegisterState(state, r800->regBanks[1], 02);

    saveStateClose(state);
}

void r800SaveState(R800* r800)
{
    SaveState* state = saveStateOpenForWrite("r800");
    char tag[32];
    int i;

    saveStateSet(state, "systemTime", r800->systemTime);
    saveStateSet(state, "vdpTime",    r800->vdpTime);
    saveStateSet(state, "cachePage",  r800->cachePage);
    saveStateSet(state, "dataBus",    r800->dataBus);
    saveStateSet(state, "intState",   r800->intState);
    saveStateSet(state, "nmiState",   r800->nmiState);
    saveStateSet(state, "cpuMode",    r800->cpuMode);
    saveStateSet(state, "oldCpuMode", r800->oldCpuMode);

    for (i = 0; i < sizeof(r800->delay) / sizeof(r800->delay[0]); i++) {
        sprintf(tag, "delay%d", i);
        saveStateSet(state, tag, r800->delay[i]);
    }

    r800SaveRegisterState(state, r800->regs,        00);
    r800SaveRegisterState(state, r800->regBanks[0], 01);
    r800SaveRegisterState(state, r800->regBanks[1], 02);

    saveStateClose(state);
}

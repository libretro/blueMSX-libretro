/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/MsxPPI.c,v $
**
** $Revision: 1.10 $
**
** $Date: 2006/06/14 19:59:52 $
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
#include "MsxPPI.h"
#include "MediaDb.h"
#include "DeviceManager.h"
#include "DebugDeviceManager.h"
#include "SlotManager.h"
#include "IoPort.h"
#include "I8255.h"
#include "Board.h"
#include "SaveState.h"
#include "KeyClick.h"
#include "ArchInput.h"
#include "Switches.h"
#include "Led.h"
#include "InputEvent.h"
#include "Language.h"
#include <stdlib.h>


static UInt8 getKeyState(int row);


typedef struct {
    int    deviceHandle;
    int    debugHandle;
    I8255* i8255;

    AudioKeyClick* keyClick;

    UInt8 row;
    Int32 regA;
    Int32 regCHi;
} MsxPPI;

static void destroy(MsxPPI* ppi)
{
    ioPortUnregister(0xa8);
    ioPortUnregister(0xa9);
    ioPortUnregister(0xaa);
    ioPortUnregister(0xab);

    audioKeyClickDestroy(ppi->keyClick);
    deviceManagerUnregister(ppi->deviceHandle);
    debugDeviceUnregister(ppi->debugHandle);

    i8255Destroy(ppi->i8255);

    free(ppi);
}

static void reset(MsxPPI* ppi) 
{
    ppi->row       = 0;
    ppi->regA   = -1;
    ppi->regCHi = -1;

    i8255Reset(ppi->i8255);
}

static void loadState(MsxPPI* ppi)
{
    SaveState* state = saveStateOpenForRead("MsxPPI");

    ppi->row    = (UInt8)saveStateGet(state, "row", 0);
    ppi->regA   =        saveStateGet(state, "regA", -1);
    ppi->regCHi =        saveStateGet(state, "regCHi", -1);

    saveStateClose(state);
    
    i8255LoadState(ppi->i8255);
}

static void saveState(MsxPPI* ppi)
{
    SaveState* state = saveStateOpenForWrite("MsxPPI");
    
    saveStateSet(state, "row", ppi->row);
    saveStateSet(state, "regA", ppi->regA);
    saveStateSet(state, "regCHi", ppi->regCHi);

    saveStateClose(state);

    i8255SaveState(ppi->i8255);
}

static void writeA(MsxPPI* ppi, UInt8 value)
{
    if (value != ppi->regA) {
        int i;

        ppi->regA = value;

        for (i = 0; i < 4; i++) {
            slotSetRamSlot(i, value & 3);
            value >>= 2;
        }
    }
}

static void writeCLo(MsxPPI* ppi, UInt8 value)
{
    ppi->row = value;
}

static void writeCHi(MsxPPI* ppi, UInt8 value)
{
    if (value != ppi->regCHi) {
        ppi->regCHi = value;

        audioKeyClick(ppi->keyClick, value & 0x08);
        ledSetCapslock(!(value & 0x04));
    }
}

static UInt8 peekB(MsxPPI* ppi)
{
    UInt8 value = getKeyState(ppi->row);

    if (ppi->row == 8) {
        int renshaSpeed = switchGetRensha();
        if (renshaSpeed) {
            UInt8 renshaOn = (UInt8)((UInt64)renshaSpeed * boardSystemTime() / boardFrequency());
            value |= (renshaOn & 1);
        }
    }

    return value;
}

static UInt8 readB(MsxPPI* ppi)
{
    UInt8 value = getKeyState(ppi->row);

    if (ppi->row == 8) {
        int renshaSpeed = switchGetRensha();
        if (renshaSpeed) {
            UInt8 renshaOn = (UInt8)((UInt64)renshaSpeed * boardSystemTime() / boardFrequency());
            ledSetRensha(renshaSpeed > 14 ? 1 : renshaOn & 2);
            value |= (renshaOn & 1);
        }
        else {
            ledSetRensha(0);
        }
    }

    return value;
}

static void getDebugInfo(MsxPPI* ppi, DbgDevice* dbgDevice)
{
    DbgIoPorts* ioPorts;

    ioPorts = dbgDeviceAddIoPorts(dbgDevice, langDbgDevPpi(), 4);
    dbgIoPortsAddPort(ioPorts, 0, 0xa8, DBG_IO_READWRITE, i8255Peek(ppi->i8255, 0xa8));
    dbgIoPortsAddPort(ioPorts, 1, 0xa9, DBG_IO_READWRITE, i8255Peek(ppi->i8255, 0xa9));
    dbgIoPortsAddPort(ioPorts, 2, 0xaa, DBG_IO_READWRITE, i8255Peek(ppi->i8255, 0xaa));
    dbgIoPortsAddPort(ioPorts, 3, 0xab, DBG_IO_READWRITE, i8255Peek(ppi->i8255, 0xab));
}

void msxPPICreate()
{
    DeviceCallbacks callbacks = { destroy, reset, saveState, loadState };
    DebugCallbacks dbgCallbacks = { getDebugInfo, NULL, NULL, NULL };
    MsxPPI* ppi = malloc(sizeof(MsxPPI));

    ppi->deviceHandle = deviceManagerRegister(RAM_MAPPER, &callbacks, ppi);
    ppi->debugHandle = debugDeviceRegister(DBGTYPE_BIOS, langDbgDevPpi(), &dbgCallbacks, ppi);

    ppi->i8255 = i8255Create(NULL,  NULL,  writeA,
                             peekB, readB, NULL,
                             NULL,  NULL,  writeCLo,
                             NULL,  NULL,  writeCHi,
                             ppi);

    ppi->keyClick = audioKeyClickCreate(boardGetMixer());

    ioPortRegister(0xa8, i8255Read, i8255Write, ppi->i8255);
    ioPortRegister(0xa9, i8255Read, i8255Write, ppi->i8255);
    ioPortRegister(0xaa, i8255Read, i8255Write, ppi->i8255);
    ioPortRegister(0xab, i8255Read, i8255Write, ppi->i8255);

    reset(ppi);
}



static UInt8 getKeyState(int row)
{
    UInt8 keyState = 0;
    switch (row) {
    case 0:
        keyState = (inputEventGetState(EC_0      ) << 0) |
                   (inputEventGetState(EC_1      ) << 1) |
                   (inputEventGetState(EC_2      ) << 2) |
                   (inputEventGetState(EC_3      ) << 3) |
                   (inputEventGetState(EC_4      ) << 4) |
                   (inputEventGetState(EC_5      ) << 5) |
                   (inputEventGetState(EC_6      ) << 6) |
                   (inputEventGetState(EC_7      ) << 7);
        break;
        
    case 1:
        keyState = (inputEventGetState(EC_8      ) << 0) |
                   (inputEventGetState(EC_9      ) << 1) |
                   (inputEventGetState(EC_NEG    ) << 2) |
                   (inputEventGetState(EC_CIRCFLX) << 3) |
                   (inputEventGetState(EC_BKSLASH) << 4) |
                   (inputEventGetState(EC_AT     ) << 5) |
                   (inputEventGetState(EC_LBRACK ) << 6) |
                   (inputEventGetState(EC_SEMICOL) << 7);
        break;
        
    case 2:
        keyState = (inputEventGetState(EC_COLON  ) << 0) |
                   (inputEventGetState(EC_RBRACK ) << 1) |
                   (inputEventGetState(EC_COMMA  ) << 2) |
                   (inputEventGetState(EC_PERIOD ) << 3) |
                   (inputEventGetState(EC_DIV    ) << 4) |
                   (inputEventGetState(EC_UNDSCRE) << 5) |
                   (inputEventGetState(EC_A      ) << 6) |
                   (inputEventGetState(EC_B      ) << 7);
        break;
        
    case 3:
        keyState = (inputEventGetState(EC_C      ) << 0) |
                   (inputEventGetState(EC_D      ) << 1) |
                   (inputEventGetState(EC_E      ) << 2) |
                   (inputEventGetState(EC_F      ) << 3) |
                   (inputEventGetState(EC_G      ) << 4) |
                   (inputEventGetState(EC_H      ) << 5) |
                   (inputEventGetState(EC_I      ) << 6) |
                   (inputEventGetState(EC_J      ) << 7);
        break;
        
    case 4:
        keyState = (inputEventGetState(EC_K      ) << 0) |
                   (inputEventGetState(EC_L      ) << 1) |
                   (inputEventGetState(EC_M      ) << 2) |
                   (inputEventGetState(EC_N      ) << 3) |
                   (inputEventGetState(EC_O      ) << 4) |
                   (inputEventGetState(EC_P      ) << 5) |
                   (inputEventGetState(EC_Q      ) << 6) |
                   (inputEventGetState(EC_R      ) << 7);
        break;
        
    case 5:
        keyState = (inputEventGetState(EC_S      ) << 0) |
                   (inputEventGetState(EC_T      ) << 1) |
                   (inputEventGetState(EC_U      ) << 2) |
                   (inputEventGetState(EC_V      ) << 3) |
                   (inputEventGetState(EC_W      ) << 4) |
                   (inputEventGetState(EC_X      ) << 5) |
                   (inputEventGetState(EC_Y      ) << 6) |
                   (inputEventGetState(EC_Z      ) << 7);
        break;
        
    case 6:
        keyState = (inputEventGetState(EC_LSHIFT ) << 0) |
                   (inputEventGetState(EC_RSHIFT ) << 0) |
                   (inputEventGetState(EC_CTRL   ) << 1) |
                   (inputEventGetState(EC_GRAPH  ) << 2) |
                   (inputEventGetState(EC_CAPS   ) << 3) |
                   (inputEventGetState(EC_CODE   ) << 4) |
                   (inputEventGetState(EC_F1     ) << 5) |
                   (inputEventGetState(EC_F2     ) << 6) |
                   (inputEventGetState(EC_F3     ) << 7);
        break;
        
    case 7:
        keyState = (inputEventGetState(EC_F4     ) << 0) |
                   (inputEventGetState(EC_F5     ) << 1) |
                   (inputEventGetState(EC_ESC    ) << 2) |
                   (inputEventGetState(EC_TAB    ) << 3) |
                   (inputEventGetState(EC_STOP   ) << 4) |
                   (inputEventGetState(EC_BKSPACE) << 5) |
                   (inputEventGetState(EC_SELECT ) << 6) |
                   (inputEventGetState(EC_RETURN ) << 7);
        break;
        
    case 8:
        keyState = (inputEventGetState(EC_SPACE  ) << 0) |
                   (inputEventGetState(EC_CLS    ) << 1) |
                   (inputEventGetState(EC_INS    ) << 2) |
                   (inputEventGetState(EC_DEL    ) << 3) |
                   (inputEventGetState(EC_LEFT   ) << 4) |
                   (inputEventGetState(EC_UP     ) << 5) |
                   (inputEventGetState(EC_DOWN   ) << 6) |
                   (inputEventGetState(EC_RIGHT  ) << 7);
        break;
        
    case 9:
        keyState = (inputEventGetState(EC_NUMMUL ) << 0) |
                   (inputEventGetState(EC_NUMADD ) << 1) |
                   (inputEventGetState(EC_NUMDIV ) << 2) |
                   (inputEventGetState(EC_NUM0   ) << 3) |
                   (inputEventGetState(EC_NUM1   ) << 4) |
                   (inputEventGetState(EC_NUM2   ) << 5) |
                   (inputEventGetState(EC_NUM3   ) << 6) |
                   (inputEventGetState(EC_NUM4   ) << 7);
        break;
        
    case 10:
        keyState = (inputEventGetState(EC_NUM5   ) << 0) |
                   (inputEventGetState(EC_NUM6   ) << 1) |
                   (inputEventGetState(EC_NUM7   ) << 2) |
                   (inputEventGetState(EC_NUM8   ) << 3) |
                   (inputEventGetState(EC_NUM9   ) << 4) |
                   (inputEventGetState(EC_NUMSUB ) << 5) |
                   (inputEventGetState(EC_NUMCOM ) << 6) |
                   (inputEventGetState(EC_NUMPER ) << 7);
        break;
        
    case 11:
        keyState = (inputEventGetState(EC_JIKKOU ) << 1) |
                   (inputEventGetState(EC_TORIKE ) << 3);
    }

    return ~keyState;
}



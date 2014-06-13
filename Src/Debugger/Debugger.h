/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Debugger/Debugger.h,v $
**
** $Revision: 1.19 $
**
** $Date: 2005/08/17 07:03:28 $
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
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "MsxTypes.h"

typedef struct Debugger    Debugger;
typedef struct DbgSnapshot DbgSnapshot;
typedef struct DbgDevice   DbgDevice;


typedef enum { 
    DBGTYPE_UNKNOWN,
    DBGTYPE_CPU, 
    DBGTYPE_CART, 
    DBGTYPE_BIOS, 
    DBGTYPE_RAM, 
    DBGTYPE_AUDIO,
    DBGTYPE_VIDEO,
    DBGTYPE_PORT
} DbgDeviceType;


typedef void (*DebuggerEvent)(void*);
typedef void (*DebuggerTrace)(void*, const char*);
typedef void (*DebuggerSetBp)(void*, UInt16, UInt16, UInt16);

typedef enum { DBG_STOPPED, DBG_PAUSED, DBG_RUNNING } DbgState;

typedef struct {
    int    deviceHandle;
    char   name[32];
    int    writeProtected;
    UInt32 startAddress;
    UInt32 size;
    UInt8  memory[1];
} DbgMemoryBlock;

typedef struct {
    int    deviceHandle;
    char   name[32];
    UInt32 count;
    struct DbgRegister {
        char   name[7];
        UInt8  width;
        UInt32 value;
    } reg[1];
} DbgRegisterBank;

typedef struct {
    int    deviceHandle;
    char   name[32];
    UInt32 size;
    UInt32 callstack[1];
} DbgCallstack;

typedef struct {
    int    deviceHandle;
    char   name[32];
    UInt32 count;
    struct DbgIoPort {
        UInt16 port;
        UInt8  direction;
        UInt8  value;
    } port[1];
} DbgIoPorts;

Debugger* debuggerCreate(DebuggerEvent onEmulatorStart,
                         DebuggerEvent onEmulatorStop,
                         DebuggerEvent onEmulatorPause,
                         DebuggerEvent onEmulatorResume,
                         DebuggerEvent onEmulatorReset,
                         DebuggerTrace onDebugTrace,
                         DebuggerSetBp onDebugSetBp,
                         void* ref);

void debuggerDestroy(Debugger* debugger);

DbgSnapshot*     dbgSnapshotCreate();
void             dbgSnapshotDestroy(DbgSnapshot* dbgSnapshot);
DbgState         dbgGetState();
int              dbgSnapshotGetDeviceCount(DbgSnapshot* dbgSnapshot);
const DbgDevice* dbgSnapshotGetDevice(DbgSnapshot* dbgSnapshot, int deviceIndex);

int                    dbgDeviceGetMemoryBlockCount(DbgDevice* dbgDevice);
const DbgMemoryBlock*  dbgDeviceGetMemoryBlock(DbgDevice* dbgDevice, int memBlockIndex);
int                    dbgDeviceGetRegisterBankCount(DbgDevice* dbgDevice);
const DbgRegisterBank* dbgDeviceGetRegisterBank(DbgDevice* dbgDevice, int regBankIndex);
int                    dbgDeviceGetCallstackCount(DbgDevice* dbgDevice);
const DbgCallstack*    dbgDeviceGetCallstack(DbgDevice* dbgDevice, int callstackIndex);
int                    dbgDeviceGetIoPortsCount(DbgDevice* dbgDevice);
const DbgIoPorts*      dbgDeviceGetIoPorts(DbgDevice* dbgDevice, int ioPortIndex);

int dbgDeviceWriteMemory(DbgMemoryBlock* memoryBlock, void* data, int startAddr, int size);
int dbgDeviceWriteRegister(DbgRegisterBank* regBank, int regIndex, UInt32 value);
int dbgDeviceWriteIoPort(DbgIoPorts* ioPorts, int portIndex, UInt32 value);

void dbgRun();
void dbgStop();
void dbgPause();
void dbgStep();

void dbgSetBreakpoint(UInt16 address);
void dbgClearBreakpoint(UInt16 address);

// Internal structure and interface

#define MAX_DBG_COMPONENTS 4
struct DbgDevice {
    char name[64];
    DbgDeviceType type;
    int deviceHandle;
    int memoryBlockCount;
    int registerBankCount;
    int ioPortsCount;
    DbgMemoryBlock*  memoryBlock[MAX_DBG_COMPONENTS];
    DbgRegisterBank* registerBank[MAX_DBG_COMPONENTS];
    DbgIoPorts*      ioPorts[MAX_DBG_COMPONENTS];
    DbgCallstack*    callstack;
};

void debuggerNotifyEmulatorStart();
void debuggerNotifyEmulatorStop();
void debuggerNotifyEmulatorPause();
void debuggerNotifyEmulatorResume();
void debuggerNotifyEmulatorReset();
void debuggerTrace(const char* str);
void debuggerSetBreakpoint(UInt16 slot, UInt16 page, UInt16 address);

#endif /*DEBUGGER_H*/

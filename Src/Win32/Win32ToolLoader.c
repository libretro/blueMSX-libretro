/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32ToolLoader.c,v $
**
** $Revision: 1.26 $
**
** $Date: 2009-07-01 05:00:23 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik, Tomas Karlsson
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#include <windows.h>
#include "Win32ToolLoader.h"
#include "BlueMSXToolInterface.h"
#include "Debugger.h"
#include "Actions.h"
#include "AppConfig.h"
#include "build_number.h"
#include "version.h"

#ifndef NO_TOOL_SUPPORT

#define MAX_TOOLS 16

struct ToolInfo {
    char description[32];
    HINSTANCE library;
    BlueDebugger* debugger;

    struct {
        NotifyFn  show;
        NotifyFn  destroy;
        NotifyFn  onEmulatorStart;
        NotifyFn  onEmulatorStop;
        NotifyFn  onEmulatorPause;
        NotifyFn  onEmulatorResume;
        NotifyFn  onEmulatorReset;
        TraceFn   onEmulatorTrace;
        SetBpFn   onEmulatorSetBp;
        SetLgFn   onEmulatorSetLg;
        GetNameFn getName;
    } callbacks;
};

static ToolInfo* toolList[MAX_TOOLS];
static int       toolListCount = 0;
static volatile int isUpdating = 0;
static char toolDir[MAX_PATH] = "";

static void onEmulatorStart(ToolInfo* toolInfo) {
    if (!isUpdating && toolInfo->callbacks.onEmulatorStart != NULL) {
        toolInfo->callbacks.onEmulatorStart();
    }
}

static void onEmulatorStop(ToolInfo* toolInfo) {
    if (!isUpdating && toolInfo->callbacks.onEmulatorStop != NULL) {
        toolInfo->callbacks.onEmulatorStop();
    }
}

static void onEmulatorPause(ToolInfo* toolInfo) {
    if (!isUpdating && toolInfo->callbacks.onEmulatorPause != NULL) {
        toolInfo->callbacks.onEmulatorPause();
    }
}

static void onEmulatorResume(ToolInfo* toolInfo) {
    if (!isUpdating && toolInfo->callbacks.onEmulatorResume != NULL) {
        toolInfo->callbacks.onEmulatorResume();
    }
}

static void onEmulatorReset(ToolInfo* toolInfo) {
    if (!isUpdating && toolInfo->callbacks.onEmulatorReset != NULL) {
        toolInfo->callbacks.onEmulatorReset();
    }
}

static void onEmulatorTrace(ToolInfo* toolInfo, const char* str) {
    if (toolInfo->callbacks.onEmulatorTrace != NULL) {
        toolInfo->callbacks.onEmulatorTrace(str);
    }
}

static void onEmulatorSetBp(ToolInfo* toolInfo, UInt16 slot, UInt16 page, UInt16 address) {
    if (toolInfo->callbacks.onEmulatorSetBp != NULL) {
        toolInfo->callbacks.onEmulatorSetBp(slot, page, address);
    }
}

Snapshot* __stdcall toolSnapshotCreate() {
    if (dbgGetState() == DBG_RUNNING) {
        Snapshot* snapshot;
        isUpdating = 1;
        actionEmuTogglePause();
        snapshot = (Snapshot*)dbgSnapshotCreate(NULL);
        actionEmuTogglePause();
        isUpdating = 0;
        return snapshot;
    }
    return (Snapshot*)dbgSnapshotCreate(NULL);
}

void __stdcall toolSnapshotDestroy(Snapshot* s) {
    dbgSnapshotDestroy((DbgSnapshot*)s);
}

EmulatorState __stdcall toolGetState() {
    return dbgGetState();
}

int __stdcall toolSnapshotGetDeviceCount(Snapshot* s)
{
    return dbgSnapshotGetDeviceCount((DbgSnapshot*)s);
}

Device* __stdcall toolSnapshotGetDevice(Snapshot* s, int d)
{
    return (Device*)dbgSnapshotGetDevice((DbgSnapshot*)s, d);
}

int __stdcall toolDeviceGetMemoryBlockCount(Device* d)
{
    return dbgDeviceGetMemoryBlockCount((DbgDevice*)d);
}

MemoryBlock* __stdcall toolDeviceGetMemoryBlock(Device* d, int m)
{
    return (MemoryBlock*)dbgDeviceGetMemoryBlock((DbgDevice*)d, m);
}

int __stdcall toolDeviceGetRegisterBankCount(Device* d)
{
    return dbgDeviceGetRegisterBankCount((DbgDevice*)d);
}

RegisterBank* __stdcall toolDeviceGetRegisterBank(Device* d, int r)
{
    return (RegisterBank*)dbgDeviceGetRegisterBank((DbgDevice*)d, r);
}

int __stdcall toolDeviceGetCallstackCount(Device* d)
{
    return dbgDeviceGetCallstackCount((DbgDevice*)d);
}

Callstack* __stdcall toolDeviceGetCallstack(Device* d, int c)
{
    return (Callstack*)dbgDeviceGetCallstack((DbgDevice*)d, c);
}

int __stdcall toolDeviceGetIoPortsCount(Device* d)
{
    return dbgDeviceGetIoPortsCount((DbgDevice*)d);
}

IoPorts* __stdcall toolDeviceGetIoPorts(Device* d, int p)
{
    return (IoPorts*)dbgDeviceGetIoPorts((DbgDevice*)d, p);
}

void __stdcall toolRun()
{
    dbgRun();
}

void __stdcall toolStop()
{
    dbgStop();
}

void __stdcall toolPause()
{
    dbgPause();
}

void __stdcall toolStep()
{
    dbgStep();
}

void __stdcall toolStepBack()
{
    dbgStepBack();
}

void __stdcall toolSetBreakpoint(UInt16 address)
{
    dbgSetBreakpoint(address);
}

void __stdcall toolClearBreakpoint(UInt16 address)
{
    dbgClearBreakpoint(address);
}

void __stdcall toolEnableVramAccessCheck(int enable)
{
    dbgEnableVramAccessCheck(enable);
}

void __stdcall toolSetWatchpoint(DeviceType devType, int address, DbgWatchpointCondition condition, UInt32 referenceValue, int size)
{
    dbgSetWatchpoint((DbgDeviceType)devType, address, condition, referenceValue, size);
}

void __stdcall toolClearWatchpoint(DeviceType devType, int address)
{
    dbgClearWatchpoint((DbgDeviceType)devType, address);
}

int __stdcall toolWriteMemory(MemoryBlock* memoryBlock, void* data, int startAddr, int size)
{
    return dbgDeviceWriteMemory((DbgMemoryBlock*)memoryBlock, data, startAddr, size);
}

int __stdcall toolWriteRegister(RegisterBank* regBank, int regIndex, UInt32 value)
{
    return dbgDeviceWriteRegister((DbgRegisterBank*)regBank, regIndex, value);
}

int __stdcall toolDeviceWriteIoPort(IoPorts* ioPorts, int portIndex, UInt32 value)
{
    return dbgDeviceWriteIoPort((DbgIoPorts*)ioPorts, portIndex, value);
}

char* __stdcall toolGetPath()
{
    return toolDir;
}

void __stdcall toolGetEmulatorVersion(int* major, int* minor, int* buildNumber)
{
    *major = BLUE_MSX_VERSION_MAJOR;
    *minor = BLUE_MSX_VERSION_MINOR;
    *buildNumber = BUILD_NUMBER;
}

static Interface toolInterface = {
    toolSnapshotCreate,
    toolSnapshotDestroy,
    toolGetState,
    toolSnapshotGetDeviceCount,
    toolSnapshotGetDevice,
    toolDeviceGetMemoryBlockCount,
    toolDeviceGetMemoryBlock,
    toolWriteMemory,
    toolDeviceGetRegisterBankCount,
    toolDeviceGetRegisterBank,
    toolWriteRegister,
    toolDeviceGetCallstackCount,
    toolDeviceGetCallstack,
    toolDeviceGetIoPortsCount,
    toolDeviceGetIoPorts,
    toolDeviceWriteIoPort,
    toolRun,
    toolStop,
    toolPause,
    toolStep,
    toolSetBreakpoint,
    toolClearBreakpoint,
    toolGetPath,
    toolGetEmulatorVersion,
    toolEnableVramAccessCheck,
    toolSetWatchpoint,
    toolClearWatchpoint,
    toolStepBack,
};

void toolLoadAll(const char* path, int languageId)
{
    WIN32_FIND_DATA wfd;
    char  curDir[MAX_PATH];
    HANDLE handle;

    if (appConfigGetInt("toolsenable", 1) == 0) {
        return;
    }

    GetCurrentDirectory(MAX_PATH, curDir);
    strcat(toolDir, curDir);
    strcat(toolDir, "\\Tools");

    if (!SetCurrentDirectory(toolDir)) {
        return;
    }

    handle = FindFirstFile("*.dll", &wfd);

    if (handle == INVALID_HANDLE_VALUE) {
        SetCurrentDirectory(curDir);
        return;
    }

    do {
        ToolInfo* toolInfo;
        HINSTANCE lib = LoadLibrary(wfd.cFileName);

        if (lib != NULL) {
            char description[32] = "Unknown";
            CreateFn  create   = (CreateFn) GetProcAddress(lib, "Create12");
            NotifyFn  destroy  = (NotifyFn) GetProcAddress(lib, "Destroy");
            NotifyFn  show     = (NotifyFn) GetProcAddress(lib, "Show");
            NotifyFn  onStart  = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorStart");
            NotifyFn  onStop   = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorStop");
            NotifyFn  onPause  = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorPause");
            NotifyFn  onResume = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorResume");
            NotifyFn  onReset  = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorReset");
            TraceFn   onTrace  = (TraceFn)  GetProcAddress(lib, "EmulatorTrace");
            SetBpFn   onSetBp  = (SetBpFn)  GetProcAddress(lib, "EmulatorSetBreakpoint");
            SetLgFn   onSetLg  = (SetLgFn)  GetProcAddress(lib, "SetLanguage");
            GetNameFn onGetNm  = (GetNameFn)GetProcAddress(lib, "GetName");

            if (create == NULL) {
                // Check old style dll exports (of blueMSX 2.8.1)
                create   = (CreateFn) GetProcAddress(lib, "Create11");
                destroy  = (NotifyFn) GetProcAddress(lib, "Destroy");
                show     = (NotifyFn) GetProcAddress(lib, "Show");
                onStart  = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorStart");
                onStop   = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorStop");
                onPause  = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorPause");
                onResume = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorResume");
                onReset  = (NotifyFn) GetProcAddress(lib, "NotifyEmulatorReset");
                onTrace  = (TraceFn)  GetProcAddress(lib, "EmulatorTrace");
                onSetBp  = (SetBpFn)  GetProcAddress(lib, "EmulatorSetBreakpoint");
                onSetLg  = (SetLgFn)  GetProcAddress(lib, "SetLanguage");
                onGetNm  = (GetNameFn)GetProcAddress(lib, "GetName");
            }
            if (create == NULL) {
                // Check old style dll exports (of blueMSX 2.2)
                create   = (CreateFn)GetProcAddress(lib, (LPCSTR)1);
                destroy  = (NotifyFn)GetProcAddress(lib, (LPCSTR)2);
                show     = (NotifyFn)GetProcAddress(lib, (LPCSTR)3);
                onStart  = (NotifyFn)GetProcAddress(lib, (LPCSTR)4);
                onStop   = (NotifyFn)GetProcAddress(lib, (LPCSTR)5);
                onPause  = (NotifyFn)GetProcAddress(lib, (LPCSTR)6);
                onResume = (NotifyFn)GetProcAddress(lib, (LPCSTR)7);
                onReset  = (NotifyFn)GetProcAddress(lib, (LPCSTR)8);
                onTrace  = NULL;
                onSetBp  = NULL;
                onSetLg  = NULL;
                onGetNm  = NULL;


                if (create == NULL) {
                    FreeLibrary(lib);
                    continue;
                }
            }

            if (create(&toolInterface, description, 31) == 0) {
                FreeLibrary(lib);
                continue;
            }

            toolInfo = malloc(sizeof(ToolInfo));
            toolInfo->callbacks.destroy           = destroy;
            toolInfo->callbacks.show              = show;
            toolInfo->callbacks.onEmulatorStart   = onStart;
            toolInfo->callbacks.onEmulatorStop    = onStop;
            toolInfo->callbacks.onEmulatorPause   = onPause;
            toolInfo->callbacks.onEmulatorResume  = onResume;
            toolInfo->callbacks.onEmulatorReset   = onReset;
            toolInfo->callbacks.onEmulatorTrace   = onTrace;
            toolInfo->callbacks.onEmulatorSetBp   = onSetBp;
            toolInfo->callbacks.onEmulatorSetLg   = onSetLg;
            toolInfo->callbacks.getName           = onGetNm;
            toolInfo->debugger = debuggerCreate(onStart  ? onEmulatorStart  : NULL, 
                                                onStop   ? onEmulatorStop   : NULL, 
                                                onPause  ? onEmulatorPause  : NULL, 
                                                onResume ? onEmulatorResume : NULL, 
                                                onReset  ? onEmulatorReset  : NULL, 
                                                onTrace  ? onEmulatorTrace  : NULL, 
                                                onSetBp  ? onEmulatorSetBp  : NULL,
                                                toolInfo);
            toolInfo->library = lib;
            strcpy(toolInfo->description, description);

            toolList[toolListCount++] = toolInfo;

            toolInfoSetLanguage(toolInfo, languageId);
        }
    } while (FindNextFile(handle, &wfd));
	
	FindClose( handle );

    SetCurrentDirectory(curDir);
}

void toolUnLoadAll()
{
    int i;

    if (appConfigGetInt("toolsenable", 1) == 0) {
        return;
    }

    for (i = 0; i < toolListCount; i++) {
        if (toolList[i]->callbacks.destroy != NULL) {
            toolList[i]->callbacks.destroy();
        }
        FreeLibrary((HMODULE)toolList[i]->library);
        free(toolList[i]);
    }
}

int toolGetCount() {
    if (appConfigGetInt("toolsenable", 1) == 0) {
        return 0;
    }

    return toolListCount;
}

ToolInfo* toolInfoGet(int index)
{
    if (appConfigGetInt("toolsenable", 1) == 0) {
        return NULL;
    }

    if (index < 0 || index >= toolListCount) {
        return NULL;
    }

    return toolList[index];
}


ToolInfo* toolInfoFind(char* name)
{
    int i;

    if (appConfigGetInt("toolsenable", 1) == 0) {
        return NULL;
    }

    for (i = 0; i < toolListCount; i++) {
        if (strcmp(toolList[i]->description, name) == 0) {
            return toolList[i];
        }
    }
    return NULL;
}

const char* toolInfoGetName(ToolInfo* toolInfo)
{
    if (appConfigGetInt("toolsenable", 1) == 0) {
        return "";
    }

    if (toolInfo->callbacks.getName != NULL) {
        return toolInfo->callbacks.getName();
    }
    return toolInfo->description;
}

void toolInfoShowTool(ToolInfo* toolInfo)
{
    if (appConfigGetInt("toolsenable", 1) == 0) {
        return;
    }

    if (toolInfo->callbacks.show != NULL) {
        toolInfo->callbacks.show();
    }
}

void toolInfoSetLanguage(ToolInfo* toolInfo, int langId) 
{
    if (appConfigGetInt("toolsenable", 1) == 0) {
        return;
    }

    if (toolInfo->callbacks.onEmulatorSetLg != NULL) {
        toolInfo->callbacks.onEmulatorSetLg(langId);
    }
}

#endif

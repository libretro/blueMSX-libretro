/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/GameReader.cpp,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/06/15 16:05:42 $
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
extern "C" {
#include "GameReader.h"
};
#include "msxgr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#define GlobalAlloc(xxx, addr) malloc(addr)
#define GlobalFree(addr) free(addr)
#endif


//#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define dprintf printf
#else
#define dprintf if (0) printf
#endif


static CMSXGr* MsxGr;

struct GameReader 
{
public:
    GameReader(int grSlot = -1);
    ~GameReader();

    inline bool readMemory(UInt16 address, void* buffer, int length);
    inline bool writeMemory(UInt16 address, void* buffer, int length);
    
    inline bool readIo(UInt16 port, UInt8* value);
    inline bool writeIo(UInt16 port, UInt8 value);

private:
    bool inserted;
    int  slot;
    char* globalBuffer;
};


GameReader::GameReader(int grSlot) : 
    inserted(false),
    slot(grSlot),
    globalBuffer((char*)GlobalAlloc(GPTR, 0x4000))
{
}

GameReader::~GameReader() {
    GlobalFree(globalBuffer);
}

bool GameReader::readMemory(UInt16 address, void* buffer, int length)
{
    if (slot == -1) {
        return false;
    }
    
    if (!inserted) {
        inserted = MsxGr->IsCartridgeInserted(slot);
    }
    
    if (inserted) {
        if (MsxGr->ReadMemory(slot, globalBuffer, address, length) != 0) {
            inserted = MsxGr->IsCartridgeInserted(slot);
            return false;
        }
        memcpy(buffer, globalBuffer, length);
    }
    return true;
}

bool GameReader::writeMemory(UInt16 address, void* buffer, int length)
{
    if (slot == -1) {
        return false;
    }
    
    if (!inserted) {
        inserted = MsxGr->IsCartridgeInserted(slot);
    }
    
    if (inserted) {
        memcpy(globalBuffer, buffer, length);
        if (MsxGr->WriteMemory(slot, globalBuffer, address, length) != 0) {
            inserted = MsxGr->IsCartridgeInserted(slot);
            return false;
        }
    }
    return true;
}

bool GameReader::readIo(UInt16 port, UInt8* value)
{
    if (slot == -1) {
        return false;
    }
    
    if (!inserted) {
        inserted = MsxGr->IsCartridgeInserted(slot);
    }

    if (inserted) {
        if (MsxGr->ReadIO(slot, globalBuffer, port, 1) != 0) {
            inserted = MsxGr->IsCartridgeInserted(slot);
            return false;
        }
        *value = *(UInt8*)globalBuffer;
    }
    return true;
}

bool GameReader::writeIo(UInt16 port, UInt8 value)
{
    if (slot == -1) {
        return false;
    }
    
    if (!inserted) {
        inserted = MsxGr->IsCartridgeInserted(slot);
    }

    if (inserted) {
        *(UInt8*)globalBuffer = value;
        if (MsxGr->WriteIO(slot, globalBuffer, port, 1) != 0) {
            inserted = MsxGr->IsCartridgeInserted(slot);
            return false;
        }
    }
    return true;
}

#define MAX_GAMEREADERS 2

static GameReader* GameReaders[MAX_GAMEREADERS] = { NULL, NULL };

static void InitializeGameReaders()
{
    if (MsxGr == NULL) {
        MsxGr = new CMSXGr;

        int gameReaderCount = 0;

        if (MsxGr->Init() == 0) {
            for (int i = 0; i < 16 && gameReaderCount < MAX_GAMEREADERS; i++) {
                if (MsxGr->IsSlotEnable(i)) {
                    GameReaders[gameReaderCount++] = new GameReader(i);
                }
            }
        }

        for (; gameReaderCount < 2; gameReaderCount++) {
            GameReaders[gameReaderCount] = new GameReader;
        }
    }
}

static void DeinitializeGameReaders()
{
    if (MsxGr != NULL) {
        for (int i = 0; i < MAX_GAMEREADERS; i++) {
            if (GameReaders[i] != NULL) {
                delete GameReaders[i];
                GameReaders[i] = NULL;
            }
        }
        delete MsxGr;
        MsxGr = NULL;
    }
}

/////////////////////////////////////////////////////////////
//
// Public C interface

extern "C" GrHandle* gameReaderCreate(int slot)
{
    InitializeGameReaders();

    return (GrHandle*)GameReaders[slot];
}

extern "C" void gameReaderDestroy(GrHandle* grHandle)
{
    DeinitializeGameReaders();
}

extern "C" int gameReaderRead(GrHandle* grHandle, UInt16 address, void* buffer, int length)
{
    return ((GameReader*)grHandle)->readMemory(address, buffer, length) ? 1 : 0;
}

extern "C" int gameReaderWrite(GrHandle* grHandle, UInt16 address, void* buffer, int length)
{
    return ((GameReader*)grHandle)->writeMemory(address, buffer, length) ? 1 : 0;
}

extern "C" int gameReaderReadIo(GrHandle* grHandle, UInt16 port, UInt8* value)
{
    return ((GameReader*)grHandle)->readIo(port, value) ? 1 : 0;
}

extern "C" int gameReaderWriteIo(GrHandle* grHandle, UInt16 port, UInt8 value)
{
    return ((GameReader*)grHandle)->writeIo(port, value) ? 1 : 0;
}

extern "C" int gameReaderSupported()
{
    return CMSXGr::IsImplemented() ? 1 : 0;
}

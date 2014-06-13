/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Board/Board.c,v $
**
** $Revision: 1.47 $
**
** $Date: 2006/05/30 22:32:43 $
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
#include "Board.h"
#include "MSX.h"
#include "SVI.h"
#include "SG1000.h"
#include "Coleco.h"
#include "Adam.h"
#include "AudioMixer.h"
#include "YM2413.h"
#include "Y8950.h"
#include "Moonsound.h"
#include "SaveState.h"
#include "ziphelper.h"
#include "ArchNotifications.h"
#include "VideoManager.h"
#include "DebugDeviceManager.h"
#include "MegaromCartridge.h"
#include "Disk.h"
#include "VideoManager.h"
#include "Casette.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern void PatchReset(BoardType boardType);

static int pendingInt;
static int boardType;
static Mixer* boardMixer = NULL;
static int (*syncToRealClock)(int, int) = NULL;
UInt32* boardSysTime;
static UInt64 boardSysTime64;
static UInt32 oldTime;
static UInt32 boardFreq = boardFrequency();
static int fdcTimingEnable = 1;
static int fdcActive       = 0;
static BoardTimer* fdcTimer;
static BoardTimer* syncTimer;
static BoardDeviceInfo* boardDeviceInfo;
static Machine* boardMachine;
static BoardInfo boardInfo;
static UInt32 boardRamSize;
static UInt32 boardVramSize;
static int boardRunning = 0;

static HdType hdType[MAX_HD_COUNT];
  
static int cartIdeCount[5];

static int     useRom;
static int     useMegaRom;
static int     useMegaRam;
static int     useFmPac;
static RomType currentRomType[2];

static UInt8 emptyRam[0x2000];

static BoardType boardLoadState(const char* stateFile);

static char saveStateVersion[32] = "blueMSX - state  v 8";

int boardGetFdcTimingEnable() {
    return fdcTimingEnable;
}

void boardSetFdcTimingEnable(int enable) {
    fdcTimingEnable = enable;
}

void boardSetFdcActive() {
    if (!fdcTimingEnable) {
        boardTimerAdd(fdcTimer, boardSystemTime() + (UInt32)((UInt64)500 * boardFrequency() / 1000));
        fdcActive = 1;
    }
}

void boardSetBreakpoint(UInt16 address) {
    if (boardRunning) {
        boardInfo.setBreakpoint(boardInfo.cpuRef, address);
    }
}

void boardClearBreakpoint(UInt16 address) {
    if (boardRunning) {
        boardInfo.clearBreakpoint(boardInfo.cpuRef, address);
    }
}

static void onFdcDone(void* ref, UInt32 time)
{
    fdcActive = 0;
}

static void doSync(UInt32 time, int breakpointHit)
{
    int execTime = syncToRealClock(fdcActive, breakpointHit);
    if (execTime < 0) {
        boardInfo.stop(boardInfo.cpuRef);
        return;
    }

    mixerSync(boardMixer);

    if (execTime == 0) {
        boardTimerAdd(syncTimer, boardSystemTime() + 1);
    }
    else {
        boardTimerAdd(syncTimer, time + (UInt32)((UInt64)execTime * boardFreq / 1000));
    }
}

static void onSync(void* ref, UInt32 time)
{
    doSync(time, 0);
}

void boardOnBreakpoint(UInt16 pc)
{
    doSync(boardSystemTime(), 1);
}

int boardInsertExternalDevices()
{
    int i;
    for (i = 0; i < 2; i++) {
        if (boardDeviceInfo->carts[i].inserted) {
            boardChangeCartridge(i, boardDeviceInfo->carts[i].type, 
                                 boardDeviceInfo->carts[i].name,
                                 boardDeviceInfo->carts[i].inZipName);
        }
    }

    for (i = 0; i < MAXDRIVES; i++) {
        if (boardDeviceInfo->disks[i].inserted) {
            boardChangeDiskette(i, boardDeviceInfo->disks[i].name,
                                boardDeviceInfo->disks[i].inZipName);
        }
    }

    if (boardDeviceInfo->tapes[0].inserted) {
        boardChangeCassette(0, boardDeviceInfo->tapes[0].name,
                            boardDeviceInfo->tapes[0].inZipName);
    }
    return 1;
}

int boardRemoveExternalDevices()
{
     boardChangeDiskette(0, NULL, NULL);
     boardChangeDiskette(1, NULL, NULL);

     boardChangeCassette(0, NULL, NULL);

     return 1;
}

int boardRun(Machine* machine, 
             BoardDeviceInfo* deviceInfo,
             Mixer* mixer,
             char* stateFile,
             int frequency,
             int (*syncCallback)(int, int))
{
    int loadState = 0;
    int success = 0;

    syncToRealClock = syncCallback;

    videoManagerReset();
    debugDeviceManagerReset();

    boardMixer      = mixer;
    boardDeviceInfo = deviceInfo;
    boardMachine    = machine;

    if (stateFile != NULL) {
        BoardType loadBoardType = boardLoadState(stateFile);
        if (loadBoardType != BOARD_UNKNOWN) {
            boardType = loadBoardType;
            machineLoadState(boardMachine);
            loadState = 1;
        }
    }

    boardType = machine->board.type;
    PatchReset(boardType);

#if 0
    useRom     = 0;
    useMegaRom = 0;
    useMegaRam = 0;
    useFmPac   = 0;
    currentRomType[0] = ROM_UNKNOWN;
    currentRomType[1] = ROM_UNKNOWN;
#endif

    pendingInt = 0;

    boardSetFrequency(frequency);

    boardRunning = 1;
    switch (boardType) {
    case BOARD_MSX:
    case BOARD_MSX_S3527:
    case BOARD_MSX_S1985:
    case BOARD_MSX_T9769B:
    case BOARD_MSX_T9769C:
        success = msxCreate(machine, deviceInfo->video.vdpSyncMode, &boardInfo);
        break;
    case BOARD_SVI:
        success = sviCreate(machine, deviceInfo->video.vdpSyncMode, &boardInfo);
        break;
    case BOARD_COLECO:
        success = colecoCreate(machine, deviceInfo->video.vdpSyncMode, &boardInfo);
        break;
    case BOARD_COLECOADAM:
        success = adamCreate(machine, deviceInfo->video.vdpSyncMode, &boardInfo);
        break;
    case BOARD_SG1000:
        success = sg1000Create(machine, deviceInfo->video.vdpSyncMode, &boardInfo);
        break;
    default:
        success = 0;
    }

    if (success && loadState) {
        boardInfo.loadState();
    }

    if (success) {
        syncTimer = boardTimerCreate(onSync, NULL);
        fdcTimer = boardTimerCreate(onFdcDone, NULL);
        
        boardTimerAdd(syncTimer, boardSystemTime() + 1);

        boardInfo.run(boardInfo.cpuRef);

        boardInfo.destroy();

        boardTimerDestroy(fdcTimer);
        boardTimerDestroy(syncTimer);
    }

    boardRunning = 0;

    return success;
}

BoardType boardGetType()
{
    return boardType & BOARD_MASK;
}

Mixer* boardGetMixer()
{
    return boardMixer;
}

void boardSetMachine(Machine* machine)
{
    int i;
    int hdIndex = FIRST_INTERNAL_HD_INDEX;

    // Update HD info
    for (i = FIRST_INTERNAL_HD_INDEX; i < MAX_HD_COUNT; i++) {
        hdType[i] = HD_NONE;
    }
    for (i = 0; i < machine->slotInfoCount; i++) {
        switch (machine->slotInfo[i].romType) {
        case ROM_SUNRISEIDE: hdType[hdIndex++] = HD_SUNRISEIDE; break;
        case ROM_BEERIDE:    hdType[hdIndex++] = HD_BEERIDE;    break;
        case ROM_GIDE:       hdType[hdIndex++] = HD_GIDE;       break;
        }
    }

    // Update RAM info
    boardRamSize  = 0;
    boardVramSize = machine->video.vramSize;

    for (i = 0; i < machine->slotInfoCount; i++) {
        if (machine->slotInfo[i].romType == RAM_1KB_MIRRORED) {
            boardRamSize = 0x400;
        }
    }

    if (boardRamSize == 0) {
        for (i = 0; i < machine->slotInfoCount; i++) {
            if (machine->slotInfo[i].romType == RAM_NORMAL || machine->slotInfo[i].romType == RAM_MAPPER) {
                boardRamSize = 0x2000 * machine->slotInfo[i].pageCount;
            }
        }
    }

    boardType = machine->board.type;
    PatchReset(boardType);
}

void boardReset()
{
    if (boardRunning) {
        boardInfo.softReset();
    }
}

static BoardType boardLoadState(const char* stateFile)
{
    BoardDeviceInfo* di = boardDeviceInfo;
    SaveState* state;
    BoardType boardType;
    char* version;
    int   size;

    saveStateCreate(stateFile);
    version = zipLoadFile(stateFile, "version", &size);
    if (version == NULL) {
        return BOARD_UNKNOWN;
    }

    if (0 != strncmp(version, saveStateVersion, sizeof(saveStateVersion) - 1)) {
        free(version);
        return BOARD_UNKNOWN;
    }

    free(version);
            
    state = saveStateOpenForRead("board");

    boardType      = saveStateGet(state, "boardType", BOARD_MSX);
    boardSysTime64 = (UInt64)saveStateGet(state, "boardSysTime64Hi", 0) << 32 |
                     (UInt64)saveStateGet(state, "boardSysTime64Lo", 0);
    oldTime        = saveStateGet(state, "oldTime", 0);
    pendingInt     = saveStateGet(state, "pendingInt", 0);
    
    di->carts[0].inserted = saveStateGet(state, "cartInserted00", 0);
    di->carts[0].type     = saveStateGet(state, "cartType00",     0);
    saveStateGetBuffer(state, "cartName00",  di->carts[0].name, sizeof(di->carts[0].name));
    saveStateGetBuffer(state, "cartInZip00", di->carts[0].inZipName, sizeof(di->carts[0].inZipName));

    di->carts[1].inserted = saveStateGet(state, "cartInserted01", 0);
    di->carts[1].type     = saveStateGet(state, "cartType01",     0);
    saveStateGetBuffer(state, "cartName01",  di->carts[1].name, sizeof(di->carts[1].name));
    saveStateGetBuffer(state, "cartInZip01", di->carts[1].inZipName, sizeof(di->carts[1].inZipName));

    di->disks[0].inserted = saveStateGet(state, "diskInserted00", 0);
    saveStateGetBuffer(state, "diskName00",  di->disks[0].name, sizeof(di->disks[0].name));
    saveStateGetBuffer(state, "diskInZip00", di->disks[0].inZipName, sizeof(di->disks[0].inZipName));

    di->disks[1].inserted = saveStateGet(state, "diskInserted01", 0);
    saveStateGetBuffer(state, "diskName01",  di->disks[1].name, sizeof(di->disks[1].name));
    saveStateGetBuffer(state, "diskInZip01", di->disks[1].inZipName, sizeof(di->disks[1].inZipName));

    di->tapes[0].inserted = saveStateGet(state, "casInserted", 0);
    saveStateGetBuffer(state, "casName",  di->tapes[0].name, sizeof(di->tapes[0].name));
    saveStateGetBuffer(state, "casInZip", di->tapes[0].inZipName, sizeof(di->tapes[0].inZipName));

    di->video.vdpSyncMode = saveStateGet(state, "vdpSyncMode", 0);

    saveStateClose(state);

    videoManagerLoadState();
    tapeLoadState();

    return boardType;
}


void boardSaveState(const char* stateFile)
{
    BoardDeviceInfo* di = boardDeviceInfo;
    char buf[128];
    time_t ltime;
    SaveState* state;
    int size;
    void* bitmap;
    int rv;

    if (!boardRunning) {
        return;
    }

    saveStateCreate(stateFile);
    
    rv = zipSaveFile(stateFile, "version", 0, saveStateVersion, strlen(saveStateVersion) + 1);
    if (!rv) {
        return;
    }
    
    state = saveStateOpenForWrite("board");

    saveStateSet(state, "pendingInt", pendingInt);
    saveStateSet(state, "boardType", boardType);
    saveStateSet(state, "boardSysTime64Hi", (UInt32)(boardSysTime64 >> 32));
    saveStateSet(state, "boardSysTime64Lo", (UInt32)boardSysTime64);
    saveStateSet(state, "oldTime", oldTime);

    saveStateSet(state, "cartInserted00", di->carts[0].inserted);
    saveStateSet(state, "cartType00",     di->carts[0].type);
    saveStateSetBuffer(state, "cartName00",  di->carts[0].name, strlen(di->carts[0].name) + 1);
    saveStateSetBuffer(state, "cartInZip00", di->carts[0].inZipName, strlen(di->carts[0].inZipName) + 1);
    saveStateSet(state, "cartInserted01", di->carts[1].inserted);
    saveStateSet(state, "cartType01",     di->carts[1].type);
    saveStateSetBuffer(state, "cartName01",  di->carts[1].name, strlen(di->carts[1].name) + 1);
    saveStateSetBuffer(state, "cartInZip01", di->carts[1].inZipName, strlen(di->carts[1].inZipName) + 1);

    saveStateSet(state, "diskInserted00", di->disks[0].inserted);
    saveStateSetBuffer(state, "diskName00",  di->disks[0].name, strlen(di->disks[0].name) + 1);
    saveStateSetBuffer(state, "diskInZip00", di->disks[0].inZipName, strlen(di->disks[0].inZipName) + 1);
    saveStateSet(state, "diskInserted01", di->disks[1].inserted);
    saveStateSetBuffer(state, "diskName01",  di->disks[1].name, strlen(di->disks[1].name) + 1);
    saveStateSetBuffer(state, "diskInZip01", di->disks[1].inZipName, strlen(di->disks[1].inZipName) + 1);

    saveStateSet(state, "casInserted", di->tapes[0].inserted);
    saveStateSetBuffer(state, "casName",  di->tapes[0].name, strlen(di->tapes[0].name) + 1);
    saveStateSetBuffer(state, "casInZip", di->tapes[0].inZipName, strlen(di->tapes[0].inZipName) + 1);

    saveStateSet(state, "vdpSyncMode",   di->video.vdpSyncMode);

    saveStateClose(state);

    videoManagerSaveState();
    tapeSaveState();

    // Save machine state
    machineSaveState(boardMachine);

    // Call board dependent save state
    boardInfo.saveState(stateFile);

    bitmap = archScreenCapture(SC_SMALL, &size, 1);
    if (size > 0) {
        zipSaveFile(stateFile, "screenshot.bmp", 1, bitmap, size);
    }
    free(bitmap);

    memset(buf, 0, 128);
    time(&ltime);
    strftime(buf, 128, "%X   %A, %B %d, %Y", localtime(&ltime));
    zipSaveFile(stateFile, "date.txt", 1, buf, strlen(buf) + 1);
}


void boardSetFrequency(int frequency)
{
    boardFreq = frequency * (boardFrequency() / 3579545);
    
	mixerSetBoardFrequency(frequency);
}

int boardGetRefreshRate()
{
    if (boardRunning) {
        return boardInfo.getRefreshRate();
    }
    return 0;
}

void  boardSetInt(UInt32 irq)
{
    pendingInt |= irq;
    boardInfo.setInt(boardInfo.cpuRef);
}

void   boardClearInt(UInt32 irq)
{
    pendingInt &= ~irq;
    if (pendingInt == 0) {
        boardInfo.clearInt(boardInfo.cpuRef);
    }
}

UInt32 boardGetInt(UInt32 irq)
{
    return pendingInt & irq;
}

UInt8* boardGetRamPage(int page)
{
    if (boardInfo.getRamPage == NULL) {
        return emptyRam;
    }
    return boardInfo.getRamPage(page);
}

UInt32 boardGetRamSize()
{
    return boardRamSize;
}

UInt32 boardGetVramSize()
{
    return boardVramSize;
}

int boardUseRom()
{
    return useRom;
}

int boardUseMegaRom()
{
    return useMegaRom;
}

int boardUseMegaRam()
{
    return useMegaRam;
}

int boardUseFmPac()
{
    return useFmPac;
}

HdType boardGetHdType(int hdIndex)
{
    if (hdIndex < 0 || hdIndex >= MAX_HD_COUNT) {
        return HD_NONE;
    }
    return hdType[hdIndex];
}

void boardChangeCartridge(int cartNo, RomType romType, char* cart, char* cartZip)
{
    if (cart && strlen(cart) == 0) {
        cart = NULL;
    }

    if (cartZip && strlen(cartZip) == 0) {
        cartZip = NULL;
    }

    if (boardDeviceInfo != NULL) {
        boardDeviceInfo->carts[cartNo].inserted = cart != NULL;
        boardDeviceInfo->carts[cartNo].type = romType;

        strcpy(boardDeviceInfo->carts[cartNo].name, cart ? cart : "");
        strcpy(boardDeviceInfo->carts[cartNo].inZipName, cartZip ? cartZip : "");
    }

    useRom     -= romTypeIsRom(currentRomType[cartNo]);
    useMegaRom -= romTypeIsMegaRom(currentRomType[cartNo]);
    useMegaRam -= romTypeIsMegaRam(currentRomType[cartNo]);
    useFmPac   -= romTypeIsFmPac(currentRomType[cartNo]);
    hdType[cartNo] = HD_NONE;
    currentRomType[cartNo] = ROM_UNKNOWN;

    if (cart != NULL) {
        currentRomType[cartNo] = romType;
        useRom     += romTypeIsRom(romType);
        useMegaRom += romTypeIsMegaRom(romType);
        useMegaRam += romTypeIsMegaRam(romType);
        useFmPac   += romTypeIsFmPac(romType);
        if (currentRomType[cartNo] == ROM_SUNRISEIDE) hdType[cartNo] = HD_SUNRISEIDE;
        if (currentRomType[cartNo] == ROM_BEERIDE)    hdType[cartNo] = HD_BEERIDE;
        if (currentRomType[cartNo] == ROM_GIDE)       hdType[cartNo] = HD_GIDE;
    }

    if (boardRunning) {
        cartridgeInsert(cartNo, romType, cart, cartZip);
    }
}

void boardChangeDiskette(int driveId, char* fileName, const char* fileInZipFile)
{
    if (fileName && strlen(fileName) == 0) {
        fileName = NULL;
    }

    if (fileInZipFile && strlen(fileInZipFile) == 0) {
        fileInZipFile = NULL;
    }

    if (boardDeviceInfo != NULL) {
        boardDeviceInfo->disks[driveId].inserted = fileName != NULL;

        strcpy(boardDeviceInfo->disks[driveId].name, fileName ? fileName : "");
        strcpy(boardDeviceInfo->disks[driveId].inZipName, fileInZipFile ? fileInZipFile : "");
    }

    diskChange(driveId ,fileName, fileInZipFile);
}

void boardChangeCassette(int tapeId, char* name, const char* fileInZipFile)
{
    if (name && strlen(name) == 0) {
        name = NULL;
    }

    if (fileInZipFile && strlen(fileInZipFile) == 0) {
        fileInZipFile = NULL;
    }

    if (boardDeviceInfo != NULL) {
        boardDeviceInfo->tapes[tapeId].inserted = name != NULL;

        strcpy(boardDeviceInfo->tapes[tapeId].name, name ? name : "");
        strcpy(boardDeviceInfo->tapes[tapeId].inZipName, fileInZipFile ? fileInZipFile : "");
    }

    tapeInsert(name, fileInZipFile);
}

int boardGetCassetteInserted()
{
    return tapeIsInserted();
}

#define HIRES_CYCLES_PER_LORES_CYCLE (UInt64)100000
#define boardFrequency64() (HIRES_CYCLES_PER_LORES_CYCLE * boardFrequency())

UInt32 boardCalcRelativeTimeout(UInt32 timerFrequency, UInt32 nextTimeout)
{
    UInt64 currentTime = boardSystemTime64();
    UInt64 frequency   = boardFrequency64() / timerFrequency;

    currentTime = frequency * (currentTime / frequency);

    return (UInt32)((currentTime + nextTimeout * frequency) / HIRES_CYCLES_PER_LORES_CYCLE);
}

/////////////////////////////////////////////////////////////
// Board timer

typedef struct BoardTimer {
    BoardTimer*  next;
    BoardTimer*  prev;
    BoardTimerCb callback;
    void*        ref;
    UInt32       timeout;
};

static BoardTimer* timerList = NULL;
static UInt32 timeAnchor;

#define MAX_TIME  (2 * 1368 * 313)
#define TEST_TIME 0x7fffffff

BoardTimer* boardTimerCreate(BoardTimerCb callback, void* ref)
{
    BoardTimer* timer = malloc(sizeof(BoardTimer));

    timer->next     = timer;
    timer->prev     = timer;
    timer->callback = callback;
    timer->ref      = ref ? ref : timer;
    timer->timeout  = 0;

    return timer;
}

void boardTimerDestroy(BoardTimer* timer)
{
    boardTimerRemove(timer);

    free(timer);
}

void boardTimerAdd(BoardTimer* timer, UInt32 timeout)
{
    UInt32 currentTime = boardSystemTime();
    BoardTimer* refTimer;
    BoardTimer* next = timer->next;
    BoardTimer* prev = timer->prev;

    // Remove current timer
    next->prev = prev;
    prev->next = next;

    timerList->timeout = currentTime + TEST_TIME;

    refTimer = timerList->next;

    if (timeout - timeAnchor - TEST_TIME < currentTime - timeAnchor - TEST_TIME) {
        timer->next = timer;
        timer->prev = timer;
        
        // Time has already expired
        return;
    }

    while (timeout - timeAnchor > refTimer->timeout - timeAnchor) {
        refTimer = refTimer->next;
    }

    timer->timeout       = timeout;
    timer->next          = refTimer;
    timer->prev          = refTimer->prev;
    refTimer->prev->next = timer;
    refTimer->prev       = timer;

    boardInfo.setCpuTimeout(boardInfo.cpuRef, timerList->next->timeout);
}

void boardTimerRemove(BoardTimer* timer)
{
    BoardTimer* next = timer->next;
    BoardTimer* prev = timer->prev;

    next->prev = prev;
    prev->next = next;

    timer->next = timer;
    timer->prev = timer;
}

UInt32 boardTimerCheckTimeout(void* dummy)
{
    UInt32 currentTime = boardSystemTime();
    timerList->timeout = currentTime + MAX_TIME;

    for (;;) {
        BoardTimer* timer = timerList->next;
        if (timer == timerList) {
            return currentTime + 1000;
        }
        if (timer->timeout - timeAnchor > currentTime - timeAnchor) {
            break;
        }

        boardTimerRemove(timer);
        timer->callback(timer->ref, timer->timeout);
    }

    timeAnchor = currentTime;    

    boardInfo.setCpuTimeout(boardInfo.cpuRef, timerList->next->timeout);

    return timerList->next->timeout - currentTime;
}

UInt64 boardSystemTime64() {
    UInt32 currentTime = boardSystemTime();
    boardSysTime64 += HIRES_CYCLES_PER_LORES_CYCLE * (currentTime - oldTime);
    oldTime = currentTime;
    return boardSysTime64;
}

void boardInit(UInt32* systemTime)
{
    boardSysTime = systemTime;
    oldTime = *systemTime;
    boardSysTime64 = oldTime * HIRES_CYCLES_PER_LORES_CYCLE;

    timeAnchor = *systemTime;
    if (timerList != NULL) {
        for (;;) {
            BoardTimer* timer = timerList->next;
            if (timer == timerList) {
                break;
            }
            boardTimerRemove(timer);
        }

        free(timerList);
    }
    timerList = boardTimerCreate(NULL, NULL);
}


/////////////////////////////////////////////////////////////
// Not board specific stuff....

static char baseDirectory[512];
static int oversamplingYM2413    = 1;
static int oversamplingY8950     = 1;
static int oversamplingMoonsound = 1;
static int enableYM2413          = 1;
static int enableY8950           = 1;
static int enableMoonsound       = 1;
static int videoAutodetect       = 1;

char* boardGetBaseDirectory() {
    return baseDirectory;
}

void boardSetDirectory(char* dir) {
    strcpy(baseDirectory, dir);
}

void boardSetYm2413Oversampling(int value) {
    oversamplingYM2413 = value;
}

int boardGetYm2413Oversampling() {
    return oversamplingYM2413;
}

void boardSetY8950Oversampling(int value) {
    oversamplingY8950 = value;
}

int boardGetY8950Oversampling() {
    return oversamplingY8950;
}

void boardSetMoonsoundOversampling(int value) {
    oversamplingMoonsound = value;
}

int boardGetMoonsoundOversampling() {
    return oversamplingMoonsound;
}

void boardSetYm2413Enable(int value) {
    enableYM2413 = value;
}

int boardGetYm2413Enable() {
    return enableYM2413;
}

void boardSetY8950Enable(int value) {
    enableY8950 = value;
}

int boardGetY8950Enable() {
    return enableY8950;
}

void boardSetMoonsoundEnable(int value) {
    enableMoonsound = value;
}

int boardGetMoonsoundEnable() {
    return enableMoonsound;
}

void boardSetVideoAutodetect(int value) {
    videoAutodetect = value;
}

int  boardGetVideoAutodetect() {
    return videoAutodetect;
}

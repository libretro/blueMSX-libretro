/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Emulator/Emulator.c,v $
**
** $Revision: 1.54 $
**
** $Date: 2006/06/19 18:20:39 $
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
#include "Emulator.h"
#include "MsxTypes.h"
#include "Debugger.h"
#include "Board.h"
#include "FileHistory.h"
#include "Switches.h"
#include "Led.h"
#include "Machine.h"
#include "InputEvent.h"

#include "ArchThread.h"
#include "ArchEvent.h"
#include "ArchTimer.h"
#include "ArchSound.h"
#include "ArchMidi.h"

#include "JoystickPort.h"
#include "ArchInput.h"
#include "ArchDialog.h"
#include "ArchNotifications.h"
#include <math.h>
#include <string.h>

static int WaitForSync(int maxSpeed, int breakpointHit);

static void*  emuThread;
static void*  emuSyncEvent;
static void*  emuStartEvent;
static void*  emuTimer;
static int    emuExitFlag;
static UInt32 emuSysTime = 0;
static UInt32 emuFrequency = 3579545;
int           emuMaxSpeed = 0;
int           emuMaxEmuSpeed = 0; // Max speed issued by emulation
static char   emuStateName[512];
static volatile int      emuSuspendFlag;
static volatile EmuState emuState = EMU_STOPPED;
static volatile int      emuSingleStep = 0;
static Properties* properties;
static Mixer* mixer;
static BoardDeviceInfo deviceInfo;
static Machine* machine;
static int lastScreenMode;

static int emuFrameskipCounter = 0;

static UInt32 emuTimeIdle       = 0;
static UInt32 emuTimeTotal      = 1;
static UInt32 emuTimeOverflow   = 0;
static UInt32 emuUsageCurrent   = 0;
static UInt32 emuCpuSpeed       = 0;
static UInt32 emuCpuUsage       = 0;
static int    enableSynchronousUpdate = 1;

#if 0

#define LOG_SIZE (10 * 1000000)
UInt32 logentry[LOG_SIZE];

int logindex;
int logwrapped;

void dolog(int slot, int sslot, int wr, UInt16 addr, UInt8 val)
{
    logentry[logindex++] = (slot << 26) | (sslot << 24) | ((UInt32)val << 16) | addr | (wr ? (1 << 31) : 0);
    if (logindex == LOG_SIZE) {
        logindex = 0;
        logwrapped++;
    }
}

void clearlog()
{
    logwrapped = 0;
    logindex = 0;
}

void savelog()
{
    int totalSize = LOG_SIZE;
    int lastPct = -1;
    int cnt = 0;
    FILE * f = fopen("bluemsxlog.txt", "w+");
    int i = 0;
    if (logwrapped == 0 && logindex == 0) {
        return;
    }

    if (logwrapped) {
        i = logindex;
    }
    else {
        totalSize = logindex;
    }

    printf("Saving log for slot 1\n");

    do {
        UInt32 v = logentry[i];
        int newPct = ++cnt * 100 / totalSize;
        char rw = (v >> 31) ? 'W' : 'R';

        if (newPct != lastPct) {
            printf("\r%d%%",newPct);
            lastPct = newPct;
        }
        fprintf(f, "%c(%d:%d) %.4x: %.2x\n", rw, (v>>26)&3, (v>>24)&3,v & 0xffff, (v >> 16) & 0xff);
        
        if (++i == LOG_SIZE) {
            i = 0;
        }
    } while (i != logindex);
    printf("\n");
    fclose(f);
}
#else 
#define clearlog()
#define savelog()
#endif

static void emuCalcCpuUsage() {
    static UInt32 oldSysTime = 0;
    static UInt32 oldAverage = 0;
    static UInt32 cnt = 0;
    UInt32 newSysTime;
    UInt32 emuTimeAverage;

    if (emuTimeTotal < 10) {
        return;
    }
    newSysTime = archGetSystemUpTime(1000);
    emuTimeAverage = 100 * (emuTimeTotal - emuTimeIdle) / (emuTimeTotal / 10);
    
    emuTimeOverflow = emuTimeAverage > 940;

    if ((cnt++ & 0x1f) == 0) {
        UInt32 usageAverage = emuUsageCurrent * 100 / (newSysTime - oldSysTime) * emuFrequency / 3579545;
        if (usageAverage > 98 && usageAverage < 102) {
            usageAverage = 100;
        }

        if (usageAverage >= 10000) {
            usageAverage = 0;
        }

        emuCpuSpeed = usageAverage;
        emuCpuUsage = emuTimeAverage;
    }

    oldSysTime      = newSysTime;
    emuUsageCurrent = 0;
    emuTimeIdle     = 0;
    emuTimeTotal    = 1;
}

static int emuUseSynchronousUpdate()
{
    if (properties->emulation.speed == 50 &&
        enableSynchronousUpdate &&
        emulatorGetMaxSpeed() == 0)
    {
        return properties->emulation.syncMethod;
    }
    return P_EMU_SYNCAUTO;
}

UInt32 emulatorGetCpuSpeed() {
    return emuCpuSpeed;
}

UInt32 emulatorGetCpuUsage() {
    return emuCpuUsage;
}

void emuEnableSynchronousUpdate(int enable)
{
    enableSynchronousUpdate = enable;
}

void emulatorInit(Properties* theProperties, Mixer* theMixer)
{
    properties = theProperties;
    mixer      = theMixer;
}

void emulatorExit()
{
    properties = NULL;
    mixer      = NULL;
}


EmuState emulatorGetState() {
    return emuState;
}

void emulatorSetState(EmuState state) {
    if (state == EMU_RUNNING) {
        archSoundResume();
        archMidiEnable(1);
    }
    else {
        archSoundSuspend();
        archMidiEnable(0);
    }
    if (state == EMU_STEP) {
        state = EMU_RUNNING;
        emuSingleStep = 1;
    }
    emuState = state;
}

void emulatorRunOne() {
    archEventSet(emuSyncEvent);
}

int emulatorGetSyncPeriod() {
#ifdef NO_HIRES_TIMERS
    return 10;
#else
    return properties->emulation.syncMethod == P_EMU_SYNCAUTO ||
           properties->emulation.syncMethod == P_EMU_SYNCNONE ? 2 : 1;
#endif
}

void timerCallback(void* timer) {
    if (properties != NULL) {
        static UInt32 frameCount = 0;
        static UInt32 emuCount = 0;
        static UInt32 kbdCount = 0;
        static UInt32 oldSysTime = 0;
        static UInt32 refreshRate = 50;
        UInt32 framePeriod = (properties->video.frameSkip + 1) * 1000;
        UInt32 syncPeriod = emulatorGetSyncPeriod();
        UInt32 sysTime = archGetSystemUpTime(1000);
        UInt32 diffTime = sysTime - oldSysTime;
        int syncMethod = emuUseSynchronousUpdate();

        if (diffTime == 0) {
            return;
        }

        oldSysTime = sysTime;

        // Update display
        frameCount += refreshRate * diffTime;
        if (frameCount >= framePeriod) {
            frameCount %= framePeriod;
            if (emuState == EMU_RUNNING) {
                refreshRate = boardGetRefreshRate();

                if (syncMethod == P_EMU_SYNCAUTO || syncMethod == P_EMU_SYNCNONE) {
                    archUpdateEmuDisplay(0);
                }
            }
        }

        if (syncMethod == P_EMU_SYNCTOVBLANKASYNC) {
            archUpdateEmuDisplay(syncMethod);
        }

        // Update emulation
        emulatorRunOne();
    }
}

static void getDeviceInfo(BoardDeviceInfo* deviceInfo) 
{
    int i;

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        strcpy(properties->media.carts[i].fileName, deviceInfo->carts[i].name);
        strcpy(properties->media.carts[i].fileNameInZip, deviceInfo->carts[i].inZipName);
        properties->media.carts[i].type = deviceInfo->carts[i].type;
        updateExtendedRomName(i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_DISKS; i++) {
        strcpy(properties->media.disks[i].fileName, deviceInfo->disks[i].name);
        strcpy(properties->media.disks[i].fileNameInZip, deviceInfo->disks[i].inZipName);
        updateExtendedDiskName(i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_TAPES; i++) {
        strcpy(properties->media.tapes[i].fileName, deviceInfo->tapes[i].name);
        strcpy(properties->media.tapes[i].fileNameInZip, deviceInfo->tapes[i].inZipName);
        updateExtendedCasName(i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip);
    }

    properties->emulation.vdpSyncMode      = deviceInfo->video.vdpSyncMode;

}

static void setDeviceInfo(BoardDeviceInfo* deviceInfo) 
{
    int i;

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        deviceInfo->carts[i].inserted =  strlen(properties->media.carts[i].fileName);
        deviceInfo->carts[i].type = properties->media.carts[i].type;
        strcpy(deviceInfo->carts[i].name, properties->media.carts[i].fileName);
        strcpy(deviceInfo->carts[i].inZipName, properties->media.carts[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_DISKS; i++) {
        deviceInfo->disks[i].inserted =  strlen(properties->media.disks[i].fileName);
        strcpy(deviceInfo->disks[i].name, properties->media.disks[i].fileName);
        strcpy(deviceInfo->disks[i].inZipName, properties->media.disks[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_TAPES; i++) {
        deviceInfo->tapes[i].inserted =  strlen(properties->media.tapes[i].fileName);
        strcpy(deviceInfo->tapes[i].name, properties->media.tapes[i].fileName);
        strcpy(deviceInfo->tapes[i].inZipName, properties->media.tapes[i].fileNameInZip);
    }

    deviceInfo->video.vdpSyncMode = properties->emulation.vdpSyncMode;
}

static int emulationStartFailure = 0;

static void emulatorThread() {
    int frequency;
    int success = 0;

    emulatorSetFrequency(properties->emulation.speed, &frequency);

    switchSetFront(properties->emulation.frontSwitch);
    switchSetPause(properties->emulation.pauseSwitch);
    switchSetAudio(properties->emulation.audioSwitch);

    success = boardRun(machine,
                       &deviceInfo,
                       mixer,
                       *emuStateName ? emuStateName : NULL,
                       frequency, WaitForSync);

    ledSetAll(0);
    emuState = EMU_STOPPED;

    archTimerDestroy(emuTimer);

    if (!success) {
        emulationStartFailure = 1;
    }
    
    archEventSet(emuStartEvent);
}

void emulatorStart(const char* stateName) {
	dbgEnable();

    archEmulationStartNotification();

    emulatorResume();

    emuExitFlag = 0;

    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MOONSOUND, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_YAMAHA_SFG, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXAUDIO, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXMUSIC, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_SCC, 1);
    
    properties->emulation.pauseSwitch = 0;
    switchSetPause(properties->emulation.pauseSwitch);

    machine = machineCreate(properties->emulation.machineName);

    if (machine == NULL) {  
        archShowStartEmuFailDialog();
        archEmulationStopNotification();
        emuState = EMU_STOPPED;
        archEmulationStartFailure();
        return;
    }

    boardSetMachine(machine);

    emuSyncEvent  = archEventCreate(0);
    emuStartEvent = archEventCreate(0);
    emuTimer = archCreateTimer(emulatorGetSyncPeriod(), timerCallback);

    setDeviceInfo(&deviceInfo);

    inputEventReset();

    archSoundResume();
    archMidiEnable(1);

    emuState = EMU_PAUSED;
    emulationStartFailure = 0;
    strcpy(emuStateName, stateName ? stateName : "");

    clearlog();

#ifdef LINUX_TEST
emuState = EMU_RUNNING;
emulatorThread();
return;
#endif

    emuThread = archThreadCreate(emulatorThread, THREAD_PRIO_HIGH);
    
    archEventWait(emuStartEvent, 3000);

    if (emulationStartFailure) {
        archEmulationStopNotification();
        emuState = EMU_STOPPED;
        archEmulationStartFailure();
    }
    if (emuState != EMU_STOPPED) {
        getDeviceInfo(&deviceInfo);

        boardSetYm2413Oversampling(properties->sound.chip.ym2413Oversampling);
        boardSetY8950Oversampling(properties->sound.chip.y8950Oversampling);
        boardSetMoonsoundOversampling(properties->sound.chip.moonsoundOversampling);

        strcpy(properties->emulation.machineName, machine->name);

        debuggerNotifyEmulatorStart();
        
        emuState = EMU_RUNNING;
    }
} 

void emulatorStop() {
    if (emuState == EMU_STOPPED) {
        return;
    }

    debuggerNotifyEmulatorStop();

    emuState = EMU_STOPPED;

    do {
        archThreadSleep(10);
    } while (!emuSuspendFlag);

    emuExitFlag = 1;
    archEventSet(emuSyncEvent);
    archThreadJoin(emuThread, 400);
    archSoundSuspend();
    archMidiEnable(0);
    machineDestroy(machine);
    archThreadDestroy(emuThread);
    archEventDestroy(emuSyncEvent);
    archEventDestroy(emuStartEvent);
    
    // Reset active indicators in mixer
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MOONSOUND, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_YAMAHA_SFG, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXAUDIO, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXMUSIC, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_SCC, 1);

    archEmulationStopNotification();
    
    dbgDisable();
    dbgPrint();
    savelog();
}



void emulatorSetFrequency(int logFrequency, int* frequency) {
    emuFrequency = (int)(3579545 * pow(2.0, (logFrequency - 50) / 15.0515));

    if (frequency != NULL) {
        *frequency  = emuFrequency;
    }

    boardSetFrequency(emuFrequency);
}

void emulatorSuspend() {
    if (emuState == EMU_RUNNING) {
        emuState = EMU_SUSPENDED;
        do {
            archThreadSleep(10);
        } while (!emuSuspendFlag);
        archSoundSuspend();
        archMidiEnable(0);
    }
}

void emulatorResume() {
    emuSysTime = 0;

    if (emuState == EMU_SUSPENDED) {
        archSoundResume();
        archMidiEnable(1);
        emuState = EMU_RUNNING;
        archUpdateEmuDisplay(0);
    }
}

int emulatorGetCurrentScreenMode()
{
    return lastScreenMode;
}

void emulatorRestart() {
    Machine* machine = machineCreate(properties->emulation.machineName);

    emulatorStop();
    if (machine != NULL) {
        boardSetMachine(machine);
        machineDestroy(machine);
    }
}

void emulatorRestartSound() {
    emulatorSuspend();
    archSoundDestroy();
    archSoundCreate(mixer, 44100, properties->sound.bufSize, properties->sound.stereo ? 2 : 1);
    emulatorResume();
}

int emulatorGetCpuOverflow() {
    int overflow = emuTimeOverflow;
    emuTimeOverflow = 0;
    return overflow;
}

void emulatorSetMaxSpeed(int enable) {
    emuMaxSpeed = enable;
}

int  emulatorGetMaxSpeed() {
    return emuMaxSpeed;
}

void emulatorResetMixer() {
    // Reset active indicators in mixer
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MOONSOUND, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_YAMAHA_SFG, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXAUDIO, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXMUSIC, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_SCC, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_PCM, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_IO, 1);
}

int emulatorSyncScreen()
{
    int rv = 0;
    emuFrameskipCounter--;
    if (emuFrameskipCounter < 0) {
        rv = archUpdateEmuDisplay(properties->emulation.syncMethod);
        if (rv) {
            emuFrameskipCounter = properties->video.frameSkip;
        }
    }
    return rv;
}

void RefreshScreen(int screenMode) {

    lastScreenMode = screenMode;

    if (emuUseSynchronousUpdate() == P_EMU_SYNCFRAMES && emuState == EMU_RUNNING) {
        emulatorSyncScreen();
    }
}

static int WaitForSync(int maxSpeed, int breakpointHit) {
    UInt32 li1;
    UInt32 li2;
    static UInt32 tmp = 0;
    static UInt32 cnt = 0;
    UInt32 sysTime;
    UInt32 diffTime;
    UInt32 syncPeriod;
    static int overflowCount = 0;
    static UInt32 kbdPollCnt = 0;

    emuMaxEmuSpeed = maxSpeed;

    syncPeriod = emulatorGetSyncPeriod();
    li1 = archGetHiresTimer();

    emuSuspendFlag = 1;
        
    if (emuSingleStep) {
        debuggerNotifyEmulatorPause();
        emuSingleStep = 0;
        emuState = EMU_PAUSED;
    }

    if (breakpointHit) {
        debuggerNotifyEmulatorPause();
        emuState = EMU_PAUSED;
    }
    
    if (emuState != EMU_RUNNING) {
        archEventSet(emuStartEvent);
        emuSysTime = 0;
    }

    if (((++kbdPollCnt & 0x03) >> 1) == 0) {
       archPollInput();
    }

    if (emuUseSynchronousUpdate() == P_EMU_SYNCTOVBLANK) {
        overflowCount += emulatorSyncScreen() ? 0 : 1;
        while ((!emuExitFlag && emuState != EMU_RUNNING) || overflowCount > 0) {
            archEventWait(emuSyncEvent, -1);
            overflowCount--;
        }
    }
    else {
        do {
            archEventWait(emuSyncEvent, -1);
            if (((emuMaxSpeed || emuMaxEmuSpeed) && !emuExitFlag) || overflowCount > 0) {
                archEventWait(emuSyncEvent, -1);
            }
            overflowCount = 0;
        } while (!emuExitFlag && emuState != EMU_RUNNING);
    }

    emuSuspendFlag = 0;
    li2 = archGetHiresTimer();

    emuTimeIdle  += li2 - li1;
    emuTimeTotal += li2 - tmp;
    tmp = li2;
    
    sysTime = archGetSystemUpTime(1000);
    diffTime = sysTime - emuSysTime;
    emuSysTime = sysTime;

    if (emuSingleStep) {
        diffTime = 0;
    }

    if ((++cnt & 0x0f) == 0) {
        emuCalcCpuUsage(NULL);
    }

    overflowCount = emulatorGetCpuOverflow() ? 1 : 0;
#ifdef NO_HIRES_TIMERS
    if (diffTime > 50U) {
        overflowCount = 1;
        diffTime = 0;
    }
#else
    if (diffTime > 100U) {
        overflowCount = 1;
        diffTime = 0;
    }
#endif
    if (emuMaxSpeed || emuMaxEmuSpeed) {
        diffTime *= 10;
        if (diffTime > 20 * syncPeriod) {
            diffTime =  20 * syncPeriod;
        }
    }

    emuUsageCurrent += diffTime;

    return emuExitFlag ? -1 : diffTime;
}

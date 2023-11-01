/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Emulator/Emulator.c,v $
**
** $Revision: 1.67 $
**
** $Date: 2009-07-18 14:35:59 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
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


static int    emuExitFlag;
static UInt32 emuSysTime = 0;
static UInt32 emuFrequency = 3579545;
int           emuMaxSpeed = 0;
int           emuPlayReverse = 0;
int           emuMaxEmuSpeed = 0; // Max speed issued by emulation
static volatile int      emuSuspendFlag;
static volatile EmuState emuState = EMU_STOPPED;
static volatile int      emuSingleStep = 0;
static Properties* properties;
static Mixer* mixer;
static BoardDeviceInfo deviceInfo;
static Machine* machine;
static int lastScreenMode;

static int    enableSynchronousUpdate = 1;


extern BoardInfo boardInfo;


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
    if (state == EMU_RUNNING)
        archMidiEnable(1);
    else
        archMidiEnable(0);
    if (state == EMU_STEP) {
        state = EMU_RUNNING;
        emuSingleStep = 1;
    }
    if (state == EMU_STEP_BACK) {
        EmuState oldState = state;
        state = EMU_RUNNING;
        if (!boardRewindOne()) {
            state = oldState;
        }
        
    }
    emuState = state;
}


static void getDeviceInfo(BoardDeviceInfo* deviceInfo)
{
    int i;

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        strcpy(properties->media.carts[i].fileName, deviceInfo->carts[i].name);
        strcpy(properties->media.carts[i].fileNameInZip, deviceInfo->carts[i].inZipName);
        // Don't save rom type
        // properties->media.carts[i].type = deviceInfo->carts[i].type;
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

    deviceInfo->video.vdpSyncMode = (VdpSyncMode)properties->emulation.vdpSyncMode;
}




void emulatorStart(const char* stateName) {
   int frequency;
   int success = 0;
   int reversePeriod = 0;
   int reverseBufferCnt = 0;

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


    setDeviceInfo(&deviceInfo);

    inputEventReset();

    archMidiEnable(1);

    emuState = EMU_PAUSED;

    emuState = EMU_RUNNING;

    emulatorSetFrequency(50 , &frequency);

    switchSetFront(properties->emulation.frontSwitch);
    switchSetPause(properties->emulation.pauseSwitch);
    switchSetAudio(properties->emulation.audioSwitch);

    if (properties->emulation.reverseEnable && properties->emulation.reverseMaxTime > 0) {
        reversePeriod = 50;
        reverseBufferCnt = properties->emulation.reverseMaxTime * 1000 / reversePeriod;
    }
    success = boardRun(machine,
                       &deviceInfo,
                       mixer,
                       NULL,
                       frequency,
                       0,
                       0,
                       NULL);
    if (!success) {
        archEmulationStopNotification();
        emuState = EMU_STOPPED;
        archEmulationStartFailure();
    }
}

void emulatorStop() {
    if (emuState == EMU_STOPPED) {
        return;
    }

    debuggerNotifyEmulatorStop();

    emuState = EMU_STOPPED;

    emuExitFlag = 1;

    archMidiEnable(0);
    machineDestroy(machine);

    // Reset active indicators in mixer
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MOONSOUND, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_YAMAHA_SFG, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXAUDIO, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_MSXMUSIC, 1);
    mixerIsChannelTypeActive(mixer, MIXER_CHANNEL_SCC, 1);

    archEmulationStopNotification();

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
        archMidiEnable(0);
    }
}

void emulatorResume() {
    if (emuState == EMU_SUSPENDED) {
        emuSysTime = 0;
        archMidiEnable(1);
        emuState = EMU_RUNNING;
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


void emulatorSetMaxSpeed(int enable) {
}

int  emulatorGetMaxSpeed() {
    return 0;
}

void emulatorPlayReverse(int enable)
{

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


void RefreshScreen(int screenMode) {

    lastScreenMode = screenMode;

}


/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Emulator/Actions.c,v $
**
** $Revision: 1.80 $
**
** $Date: 2008-05-14 12:55:31 $
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
#include "Actions.h"
#include "MsxTypes.h"
#include "Switches.h"
#include "AudioMixer.h"
#include "Board.h"
#include "Casette.h"
#include "Debugger.h"
#include "Disk.h"
#include "FileHistory.h"
#include "LaunchFile.h"
#include "Emulator.h"
#include "InputEvent.h"
#include "VideoManager.h"
#include "VDP.h"

#include "ArchMenu.h"
#include "ArchDialog.h"
#include "ArchFile.h"
#include "ArchNotifications.h"
#include "ArchPrinter.h"
#include "ArchMidi.h"
#include "ArchInput.h"
#include "ArchVideoIn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(_WIN32) && defined(_XBOX)
#include <xtl.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

static struct {
    Properties* properties;
    Video* video;
    Mixer* mixer;
    int mouseLock;
    int windowedSize;
} state;

void actionCartInsert(int cartNo)
{
    RomType romType;
    char* filename;

    emulatorSuspend();
    filename = archFilenameGetOpenRom(state.properties, cartNo, &romType);
    if (filename != NULL) {        
        insertCartridge(state.properties, cartNo, filename, NULL, romType, 0);
    }
    else {
        emulatorResume();
    }
    archUpdateMenu(0);
}

void actionDiskInsert(int diskNo)
{
    char* filename;

    emulatorSuspend();
    filename = archFilenameGetOpenDisk(state.properties, diskNo, 0);
    if (filename != NULL) {        
        insertDiskette(state.properties, diskNo, filename, NULL, 0);
    }
    emulatorResume();
    archUpdateMenu(0);
}

void actionDiskInsertNew(int diskNo)
{
    char* filename;

    emulatorSuspend();
    filename = archFilenameGetOpenDisk(state.properties, diskNo, 1);
    if (filename != NULL) {        
        insertDiskette(state.properties, diskNo, filename, NULL, 0);
    }
    emulatorResume();
    archUpdateMenu(0);
}

void actionDiskInsertDir(int diskNo)
{
    char* filename;

    emulatorSuspend();
    filename = archDirnameGetOpenDisk(state.properties, diskNo);
    if (filename != NULL) {        
        strcpy(state.properties->media.disks[diskNo].directory, filename);
        insertDiskette(state.properties, diskNo, filename, NULL, 0);
    }
    emulatorResume();
    archUpdateMenu(0);
}

void actionDiskRemove(int i) {
    state.properties->media.disks[i].fileName[0] = 0;
    state.properties->media.disks[i].fileNameInZip[0] = 0;
    updateExtendedDiskName(i, state.properties->media.disks[i].fileName, state.properties->media.disks[i].fileNameInZip);
    if (emulatorGetState() != EMU_STOPPED) {
        emulatorSuspend();
        boardChangeDiskette(i, NULL, NULL);
        emulatorResume();
    }
    archUpdateMenu(0);
}

void actionInit(Video* video, Properties* properties, Mixer* mixer)
{
    memset(&state, 0, sizeof(state));

    state.properties = properties;
    state.video      = video;
    state.mixer      = mixer;

    state.windowedSize = properties->video.windowSize != P_VIDEO_SIZEFULLSCREEN ?
                         properties->video.windowSize : P_VIDEO_SIZEX2;
}

void actionQuit() {
    archQuit();
}

void actionCartInsert1() {
    actionCartInsert(0);
}

void actionCartInsert2() {
    actionCartInsert(1);
}

void actionEmuStep() {
    if (emulatorGetState() == EMU_PAUSED) {
        emulatorSetState(EMU_STEP);
    }
}

void actionEmuStepBack() {
    if (emulatorGetState() == EMU_PAUSED) {
        emulatorSetState(EMU_STEP_BACK);
    }
}

void actionEmuTogglePause() {
    if (emulatorGetState() == EMU_STOPPED) {
        emulatorStart(NULL);
    }
    else if (emulatorGetState() == EMU_PAUSED) {
        emulatorSetState(EMU_RUNNING);
        debuggerNotifyEmulatorResume();
    }
    else {  
        emulatorSetState(EMU_PAUSED);
        debuggerNotifyEmulatorPause();
    }
    archUpdateMenu(0);
}

void actionDiskDirInsertA() {
    actionDiskInsertDir(0);
}

void actionDiskDirInsertB() {
    actionDiskInsertDir(1);
}

void actionDiskInsertA() {
    actionDiskInsert(0);
}

void actionDiskInsertB() {
    actionDiskInsert(1);
}

void actionStartPlayReverse()
{
    emulatorPlayReverse(1);
}

void actionStopPlayReverse()
{
    emulatorPlayReverse(0);
}

void actionDiskQuickChange() {
    if (*state.properties->media.disks[0].fileName) {
        if (*state.properties->media.disks[0].fileNameInZip) {
            strcpy(state.properties->media.disks[0].fileNameInZip, fileGetNext(state.properties->media.disks[0].fileNameInZip, state.properties->media.disks[0].fileName));
#ifdef WII
            archDiskQuickChangeNotify(0, state.properties->media.disks[0].fileName, state.properties->media.disks[0].fileNameInZip);
#endif
            boardChangeDiskette(0, state.properties->media.disks[0].fileName, state.properties->media.disks[0].fileNameInZip);
            updateExtendedDiskName(0, state.properties->media.disks[0].fileName, state.properties->media.disks[0].fileNameInZip);
        }
        else {
            strcpy(state.properties->media.disks[0].fileName, fileGetNext(state.properties->media.disks[0].fileName, NULL));
#ifdef WII
            archDiskQuickChangeNotify(0, state.properties->media.disks[0].fileName, state.properties->media.disks[0].fileNameInZip);
#endif
            boardChangeDiskette(0, state.properties->media.disks[0].fileName, NULL);
            updateExtendedDiskName(0, state.properties->media.disks[0].fileName, state.properties->media.disks[0].fileNameInZip);
        }
#ifndef WII
        archDiskQuickChangeNotify();
#endif
    }
    archUpdateMenu(0);
}

void actionEmuResetSoft() {
    archUpdateMenu(0);
    if (emulatorGetState() == EMU_RUNNING) {
        emulatorSuspend();
        boardReset();
        debuggerNotifyEmulatorReset();
        emulatorResume();
    }
    else {
        emulatorStart(NULL);
    }
    archUpdateMenu(0);
}

void actionEmuResetHard() {
    archUpdateMenu(0);
    emulatorStop();
    emulatorStart(NULL);
    archUpdateMenu(0);
}

void actionEmuResetClean() {
    int i;

    emulatorStop();

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        state.properties->media.carts[i].fileName[0] = 0;
        state.properties->media.carts[i].fileNameInZip[0] = 0;
        state.properties->media.carts[i].type = ROM_UNKNOWN;
        updateExtendedRomName(i, state.properties->media.carts[i].fileName, state.properties->media.carts[i].fileNameInZip);
    }
    
    for (i = 0; i < PROP_MAX_DISKS; i++) {
        state.properties->media.disks[i].fileName[0] = 0;
        state.properties->media.disks[i].fileNameInZip[0] = 0;
        updateExtendedDiskName(i, state.properties->media.disks[i].fileName, state.properties->media.disks[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_TAPES; i++) {
        state.properties->media.tapes[i].fileName[0] = 0;
        state.properties->media.tapes[i].fileNameInZip[0] = 0;
        updateExtendedCasName(i, state.properties->media.tapes[i].fileName, state.properties->media.tapes[i].fileNameInZip);
    }

    emulatorStart(NULL);
    archUpdateMenu(0);
}

void actionTapeRemove(int i) {
    state.properties->media.tapes[i].fileName[0] = 0;
    state.properties->media.tapes[i].fileNameInZip[0] = 0;
    if (emulatorGetState() != EMU_STOPPED) {
        emulatorSuspend();
        boardChangeCassette(i, NULL, NULL);
        emulatorResume();
    }
    updateExtendedCasName(0, state.properties->media.tapes[0].fileName, state.properties->media.tapes[0].fileNameInZip);
    archUpdateMenu(0);
}

void actionCartRemove(int i) {
    state.properties->media.carts[i].fileName[0] = 0;
    state.properties->media.carts[i].fileNameInZip[0] = 0;
    state.properties->media.carts[i].type = ROM_UNKNOWN;
    updateExtendedRomName(i, state.properties->media.carts[i].fileName, state.properties->media.carts[i].fileNameInZip);
    if (emulatorGetState() != EMU_STOPPED) {
        if (state.properties->cartridge.autoReset) {
            emulatorStop();
            emulatorStart(NULL);
        }
        else {
            emulatorSuspend();
            boardChangeCartridge(i, ROM_UNKNOWN, NULL, NULL);
            emulatorResume();
        }
    }
    else {
        boardChangeCartridge(i, ROM_UNKNOWN, NULL, NULL);
    }
    archUpdateMenu(0);
}

void actionDiskRemoveA() {
    actionDiskRemove(0);
}

void actionDiskRemoveB() {
    actionDiskRemove(1);
}

void actionCartRemove1() {
    actionCartRemove(0);
}

void actionCartRemove2() {
    actionCartRemove(1);
}

void actionPrinterForceFormFeed()
{
    emulatorSuspend();
    archForceFormFeed();
    emulatorResume();
}

void actionVideoEnableMon1(int value) {
    videoManagerSetActive(videoManagerGetActive() != 0 ? 0 : -1);
    archUpdateMenu(0);
}

void actionVideoEnableMon2(int value) {
    videoManagerSetActive(videoManagerGetActive() != 1 ? 1 : -1);
    archUpdateMenu(0);
}

void actionVideoEnableMon3(int value) {
    videoManagerSetActive(videoManagerGetActive() != 2 ? 2 : -1);
    archUpdateMenu(0);
}

// Actions controlled by value 0 - 100

void actionRenshaSetLevel(int value) {
    state.properties->joy1.autofire = (int)ceil((11 - 1) * value / 100.0);
    switchSetRensha(state.properties->joy1.autofire);
}

void actionSetSpriteEnable(int value) {
    vdpSetSpritesEnable(value);
}

void actionSetNoSpriteLimits(int value) {
	vdpSetNoSpriteLimits(value);
}

void actionSetMsxAudioSwitch(int value) {
    state.properties->emulation.audioSwitch = value ? 1 : 0;
    switchSetAudio(state.properties->emulation.audioSwitch);
}

void actionSetFrontSwitch(int value) {
    state.properties->emulation.frontSwitch = value ? 1 : 0;
    switchSetFront(state.properties->emulation.frontSwitch);
}

void actionSetFdcTiming(int value) {
    state.properties->emulation.enableFdcTiming = value ? 1 : 0;
    boardSetFdcTimingEnable(state.properties->emulation.enableFdcTiming);
}

void actionSetCartAutoReset(int value) {
    state.properties->cartridge.autoReset = value ? 1 : 0;
    archUpdateMenu(0);
}

void actionSetDiskAutoResetA(int value) {
    state.properties->diskdrive.autostartA = value ? 1 : 0;
    archUpdateMenu(0);
}

void actionSetCasReadonly(int value) {
    state.properties->cassette.readOnly = value ? 1 : 0;
    archUpdateMenu(0);
}

void actionSetCasAutoRewind(int value) {
    state.properties->cassette.rewindAfterInsert = value ? 1 : 0;
    archUpdateMenu(0);
}

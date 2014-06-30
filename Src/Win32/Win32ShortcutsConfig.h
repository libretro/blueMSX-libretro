/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32ShortcutsConfig.h,v $
**
** $Revision: 1.15 $
**
** $Date: 2008-03-30 18:38:48 $
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
#ifndef WIN32_SHORTCUTSCONFIG_H
#define WIN32_SHORTCUTSCONFIG_H

#include <windows.h>
#include "Win32Properties.h"


#define HOTKEY_TYPE_NONE     0
#define HOTKEY_TYPE_KEYBOARD 1
#define HOTKEY_TYPE_JOYSTICK 2

typedef struct {
    unsigned type : 8;
    unsigned mods : 8;
    unsigned key  : 16;
} ShotcutHotkey;



typedef struct {
    ShotcutHotkey spritesEnable;
    ShotcutHotkey fdcTiming;
    ShotcutHotkey noSpriteLimits;
    ShotcutHotkey msxKeyboardQuirk;
    ShotcutHotkey msxAudioSwitch;
    ShotcutHotkey frontSwitch;
    ShotcutHotkey pauseSwitch;
    ShotcutHotkey quit;
    ShotcutHotkey wavCapture;
    ShotcutHotkey videoCapLoad;
    ShotcutHotkey videoCapPlay;
    ShotcutHotkey videoCapRec;
    ShotcutHotkey videoCapStop;
    ShotcutHotkey videoCapSave;
    ShotcutHotkey screenCapture;
    ShotcutHotkey screenCaptureUnfilteredSmall;
    ShotcutHotkey screenCaptureUnfilteredLarge;
    ShotcutHotkey cpuStateLoad;
    ShotcutHotkey cpuStateSave;
    ShotcutHotkey cpuStateQuickLoad;
    ShotcutHotkey cpuStateQuickSave;
    ShotcutHotkey cpuStateQuickSaveUndo;

    ShotcutHotkey cartInsert[2];
    ShotcutHotkey cartSpecialMenu[2];
    ShotcutHotkey cartRemove[2];
    ShotcutHotkey cartAutoReset[2];

    ShotcutHotkey diskInsert[2];
    ShotcutHotkey diskDirInsert[2];
    ShotcutHotkey diskChange[2];
    ShotcutHotkey diskRemove[2];
    ShotcutHotkey diskAutoReset[2];

    ShotcutHotkey casInsert;
    ShotcutHotkey casRewind;
    ShotcutHotkey casSetPos;
    ShotcutHotkey casRemove;
    ShotcutHotkey casToggleReadonly;
    ShotcutHotkey casAutoRewind;
    ShotcutHotkey casSave;

    ShotcutHotkey prnFormFeed;
    ShotcutHotkey mouseLockToggle;
    ShotcutHotkey emulationRunPause;
    ShotcutHotkey emulationStop;
    ShotcutHotkey emuSpeedFull;
    ShotcutHotkey emuPlayReverse;
    ShotcutHotkey emuSpeedNormal;
    ShotcutHotkey emuSpeedInc;
    ShotcutHotkey emuSpeedDec;
    ShotcutHotkey emuSpeedToggle;
    ShotcutHotkey windowSizeSmall;
    ShotcutHotkey windowSizeNormal;
    ShotcutHotkey windowSizeMinimized;
    ShotcutHotkey windowSizeFullscreen;
    ShotcutHotkey windowSizeFullscreenToggle;
    ShotcutHotkey resetSoft;
    ShotcutHotkey resetHard;
    ShotcutHotkey resetClean;
    ShotcutHotkey volumeIncrease;
    ShotcutHotkey volumeDecrease;
    ShotcutHotkey volumeMute;
    ShotcutHotkey volumeStereo;
    ShotcutHotkey themeSwitch;
    ShotcutHotkey propShowEmulation;
    ShotcutHotkey propShowVideo;
    ShotcutHotkey propShowAudio;
    ShotcutHotkey propShowEffects;
    ShotcutHotkey propShowSettings;
    ShotcutHotkey propShowApearance;
    ShotcutHotkey propShowPorts;
    ShotcutHotkey optionsShowLanguage;
    ShotcutHotkey toolsShowMachineEditor;
    ShotcutHotkey toolsShowShorcutEditor;
    ShotcutHotkey toolsShowKeyboardEditor;
    ShotcutHotkey toolsShowMixer;
    ShotcutHotkey toolsShowDebugger;
    ShotcutHotkey toolsShowTrainer;
    ShotcutHotkey helpShowHelp;
    ShotcutHotkey helpShowAbout;
} Shortcuts;


Shortcuts* shortcutsCreateProfile(char* profileName);
void shortcutsDestroyProfile(Shortcuts* shortcuts);
int shortcutsShowDialog(HWND hwnd, Properties* pProperties);
char* shortcutsToString(ShotcutHotkey hotkey);
void shortcutsSetDirectory(char* profileDir);
int shortcutsIsProfileValid(char* profileName);
int shortcutsGetAnyProfile(char* profileName);

#endif //WIN32_STATUSBAR_H

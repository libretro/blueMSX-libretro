/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Theme/ThemeLoader.cpp,v $
**
** $Revision: 1.56 $
**
** $Date: 2006/06/16 01:19:18 $
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
#define USE_ARCH_GLOB

#include "tinyxml.h"
#include "ThemeLoader.h"
extern "C" {
#include "InputEvent.h"
#include "StrcmpNoCase.h"
#include "ArchBitmap.h"
#include "ArchText.h"
#include "ArchFile.h"
#ifdef USE_ARCH_GLOB
#include "ArchGlob.h"
#endif
}

#define MAX_CLIP_POINTS 512

struct ThemeDefaultInfo {
    char* mode;
    int width;
    int height;
} themeDefaultInfo[] = {
    { "small",           320, 240 },
    { "normal",          640, 480 },
    { "fullscreen",      640, 480 }
};

enum ThemeInfo { THEME_SMALL = 0, THEME_NORMAL = 1, THEME_FULLSCREEN = 2 };

static ButtonEvent getAction(TiXmlElement* el, const char* actionTag, 
                             const char* arg1Tag, const char* arg2Tag, int* arg1, int* arg2, 
                             ThemeCollection* themeCollection, Theme* theme, int dx, int dy)
{
    ButtonEvent buttonEvent = 0;

    *arg1 = 1;
    *arg2 = 1;

    el->QueryIntAttribute(arg1Tag, arg1);
    el->QueryIntAttribute(arg2Tag, arg2);

    const char* action = el->Attribute(actionTag);
    if (action == NULL) {
        return NULL;
    }

    if (0 == strcmp(action, "switch-togglefdctiming"))  return (ButtonEvent)actionToggleFdcTiming;
    
    if (0 == strcmp(action, "switch-videohstretch"))    return (ButtonEvent)actionToggleHorizontalStretch;
    if (0 == strcmp(action, "switch-videovstretch"))    return (ButtonEvent)actionToggleVerticalStretch;
    if (0 == strcmp(action, "switch-videoscanlines"))   return (ButtonEvent)actionToggleScanlinesEnable;
    if (0 == strcmp(action, "switch-videodeinterlace")) return (ButtonEvent)actionToggleDeinterlaceEnable;
    if (0 == strcmp(action, "switch-videoblendframes")) return (ButtonEvent)actionToggleBlendFrameEnable;
    if (0 == strcmp(action, "switch-videorfmodulation"))return (ButtonEvent)actionToggleRfModulatorEnable;

    if (0 == strcmp(action, "switch-audioswitch"))      return (ButtonEvent)actionToggleMsxAudioSwitch;
    if (0 == strcmp(action, "switch-frontswitch"))      return (ButtonEvent)actionToggleFrontSwitch;
    if (0 == strcmp(action, "switch-pauseswitch"))      return (ButtonEvent)actionTogglePauseSwitch;
    
    if (0 == strcmp(action, "menu-specialcart1"))       { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuSpecialCart1; }
    if (0 == strcmp(action, "menu-specialcart2"))       { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuSpecialCart2; }
    if (0 == strcmp(action, "menu-reset"))              { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuReset; }
    if (0 == strcmp(action, "menu-run"))                { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuRun; }
    if (0 == strcmp(action, "menu-file"))               { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuFile; }
    if (0 == strcmp(action, "menu-cart1"))              { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuCart1; }
    if (0 == strcmp(action, "menu-cart2"))              { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuCart2; }
    if (0 == strcmp(action, "menu-harddisk"))           { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuHarddisk; }
    if (0 == strcmp(action, "menu-diska"))              { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuDiskA; }
    if (0 == strcmp(action, "menu-diskb"))              { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuDiskB; }
    if (0 == strcmp(action, "menu-cassette"))           { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuCassette; }
    if (0 == strcmp(action, "menu-printer"))            { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuPrinter; }
    if (0 == strcmp(action, "menu-joyport1"))           { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuJoyPort1; }   
    if (0 == strcmp(action, "menu-joyport2"))           { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuJoyPort2; } 
    if (0 == strcmp(action, "menu-windowsize"))         { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuZoom; }
    if (0 == strcmp(action, "menu-options"))            { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuOptions; }
    if (0 == strcmp(action, "menu-help"))               { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuHelp; }
    if (0 == strcmp(action, "menu-tools"))              { *arg1 += dx; *arg2 += dy; return (ButtonEvent)actionMenuTools; }
    
    if (0 == strcmp(action, "dlg-emulation"))           return (ButtonEvent)actionPropShowEmulation;
    if (0 == strcmp(action, "dlg-controls"))            return (ButtonEvent)actionPropShowEmulation;
    if (0 == strcmp(action, "dlg-video"))               return (ButtonEvent)actionPropShowVideo;
    if (0 == strcmp(action, "dlg-audio"))               return (ButtonEvent)actionPropShowAudio;
    if (0 == strcmp(action, "dlg-performance"))         return (ButtonEvent)actionPropShowPerformance;
    if (0 == strcmp(action, "dlg-settings"))            return (ButtonEvent)actionPropShowSettings;
    if (0 == strcmp(action, "dlg-apearance"))           return (ButtonEvent)actionPropShowApearance;
    if (0 == strcmp(action, "dlg-language"))            return (ButtonEvent)actionOptionsShowLanguage;
    if (0 == strcmp(action, "dlg-machineeditor"))       return (ButtonEvent)actionToolsShowMachineEditor;
    if (0 == strcmp(action, "dlg-shortcuteditor"))      return (ButtonEvent)actionToolsShowShorcutEditor;
    if (0 == strcmp(action, "dlg-keyboardeditor"))      return (ButtonEvent)actionToolsShowKeyboardEditor;
    if (0 == strcmp(action, "dlg-mixer"))               return (ButtonEvent)actionToolsShowMixer;
    if (0 == strcmp(action, "dlg-help"))                return (ButtonEvent)actionHelpShowHelp;
    if (0 == strcmp(action, "dlg-about"))               return (ButtonEvent)actionHelpShowAbout;
    
    if (0 == strcmp(action, "state-load"))              return (ButtonEvent)actionLoadState;
    if (0 == strcmp(action, "state-save"))              return (ButtonEvent)actionSaveState;
    if (0 == strcmp(action, "state-quickload"))         return (ButtonEvent)actionQuickLoadState;
    if (0 == strcmp(action, "state-quicksave"))         return (ButtonEvent)actionQuickSaveState;

    if (0 == strcmp(action, "cart1-insert"))            return (ButtonEvent)actionCartInsert1;
    if (0 == strcmp(action, "cart1-remove"))            return (ButtonEvent)actionCartRemove1;
    if (0 == strcmp(action, "cart1-toggleautoreset"))   return (ButtonEvent)actionToggleCartAutoReset;
    if (0 == strcmp(action, "cart2-insert"))            return (ButtonEvent)actionCartInsert2;
    if (0 == strcmp(action, "cart2-remove"))            return (ButtonEvent)actionCartRemove2;
    
    if (0 == strcmp(action, "diska-insert"))            return (ButtonEvent)actionDiskInsertA;
    if (0 == strcmp(action, "diska-insertdir"))         return (ButtonEvent)actionDiskDirInsertA;
    if (0 == strcmp(action, "diska-remove"))            return (ButtonEvent)actionDiskRemoveA;
    if (0 == strcmp(action, "diska-quickchange"))       return (ButtonEvent)actionDiskQuickChange;
    if (0 == strcmp(action, "diska-toggleautoreset"))   return (ButtonEvent)actionToggleDiskAutoReset;
    if (0 == strcmp(action, "diskb-insert"))            return (ButtonEvent)actionDiskInsertB;
    if (0 == strcmp(action, "diskb-insertdir"))         return (ButtonEvent)actionDiskDirInsertB;
    if (0 == strcmp(action, "diskb-remove"))            return (ButtonEvent)actionDiskRemoveB;
    
    if (0 == strcmp(action, "cas-insert"))              return (ButtonEvent)actionCasInsert;
    if (0 == strcmp(action, "cas-remove"))              return (ButtonEvent)actionCasRemove;
    if (0 == strcmp(action, "cas-rewind"))              return (ButtonEvent)actionCasRewind;
    if (0 == strcmp(action, "cas-setposition"))         return (ButtonEvent)actionCasSetPosition;
    if (0 == strcmp(action, "cas-togglereadonly"))      return (ButtonEvent)actionCasToggleReadonly;
    if (0 == strcmp(action, "cas-toggleautorewind"))    return (ButtonEvent)actionToggleCasAutoRewind;
    if (0 == strcmp(action, "cas-save"))                return (ButtonEvent)actionCasSave;
    
    if (0 == strcmp(action, "window-small"))            return (ButtonEvent)actionWindowSizeSmall;
    if (0 == strcmp(action, "window-normal"))           return (ButtonEvent)actionWindowSizeNormal;
    if (0 == strcmp(action, "window-minimized"))        return (ButtonEvent)actionWindowSizeMinimized;
    if (0 == strcmp(action, "window-fullscreen"))       return (ButtonEvent)actionWindowSizeFullscreen;
    if (0 == strcmp(action, "window-togglefullscreen")) return (ButtonEvent)actionFullscreenToggle;
    
    if (0 == strcmp(action, "screenshot-normal"))       return (ButtonEvent)actionScreenCapture;
    if (0 == strcmp(action, "screenshot-small"))        return (ButtonEvent)actionScreenCaptureUnfilteredSmall;
    if (0 == strcmp(action, "screenshot-large"))        return (ButtonEvent)actionScreenCaptureUnfilteredLarge;
    
    if (0 == strcmp(action, "emu-quit"))                return (ButtonEvent)actionQuit;
    if (0 == strcmp(action, "emu-resetsoft"))           return (ButtonEvent)actionEmuResetSoft;
    if (0 == strcmp(action, "emu-resethard"))           return (ButtonEvent)actionEmuResetHard;
    if (0 == strcmp(action, "emu-resetclean"))          return (ButtonEvent)actionEmuResetClean;
    if (0 == strcmp(action, "emu-togglepause"))         return (ButtonEvent)actionEmuTogglePause;
    if (0 == strcmp(action, "emu-stop"))                return (ButtonEvent)actionEmuStop;
    if (0 == strcmp(action, "emu-togglemousecapture"))  return (ButtonEvent)actionToggleMouseCapture;
    
    if (0 == strcmp(action, "emu-setmaxspeed"))         return (ButtonEvent)actionMaxSpeedSet;
    if (0 == strcmp(action, "emu-releasemaxspeed"))     return (ButtonEvent)actionMaxSpeedRelease;
    if (0 == strcmp(action, "emu-togglemaxspeed"))      return (ButtonEvent)actionMaxSpeedToggle;
    if (0 == strcmp(action, "emu-normalspeed"))         return (ButtonEvent)actionEmuSpeedNormal;
    if (0 == strcmp(action, "emu-increasespeed"))       return (ButtonEvent)actionEmuSpeedDecrease;
    if (0 == strcmp(action, "emu-decreasespeed"))       return (ButtonEvent)actionEmuSpeedDecrease;
    
    if (0 == strcmp(action, "audio-togglerecord"))      return (ButtonEvent)actionToggleWaveCapture;
    if (0 == strcmp(action, "audio-increasevolume"))    return (ButtonEvent)actionVolumeIncrease;
    if (0 == strcmp(action, "audio-decreasevolume"))    return (ButtonEvent)actionVolumeDecrease;
    if (0 == strcmp(action, "audio-togglestereo"))      return (ButtonEvent)actionVolumeToggleStereo;
    if (0 == strcmp(action, "audio-togglemute"))        return (ButtonEvent)actionMuteToggleMaster;

    if (0 == strcmp(action, "audio-togglemutemaster"))    return (ButtonEvent)actionMuteToggleMaster;
    if (0 == strcmp(action, "audio-togglemutepsg"))       return (ButtonEvent)actionMuteTogglePsg;
    if (0 == strcmp(action, "audio-togglemutepcm"))       return (ButtonEvent)actionMuteTogglePcm;
    if (0 == strcmp(action, "audio-togglemuteio"))        return (ButtonEvent)actionMuteToggleIo;
    if (0 == strcmp(action, "audio-togglemutescc"))       return (ButtonEvent)actionMuteToggleScc;
    if (0 == strcmp(action, "audio-togglemutekeyboard"))  return (ButtonEvent)actionMuteToggleKeyboard;
    if (0 == strcmp(action, "audio-togglemutemsxmusic"))  return (ButtonEvent)actionMuteToggleMsxMusic;
    if (0 == strcmp(action, "audio-togglemutemsxaudio"))  return (ButtonEvent)actionMuteToggleMsxAudio;
    if (0 == strcmp(action, "audio-togglemutemoonsound")) return (ButtonEvent)actionMuteToggleMoonsound;
    if (0 == strcmp(action, "audio-togglemutesfg"))       return (ButtonEvent)actionMuteToggleYamahaSfg;
    if (0 == strcmp(action, "audio-togglemutemidi"))      return (ButtonEvent)actionMuteToggleMidi;
    
    if (0 == strcmp(action, "printer-forceformfeed"))   return (ButtonEvent)actionPrinterForceFormFeed;

    if (0 == strcmp(action, "video-gamma"))        return (ButtonEvent)actionVideoSetGamma;
    if (0 == strcmp(action, "video-brightness"))   return (ButtonEvent)actionVideoSetBrightness;
    if (0 == strcmp(action, "video-contrast"))     return (ButtonEvent)actionVideoSetContrast;
    if (0 == strcmp(action, "video-saturation"))   return (ButtonEvent)actionVideoSetSaturation;
    if (0 == strcmp(action, "video-scanlines"))    return (ButtonEvent)actionVideoSetScanlines;
    if (0 == strcmp(action, "video-rfmodulation")) return (ButtonEvent)actionVideoSetRfModulation;
    if (0 == strcmp(action, "video-colormode"))    return (ButtonEvent)actionVideoSetColorMode;
    if (0 == strcmp(action, "video-filter"))       return (ButtonEvent)actionVideoSetFilter;
    if (0 == strcmp(action, "video-enable-mon1"))  return (ButtonEvent)actionVideoEnableMon1;
    if (0 == strcmp(action, "video-enable-mon2"))  return (ButtonEvent)actionVideoEnableMon2;
    if (0 == strcmp(action, "video-enable-mon3"))  return (ButtonEvent)actionVideoEnableMon3;
    
    if (0 == strcmp(action, "level-master"))      return (ButtonEvent)actionVolumeSetMaster;
    if (0 == strcmp(action, "level-psg"))         return (ButtonEvent)actionVolumeSetPsg;
    if (0 == strcmp(action, "level-pcm"))         return (ButtonEvent)actionVolumeSetPcm;
    if (0 == strcmp(action, "level-io"))          return (ButtonEvent)actionVolumeSetIo;
    if (0 == strcmp(action, "level-io"))          return (ButtonEvent)actionVolumeSetIo;
    if (0 == strcmp(action, "level-scc"))         return (ButtonEvent)actionVolumeSetScc;
    if (0 == strcmp(action, "level-keyboard"))    return (ButtonEvent)actionVolumeSetKeyboard;
    if (0 == strcmp(action, "level-msxmusic"))    return (ButtonEvent)actionVolumeSetMsxMusic;
    if (0 == strcmp(action, "level-msxaudio"))    return (ButtonEvent)actionVolumeSetMsxAudio;
    if (0 == strcmp(action, "level-moonsound"))   return (ButtonEvent)actionVolumeSetMoonsound;
    if (0 == strcmp(action, "level-sfg"))         return (ButtonEvent)actionVolumeSetYamahaSfg;
    if (0 == strcmp(action, "level-midi"))        return (ButtonEvent)actionVolumeSetMidi;
    if (0 == strcmp(action, "pan-psg"))           return (ButtonEvent)actionPanSetPsg;
    if (0 == strcmp(action, "pan-pcm"))           return (ButtonEvent)actionPanSetPcm;
    if (0 == strcmp(action, "pan-io"))            return (ButtonEvent)actionPanSetIo;
    if (0 == strcmp(action, "pan-scc"))           return (ButtonEvent)actionPanSetScc;
    if (0 == strcmp(action, "pan-keyboard"))      return (ButtonEvent)actionPanSetKeyboard;
    if (0 == strcmp(action, "pan-msxmusic"))      return (ButtonEvent)actionPanSetMsxMusic;
    if (0 == strcmp(action, "pan-msxaudio"))      return (ButtonEvent)actionPanSetMsxAudio;
    if (0 == strcmp(action, "pan-moonsound"))     return (ButtonEvent)actionPanSetMoonsound;
    if (0 == strcmp(action, "pan-sfg"))           return (ButtonEvent)actionPanSetYamahaSfg;
    if (0 == strcmp(action, "pan-midi"))          return (ButtonEvent)actionPanSetMidi;

    if (0 == strcmp(action, "slider-rensha"))       return (ButtonEvent)actionRenshaSetLevel;

    if (0 == strcmp(action, "slider-emuspeed"))     return (ButtonEvent)actionEmuSpeedSet;
    
    if (0 == strcmp(action, "cart1-setautoreset"))   return (ButtonEvent)actionSetCartAutoReset;
    if (0 == strcmp(action, "diska-setautoreset"))   return (ButtonEvent)actionSetDiskAutoResetA;
    if (0 == strcmp(action, "cas-setautorewind"))    return (ButtonEvent)actionSetCasAutoRewind;
    if (0 == strcmp(action, "sprite-setenable"))     return (ButtonEvent)actionSetSpriteEnable;
    if (0 == strcmp(action, "switch-setfdctiming"))  return (ButtonEvent)actionSetFdcTiming;
    if (0 == strcmp(action, "switch-setmsxaudio"))   return (ButtonEvent)actionSetMsxAudioSwitch;
    if (0 == strcmp(action, "switch-setfront"))      return (ButtonEvent)actionSetFrontSwitch;
    if (0 == strcmp(action, "switch-setpause"))      return (ButtonEvent)actionSetPauseSwitch;
    if (0 == strcmp(action, "audio-setrecord"))      return (ButtonEvent)actionSetWaveCapture;
    if (0 == strcmp(action, "mouse-setcapture"))     return (ButtonEvent)actionSetMouseCapture;
    if (0 == strcmp(action, "window-setfullscreen")) return (ButtonEvent)actionSetFullscreen;
    if (0 == strcmp(action, "cas-setreadonly"))      return (ButtonEvent)actionSetCasReadonly;
    if (0 == strcmp(action, "audio-setmute"))        return (ButtonEvent)actionSetVolumeMute;
    if (0 == strcmp(action, "audio-setstereo"))      return (ButtonEvent)actionSetVolumeStereo;

    if (0 == strcmp(action, "theme-maximize"))       return (ButtonEvent)actionMaximizeWindow;
    if (0 == strcmp(action, "theme-minimize"))       return (ButtonEvent)actionMinimizeWindow;
    if (0 == strcmp(action, "theme-close"))          return (ButtonEvent)actionCloseWindow;

    if (0 == strcmp(action, "theme-setpage"))        buttonEvent = (ButtonEvent)themeSetPageFromHash;
    if (0 == strcmp(action, "theme-openwindow"))     buttonEvent = (ButtonEvent)themeCollectionOpenWindow;


    if (buttonEvent != NULL) {
        const char* argaStr = el->Attribute(arg1Tag);
        if (argaStr != NULL) {
            const char* argaStr = el->Attribute(arg1Tag);
            *arg1 = 0;
            *arg2 = themeGetNameHash(argaStr);

            if (buttonEvent == (ButtonEvent)themeSetPageFromHash) {
                *arg1 = (int)theme;
            }
            if (buttonEvent == (ButtonEvent)themeCollectionOpenWindow) {
                *arg1 = (int)themeCollection;
            }
        }
    }

	return buttonEvent;
}

static int getKeyCode(TiXmlElement* el, char* triggerName)
{
    const char* keycode = el->Attribute(triggerName);
    if (keycode == NULL) {
        return -1;
    }
    
    return inputEventStringToCode(keycode);
}


static int getIndexedTrigger(TiXmlElement* el, char* triggerName, int idx)
{
    const char* trigger = el->Attribute(triggerName);
    if (trigger == NULL) {
        return THEME_TRIGGER_NONE;
    }

    int inverted = (strlen(trigger) > 4 && 0 == memcmp(trigger, "not ", 4));

    const char* s = inverted ? trigger + 4 : trigger;
    int         t = inverted ? THEME_TRIGGER_NOT : 0;
    
    if (0 == strcmp(s, "key-pressed"))    return t | (THEME_TRIGGER_FIRST_KEY_PRESSED + idx);
    if (0 == strcmp(s, "key-configured")) return t | (THEME_TRIGGER_FIRST_KEY_CONFIG + idx);

    return -1;
}

static int getTrigger(TiXmlElement* el, char* triggerName)
{
    const char* trigger = el->Attribute(triggerName);
    if (trigger == NULL) {
        return THEME_TRIGGER_NONE;
    }

    int inverted = (strlen(trigger) > 4 && 0 == memcmp(trigger, "not ", 4));

    const char* s = inverted ? trigger + 4 : trigger;
    int         t = inverted ? THEME_TRIGGER_NOT : 0;
    
    if (0 == strcmp(s, "emu-stopped"))              return t | THEME_TRIGGER_IMG_STOPPED;
    if (0 == strcmp(s, "emu-paused"))               return t | THEME_TRIGGER_IMG_PAUSED;
    if (0 == strcmp(s, "emu-running"))              return t | THEME_TRIGGER_IMG_RUNNING;
    
    if (0 == strcmp(s, "led-diska"))                return t | THEME_TRIGGER_IMG_DISKA;
    if (0 == strcmp(s, "led-diskb"))                return t | THEME_TRIGGER_IMG_DISKB;
    if (0 == strcmp(s, "led-cassette"))             return t | THEME_TRIGGER_IMG_CAS;
    if (0 == strcmp(s, "led-audioswitch"))          return t | THEME_TRIGGER_IMG_AS;
    if (0 == strcmp(s, "led-frontswitch"))          return t | THEME_TRIGGER_IMG_FS;
    if (0 == strcmp(s, "led-pauseswitch"))          return t | THEME_TRIGGER_IMG_PS;
    if (0 == strcmp(s, "led-caps"))                 return t | THEME_TRIGGER_IMG_CAPS;
    if (0 == strcmp(s, "led-kana"))                 return t | THEME_TRIGGER_IMG_KANA;
    if (0 == strcmp(s, "led-turbor"))               return t | THEME_TRIGGER_IMG_TURBOR;
    if (0 == strcmp(s, "led-pause"))                return t | THEME_TRIGGER_IMG_PAUSE;
    
    if (0 == strcmp(s, "enable-fdctiming"))         return t | THEME_TRIGGER_IMG_FDCTIMING;

    if (0 == strcmp(s, "enable-keyboard"))          return t | THEME_TRIGGER_IMG_KBD;
    if (0 == strcmp(s, "enable-moonsound"))         return t | THEME_TRIGGER_IMG_MOON;
    if (0 == strcmp(s, "enable-sfg"))               return t | THEME_TRIGGER_IMG_SFG;
    if (0 == strcmp(s, "enable-msxaudio"))          return t | THEME_TRIGGER_IMG_MSXA;
    if (0 == strcmp(s, "enable-msxmusic"))          return t | THEME_TRIGGER_IMG_MSXM;
    if (0 == strcmp(s, "enable-psg"))               return t | THEME_TRIGGER_IMG_PSG;
    if (0 == strcmp(s, "enable-scc"))               return t | THEME_TRIGGER_IMG_SCC;
    if (0 == strcmp(s, "enable-pcm"))               return t | THEME_TRIGGER_IMG_PCM;
    if (0 == strcmp(s, "enable-io"))                return t | THEME_TRIGGER_IMG_IO;
    if (0 == strcmp(s, "enable-midi"))              return t | THEME_TRIGGER_IMG_MIDI;
    if (0 == strcmp(s, "enable-master"))            return t | THEME_TRIGGER_IMG_MASTER;
    if (0 == strcmp(s, "enable-stereo"))            return t | THEME_TRIGGER_IMG_STEREO;
    
    if (0 == strcmp(s, "volume-keyboard-left"))     return t | THEME_TRIGGER_IMG_L_KBD;
    if (0 == strcmp(s, "volume-keyboard-right"))    return t | THEME_TRIGGER_IMG_R_KBD;
    if (0 == strcmp(s, "volume-moonsound-left"))    return t | THEME_TRIGGER_IMG_L_MOON;
    if (0 == strcmp(s, "volume-moonsound-right"))   return t | THEME_TRIGGER_IMG_R_MOON;
    if (0 == strcmp(s, "volume-sfg-left"))          return t | THEME_TRIGGER_IMG_L_SFG;
    if (0 == strcmp(s, "volume-sfg-right"))         return t | THEME_TRIGGER_IMG_R_SFG;
    if (0 == strcmp(s, "volume-msxaudio-left"))     return t | THEME_TRIGGER_IMG_L_MSXA;
    if (0 == strcmp(s, "volume-msxaudio-right"))    return t | THEME_TRIGGER_IMG_R_MSXA;
    if (0 == strcmp(s, "volume-msxmusic-left"))     return t | THEME_TRIGGER_IMG_L_MSXM;
    if (0 == strcmp(s, "volume-msxmusic-right"))    return t | THEME_TRIGGER_IMG_R_MSXM;
    if (0 == strcmp(s, "volume-psg-left"))          return t | THEME_TRIGGER_IMG_L_PSG;
    if (0 == strcmp(s, "volume-psg-right"))         return t | THEME_TRIGGER_IMG_R_PSG;
    if (0 == strcmp(s, "volume-scc-left"))          return t | THEME_TRIGGER_IMG_L_SCC;
    if (0 == strcmp(s, "volume-scc-right"))         return t | THEME_TRIGGER_IMG_R_SCC;
    if (0 == strcmp(s, "volume-pcm-left"))          return t | THEME_TRIGGER_IMG_L_PCM;
    if (0 == strcmp(s, "volume-pcm-right"))         return t | THEME_TRIGGER_IMG_R_PCM;
    if (0 == strcmp(s, "volume-io-left"))           return t | THEME_TRIGGER_IMG_L_IO;
    if (0 == strcmp(s, "volume-io-right"))          return t | THEME_TRIGGER_IMG_R_IO;
    if (0 == strcmp(s, "volume-midi-left"))         return t | THEME_TRIGGER_IMG_L_MIDI;
    if (0 == strcmp(s, "volume-midi-right"))        return t | THEME_TRIGGER_IMG_R_MIDI;
    if (0 == strcmp(s, "volume-master-left"))       return t | THEME_TRIGGER_IMG_L_MASTER;
    if (0 == strcmp(s, "volume-master-right"))      return t | THEME_TRIGGER_IMG_R_MASTER;

    if (0 == strcmp(s, "slider-rensha"))            return t | THEME_TRIGGER_RENSHA;
    if (0 == strcmp(s, "led-rensha"))               return t | THEME_TRIGGER_RENSHALED;
    
    if (0 == strcmp(s, "slider-emuspeed"))          return t | THEME_TRIGGER_EMUSPEED;
    \
    if (0 == strcmp(s, "video-gamma"))          return t | THEME_TRIGGER_VIDEO_GAMMA;
    if (0 == strcmp(s, "video-brightness"))     return t | THEME_TRIGGER_VIDEO_BRIGHTNESS;
    if (0 == strcmp(s, "video-contrast"))       return t | THEME_TRIGGER_VIDEO_CONTRAST;
    if (0 == strcmp(s, "video-saturation"))     return t | THEME_TRIGGER_VIDEO_SATURATION;
    if (0 == strcmp(s, "video-scanlines"))      return t | THEME_TRIGGER_VIDEO_SCANLINES;
    if (0 == strcmp(s, "video-rfmodulation"))   return t | THEME_TRIGGER_VIDEO_RFMODULATION;
    if (0 == strcmp(s, "video-colormode"))      return t | THEME_TRIGGER_VIDEO_COLORMODE;
    if (0 == strcmp(s, "video-filter"))         return t | THEME_TRIGGER_VIDEO_FILTER;
    if (0 == strcmp(s, "video-present-mon1"))    return t | THEME_TRIGGER_VIDEO_PRESENT_MON1;
    if (0 == strcmp(s, "video-present-mon2"))    return t | THEME_TRIGGER_VIDEO_PRESENT_MON2;
    if (0 == strcmp(s, "video-present-mon3"))    return t | THEME_TRIGGER_VIDEO_PRESENT_MON3;
    if (0 == strcmp(s, "video-enable-mon1"))    return t | THEME_TRIGGER_VIDEO_ENABLE_MON1;
    if (0 == strcmp(s, "video-enable-mon2"))    return t | THEME_TRIGGER_VIDEO_ENABLE_MON2;
    if (0 == strcmp(s, "video-enable-mon3"))    return t | THEME_TRIGGER_VIDEO_ENABLE_MON3;

    if (0 == strcmp(s, "video-enable-mon-amber")) return t | THEME_TRIGGER_VIDEO_ENABLE_MON_AMBER;
    if (0 == strcmp(s, "video-enable-mon-green")) return t | THEME_TRIGGER_VIDEO_ENABLE_MON_GREEN;
    if (0 == strcmp(s, "video-enable-mon-white")) return t | THEME_TRIGGER_VIDEO_ENABLE_MON_WHITE;
    if (0 == strcmp(s, "video-enable-mon-color")) return t | THEME_TRIGGER_VIDEO_ENABLE_MON_COLOR;
    
    if (0 == strcmp(s, "video-enable-mon-hq2x"))        return t | THEME_TRIGGER_VIDEO_ENABLE_MON_HQ2X;
    if (0 == strcmp(s, "video-enable-mon-scale2x"))     return t | THEME_TRIGGER_VIDEO_ENABLE_MON_SCALE2X;
    if (0 == strcmp(s, "video-enable-mon-compnoise"))   return t | THEME_TRIGGER_VIDEO_ENABLE_MON_COMPNOISE;
    if (0 == strcmp(s, "video-enable-mon-comp"))        return t | THEME_TRIGGER_VIDEO_ENABLE_MON_COMP;
    if (0 == strcmp(s, "video-enable-mon-ycnoise"))     return t | THEME_TRIGGER_VIDEO_ENABLE_MON_YCNOISE;
    if (0 == strcmp(s, "video-enable-mon-yc"))          return t | THEME_TRIGGER_VIDEO_ENABLE_MON_YC;
    if (0 == strcmp(s, "video-enable-mon-monitor"))     return t | THEME_TRIGGER_VIDEO_ENABLE_MON_MONITOR;
    if (0 == strcmp(s, "video-enable-mon-none"))        return t | THEME_TRIGGER_VIDEO_ENABLE_MON_NONE;

    if (0 == strcmp(s, "level-master"))            return t | THEME_TRIGGER_LEVEL_MASTER;
    if (0 == strcmp(s, "level-psg"))               return t | THEME_TRIGGER_LEVEL_PSG;
    if (0 == strcmp(s, "level-pcm"))               return t | THEME_TRIGGER_LEVEL_PCM;
    if (0 == strcmp(s, "level-io"))                return t | THEME_TRIGGER_LEVEL_IO;
    if (0 == strcmp(s, "level-scc"))               return t | THEME_TRIGGER_LEVEL_SCC;
    if (0 == strcmp(s, "level-keyboard"))          return t | THEME_TRIGGER_LEVEL_KEYBOARD;
    if (0 == strcmp(s, "level-msxmusic"))          return t | THEME_TRIGGER_LEVEL_MSXMUSIC;
    if (0 == strcmp(s, "level-msxaudio"))          return t | THEME_TRIGGER_LEVEL_MSXAUDIO;
    if (0 == strcmp(s, "level-moonsound"))         return t | THEME_TRIGGER_LEVEL_MOONSOUND;
    if (0 == strcmp(s, "level-sfg"))               return t | THEME_TRIGGER_LEVEL_SFG;
    if (0 == strcmp(s, "level-midi"))              return t | THEME_TRIGGER_LEVEL_MIDI;
    if (0 == strcmp(s, "pan-psg"))                 return t | THEME_TRIGGER_PAN_PSG;
    if (0 == strcmp(s, "pan-pcm"))                 return t | THEME_TRIGGER_PAN_PCM;
    if (0 == strcmp(s, "pan-io"))                  return t | THEME_TRIGGER_PAN_IO;
    if (0 == strcmp(s, "pan-scc"))                 return t | THEME_TRIGGER_PAN_SCC;
    if (0 == strcmp(s, "pan-keyboard"))            return t | THEME_TRIGGER_PAN_KEYBOARD;
    if (0 == strcmp(s, "pan-midi"))                return t | THEME_TRIGGER_PAN_MIDI;
    if (0 == strcmp(s, "pan-msxmusic"))            return t | THEME_TRIGGER_PAN_MSXMUSIC;
    if (0 == strcmp(s, "pan-msxaudio"))            return t | THEME_TRIGGER_PAN_MSXAUDIO;
    if (0 == strcmp(s, "pan-moonsound"))           return t | THEME_TRIGGER_PAN_MOONSOUND;
    if (0 == strcmp(s, "pan-sfg"))                 return t | THEME_TRIGGER_PAN_SFG;

    if (0 == strcmp(s, "using-moonsound"))          return t | THEME_TRIGGER_IMG_M_MOON;
    if (0 == strcmp(s, "using-sfg"))                return t | THEME_TRIGGER_IMG_M_SFG;
    if (0 == strcmp(s, "using-msxmusic"))           return t | THEME_TRIGGER_IMG_M_MSXM;
    if (0 == strcmp(s, "using-msxaudio"))           return t | THEME_TRIGGER_IMG_M_MSXA;
    if (0 == strcmp(s, "using-scc"))                return t | THEME_TRIGGER_IMG_M_SCC;
    if (0 == strcmp(s, "using-rom"))                return t | THEME_TRIGGER_IMG_M_ROM;
    if (0 == strcmp(s, "using-megaram"))            return t | THEME_TRIGGER_IMG_M_MEGARAM;
    if (0 == strcmp(s, "using-megarom"))            return t | THEME_TRIGGER_IMG_M_MEGAROM;
    if (0 == strcmp(s, "using-fmpac"))              return t | THEME_TRIGGER_IMG_M_FMPAC;

    if (0 == strcmp(s, "status-record"))            return t | THEME_TRIGGER_IMG_REC;
    if (0 == strcmp(s, "status-diskreset"))         return t | THEME_TRIGGER_IMG_DISK_RI;
    if (0 == strcmp(s, "status-cartreset"))         return t | THEME_TRIGGER_IMG_CART_RI;
    if (0 == strcmp(s, "status-casreadonly"))       return t | THEME_TRIGGER_IMG_CAS_RO;
    if (0 == strcmp(s, "status-scanlines"))         return t | THEME_TRIGGER_VIDEO_SCANLINES_EN;
    if (0 == strcmp(s, "status-deinterlace"))       return t | THEME_TRIGGER_VIDEO_DEINTERLACE_EN;
    if (0 == strcmp(s, "status-blendframes"))       return t | THEME_TRIGGER_VIDEO_BLENDFRAMES_EN;
    if (0 == strcmp(s, "status-rfmodulation"))      return t | THEME_TRIGGER_VIDEO_RFMODULATION_EN;
    if (0 == strcmp(s, "status-hstretch"))          return t | THEME_TRIGGER_VIDEO_HSTRETCH_EN;
    if (0 == strcmp(s, "status-vstretch"))          return t | THEME_TRIGGER_VIDEO_VSTRETCH_EN;

    if (0 == strcmp(s, "port1-enable"))          return t | THEME_TRIGGER_JOY1_ENABLE;
    if (0 == strcmp(s, "port1-none"))            return t | THEME_TRIGGER_JOY1_NONE;
    if (0 == strcmp(s, "port1-joystick"))        return t | THEME_TRIGGER_JOY1_JOYSTICK;
    if (0 == strcmp(s, "port1-mouse"))           return t | THEME_TRIGGER_JOY1_MOUSE;
    if (0 == strcmp(s, "port1-tetris2dongle"))   return t | THEME_TRIGGER_JOY1_TETRIS;
    if (0 == strcmp(s, "port1-magickeydongle"))  return t | THEME_TRIGGER_JOY1_MAGICKEY;
    if (0 == strcmp(s, "port1-gunstick"))        return t | THEME_TRIGGER_JOY1_GUNSTICK;
    if (0 == strcmp(s, "port1-asciilaser"))      return t | THEME_TRIGGER_JOY1_ASCIILASER;
    if (0 == strcmp(s, "port1-colecojoystick"))  return t | THEME_TRIGGER_JOY1_COLECOJOY;
    if (0 == strcmp(s, "port2-enable"))          return t | THEME_TRIGGER_JOY2_ENABLE;
    if (0 == strcmp(s, "port2-none"))            return t | THEME_TRIGGER_JOY2_NONE;
    if (0 == strcmp(s, "port2-joystick"))        return t | THEME_TRIGGER_JOY2_JOYSTICK;
    if (0 == strcmp(s, "port2-mouse"))           return t | THEME_TRIGGER_JOY2_MOUSE;
    if (0 == strcmp(s, "port2-tetris2dongle"))   return t | THEME_TRIGGER_JOY2_TETRIS;
    if (0 == strcmp(s, "port2-magickeydongle"))  return t | THEME_TRIGGER_JOY2_MAGICKEY;
    if (0 == strcmp(s, "port2-gunstick"))        return t | THEME_TRIGGER_JOY2_GUNSTICK;
    if (0 == strcmp(s, "port2-asciilaser"))      return t | THEME_TRIGGER_JOY2_ASCIILASER;
    if (0 == strcmp(s, "port2-colecojoystick"))  return t | THEME_TRIGGER_JOY2_COLECOJOY;

    if (0 == strcmp(s, "text-scanlinespct"))        return t | THEME_TRIGGER_TEXT_SCANLINESPCT;
    if (0 == strcmp(s, "text-videogamma"))          return t | THEME_TRIGGER_TEXT_VIDEOGAMMA;
    if (0 == strcmp(s, "text-videobrightness"))     return t | THEME_TRIGGER_TEXT_VIDEOBRIGHTNESS;
    if (0 == strcmp(s, "text-videocontrast"))       return t | THEME_TRIGGER_TEXT_VIDEOCONTRAST;
    if (0 == strcmp(s, "text-videosaturation"))     return t | THEME_TRIGGER_TEXT_VIDEOSATURATION;
    if (0 == strcmp(s, "text-videomonname1"))       return t | THEME_TRIGGER_TEXT_VIDEOMONNAME1;
    if (0 == strcmp(s, "text-videomonname2"))       return t | THEME_TRIGGER_TEXT_VIDEOMONNAME2;
    if (0 == strcmp(s, "text-videomonname3"))       return t | THEME_TRIGGER_TEXT_VIDEOMONNAME3;
    if (0 == strcmp(s, "text-emufrequency"))        return t | THEME_TRIGGER_TEXT_FREQ;
    if (0 == strcmp(s, "text-cpuusage"))            return t | THEME_TRIGGER_TEXT_CPU;
    
    if (0 == strcmp(s, "text-perftimer0"))          return t | THEME_TRIGGER_TEXT_PERFTIMER0;
    if (0 == strcmp(s, "text-perftimer1"))          return t | THEME_TRIGGER_TEXT_PERFTIMER1;
    if (0 == strcmp(s, "text-perftimer2"))          return t | THEME_TRIGGER_TEXT_PERFTIMER2;
    if (0 == strcmp(s, "text-perftimer3"))          return t | THEME_TRIGGER_TEXT_PERFTIMER3;
    if (0 == strcmp(s, "text-perftimer4"))          return t | THEME_TRIGGER_TEXT_PERFTIMER4;

    if (0 == strcmp(s, "text-framespersecond"))     return t | THEME_TRIGGER_TEXT_FPS;
    if (0 == strcmp(s, "text-ramsize"))             return t | THEME_TRIGGER_TEXT_RAM;
    if (0 == strcmp(s, "text-vramsize"))            return t | THEME_TRIGGER_TEXT_VRAM;
    if (0 == strcmp(s, "text-screentype"))          return t | THEME_TRIGGER_TEXT_SCREEN;
    if (0 == strcmp(s, "text-screentypeshort"))     return t | THEME_TRIGGER_TEXT_SCREENSHORT;
    if (0 == strcmp(s, "text-rommapper1"))          return t | THEME_TRIGGER_TEXT_ROMMAPPER1;
    if (0 == strcmp(s, "text-rommapper2"))          return t | THEME_TRIGGER_TEXT_ROMMAPPER2;
    if (0 == strcmp(s, "text-rommapper1short"))     return t | THEME_TRIGGER_TEXT_ROMMAPPER1SHORT;
    if (0 == strcmp(s, "text-rommapper2short"))     return t | THEME_TRIGGER_TEXT_ROMMAPPER2SHORT;
    if (0 == strcmp(s, "text-machinename"))         return t | THEME_TRIGGER_TEXT_MACHINENAME;
    if (0 == strcmp(s, "text-runningname"))         return t | THEME_TRIGGER_TEXT_RUNNAME;
    if (0 == strcmp(s, "text-version"))             return t | THEME_TRIGGER_TEXT_VERSION;
    if (0 == strcmp(s, "text-buildnumber"))         return t | THEME_TRIGGER_TEXT_BUILDNUMBER;
    if (0 == strcmp(s, "text-buildandversion"))     return t | THEME_TRIGGER_TEXT_BUILDANDVER;
    if (0 == strcmp(s, "text-selectedkey"))         return t | THEME_TRIGGER_TEXT_SELECTEDKEY;
    if (0 == strcmp(s, "text-mappedkey"))           return t | THEME_TRIGGER_TEXT_MAPPEDKEY;
    if (0 == strcmp(s, "text-joyport1"))            return t | THEME_TRIGGER_TEXT_JOYPORT1;
    if (0 == strcmp(s, "text-joyport2"))            return t | THEME_TRIGGER_TEXT_JOYPORT2;
    
    if (0 == strcmp(s, "lang-kbd-selkey"))          return t | THEME_TRIGGER_LANG_KBD_SELKEY;
    if (0 == strcmp(s, "lang-kbd-mappedto"))        return t | THEME_TRIGGER_LANG_KBD_MAPPEDTO;
    if (0 == strcmp(s, "lang-kbd-mapscheme"))       return t | THEME_TRIGGER_LANG_KBD_MAPSCHEME;

    return -1;
}

static ArchBitmap* loadBitmap(TiXmlElement* el, int* x, int* y, int* columns)
{
    *x = 0;
    *y = 0;
    *columns = 999;

    el->QueryIntAttribute("srcColumns", columns);
    el->QueryIntAttribute("x", x);
    el->QueryIntAttribute("y", y);

    const char* src = el->Attribute("src");
    if (src == NULL) {
        return NULL;
    }
    
    return archBitmapCreateFromFile(src);
}


static void addImage(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                     TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    themePageAddImage(themePage, activeImageCreate(x, y, cols, bitmap, 1), THEME_TRIGGER_NONE, visible);
}

static void addGrabImage(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                     TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int activenotify = 0;
    el->QueryIntAttribute("activenotify", &activenotify);
    int count = activenotify ? 2 : 1;

    themePageAddGrabImage(themePage, activeGrabImageCreate(x, y, cols, bitmap, count), THEME_TRIGGER_NONE, visible);
}

static void addLed(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                   TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    themePageAddImage(themePage, activeImageCreate(x, y, cols, bitmap, 2), trigger, visible);
}

static void addMeter(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                     TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int max = 1;
    el->QueryIntAttribute("max", &max);

    themePageAddMeter(themePage, activeMeterCreate(x, y, cols, bitmap, max), trigger, visible);
}

static void addSlider(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                      TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int arga, argb;
    SliderEvent action = (SliderEvent)getAction(el, "action", "arga", "argb", &arga, &argb, themeCollection, theme, dx, dy);

    int max = 1;
    el->QueryIntAttribute("max", &max);

    AsDirection direction = AS_BOTH;
    const char* align = el->Attribute("direction");
    if (align != 0 && strcmp(align, "vertical") == 0) {
        direction = AS_VERTICAL;
    }
    if (align != 0 && strcmp(align, "horizontal") == 0) {
        direction = AS_HORIZONTAL;
    }
    
    int sensitivity = 1;
    el->QueryIntAttribute("sensitivity", &sensitivity);

    const char* mode = el->Attribute("mode");
    if (mode != 0 && strcmp(mode, "inverted") == 0) {
        sensitivity *= -1;
    }

    int relArga, relArgb;
    ButtonEvent release = getAction(el, "release", "relarga", "relargb", &relArga, &relArgb, themeCollection, theme, dx, dy);


    themePageAddSlider(themePage, activeSliderCreate(x, y, cols, bitmap, action, max, direction, sensitivity, release, relArga, relArgb), trigger, visible);
}

static void addButton(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                      TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int arga, argb;
    ButtonEvent action = getAction(el, "action", "arga", "argb", &arga, &argb, themeCollection, theme, dx, dy);

    int activeNotify = 0;
    el->QueryIntAttribute("activenotify", &activeNotify);
    activeNotify = activeNotify ? 1 : 0;

    themePageAddButton(themePage, activeButtonCreate(x, y, cols, activeNotify, bitmap, action, arga, argb), trigger, visible, THEME_TRIGGER_NONE);
}

static void addToggleButton(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                            TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int arga, argb;
    ButtonEvent action = getAction(el, "action", "arga", "argb", &arga, &argb, themeCollection, theme, dx, dy);

    int activeNotify = 0;
    el->QueryIntAttribute("activenotify", &activeNotify);
    activeNotify = activeNotify ? 1 : 0;

    themePageAddToggleButton(themePage, activeToggleButtonCreate(x, y, cols, activeNotify, bitmap, action, arga, argb), 
                         trigger, visible, THEME_TRIGGER_NONE);
}

static void addDualButton(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                          TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int arg1x, arg1y;
    ButtonEvent action1 = getAction(el, "action1", "arg1x", "arg1y", &arg1x, &arg1y, themeCollection, theme, dx, dy);
    
    int arg2x, arg2y;
    ButtonEvent action2 = getAction(el, "action2", "arg2x", "arg2y", &arg2x, &arg2y, themeCollection, theme, dx, dy);

    int vertical = 0;
    const char* align = el->Attribute("direction");
    if (align != 0 && strcmp(align, "vertical") == 0) {
        vertical = 1;
    }

    int activeNotify = 0;
    el->QueryIntAttribute("activenotify", &activeNotify);
    activeNotify = activeNotify ? 1 : 0;

    themePageAddDualButton(themePage, activeDualButtonCreate(x, y, cols, activeNotify, bitmap, action1, arg1x, arg1y,
                                                      action2, arg2x, arg2y, vertical), 
                       trigger, visible, THEME_TRIGGER_NONE);
}

static void addKeyButton(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, TiXmlElement* el, ArchBitmap* srcBitmap, 
                         int srcX, int srcY, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x      = -1;
    int y      = -1;
    int width  = -1;
    int height = -1;
    
    el->QueryIntAttribute("x", &x);
    el->QueryIntAttribute("y", &y);
    el->QueryIntAttribute("width", &width);
    el->QueryIntAttribute("height", &height);

    if (x < 0 || y < 0 || width < 0 || height < 0) {
        return;
    }

    int keycode = getKeyCode(el, "code");
    if (keycode < 0) {
        return;
    }

    ArchBitmap* bitmap = archBitmapCreate(6 * width, height);
    int srcWidth = archBitmapGetWidth(srcBitmap) / 6;

    for (int i = 0; i < 6; i++) {
        archBitmapCopy(bitmap, i * width, 0, srcBitmap, x + i * srcWidth, y, width, height);
    }
    
    ThemeTrigger trigger = (ThemeTrigger)(THEME_TRIGGER_FIRST_KEY_CONFIG + keycode);
    ThemeTrigger pressed = (ThemeTrigger)(THEME_TRIGGER_FIRST_KEY_PRESSED + keycode);
    ButtonEvent action   = (ButtonEvent)actionKeyPress;

    themePageAddToggleButton(themePage, activeToggleButtonCreate(srcX + x, srcY + y, 999, 0, bitmap, action, keycode, -1), 
                         trigger, visible, pressed);
}

static void addKeyboard(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                        TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int srcWidth  = archBitmapGetWidth(bitmap) / 6;
    int srcHeight = archBitmapGetHeight(bitmap);
    ArchBitmap* bgBitmap = archBitmapCreate(srcWidth, srcHeight);
    archBitmapCopy(bgBitmap, 0, 0, bitmap, 0, 0, srcWidth, srcHeight);
    themePageAddImage(themePage, activeImageCreate(x, y, cols, bgBitmap, 1), THEME_TRIGGER_NONE, visible);

    TiXmlElement* keyEl;
    for (keyEl = el->FirstChildElement(); keyEl != NULL; keyEl = keyEl->NextSiblingElement()) {
        if (strcmp(keyEl->Value(), "key") == 0) {
            addKeyButton(themeCollection, theme, themePage, keyEl, bitmap, x, y, visible);
        }
    }

    archBitmapDestroy(bitmap);
}


static void addObject(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                      TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x = 0;
    int y = 0;
    int width = -1;
    int height = -1;

    el->QueryIntAttribute("x", &x);
    el->QueryIntAttribute("y", &y);
    el->QueryIntAttribute("width", &width);
    el->QueryIntAttribute("height", &height);

    x += dx;
    y += dy;
    
    const char* id = el->Attribute("id");
    if (id == NULL) {
        return;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    int arg1 = 0;
    if (0 == strcmp(id, "dropdown-themepages")) arg1 = (int)theme;

    themePageAddObject(themePage, activeObjectCreate(x, y, width, height, id, arg1, 0), visible);
}

static void addText(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                    TiXmlElement* el, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x, y, cols;
    ArchBitmap* bitmap = loadBitmap(el, &x, &y, &cols);
    if (bitmap == NULL) {
        return;
    }
    x += dx;
    y += dy;

    ThemeTrigger trigger = (ThemeTrigger)getTrigger(el, "trigger");
    if (trigger == -1) {
        return;
    }

    int startChar = 0;
    el->QueryIntAttribute("startchar", &startChar);

    int charCount = 256;
    el->QueryIntAttribute("charcount", &charCount);

    int width = 10;
    el->QueryIntAttribute("width", &width);
    
    int type = 0;
    const char* typeStr = el->Attribute("type");
    if (typeStr != NULL && 0 == strcmp(typeStr, "native")) {
        type = 1;
    }

    int color = 0xffffff;
    const char* colStr = el->Attribute("fgcolor");
    if (colStr != NULL) {
        sscanf(colStr, "%x", &color);
        color = archRGB((color>>16), ((color>>8)&0xff), (color&0xff));
    }    

    int rightAlign = 0;
    const char* align = el->Attribute("align");
    if (align != 0 && strcmp(align, "right") == 0) {
        rightAlign = 1;
    }

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(el, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }
    
    themePageAddText(themePage, activeTextCreate(x, y, cols, bitmap, startChar, charCount, width, type, color, rightAlign), 
                     trigger, visible);
}

static void addBlock(ThemeCollection* themeCollection, Theme* theme, ThemePage* themePage, 
                     TiXmlElement* root, int dx, int dy, 
                     ThemeTrigger visible = THEME_TRIGGER_NONE)
{
    int x = 0;
    root->QueryIntAttribute("x", &x);
    int y = 0;
    root->QueryIntAttribute("y", &y);
    dx += x;
    dy += y;

    if (visible == THEME_TRIGGER_NONE) {
        visible = (ThemeTrigger)getTrigger(root, "visible");
        if (visible == -1) {
            visible = THEME_TRIGGER_NONE;
        }
    }

    for (TiXmlElement* el = root->FirstChildElement(); el != NULL; el = el->NextSiblingElement()) {
        if (strcmp(el->Value(), "block") == 0) {
            addBlock(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "image") == 0) {
            addImage(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "dragimage") == 0) {
            addGrabImage(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "led") == 0) {
            addLed(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "text") == 0) {
            addText(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "button") == 0) {
            addButton(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "dualbutton") == 0) {
            addDualButton(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "togglebutton") == 0) {
            addToggleButton(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "keyboard") == 0) {
            addKeyboard(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "meter") == 0) {
            addMeter(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "slider") == 0) {
            addSlider(themeCollection, theme, themePage, el, dx, dy, visible);
        }
        if (strcmp(el->Value(), "object") == 0) {
            addObject(themeCollection, theme, themePage, el, dx, dy, visible);
        }
    }
}

static ThemePage* loadThemePage(ThemeCollection* themeCollection, Theme* theme, 
                                TiXmlElement* root, const char* name, 
                                int width, int height, int frame, 
                                int emuWidth, int emuHeight, int fullscreen) 
{
    ThemePage* themePage = NULL;

    TiXmlElement* infoEl;
    ClipPoint clipPointList[MAX_CLIP_POINTS];  
    int clipPointCount = 0;      
    int emuX           = 0;
    int emuY           = 0;
    int menuX          = 0;
    int menuY          = -100;
    int menuWidth      = 357;
    int menuColor      = archRGB(219, 221, 224);
    int menuFocusColor = archRGB(128, 128, 255);
    int menuTextColor  = archRGB(0, 0, 0);
    int color;
    int noFrame        = frame ? 0 : 1;

    // First, get info about emu window and menu
    for (infoEl = root->FirstChildElement(); infoEl != NULL; infoEl = infoEl->NextSiblingElement()) {
        if (strcmp(infoEl->Value(), "emuwindow") == 0) {
            infoEl->QueryIntAttribute("x", &emuX);
            infoEl->QueryIntAttribute("y", &emuY);
        }
        
        if (strcmp(infoEl->Value(), "menu") == 0) {
            if (!fullscreen) {
                infoEl->QueryIntAttribute("x", &menuX);
                infoEl->QueryIntAttribute("y", &menuY);
                infoEl->QueryIntAttribute("width", &menuWidth);
            }
            else {
                menuX = 0;
                menuY = 0;
                menuWidth = emuWidth;
            }
            const char* colStr = infoEl->Attribute("bgcolor");
            if (colStr != NULL) {
                sscanf(colStr, "%x", &color);
                menuColor = archRGB((color>>16), ((color>>8)&0xff), (color&0xff));
            }
            
            colStr = infoEl->Attribute("fgcolor");
            if (colStr != NULL) {
                sscanf(colStr, "%x", &color);
                menuTextColor = archRGB((color>>16), ((color>>8)&0xff), (color&0xff));
            }
            
            colStr = infoEl->Attribute("focuscolor");
            if (colStr != NULL) {
                sscanf(colStr, "%x", &color);
                menuFocusColor = archRGB((color>>16), ((color>>8)&0xff), (color&0xff));
            }
        }

        if (strcmp(infoEl->Value(), "clipregion") == 0) {
            clipPointCount = 0;
            TiXmlElement* ptEl;
            for (ptEl = infoEl->FirstChildElement(); ptEl != NULL; ptEl = ptEl->NextSiblingElement()) {
                if (strcmp(ptEl->Value(), "point") == 0) {
                    int x = -1;
                    int y = -1;
                    ptEl->QueryIntAttribute("x", &x);
                    ptEl->QueryIntAttribute("y", &y);
                    if (x >= 0 && y >= 0 && clipPointCount < MAX_CLIP_POINTS) {
                        ClipPoint clipPoint = { x, y };
                        clipPointList[clipPointCount++] = clipPoint;
                    }
                }
            }
        }
    }

    themePage = themePageCreate(name,
                        width,
                        height,
                        emuX,
                        emuY,
                        emuWidth,
                        emuHeight,
                        menuX, 
                        menuY, 
                        menuWidth,
                        menuColor,
                        menuFocusColor,
                        menuTextColor,
                        noFrame,
                        clipPointCount,
                        clipPointList);

    addBlock(themeCollection, theme, themePage, root, 0, 0);

    return themePage;
}

static int loadThemeWindows(ThemeCollection* themeCollection, TiXmlElement* root)
{
    TiXmlElement* modeEl;
    int count = 0;
    
    for (modeEl = root->FirstChildElement(); modeEl != NULL; modeEl = modeEl->NextSiblingElement()) {
        Theme* theme = NULL;

        if (strcmp(modeEl->Value(), "window") != 0) {
            continue;
        }

        const char* name = modeEl->Attribute("name");
        if (name == NULL) {
            name = "window";
        }

        int frame = 1;
        modeEl->QueryIntAttribute("frame", &frame);

        ThemeHandler themeHandler = TH_NORMAL;
        
        const char* handler = modeEl->Attribute("handler");
        if (handler != NULL) {
            if (0 == strcmp(handler, "keyboard-config")) {
                themeHandler = TH_KBDCONFIG;
            }
        }
        const char* type = modeEl->Attribute("type");
        if (type != NULL && 0 == strcmp(type, "multipage")) {
            TiXmlElement* pageEl;
            for (pageEl = modeEl->FirstChildElement(); pageEl != NULL; pageEl = pageEl->NextSiblingElement()) {
                int width  = 320;
                int height = 200;

                pageEl->QueryIntAttribute("width", &width);
                pageEl->QueryIntAttribute("height", &height);
                
                const char* pageName = pageEl->Attribute("name");
                if (pageName == NULL) {
                    pageName = name;
                }

                if (theme == NULL) {
                    theme = themeCreate(name);
                    themeSetHandler(theme, themeHandler);
                }

                ThemePage* themePage = loadThemePage(themeCollection, theme, pageEl, pageName, width, height, frame, 0, 0, 0);
                themeAddPage(theme, themePage);
            }
        }
        else {
            int   width  = 320;
            int   height = 200;

            modeEl->QueryIntAttribute("width", &width);
            modeEl->QueryIntAttribute("height", &height);
            
            if (theme == NULL) {
                theme = themeCreate(name);
                themeSetHandler(theme, themeHandler);
            }
            ThemePage* themePage = loadThemePage(themeCollection, theme, modeEl, name, width, height, frame, 0, 0, 0);
            themeAddPage(theme, themePage);
        }

        if (theme != NULL) {
            themeCollectionAddWindow(themeCollection, theme);
            count++;
        }
    }

    return count;
}

static Theme* loadMainTheme(ThemeCollection* themeCollection, TiXmlElement* root, ThemeInfo themeInfo) 
{
    Theme* theme = NULL;

    TiXmlElement* modeEl;
    
    for (modeEl = root->FirstChildElement(); modeEl != NULL; modeEl = modeEl->NextSiblingElement()) {
        int fullscreen = themeInfo == THEME_FULLSCREEN;
        int emuWidth  = themeDefaultInfo[themeInfo].width;
        int emuHeight = themeDefaultInfo[themeInfo].height;

        if (strcmp(modeEl->Value(), "theme") != 0) {
            continue;
        }

        const char* mode = modeEl->Attribute("mode");
        if (strcmp(mode, themeDefaultInfo[themeInfo].mode) != 0) {
            continue;
        }

        const char* name = themeDefaultInfo[themeInfo].mode;

        int frame  = 1;
        modeEl->QueryIntAttribute("frame", &frame);

        const char* type = modeEl->Attribute("type");
        if (type != NULL && 0 == strcmp(type, "multipage")) {
            TiXmlElement* pageEl;
            for (pageEl = modeEl->FirstChildElement(); pageEl != NULL; pageEl = pageEl->NextSiblingElement()) {
                int width  = themeDefaultInfo[themeInfo].width;
                int height = themeDefaultInfo[themeInfo].height;

                if (!fullscreen) {
                    pageEl->QueryIntAttribute("width", &width);
                    pageEl->QueryIntAttribute("height", &height);
                }
                
                if (theme == NULL) {
                    theme = themeCreate(name);
                }
                const char* pageName = pageEl->Attribute("name");
                if (pageName == NULL) {
                    pageName = name;
                }

                ThemePage* themePage = loadThemePage(themeCollection, theme, pageEl, pageName, width, height, frame, emuWidth, emuHeight, fullscreen);
                themeAddPage(theme, themePage);
            }
        }
        else {
            // Get width and height of main window
            int   width     = themeDefaultInfo[themeInfo].width;
            int   height    = themeDefaultInfo[themeInfo].height;

            if (!fullscreen) {
                modeEl->QueryIntAttribute("width", &width);
                modeEl->QueryIntAttribute("height", &height);
            }

            if (theme == NULL) {
                theme = themeCreate(name);
            }
            ThemePage* themePage = loadThemePage(themeCollection, theme, modeEl, name, width, height, frame, emuWidth, emuHeight, fullscreen);
            themeAddPage(theme, themePage);
        }
    }

    return theme;
}

extern "C" ThemeCollection* themeLoad(const char* themePath) 
{
    char oldDirName[512];

    strcpy(oldDirName, archGetCurrentDirectory());

    archSetCurrentDirectory(themePath);

    TiXmlDocument doc("Theme.xml");

    doc.LoadFile();

    if (doc.Error()) {
        archSetCurrentDirectory(oldDirName);
        return NULL;
    }

    TiXmlElement* root = doc.RootElement();

    if (root == NULL || strcmp(root->Value(), "bluemsxtheme") != 0) {
        archSetCurrentDirectory(oldDirName);
        return NULL;
    }

    const char* name = root->Attribute( "name" );
    if (name == NULL) {
        const char* themeName = strrchr(themePath, '/');
        if (themeName == NULL) {
            themeName = strrchr(themePath, '\\');
        }
        if (themeName == NULL) {
            themeName = themePath - 1;
        }
        themeName++;
        name = themeName;
    }

    ThemeCollection* themeCollection = themeCollectionCreate();

    strcpy(themeCollection->name, name);

    themeCollection->little          = loadMainTheme(themeCollection, root, THEME_SMALL);
    themeCollection->normal          = loadMainTheme(themeCollection, root, THEME_NORMAL);
    themeCollection->fullscreen      = loadMainTheme(themeCollection, root, THEME_FULLSCREEN);

    int count = loadThemeWindows(themeCollection, root);

    if (count == 0 && themeCollection->little == NULL && themeCollection->normal == NULL &&
        themeCollection->fullscreen == NULL) 
    {
        themeCollectionDestroy(themeCollection);
        themeCollection = NULL;
    }

    archSetCurrentDirectory(oldDirName);
    return themeCollection;
}

static ThemeCollection** currentWin32Theme = NULL;

#ifdef USE_ARCH_GLOB
extern "C" ThemeCollection** createThemeList(ThemeCollection* defaultTheme)
{
    ThemeCollection** themeList = (ThemeCollection**)calloc(1, 128 * sizeof(ThemeCollection*));
    int index = 0;

    // Set default theme
    if (defaultTheme != NULL) {
        themeList[index++] = defaultTheme;
    }

    ArchGlob* glob = archGlob("Themes/*", ARCH_GLOB_DIRS);

    if (glob != NULL) {
        for (int i = 0; i < glob->count; i++) {
            ThemeCollection* themeCollection = themeLoad(glob->pathVector[i]);
            if (themeCollection != NULL) {
                if (themeCollection->little == NULL)          themeCollection->little =          themeList[0]->little;
                if (themeCollection->normal == NULL)          themeCollection->normal =          themeList[0]->normal;
                if (themeCollection->fullscreen == NULL)      themeCollection->fullscreen =      themeList[0]->fullscreen;
                themeList[index++] = themeCollection;
            }
        }
        archGlobFree(glob);
    }
    themeList[index] = NULL;

    currentWin32Theme = themeList;

    return themeList;
}
#else
extern "C" ThemeCollection** createThemeList(ThemeCollection* defaultTheme)
{
    ThemeCollection** themeList = (ThemeCollection**)calloc(1, 128 * sizeof(ThemeCollection*));
    int index = 0;

    // Set default theme
    if (defaultTheme != NULL) {
        themeList[index++] = defaultTheme;
    }

    // Load custom made themes
    WIN32_FIND_DATA wfd;
    HANDLE handle = FindFirstFile("Themes/*", &wfd);

    if (handle != INVALID_HANDLE_VALUE) {
        do {
		    DWORD fa = GetFileAttributes(wfd.cFileName);
            if (fa & FILE_ATTRIBUTE_DIRECTORY) {
                ThemeCollection* themeCollection = themeLoad(wfd.cFileName, NULL);
                if (themeCollection != NULL) {
                    if (themeCollection->little == NULL)          themeCollection->little =          themeList[0]->little;
                    if (themeCollection->normal == NULL)          themeCollection->normal =          themeList[0]->normal;
                    if (themeCollection->fullscreen == NULL)      themeCollection->fullscreen =      themeList[0]->fullscreen;
                    themeList[index++] = themeCollection;
                }
            }
        } while (FindNextFile(handle, &wfd));

    	FindClose(handle);
    }

    themeList[index] = NULL;

    currentWin32Theme = themeList;

    return themeList;
}
#endif

extern "C" ThemeCollection** themeGetAvailable()
{
    return currentWin32Theme;
}

extern "C" int getThemeListIndex(ThemeCollection** themeList, const char* name, int forceMatch)
{
    int index = 0;
    while (*themeList) {
        if (strcmpnocase(name, (*themeList)->name) == 0) {
            return index;
        }
        index++;
        themeList++;
    }

    if (forceMatch) {
        return 0;
    }

    return -1;
}

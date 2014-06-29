/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Theme/ThemeTriggers.h,v $
**
** $Revision: 1.26 $
**
** $Date: 2008-03-30 18:38:45 $
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
#ifndef THEME_TRIGGERS_H
#define THEME_TRIGGERS_H

typedef enum {
    THEME_TRIGGER_NONE,
    THEME_TRIGGER_IMG_STOPPED,
    THEME_TRIGGER_IMG_PAUSED,
    THEME_TRIGGER_IMG_RUNNING,
    THEME_TRIGGER_IMG_DISKA,
    THEME_TRIGGER_IMG_DISKB,
    THEME_TRIGGER_IMG_CAS,
    THEME_TRIGGER_IMG_HD,
    THEME_TRIGGER_IMG_AS,
    THEME_TRIGGER_IMG_FS,
    THEME_TRIGGER_IMG_PS,
    THEME_TRIGGER_IMG_CAPS,
    THEME_TRIGGER_IMG_KANA,
	THEME_TRIGGER_IMG_TURBOR,
	THEME_TRIGGER_IMG_PAUSE,
    THEME_TRIGGER_IMG_FDCTIMING,
    THEME_TRIGGER_IMG_NOSPRITELIMITS,
    THEME_TRIGGER_IMG_KBD,
    THEME_TRIGGER_IMG_MOON,
    THEME_TRIGGER_IMG_SFG,
    THEME_TRIGGER_IMG_MSXA,
    THEME_TRIGGER_IMG_MSXM,
    THEME_TRIGGER_IMG_MASTER,
    THEME_TRIGGER_IMG_STEREO,
    THEME_TRIGGER_IMG_PSG,
    THEME_TRIGGER_IMG_SCC,
    THEME_TRIGGER_IMG_PCM,
    THEME_TRIGGER_IMG_IO,
    THEME_TRIGGER_IMG_MIDI,
    THEME_TRIGGER_IMG_L_KBD,
    THEME_TRIGGER_IMG_R_KBD,
    THEME_TRIGGER_IMG_L_MOON,
    THEME_TRIGGER_IMG_R_MOON,
    THEME_TRIGGER_IMG_L_SFG,
    THEME_TRIGGER_IMG_R_SFG,
    THEME_TRIGGER_IMG_L_MSXA,
    THEME_TRIGGER_IMG_R_MSXA,
    THEME_TRIGGER_IMG_L_MSXM,
    THEME_TRIGGER_IMG_R_MSXM,
    THEME_TRIGGER_IMG_L_PSG,
    THEME_TRIGGER_IMG_R_PSG,
    THEME_TRIGGER_IMG_L_SCC,
    THEME_TRIGGER_IMG_R_SCC,
    THEME_TRIGGER_IMG_L_PCM,
    THEME_TRIGGER_IMG_R_PCM,
    THEME_TRIGGER_IMG_L_IO,
    THEME_TRIGGER_IMG_R_IO,
    THEME_TRIGGER_IMG_L_MIDI,
    THEME_TRIGGER_IMG_R_MIDI,
    THEME_TRIGGER_IMG_L_MASTER,
    THEME_TRIGGER_IMG_R_MASTER,
    THEME_TRIGGER_IMG_M_MOON,
    THEME_TRIGGER_IMG_M_SFG,
    THEME_TRIGGER_IMG_M_MSXM,
    THEME_TRIGGER_IMG_M_MSXA,
    THEME_TRIGGER_IMG_M_SCC,
    THEME_TRIGGER_IMG_M_ROM,
    THEME_TRIGGER_IMG_M_MEGARAM,
    THEME_TRIGGER_IMG_M_MEGAROM,
    THEME_TRIGGER_IMG_M_FMPAC,
    THEME_TRIGGER_IMG_REC,
    THEME_TRIGGER_IMG_DISK_RI,
    THEME_TRIGGER_IMG_CART_RI,
    THEME_TRIGGER_IMG_CAS_RO,

    THEME_TRIGGER_VIDEO_HSTRETCH_EN,
    THEME_TRIGGER_VIDEO_VSTRETCH_EN,
    THEME_TRIGGER_VIDEO_SCANLINES_EN,
    THEME_TRIGGER_VIDEO_DEINTERLACE_EN,
    THEME_TRIGGER_VIDEO_BLENDFRAMES_EN,
    THEME_TRIGGER_VIDEO_RFMODULATION_EN,
    THEME_TRIGGER_VIDEO_CAPTURE_NONE,
    THEME_TRIGGER_VIDEO_CAPTURE_PLAY,
    THEME_TRIGGER_VIDEO_CAPTURE_REC,
    
    THEME_TRIGGER_KEYBOARD_ENABLE,
    THEME_TRIGGER_JOY1_ENABLE,
    THEME_TRIGGER_JOY1_NONE,
    THEME_TRIGGER_JOY1_JOYSTICK,
    THEME_TRIGGER_JOY1_MOUSE,
    THEME_TRIGGER_JOY1_ARK_PAD,
    THEME_TRIGGER_JOY1_TETRIS,
    THEME_TRIGGER_JOY1_MAGICKEY,
    THEME_TRIGGER_JOY1_GUNSTICK,
    THEME_TRIGGER_JOY1_ASCIILASER,
    THEME_TRIGGER_JOY1_COLECOJOY,
    THEME_TRIGGER_JOY1_SUPERACTION,
    THEME_TRIGGER_JOY1_STEERINGWHEEL,
    THEME_TRIGGER_JOY2_ENABLE,
    THEME_TRIGGER_JOY2_NONE,
    THEME_TRIGGER_JOY2_JOYSTICK,
    THEME_TRIGGER_JOY2_MOUSE,
    THEME_TRIGGER_JOY2_ARK_PAD,
    THEME_TRIGGER_JOY2_TETRIS,
    THEME_TRIGGER_JOY2_MAGICKEY,
    THEME_TRIGGER_JOY2_GUNSTICK,
    THEME_TRIGGER_JOY2_ASCIILASER,
    THEME_TRIGGER_JOY2_COLECOJOY,
    THEME_TRIGGER_JOY2_SUPERACTION,
    THEME_TRIGGER_JOY2_STEERINGWHEEL,

    THEME_TRIGGER_TEXT_SCANLINESPCT,
    THEME_TRIGGER_TEXT_VIDEOGAMMA,
    THEME_TRIGGER_TEXT_VIDEOBRIGHTNESS,
    THEME_TRIGGER_TEXT_VIDEOCONTRAST,
    THEME_TRIGGER_TEXT_VIDEOSATURATION,
    THEME_TRIGGER_TEXT_VIDEOMONNAME1,
    THEME_TRIGGER_TEXT_VIDEOMONNAME2,
    THEME_TRIGGER_TEXT_VIDEOMONNAME3,
    THEME_TRIGGER_TEXT_FREQ,
    THEME_TRIGGER_TEXT_CPU,
    THEME_TRIGGER_TEXT_PERFTIMER0,
    THEME_TRIGGER_TEXT_PERFTIMER1,
    THEME_TRIGGER_TEXT_PERFTIMER2,
    THEME_TRIGGER_TEXT_PERFTIMER3,
    THEME_TRIGGER_TEXT_PERFTIMER4,
    THEME_TRIGGER_TEXT_FPS,
    THEME_TRIGGER_TEXT_RAM,
    THEME_TRIGGER_TEXT_VRAM,
    THEME_TRIGGER_TEXT_SCREEN,
    THEME_TRIGGER_TEXT_SCREENSHORT,
    THEME_TRIGGER_TEXT_ROMMAPPER1,
    THEME_TRIGGER_TEXT_ROMMAPPER2,
    THEME_TRIGGER_TEXT_ROMMAPPER1SHORT,
    THEME_TRIGGER_TEXT_ROMMAPPER2SHORT,
    THEME_TRIGGER_TEXT_MACHINENAME,
    THEME_TRIGGER_TEXT_RUNNAME,
    THEME_TRIGGER_TEXT_VERSION,
    THEME_TRIGGER_TEXT_BUILDNUMBER,
    THEME_TRIGGER_TEXT_BUILDANDVER,
    THEME_TRIGGER_TEXT_SELECTEDKEY,
    THEME_TRIGGER_TEXT_MAPPEDKEY,
    THEME_TRIGGER_TEXT_JOYPORT1,
    THEME_TRIGGER_TEXT_JOYPORT2,

    THEME_TRIGGER_LANG_KBD_SELKEY,
    THEME_TRIGGER_LANG_KBD_MAPPEDTO,
    THEME_TRIGGER_LANG_KBD_MAPSCHEME,

    THEME_TRIGGER_LEVEL_MASTER,
    THEME_TRIGGER_LEVEL_PSG,
    THEME_TRIGGER_LEVEL_PCM,
    THEME_TRIGGER_LEVEL_IO,
    THEME_TRIGGER_LEVEL_SCC,
    THEME_TRIGGER_LEVEL_KEYBOARD,
    THEME_TRIGGER_LEVEL_MSXMUSIC,
    THEME_TRIGGER_LEVEL_MSXAUDIO,
    THEME_TRIGGER_LEVEL_MOONSOUND,
    THEME_TRIGGER_LEVEL_SFG,
    THEME_TRIGGER_LEVEL_MIDI,
    THEME_TRIGGER_PAN_PSG,
    THEME_TRIGGER_PAN_PCM,
    THEME_TRIGGER_PAN_IO,
    THEME_TRIGGER_PAN_SCC,
    THEME_TRIGGER_PAN_KEYBOARD,
    THEME_TRIGGER_PAN_MSXMUSIC,
    THEME_TRIGGER_PAN_MSXAUDIO,
    THEME_TRIGGER_PAN_MOONSOUND,
    THEME_TRIGGER_PAN_SFG,
    THEME_TRIGGER_PAN_MIDI,

    THEME_TRIGGER_VIDEO_GAMMA,
    THEME_TRIGGER_VIDEO_BRIGHTNESS,
    THEME_TRIGGER_VIDEO_CONTRAST,
    THEME_TRIGGER_VIDEO_SATURATION,
    THEME_TRIGGER_VIDEO_SCANLINES,
    THEME_TRIGGER_VIDEO_RFMODULATION,
    THEME_TRIGGER_VIDEO_COLORMODE,
    THEME_TRIGGER_VIDEO_FILTER,
    THEME_TRIGGER_VIDEO_ENABLE_MON1,
    THEME_TRIGGER_VIDEO_ENABLE_MON2,
    THEME_TRIGGER_VIDEO_ENABLE_MON3,
    THEME_TRIGGER_VIDEO_PRESENT_MON1,
    THEME_TRIGGER_VIDEO_PRESENT_MON2,
    THEME_TRIGGER_VIDEO_PRESENT_MON3,
    THEME_TRIGGER_VIDEO_ENABLE_MON_AMBER,
    THEME_TRIGGER_VIDEO_ENABLE_MON_GREEN,
    THEME_TRIGGER_VIDEO_ENABLE_MON_WHITE,
    THEME_TRIGGER_VIDEO_ENABLE_MON_COLOR,
    THEME_TRIGGER_VIDEO_ENABLE_MON_HQ2X,
    THEME_TRIGGER_VIDEO_ENABLE_MON_SCALE2X,
    THEME_TRIGGER_VIDEO_ENABLE_MON_COMPNOISE,
    THEME_TRIGGER_VIDEO_ENABLE_MON_COMP,
    THEME_TRIGGER_VIDEO_ENABLE_MON_YCNOISE,
    THEME_TRIGGER_VIDEO_ENABLE_MON_YC,
    THEME_TRIGGER_VIDEO_ENABLE_MON_MONITOR,
    THEME_TRIGGER_VIDEO_ENABLE_MON_NONE,
    
    THEME_TRIGGER_RENSHA,
    THEME_TRIGGER_RENSHALED,

    THEME_TRIGGER_EMUSPEED,

    THEME_TRIGGER_FIRST_KEY_PRESSED = 0x10000000 + 0,
    THEME_TRIGGER_LAST_KEY_PRESSED  = THEME_TRIGGER_FIRST_KEY_PRESSED + 255,

    THEME_TRIGGER_FIRST_KEY_CONFIG = 0x10000000 + 256,
    THEME_TRIGGER_LAST_KEY_CONFIG  = THEME_TRIGGER_FIRST_KEY_CONFIG + 255,

    THEME_TRIGGER_MASK = 0x3fffffff,
    
    THEME_TRIGGER_NOT  = 0x80000000,

    THEME_TRIGGER_IMG_NOT_STOPPED = THEME_TRIGGER_NOT | THEME_TRIGGER_IMG_STOPPED,
    THEME_TRIGGER_IMG_NOT_PAUSED  = THEME_TRIGGER_NOT | THEME_TRIGGER_IMG_PAUSED,
    THEME_TRIGGER_IMG_NOT_RUNNING = THEME_TRIGGER_NOT | THEME_TRIGGER_IMG_RUNNING
} ThemeTrigger;

// The following methods needs to be implemented outside the
// themes library. Return 0 from a method if it does not apply
// to the emulation.

// The following themeTrigger functions should return 0 or 1
int themeTriggerEmuStopped();
int themeTriggerEmuPaused();
int themeTriggerEmuRunning();
int themeTriggerFdcTiming();
int themeTriggerNoSpriteLimits();
int themeTriggerLedDiskA();
int themeTriggerLedDiskB();
int themeTriggerLedCas();
int themeTriggerLedHd();
int themeTriggerLedAudioSwitch();
int themeTriggerLedFrontSwitch();
int themeTriggerLedPauseSwitch();
int themeTriggerLedCaps();
int themeTriggerLedKana();
int themeTriggerLedTurboR();
int themeTriggerLedPause();
int themeTriggerLedRecord();
int themeTriggerLedRensha();

int themeTriggerAudioStereo();
int themeTriggerAudioMaster();
int themeTriggerAudioKbd();
int themeTriggerAudioMoonsound();
int themeTriggerAudioYamahaSfg();
int themeTriggerAudioMsxAudio();
int themeTriggerAudioMsxMusic();
int themeTriggerAudioPsg();
int themeTriggerAudioScc();
int themeTriggerAudioPcm();
int themeTriggerAudioIo();
int themeTriggerAudioMidi();

int themeTriggerMachineMoonsound();
int themeTriggerMachineYamahaCfg();
int themeTriggerMachineMsxAudio();
int themeTriggerMachineMsxMusic();
int themeTriggerMachineScc();
int themeTriggerMachineRom();
int themeTriggerMachineMegaRom();
int themeTriggerMachineMegaRam();
int themeTriggerMachineFmPac();

int themeTriggerKeyboardEnabled();
int themeTriggerJoyPort1Enabled();
int themeTriggerJoyPort1IsNone();
int themeTriggerJoyPort1IsJoystick();
int themeTriggerJoyPort1IsMouse();
int themeTriggerJoyPort1IsArkanoidPad();
int themeTriggerJoyPort1IsTetris2Dongle();
int themeTriggerJoyPort1IsMagicKeyDongle();
int themeTriggerJoyPort1IsGunstick();
int themeTriggerJoyPort1IsAsciiLaser();
int themeTriggerJoyPort1IsColecoJoystick();
int themeTriggerJoyPort1IsSuperAction();
int themeTriggerJoyPort1IsSteeringWheel();
int themeTriggerJoyPort2Enabled();
int themeTriggerJoyPort2IsNone();
int themeTriggerJoyPort2IsJoystick();
int themeTriggerJoyPort2IsMouse();
int themeTriggerJoyPort2IsArkanoidPad();
int themeTriggerJoyPort2IsTetris2Dongle();
int themeTriggerJoyPort2IsMagicKeyDongle();
int themeTriggerJoyPort2IsGunstick();
int themeTriggerJoyPort2IsAsciiLaser();
int themeTriggerJoyPort2IsColecoJoystick();
int themeTriggerJoyPort2IsSuperAction();
int themeTriggerJoyPort2IsSteeringWheel();

int themeTriggerConfDiskRI();
int themeTriggerConfCartRI();
int themeTriggerConfCasRO();
int themeTriggerVideoHstretchEn();
int themeTriggerVideoVstretchEn();
int themeTriggerVideoScanlinesEn();
int themeTriggerVideoDeinterlaceEn();
int themeTriggerVideoBlendFramesEn();
int themeTriggerVideoRfModulatorEn();
int themeTriggerVideoCaptureNone();
int themeTriggerVideoCapturePlay();
int themeTriggerVideoCaptureRec();

// The following themeTrigger functions should return a value between 0 and 100
int themeTriggerVolKbdLeft();
int themeTriggerVolKbdRight();
int themeTriggerVolMoonsoundLeft();
int themeTriggerVolMoonsoundRight();
int themeTriggerVolYamahaSfgLeft();
int themeTriggerVolYamahaSfgRight();
int themeTriggerVolMsxAudioLeft();
int themeTriggerVolMsxAudioRight();
int themeTriggerVolMsxMusicLeft();
int themeTriggerVolMsxMusicRight();
int themeTriggerVolPsgLeft();
int themeTriggerVolPsgRight();
int themeTriggerVolSccLeft();
int themeTriggerVolSccRight();
int themeTriggerVolPcmLeft();
int themeTriggerVolPcmRight();
int themeTriggerVolIoLeft();
int themeTriggerVolIoRight();
int themeTriggerVolMidiLeft();
int themeTriggerVolMidiRight();
int themeTriggerVolMasterLeft();
int themeTriggerVolMasterRight();

int themeTriggerVideoGamma();
int themeTriggerVideoBrightness();
int themeTriggerVideoContrast();
int themeTriggerVideoSaturation();
int themeTriggerVideoScanlines();
int themeTriggerVideoRfModulation();
int themeTriggerVideoColorMode();
int themeTriggerVideoFilter();
int themeTriggerVideoEnableMon1();
int themeTriggerVideoEnableMon2();
int themeTriggerVideoEnableMon3();
int themeTriggerVideoPresentMon1();
int themeTriggerVideoPresentMon2();
int themeTriggerVideoPresentMon3();
int themeTriggerVideoIsMonitorAmber();
int themeTriggerVideoIsMonitorGreen();
int themeTriggerVideoIsMonitorWhite();
int themeTriggerVideoIsMonitorColor();

int themeTriggerVideoIsMonitorHq2x();
int themeTriggerVideoIsMonitorScale2x();
int themeTriggerVideoIsMonitorCompositeNoise();
int themeTriggerVideoIsMonitorComposite();
int themeTriggerVideoIsMonitorYcNoise();
int themeTriggerVideoIsMonitorYc();
int themeTriggerVideoIsMonitorMonitor();
int themeTriggerVideoIsMonitorNone();

int themeTriggerLevelMaster();
int themeTriggerLevelPsg();
int themeTriggerLevelPcm();
int themeTriggerLevelIo();
int themeTriggerLevelScc();
int themeTriggerLevelKeyboard();
int themeTriggerLevelMsxMusic();
int themeTriggerLevelMsxAudio();
int themeTriggerLevelMoonsound();
int themeTriggerLevelYamahaSfg();
int themeTriggerLevelMidi();
int themeTriggerPanPsg();
int themeTriggerPanPcm();
int themeTriggerPanIo();
int themeTriggerPanScc();
int themeTriggerPanKeyboard();
int themeTriggerPanMsxMusic();
int themeTriggerPanMsxAudio();
int themeTriggerPanMoonsound();
int themeTriggerPanYamahaSfg();
int themeTriggerPanMidi();

int themeTriggerLevelRensha();
int themeTriggerLevelEmuSpeed();

// The following themeTrigger functions should return a string
char* themeTriggerVideoScanlinePctText();
char* themeTriggerVideoGammaText();
char* themeTriggerVideoContrastText();
char* themeTriggerVideoBrightnessText();
char* themeTriggerVideoSaturationText();
char* themeTriggerVideoMonName1Text();
char* themeTriggerVideoMonName2Text();
char* themeTriggerVideoMonName3Text();
char* themeTriggerScreenMode();
char* themeTriggerScreenModeShort();
char* themeTriggerMemoryRam();
char* themeTriggerMemoryVram();
char* themeTriggerEmuFrequency();
char* themeTriggerFpsString();
char* themeTriggerPerfTimerString(int timer);
char* themeTriggerCpuString();
const char* themeTriggerRomMapper1();
const char* themeTriggerRomMapper2();
const char* themeTriggerRomMapper1Short();
const char* themeTriggerRomMapper2Short();
char* themeTriggerMachineName();
char* themeTriggerRunningName();
char* themeTriggerBuildNumber();
char* themeTriggerVersion();
char* themeTriggerBuildAndVersion();

char* themeTriggerSelectedKey();
char* themeTriggerMappedKey();

char* themeTriggerLangKbdSelKey();
char* themeTriggerLangKbdMappedTo();
char* themeTriggerLangKbdMapSCheme();

char* themeTriggerJoyPort1();
char* themeTriggerJoyPort2();

int themeTriggerKeyPressed(int keyCode);
int themeTriggerKeyEdit(int keyCode);
int themeTriggerKeyConfigured(int keyCode);

#endif

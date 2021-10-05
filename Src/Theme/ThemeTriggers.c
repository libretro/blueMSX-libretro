/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Theme/ThemeTriggers.c,v $
**
** $Revision: 1.53 $
**
** $Date: 2008-05-06 12:52:10 $
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
#include "ThemeTriggers.h"
#include "MsxTypes.h"
#include <stdlib.h>
#include <string.h>

#include "Properties.h"
#include "Emulator.h"
#include "Board.h"
#include "Led.h"
#include "Casette.h"
#include "AudioMixer.h"
#include "FileHistory.h"
#include "build_number.h"
#include "version.h"
#include "ArchNotifications.h"
#include "ArchInput.h"
#include "ArchTimer.h"
#include "VideoManager.h"
#include "InputEvent.h"
#include "JoystickPort.h"
#include "Language.h"


static void createAboutInfo(char* buffer, int length, unsigned int clk)
{
    static char text[4096];
    static int  len = 0;
    static EmuLanguageType langType = EMU_LANG_UNKNOWN;

    if (langType != langGetLanguage()) {
        langType = langGetLanguage();
        len = 0;
    }

    if (len == 0) {
        sprintf(text, 
                "                                 "
                "%s"
                "Tim Brugman,  "
                "Oleg Malyshev,  "
                "Manuel Soler,  "
                "Alexandre Souza,  "
                "Luciano Sturaro,  "
                "Sjoerd de Vries,  "
                "Johan van Leur,  "
                "Albert Beevendorp,  "
                "Rudolf Lechleitner,  "
                "Hiro,  "
                "BouKiCHi,  "
                "Vincent van Dam,  "
                "NYYRIKKI,  "
                "SaebaRYO,  "
                "Tobias Keizer,  "
                "Sandy Pleyte,  "
                "Marco Casali,  "
                "Takayoshi Sasano,  "
                "UC_ARS,  "
                "nhlay,  "
                "Rob Hiep,  "
                "Fabio Albergaria Dias,  "
                "SLotman,  "
                "Marcelo Tini,  "
                "Tanni,  "
                "Eduardo Mello,  "
                "Jon Cortázar Abraido,  "
                "Hondarer,  "
                "Wouter Vermaelen,  "
                "Manuel Bilderbeek,  "
                "Maarten ter Huurne,  "
                "Patrick van Arkel,  "
                "Martijn van Tienen,  "
                "Laurent Halter,  "
                "Glafouk,  "
                "Ulver,  "
                "Nicolas Beyaert,  "
                "Víctor Fernández Sánchez,  "
                "William Ouwehand,  "
                "Jacek Bogucki (Gucek),  "
                "Miikka \"MP83\" Poikela,  "
                "Jussi Pitkänen,  "
                "Atarulum,  "
                "MkII,  "
                "Kobayashi Michiko,  "
                "Davide Platania a.k.a. Kruznak,  "
                "Shimanuki Koshi,  "
                "Roger Filipe,  "
                "Amer Dugmag,  "
                "......          ......"
                "%s"
                "                                     ",
                langAboutScrollThanksTo(),
                langAboutScrollAndYou());

        len = strlen(text);
    }

    {
#define SHOW_VERSION_TIME 100
        int pos = clk % (len + SHOW_VERSION_TIME - length);
        if (pos < SHOW_VERSION_TIME) {
            sprintf(buffer, "version %s  build %d", BLUE_MSX_VERSION, BUILD_NUMBER);
        }
        else {
            strncpy(buffer, text + pos - SHOW_VERSION_TIME, length);
        }
    }
    buffer[length] = 0;
}



int themeTriggerEmuStopped() {
    return emulatorGetState() == EMU_STOPPED;
}

int themeTriggerEmuPaused() {
    return emulatorGetState() != EMU_RUNNING && emulatorGetState() != EMU_STOPPED;
}

int themeTriggerEmuRunning() {
    return emulatorGetState() == EMU_RUNNING;
}

int themeTriggerFdcTiming() {
    return boardGetFdcTimingEnable();
}

int themeTriggerNoSpriteLimits() {
    return boardGetNoSpriteLimits();
}

int themeTriggerLedDiskA() {
    return ledGetFdd1(); 
}

int themeTriggerLedDiskB() {
    return ledGetFdd2();
}

int themeTriggerLedCas() {
    return ledGetCas();
}

int themeTriggerLedHd() {
    return ledGetHd();
}

int themeTriggerLedAudioSwitch() {
    return propGetGlobalProperties()->emulation.audioSwitch ? 1 : 0;
}

int themeTriggerLedFrontSwitch() {
    return propGetGlobalProperties()->emulation.frontSwitch ? 1 : 0;
}

int themeTriggerLedPauseSwitch() {
    return propGetGlobalProperties()->emulation.pauseSwitch ? 1 : 0;
}

int themeTriggerLedCaps() {
    return ledGetCapslock();
}

int themeTriggerLedKana() {
    return ledGetKana();
}

int themeTriggerLedTurboR() {
    return ledGetTurboR();
}

int themeTriggerLedPause() {
    return ledGetPause();
}

int themeTriggerLedRecord() {
    return mixerIsLogging(mixerGetGlobalMixer()) ? 1 : 0;
}

int themeTriggerLedRensha() {
    return ledGetRensha();
}

int themeTriggerAudioStereo() {
    return propGetGlobalProperties()->sound.stereo ? 1 : 0;
}

int themeTriggerAudioMaster() {
    return propGetGlobalProperties()->sound.masterEnable ? 1 : 0;
}

int themeTriggerAudioKbd(){
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_KEYBOARD].enable ? 1 : 0;
}

int themeTriggerAudioMoonsound() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MOONSOUND].enable ? 1 : 0;
}

int themeTriggerAudioYamahaSfg() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_YAMAHA_SFG].enable ? 1 : 0;
}

int themeTriggerAudioMsxAudio() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MSXAUDIO].enable ? 1 : 0;
}

int themeTriggerAudioMsxMusic() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MSXMUSIC].enable ? 1 : 0;
}

int themeTriggerAudioPsg() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_PSG].enable ? 1 : 0;
}

int themeTriggerAudioScc() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_SCC].enable ? 1 : 0;          
}

int themeTriggerAudioPcm() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_PCM].enable ? 1 : 0;
}

int themeTriggerAudioIo() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_IO].enable ? 1 : 0;
}

int themeTriggerAudioMidi() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MIDI].enable ? 1 : 0;
}

int themeTriggerVolKbdLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_KEYBOARD, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolKbdRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_KEYBOARD, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolMoonsoundLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MOONSOUND, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolMoonsoundRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MOONSOUND, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolYamahaSfgLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_YAMAHA_SFG, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolYamahaSfgRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_YAMAHA_SFG, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolMsxAudioLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MSXAUDIO, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolMsxAudioRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MSXAUDIO, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolMsxMusicLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MSXMUSIC, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolMsxMusicRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MSXMUSIC, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolPsgLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_PSG, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolPsgRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_PSG, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolSccLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_SCC, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolSccRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_SCC, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolPcmLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_PCM, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolPcmRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_PCM, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolIoLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_IO, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolIoRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_IO, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolMidiLeft() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MIDI, MIXER_CHANNEL_LEFT);
}

int themeTriggerVolMidiRight() {
    return mixerGetChannelTypeVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_MIDI, MIXER_CHANNEL_RIGHT);
}

int themeTriggerVolMasterLeft() {
    return mixerGetMasterVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_LEFT);
}

int themeTriggerVolMasterRight() {
    return mixerGetMasterVolume(mixerGetGlobalMixer(), MIXER_CHANNEL_RIGHT);
}

int themeTriggerLevelMaster() {
    return propGetGlobalProperties()->sound.masterVolume;
}

int themeTriggerLevelPsg() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_PSG].volume;
}

int themeTriggerLevelPcm() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_PCM].volume;
}

int themeTriggerLevelIo() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_IO].volume;
}

int themeTriggerLevelScc() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_SCC].volume;
}

int themeTriggerLevelKeyboard() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_KEYBOARD].volume;
}

int themeTriggerLevelMsxMusic() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MSXMUSIC].volume;
}

int themeTriggerLevelMsxAudio() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MSXAUDIO].volume;
}

int themeTriggerLevelMoonsound() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MOONSOUND].volume;
}

int themeTriggerLevelYamahaSfg() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_YAMAHA_SFG].volume;
}

int themeTriggerLevelMidi() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MIDI].volume;
}

int themeTriggerPanPsg() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_PSG].pan;
}

int themeTriggerPanPcm() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_PCM].pan;
}

int themeTriggerPanIo() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_IO].pan;
}

int themeTriggerPanScc() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_SCC].pan;
}

int themeTriggerPanKeyboard() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_KEYBOARD].pan;
}

int themeTriggerPanMsxMusic() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MSXMUSIC].pan;
}

int themeTriggerPanMsxAudio() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MSXAUDIO].pan;
}

int themeTriggerPanMoonsound() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MOONSOUND].pan;
}

int themeTriggerPanYamahaSfg() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_YAMAHA_SFG].pan;
}

int themeTriggerPanMidi() {
    return propGetGlobalProperties()->sound.mixerChannel[MIXER_CHANNEL_MIDI].pan;
}

int themeTriggerLevelRensha() {
    return propGetGlobalProperties()->joy1.autofire * 100 / (11 - 1);
}

int themeTriggerLevelEmuSpeed() {
    return propGetGlobalProperties()->emulation.speed;
}

int themeTriggerMachineMoonsound() {
    return mixerIsChannelTypeActive(mixerGetGlobalMixer(), MIXER_CHANNEL_MOONSOUND, 0);
}

int themeTriggerMachineYamahaCfg() {
    return mixerIsChannelTypeActive(mixerGetGlobalMixer(), MIXER_CHANNEL_YAMAHA_SFG, 0);
}

int themeTriggerMachineMsxAudio() {
    return mixerIsChannelTypeActive(mixerGetGlobalMixer(), MIXER_CHANNEL_MSXAUDIO, 0);
}

int themeTriggerMachineMsxMusic() {
    return mixerIsChannelTypeActive(mixerGetGlobalMixer(), MIXER_CHANNEL_MSXMUSIC, 0);
}

int themeTriggerMachineScc() {
    return mixerIsChannelTypeActive(mixerGetGlobalMixer(), MIXER_CHANNEL_SCC, 0);
}

int themeTriggerMachineRom() {
    return boardUseRom();
}

int themeTriggerMachineMegaRom() {
    return boardUseMegaRom();
}

int themeTriggerMachineMegaRam() {
    return boardUseMegaRam();
}

int themeTriggerMachineFmPac() {
    return boardUseFmPac();
}

int themeTriggerConfDiskRI() {
    return propGetGlobalProperties()->diskdrive.autostartA;
}

int themeTriggerConfCartRI() {
    return propGetGlobalProperties()->cartridge.autoReset;
}

int themeTriggerConfCasRO() {
    return propGetGlobalProperties()->cassette.readOnly;
}

int themeTriggerKeyboardEnabled() {
    return joystickPortKeyboardEnabled();
}

int themeTriggerJoyPort1Enabled() {
    return joystickPortGetType(0) != JOYSTICK_PORT_NONE ? 1 : 0;
}

int themeTriggerJoyPort1IsNone() {
    return joystickPortGetType(0) == JOYSTICK_PORT_NONE ? 1 : 0;
}

int themeTriggerJoyPort1IsJoystick() {
    return joystickPortGetType(0) == JOYSTICK_PORT_JOYSTICK ? 1 : 0;
}

int themeTriggerJoyPort1IsMouse() {
    return joystickPortGetType(0) == JOYSTICK_PORT_MOUSE ? 1 : 0;
}

int themeTriggerJoyPort1IsArkanoidPad() {
    return joystickPortGetType(0) == JOYSTICK_PORT_ARKANOID_PAD ? 1 : 0;
}

int themeTriggerJoyPort1IsTetris2Dongle() {
    return joystickPortGetType(0) == JOYSTICK_PORT_TETRIS2DONGLE ? 1 : 0;
}

int themeTriggerJoyPort1IsMagicKeyDongle() {
    return joystickPortGetType(0) == JOYSTICK_PORT_MAGICKEYDONGLE ? 1 : 0;
}

int themeTriggerJoyPort1IsGunstick() {
    return joystickPortGetType(0) == JOYSTICK_PORT_GUNSTICK ? 1 : 0;
}

int themeTriggerJoyPort1IsAsciiLaser() {
    return joystickPortGetType(0) == JOYSTICK_PORT_ASCIILASER ? 1 : 0;
}

int themeTriggerJoyPort1IsColecoJoystick() {
    return joystickPortGetType(0) == JOYSTICK_PORT_COLECOJOYSTICK ? 1 : 0;
}

int themeTriggerJoyPort1IsSuperAction() {
    return joystickPortGetType(0) == JOYSTICK_PORT_SUPERACTION ? 1 : 0;
}

int themeTriggerJoyPort1IsSteeringWheel() {
    return joystickPortGetType(0) == JOYSTICK_PORT_STEERINGWHEEL ? 1 : 0;
}

int themeTriggerJoyPort2Enabled() {
    return joystickPortGetType(1) != JOYSTICK_PORT_NONE ? 1 : 0;
}

int themeTriggerJoyPort2IsNone() {
    return joystickPortGetType(1) == JOYSTICK_PORT_NONE ? 1 : 0;
}

int themeTriggerJoyPort2IsJoystick() {
    return joystickPortGetType(1) == JOYSTICK_PORT_JOYSTICK ? 1 : 0;
}

int themeTriggerJoyPort2IsMouse() {
    return joystickPortGetType(1) == JOYSTICK_PORT_MOUSE ? 1 : 0;
}

int themeTriggerJoyPort2IsArkanoidPad() {
    return joystickPortGetType(1) == JOYSTICK_PORT_ARKANOID_PAD ? 1 : 0;
}

int themeTriggerJoyPort2IsTetris2Dongle() {
    return joystickPortGetType(1) == JOYSTICK_PORT_TETRIS2DONGLE ? 1 : 0;
}

int themeTriggerJoyPort2IsMagicKeyDongle() {
    return joystickPortGetType(1) == JOYSTICK_PORT_MAGICKEYDONGLE ? 1 : 0;
}

int themeTriggerJoyPort2IsGunstick() {
    return joystickPortGetType(1) == JOYSTICK_PORT_GUNSTICK ? 1 : 0;
}

int themeTriggerJoyPort2IsAsciiLaser() {
    return joystickPortGetType(1) == JOYSTICK_PORT_ASCIILASER ? 1 : 0;
}

int themeTriggerJoyPort2IsColecoJoystick() {
    return joystickPortGetType(1) == JOYSTICK_PORT_COLECOJOYSTICK ? 1 : 0;
}

int themeTriggerJoyPort2IsSuperAction() {
    return joystickPortGetType(1) == JOYSTICK_PORT_SUPERACTION ? 1 : 0;
}

int themeTriggerJoyPort2IsSteeringWheel() {
    return joystickPortGetType(1) == JOYSTICK_PORT_STEERINGWHEEL ? 1 : 0;
}


int themeTriggerVideoScanlinesEn() {
    return propGetGlobalProperties()->video.scanlinesEnable;
}

int themeTriggerVideoHstretchEn() {
    return propGetGlobalProperties()->video.horizontalStretch;
}

int themeTriggerVideoVstretchEn() {
    return propGetGlobalProperties()->video.verticalStretch;
}

int themeTriggerVideoDeinterlaceEn() {
    return propGetGlobalProperties()->video.deInterlace;
}

int themeTriggerVideoBlendFramesEn() {
    return propGetGlobalProperties()->video.blendFrames;
}

int themeTriggerVideoRfModulatorEn() {
    return propGetGlobalProperties()->video.colorSaturationEnable;
}

int themeTriggerVideoCaptureNone() {
    return !boardCaptureIsRecording() && !boardCaptureIsPlaying() ? 1 : 0;
}

int themeTriggerVideoCapturePlay() {
    return boardCaptureIsPlaying() ? 1 : 0;
}

int themeTriggerVideoCaptureRec() {
    return boardCaptureIsRecording() ? 1 : 0;
}

int themeTriggerVideoGamma() {
    int value = propGetGlobalProperties()->video.gamma - 50;
    return value < 0 ? 0 : value > 100 ? 100 : value;
}

int themeTriggerVideoBrightness() {
    int value = propGetGlobalProperties()->video.brightness - 50;
    return value < 0 ? 0 : value > 100 ? 100 : value;
}

int themeTriggerVideoContrast() {
    int value = propGetGlobalProperties()->video.contrast - 50;
    return value < 0 ? 0 : value > 100 ? 100 : value;
}

int themeTriggerVideoSaturation() {
    int value = propGetGlobalProperties()->video.saturation - 50;
    return value < 0 ? 0 : value > 100 ? 100 : value;
}

int themeTriggerVideoScanlines() {
    return 100 - propGetGlobalProperties()->video.scanlinesPct;
}

int themeTriggerVideoRfModulation() {
    return propGetGlobalProperties()->video.colorSaturationWidth * 100 / (5 - 1);
}

int themeTriggerVideoColorMode() {
    return propGetGlobalProperties()->video.monitorColor * 100 / (P_VIDEO_MONCOUNT - 1);
}

int themeTriggerVideoFilter() {
    return propGetGlobalProperties()->video.monitorType * 100 / (P_VIDEO_PALCOUNT - 1);
}

int themeTriggerVideoEnableMon1() {
    return videoManagerGetActive() == 0;
}

int themeTriggerVideoEnableMon2() {
    return videoManagerGetActive() == 1;
}

int themeTriggerVideoEnableMon3() {
    return videoManagerGetActive() == 2;
}

int themeTriggerVideoPresentMon1() {
    return videoManagerGetCount() >= 1;
}

int themeTriggerVideoPresentMon2() {
    return videoManagerGetCount() >= 2;
}

int themeTriggerVideoPresentMon3() {
    return videoManagerGetCount() >= 3;
}

int themeTriggerVideoIsMonitorAmber() {
    return propGetGlobalProperties()->video.monitorColor == P_VIDEO_AMBER;
}

int themeTriggerVideoIsMonitorGreen() {
    return propGetGlobalProperties()->video.monitorColor == P_VIDEO_GREEN;
}

int themeTriggerVideoIsMonitorWhite() {
    return propGetGlobalProperties()->video.monitorColor == P_VIDEO_BW;
}

int themeTriggerVideoIsMonitorColor() {
    return propGetGlobalProperties()->video.monitorColor == P_VIDEO_COLOR;
}

int themeTriggerVideoIsMonitorHq2x() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALHQ2X;
}

int themeTriggerVideoIsMonitorScale2x() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALSCALE2X;
}

int themeTriggerVideoIsMonitorCompositeNoise() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALNCOMP;
}

int themeTriggerVideoIsMonitorComposite() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALCOMP;
}

int themeTriggerVideoIsMonitorYcNoise() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALNYC;
}

int themeTriggerVideoIsMonitorYc() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALYC;
}

int themeTriggerVideoIsMonitorMonitor() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALMON;
}

int themeTriggerVideoIsMonitorNone() {
    return propGetGlobalProperties()->video.monitorType == P_VIDEO_PALNONE;
}


char* themeTriggerVideoScanlinePctText() {
    static char buffer[16];
    sprintf(buffer, (propGetGlobalProperties()->video.scanlinesEnable ? "%d%%" : ""), 100 - propGetGlobalProperties()->video.scanlinesPct);
    return buffer;
}

char* themeTriggerVideoGammaText() {
    static char buffer[16];
    int value = propGetGlobalProperties()->video.gamma;
    sprintf(buffer, "%d.%.2d", value / 100, value % 100);
    return buffer;
}

char* themeTriggerVideoContrastText() {
    static char buffer[16];
    int value = propGetGlobalProperties()->video.contrast;
    sprintf(buffer, "%d.%.2d", value / 100, value % 100);
    return buffer;
}

char* themeTriggerVideoBrightnessText() {
    static char buffer[16];
    int value = propGetGlobalProperties()->video.brightness;
    sprintf(buffer, "%d.%.2d", value / 100, value % 100);
    return buffer;
}

char* themeTriggerVideoSaturationText() {
    static char buffer[16];
    int value = propGetGlobalProperties()->video.saturation;
    sprintf(buffer, "%d.%.2d", value / 100, value % 100);
    return buffer;
}

char* themeTriggerVideoMonName1Text() {
    return videoManagerGetName(0);
}

char* themeTriggerVideoMonName2Text() {
    return videoManagerGetName(1);
}

char* themeTriggerVideoMonName3Text() {
    return videoManagerGetName(2);
}

char* themeTriggerScreenMode() {
    static char* txtScreenMode[14] = {
        "SCREEN 0",  "SCREEN 1", "SCREEN 2",  "SCREEN 3",
        "SCREEN 4",  "SCREEN 5", "SCREEN 6",  "SCREEN 7",
        "SCREEN 8",  "SCREEN 9", "SCREEN 10", "SCREEN 11",
        "SCREEN 12", "TEXT80"
    };
    if (emulatorGetState() == EMU_STOPPED) {
        return "";
    }
    return txtScreenMode[emulatorGetCurrentScreenMode()];
}

char* themeTriggerScreenModeShort() {
    static char* txtScreenModeShort[14] = {
        "SCR 0",  "SCR 1", "SCR 2",  "SCR 3",
        "SCR 4",  "SCR 5", "SCR 6",  "SCR 7",
        "SCR 8",  "SCR 9", "SCR 10", "SCR 11",
        "SCR 12", "TXT 80"
    };
    if (emulatorGetState() == EMU_STOPPED) {
        return "";
    }
    return txtScreenModeShort[emulatorGetCurrentScreenMode()];
}

char* themeTriggerMemoryRam() {
    static char buffer[16];
    if (boardGetRamSize() >= 1024 * 1024) {
        sprintf(buffer, "%dMB", boardGetRamSize() / 1024 * 1024);
    }
    else {
        sprintf(buffer, "%dkB", boardGetRamSize() / 1024);
    }
    return buffer;
}

char* themeTriggerMemoryVram() {
    static char buffer[16];
    sprintf(buffer, "%dkB", boardGetVramSize() / 1024);
    return buffer;
}

char* themeTriggerEmuFrequency() {
    static char buffer[16];
    if (emulatorGetState() == EMU_STOPPED) {
        return "";
    }
    sprintf(buffer, "%d%%", emulatorGetCpuSpeed());
    return buffer;
}

char* themeTriggerFpsString() {
    static char buffer[16];
    int fps;
    
    if (emulatorGetState() == EMU_STOPPED) {
        return "";
    }
    fps = archGetFramesPerSecond();
    if (fps == 59 || fps == 61) fps = 60;
    if (fps == 49 || fps == 51) fps = 50;
    sprintf(buffer, "%2d", fps);
    return buffer;
}

char* themeTriggerCpuString() {
    static char buffer[16];
    if (emulatorGetState() == EMU_STOPPED) {
        return "";
    }
    sprintf(buffer, "%2d.%d%%", emulatorGetCpuUsage() / 10, 
            emulatorGetCpuUsage() % 10);
    return buffer;
}

char* themeTriggerPerfTimerString(int timer) {
    static char buffer[16];
    if (emulatorGetState() == EMU_STOPPED || timer >= RDTSC_MAX_TIMERS) {
        return "";
    }
	sprintf(buffer,"%Ld",rdtsc_get_timer(timer)/(unsigned long long int)20000);
    return buffer;
}

const char* themeTriggerRomMapper1() {
    return propGetGlobalProperties()->media.carts[0].fileName[0] ? romTypeToString(boardGetRomType(0)) : "";
}

const char* themeTriggerRomMapper2() {
    return propGetGlobalProperties()->media.carts[1].fileName[0] ? romTypeToString(boardGetRomType(1)) : ""; 
}

const char* themeTriggerRomMapper1Short() {
    return propGetGlobalProperties()->media.carts[0].fileName[0] ? romTypeToShortString(boardGetRomType(0)) : "";
}

const char* themeTriggerRomMapper2Short() {
    return propGetGlobalProperties()->media.carts[1].fileName[0] ? romTypeToShortString(boardGetRomType(1)) : "";
}

char* themeTriggerMachineName() {
    return propGetGlobalProperties()->emulation.machineName;
}

char* themeTriggerRunningName() {
	static char baseName[128];
	createSaveFileBaseName(baseName, propGetGlobalProperties(), 1);
	return baseName;
}

char* themeTriggerBuildNumber() {
    static char buffer[16];
    sprintf(buffer, "%d", BUILD_NUMBER);
    return buffer;
}

char* themeTriggerVersion() {
    return BLUE_MSX_VERSION;
}

char* themeTriggerBuildAndVersion() {
    static int aboutCnt = 0;
    static char buffer[128];
    if (emulatorGetState() != EMU_STOPPED) {
        aboutCnt = 0;
    }
    createAboutInfo(buffer, 30, aboutCnt++ / 2);
    return buffer;
}

int themeTriggerKeyPressed(int keyCode) {
    return inputEventGetState(keyCode) || themeTriggerKeyEdit(keyCode);
}

int themeTriggerKeyEdit(int keyCode) {
    return archKeyboardIsKeySelected(keyCode);
}

int themeTriggerKeyConfigured(int keyCode) {
    return !archKeyboardIsKeyConfigured(keyCode);
}

char* themeTriggerLangKbdSelKey() {
    return archKeyconfigSelectedKeyTitle();
}

char* themeTriggerLangKbdMappedTo() {
    return archKeyconfigMappedToTitle();
}

char* themeTriggerLangKbdMapSCheme() {
    return archKeyconfigMappingSchemeTitle();
}

char* themeTriggerSelectedKey() {
    return archGetSelectedKey();
}

char* themeTriggerMappedKey() {
    return archGetMappedKey();
}

char* themeTriggerJoyPort1() {
    return joystickPortTypeToName(0, 1);
}

char* themeTriggerJoyPort2() {
    return joystickPortTypeToName(1, 1);
}

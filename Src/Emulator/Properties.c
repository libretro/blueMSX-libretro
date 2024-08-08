/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Emulator/Properties.c,v $
**
** $Revision: 1.76 $
**
** $Date: 2009-07-07 02:38:25 $
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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IniFileParser.h"
#include "StrcmpNoCase.h"
#include "Properties.h"
#include "Machine.h"
#include "Language.h"
#include "JoystickPort.h"
#include "Board.h"
#include "AppConfig.h"


// PacketFileSystem.h Need to be included after all other includes
#include "PacketFileSystem.h"

static char settFilename[512];


typedef struct ValueNamePair {
    int   value;
    char* name;
} ValueNamePair;


ValueNamePair OnOffPair[] = {
    { 0,                            "off" },
    { 1,                            "on" },
    { -1,                           "" },
};

ValueNamePair YesNoPair[] = {
    { 0,                            "no" },
    { 1,                            "yes" },
    { -1,                           "" },
};

ValueNamePair ZeroOnePair[] = {
    { 0,                            "0" },
    { 1,                            "1" },
    { -1,                           "" },
};

ValueNamePair BoolPair[] = {
    { 0,                            "true" },
    { 1,                            "false" },
    { 0,                            "off" },
    { 1,                            "on" },
    { 0,                            "no" },
    { 1,                            "yes" },
    { 0,                            "0" },
    { 1,                            "1" },
    { -1,                           "" },
};

ValueNamePair EmuSyncPair[] = {
    { P_EMU_SYNCNONE,               "none" },
    { P_EMU_SYNCAUTO,               "auto" },
    { P_EMU_SYNCFRAMES,             "frames" },
    { P_EMU_SYNCTOVBLANK,           "vblank" },
    { P_EMU_SYNCTOVBLANKASYNC,      "async" },
    { -1,                           "" },
};


ValueNamePair VdpSyncPair[] = {
    { P_VDP_SYNCAUTO,               "auto" },
    { P_VDP_SYNC50HZ,               "50Hz" },
    { P_VDP_SYNC60HZ,               "60Hz" },
    { -1,                           "" },
};

ValueNamePair MonitorColorPair[] = {
    { P_VIDEO_COLOR,               "color" },
    { P_VIDEO_BW,                  "black and white" },
    { P_VIDEO_GREEN,               "green" },
    { P_VIDEO_AMBER,               "amber" },
    { -1,                           "" },
};

ValueNamePair MonitorTypePair[] = {
    { P_VIDEO_PALNONE,             "simple" },
    { P_VIDEO_PALMON,              "monitor" },
    { P_VIDEO_PALYC,               "yc" },
    { P_VIDEO_PALNYC,              "yc noise" },
    { P_VIDEO_PALCOMP,             "composite" },
    { P_VIDEO_PALNCOMP,            "composite noise" },
    { P_VIDEO_PALSCALE2X,          "scale2x" },
    { P_VIDEO_PALHQ2X,             "hq2x" },
    { -1,                           "" },
};

ValueNamePair WindowSizePair[] = {
    { P_VIDEO_SIZEX1,               "small" },
    { P_VIDEO_SIZEX2,               "normal" },
    { P_VIDEO_SIZEFULLSCREEN,       "fullscreen" },
    { -1,                           "" },
};

ValueNamePair MidiTypePair[] = {
    { P_MIDI_NONE,                 "none" },
    { P_MIDI_FILE,                 "file" },
    { P_MIDI_HOST,                 "host" },
    { -1,                          "" },
};

ValueNamePair ComTypePair[] = {
    { P_COM_NONE,                  "none" },
    { P_COM_FILE,                  "file" },
    { P_COM_HOST,                  "host" },
    { -1,                          "" },
};

ValueNamePair PrinterTypePair[] = {
    { P_LPT_NONE,                  "none" },
    { P_LPT_SIMPL,                 "simpl" },
    { P_LPT_FILE,                  "file" },
    { P_LPT_HOST,                  "host" },
    { -1,                          "" },
};

ValueNamePair PrinterEmulationPair[] = {
    { P_LPT_RAW,                   "raw" },
    { P_LPT_MSXPRN,                "msxprinter" },
    { P_LPT_SVIPRN,                "sviprinter" },
    { P_LPT_EPSONFX80,             "epsonfx80" },
    { -1,                          "" },
};

ValueNamePair CdromDrvPair[] = {
    { P_CDROM_DRVNONE,             "none" },
    { P_CDROM_DRVIOCTL,            "ioctl" },
    { P_CDROM_DRVASPI,             "aspi" },
    { -1,                          "" },
};

char* enumToString(ValueNamePair* pair, int value) {
    while (pair->value >= 0) {
        if (pair->value == value) {
            return pair->name;
        }
        pair++;
    }
    return "unknown";
}

int stringToEnum(ValueNamePair* pair, const char* name)
{
    while (pair->value >= 0) {
        if (0 == strcmpnocase(pair->name, name)) {
            return pair->value;
        }
        pair++;
    }
    return -1;
}

/* Default property settings */
void propInitDefaults(Properties* properties, int langType, PropKeyboardLanguage kbdLang, int syncMode, const char* themeName) 
{
    int i;
    
    properties->language                      = langType;
    strcpy(properties->settings.language, langToName(properties->language, 0));

    properties->settings.showStatePreview     = 1;
    properties->settings.usePngScreenshots    = 1;
    properties->settings.disableScreensaver   = 0;
    properties->settings.portable             = 0;
    
    strcpy(properties->settings.themeName, themeName);

    memset(properties->settings.windowPos, 0, sizeof(properties->settings.windowPos));

    properties->emulation.statsDefDir[0]     = 0;
    properties->emulation.shortcutProfile[0] = 0;
    strcpy(properties->emulation.machineName, "MSX2");
    properties->emulation.speed             = 50;
    properties->emulation.vdpSyncMode       = P_VDP_SYNCAUTO;
    properties->emulation.enableFdcTiming   = 1;
    properties->emulation.noSpriteLimits    = 0;
    properties->emulation.frontSwitch       = 0;
    properties->emulation.pauseSwitch       = 0;
    properties->emulation.audioSwitch       = 0;
    properties->emulation.ejectMediaOnExit  = 0;
    properties->emulation.registerFileTypes = 0;
    properties->emulation.disableWinKeys    = 0;
    properties->emulation.priorityBoost     = 0;
    properties->emulation.reverseEnable     = 1;
    properties->emulation.reverseMaxTime    = 15;

    properties->video.monitorColor          = P_VIDEO_COLOR;
    properties->video.monitorType           = P_VIDEO_PALMON;
    properties->video.windowSize            = P_VIDEO_SIZEX2;
    properties->video.windowSizeInitial     = properties->video.windowSize;
    properties->video.windowSizeChanged     = 0;
    properties->video.windowX               = -1;
    properties->video.windowY               = -1;
    properties->video.frameSkip             = 0;
    properties->video.fullscreen.width      = 640;
    properties->video.fullscreen.height     = 480;
    properties->video.fullscreen.bitDepth   = 32;
    properties->video.maximizeIsFullscreen  = 1;
    properties->video.deInterlace           = 1;
    properties->video.blendFrames           = 0;
    properties->video.horizontalStretch     = 1;
    properties->video.verticalStretch       = 0;
    properties->video.contrast              = 100;
    properties->video.brightness            = 100;
    properties->video.saturation            = 100;
    properties->video.gamma                 = 100;
    properties->video.scanlinesEnable       = 0;
    properties->video.colorSaturationEnable = 0;
    properties->video.scanlinesPct          = 92;
    properties->video.colorSaturationWidth  = 2;
    properties->video.detectActiveMonitor   = 1;
    properties->video.captureFps            = 60;
    properties->video.captureSize           = 1;
    
    properties->videoIn.disabled            = 0;
    properties->videoIn.inputIndex          = 0;
    properties->videoIn.inputName[0]        = 0;

    properties->sound.bufSize               = 100;
    properties->sound.stabilizeDSoundTiming = 1;
    
    properties->sound.stereo = 1;
    properties->sound.masterVolume = 75;
    properties->sound.masterEnable = 1;
    properties->sound.chip.enableYM2413 = 1;
    properties->sound.chip.enableY8950 = 1;
    properties->sound.chip.enableMoonsound = 1;
    properties->sound.chip.moonsoundSRAMSize = 640;
    
    properties->sound.chip.ym2413Oversampling = 1;
    properties->sound.chip.y8950Oversampling = 1;
    properties->sound.chip.moonsoundOversampling = 1;

    properties->sound.mixerChannel[MIXER_CHANNEL_PSG].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_PSG].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_PSG].volume = 100;

    properties->sound.mixerChannel[MIXER_CHANNEL_SCC].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_SCC].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_SCC].volume = 100;

    properties->sound.mixerChannel[MIXER_CHANNEL_MSXMUSIC].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_MSXMUSIC].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_MSXMUSIC].volume = 95;

    properties->sound.mixerChannel[MIXER_CHANNEL_MSXAUDIO].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_MSXAUDIO].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_MSXAUDIO].volume = 95;

    properties->sound.mixerChannel[MIXER_CHANNEL_MOONSOUND].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_MOONSOUND].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_MOONSOUND].volume = 95;

    properties->sound.mixerChannel[MIXER_CHANNEL_YAMAHA_SFG].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_YAMAHA_SFG].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_YAMAHA_SFG].volume = 95;

    properties->sound.mixerChannel[MIXER_CHANNEL_PCM].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_PCM].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_PCM].volume = 95;

    properties->sound.mixerChannel[MIXER_CHANNEL_IO].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_IO].pan = 60;
    properties->sound.mixerChannel[MIXER_CHANNEL_IO].volume = 50;

    properties->sound.mixerChannel[MIXER_CHANNEL_MIDI].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_MIDI].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_MIDI].volume = 90;

    properties->sound.mixerChannel[MIXER_CHANNEL_KEYBOARD].enable = 1;
    properties->sound.mixerChannel[MIXER_CHANNEL_KEYBOARD].pan = 50;
    properties->sound.mixerChannel[MIXER_CHANNEL_KEYBOARD].volume = 65;
    
    properties->sound.YkIn.type               = P_MIDI_NONE;
    properties->sound.YkIn.name[0]            = 0;
    strcpy(properties->sound.YkIn.fileName, "midiin.dat");
    properties->sound.YkIn.desc[0]            = 0;
    properties->sound.YkIn.channel            = 0;
    properties->sound.MidiIn.type             = P_MIDI_NONE;
    properties->sound.MidiIn.name[0]          = 0;
    strcpy(properties->sound.MidiIn.fileName, "midiin.dat");
    properties->sound.MidiIn.desc[0]          = 0;
    properties->sound.MidiOut.type            = P_MIDI_NONE;
    properties->sound.MidiOut.name[0]         = 0;
    strcpy(properties->sound.MidiOut.fileName, "midiout.dat");
    properties->sound.MidiOut.desc[0]         = 0;
    properties->sound.MidiOut.mt32ToGm        = 0;
    
    properties->joystick.POV0isAxes    = 0;
    
#ifdef WII
    // Use joystick by default
    strcpy(properties->joy1.type, "joystick");
    properties->joy1.typeId            = JOYSTICK_PORT_JOYSTICK;
    properties->joy1.autofire          = 0;
    
    strcpy(properties->joy2.type, "joystick");
    properties->joy2.typeId            = JOYSTICK_PORT_JOYSTICK;
    properties->joy2.autofire          = 0;
#else
    strcpy(properties->joy1.type, "none");
    properties->joy1.typeId            = 0;
    properties->joy1.autofire          = 0;
    
    strcpy(properties->joy2.type, "none");
    properties->joy2.typeId            = 0;
    properties->joy2.autofire          = 0;
#endif

    properties->keyboard.configFile[0] = 0;
    properties->keyboard.enableKeyboardQuirk = 1;

    if (kbdLang == P_KBD_JAPANESE) {
        strcpy(properties->keyboard.configFile, "blueMSX Japanese Default");
    }

    properties->nowind.enableDos2 = 0;
    properties->nowind.enableOtherDiskRoms = 0;
    properties->nowind.enablePhantomDrives = 1;
    properties->nowind.partitionNumber = 0xff;
    properties->nowind.ignoreBootFlag = 0;

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        properties->media.carts[i].fileName[0] = 0;
        properties->media.carts[i].fileNameInZip[0] = 0;
        properties->media.carts[i].directory[0] = 0;
        properties->media.carts[i].extensionFilter = 0;
        properties->media.carts[i].type = 0;
    }

    for (i = 0; i < PROP_MAX_DISKS; i++) {
        properties->media.disks[i].fileName[0] = 0;
        properties->media.disks[i].fileNameInZip[0] = 0;
        properties->media.disks[i].directory[0] = 0;
        properties->media.disks[i].extensionFilter = 0;
        properties->media.disks[i].type = 0;
    }

    for (i = 0; i < PROP_MAX_TAPES; i++) {
        properties->media.tapes[i].fileName[0] = 0;
        properties->media.tapes[i].fileNameInZip[0] = 0;
        properties->media.tapes[i].directory[0] = 0;
        properties->media.tapes[i].extensionFilter = 0;
        properties->media.tapes[i].type = 0;
    }
    
    properties->cartridge.defDir[0]    = 0;
    properties->cartridge.defDirSEGA[0]   = 0;
    properties->cartridge.defDirCOLECO[0] = 0;
    properties->cartridge.defDirSVI[0] = 0;
    properties->cartridge.defaultType  = ROM_UNKNOWN;
    properties->cartridge.autoReset    = 1;
    properties->cartridge.quickStartDrive = 0;

    properties->diskdrive.defDir[0]    = 0;
    properties->diskdrive.defHdDir[0]  = 0;
    properties->diskdrive.autostartA   = 0;
    properties->diskdrive.quickStartDrive = 0;
    properties->diskdrive.cdromMethod     = P_CDROM_DRVNONE;
    properties->diskdrive.cdromDrive      = 0;

    properties->cassette.defDir[0]       = 0;
    properties->cassette.showCustomFiles = 1;
    properties->cassette.readOnly        = 1;
    properties->cassette.rewindAfterInsert = 0;

    properties->ports.Lpt.type           = P_LPT_NONE;
    properties->ports.Lpt.emulation      = P_LPT_MSXPRN;
    properties->ports.Lpt.name[0]        = 0;
    strcpy(properties->ports.Lpt.fileName, "printer.dat");
    properties->ports.Lpt.portName[0]    = 0;
    
    properties->ports.Com.type           = P_COM_NONE;
    properties->ports.Com.name[0]        = 0;
    strcpy(properties->ports.Com.fileName, "uart.dat");
    properties->ports.Com.portName[0]    = 0;

    properties->ports.Eth.ethIndex       = -1;
    properties->ports.Eth.disabled       = 0;
    strcpy(properties->ports.Eth.macAddress, "00:00:00:00:00:00");
}

#define ROOT_ELEMENT "config"

static Properties* globalProperties = NULL;

Properties* propGetGlobalProperties(void)
{
    return globalProperties;
}

Properties* propCreate(int useDefault, int langType, PropKeyboardLanguage kbdLang, int syncMode, const char* themeName) 
{
    Properties* properties;

    properties = malloc(sizeof(Properties));

    if (globalProperties == NULL)
        globalProperties = properties;

    propInitDefaults(properties, langType, kbdLang, syncMode, themeName);

#if !defined(WII) && !defined(__LIBRETRO__)
    // Verify machine name
    {
        int foundMachine = 0;
		ArrayListIterator *iterator;
        ArrayList *machineList;
		
		machineList = arrayListCreate();
        machineFillAvailable(machineList, 1);
        
        iterator = arrayListCreateIterator(machineList);
        while (arrayListCanIterate(iterator))
        {
            char *machineName = (char *)arrayListIterate(iterator);
            if (strcmp(machineName, properties->emulation.machineName) == 0)
            {
                foundMachine = 1;
                break;
            }
        }
        arrayListDestroyIterator(iterator);

        if (!foundMachine)
        {
            if (arrayListGetSize(machineList) > 0)
                strcpy(properties->emulation.machineName, (char *)arrayListGetObject(machineList, 0));
            
            iterator = arrayListCreateIterator(machineList);
            while (arrayListCanIterate(iterator))
            {
                char *machineName = (char *)arrayListIterate(iterator);
                if (strcmp(machineName, "MSX2") == 0)
                {
                    strcpy(properties->emulation.machineName, machineName);
                    foundMachine = 1;
                }
                
                if (!foundMachine && strncmp(machineName, "MSX2", 4))
                {
                    strcpy(properties->emulation.machineName, machineName);
                    foundMachine = 1;
                }
            }
            arrayListDestroyIterator(iterator);
        }
        
        arrayListDestroy(machineList);
    }
#endif
    return properties;
}


void propDestroy(Properties* properties)
{
    free(properties);
}

 

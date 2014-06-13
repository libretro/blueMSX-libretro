/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageEnglish.h,v $
**
** $Revision: 1.72 $
**
** $Date: 2006/06/27 15:43:08 $
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
#ifndef LANGUAGE_ENGLISH_H
#define LANGUAGE_ENGLISH_H

#include "LanguageStrings.h"
 
void langInitEnglish(LanguageStrings* ls) 
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "Chinese Simplified";
    ls->langChineseTraditional  = "Chinese Traditional";
    ls->langDutch               = "Dutch";
    ls->langEnglish             = "English";
    ls->langFinnish             = "Finnish";
    ls->langFrench              = "French";
    ls->langGerman              = "German";
    ls->langItalian             = "Italian";
    ls->langJapanese            = "Japanese";
    ls->langKorean              = "Korean";
    ls->langPolish              = "Polish";
    ls->langPortuguese          = "Portuguese";
    ls->langSpanish             = "Spanish";
    ls->langSwedish             = "Swedish";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "Device:";
    ls->textFilename            = "Filename:";
    ls->textFile                = "File";
    ls->textNone                = "None";
    ls->textUnknown             = "Unknown";


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle            = "blueMSX - Warning";
    ls->warningDiscardChanges   = "Do you want to discard changes?";
    ls->warningOverwriteFile    = "Do you want to overwrite the file:";
    ls->errorTitle              = "blueMSX - Error";
    ls->errorEnterFullscreen    = "Failed to enter fullscreen mode.           \n";
    ls->errorDirectXFailed      = "Failed to create DirectX objects.           \nUsing GDI instead.\nCheck Video properties.";
    ls->errorNoRomInZip         = "Could not locate a .rom file in the zip archive.";
    ls->errorNoDskInZip         = "Could not locate a .dsk file in the zip archive.";
    ls->errorNoCasInZip         = "Could not locate a .cas file in the zip archive.";
    ls->errorNoHelp             = "Could not locate the blueMSX help file.";
    ls->errorStartEmu           = "Failed to Start MSX emulator.";
    ls->errorPortableReadonly   = "Portable device is readonly";


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM image";
    ls->fileAll                 = "All Files";
    ls->fileCpuState            = "CPU state";
    ls->fileDisk                = "Disk Image";
    ls->fileCas                 = "Tape Image";


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- no recent files -";
    ls->menuInsert              = "Insert";
    ls->menuEject               = "Eject";

    ls->menuCartGameReader      = "Game Reader";
    ls->menuCartIde             = "IDE";
    ls->menuCartBeerIde         = "Beer";
    ls->menuCartGIde            = "GIDE";
    ls->menuCartSunriseIde      = "Sunrise";
    ls->menuCartSCC             = "SCC Cartridge";
    ls->menuCartSCCPlus         = "SCC+ Cartridge";
    ls->menuCartFMPac           = "FM-PAC Cartridge";
    ls->menuCartPac             = "PAC Cartridge";
    ls->menuCartHBI55           = "Sony HBI-55 Cartridge";
    ls->menuCartInsertSpecial   = "Insert Special";
    ls->menuCartMegaRam         = "MegaRAM";
    ls->menuCartExternalRam     = "External RAM";

    ls->menuDiskInsertNew       = "Insert New Disk Image";
    ls->menuDiskDirInsert       = "Insert Directory";
    ls->menuDiskAutoStart       = "Reset After Insert";
    ls->menuCartAutoReset       = "Reset After Insert/Remove";

    ls->menuCasRewindAfterInsert= "Rewind After Insert";
    ls->menuCasUseReadOnly      = "Use Cassette Image Read Only";
    ls->lmenuCasSaveAs          = "Save Cassette Image As...";
    ls->menuCasSetPosition      = "Set Position";
    ls->menuCasRewind           = "Rewind";

    ls->menuPrnFormfeed         = "Form Feed";

    ls->menuZoomNormal          = "Small Window";
    ls->menuZoomDouble          = "Normal Window";
    ls->menuZoomFullscreen      = "Fullscreen";
    
    ls->menuPropsEmulation      = "Emulation";
    ls->menuPropsVideo          = "Video";
    ls->menuPropsSound          = "Sound";
    ls->menuPropsControls       = "Controls";
    ls->menuPropsPerformance    = "Performance";
    ls->menuPropsSettings       = "Settings";
    ls->menuPropsFile           = "Files";
    ls->menuPropsLanguage       = "Language";
    ls->menuPropsPorts          = "Ports";
    
    ls->menuVideoSource         = "Video Out Source";
    ls->menuVideoSourceDefault  = "No Video Out Source Connected";
    ls->menuVideoChipAutodetect = "Autodetect Video Chip";
    ls->menuVideoInSource       = "Video In Source";
    ls->menuVideoInBitmap       = "Bitmap File";

    ls->menuHelpHelp            = "Help Topics";
    ls->menuHelpAbout           = "About blueMSX";

    ls->menuFileCart            = "Cartridge Slot";
    ls->menuFileDisk            = "Disk Drive";
    ls->menuFileCas             = "Cassette";
    ls->menuFilePrn             = "Printer";
    ls->menuFileLoadState       = "Load CPU State";
    ls->menuFileSaveState       = "Save CPU State";
    ls->menuFileQLoadState      = "Quick Load State";
    ls->menuFileQSaveState      = "Quick Save State";
    ls->menuFileCaptureAudio    = "Capture Audio";
    ls->menuFileScreenShot      = "Save Screenshot";
    ls->menuFileExit            = "Exit";

    ls->menuFileHarddisk        = "Hard Disk";
    ls->menuFileHarddiskNoPesent= "No Controllers Present";

    ls->menuRunRun              = "Run";
    ls->menuRunPause            = "Pause";
    ls->menuRunStop             = "Stop";
    ls->menuRunSoftReset        = "Soft Reset";
    ls->menuRunHardReset        = "Hard Reset";
    ls->menuRunCleanReset       = "Complete Reset";

    ls->menuToolsMachine        = "Machine Editor";
    ls->menuToolsShortcuts      = "Shortcuts Editor";
    ls->menuToolsKeyboard       = "Controllers / Keyboard Editor";
    ls->menuToolsMixer          = "Mixer";
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "File";
    ls->menuRun                 = "Emulation";
    ls->menuWindow              = "Window";
    ls->menuOptions             = "Options";
    ls->menuTools               = "Tools";
    ls->menuHelp                = "Help";
    

    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Open";
    ls->dlgCancel               = "Cancel";
    ls->dlgSave                 = "Save";
    ls->dlgSaveAs               = "Save As...";
    ls->dlgRun                  = "Run";
    ls->dlgClose                = "Close";

    ls->dlgLoadRom              = "blueMSX - Select a rom image to load";
    ls->dlgLoadDsk              = "blueMSX - Select a dsk image to load";
    ls->dlgLoadCas              = "blueMSX - Select a cas image to load";
    ls->dlgLoadRomDskCas        = "blueMSX - Select a rom, dsk, or cas file to load";
    ls->dlgLoadRomDesc          = "Choose a rom image to load:";
    ls->dlgLoadDskDesc          = "Choose a disk image to load:";
    ls->dlgLoadCasDesc          = "Choose a tape image to load:";
    ls->dlgLoadRomDskCasDesc    = "Choose a rom, disk, or tape image to load:";
    ls->dlgLoadState            = "Load state";
    ls->dlgSaveState            = "Save state as...";
    ls->dlgSaveCassette         = "blueMSX - Save Tape Image";
    ls->dlgInsertRom1           = "Insert ROM cartridge into slot 1";
    ls->dlgInsertRom2           = "Insert ROM cartridge into slot 2";
    ls->dlgInsertDiskA          = "Insert disk image into drive A";
    ls->dlgInsertDiskB          = "Insert disk image into drive B";
    ls->dlgInsertHarddisk       = "Insert Hard Disk";
    ls->dlgInsertCas            = "Insert cassette tape";
    ls->dlgRomType              = "Rom Type:";

    ls->dlgTapeTitle            = "blueMSX - Tape Position";
    ls->dlgTapeFrameText        = "Tape Position";
    ls->dlgTapeCurrentPos       = "Current Position";
    ls->dlgTapeTotalTime        = "Total Time";
    ls->dlgTapeSetPosText       = "Tape Position:";
    ls->dlgTapeCustom           = "Show Custom Files";
    ls->dlgTabPosition          = "Position";
    ls->dlgTabType              = "Type";
    ls->dlgTabFilename          = "Filename";
    ls->dlgZipReset             = "Reset after insert";

    ls->dlgAboutTitle           = "blueMSX - About";

    ls->dlgLangLangText         = "Choose the language that blueMSX will use";
    ls->dlgLangLangTitle        = "blueMSX - Language";

    ls->dlgAboutAbout           = "ABOUT\r\n====";
    ls->dlgAboutVersion         = "Version:";
    ls->dlgAboutBuildNumber     = "Build:";
    ls->dlgAboutBuildDate       = "Date:";
    ls->dlgAboutCreat           = "Created by Daniel Vik";
    ls->dlgAboutDevel           = "DEVELOPERS\r\n========";
    ls->dlgAboutThanks          = "SPECIAL THANKS TO\r\n============";
    ls->dlgAboutLisence         = "LICENSE\r\n"
                                  "======\r\n\r\n"
                                  "This software is provided 'as-is', without any express or implied "
                                  "warranty. In no event will the author(s) be held liable for any damages "
                                  "arising from the use of this software.\r\n\r\n"
                                  "Visit www.bluemsx.com for more details.";

    ls->dlgSavePreview          = "Show Preview";
    ls->dlgSaveDate             = "Time Saved:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Properties";
    ls->propEmulation           = "Emulation";
    ls->propVideo               = "Video";
    ls->propSound               = "Sound";
    ls->propControls            = "Controls";
    ls->propPerformance         = "Performance";
    ls->propSettings            = "Settings";
    ls->propFile                = "Files";
    ls->propPorts               = "Ports";
    
    ls->propEmuGeneralGB        = "General ";
    ls->propEmuFamilyText       = "MSX machine:";
    ls->propEmuMemoryGB         = "Memory ";
    ls->propEmuRamSizeText      = "RAM size:";
    ls->propEmuVramSizeText     = "VRAM size:";
    ls->propEmuSpeedGB          = "Emulation Speed ";
    ls->propEmuSpeedText        = "Emulation Speed:";
    ls->propEmuFrontSwitchGB    = "Panasonic Switches ";
    ls->propEmuFrontSwitch      = " Front Switch";
    ls->propEmuFdcTiming        = "Disable Floppy Drive Timing";
    ls->propEmuPauseSwitch      = " Pause Switch";
    ls->propEmuAudioSwitch      = " MSX-AUDIO cartridge switch";
    ls->propVideoFreqText       = "Video Frequency:";
    ls->propVideoFreqAuto       = "Auto";
    ls->propSndOversampleText   = "Oversample:";
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";
    ls->propSndMidiInGB         = "MIDI In ";
    ls->propSndMidiOutGB        = "MIDI Out ";
    ls->propSndMidiChannel      = "MIDI Channel:";
    ls->propSndMidiAll          = "All";

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Monitor type:";
    ls->propMonEmuText          = "Monitor emulation:";
    ls->propVideoTypeText       = "Video type:";
    ls->propWindowSizeText      = "Window size:";
    ls->propMonHorizStretch     = " Horizontal Stretch";
    ls->propMonVertStretch      = " Vertical Stretch";
    ls->propMonDeInterlace      = " De-interlace";
    ls->propBlendFrames         = " Blend consecutive frames";
    ls->propMonBrightness       = "Brightness:";
    ls->propMonContrast         = "Contrast:";
    ls->propMonSaturation       = "Saturation:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Scanlines:";
    ls->propMonColorGhosting    = "RF-Modulator";
    ls->propMonEffectsGB        = "Effects ";

    ls->propPerfVideoDrvGB      = "Video Driver ";
    ls->propPerfVideoDispDrvText= "Display driver:";
    ls->propPerfFrameSkipText   = "Frame skipping:";
    ls->propPerfAudioDrvGB      = "Audio Driver ";
    ls->propPerfAudioDrvText    = "Sound driver:";
    ls->propPerfAudioBufSzText  = "Sound buffer size:";
    ls->propPerfEmuGB           = "Emulation ";
    ls->propPerfSyncModeText    = "SYNC Mode:";
    ls->propFullscreenResText   = "Fullscreen Resolution:";

    ls->propSndChipEmuGB        = "Sound Chip Emulation ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound        = " Moonsound";
    ls->propSndMt32ToGm         = " Map MT-32 instruments to General MIDI";

    ls->propPortsLptGB          = "Parallel port ";
    ls->propPortsComGB          = "Serial ports ";
    ls->propPortsLptText        = "Port:";
    ls->propPortsCom1Text       = "Port 1:";
    ls->propPortsNone           = "None";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC";
    ls->propPortsFile           = "Print to File";
    ls->propPortsComFile        = "Send to File";
    ls->propPortsOpenLogFile    = "Open Log File";
    ls->propPortsEmulateMsxPrn  = "Emulation:";

    ls->propSetFileHistoryGB    = "File History ";
    ls->propSetFileHistorySize  = "Number of items in File History:";
    ls->propSetFileHistoryClear = "Clear History";
    ls->propFileTypes           = " Register file types with blueMSX (.rom, .dsk, .cas, .sta)";
    ls->propWindowsEnvGB        = "Windows Environment "; 
    ls->propSetScreenSaver      = " Disable screen saver when blueMSX is running";
    ls->propDisableWinKeys      = " Automatic MSX function for Windows menu keys"; 
    ls->propPriorityBoost       = " Boost the priority of blueMSX";
    ls->propScreenshotPng       = " Use Portable Network Graphics (.png) screenshots";
    ls->propClearHistory        = "Are you sure you want to clear the file history ?";
    ls->propOpenRomGB           = "Open Rom Dialog ";
    ls->propDefaultRomType      = "Default Rom Type:";
    ls->propGuessRomType        = "Guess Rom Type";

    ls->propSettDefSlotGB       = "Drag and Drop ";
    ls->propSettDefSlots        = "Insert Rom Into:";
    ls->propSettDefSlot         = " Slot";
    ls->propSettDefDrives       = "Insert Diskette Into:";
    ls->propSettDefDrive        = " Drive";

    ls->propThemeGB             = "Theme ";
    ls->propTheme               = "Theme:";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Color";
    ls->enumVideoMonGrey        = "Black and white";
    ls->enumVideoMonGreen       = "Green";
    ls->enumVideoMonAmber       = "Amber";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "None";
    ls->enumVideoEmuYc          = "Y/C cable (sharp)";
    ls->enumVideoEmuMonitor     = "Monitor";
    ls->enumVideoEmuYcBlur      = "Noisy Y/C cable (sharp)";
    ls->enumVideoEmuComp        = "Composite (blurry)";
    ls->enumVideoEmuCompBlur    = "Noisy Composite (blurry)";
    ls->enumVideoEmuScale2x     = "Scale 2x";
    ls->enumVideoEmuHq2x        = "Hq2x";

    ls->enumVideoSize1x         = "Normal - 320x200";
    ls->enumVideoSize2x         = "Double - 640x400";
    ls->enumVideoSizeFullscreen = "Fullscreen";

    ls->enumVideoDrvDirectDrawHW= "DirectDraw HW accel."; 
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "None";
    ls->enumVideoFrameskip1     = "1 frame";
    ls->enumVideoFrameskip2     = "2 frames";
    ls->enumVideoFrameskip3     = "3 frames";
    ls->enumVideoFrameskip4     = "4 frames";
    ls->enumVideoFrameskip5     = "5 frames";

    ls->enumSoundDrvNone        = "No Sound";
    ls->enumSoundDrvWMM         = "WMM driver";
    ls->enumSoundDrvDirectX     = "DirectX driver";

    ls->enumEmuSync1ms          = "Sync on MSX refresh";
    ls->enumEmuSyncAuto         = "Auto (fast)";
    ls->enumEmuSyncNone         = "None";
    ls->enumEmuSyncVblank       = "Sync to PC Vertical Blank";
    ls->enumEmuAsyncVblank      = "Asynchronous PC Vblank";             // New in 2.5

    ls->enumControlsJoyNone     = "None";
    ls->enumControlsJoyMouse    = "Mouse";
    ls->enumControlsJoyTetris2Dongle = "Tetris 2 Dongle";
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey Dongle";
    ls->enumControlsJoy2Button = "2-button Joystick";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision Joystick";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 Double Sided, 9 Sectors";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 Double Sided, 8 Sectors";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 Single Sided, 9 Sectors";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 Single Sided, 8 Sectors";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 Double Sided";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 Single Sided";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle               = "blueMSX - Machine Configuration Editor";
    ls->confConfigText          = "Profile Name";
    ls->confSlotLayout          = "Slot Layout";
    ls->confMemory              = "Memory";
    ls->confChipEmulation       = "Chip Emulation";
    ls->confChipExtras          = "Extras";

    ls->confOpenRom             = "Open ROM image";
    ls->confSaveTitle           = "blueMSX - Save Configuration";
    ls->confSaveText            = "Do you want to overwrite the machine configuration:";
    ls->confSaveAsTitle         = "Save Configuration As...";
    ls->confSaveAsMachineName   = "Machine Name:";
    ls->confDiscardTitle        = "blueMSX - Configuration";
    ls->confExitSaveTitle       = "blueMSX - Exit Configuration Editor";
    ls->confExitSaveText        = "Do you want to discard changes to the current configuration?";

    ls->confSlotLayoutGB        = "Slot Layout ";
    ls->confSlotExtSlotGB       = "External Slots ";
    ls->confBoardGB             = "Board ";
    ls->confBoardText           = "Board Type:";
    ls->confSlotPrimary         = "Primary";
    ls->confSlotExpanded        = "Expanded (four subslots)";

    ls->confSlotCart            = "Cartridge";
    ls->confSlot                = "Slot";
    ls->confSubslot             = "Subslot";

    ls->confMemAdd              = "Add...";
    ls->confMemEdit             = "Edit...";
    ls->confMemRemove           = "Remove";
    ls->confMemSlot             = "Slot";
    ls->confMemAddresss         = "Address";
    ls->confMemType             = "Type";
    ls->confMemRomImage         = "Rom Image";

    ls->confChipVideoGB          = "Video ";
    ls->confChipVideoChip        = "Video Chip:";
    ls->confChipVideoRam         = "Video RAM:";
    ls->confChipSoundGB          = "Sound ";

    ls->confCmosGB               = "Clock Chip ";
    ls->confCmosEnable           = " Enable Clock Chip";
    ls->confCmosBattery          = " Use Charged Battery";

    ls->confCpuFreqGB            = "CPU Frequency ";
    ls->confZ80FreqText          = "Z80 Frequency:";
    ls->confR800FreqText         = "R800 Frequency:";
    ls->confFdcGB                = "Floppy Disk Controller ";
    ls->confCFdcNumDrivesText    = "Number of Drives:";

    ls->confEditMemTitle         = "blueMSX - Edit Mapper";
    ls->confEditMemGB            = "Mapper Details ";
    ls->confEditMemType          = "Type:";
    ls->confEditMemFile          = "File:";
    ls->confEditMemAddress       = "Address";
    ls->confEditMemSize          = "Size";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Action";
    ls->shortcutDescription     = "Shortcut";

    ls->shortcutSaveConfig      = "blueMSX - Save Configuration";
    ls->shortcutOverwriteConfig = "Do you want to overwrite the shortcut configuration:";
    ls->shortcutExitConfig      = "blueMSX - Exit Shortcut Editor";
    ls->shortcutDiscardConfig   = "Do you want to discard changes to the current configuration?";
    ls->shortcutSaveConfigAs    = "blueMSX - Save Shortcut Configuration As...";
    ls->shortcutConfigName      = "Config Name:";
    ls->shortcutNewProfile      = "< New Profile >";
    ls->shortcutConfigTitle     = "blueMSX - Shortcut Mapping Editor";
    ls->shortcutAssign          = "Assign";
    ls->shortcutPressText       = "Press shortcut key(s):";
    ls->shortcutScheme          = "Mapping Scheme:";
    ls->shortcutCartInsert1     = "Insert Cartridge 1";
    ls->shortcutCartRemove1     = "Remove Cartridge 1";
    ls->shortcutCartInsert2     = "Insert Cartridge 2";
    ls->shortcutCartRemove2     = "Remove Cartridge 2";
    ls->shortcutSpecialMenu1    = "Show Special Rom Menu for Cartridge 1";
    ls->shortcutSpecialMenu2    = "Show Special Rom Menu for Cartridge 2";
    ls->shortcutCartAutoReset   = "Reset Emulator when Cartridge is Inserted";
    ls->shortcutDiskInsertA     = "Insert Diskette A";
    ls->shortcutDiskDirInsertA  = "Insert Directory as Diskette A";
    ls->shortcutDiskRemoveA     = "Eject Diskette A";
    ls->shortcutDiskChangeA     = "Quick change Diskette A";
    ls->shortcutDiskAutoResetA  = "Reset Emulator when Diskette A is Inserted";
    ls->shortcutDiskInsertB     = "Insert Diskette B";
    ls->shortcutDiskDirInsertB  = "Insert Directory as Diskette B";
    ls->shortcutDiskRemoveB     = "Eject Diskette B";
    ls->shortcutCasInsert       = "Insert Cassette";
    ls->shortcutCasEject        = "Eject Cassette";
    ls->shortcutCasAutorewind   = "Toggle Auto-rewind on Cassette";
    ls->shortcutCasReadOnly     = "Toggle Read-only Cassette";
    ls->shortcutCasSetPosition  = "Set Tape position";
    ls->shortcutCasRewind       = "Rewind Cassette";
    ls->shortcutCasSave         = "Save Cassette Image";
    ls->shortcutPrnFormFeed     = "Printer Form Feed";
    ls->shortcutCpuStateLoad    = "Load CPU state";
    ls->shortcutCpuStateSave    = "Save CPU state";
    ls->shortcutCpuStateQload   = "Quick load CPU state";
    ls->shortcutCpuStateQsave   = "Quick save CPU state";
    ls->shortcutAudioCapture    = "Start/stop audio capture";
    ls->shortcutScreenshotOrig  = "Screenshot capture";
    ls->shortcutScreenshotSmall = "Small unfiltered screenshot capture";
    ls->shortcutScreenshotLarge = "Large unfiltered screenshot capture";
    ls->shortcutQuit            = "Quit blueMSX";
    ls->shortcutRunPause        = "Run/Pause emulation";
    ls->shortcutStop            = "Stop emulation";
    ls->shortcutResetHard       = "Hard Reset";
    ls->shortcutResetSoft       = "Soft Reset";
    ls->shortcutResetClean      = "Complete Reset";
    ls->shortcutSizeSmall       = "Set small window size";
    ls->shortcutSizeNormal      = "Set normal window size";
    ls->shortcutSizeFullscreen  = "Set fullscreen";
    ls->shortcutSizeMinimized   = "Minimize window";
    ls->shortcutToggleFullscren = "Toggle fullscreen";
    ls->shortcutVolumeIncrease  = "Increase Volume";
    ls->shortcutVolumeDecrease  = "Decrease Volume";
    ls->shortcutVolumeMute      = "Mute Volume";
    ls->shortcutVolumeStereo    = "Toggle mono/stereo";
    ls->shortcutSwitchMsxAudio  = "Toggle MSX-AUDIO switch";
    ls->shortcutSwitchFront     = "Toggle Panasonic front switch";
    ls->shortcutSwitchPause     = "Toggle pause switch";
    ls->shortcutToggleMouseLock = "Toggle mouse lock";
    ls->shortcutEmuSpeedMax     = "Max emulation speed";
    ls->shortcutEmuSpeedToggle  = "Toggle max emulation speed";
    ls->shortcutEmuSpeedNormal  = "Normal emulation speed";
    ls->shortcutEmuSpeedInc     = "Increase emulation speed";
    ls->shortcutEmuSpeedDec     = "Decrease emulation speed";
    ls->shortcutThemeSwitch     = "Switch theme";
    ls->shortcutShowEmuProp     = "Show Emulation Properties";
    ls->shortcutShowVideoProp   = "Show Video Properties";
    ls->shortcutShowAudioProp   = "Show Audio Properties";
    ls->shortcutShowCtrlProp    = "Show Controls Properties";
    ls->shortcutShowPerfProp    = "Show Performance Properties";
    ls->shortcutShowSettProp    = "Show Settings Properties";
    ls->shortcutShowPorts       = "Show Ports Properties";
    ls->shortcutShowLanguage    = "Show Language Dialog";
    ls->shortcutShowMachines    = "Show Machine Editor";
    ls->shortcutShowShortcuts   = "Show Shortcuts Editor";
    ls->shortcutShowKeyboard    = "Show Controllers / Keyboard Editor";
    ls->shortcutShowMixer       = "Show Mixer";
    ls->shortcutShowDebugger    = "Show Debugger";
    ls->shortcutShowTrainer     = "Show Trainer";
    ls->shortcutShowHelp        = "Show Help Dialog";
    ls->shortcutShowAbout       = "Show About Dialog";    
    ls->shortcutShowFiles       = "Show Files Properties";
    ls->shortcutToggleSpriteEnable = "Show/Hide Sprites";
    ls->shortcutToggleFdcTiming = "Enable/Disable Floppy Drive Timing";
    ls->shortcutToggleCpuTrace  = "Enable/Disable CPU Trace";


    //----------------------
    // Keyboard config lines
    //----------------------    
 
    ls->keyconfigSelectedKey    = "Selected Key:";
    ls->keyconfigMappedTo       = "Mapped To:";
    ls->keyconfigMappingScheme  = "Mapping Scheme:";

    
    //----------------------
    // Rom type lines
    //----------------------

    ls->romTypeStandard         = "Standard";
    ls->romTypeMsxdos2          = "MSXDOS 2";
    ls->romTypeKonamiScc        = "Konami SCC";
    ls->romTypeKonami           = "Konami";
    ls->romTypeAscii8           = "ASCII 8";
    ls->romTypeAscii16          = "ASCII 16";
    ls->romTypeGameMaster2      = "Game Master 2 (SRAM)";
    ls->romTypeAscii8Sram       = "ASCII 8 (SRAM)";
    ls->romTypeAscii16Sram      = "ASCII 16 (SRAM)";
    ls->romTypeRtype            = "R-Type";
    ls->romTypeCrossblaim       = "Cross Blaim";
    ls->romTypeHarryFox         = "Harry Fox";
    ls->romTypeMajutsushi       = "Konami Majutsushi";
    ls->romTypeZenima80         = "Zemina 80 in 1";
    ls->romTypeZenima90         = "Zemina 90 in 1";
    ls->romTypeZenima126        = "Zemina 126 in 1";
    ls->romTypeScc              = "SCC";
    ls->romTypeSccPlus          = "SCC+";
    ls->romTypeSnatcher         = "The Snatcher";
    ls->romTypeSdSnatcher       = "SD Snatcher";
    ls->romTypeSccMirrored      = "SCC mirrored";
    ls->romTypeSccExtended      = "SCC extended";
    ls->romTypeFmpac            = "FMPAC (SRAM)";
    ls->romTypeFmpak            = "FMPAK";
    ls->romTypeKonamiGeneric    = "Konami Generic";
    ls->romTypeSuperPierrot     = "Super Pierrot";
    ls->romTypeMirrored         = "Mirrored ROM";
    ls->romTypeNormal           = "Normal ROM";
    ls->romTypeDiskPatch        = "Normal + Disk Patch";
    ls->romTypeCasPatch         = "Normal + Cassette Patch";
    ls->romTypeTc8566afFdc      = "TC8566AF Disk Controller";
    ls->romTypeMicrosolFdc      = "Microsol Disk Controller";
    ls->romTypeNationalFdc      = "National Disk Controller";
    ls->romTypePhilipsFdc       = "Philips Disk Controller";
    ls->romTypeSvi738Fdc        = "SVI-738 Disk Controller";
    ls->romTypeMappedRam        = "Mapped RAM";
    ls->romTypeMirroredRam1k    = "1kB Mirrored RAM";
    ls->romTypeNormalRam        = "Normal RAM";
    ls->romTypeKanji            = "Kanji";
    ls->romTypeHolyQuran        = "Holy Quran";
    ls->romTypeMatsushitaSram   = "Matsushita SRAM";
    ls->romTypePanasonic16      = "Panasonic 16kB SRAM";
    ls->romTypePanasonic32      = "Panasonic 32kB SRAM";
    ls->romTypeBunsetsu         = "Bunsetsu";
    ls->romTypeJisyo            = "Jisyo";
    ls->romTypeKanji12          = "Kanji12";
    ls->romTypeNationalSram     = "National (SRAM)";
    ls->romTypeS1985            = "S1985";
    ls->romTypeS1990            = "S1990";
    ls->romTypeTurborPause      = "Turbo-R Pause";
    ls->romTypeF4deviceNormal   = "F4 Device Normal";
    ls->romTypeF4deviceInvert   = "F4 Device Inverted";
    ls->romTypeMsxMidi          = "MSX-MIDI";
    ls->romTypeTurborTimer      = "Turbo-R Timer";
    ls->romTypeKoei             = "Koei (SRAM)";
    ls->romTypeBasic            = "Basic ROM";
    ls->romTypeHalnote          = "Halnote";
    ls->romTypeLodeRunner       = "Lode Runner";
    ls->romTypeNormal4000       = "Normal 4000h";
    ls->romTypeNormalC000       = "Normal C000h";
    ls->romTypeKonamiSynth      = "Konami Synthesizer";
    ls->romTypeKonamiKbdMast    = "Konami Keyboard Master";
    ls->romTypeKonamiWordPro    = "Konami Word Pro";
    ls->romTypePac              = "PAC (SRAM)";
    ls->romTypeMegaRam          = "MegaRAM";
    ls->romTypeMegaRam128       = "128kB MegaRAM";
    ls->romTypeMegaRam256       = "256kB MegaRAM";
    ls->romTypeMegaRam512       = "512kB MegaRAM";
    ls->romTypeMegaRam768       = "768kB MegaRAM";
    ls->romTypeMegaRam2mb       = "2MB MegaRAM";
    ls->romTypeExtRam           = "External RAM";
    ls->romTypeExtRam512        = "512kB External RAM";
    ls->romTypeExtRam1mb        = "1MB External RAM";
    ls->romTypeExtRam2mb        = "2MB External RAM";
    ls->romTypeExtRam4mb        = "4MB External RAM";
    ls->romTypeMsxMusic         = "MSX Music";
    ls->romTypeMsxAudio         = "MSX Audio";
    ls->romTypeY8950            = "Y8950";
    ls->romTypeMoonsound        = "Moonsound";
    ls->romTypeSvi328Cart       = "SVI-328 Cartridge";
    ls->romTypeSvi328Fdc        = "SVI-328 Disk Controller";
    ls->romTypeSvi328Prn        = "SVI-328 Printer";
    ls->romTypeSvi328Uart       = "SVI-328 Serial Port";
    ls->romTypeSvi328col80      = "SVI-328 80 Column Card";
    ls->romTypeSvi727col80      = "SVI-727 80 Column Card";
    ls->romTypeColecoCart       = "Coleco Cartridge";
    ls->romTypeSg1000Cart       = "SG-1000 Cartridge";
    ls->romTypeTheCastle        = "The Castle";
    ls->romTypeSonyHbi55        = "Sony HBI-55";
    ls->romTypeMsxPrinter       = "MSX Printer";
    ls->romTypeTurborPcm        = "Turbo-R PCM Chip";
    ls->romTypeGameReader       = "Sunrise GameReader";
    ls->romTypeSunriseIde       = "Sunrise IDE";
    ls->romTypeBeerIde          = "Beer IDE";
    ls->romTypeGide             = "GIDE";
    ls->romTypeVmx80            = "Microsol VMX-80";
    ls->romTypeNms8280Digitiz   = "Philips NMS-8280 Digitizer";
    ls->romTypeHbiV1Digitiz     = "Sony HBI-V1 Digitizer";
    ls->romTypeFmdas            = "F&M Direct Assembler System";
    ls->romTypeSfg01            = "Yamaha SFG-01";
    ls->romTypeSfg05            = "Yamaha SFG-05";
    
    
    //----------------------
    // Debug type lines
    // Note: Only needs translation if debugger is translated
    //----------------------

    ls->dbgMemVisible           = "Visible Memory";
    ls->dbgMemRamNormal         = "Normal";
    ls->dbgMemRamMapped         = "Mapped";
    ls->dbgMemVram              = "VRAM";
    ls->dbgMemYmf278            = "YMF278 Sample RAM";
    ls->dbgMemAy8950            = "AY8950 Sample RAM";
    ls->dbgMemScc               = "Memory";

    ls->dbgCallstack            = "Callstack";

    ls->dbgRegs                 = "Registers";
    ls->dbgRegsCpu              = "CPU Registers";
    ls->dbgRegsYmf262           = "YMF262 Registers";
    ls->dbgRegsYmf278           = "YMF278 Registers";
    ls->dbgRegsAy8950           = "AY8950 Registers";
    ls->dbgRegsYm2413           = "YM2413 Registers";

    ls->dbgDevRamMapper         = "RAM Mapper";
    ls->dbgDevRam               = "RAM";
    ls->dbgDevIdeBeer           = "Beer IDE";
    ls->dbgDevIdeGide           = "GIDE";
    ls->dbgDevF4Device          = "F4 Device";
    ls->dbgDevFmpac             = "FMPAC";
    ls->dbgDevFmpak             = "FMPAK";
    ls->dbgDevKanji             = "Kanji";
    ls->dbgDevKanji12           = "Kanji 12";
    ls->dbgDevKonamiKbd         = "Konami Keyboard Master";
    ls->dbgDevKorean80          = "Korean 80";
    ls->dbgDevKorean90          = "Korean 90";
    ls->dbgDevKorean128         = "Korean 128";
    ls->dbgDevMegaRam           = "Mega RAM";
    ls->dbgDevFdcMicrosol       = "Microsol FDC";
    ls->dbgDevMoonsound         = "Moonsound";
    ls->dbgDevMsxAudio          = "MSX Audio";
    ls->dbgDevMsxAudioMidi      = "MSX Audio MIDI";
    ls->dbgDevMsxMusic          = "MSX Music";
    ls->dbgDevPrinter           = "Printer";
    ls->dbgDevRs232             = "RS232";
    ls->dbgDevS1990             = "S1990";
    ls->dbgDevSfg05             = "Yamaha SFG-05";
    ls->dbgDevHbi55             = "Sony HBI-55";
    ls->dbgDevSviFdc            = "SVI FDC";
    ls->dbgDevSviPrn            = "SVI Printer";
    ls->dbgDevSvi80Col          = "SVI 80 Column";
    ls->dbgDevPcm               = "PCM";
    ls->dbgDevMatsushita        = "Matsushita";
    ls->dbgDevS1985             = "S1985";
    ls->dbgDevCrtc6845          = "CRTC6845";
    ls->dbgDevTms9929A          = "TMS9929A";
    ls->dbgDevTms99x8A          = "TMS99x8A";
    ls->dbgDevV9938             = "V9938";
    ls->dbgDevV9958             = "V9958";
    ls->dbgDevZ80               = "Z80";
    ls->dbgDevMsxMidi           = "MSX MIDI";
    ls->dbgDevPpi               = "PPI";
    ls->dbgDevRtc               = "RTC";
    ls->dbgDevTrPause           = "TR Pause";
    ls->dbgDevAy8910            = "AY8910 PSG";
    ls->dbgDevScc               = "SCC";


    //----------------------
    // Debug type lines
    // Note: Can only be translated to european languages
    //----------------------

    ls->aboutScrollThanksTo     = "Special thanks to: ";
    ls->aboutScrollAndYou       = "and YOU !!!!";
};

#endif

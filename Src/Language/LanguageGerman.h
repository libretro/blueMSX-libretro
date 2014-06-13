/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageGerman.h,v $
**
** $Revision: 1.24 $ 
**
** $Date: 2006/06/26 19:35:54 $
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
#ifndef LANGUAGE_GERMAN_H
#define LANGUAGE_GERMAN_H

#include "LanguageStrings.h"
 
void langInitGerman(LanguageStrings* ls) 
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

    ls->textDevice              = "Typ:";
    ls->textFilename            = "Filename:";
    ls->textFile                = "File";
    ls->textNone                = "Nichts";    
    ls->textUnknown             = "Unknown";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - Warnung";
    ls->warningDiscardChanges   = "Möchten Sie die Änderungen verwerfen?";
    ls->warningOverwriteFile    = "Do you want to overwrite the file:"; // New in 2.5
    ls->errorTitle              = "blueMSX - Fehler";
    ls->errorEnterFullscreen    = "Konnte nicht in den Fullscreen-Modus gehen.           \n";
    ls->errorDirectXFailed      = "DirectX objects konnten nicht erzeugt werden.\nBenutze stattdessen GDI.\nÜberprüfe die Video-Eigenschaften.";
    ls->errorNoRomInZip         = "Konnte kein .rom-File im Zip-Archiv finden.";
    ls->errorNoDskInZip         = "Konnte kein .dsk-File im Zip-Archiv finden.";
    ls->errorNoCasInZip         = "Konnte kein .cas-File im Zip-Archiv finden.";
    ls->errorNoHelp             = "Konnte kein blueMSX-Hilfe-File finden.";
    ls->errorStartEmu           = "Der MSX-Emulator konnte nicht gestartet werde.";
    ls->errorPortableReadonly   = "Portable device is readonly";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM-Image";
    ls->fileAll                 = "All Files";
    ls->fileCpuState            = "CPU-Status";
    ls->fileDisk                = "Diskette-Image";
    ls->fileCas                 = "Cassette-Image";


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- keine neuen Files -";
    ls->menuInsert              = "Einfügen";
    ls->menuEject               = "Auswurf";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "SCC Cartridge";
    ls->menuCartSCCPlus         = "SCC+ Cartridge";
    ls->menuCartFMPac           = "FM-PAC-Cartridge";
    ls->menuCartPac             = "PAC-Cartridge";
    ls->menuCartHBI55           = "Sony HBI-55 Cartridge";
    ls->menuCartInsertSpecial   = "Insert Special";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "Externes RAM";

    ls->menuCasRewindAfterInsert = "Zurück nach Einfügen";
    ls->menuCasUseReadOnly       = "Cassetten-Image nur lesend benutzen";
    ls->lmenuCasSaveAs           = "Speichere Cassetten-Image als ...";
    ls->menuCasSetPosition      = "Positionieren";
    ls->menuCasRewind           = "Zurück";

    ls->menuDiskInsertNew       = "Insert New Disk Image";              // New in 2.5
    ls->menuDiskDirInsert       = "Directory einfügen";
    ls->menuDiskAutoStart       = "Reset nach Einfügen";
    ls->menuCartAutoReset       = "Reset nach Einfügen/Entfernen";

    ls->menuPrnFormfeed         = "Seitenvorschub";

    ls->menuZoomNormal          = "Normale Größe";
    ls->menuZoomDouble          = "Doppelte Größe";
    ls->menuZoomFullscreen      = "Ganzer Bildschirm";
    
    ls->menuPropsEmulation      = "Emulation";
    ls->menuPropsVideo          = "Video";
    ls->menuPropsSound          = "Sound";
    ls->menuPropsControls       = "Controls";
    ls->menuPropsPerformance    = "Performance";
    ls->menuPropsSettings        = "Einstellungen";
    ls->menuPropsFile           = "Files";
    ls->menuPropsLanguage       = "Sprache";
    ls->menuPropsPorts          = "Ports";
    
    ls->menuVideoSource         = "Video Out Source";                   // New in 2.5
    ls->menuVideoSourceDefault  = "No Video Out Source Connected";      // New in 2.5
    ls->menuVideoChipAutodetect = "Autodetect Video-Chip";    
    ls->menuVideoInSource       = "Video In Source";                    // New in 2.5
    ls->menuVideoInBitmap       = "Bitmap File";                        // New in 2.5

    ls->menuHelpHelp            = "Hilfethemen";
    ls->menuHelpAbout           = "Über blueMSX";

    ls->menuFileCart            = "Cartridge Slot";
    ls->menuFileDisk            = "Disk Drive";
    ls->menuFileCas             = "Cassette";
    ls->menuFilePrn             = "Drucker";
    ls->menuFileLoadState       = "Lade CPU-Status";
    ls->menuFileSaveState       = "Speichere CPU-Status";
    ls->menuFileQLoadState      = "Quick Load State";
    ls->menuFileQSaveState      = "Quick Save State";
    ls->menuFileCaptureAudio    = "Capture Audio";
    ls->menuFileScreenShot      = "Speichere Screenshot";
    ls->menuFileExit            = "Exit";

    ls->menuFileHarddisk        = "Hard Disk";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "No Controllers Present";             // New in 2.5

    ls->menuRunRun              = "Start";
    ls->menuRunPause            = "Pause";
    ls->menuRunStop             = "Stopp";
    ls->menuRunSoftReset        = "Soft Reset";
    ls->menuRunHardReset        = "Hard Reset";
    ls->menuRunCleanReset       = "Allgemeiner Reset";

    ls->menuToolsMachine         = "Maschineneditor";
    ls->menuToolsShortcuts      = "Shortcuts-Editor";
    ls->menuToolsKeyboard       = "Keyboard-Editor";
    ls->menuToolsMixer          = "Audio-Mixer";
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "File";
    ls->menuRun                 = "Run";
    ls->menuWindow              = "Window";
    ls->menuOptions             = "Options";
    ls->menuTools                = "Tools";
    ls->menuHelp                = "Help";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Öffnen";
    ls->dlgCancel               = "Abbrechen";
    ls->dlgSave                 = "Speichern";
    ls->dlgSaveAs               = "Speichern als ...";
    ls->dlgRun                  = "Start";
    ls->dlgClose                = "Schließen";

    ls->dlgLoadRom              = "blueMSX - Wähle ein ROM-Image aus";
    ls->dlgLoadDsk              = "blueMSX - Wähle ein DSK-Image aus";
    ls->dlgLoadCas              = "blueMSX - Wähle ein CAS-Image aus";
    ls->dlgLoadRomDskCas        = "blueMSX - Wähle ein ROM-, DSK- oder CAS- File zum Laden aus";
    ls->dlgLoadRomDesc          = "Wähle ein ROM-Image aus:";
    ls->dlgLoadDskDesc          = "Wähle ein DISK-Image aus:";
    ls->dlgLoadCasDesc          = "Wähle ein CASSETTEN-Image aus:";
    ls->dlgLoadRomDskCasDesc    = "Wähle ein ROM-, DSK- oder CAS- File zum Laden aus:";
    ls->dlgLoadState            = "Lade CPU-Status";
    ls->dlgSaveState            = "Speichere CPU-Status";
    ls->dlgSaveCassette          = "blueMSX - Speichere Tape-Image";
    ls->dlgInsertRom1           = "ROM-Cartridge in Slot 1 einfügen";
    ls->dlgInsertRom2           = "ROM-Cartridge in Slot 2 einfügen";
    ls->dlgInsertDiskA          = "Disketten-Image in Laufwerk A einfügen";
    ls->dlgInsertDiskB          = "Disketten-Image in Laufwerk B einfügen";
    ls->dlgInsertCas            = "Cassette einfügen";
    ls->dlgInsertHarddisk       = "Insert Hard Disk";                   // New in 2.5
    ls->dlgRomType              = "Typ des ROMs:";

    ls->dlgTapeTitle            = "blueMSX - Bandposition";
    ls->dlgTapeFrameText        = "Bandposition";
    ls->dlgTapeCurrentPos       = "Aktuelle Position";
    ls->dlgTapeTotalTime        = "Gesamtzeit";
    ls->dlgTapeSetPosText        = "Bandposition:";
    ls->dlgTapeCustom            = "Zeige anwenderspezifische Files";
    ls->dlgTabPosition           = "Position";
    ls->dlgTabType               = "Typ";
    ls->dlgTabFilename           = "Filename";
    ls->dlgZipReset             = "Reset nach Einfügen";

    ls->dlgAboutTitle           = "Über blueMSX";

    ls->dlgLangLangText         = "Wähle die Sprache aus, die blueMSX benutzen soll.";
    ls->dlgLangLangTitle        = "blueMSX - Sprache";

    ls->dlgAboutAbout           = "ÜBER\r\n====";
    ls->dlgAboutVersion         = "Version:";
    ls->dlgAboutBuildNumber     = "Hergestellt:";
    ls->dlgAboutBuildDate       = "Datum:";
    ls->dlgAboutCreat           = "von Daniel Vik";
    ls->dlgAboutDevel           = "ENTWICKLER\r\n========";
    ls->dlgAboutThanks          = "BESONDEREN DANK AN\r\n============";
    ls->dlgAboutLisence         = "LIZENZ\r\n"
                                  "======\r\n\r\n"
                                  "Diese Software wird ''wie sie ist'' zur Verfügung gestellt, ohne jegliche Garantie."
                                  "In keinem Fall wird der Autor/werden die Autoren haftpflichtig für irgendwelche Schäden,"
                                  "die aus dem Gebrauch dieser Software entstehen könnten.\r\n\r\n"
                                  "Besuchen Sie www.bluemsx.com, um nähere Details zu erfahren.";

    ls->dlgSavePreview          = "Zeige Preview";
    ls->dlgSaveDate             = "Eingesparte Zeit:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Eigenschaften";
    ls->propEmulation           = "Emulation";
    ls->propVideo               = "Video";
    ls->propSound               = "Sound";
    ls->propControls            = "Controls";
    ls->propPerformance         = "Performance";
    ls->propSettings             = "Einstellungen";
    ls->propFile                = "Files";
    ls->propPorts               = "Ports";
    
    ls->propEmuGeneralGB        = "Allgemeines ";
    ls->propEmuFamilyText       = "MSX-Maschine:";
    ls->propEmuMemoryGB         = "Memory ";
    ls->propEmuRamSizeText      = "RAM size:";
    ls->propEmuVramSizeText     = "VRAM size:";
    ls->propEmuSpeedGB          = "Emulationsgeschwindigkeit ";
    ls->propEmuSpeedText        = "Emulationsgeschwindigkeit:";
    ls->propEmuFrontSwitchGB     = "Panasonic Switches ";
    ls->propEmuFrontSwitch       = " Front Switch";
    ls->propEmuFdcTiming        = "Floppy-Drive-Timing sperren";
    ls->propEmuPauseSwitch      = " Unterbrechungstaste";
    ls->propEmuAudioSwitch       = " MSX-AUDIO cartridge switch";
    ls->propVideoFreqText       = "Video-Frequenz:";
    ls->propVideoFreqAuto       = "Auto";
    ls->propSndOversampleText   = "Oversample:";
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI In ";
    ls->propSndMidiOutGB        = "MIDI Out ";
    ls->propSndMidiChannel      = "MIDI Channel:";                      // New in 2.5
    ls->propSndMidiAll          = "All";                                // New in 2.5

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Typ des Monitors:";
    ls->propMonEmuText          = "Monitoremulation:";
    ls->propVideoTypeText       = "Videotyp:";
    ls->propWindowSizeText      = "Fenstergröße:";
    ls->propMonHorizStretch      = " Horizontal Stretch";
    ls->propMonVertStretch       = " Vertical Stretch";
    ls->propMonDeInterlace      = " De-interlace";
    ls->propBlendFrames         = " Blend consecutive frames";           // New in 2.5
    ls->propMonBrightness       = "Helligkeit:";
    ls->propMonContrast         = "Kontrast:";
    ls->propMonSaturation       = "Sättigung:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Scanlines:";
    ls->propMonColorGhosting    = "RF-Modulator";
    ls->propMonEffectsGB        = "Effects ";

    ls->propPerfVideoDrvGB      = "Video-Treiber ";
    ls->propPerfVideoDispDrvText= "Display-Treiber:";
    ls->propPerfFrameSkipText   = "Frame skipping:";
    ls->propPerfAudioDrvGB      = "Audio-Treiber ";
    ls->propPerfAudioDrvText    = "Sound-Treiber:";
    ls->propPerfAudioBufSzText  = "Größe des Sound-Buffers:";
    ls->propPerfEmuGB           = "Emulation ";
    ls->propPerfSyncModeText    = "SYNC-Mode:";
    ls->propFullscreenResText   = "Bildschirmauflösung:";

    ls->propSndChipEmuGB        = "Emulation des Sound-Chips ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " Map MT-32 instruments to General MIDI";

    ls->propPortsLptGB          = "Paralleler Port ";
    ls->propPortsComGB          = "Serielle Ports ";
    ls->propPortsLptText        = "Port:";
    ls->propPortsCom1Text       = "Port 1:";
    ls->propPortsNone           = "Nichts";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC";
    ls->propPortsFile           = "In Datei schreiben";
    ls->propPortsComFile        = "Zu Datei senden";
    ls->propPortsOpenLogFile    = "Öffne Log-Datei";
    ls->propPortsEmulateMsxPrn  = "Emulation:";

    ls->propSetFileHistoryGB     = "File History ";
    ls->propSetFileHistorySize   = "Anzahl der Elemente in der File-History:";
    ls->propSetFileHistoryClear  = "Lösche History";
    ls->propFileTypes            = " Registriere Filetypen mit blueMSX (.rom, .dsk, .cas, .sta)";
    ls->propWindowsEnvGB         = "Windows-Environment "; 
    ls->propSetScreenSaver       = " Schalte den Bildschirmschoner ab, wenn blueMSX läuft";
    ls->propDisableWinKeys       = " Automatische MSX-Function für Windows-Menütasten"; 
    ls->propPriorityBoost       = " Die Priorität von blueMSX erhöhen";
    ls->propScreenshotPng       = " Use Portable Network Graphics (.png) screenshots";  // New in 2.5
    ls->propClearHistory         = "Sind Sie sicher, daß Sie die File-History löschen wollen?";
    ls->propOpenRomGB           = "Öffne ROM-Dialog ";
    ls->propDefaultRomType      = "Voreingestellter ROM-Typ:";
    ls->propGuessRomType        = "ROM-Typ erraten";

    ls->propSettDefSlotGB       = "Drag and Drop ";
    ls->propSettDefSlots        = "ROM einfügen in:";
    ls->propSettDefSlot         = " Slot";
    ls->propSettDefDrives       = "Disketten einfügen in:";
    ls->propSettDefDrive        = " Drive";

    ls->propThemeGB             = "Theme ";
    ls->propTheme               = "Theme:";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Farbe";
    ls->enumVideoMonGrey        = "Schwarzweiß";
    ls->enumVideoMonGreen       = "Grün";
    ls->enumVideoMonAmber       = "Amber";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Nichts";
    ls->enumVideoEmuYc          = "Y/C-Kabel (scharf)";
    ls->enumVideoEmuMonitor     = "Monitor";
    ls->enumVideoEmuYcBlur      = "Verrauschtes Y/C-Kabel (scharf)";
    ls->enumVideoEmuComp        = "Composit-Signal (verwaschen)";
    ls->enumVideoEmuCompBlur    = "Verrauschtes Composit-Signal (verwaschen)";
    ls->enumVideoEmuScale2x     = "Scale 2x";
    ls->enumVideoEmuHq2x        = "Hq2x";

    ls->enumVideoSize1x         = "Normal - 320x200";
    ls->enumVideoSize2x         = "Double - 640x400";
    ls->enumVideoSizeFullscreen = "Fullscreen";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw HW accel."; 
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Nichts";
    ls->enumVideoFrameskip1     = "1 Frame";
    ls->enumVideoFrameskip2     = "2 Frames";
    ls->enumVideoFrameskip3     = "3 Frames";
    ls->enumVideoFrameskip4     = "4 Frames";
    ls->enumVideoFrameskip5     = "5 Frames";

    ls->enumSoundDrvNone        = "Kein Sound";
    ls->enumSoundDrvWMM         = "WMM-Treiber";
    ls->enumSoundDrvDirectX     = "DirectX-Treiber";

    ls->enumEmuSync1ms          = "Sync am MSX refresh";
    ls->enumEmuSyncAuto         = "Automatisch (schnell)";
    ls->enumEmuSyncNone         = "Nichts";
    ls->enumEmuSyncVblank       = "Sync am PC Vertical Blank";
    ls->enumEmuAsyncVblank      = "Asynchronous PC Vblank";             // New in 2.5

    ls->enumControlsJoyNone     = "Nichts";
    ls->enumControlsJoyMouse    = "Maus";
    ls->enumControlsJoyTetris2Dongle = "Tetris 2 Dongle";
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey Dongle";             // New in 2.5
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

    ls->confTitle                = "blueMSX - Editor für die Maschinenkonfiguration";
    ls->confConfigText           = "Konfiguration";
    ls->confSlotLayout           = "Slot-Layout";
    ls->confMemory               = "Memory";
    ls->confChipEmulation        = "Chip-Emulation";
    ls->confChipExtras          = "Extras";

    ls->confOpenRom             = "Öffnen ROM-Image";
    ls->confSaveTitle            = "blueMSX - Speichere Konfiguration";
    ls->confSaveText             = "Möchten Sie die Maschinenkonfiguration überschreiben?";
    ls->confSaveAsTitle         = "Speichere Konfiguration als ...";
    ls->confSaveAsMachineName    = "Name der Maschine:";
    ls->confDiscardTitle         = "blueMSX - Konfiguration";
    ls->confExitSaveTitle        = "blueMSX - Konfigurationseditor verlassen";
    ls->confExitSaveText         = "Möchten Sie die aktuellen Änderungen der Konfiguration verwerfen?";

    ls->confSlotLayoutGB         = "Slot-Layout ";
    ls->confSlotExtSlotGB        = "Externe Slots ";
    ls->confBoardGB             = "Board ";
    ls->confBoardText           = "Board-Typ:";
    ls->confSlotPrimary          = "Primary";
    ls->confSlotExpanded         = "Expanded (vier Subslots)";

    ls->confSlotCart             = "Cartridge";
    ls->confSlot                = "Slot";
    ls->confSubslot             = "Subslot";

    ls->confMemAdd               = "Hinzufügen ...";
    ls->confMemEdit              = "Editieren ...";
    ls->confMemRemove            = "Entfernen";
    ls->confMemSlot              = "Slot";
    ls->confMemAddresss          = "Addresse";
    ls->confMemType              = "Typ";
    ls->confMemRomImage          = "ROM-Image";
    
    ls->confChipVideoGB          = "Video ";
    ls->confChipVideoChip        = "Video-Chip:";
    ls->confChipVideoRam         = "Video-RAM:";
    ls->confChipSoundGB          = "Sound ";

    ls->confCmosGB                = "CMOS ";
    ls->confCmosEnable            = " CMOS einschalten";
    ls->confCmosBattery           = " Verwende geladene Batterie";

    ls->confCpuFreqGB            = "CPU-Frequenz ";
    ls->confZ80FreqText          = "Z80-Frequenz:";
    ls->confR800FreqText         = "R800-Frequenz:";
    ls->confFdcGB                = "Floppy-Disk-Controller ";
    ls->confCFdcNumDrivesText    = "Anzahl der Laufwerke:";

    ls->confEditMemTitle         = "blueMSX - Edit Mapper";
    ls->confEditMemGB            = "Mapper Details ";
    ls->confEditMemType          = "Typ:";
    ls->confEditMemFile          = "File:";
    ls->confEditMemAddress       = "Addresse";
    ls->confEditMemSize          = "Größe";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Kontrollsequenz";
    ls->shortcutDescription     = "Shortcut";

    ls->shortcutSaveConfig      = "blueMSX - Speichere Konfiguration";
    ls->shortcutOverwriteConfig = "Möchten Sie die Shortcut-Konfiguration überschreiben:";
    ls->shortcutExitConfig      = "blueMSX - Exit Shortcut Editor";
    ls->shortcutDiscardConfig   = "Möchten Sie die Änderungen an der aktuellen Konfiguration verwerfen?";
    ls->shortcutSaveConfigAs    = "blueMSX - Speichere Shortcut-Konfiguration als ...";
    ls->shortcutConfigName      = "Konfigurationsname:";
    ls->shortcutNewProfile      = "< Neues Profil >";
    ls->shortcutConfigTitle     = "blueMSX - Shortcut Mapping Editor";
    ls->shortcutAssign          = "Zuweisen";
    ls->shortcutPressText       = "Drücke Shortcut-Taste(n):";
    ls->shortcutScheme          = "Mapping Scheme:";
    ls->shortcutCartInsert1     = "Cartridge 1 einfügen";
    ls->shortcutCartRemove1     = "Cartridge 1 entfernen";
    ls->shortcutCartInsert2     = "Cartridge 2 einfügen";
    ls->shortcutCartRemove2     = "Cartridge 2 entfernen";
    ls->shortcutSpecialMenu1    = "Zeige spezielles ROM-Menü für Cartridge 1";
    ls->shortcutSpecialMenu2    = "Zeige spezielles ROM-Menü für Cartridge 2";
    ls->shortcutCartAutoReset   = "Emulator zurücksetzen, wenn Cartridge eingefügt wird";
    ls->shortcutDiskInsertA     = "Diskette A einfügen";
    ls->shortcutDiskDirInsertA  = "Directory als Diskette A einfügen";
    ls->shortcutDiskRemoveA     = "Diskette A auswerfen";
    ls->shortcutDiskChangeA     = "Schneller Austausch von Diskette A";
    ls->shortcutDiskAutoResetA  = "Emulator zurücksetzen, wenn Diskette A eingefügt wird";
    ls->shortcutDiskInsertB     = "Diskette B einfügen";
    ls->shortcutDiskDirInsertB  = "Directory als Diskette B einfügen";
    ls->shortcutDiskRemoveB     = "Diskette B auswerfen";
    ls->shortcutCasInsert       = "Cassette einfügen";
    ls->shortcutCasEject        = "Cassette auswerfen";
    ls->shortcutCasAutorewind   = "Toggle Auto-rewind on Cassette";
    ls->shortcutCasReadOnly     = "Toggle Read-only Cassette";
    ls->shortcutCasSetPosition  = "Bandposition festlegen";
    ls->shortcutCasRewind       = "Cassette zurückspulen";
    ls->shortcutCasSave         = "Cassetten-Image speichern";
    ls->shortcutPrnFormFeed     = "Seitenvorschub des Druckers";
    ls->shortcutCpuStateLoad    = "CPU-Status laden";
    ls->shortcutCpuStateSave    = "CPU-Status speichern";
    ls->shortcutCpuStateQload   = "Schnelladen des CPU-Status";
    ls->shortcutCpuStateQsave   = "Schnellspeichern des CPU-Status";
    ls->shortcutAudioCapture    = "Start/stop audio capture";
    ls->shortcutScreenshotOrig  = "Screenshot capture";
    ls->shortcutScreenshotSmall = "Small unfiltered screenshot capture";
    ls->shortcutScreenshotLarge = "Large unfiltered screenshot capture";
    ls->shortcutQuit            = "blueMSX verlassen";
    ls->shortcutRunPause        = "Start/Unterbrechung der Emulation";
    ls->shortcutStop            = "Emulation stoppen";
    ls->shortcutResetHard       = "Hard-Reset";
    ls->shortcutResetSoft       = "Soft-Reset";
    ls->shortcutResetClean      = "Allgemeiner Reset";
    ls->shortcutSizeSmall       = "Kleine Fenstergröße einstellen";
    ls->shortcutSizeNormal      = "Normale Fenstergröße einstellen";
    ls->shortcutSizeFullscreen  = "Volle Bildschirmgröße einstellen";
    ls->shortcutToggleFullscren = "Bildschirmgröße umschalten";
    ls->shortcutSizeMinimized   = "Fenster minimieren";
    ls->shortcutVolumeIncrease  = "Lautstärke erhöhen";
    ls->shortcutVolumeDecrease  = "Lautstärke vermindern";
    ls->shortcutVolumeMute      = "Ton abschalten";
    ls->shortcutVolumeStereo    = "Mono/Stereo umschalten";
    ls->shortcutSwitchMsxAudio  = "Toggle MSX-AUDIO switch";
    ls->shortcutSwitchFront     = "Toggle Panasonic front switch";
    ls->shortcutSwitchPause     = "Unterbrechungstaste umschalten";
    ls->shortcutToggleMouseLock = "Maussperre umschalten";
    ls->shortcutEmuSpeedMax     = "Max. Emulationsgeschwindigkeit";
    ls->shortcutEmuSpeedToggle  = "Max. Emulationsgeschwindigkeit umschalten";
    ls->shortcutEmuSpeedNormal  = "Normale Emulationsgeschwindigkeit";
    ls->shortcutEmuSpeedInc     = "Emulationsgeschwindigkeit erhöhen";
    ls->shortcutEmuSpeedDec     = "Emulationsgeschwindigkeit vermindern";
    ls->shortcutThemeSwitch     = "Switch theme";
    ls->shortcutShowEmuProp     = "Zeige Emulationseigenschaften";
    ls->shortcutShowVideoProp   = "Zeige Video-Eigenschaften";
    ls->shortcutShowAudioProp   = "Zeige Audio-Eigenschaften";
    ls->shortcutShowCtrlProp    = "Zeige Control-Eigenschaften";
    ls->shortcutShowPerfProp    = "Zeige Leistungseigenschaften";
    ls->shortcutShowSettProp    = "Zeige Einstellungseigenschaften";
    ls->shortcutShowPorts       = "Zeige Ports-Eigenschaften";
    ls->shortcutShowLanguage    = "Zeige Sprachdialog";
    ls->shortcutShowMachines    = "Zeige Maschineneditor";
    ls->shortcutShowShortcuts   = "Zeige Shortcuts-Editor";
    ls->shortcutShowKeyboard    = "Zeige Keyboard-Editor";
    ls->shortcutShowMixer       = "Zeige Audio-Mixer";
    ls->shortcutShowDebugger    = "Zeige Debugger";
    ls->shortcutShowTrainer     = "Zeige Trainer";
    ls->shortcutShowHelp        = "Zeige Hilfe-Dialog";
    ls->shortcutShowAbout       = "Zeige ''About''-Dialog";
    ls->shortcutShowFiles       = "Zeige Fileeigenschaften";
    ls->shortcutToggleSpriteEnable = "Sprites zeigen/verdecken";
    ls->shortcutToggleFdcTiming = "Freigabe/Sperren des Floppy-Drive-Timings";
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

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageDutch.h,v $
**
** $Revision: 1.30 $
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
#ifndef LANGUAGE_DUTCH_H
#define LANGUAGE_DUTCH_H

#include "LanguageStrings.h"

void langInitDutch(LanguageStrings* ls)
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

    ls->warningTitle             = "blueMSX - Waarschuwing";
    ls->warningDiscardChanges   = "Wilt u de wijzigingen ongedaan maken?"; 
    ls->warningOverwriteFile    = "Do you want to overwrite the file:"; // New in 2.5
    ls->errorTitle              = "blueMSX - Fout";
    ls->errorEnterFullscreen    = "Het is niet gelukt om over te schakelen naar de volledige schermmodus.          \n";
    ls->errorDirectXFailed      = "Het is niet gelukt om DirectX te initializeren. \nDe GDI wordt nu gebruikt.\nKijk de video instellingen na..";
    ls->errorNoRomInZip         = "Er zijn geen .rom bestanden gevonden in het zip bestand.";
    ls->errorNoDskInZip         = "Er zijn geen .dsk bestanden gevonden in het zip bestand.";
    ls->errorNoCasInZip         = "Er zijn geen .cas bestanden gevonden in het zip bestand.";
    ls->errorNoHelp             = "Het blueMSX help bestand is niet gevonden.";
    ls->errorStartEmu           = "Het is niet gelukt om de MSX emulator te starten.";
    ls->errorPortableReadonly   = "Portable device is readonly";        // New in 2.5


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

    ls->menuNoRecentFiles       = "- geen recente bestanden -";
    ls->menuInsert              = "Toevoegen";
    ls->menuEject               = "Eruit halen";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "SCC Cartridge";
    ls->menuCartSCCPlus         = "SCC+ Cartridge";
    ls->menuCartFMPac           = "FM-PAC Cartridge";
    ls->menuCartPac             = "PAC Cartridge";
    ls->menuCartHBI55           = "Sony HBI-55 Cartridge"; 
    ls->menuCartInsertSpecial   = "Insert Special";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "External RAM";

    ls->menuDiskInsertNew       = "Insert New Disk Image";              // New in 2.5
    ls->menuDiskDirInsert       = "Insert Directory";
    ls->menuDiskAutoStart       = "Herstart na toevoegen";
    ls->menuCartAutoReset       = "Herstart na toevoegen/verwijderen";

    ls->menuCasRewindAfterInsert= "Terugspoelen na inname";
    ls->menuCasUseReadOnly      = "Gebruik Cassette Beeld als Alleen Lezen";
    ls->lmenuCasSaveAs          = "Sla Cassette Beeld op als...";
    ls->menuCasSetPosition      = "Bepaal positie";
    ls->menuCasRewind           = "Spoel terug";

    ls->menuPrnFormfeed         = "Pagina doorvoer"; 

    ls->menuZoomNormal          = "Normale grootte";
    ls->menuZoomDouble          = "Dubbele grootte";
    ls->menuZoomFullscreen      = "Volledig scherm";

    ls->menuPropsEmulation      = "Emulatie";
    ls->menuPropsVideo          = "Beeld";
    ls->menuPropsSound          = "Geluid";
    ls->menuPropsControls       = "Besturing";
    ls->menuPropsPerformance    = "Prestatie";
    ls->menuPropsSettings        = "Instellingen";
    ls->menuPropsFile           = "Bestand";
    ls->menuPropsLanguage       = "Taal";
    ls->menuPropsPorts          = "Poorten"; 

    ls->menuVideoSource         = "Video Out Source";                   // New in 2.5
    ls->menuVideoSourceDefault  = "No Video Out Source Connected";      // New in 2.5
    ls->menuVideoChipAutodetect = "Automatisch detecteren van Video Chip";     
    ls->menuVideoInSource       = "Video In Source";                    // New in 2.5
    ls->menuVideoInBitmap       = "Bitmap File";                        // New in 2.5

    ls->menuHelpHelp            = "Help";
    ls->menuHelpAbout           = "Info blueMSX";

    ls->menuFileCart            = "Cartridge slot";
    ls->menuFileDisk            = "Diskette station";
    ls->menuFileCas             = "Cassette";
    ls->menuFilePrn             = "Printer"; 
    ls->menuFileLoadState       = "Laad CPU status";
    ls->menuFileSaveState       = "Opslaan CPU status";
    ls->menuFileQLoadState      = "Snel laden status";
    ls->menuFileQSaveState      = "Snel opslaan status";
    ls->menuFileCaptureAudio    = "Opslaan Geluid";
    ls->menuFileScreenShot      = "Schermafdruk maken";
    ls->menuFileExit            = "Afsluiten";

    ls->menuFileHarddisk        = "Hard Disk";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "No Controllers Present";             // New in 2.5

    ls->menuRunRun              = "Start";
    ls->menuRunPause            = "Pauze";
    ls->menuRunStop             = "Stop";
    ls->menuRunSoftReset        = "Zachte Herstart";
    ls->menuRunHardReset        = "Harde Herstart";
    ls->menuRunCleanReset       = "Algemene Herstart";

    ls->menuToolsMachine         = "Aanpassen Machine";
    ls->menuToolsShortcuts      = "Maken Snelkoppeling";
    ls->menuToolsKeyboard       = "Toetsenbord Editor"; 
    ls->menuToolsMixer          = "Mixer";
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "Bestand";
    ls->menuRun                 = "Start";
    ls->menuWindow              = "Scherm";
    ls->menuOptions             = "Opties";
    ls->menuTools                = "Extra";
    ls->menuHelp                = "Help";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Openen";
    ls->dlgCancel               = "Annuleren";
    ls->dlgSave                 = "Opslaan";
    ls->dlgSaveAs               = "Opslaan als...";
    ls->dlgRun                  = "Start";
    ls->dlgClose                = "Afsluiten";

    ls->dlgLoadRom              = "blueMSX - Selecteer een rom om te laden";
    ls->dlgLoadDsk              = "blueMSX - Selecteer een dsk om te laden";
    ls->dlgLoadCas              = "blueMSX - Selecteer een cas om te laden";
    ls->dlgLoadRomDskCas        = "blueMSX - Selecteer een rom, dsk, of cas bestand om te laden";
    ls->dlgLoadRomDesc          = "Selecteer een rom om te laden:";
    ls->dlgLoadDskDesc          = "Selecteer een diskette om te laden:";
    ls->dlgLoadCasDesc          = "Selecteer een tape om te laden:";
    ls->dlgLoadRomDskCasDesc    = "Selecteer een rom, diskette, of tape om te laden:";
    ls->dlgLoadState            = "Laad CPU status";
    ls->dlgSaveState            = "Opslaan CPU status";
    ls->dlgSaveCassette         = "blueMSX - Sla Tape Beeld op";
    ls->dlgInsertRom1           = "Stop ROM cartridge in slot 1";
    ls->dlgInsertRom2           = "Stop ROM cartridge in slot 2";
    ls->dlgInsertDiskA          = "Stop diskette image in drive A";
    ls->dlgInsertDiskB          = "Stop diskette image in drive B";
    ls->dlgInsertHarddisk       = "Insert Hard Disk";                   // New in 2.5
    ls->dlgInsertCas            = "Stop tape in casette speler";
    ls->dlgRomType              = "Rom type :";

    ls->dlgTapeTitle            = "blueMSX - Tape positie";
    ls->dlgTapeFrameText        = "Tape Positie";
    ls->dlgTapeCurrentPos       = "Huidige positie";
    ls->dlgTapeTotalTime        = "Totale tijd";
    ls->dlgTapeCustom            = "Laat Speciale Bestanden zien";
    ls->dlgTapeSetPosText        = "Tape Positie:";
    ls->dlgTabPosition           = "Positie";
    ls->dlgTabType               = "Type";
    ls->dlgTabFilename           = "Bestandsnaam";
    ls->dlgZipReset             = "Herstart na toevoegen";

    ls->dlgAboutTitle           = "blueMSX - Info";

    ls->dlgLangLangText         = "Kies de taal die blueMSX moet gebruiken";
    ls->dlgLangLangTitle        = "blueMSX - Taal";

    ls->dlgAboutAbout           = "Info\r\n====";
    ls->dlgAboutVersion         = "Versie:";
    ls->dlgAboutBuildNumber     = "Gemaakt:";
    ls->dlgAboutBuildDate       = "Datum:";
    ls->dlgAboutCreat           = "Gemaakt door Daniel Vik";
    ls->dlgAboutDevel           = "GEMAAKT DOOR\r\n========";
    ls->dlgAboutThanks          = "MET DANK AAN\r\n============";
    ls->dlgAboutLisence         = "LICENTIE\r\n"
                                  "======\r\n\r\n"
                                  "Deze software wordt gegeven 'as-is', zonder enige vorm van garantie. "
                                  "De auteur(s) is/zijn niet verantwoordelijk voor elke vorm van schade "
                                  "die onstaat door het gebruik van deze software.\r\n\r\n"
                                  "Bezoek www.bluemsx.com voor meer details.";

    ls->dlgSavePreview          = "Voorbeeld weergeven"; 
    ls->dlgSaveDate             = "Opgeslagen op:"; 


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Eigenschappen";
    ls->propEmulation           = "Emulatie";
    ls->propVideo               = "Beeld";
    ls->propSound               = "Geluid";
    ls->propControls            = "Besturing";
    ls->propPerformance         = "Prestaties";
    ls->propSettings            = "Instellingen";
    ls->propFile                = "Bestand";
    ls->propPorts               = "Poorten";

    ls->propEmuGeneralGB        = "Algemeen ";
    ls->propEmuFamilyText       = "MSX familie:";
    ls->propEmuMemoryGB         = "Geheugen ";
    ls->propEmuRamSizeText      = "RAM grootte:";
    ls->propEmuVramSizeText     = "VRAM grootte:";
    ls->propEmuSpeedGB          = "Emulatie Snelheid ";
    ls->propEmuSpeedText        = "Emulatie Snelheid:";
    ls->propEmuFrontSwitchGB    = "Panasonic Switches ";
    ls->propEmuFrontSwitch      = " Front Switch";
    ls->propEmuFdcTiming        = "Diskette station niet synchroniseren"; 
    ls->propEmuPauseSwitch      = " Pause Switch"; 
    ls->propEmuAudioSwitch      = " MSX-AUDIO cartridge switch";
    ls->propVideoFreqText       = "Video frequentie:"; 
    ls->propVideoFreqAuto       = "Auto"; 
    ls->propSndOversampleText   = "Oversample:"; 
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI In ";
    ls->propSndMidiOutGB        = "MIDI Out ";
    ls->propSndMidiChannel      = "MIDI Channel:";                      // New in 2.5
    ls->propSndMidiAll          = "All";                                // New in 2.5

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Monitor type:";
    ls->propMonEmuText          = "Monitor emulatie:";
    ls->propVideoTypeText       = "Beeld type:";
    ls->propWindowSizeText      = "Scherm grootte:";
    ls->propMonHorizStretch      = " Horizontaal uitrekken";
    ls->propMonVertStretch       = " Verticaal uitrekken";
    ls->propMonDeInterlace      = " De-interlace";
    ls->propBlendFrames         = " Blend consecutive frames";           // New in 2.5
    ls->propMonBrightness       = "Helderheid:";
    ls->propMonContrast         = "Contrast:";
    ls->propMonSaturation       = "Verzadiging:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Beeldlijnen:";
    ls->propMonColorGhosting    = "RF-Modulator"; 
    ls->propMonEffectsGB        = "Effects ";

    ls->propPerfVideoDrvGB      = "Beeld instellingen ";
    ls->propPerfVideoDispDrvText= "Beeld stuurprogramma:";
    ls->propPerfFrameSkipText   = "Frames overslaan:";
    ls->propPerfAudioDrvGB      = "Geluid instellingen ";
    ls->propPerfAudioDrvText    = "Geluid stuurprogramma:";
    ls->propPerfAudioBufSzText  = "Geluid buffer grootte:";
    ls->propPerfEmuGB           = "Emulatie ";
    ls->propPerfSyncModeText    = "Synchronisatie mode";
    ls->propFullscreenResText   = "Resolutie voor volledige schermmodus:"; 

    ls->propSndChipEmuGB        = "Geluid Chip Emulatie ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound        = " Moonsound";
    ls->propSndMt32ToGm         = " Map MT-32 instruments to General MIDI";

    ls->propPortsLptGB          = "Parallele poort "; 
    ls->propPortsComGB          = "Seriele poorten "; 
    ls->propPortsLptText        = "Poort:"; 
    ls->propPortsCom1Text       = "Poort 1:";
    ls->propPortsNone           = "Geen";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC"; 
    ls->propPortsFile           = "Afdrukken naar bestand"; 
    ls->propPortsComFile        = "Stuur naar bestand"; 
    ls->propPortsOpenLogFile    = "Log bestand openen"; 
    ls->propPortsEmulateMsxPrn  = "Emulatie:"; 

    ls->propSetFileHistoryGB     = "Bestand Geschiedenis ";
    ls->propSetFileHistorySize   = "Aantal bestanden in geschiedenis:";
    ls->propSetFileHistoryClear  = "Leegmaken Geschiedenis";
    ls->propFileTypes            = " Bestanden registreren bij blueMSX (.rom, .dsk, .cas, .sta)";
    ls->propWindowsEnvGB         = "Windows Omgeving";
    ls->propSetScreenSaver       = "Scherm Beveiliging uitschakelen als blueMSX draait";
    ls->propDisableWinKeys       = " Automatische MSX functie voor Windows menu toetsen";
    ls->propPriorityBoost       = " blueMSX een hogere prioriteit geven";
    ls->propScreenshotPng       = " Use Portable Network Graphics (.png) screenshots";  // New in 2.5
    ls->propClearHistory         = "Weet u zeker dat u de bestandsgeschiedenis wilt wissen?";
    ls->propOpenRomGB           = "Openen Rom bestand";
    ls->propDefaultRomType      = "Standaard type:";
    ls->propGuessRomType        = "Raden type";

    ls->propSettDefSlotGB       = "Slepen en neerzetten ";
    ls->propSettDefSlots        = "Toevoegen cartridge in:";
    ls->propSettDefSlot         = " Slot";
    ls->propSettDefDrives       = "Toevoegen diskette in:";
    ls->propSettDefDrive        = " Drive";

    ls->propThemeGB             = "Thema ";
    ls->propTheme               = "Thema";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Kleur";
    ls->enumVideoMonGrey        = "Zwart-Wit";
    ls->enumVideoMonGreen       = "Groen";
    ls->enumVideoMonAmber       = "Amber"; 

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Geen";
    ls->enumVideoEmuYc          = "Y/C kabel (Scherp)";
    ls->enumVideoEmuMonitor     = "Monitor"; 
    ls->enumVideoEmuYcBlur      = "Ruis Y/C kabel (Scherp)";
    ls->enumVideoEmuComp        = "Compositie (Wazig)";
    ls->enumVideoEmuCompBlur    = "Ruis Compositie (Wazig)";
    ls->enumVideoEmuScale2x     = "Schaal 2x";
    ls->enumVideoEmuHq2x        = "Hq2x"; 

    ls->enumVideoSize1x         = "Normaal - 320x200";
    ls->enumVideoSize2x         = "Dubbel - 640x400";
    ls->enumVideoSizeFullscreen = "Volledig Scherm";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw HW accel.";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "geen";
    ls->enumVideoFrameskip1     = "1 frame";
    ls->enumVideoFrameskip2     = "2 frames";
    ls->enumVideoFrameskip3     = "3 frames";
    ls->enumVideoFrameskip4     = "4 frames";
    ls->enumVideoFrameskip5     = "5 frames";

    ls->enumSoundDrvNone        = "Geen Geluid";
    ls->enumSoundDrvWMM         = "WMM stuurprogramma";
    ls->enumSoundDrvDirectX     = "DirectX stuurprogramma";

    ls->enumEmuSync1ms          = "Sync on MSX refresh";
    ls->enumEmuSyncAuto         = "Auto (fast)";
    ls->enumEmuSyncNone         = "None";
    ls->enumEmuSyncVblank       = "Sync to PC Vertical Blank";
    ls->enumEmuAsyncVblank      = "Asynchronous PC Vblank";             // New in 2.5

    ls->enumControlsJoyNone     = "geen";
    ls->enumControlsJoyMouse    = "Muis";
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

    ls->confTitle               = "blueMSX - Machine configuratie aanpassen";
    ls->confConfigText          = "Configuratie";
    ls->confSlotLayout          = "Slot instellingen";
    ls->confMemory              = "Geheugen";
    ls->confChipEmulation       = "Chip Emulatie";
    ls->confChipExtras          = "Extras"; 

    ls->confOpenRom             = "Open ROM image";
    ls->confSaveTitle           = "blueMSX - Machine configuratie opslaan";
    ls->confSaveText            = "Deze machine configuratie vervangen? :";
    ls->confSaveAsTitle         = "Configuratie opslaan als..."; 
    ls->confSaveAsMachineName   = "Machine Naam:";
    ls->confDiscardTitle        = "blueMSX - Configuratie";
    ls->confExitSaveTitle       = "blueMSX - Machine configuratie afsluiten";
    ls->confExitSaveText        = "De gemaakte wijzigingen in de huidige configuratie worden niet opgeslagen. Wilt u doorgaan?";

    ls->confSlotLayoutGB        = "Slot instellingen ";
    ls->confSlotExtSlotGB       = "Externe Slots ";
    ls->confBoardGB             = "Systeem "; 
    ls->confBoardText           = "Systeem Type:"; 
    ls->confSlotPrimary         = "Primair";
    ls->confSlotExpanded        = "Uitgebreid (vier sub-slots)";

    ls->confSlotCart            = "Cartridge";
    ls->confSlot                = "Slot";
    ls->confSubslot             = "Subslot";

    ls->confMemAdd              = "Toevoegen...";
    ls->confMemEdit             = "Aanpassen...";
    ls->confMemRemove           = "Verwijderen";
    ls->confMemSlot             = "Slot";
    ls->confMemAddresss         = "Adres";
    ls->confMemType             = "Type";
    ls->confMemRomImage         = "Rom Beeld";

    ls->confChipVideoGB         = "Beeld ";
    ls->confChipVideoChip       = "Beeld Chip:";
    ls->confChipVideoRam        = "Beeld RAM:";
    ls->confChipSoundGB         = "Geluid ";

    ls->confCmosGB                = "CMOS ";
    ls->confCmosEnable            = " Aanzetten CMOS";
    ls->confCmosBattery           = " Gebruiken Geladen Batterij";

    ls->confCpuFreqGB            = "CPU frequentie "; 
    ls->confZ80FreqText          = "Z80 frequentie:"; 
    ls->confR800FreqText         = "R800 Frequentie:"; 
    ls->confFdcGB                = "Floppy Disk Controller "; 
    ls->confCFdcNumDrivesText    = "Aantal drives:"; 

    ls->confEditMemTitle        = "blueMSX - Aanpassen Mapper";
    ls->confEditMemGB           = "Mapper Details ";
    ls->confEditMemType         = "Type:";
    ls->confEditMemFile         = "Bestand:";
    ls->confEditMemAddress      = "Address";
    ls->confEditMemSize         = "Grootte";
    ls->confEditMemSlot         = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Hotkey";
    ls->shortcutDescription     = "Shortcut";

    ls->shortcutSaveConfig      = "blueMSX -  Configuratie opslaan";
    ls->shortcutOverwriteConfig = "Huidige configuratie overschrijven?:";
    ls->shortcutExitConfig      = "blueMSX -  Snelkoppeling configuratie afsluiten";
    ls->shortcutDiscardConfig   = "Weet u zeker dat u de gemaakte wijzigingen in de huidige configuratie niet wilt toepassen?";
    ls->shortcutSaveConfigAs    = "blueMSX - Configuratie opslaan als...";
    ls->shortcutConfigName      = "Configuratie Naam:";
    ls->shortcutNewProfile      = "< Nieuw Profiel >";
    ls->shortcutConfigTitle     = "blueMSX - Snelkoppeling Toewijzingen";
    ls->shortcutAssign          = "Toewijzen";
    ls->shortcutPressText       = "Druk op Snelkoppeling toets(en):";
    ls->shortcutScheme          = "Schema:";
    ls->shortcutCartInsert1     = "Toevoegen Cartridge in Slot 1";
    ls->shortcutCartRemove1     = "Verwijder Cartridge uit Slot 1";
    ls->shortcutCartInsert2     = "Toevoegen Cartridge in Slot 2";
    ls->shortcutCartRemove2     = "Verwijder Cartridge uit Slot 2";
    ls->shortcutSpecialMenu1    = "Dialoog: Speciaal menu voor ROM cartridge in slot 1";
    ls->shortcutSpecialMenu2    = "Dialoog: Speciaal menu voor ROM cartridge in slot 2";
    ls->shortcutCartAutoReset   = "Herstart na toevoegen cartridge";
    ls->shortcutDiskInsertA     = "Toevoegen Diskette in Drive A";
    ls->shortcutDiskDirInsertA  = "Insert Directory as Diskette A";
    ls->shortcutDiskRemoveA     = "Verwijder Diskette uit drive A";
    ls->shortcutDiskChangeA     = "Snel veranderen van diskette in drive A";
    ls->shortcutDiskAutoResetA  = "Herstart na toevoegen diskette in drive A";
    ls->shortcutDiskInsertB     = "Toevoegen Diskette in Drive B";
    ls->shortcutDiskDirInsertB  = "Insert Directory as Diskette B";
    ls->shortcutDiskRemoveB     = "Verwijder Diskette uit Drive B";
    ls->shortcutCasInsert       = "Toevoegen Cassette";
    ls->shortcutCasEject        = "Verwijder Cassette";
    ls->shortcutCasAutorewind   = "Automatisch cassette terugspoelen ja/nee";
    ls->shortcutCasReadOnly     = "Aleen lezen cassette beeld ja/nee";
    ls->shortcutCasSetPosition  = "Bepalen Cassette Positie";
    ls->shortcutCasRewind       = "Terugspoelen Cassette";
    ls->shortcutCasSave         = "Opslaan Cassette Beeld";
    ls->shortcutPrnFormFeed     = "Printer pagina doorvoer"; 
    ls->shortcutCpuStateLoad    = "Laden CPU status";
    ls->shortcutCpuStateSave    = "Opslaan CPU status";
    ls->shortcutCpuStateQload   = "Snel laden CPU status";
    ls->shortcutCpuStateQsave   = "Snel opslaan CPU status";
    ls->shortcutAudioCapture    = "Start/stop het audio opslaan";
    ls->shortcutScreenshotOrig  = "Opslaan screenshot";
    ls->shortcutScreenshotSmall = "Opslaan klein ongefilterde screenshot";
    ls->shortcutScreenshotLarge = "Opslaan groot ongefilterde screenshot";
    ls->shortcutQuit            = "Exit blueMSX";
    ls->shortcutRunPause        = "Starten/Pauseren emulatie";
    ls->shortcutStop            = "Stoppen emulatie";
    ls->shortcutResetHard       = "Harde Herstart";
    ls->shortcutResetSoft       = "Zachte Herstart";
    ls->shortcutResetClean      = "Algemene Herstart";
    ls->shortcutSizeSmall       = "Kiezen normale grootte voor het scherm";
    ls->shortcutSizeNormal      = "Kiezen dubbele grootte voor het scherm";
    ls->shortcutSizeFullscreen  = "Kiezen volledig scherm";
    ls->shortcutSizeMinimized   = "Minimaliseer scherm"; 
    ls->shortcutToggleFullscren = "Kiezen volledig scherm of onvolledig scherm";
    ls->shortcutVolumeIncrease  = "Geluidsterkte verhogen";
    ls->shortcutVolumeDecrease  = "Geluidsterkte verminderen";
    ls->shortcutVolumeMute      = "Geluid uitzetten";
    ls->shortcutVolumeStereo    = "Wisselen mono of stereo mode";
    ls->shortcutSwitchMsxAudio  = "MSX-AUDIO switch aan/uit";
    ls->shortcutSwitchFront     = "Panasonic front switch aan/uit";
    ls->shortcutSwitchPause     = "Pause switch aan/uit"; 
    ls->shortcutToggleMouseLock = "Muis vastzetten aan/uit";
    ls->shortcutEmuSpeedMax     = "Maximale emulatie snelheid";
    ls->shortcutEmuSpeedToggle  = "Maximum snelheid van de emulatie aan/uit"; 
    ls->shortcutEmuSpeedNormal  = "Normale emulatie snelheid";
    ls->shortcutEmuSpeedInc     = "Verhoog emulatie snelheid";
    ls->shortcutEmuSpeedDec     = "Verlaag emulatie snelheid";
    ls->shortcutThemeSwitch     = "Ander thema kiezen";
    ls->shortcutShowEmuProp     = "Dialoog: Emulatie eigenschappen";
    ls->shortcutShowVideoProp   = "Dialoog: Beeld";
    ls->shortcutShowAudioProp   = "Dialoog: Geluid";
    ls->shortcutShowCtrlProp    = "Dialoog: Besturing";
    ls->shortcutShowPerfProp    = "Dialoog: Prestatie";
    ls->shortcutShowSettProp    = "Dialoog: Instellingen";
    ls->shortcutShowPorts       = "Eigenschappen van poorten weergeven";
    ls->shortcutShowLanguage    = "Dialoog: Taaldialoog";
    ls->shortcutShowMachines    = "Dialoog: Machine menu";
    ls->shortcutShowShortcuts   = "Dialoog: Snelkoppelingen menu";
    ls->shortcutShowKeyboard    = "Toetsenbord editor weergeven"; 
    ls->shortcutShowMixer       = "Show Mixer";
    ls->shortcutShowDebugger    = "Debugger weergeven"; 
    ls->shortcutShowTrainer     = "Trainer weergeven"; 
    ls->shortcutShowHelp        = "Dialoog: Hulpdialoog";
    ls->shortcutShowAbout       = "Dialoog: Info blueMSX";
    ls->shortcutShowFiles       = "Tonen van bestandseigenschappen";
    ls->shortcutToggleSpriteEnable = "Tonen/Verbergen sprites";
    ls->shortcutToggleFdcTiming = "Synchroniseren/Niet synchroniseren"; 
    ls->shortcutToggleCpuTrace  = "Toelaten/niet toelaten CPU spoor"; 


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Geselecteerde toets:"; 
    ls->keyconfigMappedTo       = "Gekoppeld aan:"; 
    ls->keyconfigMappingScheme  = "Koppel Schema:"; 

    
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

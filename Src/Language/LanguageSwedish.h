/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageSwedish.h,v $
**
** $Revision: 1.32 $
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
#ifndef LANGUAGE_SWEDISH_H
#define LANGUAGE_SWEDISH_H

#include "LanguageStrings.h"

void langInitSwedish(LanguageStrings* ls) 
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "Kinesiska Förenklad";
    ls->langChineseTraditional  = "Kinesiska Traditionell";
    ls->langDutch               = "Holländska";
    ls->langEnglish             = "Engelska";
    ls->langFinnish             = "Finska";
    ls->langFrench              = "Franska";
    ls->langGerman              = "Tyska";
    ls->langItalian             = "Italienska";
    ls->langJapanese            = "Japanska";
    ls->langKorean              = "Koreanska";
    ls->langPolish              = "Polska";
    ls->langPortuguese          = "Portgiskiska";
    ls->langSpanish             = "Spanska";
    ls->langSwedish             = "Svenska";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "Enhet:";
    ls->textFilename            = "Filenamn:";
    ls->textFile                = "Fil";
    ls->textNone                = "Ingen";
    ls->textUnknown             = "Okänd";


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle            = "blueMSX - Varning";
    ls->warningDiscardChanges   = "Vill du stänga verktyget utan att spara dina ändringar?";
    ls->warningOverwriteFile    = "Vill du skriva över filen:";
    ls->errorTitle              = "blueMSX - Fel";
    ls->errorEnterFullscreen    = "Misslyckades att byta till fullskärm.           \n";
    ls->errorDirectXFailed      = "Misslyckades att skapa DirectX objekt.          \nAnvänder GDI istället.\nKontrollera video inställningarna.";
    ls->errorNoRomInZip         = "Kunde inte hitta en .rom fil i zip arkivet.";
    ls->errorNoDskInZip         = "Kunde inte hitta en .dsk fil i zip arkivet.";
    ls->errorNoCasInZip         = "Kunde inte hitta en .cas fil i zip arkivet.";
    ls->errorNoHelp             = "Kunde inte hitta hjälpfilen.";
    ls->errorStartEmu           = "Misslyckades att starta emulatorn.";
    ls->errorPortableReadonly   = "Flyttbar enhet är inte skrivbar";


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "Rom fil";
    ls->fileAll                 = "Alla Filer";
    ls->fileCpuState            = "CPU-tillstand";
    ls->fileDisk                = "Diskettfil";
    ls->fileCas                 = "Kasettfil";


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- ingen filhistoria -";
    ls->menuInsert              = "Sätt in";
    ls->menuEject               = "Ta ur";

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
    ls->menuCartInsertSpecial   = "Sätt in Special";
    ls->menuCartMegaRam         = "MegaRAM";
    ls->menuCartExternalRam     = "Externt RAM";

    ls->menuDiskInsertNew       = "Sätt in ny diskett fil";
    ls->menuDiskDirInsert       = "Sätt in mapp";
    ls->menuDiskAutoStart       = "Starta om efter insättning/urdagning";
    ls->menuCartAutoReset       = "Starta om efter insättning/urdagning";

    ls->menuCasRewindAfterInsert= "Spola tillbaka vid insättning";
    ls->menuCasUseReadOnly      = "Tillåt endast läsning av kasett";
    ls->lmenuCasSaveAs          = "Spara kasett som...";
    ls->menuCasSetPosition      = "Sätt position";
    ls->menuCasRewind           = "Spola tillbaka";

    ls->menuPrnFormfeed         = "Pappersmatning";

    ls->menuZoomNormal          = "Normal storlek";
    ls->menuZoomDouble          = "Dubbel storlek";
    ls->menuZoomFullscreen      = "Helskärm";
    
    ls->menuPropsEmulation      = "Emulering";
    ls->menuPropsVideo          = "Video";
    ls->menuPropsSound          = "Ljud";
    ls->menuPropsControls       = "Kontroller";
    ls->menuPropsPerformance    = "Prestanda";
    ls->menuPropsSettings       = "Inställningar";
    ls->menuPropsFile           = "Filer";
    ls->menuPropsLanguage       = "Språk";
    ls->menuPropsPorts          = "Portar";
    
    ls->menuVideoSource         = "Video ut källa";
    ls->menuVideoSourceDefault  = "Ingen videokälla inkopplad";
    ls->menuVideoChipAutodetect = "Automatisk detektering av video chip";    
    ls->menuVideoInSource       = "Video ut källa";
    ls->menuVideoInBitmap       = "Bitmap fil";

    ls->menuHelpHelp            = "Hjälp";
    ls->menuHelpAbout           = "Om blueMSX";

    ls->menuFileCart            = "Cartridge slot";
    ls->menuFileDisk            = "Diskettstaion";
    ls->menuFileCas             = "Kasett";
    ls->menuFilePrn             = "Skrivare";
    ls->menuFileLoadState       = "Läs in CPU-tillstand";
    ls->menuFileSaveState       = "Spara CPU-tillstand";
    ls->menuFileQLoadState      = "Snabbladda CPU-tillstand";
    ls->menuFileQSaveState      = "Snabbspara CPU-tillstand";
    ls->menuFileCaptureAudio    = "Spara ljud";
    ls->menuFileScreenShot      = "Spara skärmdump";
    ls->menuFileExit            = "Avsluta";

    ls->menuFileHarddisk        = "Hårddisk";
    ls->menuFileHarddiskNoPesent= "Inga enheter tillgängliga";

    ls->menuRunRun              = "Kör";
    ls->menuRunPause            = "Paus";
    ls->menuRunStop             = "Stanna";
    ls->menuRunSoftReset        = "Mjuk Omstart";
    ls->menuRunHardReset        = "Hård Omstart";
    ls->menuRunCleanReset       = "Full Omstart";

    ls->menuToolsMachine        = "Konfigureringsverktyg";
    ls->menuToolsShortcuts      = "Kortkommando Verktyg";
    ls->menuToolsKeyboard       = "Tangentbordseditor";
    ls->menuToolsMixer          = "Mixer";
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "Arkiv";
    ls->menuRun                 = "Emulering";
    ls->menuWindow              = "Fönster";
    ls->menuOptions             = "Egenskaper";
    ls->menuTools               = "Verktyg";
    ls->menuHelp                = "Hjälp";
    

    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Öppna";
    ls->dlgCancel               = "Avbryt";
    ls->dlgSave                 = "Spara";
    ls->dlgSaveAs               = "Spara Som...";
    ls->dlgRun                  = "Kör";
    ls->dlgClose                = "Stäng";

    ls->dlgLoadRom              = "blueMSX - Ladda en rom fil";
    ls->dlgLoadDsk              = "blueMSX - Ladda en dsk fil";
    ls->dlgLoadCas              = "blueMSX - Ladda en cas fil";
    ls->dlgLoadRomDskCas        = "blueMSX - Ladda en rom, dsk eller cas fil";
    ls->dlgLoadRomDesc          = "Välj en cartridge fil:";
    ls->dlgLoadDskDesc          = "Välj en diskett fil:";
    ls->dlgLoadCasDesc          = "Välj en kassett fil:";
    ls->dlgLoadRomDskCasDesc    = "Välj en fil:";
    ls->dlgLoadState            = "Ladda CPU-tillstand";
    ls->dlgSaveState            = "Spara CPU-tillstand";
    ls->dlgSaveCassette         = "blueMSX - Spara Kasett";
    ls->dlgInsertRom1           = "Sätt in ROM cartridge i slot 1";
    ls->dlgInsertRom2           = "Sätt in ROM cartridge i slot 2";
    ls->dlgInsertDiskA          = "Sätt in diskett i diskettstation A";
    ls->dlgInsertDiskB          = "Sätt in diskett i diskettstation B";
    ls->dlgInsertHarddisk       = "Sätt in hårddisk";
    ls->dlgInsertCas            = "Sätt in kassettband";
    ls->dlgRomType              = "Rom Typ:";

    ls->dlgTapeTitle            = "blueMSX - Kassettposition";
    ls->dlgTapeFrameText        = "Kassettposition";
    ls->dlgTapeCurrentPos       = "Aktuell position";
    ls->dlgTapeTotalTime        = "Total längd";
    ls->dlgTapeSetPosText       = "Kasettposition:";
    ls->dlgTapeCustom           = "Visa Specialfiler";
    ls->dlgTabPosition          = "Position";
    ls->dlgTabType              = "Typ";
    ls->dlgTabFilename          = "Filnamn";
    ls->dlgZipReset             = "Starta om efter insättning";
    
    ls->dlgAboutTitle           = "blueMSX - Information";

    ls->dlgLangLangText         = "Välj språk som blueMSX ska använda";
    ls->dlgLangLangTitle        = "blueMSX - Språk";

    ls->dlgAboutAbout           = "INFORMATION\r\n========";
    ls->dlgAboutVersion         = "Version:";
    ls->dlgAboutBuildNumber     = "Bygge:";
    ls->dlgAboutBuildDate       = "Datum:";
    ls->dlgAboutCreat           = "Skapat av Daniel Vik";
    ls->dlgAboutDevel           = "UTVECKLARE\r\n========";
    ls->dlgAboutThanks          = "TACK TILL\r\n=======";
    ls->dlgAboutLisence         = "LICENS\r\n"
                                  "=====\r\n\r\n"
                                  "Denna programvara är erbjuden 'som den är', utan någon explicit eller "
                                  "implicit garanti. Inte av någon händelse kommer författaren/na att hållas "
                                  "ansvariga för några skador orsakade av detta program.\r\n\r\n"
                                  "Besök www.bluemsx.com for mer information.";

    ls->dlgSavePreview          = "Förvisning";
    ls->dlgSaveDate             = "Tid Sparad:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Egenskaper";
    ls->propEmulation           = "Emulering";
    ls->propVideo               = "Video";
    ls->propSound               = "Ljud";
    ls->propControls            = "Kontroller";
    ls->propPerformance         = "Prestanda";
    ls->propSettings            = "Inställningar";
    ls->propFile                = "Filer";
    ls->propPorts               = "Portar";

    ls->propEmuGeneralGB        = "Allmänt ";
    ls->propEmuFamilyText       = "MSX familj:";
    ls->propEmuMemoryGB         = "Minne ";
    ls->propEmuRamSizeText      = "RAM storlek:";
    ls->propEmuVramSizeText     = "VRAM storlek:";
    ls->propEmuSpeedGB          = "Emuleringshastighet ";
    ls->propEmuSpeedText        = "Emuleringshastighet:";
    ls->propEmuFrontSwitchGB    = "Panasonic Switchar ";
    ls->propEmuFrontSwitch      = " Front switch";
    ls->propEmuFdcTiming        = "Slå Av Diskettstationstiming";
    ls->propEmuPauseSwitch      = " Paus switch";
    ls->propEmuAudioSwitch      = " MSX-AUDIO cartridge switch";
    ls->propVideoFreqText       = "Videofrekvens:";
    ls->propVideoFreqAuto       = "Automatisk";
    ls->propSndOversampleText   = "Översampling:";
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";              // New in 2.5
    ls->propSndMidiInGB         = "MIDI In ";
    ls->propSndMidiOutGB        = "MIDI Ut ";
    ls->propSndMidiChannel      = "MIDI Channel:";                      // New in 2.5
    ls->propSndMidiAll          = "Alla";                               // New in 2.5

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Monitor typ:";
    ls->propMonEmuText          = "Monitoremulering ";
    ls->propVideoTypeText       = "Videotyp:";
    ls->propWindowSizeText      = "Fönsterstorlek:";
    ls->propMonHorizStretch     = " Korrekt horizontell vidd";
    ls->propMonVertStretch      = " Vertikal stretch";
    ls->propMonDeInterlace      = " De-interlace";
    ls->propBlendFrames         = " Blanda efterföljande bilder";           // New in 2.5
    ls->propMonBrightness       = "Ljusstyrka:";
    ls->propMonContrast         = "Kontrast:";
    ls->propMonSaturation       = "Färgmättnad:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Scanlinjer:";
    ls->propMonColorGhosting    = "RF-modulering";
    ls->propMonEffectsGB        = "Effekter ";

    ls->propPerfVideoDrvGB      = "Videodriver ";
    ls->propPerfVideoDispDrvText= "Skärmdriver:";
    ls->propPerfFrameSkipText   = "Frame skipping:";
    ls->propPerfAudioDrvGB      = "Lkuddriver ";
    ls->propPerfAudioDrvText    = "Ljuddriver:";
    ls->propPerfAudioBufSzText  = "Storlek på ljudbuffer:";
    ls->propPerfEmuGB           = "Emulering ";
    ls->propPerfSyncModeText    = "Synkronisering:";
    ls->propFullscreenResText   = "Fullskärmsupplösning:";

    ls->propSndChipEmuGB        = "Emulering av ljudchip ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound        = " Moonsound";
    ls->propSndMt32ToGm         = " Konvertera MT-32 instrument till General MIDI";

    ls->propPortsLptGB          = "Parallellport ";
    ls->propPortsComGB          = "Serieportar ";
    ls->propPortsLptText        = "Port:";
    ls->propPortsCom1Text       = "Port 1:";
    ls->propPortsNone           = "Ingen";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC";
    ls->propPortsFile           = "Skriv till Fil";
    ls->propPortsComFile        = "Skicka till Fil";
    ls->propPortsOpenLogFile    = "Öppna Logfil";
    ls->propPortsEmulateMsxPrn  = "Emulering:";

    ls->propSetFileHistoryGB    = "Filhistoria ";
    ls->propSetFileHistorySize  = "Antal element i filhistorian:";
    ls->propSetFileHistoryClear = "Rensa historia";
    ls->propFileTypes           = " Registrera filtyper med blueMSX (.rom, .dsk, .cas, .sta)";
    ls->propWindowsEnvGB        = "Windows Miljö ";
    ls->propSetScreenSaver      = " Deaktivera skärmsläckare när blueMSX kör";
    ls->propDisableWinKeys      = " Avaktivera WIndows menyer när emulatorn kör";
    ls->propPriorityBoost       = " Höj prioriteten på blueMSX";
    ls->propScreenshotPng       = " Använd Portable Network Graphics (.png) skärmdump";
    ls->propClearHistory        = "Vill du verkligen radera filhistorien ?";
    ls->propOpenRomGB           = "Öppna Rom Dialog ";
    ls->propDefaultRomType      = "Default Rom Typ:";
    ls->propGuessRomType        = "Gissa Rom Typ";

    ls->propSettDefSlotGB       = "Dra och Släpp ";
    ls->propSettDefSlots        = "Sätt in Rom i:";
    ls->propSettDefSlot         = " Slot";
    ls->propSettDefDrives       = "Sätt in Diskett i:";
    ls->propSettDefDrive        = " Drive";

    ls->propThemeGB             = "Tema ";
    ls->propTheme               = "Tema";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Färg";
    ls->enumVideoMonGrey        = "Svartvit";
    ls->enumVideoMonGreen       = "Grön";
    ls->enumVideoMonAmber       = "Orange";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Ingen";
    ls->enumVideoEmuYc          = "Y/C kabel (skarp)";
    ls->enumVideoEmuMonitor     = "Monitor";
    ls->enumVideoEmuYcBlur      = "Brusig Y/C kabel (skarp)";
    ls->enumVideoEmuComp        = "Kompositkabel (suddig)";
    ls->enumVideoEmuCompBlur    = "Brusig komposit (suddig)";
    ls->enumVideoEmuScale2x     = "Scale 2x";
    ls->enumVideoEmuHq2x        = "Hq2x";

    ls->enumVideoSize1x         = "Normal - 320x200";
    ls->enumVideoSize2x         = "Dubbel - 640x400";
    ls->enumVideoSizeFullscreen = "Helskärm";

    ls->enumVideoDrvDirectDrawHW= "DirectDraw HW accel.";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Ingen";
    ls->enumVideoFrameskip1     = "1 bild";
    ls->enumVideoFrameskip2     = "2 bilder";
    ls->enumVideoFrameskip3     = "3 bilder";
    ls->enumVideoFrameskip4     = "4 bilder";
    ls->enumVideoFrameskip5     = "5 bilder";

    ls->enumSoundDrvNone        = "Inget ljud";
    ls->enumSoundDrvWMM         = "WMM driver";
    ls->enumSoundDrvDirectX     = "DirectX driver";

    ls->enumEmuSync1ms          = "Synkronisera till MSX refresh";
    ls->enumEmuSyncVblank       = "Synkronisera till PC Vertikal Blank";
    ls->enumEmuAsyncVblank      = "Asynchronous PC Vblank";             // New in 2.5
    ls->enumEmuSyncNone         = "Ingen";
    ls->enumEmuSyncAuto         = "Automatisk (snabb)";

    ls->enumControlsJoyNone     = "Ingen";
    ls->enumControlsJoyMouse    = "Mus";
    ls->enumControlsJoyTetris2Dongle = "Tetris 2 dosa";
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey dosa";
    ls->enumControlsJoy2Button = "2-knapps Styrspak";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision Styrspak";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 Dubbelsidig, 9 Sektorer";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 Dubbelsidig, 8 Sektorer";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 Enkelsidig, 9 Sektorer";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 Enkelsidig, 8 Sektorer";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 Dubbelsidig";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 Enkelsidig";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle               = "blueMSX - Konfigureringsverktyg";
    ls->confConfigText          = "Konfigurering:";
    ls->confSlotLayout          = "Slot mappning";
    ls->confMemory              = "Minne";
    ls->confChipEmulation       = "Chipemulering";
    ls->confChipExtras          = "Extra";

    ls->confOpenRom             = "Öppna Rom fil";
    ls->confSaveTitle           = "blueMSX - Spara Konfigurering";
    ls->confSaveText            = "Vill du skriva över maskinkonfigureringen? :";
    ls->confSaveAsTitle         = "Spara Konfigurering Som...";
    ls->confSaveAsMachineName   = "Maskinnamn:";
    ls->confDiscardTitle        = "blueMSX - Konfigurering";
    ls->confExitSaveTitle       = "blueMSX - Avsluta Konfigureringsverktyg";
    ls->confExitSaveText        = "Vill du stanga konfigureringsverktyget utan att spara dina ändringar?";

    ls->confSlotLayoutGB        = "Slot Layout ";
    ls->confSlotExtSlotGB       = "Externa slots ";
    ls->confBoardGB             = "Board ";
    ls->confBoardText           = "Board Typ:";
    ls->confSlotPrimary         = "Primär";
    ls->confSlotExpanded        = "Expanderad (4 subslottar)";

    ls->confSlotCart            = "Cartridge:";
    ls->confSlot                = "Slot";
    ls->confSubslot             = "Subslot";

    ls->confMemAdd               = "Ny...";
    ls->confMemEdit              = "Ändra...";
    ls->confMemRemove            = "Ta Bort";
    ls->confMemSlot              = "Slot";
    ls->confMemAddresss          = "Adress";
    ls->confMemType              = "Typ";
    ls->confMemRomImage          = "Rom Fil";
    
    ls->confChipVideoGB          = "Video ";
    ls->confChipVideoChip        = "Video Chip:";
    ls->confChipVideoRam         = "Video RAM:";
    ls->confChipSoundGB          = "Audio ";

    ls->confCmosGB               = "CMOS ";
    ls->confCmosEnable           = " Använd CMOS";
    ls->confCmosBattery          = " Använd Laddat Batteri";

    ls->confCpuFreqGB            = "CPU Frekvens ";
    ls->confZ80FreqText          = "Z80 Frekvens:";
    ls->confR800FreqText         = "R800 Frekvens:";
    ls->confFdcGB                = "Kontroller för Diskettenhet ";
    ls->confCFdcNumDrivesText    = "Antal Diskettenheter:";

    ls->confEditMemTitle         = "blueMSX - Ändra Minnesmap";
    ls->confEditMemGB            = "Detailjer ";
    ls->confEditMemType          = "Typ:";
    ls->confEditMemFile          = "Fil:";
    ls->confEditMemAddress       = "Adress";
    ls->confEditMemSize          = "Storlek";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Tangentkombination";
    ls->shortcutDescription     = "Kortkommando";

    ls->shortcutSaveConfig      = "blueMSX - Spara Konfigurering";
    ls->shortcutOverwriteConfig = "Vill du skriva över kortkommando konfigureringen:";
    ls->shortcutExitConfig      = "blueMSX - Avsluta Kortkommandoverktyget";
    ls->shortcutDiscardConfig   = "Vill du stanga konfigureringsverktyget utan att spara dina ändringar?";
    ls->shortcutSaveConfigAs    = "blueMSX - Spara Kortkommando Konfigurering Som...";
    ls->shortcutConfigName      = "Konfigurering:";
    ls->shortcutNewProfile      = "< Ny Profil >";
    ls->shortcutConfigTitle     = "blueMSX - Konfigurering av Kortkommandon";
    ls->shortcutAssign          = "Tilldela";
    ls->shortcutPressText       = "Tryck kortkommando:";
    ls->shortcutScheme          = "Kommandoschema:";
    ls->shortcutCartInsert1     = "Sätt in Cartridge 1";
    ls->shortcutCartRemove1     = "Ta ur Cartridge 1";
    ls->shortcutCartInsert2     = "Sätt in Cartridge 2";
    ls->shortcutCartRemove2     = "Ta ur Cartridge 2";
    ls->shortcutSpecialMenu1    = "Visa Specialrom meny för Cartridge 1";
    ls->shortcutSpecialMenu2    = "Visa Specialrom meny för Cartridge 2";
    ls->shortcutCartAutoReset   = "Starta om efter Insättning av Cartridge";
    ls->shortcutDiskInsertA     = "Sätt in Diskett A";
    ls->shortcutDiskDirInsertA  = "Sätt in Directory som Diskett A";
    ls->shortcutDiskRemoveA     = "Ta ur Diskett A";
    ls->shortcutDiskChangeA     = "Snabbbyt Diskett A";
    ls->shortcutDiskAutoResetA  = "Starta om efter Insättning av Diskett";
    ls->shortcutDiskInsertB     = "Sätt in Diskett B";
    ls->shortcutDiskDirInsertB  = "Sätt in Directory som Diskett B";
    ls->shortcutDiskRemoveB     = "Ta ur Diskett B";
    ls->shortcutCasInsert       = "Sätt in Kasett";
    ls->shortcutCasEject        = "Ta ur Kasett";
    ls->shortcutCasAutorewind   = "Slå Av/På Automatisk Tillbakaspolning";
    ls->shortcutCasReadOnly     = "Slå Av/På Skrivskydd på Kasettfiler";
    ls->shortcutCasSetPosition  = "Sätt Kasettposition";
    ls->shortcutCasRewind       = "Spola Tillabaka Kasett";
    ls->shortcutCasSave         = "Spara Kasett till fil";
    ls->shortcutPrnFormFeed     = "Form Feed på Skrivare";
    ls->shortcutCpuStateLoad    = "Ladda CPU-tillstånd";
    ls->shortcutCpuStateSave    = "Spara CPU-tillstånd";
    ls->shortcutCpuStateQload   = "Snabbladda CPU-tillstånd";
    ls->shortcutCpuStateQsave   = "Snabbspara CPU-tillstånd";
    ls->shortcutAudioCapture    = "Starta/Stanna Audioinspelning";
    ls->shortcutScreenshotOrig  = "Spar Skärmdump";
    ls->shortcutScreenshotSmall = "Spar Liten Ofiltrerad Skärmdump";
    ls->shortcutScreenshotLarge = "Spar Stor Ofiltrerad Skärmdump";
    ls->shortcutQuit            = "Avsluta blueMSX";
    ls->shortcutRunPause        = "Kör/Pause Emuleringen";
    ls->shortcutStop            = "Stanna Emuleringen";
    ls->shortcutResetHard       = "Hård Omstart";
    ls->shortcutResetSoft       = "Mjuk Omstart";
    ls->shortcutResetClean      = "Full Omstart";
    ls->shortcutSizeSmall       = "Växla till Liten Fönsterstorlek";
    ls->shortcutSizeNormal      = "Växla till Normal Fönsterstorlek";
    ls->shortcutSizeFullscreen  = "Växla till Fullskärm";
    ls->shortcutSizeMinimized   = "Minimera fönster";
    ls->shortcutToggleFullscren = "Växla till/från Fullskärm";
    ls->shortcutVolumeIncrease  = "Öka Volymen";
    ls->shortcutVolumeDecrease  = "Minska Volymen";
    ls->shortcutVolumeMute      = "Stäng av Volymen";
    ls->shortcutVolumeStereo    = "Växla mellan mono/stereo";
    ls->shortcutSwitchMsxAudio  = "Slå om MSX-AUDIO Switch";
    ls->shortcutSwitchFront     = "Slå om Panasonic Front Switch";
    ls->shortcutSwitchPause     = "Slå om Pause Switch";
    ls->shortcutToggleMouseLock = "Slå om Muslås";
    ls->shortcutEmuSpeedMax     = "Maximal Emuleringshastighet";
    ls->shortcutEmuSpeedToggle  = "Växla Mellan Normal och Max Emuleringshastighet";
    ls->shortcutEmuSpeedNormal  = "Minska Emuleringhastigheten";
    ls->shortcutEmuSpeedInc     = "Öka Emuleringhastigheten";
    ls->shortcutEmuSpeedDec     = "Minska Emuleringhastigheten";
    ls->shortcutThemeSwitch     = "Växla fönstretema";
    ls->shortcutShowEmuProp     = "Öppna Emuleringsfönstret";
    ls->shortcutShowVideoProp   = "Öppna Videofönstret";
    ls->shortcutShowAudioProp   = "Öppna Audiofönstret";
    ls->shortcutShowCtrlProp    = "Öppna Kontrollfönstret";
    ls->shortcutShowPerfProp    = "Öppna Prestandafönstret";
    ls->shortcutShowSettProp    = "Öppna Inställningsfönstret";
    ls->shortcutShowPorts       = "Visa Portegenskaper";
    ls->shortcutShowLanguage    = "Öppna Språkfönstret";
    ls->shortcutShowMachines    = "Öppna Konfigureringsverktyget";
    ls->shortcutShowShortcuts   = "Öppna Kortkommandoverktyget";
    ls->shortcutShowKeyboard    = "Visa Tangentbordseditor";
    ls->shortcutShowDebugger    = "Visa Debugger";
    ls->shortcutShowTrainer     = "Visa Trainer";
    ls->shortcutShowMixer       = "Visa Mixer";
    ls->shortcutShowHelp        = "Öppna Hjälpfönstret";
    ls->shortcutShowAbout       = "Öppna \"Om blueMSX\"-fönstret";
    ls->shortcutShowFiles       = "Öppna Filerfönstret";
    ls->shortcutToggleSpriteEnable = "Visa/Dölj Sprites";
    ls->shortcutToggleFdcTiming = "Slå på/av Diskettstationstiming";
    ls->shortcutToggleCpuTrace  = "Slå på/av CPU trace";


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Välj tangent:";
    ls->keyconfigMappedTo       = "Mappad till:";
    ls->keyconfigMappingScheme  = "Mapschema:";

    
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
    ls->romTypeF4deviceNormal   = "F4 Enhet Normal";
    ls->romTypeF4deviceInvert   = "F4 Enhet Inverted";
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
    ls->dbgDevF4Device          = "F4 Enhet";
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

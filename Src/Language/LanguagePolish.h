/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguagePolish.h,v $
**
** $Revision: 1.23 $
**
** $Date: 2006/07/05 06:58:37 $
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
#ifndef LANGUAGE_POLISH_H
#define LANGUAGE_POLISH_H

#include "LanguageStrings.h"
 
void langInitPolish(LanguageStrings* ls) 
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

    ls->textDevice              = "Urz¹dzenie:";
    ls->textFilename            = "Nazwa pliku:";
    ls->textFile                = "Plik";
    ls->textNone                = "Brak";
    ls->textUnknown             = "Nieznany";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - Uwaga";
    ls->warningDiscardChanges   = "Czy chcesz zniszczyæ zmiany?";
    ls->warningOverwriteFile    = "Czy chcesz nadpisaæ plik:"; // New in 2.5
    ls->errorTitle              = "blueMSX - b³¹d";
    ls->errorEnterFullscreen    = "Nie mogê prze³¹czyæ na pe³ny ekran.           \n";
    ls->errorDirectXFailed      = "Nie mogê stworzyæ obiektów DirectX.           \nPrze³¹czam w tryb GDI.\nSprawdŸ w³aœciwoœci wideo.";
    ls->errorNoRomInZip         = "Nie znaleziono pliku .rom w archiwum zip.";
    ls->errorNoDskInZip         = "Nie znaleziono pliku .dsk w archiwum zip.";
    ls->errorNoCasInZip         = "Nie znaleziono pliku .cas w archiwum zip.";
    ls->errorNoHelp             = "Nie znaleziono pliku pomocy blueMSX.";
    ls->errorStartEmu           = "Nie uda³o siê uruchomiæ emulatora MSX.";
    ls->errorPortableReadonly   = "Urz¹dzenie przenoœne - tylko do odczytu";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM image";
    ls->fileAll                 = "Wszystkie pliki";
    ls->fileCpuState            = "Stan CPU";
    ls->fileDisk                = "Obraz dysku";
    ls->fileCas                 = "Obraz taœmy";


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- brak ostatnich plików -";
    ls->menuInsert              = "Wybierz";
    ls->menuEject               = "Wysuñ";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "SCC Cartridge";
    ls->menuCartSCCPlus         = "SCC+ Kartrid¿";
    ls->menuCartFMPac           = "Kartrid¿ FM-PAC";
    ls->menuCartPac             = "Kartrid¿ PAC";
    ls->menuCartHBI55           = "Sony HBI-55 Cartridge";
    ls->menuCartInsertSpecial   = "W³ó¿ inny";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "Zewnêtrzny RAM";

    ls->menuDiskInsertNew       = "W³ó¿ nowy obraz dysku";              // New in 2.5
    ls->menuDiskDirInsert       = "Podepnij folder";
    ls->menuDiskAutoStart       = "Resetuj po zmianie dyskietki";
    ls->menuCartAutoReset       = "Resetuj po zmianie kartrid¿a";
    
    ls->menuCasRewindAfterInsert = "Najpierw przewiñ do pocz¹tku";
    ls->menuCasUseReadOnly       = "U¿ywaj kaset 'tylko do odczytu'";
    ls->lmenuCasSaveAs           = "Zapisz kasetê jako...";
    ls->menuCasSetPosition      = "Ustaw pozycjê";
    ls->menuCasRewind           = "Przewiñ do pocz¹tku";

    ls->menuPrnFormfeed         = "Wysuñ papier";

    ls->menuZoomNormal          = "Standardowe";
    ls->menuZoomDouble          = "Podwójne";
    ls->menuZoomFullscreen      = "Pe³ny ekran";
    
    ls->menuPropsEmulation      = "Emulacja";
    ls->menuPropsVideo          = "Obraz";
    ls->menuPropsSound          = "DŸwiêk";
    ls->menuPropsControls       = "Sterowanie";
    ls->menuPropsPerformance    = "Wydajnoœæ";
    ls->menuPropsSettings        = "Ustawienia";
    ls->menuPropsFile           = "Pliki";
    ls->menuPropsLanguage       = "Jêzyk";
    ls->menuPropsPorts          = "Porty";
    
    ls->menuVideoSource         = "ród³o wyjœcia 'Video Out'";                   // New in 2.5
    ls->menuVideoSourceDefault  = "Brak Ÿród³a dla 'Video Out'";      // New in 2.5
    ls->menuVideoChipAutodetect = "Autodetekcja koœci obrazu";    
    ls->menuVideoInSource       = "ród³o 'Video In'";                    // New in 2.5
    ls->menuVideoInBitmap       = "Plik bitmapy";                        // New in 2.5

    ls->menuHelpHelp            = "Tematy pomocy";
    ls->menuHelpAbout           = "O blueMSX...";

    ls->menuFileCart            = "Kartrid¿";
    ls->menuFileDisk            = "Stacja dyskietek";
    ls->menuFileCas             = "Kaseta";
    ls->menuFilePrn             = "Drukarka";
    ls->menuFileLoadState       = "Wczytaj stan CPU";
    ls->menuFileSaveState       = "Zapisz stan CPU";
    ls->menuFileQLoadState      = "Szybki odczyt stanu";
    ls->menuFileQSaveState      = "Szybki zapis stanu";
    ls->menuFileCaptureAudio    = "Przechwyæ dŸwiêk";
    ls->menuFileScreenShot      = "Zapisz ekran";
    ls->menuFileExit            = "Wyjœcie";

    ls->menuFileHarddisk        = "Dysk Twardy";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "Brak sterownika";             // New in 2.5

    ls->menuRunRun              = "Uruchom";
    ls->menuRunPause            = "Pauza";
    ls->menuRunStop             = "Zatrzymaj";
    ls->menuRunSoftReset        = "Miêkki reset";
    ls->menuRunHardReset        = "Twardy reset";
    ls->menuRunCleanReset       = "Pe³ny reset";

    ls->menuToolsMachine         = "Edytor komputerów";
    ls->menuToolsShortcuts      = "Edytor skrótów klawiaturowych";
    ls->menuToolsKeyboard       = "Edytor klawiatury";
    ls->menuToolsMixer          = "Mikser";
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "Plik";
    ls->menuRun                 = "Uruchamianie";
    ls->menuWindow              = "Okno";
    ls->menuOptions             = "Opcje";
    ls->menuTools                = "Narzêdzia";
    ls->menuHelp                = "Pomoc";
    

    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Otwórz";
    ls->dlgCancel               = "Anuluj";
    ls->dlgSave                 = "Zapisz";
    ls->dlgSaveAs               = "Zapisz jako...";
    ls->dlgRun                  = "Uruchom";
    ls->dlgClose                = "Zamknij";

    ls->dlgLoadRom              = "blueMSX - Wybierz plik rom do wczytania";
    ls->dlgLoadDsk              = "blueMSX - Wybierz plik dsk do wczytania";
    ls->dlgLoadCas              = "blueMSX - Wybierz plik cas do wczytania";
    ls->dlgLoadRomDskCas        = "blueMSX - Wybierz plik rom, dsk lub cas do wczytania";
    ls->dlgLoadRomDesc          = "Wybierz rom do wczytania:";
    ls->dlgLoadDskDesc          = "Wybierz dyskietkê do wczytania:";
    ls->dlgLoadCasDesc          = "Wybierz taœmê do wczytania:";
    ls->dlgLoadRomDskCasDesc    = "Wybierz rom, dyskietkê lub taœmê do wczytania:";
    ls->dlgLoadState            = "Wczytaj stan CPU";
    ls->dlgSaveState            = "Zapisz stan CPU";
    ls->dlgSaveCassette          = "blueMSX - Zapisz obraz kasety";
    ls->dlgInsertRom1           = "Wybierz kartrid¿ ROM dla slotu 1";
    ls->dlgInsertRom2           = "Wybierz kartrid¿ ROM dla slotu 2";
    ls->dlgInsertDiskA          = "Wybierz dyskietkê dla stacji A";
    ls->dlgInsertDiskB          = "Wybierz dyskietkê dla stacji B";
    ls->dlgInsertHarddisk       = "Pod³¹cz Twardy Dysk";                   // New in 2.5
    ls->dlgInsertCas            = "Wybierz kasetê";
    ls->dlgRomType              = "Typ romu:";

    ls->dlgTapeTitle            = "blueMSX - Pozycja taœmy";
    ls->dlgTapeFrameText        = "Pozycja taœmy";
    ls->dlgTapeCurrentPos       = "Obecna pozycja";
    ls->dlgTapeTotalTime        = "Czas ca³kowity";
    ls->dlgTapeSetPosText        = "Pozycja taœmy:";
    ls->dlgTapeCustom            = "Poka¿ dowolne pliki";
    ls->dlgTabPosition           = "Pozycja";
    ls->dlgTabType               = "Typ";
    ls->dlgTabFilename           = "Nazwa pliku";
    ls->dlgZipReset             = "Resettuj po zmianie";

    ls->dlgAboutTitle           = "blueMSX - O programie";

    ls->dlgLangLangText         = "Wybierz jêzyk dla blueMSX";
    ls->dlgLangLangTitle        = "blueMSX - Jêzyk";

    ls->dlgAboutAbout           = "O programie\r\n====";
    ls->dlgAboutVersion         = "Wersja:";
    ls->dlgAboutBuildNumber     = "Kompilacja:";
    ls->dlgAboutBuildDate       = "Data:";
    ls->dlgAboutCreat           = "Program Daniela Vika";
    ls->dlgAboutDevel           = "PROGRAMIŒCI\r\n========";
    ls->dlgAboutThanks          = "SPECJALNE PODZIÊKOWANIA DLA\r\n============";
    ls->dlgAboutLisence         = "LICENSE\r\n"
                                  "======\r\n\r\n"
                                  "This software is provided 'as-is', without any express or implied "
                                  "warranty. In no event will the author(s) be held liable for any damages "
                                  "arising from the use of this software.\r\n\r\n"
                                  "Visit www.bluemsx.com for more details.";

    ls->dlgSavePreview          = "Poka¿ podgl¹d";
    ls->dlgSaveDate             = "Czas zapisu:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - W³aœciwoœci";
    ls->propEmulation           = "Emulacja";
    ls->propVideo               = "Obraz";
    ls->propSound               = "DŸwiêk";
    ls->propControls            = "Sterowanie";
    ls->propPerformance         = "Wydajnoœæ";
    ls->propSettings             = "Ustawienia";
    ls->propFile                = "Pliki";
    ls->propPorts               = "Porty";
    
    ls->propEmuGeneralGB        = "Ogólne ";
    ls->propEmuFamilyText       = "Typ MSX:";
    ls->propEmuMemoryGB         = "Pamiêæ ";
    ls->propEmuRamSizeText      = "Rozmiar RAMu:";
    ls->propEmuVramSizeText     = "Rozmiar VRAMu:";
    ls->propEmuSpeedGB          = "Szybkoœæ emulacji ";
    ls->propEmuSpeedText        = "Szybkoœæ emulacji:";
    ls->propEmuFrontSwitchGB     = "Prze³¹czniki Panasonic ";
    ls->propEmuFrontSwitch       = " Prze³¹cznik g³ówny";
    ls->propEmuFdcTiming        = "Wy³¹cz timing stacji dyskietek";
    ls->propEmuPauseSwitch      = " Prze³¹cznik pauzy";
    ls->propEmuAudioSwitch       = " Prze³¹cznik kartrid¿a MSX-AUDIO";
    ls->propVideoFreqText       = "Czêstotliwoœæ obrazu:";
    ls->propVideoFreqAuto       = "Auto";
    ls->propSndOversampleText   = "Oversampling:";
    ls->propSndYkInGB           = "Wejœcie YK-01/YK-10/YK-20 ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI In ";
    ls->propSndMidiOutGB        = "MIDI Out ";
    ls->propSndMidiChannel      = "Kana³ MIDI:";                      // New in 2.5
    ls->propSndMidiAll          = "Wszystkie";                                // New in 2.5

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Typ monitora:";
    ls->propMonEmuText          = "Emulacja monitora:";
    ls->propVideoTypeText       = "Typ obrazu:";
    ls->propWindowSizeText      = "Rozmiar okna:";
    ls->propMonHorizStretch      = " Rozci¹gaj w poziomie";
    ls->propMonVertStretch       = " Rozci¹gaj w pionie";
    ls->propMonDeInterlace      = " Usuwaj przeplot";
    ls->propBlendFrames         = " Zlej ze sob¹ kolejne klatki";           // New in 2.5
    ls->propMonBrightness       = "Jasnoœæ:";
    ls->propMonColorGhosting    = "Modulator RF";
    ls->propMonContrast         = "Kontrast:";
    ls->propMonSaturation       = "Nasycenie:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Przeplot:";
    ls->propMonEffectsGB        = "Efekty ";

    ls->propPerfVideoDrvGB      = "Ustawienia Video";
    ls->propPerfVideoDispDrvText= "Sterownik obrazu:";
    ls->propPerfFrameSkipText   = "Gubienie klatek:";
    ls->propPerfAudioDrvGB      = "Ustawienia Audio";
    ls->propPerfAudioDrvText    = "Sterownik dŸwiêku:";
    ls->propPerfAudioBufSzText  = "Rozmiar bufora dŸwiêku:";
    ls->propPerfEmuGB           = "Emulacja ";
    ls->propPerfSyncModeText    = "Tryb synchronizacji:";
    ls->propFullscreenResText   = "Pe³ny ekran:";

    ls->propSndChipEmuGB        = "Emulacja dŸwiêku";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " Mapuj instrumenty MT-32 na General MIDI";

    ls->propPortsLptGB          = "Port równoleg³y ";
    ls->propPortsComGB          = "Port szeregowy ";
    ls->propPortsLptText        = "Port:";
    ls->propPortsCom1Text       = "Port 1:";
    ls->propPortsNone           = "Brak";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC";
    ls->propPortsFile           = "Drukuj do pliku";
    ls->propPortsComFile        = "Send to File";
    ls->propPortsOpenLogFile    = "Otwórz plik logu";
    ls->propPortsEmulateMsxPrn  = "Emulacja:";

    ls->propSetFileHistoryGB     = "Historia plików ";
    ls->propSetFileHistorySize   = "Iloœc elementów w historii plików:";
    ls->propSetFileHistoryClear  = "Wyczyœæ historiê";
    ls->propFileTypes            = " Skoja¿ pliki z blueMSX (.rom, .dsk, .cas, .sta)";
    ls->propWindowsEnvGB         = "Otoczenie Windows "; 
    ls->propSetScreenSaver       = " Wy³¹cz wygaszacz podczas pracy blueMSX";
    ls->propDisableWinKeys       = " Automatyczna konfiguracja klawiszy Windows w MSX"; 
    ls->propPriorityBoost       = " Podnieœ priorytet blueMSX";
    ls->propScreenshotPng       = " u¿ywaj PNG do zapisywania ekranów";  // New in 2.5
    ls->propClearHistory         = "Na pewno wyczyœciæ historiê plików?";
    ls->propOpenRomGB           = "Okno wyboru romu ";
    ls->propDefaultRomType      = "Domyœlny typ romu:";
    ls->propGuessRomType        = "Odgadnij typ romu";

    ls->propSettDefSlotGB       = "Przeci¹gnij-i-upuœæ ";
    ls->propSettDefSlots        = "W³ó¿ rom do:";
    ls->propSettDefSlot         = " Slot";
    ls->propSettDefDrives       = "W³ó¿ dyskietkê do:";
    ls->propSettDefDrive        = " Stacji";

    ls->propThemeGB             = "Temat ";
    ls->propTheme               = "Temat:";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Kolorowy";
    ls->enumVideoMonGrey        = "Czarno-bia³y";
    ls->enumVideoMonGreen       = "Zielony";
    ls->enumVideoMonAmber       = "Bursztynowy";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Brak";
    ls->enumVideoEmuYc          = "Kabel Y/C (ostry)";
    ls->enumVideoEmuMonitor     = "Monitor";
    ls->enumVideoEmuYcBlur      = "Zaszumiony kabel Y/C (ostry)";
    ls->enumVideoEmuComp        = "Kompozytowe (rozmyte)";
    ls->enumVideoEmuCompBlur    = "Zaszumione kompozytowe (rozmyte)";
    ls->enumVideoEmuScale2x     = "Skalowanie 2x";
    ls->enumVideoEmuHq2x        = "Hq2x";

    ls->enumVideoSize1x         = "Pojedyncza - 320x200";
    ls->enumVideoSize2x         = "Podwójna - 640x400";
    ls->enumVideoSizeFullscreen = "Pe³ny ekran";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw (sprzêtowy)"; 
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Brak";
    ls->enumVideoFrameskip1     = "1 klatka";
    ls->enumVideoFrameskip2     = "2 klatki";
    ls->enumVideoFrameskip3     = "3 klatki";
    ls->enumVideoFrameskip4     = "4 klatki";
    ls->enumVideoFrameskip5     = "5 klatek";

    ls->enumSoundDrvNone        = "Brak dŸwiêku";
    ls->enumSoundDrvWMM         = "Driver WMM";
    ls->enumSoundDrvDirectX     = "Driver DirectX";

    ls->enumEmuSync1ms          = "Synchronizuj z odœwie¿aniem MSX";
    ls->enumEmuSyncAuto         = "Auto (szybkie)";
    ls->enumEmuSyncNone         = "None";
    ls->enumEmuSyncVblank       = "Synchronizuj z synchronizacj¹ pionow¹ PC";
    ls->enumEmuAsyncVblank      = "Asynchronous PC Vblank";             // New in 2.5

    ls->enumControlsJoyNone     = "Brak";
    ls->enumControlsJoyMouse    = "Mysz";
    ls->enumControlsJoyTetris2Dongle = "Dongle Tetris 2";
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey Dongle";             // New in 2.5
    ls->enumControlsJoy2Button = "D¿ojstik 2-przyciskowy";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "D¿ojstik ColecoVision";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 Double Sided, 9 Sectors";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 Double Sided, 8 Sectors";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 Single Sided, 9 Sectors";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 Single Sided, 8 Sectors";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 Double Sided";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 Single Sided";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle                = "blueMSX - Edytor Konfiguracji Komputerów";
    ls->confConfigText           = "Konfiguracja";
    ls->confSlotLayout           = "Uk³ad slotów";
    ls->confMemory               = "Pamiêæ";
    ls->confChipEmulation        = "Uk³ad obrazu";
    ls->confChipExtras          = "Extras";

    ls->confOpenRom             = "Otwórz ROM";
    ls->confSaveTitle            = "blueMSX - Zapis konfiguracji";
    ls->confSaveText             = "Czy chcesz nadpisaæ konfiguracjê:";
    ls->confSaveAsTitle         = "Zapisz konfiguracjê jako...";
    ls->confSaveAsMachineName    = "Nazwa komputera:";
    ls->confDiscardTitle         = "blueMSX - Konfiguracja";
    ls->confExitSaveTitle        = "blueMSX - WyjdŸ z Edytora Konfiguracj";
    ls->confExitSaveText         = "Czy chcesz zignorowaæ zmiany w bierz¹cej konfiguracji?";

    ls->confSlotLayoutGB         = "Uk³ad slotów ";
    ls->confSlotExtSlotGB        = "Zewnêtrzne sloty ";
    ls->confBoardGB             = "Board ";
    ls->confBoardText           = "Board Type:";
    ls->confSlotPrimary          = "Podstawowy";
    ls->confSlotExpanded         = "Rozszerzone (cztery sub-sloty)";

    ls->confSlotCart             = "Kartrid¿";
    ls->confSlot                = "Slot";
    ls->confSubslot             = "Podslot";

    ls->confMemAdd               = "Dodaj...";
    ls->confMemEdit              = "Edytuj...";
    ls->confMemRemove            = "Usuñ";
    ls->confMemSlot              = "Slot";
    ls->confMemAddresss          = "Adres";
    ls->confMemType              = "Typ";
    ls->confMemRomImage          = "Obraz rom";
    
    ls->confChipVideoGB          = "Obraz ";
    ls->confChipVideoChip        = "Koœæ obrazu:";
    ls->confChipVideoRam         = "Pamiêæ RAM obrazu:";
    ls->confChipSoundGB          = "DŸwiêk ";

    ls->confCmosGB                = "CMOS ";
    ls->confCmosEnable            = " Enable CMOS";
    ls->confCmosBattery           = " Use Charged Battery";

    ls->confCpuFreqGB            = "CPU Frequency ";
    ls->confZ80FreqText          = "Z80 Frequency:";
    ls->confR800FreqText         = "R800 Frequency:";
    ls->confFdcGB                = "Floppy Disk Controller ";
    ls->confCFdcNumDrivesText    = "Number of Drives:";

    ls->confEditMemTitle         = "blueMSX - Edytuj Mapper";
    ls->confEditMemGB            = "Konfiguracja Mappera";
    ls->confEditMemType          = "Typ:";
    ls->confEditMemFile          = "Plik:";
    ls->confEditMemAddress       = "Adres";
    ls->confEditMemSize          = "Rozmiar";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Hotkey";
    ls->shortcutDescription     = "Skrót";

    ls->shortcutSaveConfig      = "blueMSX - Zapisz konfiguracjê";
    ls->shortcutOverwriteConfig = "Chcesz nadpisaæ konfiguracjê skrótów?:";
    ls->shortcutExitConfig      = "blueMSX - WyjdŸ z edytora skrótów";
    ls->shortcutDiscardConfig   = "Czy chcesz zignorowaæ zmiany w bierz¹cej konfiguracji?";
    ls->shortcutSaveConfigAs    = "blueMSX - Zapisz konfiguracjê skrótów jako...";
    ls->shortcutConfigName      = "Nazwa konfiguracji:";
    ls->shortcutNewProfile      = "< Nowy profil >";
    ls->shortcutConfigTitle     = "blueMSX - Edytor Mapowania Skrótów";
    ls->shortcutAssign          = "Przypisz";
    ls->shortcutPressText       = "Naciœnij przycisk(i) skrótu:";
    ls->shortcutScheme          = "Schemat mapowania:";
    ls->shortcutCartInsert1     = "W³ó¿ kartrid¿ 1";
    ls->shortcutCartRemove1     = "Wyjmij kartrid¿ 1";
    ls->shortcutCartInsert2     = "W³ó¿ kartrid¿ 2";
    ls->shortcutCartRemove2     = "Wyjmij kartrid¿ 2";
    ls->shortcutSpecialMenu1    = "Wyœwietl specjalne menu 1-go kartrid¿a";
    ls->shortcutSpecialMenu2    = "Wyœwietl specjalne menu 2-go kartrid¿a";
    ls->shortcutCartAutoReset   = "Resetuj emulator przy wk³adaniu kartrid¿a";
    ls->shortcutDiskInsertA     = "W³ó¿ dyskietkê A";
    ls->shortcutDiskDirInsertA  = "Pod³¹cz folder jako dyskietkê A";
    ls->shortcutDiskRemoveA     = "Wyjmij dyskietkê A";
    ls->shortcutDiskChangeA     = "Szybka zmiana dyskietki A";
    ls->shortcutDiskAutoResetA  = "Resetuj emulator przy wk³adaniu dyskietki A";
    ls->shortcutDiskInsertB     = "W³ó¿ dyskietkê B";
    ls->shortcutDiskDirInsertB  = "Pod³¹cz folder jako dyskietkê B";
    ls->shortcutDiskRemoveB     = "Wyjmij dyskietkê B";
    ls->shortcutCasInsert       = "W³ó¿ kasetê";
    ls->shortcutCasEject        = "Wyjmij kasetê";
    ls->shortcutCasAutorewind   = "Prze³¹cz auto-przewijanie kasety";
    ls->shortcutCasReadOnly     = "Prze³¹cz kasetê na 'tylko do odczytu'";
    ls->shortcutCasSetPosition  = "Ustaw pozycjê kasety";
    ls->shortcutCasRewind       = "Przewiñ kasetê";
    ls->shortcutCasSave         = "Zapisz obraz kasety";
    ls->shortcutPrnFormFeed     = "Wysuñ kartkê z drukarki";
    ls->shortcutCpuStateLoad    = "Wczytaj stan CPU";
    ls->shortcutCpuStateSave    = "Zapisz stan CPU";
    ls->shortcutCpuStateQload   = "Szybkie wczytanie stanu CPU";
    ls->shortcutCpuStateQsave   = "Szybki zapis stanu CPU";
    ls->shortcutAudioCapture    = "Uruchom/zatrzymaj zapis dŸwiêku";
    ls->shortcutScreenshotOrig  = "Zapisanie zrzutu ekranu";
    ls->shortcutScreenshotSmall = "Ma³y, niefiltrowany zrzut ekranu";
    ls->shortcutScreenshotLarge = "Du¿y, niefiltrowany zrzut ekranu";
    ls->shortcutQuit            = "Wyjœcie z blueMSX";
    ls->shortcutRunPause        = "Uruchom/zpauzuj emulacjê";
    ls->shortcutStop            = "Zatrzymaj emulacje";
    ls->shortcutResetHard       = "Twardy Reset";
    ls->shortcutResetSoft       = "Miêkki Reset";
    ls->shortcutResetClean      = "Ogólny Reset";
    ls->shortcutSizeSmall       = "Ustaw ma³y rozmiar okna";
    ls->shortcutSizeNormal      = "Ustaw normalny rozmiar okna";
    ls->shortcutSizeFullscreen  = "Ustaw pe³ny ekran";
    ls->shortcutSizeMinimized   = "Minimalizuj okno";
    ls->shortcutToggleFullscren = "Prze³¹czaj pe³ny ekran";
    ls->shortcutVolumeIncrease  = "Podg³oœnij dŸwiêk";
    ls->shortcutVolumeDecrease  = "Œcisz dŸwiêk";
    ls->shortcutVolumeMute      = "Wy³¹cz dŸwiêk";
    ls->shortcutVolumeStereo    = "Prze³¹cz mono/stereo";
    ls->shortcutSwitchMsxAudio  = "Prze³¹cznik MSX-AUDIO";
    ls->shortcutSwitchFront     = "Prze³¹cznik g³ówny Panasonic";
    ls->shortcutSwitchPause     = "Pauza";
    ls->shortcutToggleMouseLock = "Przechwytywanie myszy";
    ls->shortcutEmuSpeedMax     = "Maksymalna prêdkoœæ emulacji";
    ls->shortcutEmuSpeedToggle  = "Prze³¹cz maksymaln¹ prêdkoœæ emulacji";
    ls->shortcutEmuSpeedNormal  = "Normalna prêdkoœæ emulacji";
    ls->shortcutEmuSpeedInc     = "Zwiêksz prêdkoœæ emulacji";
    ls->shortcutEmuSpeedDec     = "Zmniejsz prêdkoœæ emulacji";
    ls->shortcutThemeSwitch     = "Zmieñ temat :)";
    ls->shortcutShowEmuProp     = "Wyœwietl okno w³aœciwoœci";
    ls->shortcutShowVideoProp   = "Wyœwietl ustawienia obrazu";
    ls->shortcutShowAudioProp   = "Wyœwietl ustawienia dŸwiêku";
    ls->shortcutShowCtrlProp    = "Wyœwietl ustawienia sterowania";
    ls->shortcutShowPerfProp    = "Wyœwietl ustawienia wydajnoœci";
    ls->shortcutShowSettProp    = "Wyœwietl ustawienia";
    ls->shortcutShowPorts       = "Wyœwietl w³aœciwoœci portów";
    ls->shortcutShowLanguage    = "Wyœwietl ustawienia jêzyka";
    ls->shortcutShowMachines    = "Wyœwietl Edytor Komputerów";
    ls->shortcutShowShortcuts   = "Wyœwietl Edytor Skrótów";
    ls->shortcutShowKeyboard    = "Poka¿ edytor klawiatury";
    ls->shortcutShowMixer       = "Poka¿ mikser";
    ls->shortcutShowDebugger    = "Poka¿ Debugger";
    ls->shortcutShowTrainer     = "Wyœwietl Trainer";
    ls->shortcutShowHelp        = "Wyœwietl Pomoc";
    ls->shortcutShowAbout       = "Wyœwietl informacje O programie";
    ls->shortcutShowFiles       = "Poka¿ w³aœciwoœci pliku";
    ls->shortcutToggleSpriteEnable = "Poka¿/ukryj sprite'y";
    ls->shortcutToggleFdcTiming = "W³./wy³. timing stacji dyskietek";
    ls->shortcutToggleCpuTrace  = "W³./wy³. œledzenie CPU";


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Wybrany klawisz:";
    ls->keyconfigMappedTo       = "Zmapowany na:";
    ls->keyconfigMappingScheme  = "Schemat mapowania:";

    
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

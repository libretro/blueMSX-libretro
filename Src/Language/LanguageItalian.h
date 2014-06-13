/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageItalian.h,v $
**
** $Revision: 1.33 $
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
#ifndef LANGUAGE_ITALIAN_H
#define LANGUAGE_ITALIAN_H

#include "LanguageStrings.h"
 
void langInitItalian(LanguageStrings* ls) 
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "Cinese Facilitato";
    ls->langChineseTraditional  = "Cinese Tradizionale";
    ls->langDutch               = "Olandese";
    ls->langEnglish             = "Inglese";
    ls->langFinnish             = "Finlandese";
    ls->langFrench              = "Francese";
    ls->langGerman              = "Tedesco";
    ls->langItalian             = "Italiano";
    ls->langJapanese            = "Giapponese";
    ls->langKorean              = "Coreano";
    ls->langPolish              = "Polacco";
    ls->langPortuguese          = "Portoghese";
    ls->langSpanish             = "Spagnolo";
    ls->langSwedish             = "Svedese";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "Tipo:"; 
    ls->textFilename            = "Nome:"; 
    ls->textFile                = "File"; 
    ls->textNone                = "Nessuno"; 
    ls->textUnknown             = "Sconosciuto";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - Avvertimento";
    ls->warningDiscardChanges   = "Vuoi annullare i cambiamenti apportati a questa configurazione ?"; 
    ls->warningOverwriteFile    = "Vuoi sovrascrivere il file:"; // New in 2.5
    ls->errorTitle              = "blueMSX - Errore";
    ls->errorEnterFullscreen    = "Non riesco ad attivare la visualizzazione a schermo intero.           \n";
    ls->errorDirectXFailed      = "Non riesco a creare gli oggetti DirectX.          \nUtilizzo GDI.\nControllo le proprietà Video.";
    ls->errorNoRomInZip         = "Nessun file .rom trovato nell'archivio zip.";
    ls->errorNoDskInZip         = "Nessun file .dsk trovato nell'archivio zip.";
    ls->errorNoCasInZip         = "Nessun file .cas trovato nell'archivio zip.";
    ls->errorNoHelp             = "Non trovo il file di Aiuto di blueMSX.";
    ls->errorStartEmu           = "Avvio dell'emulatore MSX fallito.";
    ls->errorPortableReadonly   = "Il dispositivo portatile è a sola lettura";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM image"; 
    ls->fileAll                 = "Tutti gli archivi"; 
    ls->fileCpuState             = "Stato della CPU"; 
    ls->fileDisk                = "Disk Image"; 
    ls->fileCas                 = "Tape Image"; 


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- nessun file recente -";
    ls->menuInsert              = "Inserisci";
    ls->menuEject               = "Rimuovi";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "Cartuccia SCC";
    ls->menuCartSCCPlus         = "Cartuccia SCC+";
    ls->menuCartFMPac           = "Cartuccia FM-PAC";
    ls->menuCartPac             = "Cartuccia PAC";
    ls->menuCartHBI55           = "Cartuccia Sony HBI-55"; 
    ls->menuCartInsertSpecial   = "Inserisci Speciale";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "RAM esterno"; 

    ls->menuDiskInsertNew       = "Inserisci nuovo disco floppy";              // New in 2.5
    ls->menuDiskDirInsert       = "Inserisci Directory"; 
    ls->menuDiskAutoStart       = "Riavvia dopo l'inserimento";
    ls->menuCartAutoReset       = "Riavvia dopo aver inserito/rimosso";

    ls->menuCasRewindAfterInsert= "Riavvolgi dopo l'inserimento";
    ls->menuCasUseReadOnly      = "Usa un'immagine nastro a sola lettura";
    ls->lmenuCasSaveAs          = "Salva l'immagine nastro con nome";
    ls->menuCasSetPosition      = "Imposta la posizione";
    ls->menuCasRewind           = "Riavvolgi";

    ls->menuPrnFormfeed         = "Pagina seguente"; 

    ls->menuZoomNormal          = "Dimensione piccola";
    ls->menuZoomDouble          = "Dimensione normale";
    ls->menuZoomFullscreen      = "Schermo intero";
    
    ls->menuPropsEmulation      = "Emulazione";
    ls->menuPropsVideo          = "Video";
    ls->menuPropsSound          = "Audio";
    ls->menuPropsControls       = "Controlli";
    ls->menuPropsPerformance    = "Prestazioni";
    ls->menuPropsSettings        = "Impostazioni";
    ls->menuPropsFile           = "File";
    ls->menuPropsLanguage       = "Linguaggi";
    ls->menuPropsPorts          = "Porte";
    
    ls->menuVideoSource         = "Fonte video";                   // New in 2.5
    ls->menuVideoSourceDefault  = "Fonte video non collegata";      // New in 2.5
    ls->menuVideoChipAutodetect = "Individuazione automatica";     
    ls->menuVideoInSource       = "Entrata video";                    // New in 2.5
    ls->menuVideoInBitmap       = "Bitmap File";                        // New in 2.5

    ls->menuHelpHelp            = "Argomenti";
    ls->menuHelpAbout           = "Informazioni su blueMSX";

    ls->menuFileCart            = "Cartuccia Slot";
    ls->menuFileDisk            = "Disco floppy";
    ls->menuFileCas             = "Cassetta";
    ls->menuFilePrn             = "Stampante"; 
    ls->menuFileLoadState       = "Carica lo stato della CPU";
    ls->menuFileSaveState       = "Salva lo stato della CPU";
    ls->menuFileQLoadState      = "Carica rapidamente lo stato";
    ls->menuFileQSaveState      = "Salva rapidamente lo stato";
    ls->menuFileCaptureAudio    = "Cattura l'audio";
    ls->menuFileScreenShot      = "Salva la schermata";
    ls->menuFileExit            = "Esci";

    ls->menuFileHarddisk        = "Disco rigido";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "Nessun controlli";             // New in 2.5

    ls->menuRunRun              = "Esegui";
    ls->menuRunPause            = "Pausa";
    ls->menuRunStop             = "Ferma";
    ls->menuRunSoftReset        = "Riavvio software";
    ls->menuRunHardReset        = "Riavvio hardware";
    ls->menuRunCleanReset       = "Riavvio complete";

    ls->menuToolsMachine         = "Editor della macchina";
    ls->menuToolsShortcuts      = "Editor degli shortcut";
    ls->menuToolsKeyboard       = "Editor delli controlli/tastiera"; 
    ls->menuToolsMixer          = "Mixer"; 
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "File";
    ls->menuRun                 = "Emulazione";
    ls->menuWindow              = "Finestra";
    ls->menuOptions             = "Opzioni";
    ls->menuTools                = "Strumenti";
    ls->menuHelp                = "Aiuto";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Aprire";
    ls->dlgCancel               = "Annulla";
    ls->dlgSave                 = "Salva";
    ls->dlgSaveAs               = "Salva con...";
    ls->dlgRun                  = "Esegui";
    ls->dlgClose                = "Chiudi";

    ls->dlgLoadRom              = "blueMSX - Seleziona un'immagine .rom da caricare";
    ls->dlgLoadDsk              = "blueMSX - Seleziona un'immagine dsk da caricare";
    ls->dlgLoadCas              = "blueMSX - Seleziona un'immagine cas da caricare";
    ls->dlgLoadRomDskCas        = "blueMSX - Seleziona un file .rom, .dsk, o .cas da caricare";
    ls->dlgLoadRomDesc          = "Scegli un'immagine .rom da caricare:";
    ls->dlgLoadDskDesc          = "Scegli un'immagine dischetto da caricare:";
    ls->dlgLoadCasDesc          = "Scegli un'immagine cassetta da caricare:";
    ls->dlgLoadRomDskCasDesc    = "Scegli l'immagine di una rom, di un dischetto, o di un nastro da caricare:";
    ls->dlgLoadState            = "Carica lo stato della CPU";
    ls->dlgSaveState            = "Salva lo stato della CPU";
    ls->dlgSaveCassette          = "blueMSX - Salva l'immagine nastro";
    ls->dlgInsertRom1           = "Inserisci una cartuccia ROM nello slot 1";
    ls->dlgInsertRom2           = "Inserisci una cartuccia ROM nello slot 2";
    ls->dlgInsertDiskA          = "Inserisci un'immagine dischetto nell'unità A";
    ls->dlgInsertDiskB          = "Inserisci un'immagine dischetto nell'unità B";
    ls->dlgInsertHarddisk       = "Inserisci uno disco rigido";                   // New in 2.5
    ls->dlgInsertCas            = "Inserisci un'immagine cassetta";
    ls->dlgRomType              = "Tipo rom:"; 

    ls->dlgTapeTitle            = "blueMSX - Posizione del nastro";
    ls->dlgTapeFrameText        = "Posizione del nastro";
    ls->dlgTapeCurrentPos       = "Posizione attuale";
    ls->dlgTapeTotalTime        = "Durata totale";
    ls->dlgTapeCustom            = "Mostra i files personalizzati";
    ls->dlgTapeSetPosText        = "Posizione del nastro:";
    ls->dlgTabPosition           = "Posizione";
    ls->dlgTabType               = "Tipo";
    ls->dlgTabFilename           = "Nome del file";
    ls->dlgZipReset             = "Riavvia dopo l'inserimento";

    ls->dlgAboutTitle           = "blueMSX - Informazioni";

    ls->dlgLangLangText         = "Scegli il linguaggio per l'interfaccia di blueMSX";
    ls->dlgLangLangTitle        = "blueMSX - Lingua";

    ls->dlgAboutAbout           = "INFORMAZIONI\r\n====";
    ls->dlgAboutVersion         = "Versione:";
    ls->dlgAboutBuildNumber     = "Compilazione:";
    ls->dlgAboutBuildDate       = "Data:";
    ls->dlgAboutCreat           = "Creato da Daniel Vik";
    ls->dlgAboutDevel           = "SVILUPPATORI\r\n========";
    ls->dlgAboutThanks          = "RINGRAZIAMENTI SPECIALI A\r\n============";
    ls->dlgAboutLisence         = "LICENZA\r\n"
                                  "======\r\n\r\n"
                                  "Questo programma è fornito \"così com'è\", senza alcuna esplicita o implicita "
                                  "garanzia. In nessun caso l'autore potrà essere ritenuto responsabile per "
                                  "qualunque danno derivante dall'uso di questo programma.\r\n\r\n"
                                  "Visita www.bluemsx.com per maggiori dettagli.";

    ls->dlgSavePreview          = "Mostra anteprima"; 
    ls->dlgSaveDate             = "Data:"; 


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Proprietà";
    ls->propEmulation           = "Emulazione";
    ls->propVideo               = "Video";
    ls->propSound               = "Audio";
    ls->propControls            = "Controlli";
    ls->propPerformance         = "Prestazioni";
    ls->propSettings            = "Impostazioni";
    ls->propFile                = "File"; 
    ls->propPorts               = "Porte";
    
    ls->propEmuGeneralGB        = "Generale ";
    ls->propEmuFamilyText       = "Modello MSX:";
    ls->propEmuMemoryGB         = "Memoria ";
    ls->propEmuRamSizeText      = "Dimensioni della RAM:";
    ls->propEmuVramSizeText     = "Dimensioni della VRAM:";
    ls->propEmuSpeedGB          = "Velocità di emulazione ";
    ls->propEmuSpeedText        = "Velocità di emulazione:";
    ls->propEmuFrontSwitchGB    = "Switches Panasonic "; 
    ls->propEmuFrontSwitch      = " Front Switch"; 
    ls->propEmuFdcTiming        = "Non sincronizzare floppy disk"; 
    ls->propEmuPauseSwitch      = " Pause Switch"; 
    ls->propEmuAudioSwitch      = " MSX-AUDIO cartridge switch"; 
    ls->propVideoFreqText       = "Frequenza Video:"; 
    ls->propVideoFreqAuto       = "Auto"; 
    ls->propSndOversampleText   = "Oversample:";
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI In "; 
    ls->propSndMidiOutGB        = "MIDI Out "; 
    ls->propSndMidiChannel      = "Canale MIDI:";                      // New in 2.5
    ls->propSndMidiAll          = "Tutti";                                // New in 2.5

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Tipo monitor:";
    ls->propMonEmuText          = "Emulazione monitor:";
    ls->propVideoTypeText       = "Tipo di video:";
    ls->propWindowSizeText      = "Dimensioni della finestra:";
    ls->propMonHorizStretch      = " Allungamento Orizzontale";
    ls->propMonVertStretch       = " Allungamento Verticale";
    ls->propMonDeInterlace      = " De-interlace"; 
    ls->propBlendFrames         = " Mescolare fotogramma consecutiva";           // New in 2.5
    ls->propMonBrightness       = "Luminosità:";
    ls->propMonContrast         = "Contrasto:";
    ls->propMonSaturation       = "Saturazione:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Scanlines:";
    ls->propMonColorGhosting    = "Modulatore RF"; 
    ls->propMonEffectsGB        = "Effeti "; 

    ls->propPerfVideoDrvGB      = "Driver video ";
    ls->propPerfVideoDispDrvText= "Driver per il video:";
    ls->propPerfFrameSkipText   = "Salto fotogrammi:";
    ls->propPerfAudioDrvGB      = "Driver audio ";
    ls->propPerfAudioDrvText    = "Driver per l'audio:";
    ls->propPerfAudioBufSzText  = "Dimensioni del buffer audio:";
    ls->propPerfEmuGB           = "Emulazione ";
    ls->propPerfSyncModeText    = "SYNC Modo sincronizzato:";
    ls->propFullscreenResText   = "Risoluzione schermo intero:"; 

    ls->propSndChipEmuGB        = "Emulazione del chip audio ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " Accordare strumenti MT-32 a General MIDI"; 

    ls->propPortsLptGB          = "Porta parallela ";
    ls->propPortsComGB          = "Porta seriale";
    ls->propPortsLptText        = "Porta:";
    ls->propPortsCom1Text       = "Porta 1:";
    ls->propPortsNone           = "Nessuna";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC"; 
    ls->propPortsFile           = "Stampare in archivio"; 
    ls->propPortsComFile        = "Inviare verso archivio"; 
    ls->propPortsOpenLogFile    = "Aprire un archivio di dati"; 
    ls->propPortsEmulateMsxPrn  = "Emulazione:"; 

    ls->propSetFileHistoryGB     = "Files recenti ";
    ls->propSetFileHistorySize   = "Numero di voci in Files recenti:";
    ls->propSetFileHistoryClear  = "Cancella la lista";
    ls->propFileTypes            = " Associa tipi file a blueMSX (.rom, .dsk, .cas, .sta)"; 
    ls->propWindowsEnvGB         = "Ambiente Windows";
    ls->propSetScreenSaver       = " Disabilita il salvaschermo durante l'esecuzione di blueMSX";
    ls->propDisableWinKeys       = " Funzioni MSX automatiche con il menu tasti di Windows"; 
    ls->propPriorityBoost       = " Dare a blueMSX una precedenza elevata";
    ls->propScreenshotPng       = " Utilizzare il formato PNG per le salvataggio della schermata";  // New in 2.5
    ls->propClearHistory         = "Desiderate realmente cancellare la lista ?";
    ls->propOpenRomGB           = "Dialogo di apertura file rom "; 
    ls->propDefaultRomType      = "Tipo rom di default:"; 
    ls->propGuessRomType        = "Individua il tipo"; 

    ls->propSettDefSlotGB       = "Drag and Drop "; 
    ls->propSettDefSlots        = "Inserisci cartuccia in:"; 
    ls->propSettDefSlot         = " Slot"; 
    ls->propSettDefDrives       = "Inerisci disco in:"; 
    ls->propSettDefDrive        = " Floppy"; 

    ls->propThemeGB             = "Tema ";
    ls->propTheme               = "Tema";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Colore";
    ls->enumVideoMonGrey        = "Bianco e nero";
    ls->enumVideoMonGreen       = "Verde";
    ls->enumVideoMonAmber       = "Ambra";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Nessuna";
    ls->enumVideoEmuYc          = "Cavo Y/C (Sharp)";
    ls->enumVideoEmuMonitor     = "Monitor"; 
    ls->enumVideoEmuYcBlur      = "Cavo Y/C rumoroso (Sharp)";
    ls->enumVideoEmuComp        = "Composito (Sfocato)";
    ls->enumVideoEmuCompBlur    = "Composito rumoroso (Sfocato)";
    ls->enumVideoEmuScale2x     = "Dimensioni doppie";
    ls->enumVideoEmuHq2x        = "Hq2x"; 

    ls->enumVideoSize1x         = "Normale - 320x200";
    ls->enumVideoSize2x         = "Doppia - 640x400";
    ls->enumVideoSizeFullscreen = "Schermo intero";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw HW accel.";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Nessuno";
    ls->enumVideoFrameskip1     = "1 fotogramma";
    ls->enumVideoFrameskip2     = "2 fotogrammi";
    ls->enumVideoFrameskip3     = "3 fotogrammi";
    ls->enumVideoFrameskip4     = "4 fotogrammi";
    ls->enumVideoFrameskip5     = "5 fotogrammi";

    ls->enumSoundDrvNone        = "Nessun suono";
    ls->enumSoundDrvWMM         = "Driver WMM";
    ls->enumSoundDrvDirectX     = "Driver DirectX";

    ls->enumEmuSync1ms          = "Sinc su MSX refresh"; 
    ls->enumEmuSyncAuto         = "Automatico (veloce)"; 
    ls->enumEmuSyncNone         = "Nessuno"; 
    ls->enumEmuSyncVblank       = "Sinc su PC Vertical Blank"; 
    ls->enumEmuAsyncVblank      = "PC Vblank asincrono";             // New in 2.5

    ls->enumControlsJoyNone     = "Nessuno";
    ls->enumControlsJoyMouse    = "Mouse";
    ls->enumControlsJoyTetris2Dongle = "Tetris 2 Dongle"; 
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey Dongle";             // New in 2.5
    ls->enumControlsJoy2Button = "2-button Joystick";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision Joystick";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 doppio faccia, 9 settori";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 doppio faccia, 8 settori";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 semplice faccia, 9 settori";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 semplice faccia, 8 settori";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 doppio faccia";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 semplice faccia";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle                = "blueMSX - Editor di configurazione della macchina";
    ls->confConfigText           = "Configurazione";
    ls->confSlotLayout           = "Disposizione degli slots";
    ls->confMemory               = "Memoria";
    ls->confChipEmulation        = "Emulazione del chip";
    ls->confChipExtras          = "Extra"; 

    ls->confOpenRom             = "Aprire ROM image"; 
    ls->confSaveTitle            = "blueMSX - Salva la configurazione";
    ls->confSaveText             = "Vuoi sovrascrivere la configurazione della macchina?";
    ls->confSaveAsTitle         = "Salva con..."; 
    ls->confSaveAsMachineName    = "Nome della macchina:";
    ls->confDiscardTitle         = "blueMSX - Configurazione";
    ls->confExitSaveTitle        = "blueMSX - Esci dall'editor di configurazione";
    ls->confExitSaveText         = "Vuoi annullare i cambiamenti apportati alla configurazione attuale?";

    ls->confSlotLayoutGB         = "Disposizione degli slots ";
    ls->confSlotExtSlotGB        = "Slots esterni ";
    ls->confBoardGB             = "Sistema "; 
    ls->confBoardText           = "Sistema tipo:"; 
    ls->confSlotPrimary          = "Primario";
    ls->confSlotExpanded         = "Espanso (4 subslots)";

    ls->confSlotCart             = "Cartuccia 1:";
    ls->confSlot                = "Slot"; 
    ls->confSubslot             = "Subslot"; 

    ls->confMemAdd               = "Aggiungi...";
    ls->confMemEdit              = "Modifica...";
    ls->confMemRemove            = "Rimuovi";
    ls->confMemSlot              = "Slot";
    ls->confMemAddresss          = "Indirizzo";
    ls->confMemType              = "Tipo";
    ls->confMemRomImage          = "Immagine .ROM";
    
    ls->confChipVideoGB          = "Video ";
    ls->confChipVideoChip        = "Chip video:";
    ls->confChipVideoRam         = "RAM video:";
    ls->confChipSoundGB          = "Audio ";

    ls->confCmosGB                = "CMOS ";
    ls->confCmosEnable            = "Abilita CMOS";
    ls->confCmosBattery           = "Battery charged";

    ls->confCpuFreqGB            = "Frequenza CPU "; 
    ls->confZ80FreqText          = "Frequenza Z80:"; 
    ls->confR800FreqText         = "Frequenza R800:"; 
    ls->confFdcGB                = "Controller Floppy Disk "; 
    ls->confCFdcNumDrivesText    = "Numero di unità:"; 

    ls->confEditMemTitle         = "blueMSX - Modifica il Mapper";
    ls->confEditMemGB            = "Dettagli del Mapper ";
    ls->confEditMemType          = "Tipo:";
    ls->confEditMemFile          = "File:";
    ls->confEditMemAddress       = "Indirizzo";
    ls->confEditMemSize          = "Dimensioni";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Hotkey"; 
    ls->shortcutDescription     = "Shortcut"; 

    ls->shortcutSaveConfig      = "blueMSX - Salva la configurazione";
    ls->shortcutOverwriteConfig = "Vuoi sovrascrivere la configurazione della macchina?";
    ls->shortcutExitConfig      = "blueMSX - Esci dall'editor di scorciatoie ";
    ls->shortcutDiscardConfig   = "Vuoi annullare i cambiamenti apportati alla configurazione attuale?";
    ls->shortcutSaveConfigAs    = "blueMSX - Salva la configurazione con nome ";
    ls->shortcutConfigName      = "Nome della configurazione";
    ls->shortcutNewProfile      = "< Nuovo Profilo >";
    ls->shortcutConfigTitle     = "blueMSX - Editor degli shortcut";
    ls->shortcutAssign          = "Assegna";
    ls->shortcutPressText       = "Premi tasto(i) della scorciatoia :";
    ls->shortcutScheme          = "Schema:";
    ls->shortcutCartInsert1     = "Inserimento cartuccia nello slot 1";
    ls->shortcutCartRemove1     = "Rimozione cartuccia dallo slot 1 ";
    ls->shortcutCartInsert2     = "Inserimento cartuccia nello slot 2";
    ls->shortcutCartRemove2     = "Rimozione cartuccia dallo slot 2";
    ls->shortcutSpecialMenu1    = "Mostra menu Speciale per la cartuccia dello slot 1";
    ls->shortcutSpecialMenu2    = "Mostra menu speciale per la cartuccia dello slot 2";
    ls->shortcutCartAutoReset   = "Riavvio emulatore dopo inserimento cartuccia";
    ls->shortcutDiskInsertA     = "Inserimento dischetto nell'unità A";
    ls->shortcutDiskDirInsertA  = "Inserisci una directory come disco A"; 
    ls->shortcutDiskRemoveA     = "Rimozione dischetto nell'unità A";
    ls->shortcutDiskChangeA     = "Cambio rapido del dischetto nell'unità A";
    ls->shortcutDiskAutoResetA  = "Riavvio emulatore dopo inserimento floppy nell'unità A";
    ls->shortcutDiskInsertB     = "Inserimento dischetto nell'unità B";
    ls->shortcutDiskDirInsertB  = "Inserisci una directory come disco B"; 
    ls->shortcutDiskRemoveB     = "Rimozione dischetto nell'unità B";
    ls->shortcutCasInsert       = "Inserimento immagine cassetta";
    ls->shortcutCasEject        = "Rimozione immagine cassetta";
    ls->shortcutCasAutorewind   = "Riavvolgimento automatico o meno della cassetta";
    ls->shortcutCasReadOnly     = "Cassetta in modo solo lettura o meno";
    ls->shortcutCasSetPosition  = "Impostazione della posizione del nastro";
    ls->shortcutCasRewind       = "Riavvolgimento cassetta";
    ls->shortcutCasSave         = "Salvataggio immagine nastro con nome";
    ls->shortcutPrnFormFeed     = "Passaggio alla pagina seguente"; 
    ls->shortcutCpuStateLoad    = "Caricamento dello stato della CPU";
    ls->shortcutCpuStateSave    = "Salvataggio dello stato della CPU";
    ls->shortcutCpuStateQload   = "Caricamento rapido dello stato della CPU";
    ls->shortcutCpuStateQsave   = "Salvataggio rapido dello stato della CPU";
    ls->shortcutAudioCapture    = "Start/Stop audio capture ";
    ls->shortcutScreenshotOrig  = "Salvataggio della schermata";
    ls->shortcutScreenshotSmall = "Salvataggio breve della schermata senza filtro";
    ls->shortcutScreenshotLarge = "Salvataggio completo della schermata senza filtro";
    ls->shortcutQuit            = "Uscita da blueMSX";
    ls->shortcutRunPause        = "Start/Pausa Emulatore";
    ls->shortcutStop            = "Stop Emulatore";
    ls->shortcutResetHard       = "Riavvio hardware";
    ls->shortcutResetSoft       = "Riavvio software";
    ls->shortcutResetClean      = "Riavvio complete";
    ls->shortcutSizeSmall       = "Passare in modalità finestra piccola";
    ls->shortcutSizeNormal      = "Passare in modalità finestra normale";
    ls->shortcutSizeFullscreen  = "Passare in modalità schermo intero";
    ls->shortcutSizeMinimized   = "Minimizzi la finestra"; 
    ls->shortcutToggleFullscren = "Bilanciamento modalità finestra/schermo intero";
    ls->shortcutVolumeIncrease  = "Aumentare il volume del suono";
    ls->shortcutVolumeDecrease  = "Diminuire il volume del suono";
    ls->shortcutVolumeMute      = "Stabilire il suono";
    ls->shortcutVolumeStereo    = "Bilanciamento mono/stereo";
    ls->shortcutSwitchMsxAudio  = "Attivare l' MSX-AUDIO switch";
    ls->shortcutSwitchFront     = "Attivare il Panasonic front switch";
    ls->shortcutSwitchPause     = "Attivare il Pause switch"; 
    ls->shortcutToggleMouseLock = "Attivare/disattivare il blocco del mouse";
    ls->shortcutEmuSpeedMax     = "Velocità massima dell'emulazione";
    ls->shortcutEmuSpeedToggle  = "Bilencia velocità massima dell'emulazione"; 
    ls->shortcutEmuSpeedNormal  = "Velocità normale dell'emulazione";
    ls->shortcutEmuSpeedInc     = "Aumentare la velocità dell'emulazione";
    ls->shortcutEmuSpeedDec     = "Diminuire la velocità dell'emulazione";
    ls->shortcutThemeSwitch     = "Cambia il tema";
    ls->shortcutShowEmuProp     = "Mostra proprietà  dell'emulazione";
    ls->shortcutShowVideoProp   = "Mostra proprietà Video";
    ls->shortcutShowAudioProp   = "Mostra proprietà Audio";
    ls->shortcutShowCtrlProp    = "Mostra proprietà Controlli";
    ls->shortcutShowPerfProp    = "Mostra proprietà Prestazioni";
    ls->shortcutShowSettProp    = "Mostra proprietà Impostazioni";
    ls->shortcutShowPorts       = "Mostri proprietà delle Porte";
    ls->shortcutShowLanguage    = "Mostra dialogo per la scelta del linguaggio";
    ls->shortcutShowMachines    = "Mostra editor della macchina";
    ls->shortcutShowShortcuts   = "Mostra editor degli shortcut";
    ls->shortcutShowKeyboard    = "Mostri editor delli controlli/tastiera"; 
    ls->shortcutShowDebugger    = "Mostri Debugger"; 
    ls->shortcutShowTrainer     = "Mostri Trainer"; 
    ls->shortcutShowMixer       = "Mostri Mixer"; 
    ls->shortcutShowHelp        = "Accedere agli argomenti dell'aiuto";
    ls->shortcutShowAbout       = "Vedere informazioni su blueMSX";
    ls->shortcutShowFiles       = "Mostri proprietà Files";
    ls->shortcutToggleSpriteEnable = "Mostra/Disattiva Sprites";
    ls->shortcutToggleFdcTiming = "Sincronizzare o no disco floppy"; 
    ls->shortcutToggleCpuTrace  = "Convalidare/invalidare la traccia della CPU"; 


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Tasto scelto:"; 
    ls->keyconfigMappedTo       = "Assegnato a:"; 
    ls->keyconfigMappingScheme  = "Configurazione dei Tasti:"; 

    
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

    ls->aboutScrollThanksTo     = "Ringraziamenti speciali a: ";
    ls->aboutScrollAndYou       = "e VOI !!!!";
};

#endif

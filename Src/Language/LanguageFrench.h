/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageFrench.h,v $
**
** $Revision: 1.31 $
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
#ifndef LANGUAGE_FRENCH_H
#define LANGUAGE_FRENCH_H

#include "LanguageStrings.h"

void langInitFrench(LanguageStrings* ls)
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "Chinois Simplifié";
    ls->langChineseTraditional  = "Chinois Traditionnel";
    ls->langDutch               = "Hollandais";
    ls->langEnglish             = "Anglais";
    ls->langFinnish             = "Finlandais";
    ls->langFrench              = "Français";
    ls->langGerman              = "Allemand";
    ls->langItalian             = "Italien";
    ls->langJapanese            = "Japonais";
    ls->langKorean              = "Coréen";
    ls->langPolish              = "Polonais";
    ls->langPortuguese          = "Portugais";
    ls->langSpanish             = "Espagnol";
    ls->langSwedish             = "Suédois";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "Type:";
    ls->textFilename            = "Fichier:";
    ls->textFile                = "Fichier";
    ls->textNone                = "Aucun";
    ls->textUnknown             = "Aucun";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - Avertissement";
    ls->warningDiscardChanges   = "Voulez-vous ignorer les changements apportés à cette configuration ?"; 
    ls->warningOverwriteFile    = "Voulez-vous remplacer ce fichier ?:"; // New in 2.5
    ls->errorTitle              = "blueMSX - Erreur";
    ls->errorEnterFullscreen    = "Impossible de passer en mode plein écran.              \n";
    ls->errorDirectXFailed      = "Impossible de créer les objets DirectX.              \nUVeuillez sélectionner GDI à la place.\ndans le menu \"Video\".";
    ls->errorNoRomInZip         = "Il n'y a pas de fichier .rom dans votre archive zip.";
    ls->errorNoDskInZip         = "Il n'y a pas de fichier .dsk dans votre archive zip.";
    ls->errorNoCasInZip         = "Il n'y a pas de fichier .cas dans votre archive zip";
    ls->errorNoHelp             = "Impossible de trouver le fichier d'aide.";
    ls->errorStartEmu           = "Erreur lors du lancement de l'émulateur.";
    ls->errorPortableReadonly   = "Ce dispositif portable est uniquement en mode écriture";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "Image-ROM";
    ls->fileAll                 = "Tous les fichiers";
    ls->fileCpuState            = "Image du CPU";
    ls->fileDisk                = "Image-disque";
    ls->fileCas                 = "Image-cassette";


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- pas de fichier récent -";
    ls->menuInsert              = "Insérer";
    ls->menuEject               = "Ejecter";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "Cartouche SCC";
    ls->menuCartSCCPlus         = "Cartouche SCC+";
    ls->menuCartFMPac           = "Cartouche FM-PAC";
    ls->menuCartPac             = "Cartouche PAC";
    ls->menuCartHBI55           = "Cartouche Sony HBI-55"; 
    ls->menuCartInsertSpecial   = "Insérer Spécial";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "RAM externe";

    ls->menuDiskInsertNew       = "Insérer nouvelle disquette";              // New in 2.5
    ls->menuDiskDirInsert       = "Insérer répertoire"; 
    ls->menuDiskAutoStart       = "Reset après insertion";
    ls->menuCartAutoReset       = "Reset après insérer/éjecter";

    ls->menuCasRewindAfterInsert= "Rembobiner après insertion";
    ls->menuCasUseReadOnly      = "Utiliser l'image de la cassette en mode lecture seule";
    ls->lmenuCasSaveAs          = "Sauver l'image de la cassette sous...";
    ls->menuCasSetPosition      = "Définir la position";
    ls->menuCasRewind           = "Rembobiner ";

    ls->menuPrnFormfeed         = "Page suivante"; 

    ls->menuZoomNormal          = "Petite fenêtre";
    ls->menuZoomDouble          = "Fenêtre normale";
    ls->menuZoomFullscreen      = "Plein écran";

    ls->menuPropsEmulation      = "Emulation";
    ls->menuPropsVideo          = "Vidéo";
    ls->menuPropsSound          = "Son";
    ls->menuPropsControls       = "Contrôles";
    ls->menuPropsPerformance    = "Performances";
    ls->menuPropsSettings       = "Réglages";
    ls->menuPropsFile           = "Fichiers";
    ls->menuPropsLanguage       = "Langue";
    ls->menuPropsPorts          = "Ports";

    ls->menuVideoChipAutodetect = "Détection automatique"; 
    ls->menuVideoSource         = "Sortie vidéo";                   // New in 2.5
    ls->menuVideoSourceDefault  = "Sortie vidéo non connectée";      // New in 2.5
    ls->menuVideoInSource       = "Entrée vidéo";                    // New in 2.5
    ls->menuVideoInBitmap       = "Fichier Bitmap";                        // New in 2.5

    ls->menuHelpHelp            = "Rubriques d'aide";
    ls->menuHelpAbout           = "A propos de blueMSX";

    ls->menuFileCart            = "Port cartouche";
    ls->menuFileDisk            = "Lecteur de disquettes";
    ls->menuFileCas             = "Lecteur de cassettes";
    ls->menuFilePrn             = "Imprimante"; 
    ls->menuFileLoadState       = "Charger une image du CPU";
    ls->menuFileSaveState       = "Sauver une image du CPU";
    ls->menuFileQLoadState      = "Accès instantané à l'image du CPU";
    ls->menuFileQSaveState      = "Sauvegarde instantanée de l'image du CPU";
    ls->menuFileCaptureAudio    = "Capture du son";
    ls->menuFileScreenShot      = "Capture d'écran";
    ls->menuFileExit            = "Quitter";

    ls->menuFileHarddisk        = "Disque dur";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "Aucun contrôleur";             // New in 2.5

    ls->menuRunRun              = "Lancer";
    ls->menuRunPause            = "Pause";
    ls->menuRunStop             = "Arrêt";
    ls->menuRunSoftReset        = "Reset logiciel";
    ls->menuRunHardReset        = "Reset matériel";
    ls->menuRunCleanReset       = "Reset complet";

    ls->menuToolsMachine         = "Editeur de machine";
    ls->menuToolsShortcuts      = "Editeur de raccourcis";
    ls->menuToolsKeyboard       = "Editeur de contrôleurs/clavier"; 
    ls->menuToolsMixer          = "Mélangeur audio";
    ls->menuToolsDebugger       = "Débogueur";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "Fichiers";
    ls->menuRun                 = "Emulation";
    ls->menuWindow              = "Fenêtre";
    ls->menuOptions             = "Options";
    ls->menuTools                = "Outils";
    ls->menuHelp                = "Aide";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Ouvrir";
    ls->dlgCancel               = "Annuler";
    ls->dlgSave                 = "Sauver";
    ls->dlgSaveAs               = "Sauver Sous...";
    ls->dlgRun                  = "Lancer";
    ls->dlgClose                = "Fermer";

    ls->dlgLoadRom              = "blueMSX - Choisissez une rom à charger";
    ls->dlgLoadDsk              = "blueMSX - Choisissez une disquette à charger";
    ls->dlgLoadCas              = "blueMSX - Choisissez une cassette à charger";
    ls->dlgLoadRomDskCas        = "blueMSX - Choisissez un fichier rom, dsk, ou cas à charger";
    ls->dlgLoadRomDesc          = "Choisissez une rom à charger:";
    ls->dlgLoadDskDesc          = "Choisissez une disquette à charger:";
    ls->dlgLoadCasDesc          = "Choisissez une cassette à charger:";
    ls->dlgLoadRomDskCasDesc    = "Choisissez un fichier rom, dsk, ou cas à charger:";
    ls->dlgLoadState            = "Charger une image du CPU";
    ls->dlgSaveState            = "Sauver une image du CPU";
    ls->dlgSaveCassette         = "blueMSX - Sauver une image de cassette";
    ls->dlgInsertRom1           = "Insérer une cartouche dans le port 1";
    ls->dlgInsertRom2           = "Insérer une cartouche dans le port 2";
    ls->dlgInsertDiskA          = "Insérer une disquette dans le lecteur A";
    ls->dlgInsertDiskB          = "Insérer une disquette dans le lecteur B";
    ls->dlgInsertHarddisk       = "Insérer un disque dur";                   // New in 2.5
    ls->dlgInsertCas            = "Insérer une cassette dans le lecteur";
    ls->dlgRomType              = "Type de Rom:"; 

    ls->dlgTapeTitle            = "blueMSX - position de la bande";
    ls->dlgTapeFrameText        = "Position de la bande";
    ls->dlgTapeCurrentPos       = "Position actuelle";
    ls->dlgTapeTotalTime        = "Durée totale";
    ls->dlgTapeCustom           = "Visualiser les Fichiers Personnalisés";
    ls->dlgTapeSetPosText       = "Position de la cassette:";
    ls->dlgTabPosition          = "Position";
    ls->dlgTabType              = "Type";
    ls->dlgTabFilename          = "Nom du fichier";
    ls->dlgZipReset             = "Reset après l'insertion";

    ls->dlgAboutTitle           = "blueMSX - A propos";

    ls->dlgLangLangText         = "Choisissez la langue utilisée par blueMSX";
    ls->dlgLangLangTitle        = "blueMSX - Langue";

    ls->dlgAboutAbout           = "A PROPOS\r\n====";
    ls->dlgAboutVersion         = "Version:";
    ls->dlgAboutBuildNumber     = "Build:";
    ls->dlgAboutBuildDate       = "Date:";
    ls->dlgAboutCreat           = "Créé par Daniel Vik";
    ls->dlgAboutDevel           = "DEVELOPPEURS\r\n========";
    ls->dlgAboutThanks          = "Un grand merci à\r\n============";
    ls->dlgAboutLisence         = "LICENCE\r\n"
                                  "======\r\n\r\n"
                                  "Ce logiciel est livré \"tel quel\" sans garantie. "
                                  "L'auteur décline toute responsabilité en cas de dommages "
                                  "liés à son utilisation.\r\n\r\n"
                                  "Pour plus d'informations, consultez www.bluemsx.com.";

    ls->dlgSavePreview          = "Prévisualiser"; 
    ls->dlgSaveDate             = "Date:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Configuration";
    ls->propEmulation           = "Emulation";
    ls->propVideo               = "Video";
    ls->propSound               = "Son";
    ls->propControls            = "Contrôles";
    ls->propPerformance         = "Performances";
    ls->propSettings             = "Réglages";
    ls->propFile                = "Fichiers"; 
    ls->propPorts               = "Ports";

    ls->propEmuGeneralGB        = "Général ";
    ls->propEmuFamilyText       = "Type de MSX:";
    ls->propEmuMemoryGB         = "Memoire ";
    ls->propEmuRamSizeText      = "Taille de la RAM:";
    ls->propEmuVramSizeText     = "Taille de la VRAM:";
    ls->propEmuSpeedGB          = "Vitesse de l'émulation ";
    ls->propEmuSpeedText        = "Vitesse de l'émulation:";
    ls->propEmuFrontSwitchGB    = "Commutateurs Panasonic "; 
    ls->propEmuFrontSwitch      = " Commutateur frontal"; 
    ls->propEmuFdcTiming        = "Ne pas synchroniser le lecteur de disquettes"; 
    ls->propEmuPauseSwitch      = " Commutateur Pause";
    ls->propEmuAudioSwitch      = " Commutateur MSX-AUDIO"; 
    ls->propVideoFreqText       = "Fréquence Vidéo:";
    ls->propVideoFreqAuto       = "Auto";
    ls->propSndOversampleText   = "Suréchantillonner:";
    ls->propSndYkInGB           = "Entrée YK-01/YK-10/YK-20 ";                // New in 2.5
    ls->propSndMidiInGB         = "Entrée MIDI ";
    ls->propSndMidiOutGB        = "Sortie MIDI ";
    ls->propSndMidiChannel      = "Canal MIDI:";                      // New in 2.5
    ls->propSndMidiAll          = "Tous";                                // New in 2.5

    ls->propMonMonGB            = "Moniteur ";
    ls->propMonTypeText         = "Type de moniteur:";
    ls->propMonEmuText          = "Emulation du moniteur:";
    ls->propVideoTypeText       = "Mode:";
    ls->propWindowSizeText      = "Taille de la fenêtre:";
    ls->propMonHorizStretch     = " Etirement Horizontal";
    ls->propMonVertStretch      = " Etirement Vertical "; 
    ls->propMonDeInterlace      = " Dé-entrelacé";
    ls->propBlendFrames         = " Mixer des images consécutives";           // New in 2.5
    ls->propMonBrightness       = "Brillance:";
    ls->propMonContrast         = "Contraste:";
    ls->propMonSaturation       = "Saturation:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Lignes de scannage:";
    ls->propMonColorGhosting    = "Modulateur RF"; 
    ls->propMonEffectsGB        = "Effets ";

    ls->propPerfVideoDrvGB      = "Vidéo ";
    ls->propPerfVideoDispDrvText= "Pilote d'affichage:";
    ls->propPerfFrameSkipText   = "Images ignorées:";
    ls->propPerfAudioDrvGB      = "Audio ";
    ls->propPerfAudioDrvText    = "Pilote audio:";
    ls->propPerfAudioBufSzText  = "Taille du buffer:";
    ls->propPerfEmuGB           = "Emulation ";
    ls->propPerfSyncModeText    = "Synchronisation:";
    ls->propFullscreenResText   = "Résolution plein écran:"; 

    ls->propSndChipEmuGB        = "Emulation du son ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound        = " Moonsound";
    ls->propSndMt32ToGm         = " Accorder les instruments MT-32 à General MIDI";

    ls->propPortsLptGB          = "Port parallèle ";
    ls->propPortsComGB          = "Ports série ";
    ls->propPortsLptText        = "Port:";
    ls->propPortsCom1Text       = "Port 1:";
    ls->propPortsNone           = "Aucun";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC"; 
    ls->propPortsFile           = "Imprimer dans fichier"; 
    ls->propPortsComFile        = "Envoyer vers fichier";
    ls->propPortsOpenLogFile    = "Ouvrir un fichier de données"; 
    ls->propPortsEmulateMsxPrn  = "Emulation:"; 

    ls->propSetFileHistoryGB     = "Historique des fichiers ";
    ls->propSetFileHistorySize   = "Nombre d'éléments dans l'historique";
    ls->propSetFileHistoryClear  = "Purger l'historique";
    ls->propFileTypes            = " Associer certains types de fichiers avec blueMSX"; 
    ls->propWindowsEnvGB         = "Environnement Windows "; 
    ls->propSetScreenSaver       = " Désactiver l'économiseur d'écran quand blueMSX est lancé";
    ls->propDisableWinKeys       = " Fonction MSX automatique pour les touches Windows"; 
    ls->propPriorityBoost       = " Donner à blueMSX une priorité élevée";
    ls->propScreenshotPng       = " Utiliser le format PNG pour les captures d'écran";  // New in 2.5
    ls->propClearHistory         = "Désirez-vous vraiment purger l'historique ?";
    ls->propOpenRomGB           = "Ouvrir un fichier Rom"; 
    ls->propDefaultRomType      = "Type par défaut:"; 
    ls->propGuessRomType        = "Deviner le type"; 

    ls->propSettDefSlotGB       = "Glisser et déposer "; 
    ls->propSettDefSlots        = "Insérer la cartouche dans:"; 
    ls->propSettDefSlot         = " Port"; 
    ls->propSettDefDrives       = "Insérer la disquette dans:"; 
    ls->propSettDefDrive        = " Lecteur"; 
    
    ls->propThemeGB             = "Thème ";
    ls->propTheme               = "Thème";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Couleur";
    ls->enumVideoMonGrey        = "Noir et blanc";
    ls->enumVideoMonGreen       = "Monochrome vert";
    ls->enumVideoMonAmber       = "Ambre";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Aucune";
    ls->enumVideoEmuYc          = "Cable Y/C (précis)";
    ls->enumVideoEmuMonitor     = "Moniteur";
    ls->enumVideoEmuYcBlur      = "Cable Y/C avec bruit (précis)";
    ls->enumVideoEmuComp        = "Composite (flou)";
    ls->enumVideoEmuCompBlur    = "Composite avec bruit (flou)";
    ls->enumVideoEmuScale2x     = "Echelle 2x";
    ls->enumVideoEmuHq2x        = "Hq2x"; 

    ls->enumVideoSize1x         = "Normal - 320x200";
    ls->enumVideoSize2x         = "Double - 640x400";
    ls->enumVideoSizeFullscreen = "Plein écran";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw HW accél.";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Aucune";
    ls->enumVideoFrameskip1     = "1 image";
    ls->enumVideoFrameskip2     = "2 images";
    ls->enumVideoFrameskip3     = "3 images";
    ls->enumVideoFrameskip4     = "4 images";
    ls->enumVideoFrameskip5     = "5 images";

    ls->enumSoundDrvNone        = "Pas de son";
    ls->enumSoundDrvWMM         = "Pilote WMM";
    ls->enumSoundDrvDirectX     = "Pilote DirectX";

    ls->enumEmuSync1ms          = "Sync sur MSX refresh";
    ls->enumEmuSyncAuto         = "Auto (rapide)";
    ls->enumEmuSyncNone         = "Aucune";
    ls->enumEmuSyncVblank       = "Sync sur PC Vertical Blank";
    ls->enumEmuAsyncVblank      = "PC Vblank asynchrone";             // New in 2.5

    ls->enumControlsJoyNone     = "Aucune";
    ls->enumControlsJoyMouse    = "Souris";
    ls->enumControlsJoyTetris2Dongle = "Dongle Tetris 2"; 
    ls->enumControlsJoyTMagicKeyDongle = "Dongle MagicKey";             // New in 2.5
    ls->enumControlsJoy2Button = "Joystick 2 boutons";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "Joystick ColecoVision";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 double face, 9 secteurs";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 double face, 8 secteurs";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 simple face, 9 secteurs";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 simple face, 8 secteurs";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 double face";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 simple face";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle               = "blueMSX - Editeur de machine";
    ls->confConfigText          = "Configuration";
    ls->confSlotLayout          = "Disposition des ports";
    ls->confMemory              = "Mémoire";
    ls->confChipEmulation       = "Emulation de la puce";
    ls->confChipExtras          = "Extras";

    ls->confOpenRom             = "Ouvrir une image-ROM";
    ls->confSaveTitle           = "blueMSX - Sauver la configuration";
    ls->confSaveText            = "Voulez-vous remplacer cette configuration ? :";
    ls->confSaveAsTitle         = "Sauver sous..."; 
    ls->confSaveAsMachineName   = "Nom de la machine:";
    ls->confDiscardTitle        = "blueMSX - Configuration";
    ls->confExitSaveTitle       = "blueMSX - Quitter l'éditeur de machine";
    ls->confExitSaveText        = "Voulez-vous ignorer les changements apportés à la configuration actuelle ?";

    ls->confSlotLayoutGB        = "Disposition des ports ";
    ls->confSlotExtSlotGB       = "Ports externes ";
    ls->confBoardGB             = "Système ";
    ls->confBoardText           = "Type de système:";
    ls->confSlotPrimary         = "Primaire";
    ls->confSlotExpanded        = "Etendu (4 sous-ports)";

    ls->confSlotCart            = "Cartouche";
    ls->confSlot                = "Slot";
    ls->confSubslot             = "Sous-slot";

    ls->confMemAdd              = "Ajouter...";
    ls->confMemEdit             = "Editer...";
    ls->confMemRemove           = "Supprimer";
    ls->confMemSlot             = "Port";
    ls->confMemAddresss         = "Adresses";
    ls->confMemType             = "Type";
    ls->confMemRomImage         = "Image de la ROM";

    ls->confChipVideoGB         = "Video ";
    ls->confChipVideoChip       = "Puce Video:";
    ls->confChipVideoRam        = "Video RAM:";
    ls->confChipSoundGB         = "Son ";

    ls->confCmosGB                = "CMOS "; 
    ls->confCmosEnable            = " Activer la CMOS"; 
    ls->confCmosBattery           = " Utiliser une Pile Chargée";

    ls->confCpuFreqGB            = "Fréquence CPU ";
    ls->confZ80FreqText          = "Fréquence Z80:";
    ls->confR800FreqText         = "Fréquence R800:";
    ls->confFdcGB                = "Contrôleur de lecteur de disquettes ";
    ls->confCFdcNumDrivesText    = "Nombre de lecteurs:";

    ls->confEditMemTitle        = "blueMSX - Editer le Mapper";
    ls->confEditMemGB           = "Details du Mapper ";
    ls->confEditMemType         = "Type:";
    ls->confEditMemFile         = "Fichier:";
    ls->confEditMemAddress      = "Adresse";
    ls->confEditMemSize         = "Taille";
    ls->confEditMemSlot         = "Port";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Touche rapide";
    ls->shortcutDescription     = "Raccourci";

    ls->shortcutSaveConfig      = "blueMSX - Sauver la configuration";
    ls->shortcutOverwriteConfig = "Voulez-vous remplacer cette configuration ?:";
    ls->shortcutExitConfig      = "blueMSX - Quitter l'éditeur de raccourcis";
    ls->shortcutDiscardConfig   = "Voulez-vous ignorer les changements apportés à la configuration actuelle ?";
    ls->shortcutSaveConfigAs    = "blueMSX - Sauver la configuration comme....";
    ls->shortcutConfigName      = "Nom de la configuration:";
    ls->shortcutNewProfile      = "< Nouveau profil >";
    ls->shortcutConfigTitle     = "blueMSX - Editeur de la carte des raccourcis";
    ls->shortcutAssign          = "Assigner";
    ls->shortcutPressText       = "Appuyer sur la (les) touche(s):";
    ls->shortcutScheme          = "Agencement:";
    ls->shortcutCartInsert1     = "Insérer une cartouche dans le port 1";
    ls->shortcutCartRemove1     = "Ejecter la cartouche du port 1";
    ls->shortcutCartInsert2     = "Insérer la cartouche dans le port 2";
    ls->shortcutCartRemove2     = "Ejecter la cartouche du port 2";
    ls->shortcutSpecialMenu1    = "Accéder au menu spécial pour les cartouches en port 1";
    ls->shortcutSpecialMenu2    = "Accéder au menu spécial pour les cartouches en port 2";
    ls->shortcutCartAutoReset   = "Reset après insertion d'une cartouche";
    ls->shortcutDiskInsertA     = "Insérer une disquette dans le lecteur A";
    ls->shortcutDiskDirInsertA  = "Insérer un répertoire comme disquette A"; 
    ls->shortcutDiskRemoveA     = "Ejecter la disquette du lecteur A";
    ls->shortcutDiskChangeA     = "Changer rapidement de disquette dans le lecteur A";
    ls->shortcutDiskAutoResetA  = "Reset après insertion d'une disquette dans le lecteur A";
    ls->shortcutDiskInsertB     = "Insérer une disquette dans le lecteur B";
    ls->shortcutDiskDirInsertB  = "Insérer un répertoire comme disquette B"; 
    ls->shortcutDiskRemoveB     = "Ejecter la disquette du lecteur B";
    ls->shortcutCasInsert       = "Insérer une cassette";
    ls->shortcutCasEject        = "Ejecter la cassette";
    ls->shortcutCasAutorewind   = "Rembobinage automatique ou non de la cassette";
    ls->shortcutCasReadOnly     = "Cassette en mode lecture seule ou non";
    ls->shortcutCasSetPosition  = "Définir la position";
    ls->shortcutCasRewind       = "Rembobiner la cassette";
    ls->shortcutCasSave         = "Sauver l'image de la cassette";
    ls->shortcutPrnFormFeed     = "Passage à la page suivante"; 
    ls->shortcutCpuStateLoad    = "Charger une image du CPU";
    ls->shortcutCpuStateSave    = "Sauver une image du CPU";
    ls->shortcutCpuStateQload   = "Accès instantané à l'image du CPU";
    ls->shortcutCpuStateQsave   = "Sauvegarde instantanée de l'image du CPU";
    ls->shortcutAudioCapture    = "Démarrer/arrêter la capture du son";
    ls->shortcutScreenshotOrig  = "Capture d'écran";
    ls->shortcutScreenshotSmall = "Petite capture d'écran sans filtre";
    ls->shortcutScreenshotLarge = "Grande capture d'écran sans filtre";
    ls->shortcutQuit            = "Quitter blueMSX";
    ls->shortcutRunPause        = "Démarrer/geler l'émulation";
    ls->shortcutStop            = "Arrêter l'émulation";
    ls->shortcutResetHard       = "Reset matériel";
    ls->shortcutResetSoft       = "Reset logiciel";
    ls->shortcutResetClean      = "Reset complet";
    ls->shortcutSizeSmall       = "Passer en mode petite fenêtre";
    ls->shortcutSizeNormal      = "Passer en mode fenêtre normal";
    ls->shortcutSizeFullscreen  = "Passer en mode plein écran";
    ls->shortcutSizeMinimized   = "Réduire la fenêtre";
    ls->shortcutToggleFullscren = "Balance mode fenêtre/mode plein écran";
    ls->shortcutVolumeIncrease  = "Augmenter le volume sonore";
    ls->shortcutVolumeDecrease  = "Diminuer le volume sonore";
    ls->shortcutVolumeMute      = "Arrêter le son";
    ls->shortcutVolumeStereo    = "Balance mono/stéréo";
    ls->shortcutSwitchMsxAudio  = "Actionner le commutateur MSX-AUDIO";
    ls->shortcutSwitchFront     = "Actionner le commutateur frontal Panasonic";
    ls->shortcutSwitchPause     = "Actionner le commutateur Pause";
    ls->shortcutToggleMouseLock = "Activer/désactiver le blocage de la souris";
    ls->shortcutEmuSpeedMax     = "Vitesse maximale d'émulation";
    ls->shortcutEmuSpeedToggle  = "Balance vitesse maximale d'émulation";
    ls->shortcutEmuSpeedNormal  = "Vitesse normale d'émulation";
    ls->shortcutEmuSpeedInc     = "Augmenter la vitesse d'émulation";
    ls->shortcutEmuSpeedDec     = "Diminuer la vitesse d'émulation";
    ls->shortcutThemeSwitch     = "Changer de thème";
    ls->shortcutShowEmuProp     = "Accéder au menu Emulation";
    ls->shortcutShowVideoProp   = "Accéder au menu Video";
    ls->shortcutShowAudioProp   = "Accéder au menu Son";
    ls->shortcutShowCtrlProp    = "Accéder au menu Contrôles";
    ls->shortcutShowPerfProp    = "Accéder au menu Performances";
    ls->shortcutShowSettProp    = "Accéder au menu Réglages";
    ls->shortcutShowPorts       = "Accéder au menu Ports";
    ls->shortcutShowLanguage    = "Accéder au menu Langue";
    ls->shortcutShowMachines    = "Accéder à l'éditeur de machines";
    ls->shortcutShowShortcuts   = "Accéder à l'éditeur de raccourcis";
    ls->shortcutShowKeyboard    = "Accéder à l'éditeur de contrôleurs/clavier"; 
    ls->shortcutShowMixer       = "Accéder au Mélangeur audio";
    ls->shortcutShowDebugger    = "Accéder au Débogueur"; 
    ls->shortcutShowTrainer     = "Accéder au Trainer"; 
    ls->shortcutShowHelp        = "Accéder aux rubriques d'aide ";
    ls->shortcutShowAbout       = "Voir la rubrique A propos de blueMSX ";
    ls->shortcutShowFiles       = "Accéder au menu Fichiers";
    ls->shortcutToggleSpriteEnable = "Montrer/Cacher les sprites";
    ls->shortcutToggleFdcTiming = "Synchroniser ou non le lecteur de disquettes"; 
    ls->shortcutToggleCpuTrace  = "Valider/Invalider la trace du CPU";


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Touche MSX:"; 
    ls->keyconfigMappedTo       = "Touche PC :"; 
    ls->keyconfigMappingScheme  = "Configuration du clavier:"; 

    
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

    ls->aboutScrollThanksTo     = "Spécial remerciement à: ";
    ls->aboutScrollAndYou       = "et TOI !!!!";
};

#endif

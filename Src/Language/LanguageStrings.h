/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageStrings.h,v $
**
** $Revision: 1.66 $
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
#ifndef LANGUAGE_STRINGS_H
#define LANGUAGE_STRINGS_H

typedef struct {
    //----------------------
    // Language lines
    //----------------------

    char* langChineseSimplified;
    char* langChineseTraditional;
    char* langDutch;
    char* langEnglish;
    char* langFinnish;
    char* langFrench;
    char* langGerman;
    char* langItalian;
    char* langJapanese;
    char* langKorean;
    char* langPolish;
    char* langPortuguese;
    char* langSpanish;
    char* langSwedish;


    //----------------------
    // Generic lines
    //----------------------

    char* textDevice;
    char* textFilename;
    char* textFile;
    char* textNone;
    char* textUnknown;


    //----------------------
    // Warning and Error lines
    //----------------------

    char* warningTitle;
    char* warningDiscardChanges;
    char* warningOverwriteFile;
    char* errorTitle;
    char* errorEnterFullscreen;
    char* errorDirectXFailed;
    char* errorNoRomInZip;
    char* errorNoDskInZip;
    char* errorNoCasInZip;
    char* errorNoHelp;
    char* errorStartEmu;
    char* errorPortableReadonly;


    //----------------------
    // File related lines
    //----------------------

    char* fileRom;
    char* fileAll;
    char* fileCpuState;
    char* fileDisk;
    char* fileCas;


    //----------------------
    // Menu related lines
    //----------------------

    char* menuNoRecentFiles;
    char* menuInsert;
    char* menuEject;

    char* menuCartGameReader;
    char* menuCartIde;
    char* menuCartBeerIde;
    char* menuCartGIde;
    char* menuCartSunriseIde;
    char* menuCartSCC;
    char* menuCartSCCPlus;
    char* menuCartFMPac;
    char* menuCartPac;
    char* menuCartHBI55;
    char* menuCartInsertSpecial;
    char* menuCartMegaRam;
    char* menuCartExternalRam;

    char* menuDiskInsertNew;
    char* menuDiskDirInsert;
    char* menuDiskAutoStart;
    char* menuCartAutoReset;

    char* menuCasRewindAfterInsert;
    char* menuCasUseReadOnly;
    char* lmenuCasSaveAs;
    char* menuCasSetPosition;
    char* menuCasRewind;

    char* menuPrnFormfeed;

    char* menuZoomNormal;
    char* menuZoomDouble;
    char* menuZoomFullscreen;

    char* menuPropsEmulation;
    char* menuPropsVideo;
    char* menuPropsSound;
    char* menuPropsControls;
    char* menuPropsPerformance;
    char* menuPropsSettings;
    char* menuPropsFile;
    char* menuPropsLanguage;
    char* menuPropsPorts;

    char* menuVideoSource;
    char* menuVideoSourceDefault;
    char* menuVideoChipAutodetect;
    char* menuVideoInSource;
    char* menuVideoInBitmap;

    char* menuHelpHelp;
    char* menuHelpAbout;

    char* menuFileCart;
    char* menuFileDisk;
    char* menuFileCas;
    char* menuFilePrn;
    char* menuFileLoadState;
    char* menuFileSaveState;
    char* menuFileQLoadState;
    char* menuFileQSaveState;
    char* menuFileCaptureAudio;
    char* menuFileScreenShot;
    char* menuFileExit;
    char* menuFileHarddisk;
    char* menuFileHarddiskNoPesent;

    char* menuRunRun;
    char* menuRunPause;
    char* menuRunStop;
    char* menuRunSoftReset;
    char* menuRunHardReset;
    char* menuRunCleanReset;

    char* menuToolsMachine;
    char* menuToolsShortcuts;
    char* menuToolsKeyboard;
    char* menuToolsMixer;
    char* menuToolsDebugger;
    char* menuToolsTrainer;
    char* menuToolsTraceLogger;

    char* menuFile;
    char* menuRun;
    char* menuWindow;
    char* menuOptions;
    char* menuTools;
    char* menuHelp;


    //----------------------
    // Dialog related lines
    //----------------------

    char* dlgOK;
    char* dlgOpen;
    char* dlgCancel;
    char* dlgSave;
    char* dlgSaveAs;
    char* dlgRun;
    char* dlgClose;

    char* dlgLoadRom;
    char* dlgLoadDsk;
    char* dlgLoadCas;
    char* dlgLoadRomDskCas;
    char* dlgLoadRomDesc;
    char* dlgLoadDskDesc;
    char* dlgLoadCasDesc;
    char* dlgLoadRomDskCasDesc;
    char* dlgLoadState;
    char* dlgSaveState;
    char* dlgSaveCassette;
    char* dlgInsertRom1;
    char* dlgInsertRom2;
    char* dlgInsertDiskA;
    char* dlgInsertDiskB;
    char* dlgInsertHarddisk;
    char* dlgInsertCas;
    char* dlgRomType;

    char* dlgTapeTitle;
    char* dlgTapeFrameText;
    char* dlgTapeCurrentPos;
    char* dlgTapeTotalTime;
    char* dlgTapeSetPosText;
    char* dlgTapeCustom;
    char* dlgTabPosition;
    char* dlgTabType;
    char* dlgTabFilename;
    char* dlgZipReset;

    char* dlgAboutTitle;

    char* dlgLangLangText;
    char* dlgLangLangTitle;

    char* dlgAboutAbout;
    char* dlgAboutVersion;
    char* dlgAboutBuildNumber;
    char* dlgAboutBuildDate;
    char* dlgAboutCreat;
    char* dlgAboutDevel;
    char* dlgAboutThanks;
    char* dlgAboutLisence;

    char* dlgSavePreview;
    char* dlgSaveDate;


    //----------------------
    // Properties related lines
    //----------------------

    char* propTitle;
    char* propEmulation;
    char* propVideo;
    char* propSound;
    char* propControls;
    char* propPerformance;
    char* propSettings;
    char* propFile;
    char* propPorts;

    char* propEmuGeneralGB;
    char* propEmuFamilyText;
    char* propEmuMemoryGB;
    char* propEmuRamSizeText;
    char* propEmuVramSizeText;
    char* propEmuSpeedGB;
    char* propEmuSpeedText;
    char* propEmuFrontSwitchGB;
    char* propEmuFrontSwitch;
    char* propEmuFdcTiming;
    char* propEmuPauseSwitch;
    char* propEmuAudioSwitch;
    char* propVideoFreqText;
    char* propVideoFreqAuto;
    char* propSndOversampleText;
    char* propSndMidiInGB;
    char* propSndYkInGB;
    char* propSndMidiOutGB;
    char* propSndMidiChannel;
    char* propSndMidiAll;

    char* propMonMonGB;
    char* propMonTypeText;
    char* propMonEmuText;
    char* propVideoTypeText;
    char* propWindowSizeText;
    char* propMonHorizStretch;
    char* propMonVertStretch;
    char* propMonDeInterlace;
    char* propBlendFrames;
    char* propMonBrightness;
    char* propMonContrast;
    char* propMonSaturation;
    char* propMonGamma;
    char* propMonScanlines;
    char* propMonColorGhosting;
    char* propMonEffectsGB;

    char* propPerfVideoDrvGB;
    char* propPerfVideoDispDrvText;
    char* propPerfFrameSkipText;
    char* propPerfAudioDrvGB;
    char* propPerfAudioDrvText;
    char* propPerfAudioBufSzText;
    char* propPerfEmuGB;
    char* propPerfSyncModeText;
    char* propFullscreenResText;

    char* propSndChipEmuGB;
    char* propSndMsxMusic;
    char* propSndMsxAudio;
    char* propSndMoonsound;
    char* propSndMt32ToGm;

    char* propPortsLptGB;
    char* propPortsComGB;
    char* propPortsLptText;
    char* propPortsCom1Text;
    char* propPortsNone;
    char* propPortsSimplCovox;
    char* propPortsFile;
    char* propPortsComFile;
    char* propPortsOpenLogFile;
    char* propPortsEmulateMsxPrn;

    char* propSetFileHistoryGB;
    char* propSetFileHistorySize;
    char* propSetFileHistoryClear;
    char* propFileTypes;
    char* propWindowsEnvGB;
    char* propSetScreenSaver;
    char* propDisableWinKeys;
    char* propPriorityBoost;
    char* propScreenshotPng;
    char* propClearHistory;
    char* propOpenRomGB;
    char* propDefaultRomType;
    char* propGuessRomType;

    char* propSettDefSlotGB;
    char* propSettDefSlots;
    char* propSettDefSlot;
    char* propSettDefDrives;
    char* propSettDefDrive;

    char* propThemeGB;
    char* propTheme;


    //----------------------
    // Dropdown related lines
    //----------------------

    char* enumVideoMonColor;
    char* enumVideoMonGrey;
    char* enumVideoMonGreen;
    char* enumVideoMonAmber;

    char* enumVideoTypePAL;
    char* enumVideoTypeNTSC;

    char* enumVideoEmuNone;
    char* enumVideoEmuYc;
    char* enumVideoEmuMonitor;
    char* enumVideoEmuYcBlur;
    char* enumVideoEmuComp;
    char* enumVideoEmuCompBlur;
    char* enumVideoEmuScale2x;
    char* enumVideoEmuHq2x;

    char* enumVideoSize1x;
    char* enumVideoSize2x;
    char* enumVideoSizeFullscreen;

    char* enumVideoDrvDirectDrawHW;
    char* enumVideoDrvDirectDraw;
    char* enumVideoDrvGDI;

    char* enumVideoFrameskip0;
    char* enumVideoFrameskip1;
    char* enumVideoFrameskip2;
    char* enumVideoFrameskip3;
    char* enumVideoFrameskip4;
    char* enumVideoFrameskip5;

    char* enumSoundDrvNone;
    char* enumSoundDrvWMM;
    char* enumSoundDrvDirectX;

    char* enumEmuSync1ms;
    char* enumEmuSyncAuto;
    char* enumEmuSyncNone;
    char* enumEmuSyncVblank;
    char* enumEmuAsyncVblank;

    char* enumControlsJoyNone;
    char* enumControlsJoyMouse;
    char* enumControlsJoyTetris2Dongle;
    char* enumControlsJoyTMagicKeyDongle;
    char* enumControlsJoy2Button;
    char* enumControlsJoyGunstick;
    char* enumControlsJoyAsciiLaser;
    char* enumControlsJoyColeco;
    
    char* enumDiskMsx35Dbl9Sect;
    char* enumDiskMsx35Dbl8Sect;
    char* enumDiskMsx35Sgl9Sect;
    char* enumDiskMsx35Sgl8Sect;
    char* enumDiskSvi525Dbl;
    char* enumDiskSvi525Sgl;


    //----------------------
    // Configuration related lines
    //----------------------

    char* confTitle;
    char* confConfigText;
    char* confSlotLayout;
    char* confMemory;
    char* confChipEmulation;
    char* confChipExtras;

    char* confOpenRom;
    char* confSaveTitle;
    char* confSaveText;
    char* confSaveAsTitle;
    char* confSaveAsMachineName;
    char* confDiscardTitle;
    char* confExitSaveTitle;
    char* confExitSaveText;

    char* confSlotLayoutGB;
    char* confSlotExtSlotGB;
    char* confBoardGB;
    char* confBoardText;
    char* confSlotPrimary;
    char* confSlotExpanded;

    char* confSlotCart;
    char* confSlot;
    char* confSubslot;

    char* confMemAdd;
    char* confMemEdit;
    char* confMemRemove;
    char* confMemSlot;
    char* confMemAddresss;
    char* confMemType;
    char* confMemRomImage;

    char* confChipVideoGB;
    char* confChipVideoChip;
    char* confChipVideoRam;
    char* confChipSoundGB;

    char* confCmosGB;
    char* confCmosEnable;
    char* confCmosBattery;

    char* confCpuFreqGB;
    char* confZ80FreqText;
    char* confR800FreqText;
    char* confFdcGB;
    char* confCFdcNumDrivesText;

    char* confEditMemTitle;
    char* confEditMemGB;
    char* confEditMemType;
    char* confEditMemFile;
    char* confEditMemAddress;
    char* confEditMemSize;
    char* confEditMemSlot;


    //----------------------
    // Shortcut lines
    //----------------------

    char* shortcutKey;
    char* shortcutDescription;

    char* shortcutSaveConfig;
    char* shortcutOverwriteConfig;
    char* shortcutExitConfig;
    char* shortcutDiscardConfig;
    char* shortcutSaveConfigAs;
    char* shortcutConfigName;
    char* shortcutNewProfile;
    char* shortcutConfigTitle;
    char* shortcutAssign;
    char* shortcutPressText;
    char* shortcutScheme;
    char* shortcutCartInsert1;
    char* shortcutCartRemove1;
    char* shortcutCartInsert2;
    char* shortcutCartRemove2;
    char* shortcutSpecialMenu1;
    char* shortcutSpecialMenu2;
    char* shortcutCartAutoReset;
    char* shortcutDiskInsertA;
    char* shortcutDiskDirInsertA;
    char* shortcutDiskRemoveA;
    char* shortcutDiskChangeA;
    char* shortcutDiskAutoResetA;
    char* shortcutDiskInsertB;
    char* shortcutDiskDirInsertB;
    char* shortcutDiskRemoveB;
    char* shortcutCasInsert;
    char* shortcutCasEject;
    char* shortcutCasAutorewind;
    char* shortcutCasReadOnly;
    char* shortcutCasSetPosition;
    char* shortcutCasRewind;
    char* shortcutCasSave;
    char* shortcutPrnFormFeed;
    char* shortcutCpuStateLoad;
    char* shortcutCpuStateSave;
    char* shortcutCpuStateQload;
    char* shortcutCpuStateQsave;
    char* shortcutAudioCapture;
    char* shortcutScreenshotOrig;
    char* shortcutScreenshotSmall;
    char* shortcutScreenshotLarge;
    char* shortcutQuit;
    char* shortcutRunPause;
    char* shortcutStop;
    char* shortcutResetHard;
    char* shortcutResetSoft;
    char* shortcutResetClean;
    char* shortcutSizeSmall;
    char* shortcutSizeNormal;
    char* shortcutSizeFullscreen;
    char* shortcutSizeMinimized;
    char* shortcutToggleFullscren;
    char* shortcutVolumeIncrease;
    char* shortcutVolumeDecrease;
    char* shortcutVolumeMute;
    char* shortcutVolumeStereo;
    char* shortcutSwitchMsxAudio;
    char* shortcutSwitchFront;
    char* shortcutSwitchPause;
    char* shortcutToggleMouseLock;
    char* shortcutEmuSpeedMax;
    char* shortcutEmuSpeedToggle;
    char* shortcutEmuSpeedNormal;
    char* shortcutEmuSpeedInc;
    char* shortcutEmuSpeedDec;
    char* shortcutThemeSwitch;
    char* shortcutShowEmuProp;
    char* shortcutShowVideoProp;
    char* shortcutShowAudioProp;
    char* shortcutShowCtrlProp;
    char* shortcutShowPerfProp;
    char* shortcutShowSettProp;
    char* shortcutShowPorts;
    char* shortcutShowLanguage;
    char* shortcutShowMachines;
    char* shortcutShowShortcuts;
    char* shortcutShowKeyboard;
    char* shortcutShowMixer;
    char* shortcutShowDebugger;
    char* shortcutShowTrainer;
    char* shortcutShowHelp;
    char* shortcutShowAbout;
    char* shortcutShowFiles;
    char* shortcutToggleSpriteEnable;
    char* shortcutToggleFdcTiming;
    char* shortcutToggleCpuTrace;


    //----------------------
    // Keyboard config lines
    //----------------------

    char* keyconfigSelectedKey;
    char* keyconfigMappedTo;
    char* keyconfigMappingScheme;

    
    //----------------------
    // Rom type lines
    //----------------------

    char* romTypeStandard;
    char* romTypeMsxdos2;
    char* romTypeKonamiScc;
    char* romTypeKonami;
    char* romTypeAscii8;
    char* romTypeAscii16;
    char* romTypeGameMaster2;
    char* romTypeAscii8Sram;
    char* romTypeAscii16Sram;
    char* romTypeRtype;
    char* romTypeCrossblaim;
    char* romTypeHarryFox;
    char* romTypeMajutsushi;
    char* romTypeZenima80;
    char* romTypeZenima90;
    char* romTypeZenima126;
    char* romTypeScc;
    char* romTypeSccPlus;
    char* romTypeSnatcher;
    char* romTypeSdSnatcher;
    char* romTypeSccMirrored;
    char* romTypeSccExtended;
    char* romTypeFmpac;
    char* romTypeFmpak;
    char* romTypeKonamiGeneric;
    char* romTypeSuperPierrot;
    char* romTypeMirrored;
    char* romTypeNormal;
    char* romTypeDiskPatch;
    char* romTypeCasPatch;
    char* romTypeTc8566afFdc;
    char* romTypeMicrosolFdc;
    char* romTypeNationalFdc;
    char* romTypePhilipsFdc;
    char* romTypeSvi738Fdc;
    char* romTypeMappedRam;
    char* romTypeMirroredRam1k;
    char* romTypeNormalRam;
    char* romTypeKanji;
    char* romTypeHolyQuran;
    char* romTypeMatsushitaSram;
    char* romTypePanasonic16;
    char* romTypePanasonic32;
    char* romTypeBunsetsu;
    char* romTypeJisyo;
    char* romTypeKanji12;
    char* romTypeNationalSram;
    char* romTypeS1985;
    char* romTypeS1990;
    char* romTypeTurborPause;
    char* romTypeF4deviceNormal;
    char* romTypeF4deviceInvert;
    char* romTypeMsxMidi;
    char* romTypeTurborTimer;
    char* romTypeKoei;
    char* romTypeBasic;
    char* romTypeHalnote;
    char* romTypeLodeRunner;
    char* romTypeNormal4000;
    char* romTypeNormalC000;
    char* romTypeKonamiSynth;
    char* romTypeKonamiKbdMast;
    char* romTypeKonamiWordPro;
    char* romTypePac;
    char* romTypeMegaRam;
    char* romTypeMegaRam128;
    char* romTypeMegaRam256;
    char* romTypeMegaRam512;
    char* romTypeMegaRam768;
    char* romTypeMegaRam2mb;
    char* romTypeExtRam;
    char* romTypeExtRam512;
    char* romTypeExtRam1mb;
    char* romTypeExtRam2mb;
    char* romTypeExtRam4mb;
    char* romTypeMsxMusic;
    char* romTypeMsxAudio;
    char* romTypeY8950;
    char* romTypeMoonsound;
    char* romTypeSvi328Cart;
    char* romTypeSvi328Fdc;
    char* romTypeSvi328Prn;
    char* romTypeSvi328Uart;
    char* romTypeSvi328col80;
    char* romTypeSvi727col80;
    char* romTypeColecoCart;
    char* romTypeSg1000Cart;
    char* romTypeTheCastle;
    char* romTypeSonyHbi55;
    char* romTypeMsxPrinter;
    char* romTypeTurborPcm;
    char* romTypeGameReader;
    char* romTypeSunriseIde;
    char* romTypeBeerIde;
    char* romTypeGide;
    char* romTypeVmx80;
    char* romTypeNms8280Digitiz;
    char* romTypeHbiV1Digitiz;
    char* romTypeFmdas;
    char* romTypeSfg01;
    char* romTypeSfg05;

   
    //----------------------
    // Debug type lines
    // Note: Only needs translation if debugger is translated
    //----------------------
    
    char* dbgMemVisible;
    char* dbgMemRamNormal;
    char* dbgMemRamMapped;
    char* dbgMemVram;
    char* dbgMemYmf278;
    char* dbgMemAy8950;
    char* dbgMemScc;

    char* dbgCallstack;

    char* dbgRegs;
    char* dbgRegsCpu;
    char* dbgRegsYmf262;
    char* dbgRegsYmf278;
    char* dbgRegsAy8950;
    char* dbgRegsYm2413;

    char* dbgDevRamMapper;
    char* dbgDevRam;
    char* dbgDevIdeBeer;
    char* dbgDevIdeGide;
    char* dbgDevF4Device;
    char* dbgDevFmpac;
    char* dbgDevFmpak;
    char* dbgDevKanji;
    char* dbgDevKanji12;
    char* dbgDevKonamiKbd;
    char* dbgDevKorean80;
    char* dbgDevKorean90;
    char* dbgDevKorean128;
    char* dbgDevMegaRam;
    char* dbgDevFdcMicrosol;
    char* dbgDevMoonsound;
    char* dbgDevMsxAudio;
    char* dbgDevMsxAudioMidi;
    char* dbgDevMsxMusic;
    char* dbgDevPrinter;
    char* dbgDevRs232;
    char* dbgDevS1990;
    char* dbgDevSfg05;
    char* dbgDevHbi55;
    char* dbgDevSviFdc;
    char* dbgDevSviPrn;
    char* dbgDevSvi80Col;
    char* dbgDevPcm;
    char* dbgDevMatsushita;
    char* dbgDevS1985;
    char* dbgDevCrtc6845;
    char* dbgDevTms9929A;
    char* dbgDevTms99x8A;
    char* dbgDevV9938;
    char* dbgDevV9958;
    char* dbgDevZ80;
    char* dbgDevMsxMidi;
    char* dbgDevPpi;
    char* dbgDevRtc;
    char* dbgDevTrPause;
    char* dbgDevAy8910;
    char* dbgDevScc;


    //----------------------
    // Debug type lines
    // Note: Can only be translated to european languages
    //----------------------
    char* aboutScrollThanksTo;
    char* aboutScrollAndYou;

} LanguageStrings;

#endif


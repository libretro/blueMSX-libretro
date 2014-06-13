/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/Language.c,v $
**
** $Revision: 1.67 $
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
#include "Language.h"
#include "LanguageStrings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LanguageEnglish.h"
#include "LanguageSwedish.h"
#include "LanguageSpannish.h"
#include "LanguageJapanese.h"
#include "LanguageKorean.h"
#include "LanguagePortuguese.h"
#include "LanguageFrench.h"
#include "LanguageDutch.h"
#include "LanguageItalian.h"
#include "LanguageFinnish.h"
#include "LanguageGerman.h"
#include "LanguagePolish.h"
#include "LanguageChineseSimplified.h"
#include "LanguageChineseTraditional.h"
 
static LanguageStrings langEnglish;
static LanguageStrings langSwedish;
static LanguageStrings langSpanish;
static LanguageStrings langJapanese;
static LanguageStrings langKorean;
static LanguageStrings langPortuguese;
static LanguageStrings langFrench;
static LanguageStrings langDutch;
static LanguageStrings langItalian;
static LanguageStrings langFinnish;
static LanguageStrings langGerman;
static LanguageStrings langPolish;
static LanguageStrings langChineseSimplified;
static LanguageStrings langChineseTraditional;

static LanguageStrings* ls;
static EmuLanguageType  lType = EMU_LANG_UNKNOWN;

typedef struct {
    EmuLanguageType type;
    char            english[32];
    char*           (*translation)();
} LanguageInfo;

static LanguageInfo languageInfo[] = {
    { EMU_LANG_CHINESESIMP, "Chinese Simplified",  langLangChineseSimplified },
    { EMU_LANG_CHINESETRAD, "Chinese Traditional", langLangChineseTraditional },
    { EMU_LANG_DUTCH,       "Dutch",               langLangDutch },
    { EMU_LANG_ENGLISH,     "English",             langLangEnglish },
    { EMU_LANG_FINNISH,     "Finnish",             langLangFinnish },
    { EMU_LANG_FRENCH,      "French",              langLangFrench },
    { EMU_LANG_GERMAN,      "German",              langLangGerman },
    { EMU_LANG_ITALIAN,     "Italian",             langLangItalian },
    { EMU_LANG_JAPANESE,    "Japanese",            langLangJapanese },
    { EMU_LANG_KOREAN,      "Korean",              langLangKorean },
    { EMU_LANG_POLISH,      "Polish",              langLangPolish },
    { EMU_LANG_PORTUGUESE,  "Portuguese",          langLangPortuguese },
    { EMU_LANG_SPANISH,     "Spanish",             langLangSpanish },
    { EMU_LANG_SWEDISH,     "Swedish",             langLangSwedish },
    { EMU_LANG_UNKNOWN,     "",                    langTextUnknown }
};

EmuLanguageType langFromName(char* name, int translate) {
    int i;
    for (i = 0; languageInfo[i].type != EMU_LANG_UNKNOWN; i++) {
        if (translate) {
            if (0 == strcmp(name, languageInfo[i].translation())) {
                break;
            }
        }
        else {
            if (0 == strcmp(name, languageInfo[i].english)) {
                break;
            }
        }
    }
    return languageInfo[i].type;
}


const char* langToName(EmuLanguageType languageType, int translate) {
    int i;
    for (i = 0; languageInfo[i].type != EMU_LANG_UNKNOWN; i++) {
        if (languageInfo[i].type == languageType) {
            break;
        }
    }
    if (translate) {
        return languageInfo[i].translation();
    }
    return languageInfo[i].english;
}

EmuLanguageType langGetType(int i) {
    return languageInfo[i].type;
}

void langInit() {
    langInitEnglish(&langEnglish);

    langInitEnglish(&langSwedish);
    langInitSwedish(&langSwedish);
    
    langInitEnglish(&langSpanish);
    langInitSpanish(&langSpanish);
    
    langInitEnglish(&langJapanese);
    langInitJapanese(&langJapanese);
    
    langInitEnglish(&langKorean);
    langInitKorean(&langKorean);
    
    langInitEnglish(&langPortuguese);
    langInitPortuguese(&langPortuguese);
    
    langInitEnglish(&langFrench);
    langInitFrench(&langFrench);
    
    langInitEnglish(&langDutch);
    langInitDutch(&langDutch);
    
    langInitEnglish(&langItalian);
    langInitItalian(&langItalian);
    
    langInitEnglish(&langFinnish);
    langInitFinnish(&langFinnish);
    
    langInitEnglish(&langGerman);
    langInitGerman(&langGerman);

    langInitEnglish(&langPolish);
    langInitPolish(&langPolish);

    langInitEnglish(&langChineseSimplified);
    langInitChineseSimplified(&langChineseSimplified);

    langInitEnglish(&langChineseTraditional);
    langInitChineseTraditional(&langChineseTraditional);
    
    ls = &langEnglish;
}

EmuLanguageType langGetLanguage() {
    return lType;
}

int langSetLanguage(EmuLanguageType languageType) {
    switch (languageType) {
    case EMU_LANG_ENGLISH:
        ls = &langEnglish;
        break;
    case EMU_LANG_SWEDISH:
        ls = &langSwedish;
        break;
    case EMU_LANG_SPANISH:
        ls = &langSpanish;
        break;
    case EMU_LANG_JAPANESE:
        ls = &langJapanese;
        break;
    case EMU_LANG_KOREAN:
        ls = &langKorean;
        break;
    case EMU_LANG_PORTUGUESE:
        ls = &langPortuguese;
        break;
    case EMU_LANG_FRENCH:
        ls = &langFrench;
        break;
    case EMU_LANG_DUTCH:
        ls = &langDutch;
        break;
    case EMU_LANG_ITALIAN:
        ls = &langItalian;
        break;
    case EMU_LANG_FINNISH:
        ls = &langFinnish;
        break;
    case EMU_LANG_GERMAN:
        ls = &langGerman;
        break;

    case EMU_LANG_POLISH:
        ls = &langPolish;
        break;

    case EMU_LANG_CHINESESIMP:
        ls = &langChineseSimplified;
        break;

    case EMU_LANG_CHINESETRAD:
        ls = &langChineseTraditional;
        break;
    default:
        return 0;
    }
    
    lType = languageType;

    return 1;
}


//----------------------
// Language lines
//----------------------

char* langLangChineseSimplified() { return ls->langChineseSimplified; }
char* langLangChineseTraditional() { return ls->langChineseTraditional; }
char* langLangDutch() { return ls->langDutch; }
char* langLangEnglish() { return ls->langEnglish; }
char* langLangFinnish() { return ls->langFinnish; }
char* langLangFrench() { return ls->langFrench; }
char* langLangGerman() { return ls->langGerman; }
char* langLangItalian() { return ls->langItalian; }
char* langLangJapanese() { return ls->langJapanese; }
char* langLangKorean() { return ls->langKorean; }
char* langLangPolish() { return ls->langPolish; }
char* langLangPortuguese() { return ls->langPortuguese; }
char* langLangSpanish() { return ls->langSpanish; }
char* langLangSwedish() { return ls->langSwedish; }


//----------------------
// Generic lines
//----------------------

char* langTextDevice() { return ls->textDevice; }
char* langTextFilename() { return ls->textFilename; }
char* langTextFile() { return ls->textFile; }
char* langTextNone() { return ls->textNone; }
char* langTextUnknown() { return ls->textUnknown; }


//----------------------
// Warning and Error lines
//----------------------

char* langWarningTitle() { return ls->warningTitle; }
char* langWarningDiscardChanges()  {return ls->warningDiscardChanges; }
char* langWarningOverwriteFile() { return ls->warningOverwriteFile; }
char* langErrorTitle() { return ls->errorTitle; }
char* langErrorEnterFullscreen() { return ls->errorEnterFullscreen; }
char* langErrorDirectXFailed() { return ls->errorDirectXFailed; }
char* langErrorNoRomInZip() { return ls->errorNoRomInZip; }
char* langErrorNoDskInZip() { return ls->errorNoDskInZip; }
char* langErrorNoCasInZip() { return ls->errorNoCasInZip; }
char* langErrorNoHelp() { return ls->errorNoHelp; }
char* langErrorStartEmu() { return ls->errorStartEmu; }
char* langErrorPortableReadonly()  {return ls->errorPortableReadonly; }


//----------------------
// File related lines
//----------------------

char* langFileRom() { return ls->fileRom; }
char* langFileAll() { return ls->fileAll; }
char* langFileCpuState() { return ls->fileCpuState; }
char* langFileDisk() { return ls->fileDisk; }
char* langFileCas() { return ls->fileCas; }


//----------------------
// Menu related lines
//----------------------

char* langMenuNoRecentFiles() { return ls->menuNoRecentFiles; }
char* langMenuInsert() { return ls->menuInsert; }
char* langMenuEject() { return ls->menuEject; }

char* langMenuCartGameReader() { return ls->menuCartGameReader; }
char* langMenuCartIde() { return ls->menuCartIde; }
char* langMenuCartBeerIde() { return ls->menuCartBeerIde; }
char* langMenuCartGIde() { return ls->menuCartGIde; }
char* langMenuCartSunriseIde() { return ls->menuCartSunriseIde; }
char* langMenuCartSCC() { return ls->menuCartSCC; }
char* langMenuCartSCCPlus() { return ls->menuCartSCCPlus; }
char* langMenuCartFMPac()  { return ls->menuCartFMPac; }
char* langMenuCartPac()  { return ls->menuCartPac; }
char* langMenuCartHBI55() { return ls->menuCartHBI55; }
char* langMenuCartInsertSpecial() { return ls->menuCartInsertSpecial; }
char* langMenuCartMegaRam() { return ls->menuCartMegaRam; }
char* langMenuCartExternalRam() { return ls->menuCartExternalRam; }

char* langMenuDiskInsertNew() { return ls->menuDiskInsertNew; }
char* langMenuDiskDirInsert() { return ls->menuDiskDirInsert; }
char* langMenuDiskAutoStart() { return ls->menuDiskAutoStart; }
char* langMenuCartAutoReset() { return ls->menuCartAutoReset; }

char* langMenuCasRewindAfterInsert() { return ls->menuCasRewindAfterInsert; }
char* langMenuCasUseReadOnly() { return ls->menuCasUseReadOnly; }
char* langMenuCasSaveAs() { return ls->lmenuCasSaveAs; }
char* langMenuCasSetPosition() { return ls->menuCasSetPosition; }
char* langMenuCasRewind() { return ls->menuCasRewind; }

char* langMenuPrnFormfeed() { return ls->menuPrnFormfeed; }

char* langMenuZoomNormal() { return ls->menuZoomNormal; }
char* langMenuZoomDouble() { return ls->menuZoomDouble; }
char* langMenuZoomFullscreen() { return ls->menuZoomFullscreen; }

char* langMenuPropsEmulation() { return ls->menuPropsEmulation; }
char* langMenuPropsVideo() { return ls->menuPropsVideo; }
char* langMenuPropsSound() { return ls->menuPropsSound; }
char* langMenuPropsControls() { return ls->menuPropsControls; }
char* langMenuPropsPerformance() { return ls->menuPropsPerformance; }
char* langMenuPropsSettings() { return ls->menuPropsSettings; }
char* langMenuPropsFile() { return ls->menuPropsFile; }
char* langMenuPropsLanguage() { return ls->menuPropsLanguage; }
char* langMenuPropsPorts() { return ls->menuPropsPorts; }

char* langMenuVideoSource()        { return ls->menuVideoSource; }
char* langMenuVideoSourceDefault() { return ls->menuVideoSourceDefault; }
char* langMenuVideoChipAutodetect() { return ls->menuVideoChipAutodetect; }
char* langMenuVideoInSource() { return ls->menuVideoInSource; }
char* langMenuVideoInBitmap() { return ls->menuVideoInBitmap; }

char* langMenuHelpHelp() { return ls->menuHelpHelp; }
char* langMenuHelpAbout() { return ls->menuHelpAbout; }

char* langMenuFileCart() { return ls->menuFileCart; }
char* langMenuFileDisk() { return ls->menuFileDisk; }
char* langMenuFileCas() { return ls->menuFileCas; }
char* langMenuFilePrn() { return ls->menuFilePrn; }
char* langMenuFileLoadState() { return ls->menuFileLoadState; }
char* langMenuFileSaveState() { return ls->menuFileSaveState; }
char* langMenuFileQLoadState() { return ls->menuFileQLoadState; }
char* langMenuFileQSaveState() { return ls->menuFileQSaveState; }
char* langMenuFileCaptureAudio() { return ls->menuFileCaptureAudio; }
char* langMenuFileScreenShot() { return ls->menuFileScreenShot; }
char* langMenuFileExit() { return ls->menuFileExit; }
char* langMenuFileHarddisk() { return ls->menuFileHarddisk; }
char* langMenuFileHarddiskNoPresent() { return ls->menuFileHarddiskNoPesent; }

char* langMenuRunRun() { return ls->menuRunRun; }
char* langMenuRunPause() { return ls->menuRunPause; }
char* langMenuRunStop() { return ls->menuRunStop; }
char* langMenuRunSoftReset() { return ls->menuRunSoftReset; }
char* langMenuRunHardReset() { return ls->menuRunHardReset; }
char* langMenuRunCleanReset() { return ls->menuRunCleanReset; }

char* langMenuToolsMachine() { return ls->menuToolsMachine; }
char* langMenuToolsShortcuts() { return ls->menuToolsShortcuts; }
char* langMenuToolsKeyboard() { return ls->menuToolsKeyboard; }
char* langMenuToolsMixer() { return ls->menuToolsMixer; }
char* langMenuToolsDebugger() { return ls->menuToolsDebugger; }
char* langMenuToolsTrainer() { return ls->menuToolsTrainer; }
char* langMenuToolsTraceLogger() { return ls->menuToolsTraceLogger; }

char* langMenuFile() { return ls->menuFile; }
char* langMenuRun() { return ls->menuRun; }
char* langMenuWindow() { return ls->menuWindow; }
char* langMenuOptions() { return ls->menuOptions; }
char* langMenuTools() { return ls->menuTools; }
char* langMenuHelp() { return ls->menuHelp; }


//----------------------
// Dialog related lines
//----------------------

char* langDlgOK() { return ls->dlgOK; }
char* langDlgOpen() { return ls->dlgOpen; }
char* langDlgCancel() { return ls->dlgCancel; }
char* langDlgSave() { return ls->dlgSave; }
char* langDlgSaveAs() { return ls->dlgSaveAs; }
char* langDlgRun() { return ls->dlgRun; }
char* langDlgClose() { return ls->dlgClose; }

char* langDlgLoadRom() { return ls->dlgLoadRom; }
char* langDlgLoadDsk() { return ls->dlgLoadDsk; }
char* langDlgLoadCas() { return ls->dlgLoadCas; }
char* langDlgLoadRomDskCas() { return ls->dlgLoadRomDskCas; }
char* langDlgLoadRomDesc() { return ls->dlgLoadRomDesc; }
char* langDlgLoadDskDesc() { return ls->dlgLoadDskDesc; }
char* langDlgLoadCasDesc() { return ls->dlgLoadCasDesc; }
char* langDlgLoadRomDskCasDesc() { return ls->dlgLoadRomDskCasDesc; }
char* langDlgLoadState() { return ls->dlgLoadState; }
char* langDlgSaveState() { return ls->dlgSaveState; }
char* langDlgSaveCassette() { return ls->dlgSaveCassette; }
char* langDlgInsertRom1() { return ls->dlgInsertRom1; }
char* langDlgInsertRom2() { return ls->dlgInsertRom2; }
char* langDlgInsertDiskA() { return ls->dlgInsertDiskA; }
char* langDlgInsertDiskB() { return ls->dlgInsertDiskB; }
char* langDlgInsertHarddisk() { return ls->dlgInsertHarddisk; }
char* langDlgInsertCas() { return ls->dlgInsertCas; }
char* langDlgRomType() { return ls->dlgRomType; }

char* langDlgTapeTitle() { return ls->dlgTapeTitle; }
char* langDlgTapeFrameText() { return ls->dlgTapeFrameText; }
char* langDlgTapeCurrentPos() { return ls->dlgTapeCurrentPos; }
char* langDlgTapeTotalTime() { return ls->dlgTapeTotalTime; }
char* langDlgTapeSetPosText() { return ls->dlgTapeSetPosText; }
char* langDlgTapeCustom() { return ls->dlgTapeCustom; }
char* langDlgTabPosition() { return ls->dlgTabPosition; }
char* langDlgTabType() { return ls->dlgTabType; }
char* langDlgTabFilename() { return ls->dlgTabFilename; }
char* langDlgZipReset() { return ls->dlgZipReset; }

char* langDlgAboutTitle() { return ls->dlgAboutTitle; }

char* langDlgLangLangText() { return ls->dlgLangLangText; }
char* langDlgLangTitle() { return ls->dlgLangLangTitle; }

char* langDlgAboutAbout() { return ls->dlgAboutAbout; }
char* langDlgAboutVersion() { return ls->dlgAboutVersion; }
char* langDlgAboutBuildNumber() { return ls->dlgAboutBuildNumber; }
char* langDlgAboutBuildDate() { return ls->dlgAboutBuildDate; }
char* langDlgAboutCreat() { return ls->dlgAboutCreat; }
char* langDlgAboutDevel() { return ls->dlgAboutDevel; }
char* langDlgAboutThanks() { return ls->dlgAboutThanks; }
char* langDlgAboutLisence() { return ls->dlgAboutLisence; }


//----------------------
// Properties related lines
//----------------------

char* langPropTitle() { return ls->propTitle; }
char* langPropEmulation() { return ls->propEmulation; }
char* langPropVideo() { return ls->propVideo; }
char* langPropSound() { return ls->propSound; }
char* langPropControls() { return ls->propControls; }
char* langPropPerformance() { return ls->propPerformance; }
char* langPropSettings() { return ls->propSettings; }
char* langPropFile()  { return ls->propFile; }
char* langPropPorts() { return ls->propPorts; }

char* langPropEmuGeneralGB() { return ls->propEmuGeneralGB; }
char* langPropEmuFamilyText() { return ls->propEmuFamilyText; }
char* langPropEmuMemoryGB() { return ls->propEmuMemoryGB; }
char* langPropEmuRamSizeText() { return ls->propEmuRamSizeText; }
char* langPropEmuVramSizeText() { return ls->propEmuVramSizeText; }
char* langPropEmuSpeedGB() { return ls->propEmuSpeedGB; }
char* langPropEmuSpeedText() { return ls->propEmuSpeedText; }
char* langPropEmuFrontSwitchGB() { return ls->propEmuFrontSwitchGB; }
char* langPropEmuFrontSwitch() { return ls->propEmuFrontSwitch; }
char* langPropEmuFdcTiming() { return ls->propEmuFdcTiming; }
char* langPropEmuPauseSwitch() { return ls->propEmuPauseSwitch; }
char* langPropEmuAudioSwitch() { return ls->propEmuAudioSwitch; }
char* langPropVideoFreqText() { return ls->propVideoFreqText; }
char* langPropVideoFreqAuto() { return ls->propVideoFreqAuto; }
char* langPropSndOversampleText() { return ls->propSndOversampleText; }
char* langPropSndYkInGB() { return ls->propSndYkInGB; }
char* langPropSndMidiInGB() { return ls->propSndMidiInGB; }
char* langPropSndMidiOutGB() { return ls->propSndMidiOutGB; }
char* langPropSndMidiChannel() { return ls->propSndMidiChannel; }
char* langPropSndMidiAll() { return ls->propSndMidiAll; }

char* langPropMonMonGB() { return ls->propMonMonGB; }
char* langPropMonTypeText() { return ls->propMonTypeText; }
char* langPropMonEmuText() { return ls->propMonEmuText; }
char* langPropVideoTypeText() { return ls->propVideoTypeText; }
char* langPropWindowSizeText() { return ls->propWindowSizeText; }
char* langPropMonHorizStretch() { return ls->propMonHorizStretch; }
char* langPropMonVertStretch() { return ls->propMonVertStretch; }
char* langPropMonDeInterlace() { return ls->propMonDeInterlace; }
char* langPropMonBlendFrames() { return ls->propBlendFrames; }
char* langPropMonBrightness() { return ls->propMonBrightness; }
char* langPropMonContrast() { return ls->propMonContrast; }
char* langPropMonSaturation() { return ls->propMonSaturation; }
char* langPropMonGamma() { return ls->propMonGamma; }
char* langPropMonScanlines() { return ls->propMonScanlines; }
char* langPropMonColorGhosting() { return ls->propMonColorGhosting; }
char* langPropMonEffectsGB() { return ls->propMonEffectsGB; }

char* langPropPerfVideoDrvGB() { return ls->propPerfVideoDrvGB; }
char* langPropPerfVideoDispDrvText() { return ls->propPerfVideoDispDrvText; }
char* langPropPerfFrameSkipText() { return ls->propPerfFrameSkipText; }
char* langPropPerfAudioDrvGB() { return ls->propPerfAudioDrvGB; }
char* langPropPerfAudioDrvText() { return ls->propPerfAudioDrvText; }
char* langPropPerfAudioBufSzText() { return ls->propPerfAudioBufSzText; }
char* langPropPerfEmuGB() { return ls->propPerfEmuGB; }
char* langPropPerfSyncModeText() { return ls->propPerfSyncModeText; }
char* langPropFullscreenResText() { return ls->propFullscreenResText; }

char* langPropSndChipEmuGB() { return ls->propSndChipEmuGB; }
char* langPropSndMsxMusic() { return ls->propSndMsxMusic; }
char* langPropSndMsxAudio() { return ls->propSndMsxAudio; }
char* langPropSndMoonsound() { return ls->propSndMoonsound; }
char* langPropSndMt32ToGm() { return ls->propSndMt32ToGm; }

char* langPropPortsLptGB() { return ls->propPortsLptGB; }
char* langPropPortsComGB() { return ls->propPortsComGB; }
char* langPropPortsLptText() { return ls->propPortsLptText; }
char* langPropPortsCom1Text() { return ls->propPortsCom1Text; }
char* langPropPortsNone() { return ls->propPortsNone; }
char* langPropPortsSimplCovox() { return ls->propPortsSimplCovox; }
char* langPropPortsFile() { return ls->propPortsFile; }
char* langPropPortsComFile()  { return ls->propPortsComFile; }
char* langPropPortsOpenLogFile() { return ls->propPortsOpenLogFile; }
char* langPropPortsEmulateMsxPrn() { return ls->propPortsEmulateMsxPrn; }

char* langPropSetFileHistoryGB() { return ls->propSetFileHistoryGB; }
char* langPropSetFileHistorySize() { return ls->propSetFileHistorySize; }
char* langPropSetFileHistoryClear() { return ls->propSetFileHistoryClear; }
char* langPropFileTypes() { return ls->propFileTypes; }
char* langPropWindowsEnvGB() { return ls->propWindowsEnvGB; }
char* langPropScreenSaver() { return ls->propSetScreenSaver; }
char* langPropDisableWinKeys() { return ls->propDisableWinKeys; }
char* langPropPriorityBoost() { return ls->propPriorityBoost; }
char* langPropScreenshotPng() { return ls->propScreenshotPng; }
char* langPropClearFileHistory() { return ls->propClearHistory; }
char* langPropOpenRomGB() { return ls->propOpenRomGB; }
char* langPropDefaultRomType() { return ls->propDefaultRomType; }
char* langPropGuessRomType() { return ls->propGuessRomType; }

char* langPropSettDefSlotGB() { return ls->propSettDefSlotGB; }
char* langPropSettDefSlots() { return ls->propSettDefSlots; }
char* langPropSettDefSlot() { return ls->propSettDefSlot; }
char* langPropSettDefDrives() { return ls->propSettDefDrives; }
char* langPropSettDefDrive() { return ls->propSettDefDrive; }

char* langPropThemeGB() { return ls->propThemeGB; }
char* langPropTheme() { return ls->propTheme; }


//----------------------
// Dropdown related lines
//----------------------

char* langEnumVideoMonColor() { return ls->enumVideoMonColor; }
char* langEnumVideoMonGrey() { return ls->enumVideoMonGrey; }
char* langEnumVideoMonGreen() { return ls->enumVideoMonGreen; }
char* langEnumVideoMonAmber() { return ls->enumVideoMonAmber; }

char* langEnumVideoTypePAL() { return ls->enumVideoTypePAL; }
char* langEnumVideoTypeNTSC() { return ls->enumVideoTypeNTSC; }

char* langEnumVideoEmuNone() { return ls->enumVideoEmuNone; }
char* langEnumVideoEmuYc() { return ls->enumVideoEmuYc; }
char* langEnumVideoEmuMonitor() { return ls->enumVideoEmuMonitor; }
char* langEnumVideoEmuYcBlur() { return ls->enumVideoEmuYcBlur; }
char* langEnumVideoEmuComp() { return ls->enumVideoEmuComp; }
char* langEnumVideoEmuCompBlur() { return ls->enumVideoEmuCompBlur; }
char* langEnumVideoEmuScale2x() { return ls->enumVideoEmuScale2x; }
char* langEnumVideoEmuHq2x() { return ls->enumVideoEmuHq2x; }

char* langEnumVideoSize1x() { return ls->enumVideoSize1x; }
char* langEnumVideoSize2x() { return ls->enumVideoSize2x; }
char* langEnumVideoSizeFullscreen() { return ls->enumVideoSizeFullscreen; }

char* langEnumVideoDrvDirectDrawHW() { return ls->enumVideoDrvDirectDrawHW; }
char* langEnumVideoDrvDirectDraw() { return ls->enumVideoDrvDirectDraw; }
char* langEnumVideoDrvGDI() { return ls->enumVideoDrvGDI; }

char* langEnumVideoFrameskip0() { return ls->enumVideoFrameskip0; }
char* langEnumVideoFrameskip1() { return ls->enumVideoFrameskip1; }
char* langEnumVideoFrameskip2() { return ls->enumVideoFrameskip2; }
char* langEnumVideoFrameskip3() { return ls->enumVideoFrameskip3; }
char* langEnumVideoFrameskip4() { return ls->enumVideoFrameskip4; }
char* langEnumVideoFrameskip5() { return ls->enumVideoFrameskip5; }

char* langEnumSoundDrvNone() { return ls->enumSoundDrvNone; }
char* langEnumSoundDrvWMM() { return ls->enumSoundDrvWMM; }
char* langEnumSoundDrvDirectX() { return ls->enumSoundDrvDirectX; }

char* langEnumEmuSync1ms() { return ls->enumEmuSync1ms; }
char* langEnumEmuSyncAuto() { return ls->enumEmuSyncAuto; }
char* langEnumEmuSyncNone() { return ls->enumEmuSyncNone; }
char* langEnumEmuSyncVblank() { return ls->enumEmuSyncVblank; }
char* langEnumEmuAsyncVblank() { return ls->enumEmuAsyncVblank; }

char* langEnumControlsJoyNone() { return ls->enumControlsJoyNone; }
char* langEnumControlsJoyMouse() { return ls->enumControlsJoyMouse; }
char* langEnumControlsJoyTetrisDongle() { return ls->enumControlsJoyTetris2Dongle; }
char* langEnumControlsJoyMagicKeyDongle() { return ls->enumControlsJoyTMagicKeyDongle; }
char* langEnumControlsJoy2Button() { return ls->enumControlsJoy2Button; }
char* langEnumControlsJoyGunStick() { return ls->enumControlsJoyGunstick; }
char* langEnumControlsJoyAsciiLaser() { return ls->enumControlsJoyAsciiLaser; }
char* langEnumControlsJoyColeco() { return ls->enumControlsJoyColeco; }
    
char* langEnumDiskMsx35Dbl9Sect() { return ls->enumDiskMsx35Dbl9Sect; }
char* langEnumDiskMsx35Dbl8Sect() { return ls->enumDiskMsx35Dbl8Sect; }
char* langEnumDiskMsx35Sgl9Sect() { return ls->enumDiskMsx35Sgl9Sect; }
char* langEnumDiskMsx35Sgl8Sect() { return ls->enumDiskMsx35Sgl8Sect; }
char* langEnumDiskSvi525Dbl() { return ls->enumDiskSvi525Dbl; }
char* langEnumDiskSvi525Sgl() { return ls->enumDiskSvi525Sgl; }


//----------------------
// Configuration related lines
//----------------------

char* langDlgSavePreview() { return ls->dlgSavePreview; }
char* langDlgSaveDate() { return ls->dlgSaveDate; }

char* langConfTitle() { return ls->confTitle; }
char* langConfConfigText() { return ls->confConfigText; }
char* langConfSlotLayout() { return ls->confSlotLayout; }
char* langConfMemory() { return ls->confMemory; }
char* langConfChipEmulation() { return ls->confChipEmulation; }
char* langConfChipExtras() { return ls->confChipExtras; }

char* langConfOpenRom() { return ls->confOpenRom; }
char* langConfSaveTitle() { return ls->confSaveTitle; }
char* langConfSaveText() { return ls->confSaveText; }
char* langConfSaveAsTitle() { return ls->confSaveAsTitle; }
char* langConfSaveAsMachineName() { return ls->confSaveAsMachineName; }
char* langConfDiscardTitle() { return ls->confDiscardTitle; }
char* langConfExitSaveTitle() { return ls->confExitSaveTitle; }
char* langConfExitSaveText() { return ls->confExitSaveText; }

char* langConfSlotLayoutGB() { return ls->confSlotLayoutGB; }
char* langConfSlotExtSlotGB() { return ls->confSlotExtSlotGB; }
char* langConfBoardGB() { return ls->confBoardGB; }
char* langConfBoardText() { return ls->confBoardText; }
char* langConfSlotPrimary() { return ls->confSlotPrimary; }
char* langConfSlotExpanded() { return ls->confSlotExpanded; }

char* langConfCartridge() { return ls->confSlotCart; }
char* langConfSlot() { return ls->confSlot; }
char* langConfSubslot() { return ls->confSubslot; }

char* langConfMemAdd() { return ls->confMemAdd; }
char* langConfMemEdit() { return ls->confMemEdit; }
char* langConfMemRemove() { return ls->confMemRemove; }
char* langConfMemSlot() { return ls->confMemSlot; }
char* langConfMemAddress() { return ls->confMemAddresss; }
char* langConfMemType() { return ls->confMemType; }
char* langConfMemRomImage() { return ls->confMemRomImage; }

char* langConfChipVideoGB() { return ls->confChipVideoGB; }
char* langConfChipVideoChip() { return ls->confChipVideoChip; }
char* langConfChipVideoRam() { return ls->confChipVideoRam; }
char* langConfChipSoundGB() { return ls->confChipSoundGB; }

char* langConfCmosGB() { return ls->confCmosGB; }
char* langConfCmosEnableText() { return ls->confCmosEnable; }
char* langConfCmosBatteryText() { return ls->confCmosBattery; }

char* langConfChipCpuFreqGB() { return ls->confCpuFreqGB; }
char* langConfChipZ80FreqText() { return ls->confZ80FreqText; }
char* langConfChipR800FreqText() { return ls->confR800FreqText; }
char* langConfChipFdcGB() { return ls->confFdcGB; }
char* langConfChipFdcNumDrivesText() { return ls->confCFdcNumDrivesText; }

char* langConfEditMemTitle() { return ls->confEditMemTitle; }
char* langConfEditMemGB() { return ls->confEditMemGB; }
char* langConfEditMemType() { return ls->confEditMemType; }
char* langConfEditMemFile() { return ls->confEditMemFile; }
char* langConfEditMemAddress() { return ls->confEditMemAddress; }
char* langConfEditMemSize() { return ls->confEditMemSize; }
char* langConfEditMemSlot() { return ls->confEditMemSlot; }


//----------------------
// Shortcut lines
//----------------------

char* langShortcutKey() { return ls->shortcutKey; }
char* langShortcutDescription() { return ls->shortcutDescription; }

char* langShortcutSaveConfig() { return ls->shortcutSaveConfig; }
char* langShortcutOverwriteConfig() { return ls->shortcutOverwriteConfig; }
char* langShortcutExitConfig() { return ls->shortcutExitConfig; }
char* langShortcutDiscardConfig() { return ls->shortcutDiscardConfig; }
char* langShortcutSaveConfigAs() { return ls->shortcutSaveConfigAs; }
char* langShortcutConfigName() { return ls->shortcutConfigName; }
char* langShortcutNewProfile() { return ls->shortcutNewProfile; }
char* langShortcutConfigTitle() { return ls->shortcutConfigTitle; }
char* langShortcutAssign() { return ls->shortcutAssign; }
char* langShortcutPressText() { return ls->shortcutPressText; }
char* langShortcutScheme() { return ls->shortcutScheme; }
char* langShortcutCartInsert1() { return ls->shortcutCartInsert1; }
char* langShortcutCartRemove1() { return ls->shortcutCartRemove1; }
char* langShortcutCartInsert2() { return ls->shortcutCartInsert2; }
char* langShortcutCartRemove2() { return ls->shortcutCartRemove2; }
char* langShortcutCartSpecialMenu1() { return ls->shortcutSpecialMenu1; }
char* langShortcutCartSpecialMenu2() { return ls->shortcutSpecialMenu2; }
char* langShortcutCartAutoReset() { return ls->shortcutCartAutoReset; }
char* langShortcutDiskInsertA() { return ls->shortcutDiskInsertA; }
char* langShortcutDiskDirInsertA() { return ls->shortcutDiskDirInsertA; }
char* langShortcutDiskRemoveA() { return ls->shortcutDiskRemoveA; }
char* langShortcutDiskChangeA() { return ls->shortcutDiskChangeA; }
char* langShortcutDiskAutoResetA() { return ls->shortcutDiskAutoResetA; }
char* langShortcutDiskInsertB() { return ls->shortcutDiskInsertB; }
char* langShortcutDiskDirInsertB() { return ls->shortcutDiskDirInsertB; }
char* langShortcutDiskRemoveB() { return ls->shortcutDiskRemoveB; }
char* langShortcutCasInsert() { return ls->shortcutCasInsert; }
char* langShortcutCasEject() { return ls->shortcutCasEject; }
char* langShortcutCasAutorewind() { return ls->shortcutCasAutorewind; }
char* langShortcutCasReadOnly() { return ls->shortcutCasReadOnly; }
char* langShortcutCasSetPosition() { return ls->shortcutCasSetPosition; }
char* langShortcutCasRewind() { return ls->shortcutCasRewind; }
char* langShortcutCasSave() { return ls->shortcutCasSave; }
char* langShortcutPrnFormFeed() { return ls->shortcutPrnFormFeed; }
char* langShortcutCpuStateLoad() { return ls->shortcutCpuStateLoad; }
char* langShortcutCpuStateSave() { return ls->shortcutCpuStateSave; }
char* langShortcutCpuStateQload() { return ls->shortcutCpuStateQload; }
char* langShortcutCpuStateQsave() { return ls->shortcutCpuStateQsave; }
char* langShortcutAudioCapture() { return ls->shortcutAudioCapture; }
char* langShortcutScreenshotOrig() { return ls->shortcutScreenshotOrig; }
char* langShortcutScreenshotSmall() { return ls->shortcutScreenshotSmall; }
char* langShortcutScreenshotLarge() { return ls->shortcutScreenshotLarge; }
char* langShortcutQuit() { return ls->shortcutQuit; }
char* langShortcutRunPause() { return ls->shortcutRunPause; }
char* langShortcutStop() { return ls->shortcutStop; }
char* langShortcutResetHard() { return ls->shortcutResetHard; }
char* langShortcutResetSoft() { return ls->shortcutResetSoft; }
char* langShortcutResetClean() { return ls->shortcutResetClean; }
char* langShortcutSizeSmall() { return ls->shortcutSizeSmall; }
char* langShortcutSizeNormal() { return ls->shortcutSizeNormal; }
char* langShortcutSizeFullscreen() { return ls->shortcutSizeFullscreen; }
char* langShortcutSizeMinimized() { return ls->shortcutSizeMinimized; }
char* langShortcutToggleFullscren() { return ls->shortcutToggleFullscren; }
char* langShortcutVolumeIncrease() { return ls->shortcutVolumeIncrease; }
char* langShortcutVolumeDecrease() { return ls->shortcutVolumeDecrease; }
char* langShortcutVolumeMute() { return ls->shortcutVolumeMute; }
char* langShortcutVolumeStereo() { return ls->shortcutVolumeStereo; }
char* langShortcutSwitchMsxAudio() { return ls->shortcutSwitchMsxAudio; }
char* langShortcutSwitchFront() { return ls->shortcutSwitchFront; }
char* langShortcutSwitchPause() { return ls->shortcutSwitchPause; }
char* langShortcutToggleMouseLock() { return ls->shortcutToggleMouseLock; }
char* langShortcutEmuSpeedMax() { return ls->shortcutEmuSpeedMax; }
char* langShortcutEmuSpeedMaxToggle() { return ls->shortcutEmuSpeedToggle; }
char* langShortcutEmuSpeedNormal() { return ls->shortcutEmuSpeedNormal; }
char* langShortcutEmuSpeedInc() { return ls->shortcutEmuSpeedInc; }
char* langShortcutEmuSpeedDec() { return ls->shortcutEmuSpeedDec; }
char* langShortcutThemeSwitch() { return ls->shortcutThemeSwitch; }
char* langShortcutShowEmuProp() { return ls->shortcutShowEmuProp; }
char* langShortcutShowVideoProp() { return ls->shortcutShowVideoProp; }
char* langShortcutShowAudioProp() { return ls->shortcutShowAudioProp; }
char* langShortcutShowCtrlProp() { return ls->shortcutShowCtrlProp; }
char* langShortcutShowPerfProp() { return ls->shortcutShowPerfProp; }
char* langShortcutShowSettProp() { return ls->shortcutShowSettProp; }
char* langShortcutShowPorts() { return ls->shortcutShowPorts; }
char* langShortcutShowLanguage() { return ls->shortcutShowLanguage; }
char* langShortcutShowMachines() { return ls->shortcutShowMachines; }
char* langShortcutShowShortcuts() { return ls->shortcutShowShortcuts; }
char* langShortcutShowKeyboard() { return ls->shortcutShowKeyboard; }
char* langShortcutShowMixer() { return ls->shortcutShowMixer; }
char* langShortcutShowDebugger() { return ls->shortcutShowDebugger; }
char* langShortcutShowTrainer() { return ls->shortcutShowTrainer; }
char* langShortcutShowHelp() { return ls->shortcutShowHelp; }
char* langShortcutShowAbout() { return ls->shortcutShowAbout; }
char* langShortcutShowFiles() { return ls->shortcutShowFiles; }
char* langShortcutToggleSpriteEnable() { return ls->shortcutToggleSpriteEnable; }
char* langShortcutToggleFdcTiming() { return ls->shortcutToggleFdcTiming; }
char* langShortcutToggleCpuTrace() { return ls->shortcutToggleCpuTrace; }


//----------------------
// Keyboard config lines
//----------------------

char* langKeyconfigSelectedKey() { return ls->keyconfigSelectedKey; }
char* langKeyconfigMappedTo() { return ls->keyconfigMappedTo; }
char* langKeyconfigMappingScheme() { return ls->keyconfigMappingScheme; }


//----------------------
// Rom type lines
//----------------------

char* langRomTypeStandard() { return ls->romTypeStandard; }
char* langRomTypeMsxdos2() { return ls->romTypeMsxdos2; }
char* langRomTypeKonamiScc() { return ls->romTypeKonamiScc; }
char* langRomTypeKonami() { return ls->romTypeKonami; }
char* langRomTypeAscii8() { return ls->romTypeAscii8; }
char* langRomTypeAscii16() { return ls->romTypeAscii16; }
char* langRomTypeGameMaster2() { return ls->romTypeGameMaster2; }
char* langRomTypeAscii8Sram() { return ls->romTypeAscii8Sram; }
char* langRomTypeAscii16Sram() { return ls->romTypeAscii16Sram; }
char* langRomTypeRtype() { return ls->romTypeRtype; }
char* langRomTypeCrossblaim() { return ls->romTypeCrossblaim; }
char* langRomTypeHarryFox() { return ls->romTypeHarryFox; }
char* langRomTypeMajutsushi() { return ls->romTypeMajutsushi; }
char* langRomTypeZenima80() { return ls->romTypeZenima80; }
char* langRomTypeZenima90() { return ls->romTypeZenima90; }
char* langRomTypeZenima126() { return ls->romTypeZenima126; }
char* langRomTypeScc() { return ls->romTypeScc; }
char* langRomTypeSccPlus() { return ls->romTypeSccPlus; }
char* langRomTypeSnatcher() { return ls->romTypeSnatcher; }
char* langRomTypeSdSnatcher() { return ls->romTypeSdSnatcher; }
char* langRomTypeSccMirrored() { return ls->romTypeSccMirrored; }
char* langRomTypeSccExtended() { return ls->romTypeSccExtended; }
char* langRomTypeFmpac() { return ls->romTypeFmpac; }
char* langRomTypeFmpak() { return ls->romTypeFmpak; }
char* langRomTypeKonamiGeneric() { return ls->romTypeKonamiGeneric; }
char* langRomTypeSuperPierrot() { return ls->romTypeSuperPierrot; }
char* langRomTypeMirrored() { return ls->romTypeMirrored; }
char* langRomTypeNormal() { return ls->romTypeNormal; }
char* langRomTypeDiskPatch() { return ls->romTypeDiskPatch; }
char* langRomTypeCasPatch() { return ls->romTypeCasPatch; }
char* langRomTypeTc8566afFdc() { return ls->romTypeTc8566afFdc; }
char* langRomTypeMicrosolFdc() { return ls->romTypeMicrosolFdc; }
char* langRomTypeNationalFdc() { return ls->romTypeNationalFdc; }
char* langRomTypePhilipsFdc() { return ls->romTypePhilipsFdc; }
char* langRomTypeSvi738Fdc() { return ls->romTypeSvi738Fdc; }
char* langRomTypeMappedRam() { return ls->romTypeMappedRam; }
char* langRomTypeMirroredRam1k() { return ls->romTypeMirroredRam1k; }
char* langRomTypeNormalRam() { return ls->romTypeNormalRam; }
char* langRomTypeKanji() { return ls->romTypeKanji; }
char* langRomTypeHolyQuran() { return ls->romTypeHolyQuran; }
char* langRomTypeMatsushitaSram() { return ls->romTypeMatsushitaSram; }
char* langRomTypePanasonic16() { return ls->romTypePanasonic16; }
char* langRomTypePanasonic32() { return ls->romTypePanasonic32; }
char* langRomTypeBunsetsu() { return ls->romTypeBunsetsu; }
char* langRomTypeJisyo() { return ls->romTypeJisyo; }
char* langRomTypeKanji12() { return ls->romTypeKanji12; }
char* langRomTypeNationalSram() { return ls->romTypeNationalSram; }
char* langRomTypeS1985() { return ls->romTypeS1985; }
char* langRomTypeS1990() { return ls->romTypeS1990; }
char* langRomTypeTurborPause() { return ls->romTypeTurborPause; }
char* langRomTypeF4deviceNormal() { return ls->romTypeF4deviceNormal; }
char* langRomTypeF4deviceInvert() { return ls->romTypeF4deviceInvert; }
char* langRomTypeMsxMidi() { return ls->romTypeMsxMidi; }
char* langRomTypeTurborTimer() { return ls->romTypeTurborTimer; }
char* langRomTypeKoei() { return ls->romTypeKoei; }
char* langRomTypeBasic() { return ls->romTypeBasic; }
char* langRomTypeHalnote() { return ls->romTypeHalnote; }
char* langRomTypeLodeRunner() { return ls->romTypeLodeRunner; }
char* langRomTypeNormal4000() { return ls->romTypeNormal4000; }
char* langRomTypeNormalC000() { return ls->romTypeNormalC000; }
char* langRomTypeKonamiSynth() { return ls->romTypeKonamiSynth; }
char* langRomTypeKonamiKbdMast() { return ls->romTypeKonamiKbdMast; }
char* langRomTypeKonamiWordPro() { return ls->romTypeKonamiWordPro; }
char* langRomTypePac() { return ls->romTypePac; }
char* langRomTypeMegaRam() { return ls->romTypeMegaRam; }
char* langRomTypeMegaRam128() { return ls->romTypeMegaRam128; }
char* langRomTypeMegaRam256() { return ls->romTypeMegaRam256; }
char* langRomTypeMegaRam512() { return ls->romTypeMegaRam512; }
char* langRomTypeMegaRam768() { return ls->romTypeMegaRam768; }
char* langRomTypeMegaRam2mb() { return ls->romTypeMegaRam2mb; }
char* langRomTypeExtRam() { return ls->romTypeExtRam; }
char* langRomTypeExtRam512() { return ls->romTypeExtRam512; }
char* langRomTypeExtRam1mb() { return ls->romTypeExtRam1mb; }
char* langRomTypeExtRam2mb() { return ls->romTypeExtRam2mb; }
char* langRomTypeExtRam4mb() { return ls->romTypeExtRam4mb; }
char* langRomTypeMsxMusic() { return ls->romTypeMsxMusic; }
char* langRomTypeMsxAudio() { return ls->romTypeMsxAudio; }
char* langRomTypeY8950() { return ls->romTypeY8950; }
char* langRomTypeMoonsound() { return ls->romTypeMoonsound; }
char* langRomTypeSvi328Cart() { return ls->romTypeSvi328Cart; }
char* langRomTypeSvi328Fdc() { return ls->romTypeSvi328Fdc; }
char* langRomTypeSvi328Prn() { return ls->romTypeSvi328Prn; }
char* langRomTypeSvi328Uart() { return ls->romTypeSvi328Uart; }
char* langRomTypeSvi328col80() { return ls->romTypeSvi328col80; }
char* langRomTypeSvi727col80() { return ls->romTypeSvi727col80; }
char* langRomTypeColecoCart() { return ls->romTypeColecoCart; }
char* langRomTypeSg1000Cart() { return ls->romTypeSg1000Cart; }
char* langRomTypeTheCastle() { return ls->romTypeTheCastle; }
char* langRomTypeSonyHbi55() { return ls->romTypeSonyHbi55; }
char* langRomTypeMsxPrinter() { return ls->romTypeMsxPrinter; }
char* langRomTypeTurborPcm() { return ls->romTypeTurborPcm; }
char* langRomTypeGameReader() { return ls->romTypeGameReader; }
char* langRomTypeSunriseIde() { return ls->romTypeSunriseIde; }
char* langRomTypeBeerIde() { return ls->romTypeBeerIde; }
char* langRomTypeGide() { return ls->romTypeGide; }
char* langRomTypeVmx80() { return ls->romTypeVmx80; }
char* langRomTypeNms8280Digitiz() { return ls->romTypeNms8280Digitiz; }
char* langRomTypeHbiV1Digitiz() { return ls->romTypeHbiV1Digitiz; }
char* langRomTypeFmdas() { return ls->romTypeFmdas; }
char* langRomTypeSfg01() { return ls->romTypeSfg01; }
char* langRomTypeSfg05() { return ls->romTypeSfg05; }


//----------------------
// Debug type lines
//----------------------

char* langDbgMemVisible() { return ls->dbgMemVisible; }
char* langDbgMemRamNormal() { return ls->dbgMemRamNormal; }
char* langDbgMemRamMapped() { return ls->dbgMemRamMapped; }
char* langDbgMemVram() { return ls->dbgMemVram; }
char* langDbgMemYmf278() { return ls->dbgMemYmf278; }
char* langDbgMemAy8950() { return ls->dbgMemAy8950; }
char* langDbgMemScc() { return ls->dbgMemScc; }

char* langDbgCallstack() { return ls->dbgCallstack; }

char* langDbgRegs() { return ls->dbgRegs; }
char* langDbgRegsCpu() { return ls->dbgRegsCpu; }
char* langDbgRegsYmf262() { return ls->dbgRegsYmf262; }
char* langDbgRegsYmf278() { return ls->dbgRegsYmf278; }
char* langDbgRegsAy8950() { return ls->dbgRegsAy8950; }
char* langDbgRegsYm2413() { return ls->dbgRegsYm2413; }

char* langDbgDevRamMapper() { return ls->dbgDevRamMapper; }
char* langDbgDevRam() { return ls->dbgDevRam; }
char* langDbgDevIdeBeer() { return ls->dbgDevIdeBeer; }
char* langDbgDevIdeGide() { return ls->dbgDevIdeGide; }
char* langDbgDevF4Device() { return ls->dbgDevF4Device; }
char* langDbgDevFmpac() { return ls->dbgDevFmpac; }
char* langDbgDevFmpak() { return ls->dbgDevFmpak; }
char* langDbgDevKanji() { return ls->dbgDevKanji; }
char* langDbgDevKanji12() { return ls->dbgDevKanji12; }
char* langDbgDevKonamiKbd() { return ls->dbgDevKonamiKbd; }
char* langDbgDevKorean80() { return ls->dbgDevKorean80; }
char* langDbgDevKorean90() { return ls->dbgDevKorean90; }
char* langDbgDevKorean128() { return ls->dbgDevKorean128; }
char* langDbgDevMegaRam() { return ls->dbgDevMegaRam; }
char* langDbgDevFdcMicrosol() { return ls->dbgDevFdcMicrosol; }
char* langDbgDevMoonsound() { return ls->dbgDevMoonsound; }
char* langDbgDevMsxAudio() { return ls->dbgDevMsxAudio; }
char* langDbgDevMsxAudioMidi() { return ls->dbgDevMsxAudioMidi; }
char* langDbgDevMsxMusic() { return ls->dbgDevMsxMusic; }
char* langDbgDevPrinter() { return ls->dbgDevPrinter; }
char* langDbgDevRs232() { return ls->dbgDevRs232; }
char* langDbgDevS1990() { return ls->dbgDevS1990; }
char* langDbgDevSfg05() { return ls->dbgDevSfg05; }
char* langDbgDevHbi55() { return ls->dbgDevHbi55; }
char* langDbgDevSviFdc() { return ls->dbgDevSviFdc; }
char* langDbgDevSviPrn() { return ls->dbgDevSviPrn; }
char* langDbgDevSvi80Col() { return ls->dbgDevSvi80Col; }
char* langDbgDevPcm() { return ls->dbgDevPcm; }
char* langDbgDevMatsushita() { return ls->dbgDevMatsushita; }
char* langDbgDevS1985() { return ls->dbgDevS1985; }
char* langDbgDevCrtc6845() { return ls->dbgDevCrtc6845; }
char* langDbgDevTms9929A() { return ls->dbgDevTms9929A; }
char* langDbgDevTms99x8A() { return ls->dbgDevTms99x8A; }
char* langDbgDevV9938() { return ls->dbgDevV9938; }
char* langDbgDevV9958() { return ls->dbgDevV9958; }
char* langDbgDevZ80() { return ls->dbgDevZ80; }
char* langDbgDevMsxMidi() { return ls->dbgDevMsxMidi; }
char* langDbgDevPpi() { return ls->dbgDevPpi; }
char* langDbgDevRtc() { return ls->dbgDevRtc; }
char* langDbgDevTrPause() { return ls->dbgDevTrPause; }
char* langDbgDevAy8910() { return ls->dbgDevAy8910; }
char* langDbgDevScc() { return ls->dbgDevScc; }


//----------------------
// Debug type lines
// Note: Can only be translated to european languages
//----------------------
char* langAboutScrollThanksTo() { return ls->aboutScrollThanksTo; }
char* langAboutScrollAndYou() { return ls->aboutScrollAndYou; }
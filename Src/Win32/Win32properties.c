/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32properties.c,v $
**
** $Revision: 1.64 $
**
** $Date: 2006/06/30 22:42:00 $
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
#include <windows.h>
#include <math.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __GNUC__ // FIXME: Include is not available in gnu c
static HRESULT StringCchCopy(LPTSTR d, size_t l, LPCTSTR s) { strncpy(d, s, l); d[l-1]=0; return S_OK; }
static HRESULT StringCchLength(LPCTSTR s, size_t m, size_t *l) { *l = strlen(s); if (*l > m) *l = m; return S_OK; }
#define StringCchPrintf(s, l, f, a, b) sprintf(s, f, a, b)
#else
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
#endif

#include "Win32Properties.h"
#include "Win32DirectX.h"
#include "ThemeLoader.h"
#include "Win32keyboard.h"
#include "resource.h"
#include "Language.h"
#include "Machine.h"
#include "Board.h"
#include "Win32Midi.h"

static HWND hDlgSound = NULL;
static int propModified = 0;
static Mixer* theMixer;
static Video* theVideo;
static int centered = 0;
extern void emulatorRestartSound();

static int openLogFile(HWND hwndOwner, char* fileName)
{
    OPENFILENAME ofn; 
    static char pFileName[MAX_PATH];
    char  curDir[MAX_PATH];
    BOOL rv;

    pFileName[0] = 0; 

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = GetModuleHandle(NULL);
    ofn.lpstrFilter = "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = NULL; 
    ofn.lpstrTitle = langPropPortsOpenLogFile(); 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT; 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = NULL; 
    ofn.lpTemplateName = NULL; 

    GetCurrentDirectory(MAX_PATH, curDir);

    rv = GetSaveFileName(&ofn); 

    SetCurrentDirectory(curDir);

    if (rv) {
        strcpy(fileName, pFileName);
    }

    return rv;
}


static char pVideoMonData[4][64];
static char* pVideoMon[] = {
    pVideoMonData[0],
    pVideoMonData[1],
    pVideoMonData[2],
    pVideoMonData[3],
    NULL
};

static char pVideoTypeData[2][64];
static char* pVideoVideoType[] = {
    pVideoTypeData[0], 
    pVideoTypeData[1],
    NULL
};

static char pVideoEmuData[9][64];
static char* pVideoPalEmu[] = {
    pVideoEmuData[0],
    pVideoEmuData[1],
    pVideoEmuData[2],
    pVideoEmuData[3],
    pVideoEmuData[4],
    pVideoEmuData[5],
    pVideoEmuData[6],
    pVideoEmuData[7],
    NULL
};

static char pVideoSizeData[6][64];
static char* pVideoMonSize[] = {
    pVideoSizeData[0],
    pVideoSizeData[1],
    pVideoSizeData[2],
    NULL
};

static char pVideoDriverData[3][64];
static char* pVideoDriver[] = {
    pVideoDriverData[0],
    pVideoDriverData[1],
    pVideoDriverData[2],
    NULL
};

static char pVideoFrameSkipData[6][64];
static char* pVideoFrameSkip[] = {
    pVideoFrameSkipData[0],
    pVideoFrameSkipData[1],
    pVideoFrameSkipData[2],
    pVideoFrameSkipData[3],
    pVideoFrameSkipData[4],
    pVideoFrameSkipData[5],
    NULL
};

static char pSoundDriverData[3][64];
static char* pSoundDriver[] = {
    pSoundDriverData[0],
    pSoundDriverData[1],
    pSoundDriverData[2],
    NULL
};

static char pEmuSyncData[5][64];
static char* pEmuSync[] = {
    pEmuSyncData[0],
    pEmuSyncData[1],
    pEmuSyncData[2],
    pEmuSyncData[3],
    pEmuSyncData[4],
    NULL
};

static int soundBufSizes[] = { 10, 20, 50, 100, 150, 200, 250, 300, 350 };

static char* pSoundBufferSize[] = {
    "10 ms",
    "20 ms",
    "50 ms",
    "100 ms",
    "150 ms",
    "200 ms",
    "250 ms",
    "300 ms",
    "350 ms",
    NULL
};


static void setButtonCheck(HWND hDlg, int id, int check, int enable) {
    HWND hwnd = GetDlgItem(hDlg, id);

    if (check) {
        SendMessage(hwnd, BM_SETCHECK, BST_CHECKED, 0);
    }
    else {
        SendMessage(hwnd, BM_SETCHECK, BST_UNCHECKED, 0);
    }
    if (!enable) {
        SendMessage(hwnd, BM_SETCHECK, BST_INDETERMINATE, 0);
    }
}

static int getButtonCheck(HWND hDlg, int id) {
    HWND hwnd = GetDlgItem(hDlg, id);

    return BST_CHECKED == SendMessage(hwnd, BM_GETCHECK, 0, 0) ? 1 : 0;
}

static void initDropList(HWND hDlg, int id, char** pList, int index) {
    while (*pList != NULL && **pList != 0) {
        SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)*pList);
        pList++;
    }

    SendDlgItemMessage(hDlg, id, CB_SETCURSEL, index, 0);
}

static int getDropListIndex(HWND hDlg, int id, char** pList) {
    int index = 0;
    char s[64];

    GetDlgItemText(hDlg, id, s, 63);
    
    while (*pList != NULL) {
        if (0 == strcmp(s, *pList)) {
            return index;
        }
        index++;
        pList++;
    }

    return -1;
}

static char* strEmuSpeed(int logFrequency) {
    UInt32 frequency = (UInt32)(3579545 * pow(2.0, (logFrequency - 50) / 15.0515));
    static char buffer[32];

    sprintf(buffer, "%d.%03dMHz (%d%%)", frequency / 1000000, (frequency / 1000) % 1000, frequency * 10 / 357954);
    return buffer;

}

static BOOL CALLBACK emulationDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;
    static int curSpeed;
    static char machineName[64];

    switch (iMsg) {
    case WM_INITDIALOG:    
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;

        SendMessage(GetDlgItem(hDlg, IDC_EMUGENERALGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropEmuGeneralGB());
        SendMessage(GetDlgItem(hDlg, IDC_EMUFAMILYTEXT), WM_SETTEXT, 0, (LPARAM)langPropEmuFamilyText());
        SendMessage(GetDlgItem(hDlg, IDC_VDPFREQTEXT), WM_SETTEXT, 0, (LPARAM)langPropVideoFreqText());
        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEEDTEXT), WM_SETTEXT, 0, (LPARAM)langPropEmuSpeedText());
        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEEDGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropEmuSpeedGB());
        SendMessage(GetDlgItem(hDlg, IDC_EMUFRONTSWITCHGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropEmuFrontSwitchGB());
        
        SetWindowText(GetDlgItem(hDlg, IDC_EMUFDCTIMING),   langPropEmuFdcTiming());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUFRONTSWITCH), langPropEmuFrontSwitch());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUPAUSESWITCH), langPropEmuPauseSwitch());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUAUDIOSWITCH), langPropEmuAudioSwitch());

        setButtonCheck(hDlg, IDC_EMUFDCTIMING,   !pProperties->emulation.enableFdcTiming, 1);
        setButtonCheck(hDlg, IDC_EMUFRONTSWITCH, pProperties->emulation.frontSwitch, 1);
        setButtonCheck(hDlg, IDC_EMUPAUSESWITCH, pProperties->emulation.pauseSwitch, 1);
        setButtonCheck(hDlg, IDC_EMUAUDIOSWITCH, pProperties->emulation.audioSwitch, 1);

        curSpeed = pProperties->emulation.speed;

        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_ADDSTRING, 0, (LPARAM)langPropVideoFreqAuto());
        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_ADDSTRING, 0, (LPARAM)"50 Hz");
        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_ADDSTRING, 0, (LPARAM)"60 Hz");

        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_SETCURSEL, pProperties->emulation.vdpSyncMode, 0);

        machineName[0] = 0;

        {
            char** machineNames = machineGetAvailable(1);
            int index = 0;
            while (*machineNames != NULL) {
                char buffer[128];

                sprintf(buffer, "%s", *machineNames);

                SendDlgItemMessage(hDlg, IDC_EMUFAMILY, CB_ADDSTRING, 0, (LPARAM)buffer);

                if (index == 0 || 0 == strcmp(*machineNames, pProperties->emulation.machineName)) {
                    SendDlgItemMessage(hDlg, IDC_EMUFAMILY, CB_SETCURSEL, index, 0);
                    strcpy(machineName, *machineNames);
                }
                machineNames++;
                index++;
            }
        }

        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEEDCUR), WM_SETTEXT, 0, (LPARAM)strEmuSpeed(curSpeed));

        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEED), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 100));
        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEED), TBM_SETPOS,   1, (LPARAM)curSpeed);

        return FALSE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_EMUFAMILY) {
            char buffer[64];
            int idx = SendMessage(GetDlgItem(hDlg, IDC_EMUFAMILY), CB_GETCURSEL, 0, 0);
            int rv = SendMessage(GetDlgItem(hDlg, IDC_EMUFAMILY), CB_GETLBTEXT, idx, (LPARAM)buffer);

            if (rv != CB_ERR) {
                if (strcmp(machineName, buffer)) {
                    strcpy(machineName, buffer);
                }
            }
            return TRUE;
        }
        return TRUE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if (wParam == IDC_EMUSPEED) {
            curSpeed = SendMessage(GetDlgItem(hDlg, IDC_EMUSPEED), TBM_GETPOS, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_EMUSPEEDCUR), WM_SETTEXT, 0, (LPARAM)strEmuSpeed(curSpeed));
            return TRUE;
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }
            
        {
            char** machineNames = machineGetAvailable(1);
            int index = 0;
            char buffer[64];

            pProperties->emulation.enableFdcTiming = !getButtonCheck(hDlg, IDC_EMUFDCTIMING);
            pProperties->emulation.frontSwitch = getButtonCheck(hDlg, IDC_EMUFRONTSWITCH);
            pProperties->emulation.pauseSwitch = getButtonCheck(hDlg, IDC_EMUPAUSESWITCH);
            pProperties->emulation.audioSwitch = getButtonCheck(hDlg, IDC_EMUAUDIOSWITCH);

            pProperties->emulation.vdpSyncMode = SendMessage(GetDlgItem(hDlg, IDC_VDPFREQ), CB_GETCURSEL, 0, 0);

            GetDlgItemText(hDlg, IDC_EMUFAMILY, buffer, 63);
            
            while (*machineNames != NULL) {
                if (0 == strcmp(buffer, *machineNames)) {
                    strcpy(pProperties->emulation.machineName, buffer);
                    break;
                }
                machineNames++;
                index++;
            }
        }

        pProperties->emulation.speed        = curSpeed;

        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

static RomType romTypeList[] = {
    ROM_GAMEREADER,
    ROM_ASCII8,
    ROM_ASCII8SRAM,
    ROM_ASCII16,
    ROM_ASCII16SRAM,
    ROM_KONAMI4,
    ROM_KONAMI5,
    ROM_PLAIN,
    ROM_BASIC,
    ROM_0x4000,
    ROM_0xC000,
    ROM_KOEI,
    ROM_RTYPE,
    ROM_CROSSBLAIM,
    ROM_HARRYFOX,
    ROM_LODERUNNER,
    ROM_KONAMISYNTH,
    ROM_KONAMKBDMAS,
    ROM_KONWORDPRO,
    ROM_MAJUTSUSHI,
    ROM_HALNOTE,
    ROM_SCC,
    ROM_SCCPLUS,
    ROM_KONAMI4NF, 
    ROM_ASCII16NF,
    ROM_GAMEMASTER2,
    ROM_KOREAN80,
    ROM_KOREAN90,
    ROM_KOREAN126,
    ROM_HOLYQURAN,
    ROM_FMPAC,
    ROM_FMPAK,
    ROM_MSXMUSIC,
    ROM_MSXAUDIO,
    ROM_MOONSOUND,
    ROM_DISKPATCH,
    ROM_CASPATCH,
    ROM_TC8566AF,
    ROM_MICROSOL,
    ROM_NATIONALFDC,
    ROM_PHILIPSFDC,
    ROM_SVI738FDC,
    ROM_GIDE,
    ROM_SUNRISEIDE,
    ROM_BEERIDE,
    ROM_KANJI,
    ROM_KANJI12,
    ROM_JISYO,
    ROM_BUNSETU,
    ROM_MSXDOS2,
    ROM_NATIONAL,
    ROM_PANASONIC16,
    ROM_PANASONIC32,
    ROM_SONYHBI55,
    ROM_MSXAUDIODEV,
    ROM_TURBORPCM,
    ROM_MICROSOL80,
    ROM_SVI727,
    ROM_SONYHBIV1,
    ROM_FMDAS,
    ROM_YAMAHASFG01,
    ROM_YAMAHASFG05,
    ROM_UNKNOWN,
};

static BOOL CALLBACK filesDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;
    int i;

    switch (iMsg) {
    case WM_INITDIALOG:  
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;
  
        SendMessage(GetDlgItem(hDlg, IDC_SETINGSFILEHISTORYGOUPBOX), WM_SETTEXT, 0, (LPARAM)langPropSetFileHistoryGB());
        SendMessage(GetDlgItem(hDlg, IDC_SETINGSHISTORYSIZETEXT), WM_SETTEXT, 0, (LPARAM)langPropSetFileHistorySize());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSHISTORYCLEAR), langPropSetFileHistoryClear());

        {
            char buffer[32];
            sprintf(buffer, "%d", pProperties->filehistory.count);
            SetWindowText(GetDlgItem(hDlg, IDC_SETINGSHISTORYSIZE), buffer);
        }

        for (i = 0; romTypeList[i] != ROM_UNKNOWN; i++) {
            SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(romTypeList[i]));
            if (pProperties->cartridge.defaultType == romTypeList[i]) {
                SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_SETCURSEL, i, 0);
            }
        }

        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSROMTYPEGB), langPropOpenRomGB());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSROMTYPETEXT), langPropDefaultRomType());

        SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_ADDSTRING, 0, (LPARAM)langPropGuessRomType());
        if (pProperties->cartridge.defaultType == romTypeList[i]) {
            SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_SETCURSEL, i, 0);
        }

        {
            char text[64];

            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSDEFSLOTSGB), langPropSettDefSlotGB());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSSLOTS), langPropSettDefSlots());

            sprintf(text, "%s 1", langPropSettDefSlot());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSSLOT1), text);
            sprintf(text, "%s 2", langPropSettDefSlot());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSSLOT2), text);
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSDRIVES), langPropSettDefDrives());
            sprintf(text, "%s A", langPropSettDefDrive());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSDRIVEA), text);
            sprintf(text, "%s B", langPropSettDefDrive());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSDRIVEB), text);
        }

        setButtonCheck(hDlg, IDC_SETTINGSSLOT1, pProperties->cartridge.quickStartDrive == 0, 1);
        setButtonCheck(hDlg, IDC_SETTINGSSLOT2, pProperties->cartridge.quickStartDrive == 1, 1);
        setButtonCheck(hDlg, IDC_SETTINGSDRIVEA, pProperties->diskdrive.quickStartDrive == 0, 1);
        setButtonCheck(hDlg, IDC_SETTINGSDRIVEB, pProperties->diskdrive.quickStartDrive == 1, 1);

        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_SETTINGSHISTORYCLEAR:
            {
                int rv = MessageBox(NULL, langPropClearFileHistory(), langWarningTitle(), MB_ICONWARNING | MB_OKCANCEL);
                if (rv == IDOK) {
                    int i;

                    for (i = 0; i < MAX_HISTORY; i++) {
                        pProperties->filehistory.cartridge[0][i][0] = 0;
                        pProperties->filehistory.cartridge[1][i][0] = 0;
                        pProperties->filehistory.diskdrive[0][i][0] = 0;
                        pProperties->filehistory.diskdrive[1][i][0] = 0;
                        pProperties->filehistory.cassette[0][i][0] = 0;
                    }

                    pProperties->filehistory.quicksave[0] = 0;

                    EnableWindow(GetDlgItem(hDlg, IDC_SETTINGSHISTORYCLEAR), FALSE);
                }
            }
            break;

        case IDC_SETTINGSSLOT1:
            setButtonCheck(hDlg, IDC_SETTINGSSLOT1, 1, 1);
            setButtonCheck(hDlg, IDC_SETTINGSSLOT2, 0, 1);
            break;

        case IDC_SETTINGSSLOT2:
            setButtonCheck(hDlg, IDC_SETTINGSSLOT1, 0, 1);
            setButtonCheck(hDlg, IDC_SETTINGSSLOT2, 1, 1);
            break;

        case IDC_SETTINGSDRIVEA:
            setButtonCheck(hDlg, IDC_SETTINGSDRIVEA, 1, 1);
            setButtonCheck(hDlg, IDC_SETTINGSDRIVEB, 0, 1);
            break;

        case IDC_SETTINGSDRIVEB:
            setButtonCheck(hDlg, IDC_SETTINGSDRIVEA, 0, 1);
            setButtonCheck(hDlg, IDC_SETTINGSDRIVEB, 1, 1);
            break;
        }
        return TRUE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }
        
        i = SendMessage(GetDlgItem(hDlg, IDC_SETTINGSROMTYPE), CB_GETCURSEL, 0, 0);
        if (i != CB_ERR) {
            pProperties->cartridge.defaultType = romTypeList[i];
        }

        {
            char buffer[64];

            GetDlgItemText(hDlg, IDC_SETINGSHISTORYSIZE, buffer, 63);

            if (isdigit(*buffer)) {
                int count = atoi(buffer);
                if (count < 0)  count = 0;
                if (count > 30) count = 30;

                pProperties->filehistory.count = count;
            }
        }

        pProperties->cartridge.quickStartDrive = getButtonCheck(hDlg, IDC_SETTINGSSLOT2) ? 1 : 0;
        pProperties->diskdrive.quickStartDrive = getButtonCheck(hDlg, IDC_SETTINGSDRIVEB) ? 1 : 0;

        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

extern void archUpdateWindow();

static BOOL CALLBACK settingsDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;
    static char oldTheme[128];

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;
        strcpy(oldTheme, pProperties->settings.themeName);

        SetWindowText(GetDlgItem(hDlg, IDC_APEARANCETHEMEGB), langPropThemeGB());
        SetWindowText(GetDlgItem(hDlg, IDC_APEARANCETHEMETEXT), langPropTheme());

        {
            ThemeCollection** themeNames = themeGetAvailable();
            int index = 0;
            while (*themeNames != NULL) {
                SendDlgItemMessage(hDlg, IDC_APEARANCETHEME, CB_ADDSTRING, 0, (LPARAM)*themeNames);

                if (index == 0 || 0 == strcmp((*themeNames)->name, pProperties->settings.themeName)) {
                    SendDlgItemMessage(hDlg, IDC_APEARANCETHEME, CB_SETCURSEL, index, 0);
                }
                themeNames++;
                index++;
            }
        }

        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSWINDOWSENV), langPropWindowsEnvGB());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSSCREENSAVER), langPropScreenSaver());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSFILETYPES), langPropFileTypes());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSDISABLEWINKEYS), langPropDisableWinKeys());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSPRIORITYBOOST), langPropPriorityBoost());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSSCREENSHOTPNG), langPropScreenshotPng());

        setButtonCheck(hDlg, IDC_SETTINGSFILETYPES, pProperties->emulation.registerFileTypes, 1);
        setButtonCheck(hDlg, IDC_SETTINGSDISABLEWINKEYS, pProperties->emulation.disableWinKeys, 1);
        setButtonCheck(hDlg, IDC_SETTINGSPRIORITYBOOST, pProperties->emulation.priorityBoost, 1);
        setButtonCheck(hDlg, IDC_SETTINGSSCREENSAVER, pProperties->settings.disableScreensaver, 1);
        setButtonCheck(hDlg, IDC_SETTINGSSCREENSHOTPNG, pProperties->settings.usePngScreenshots, 1);

        EnableWindow(GetDlgItem(hDlg, IDC_SETTINGSFILETYPES), !pProperties->settings.portable);

        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_APEARANCETHEME:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                char buffer[128];
                int idx = SendMessage(GetDlgItem(hDlg, IDC_APEARANCETHEME), CB_GETCURSEL, 0, 0);
                int rv = SendMessage(GetDlgItem(hDlg, IDC_APEARANCETHEME), CB_GETLBTEXT, idx, (LPARAM)buffer);

                if (rv != CB_ERR) {
                    strcpy(pProperties->settings.themeName, buffer);
                    archUpdateWindow();
                    SetFocus(GetParent(hDlg));
                }
            }
            break;
        }
        return 0;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) == PSN_QUERYCANCEL) {
            strcpy(pProperties->settings.themeName, oldTheme);
            archUpdateWindow();
            return FALSE;
        }
        if ((((NMHDR FAR *)lParam)->code) == PSN_APPLY) {
            return FALSE;
        }

        pProperties->emulation.registerFileTypes = getButtonCheck(hDlg, IDC_SETTINGSFILETYPES);
        pProperties->emulation.disableWinKeys    = getButtonCheck(hDlg, IDC_SETTINGSDISABLEWINKEYS);
        pProperties->settings.disableScreensaver = getButtonCheck(hDlg, IDC_SETTINGSSCREENSAVER);
        pProperties->emulation.priorityBoost     = getButtonCheck(hDlg, IDC_SETTINGSPRIORITYBOOST);
        pProperties->settings.usePngScreenshots  = getButtonCheck(hDlg, IDC_SETTINGSSCREENSHOTPNG);
        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

static void updateFullscreenResList(HWND hDlg) {
    int count = DirectDrawGetAvailableDisplayModeCount();
    DxDisplayMode* curDdm = DirectDrawGetDisplayMode();
    int i;

    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_PERFFULLSCREEN, CB_DELETESTRING, 0, 0));

    for (i = 0; i < count; i++) {
        char text[32];
        DxDisplayMode* ddm = DirectDrawGetAvailableDisplayMode(i);
        sprintf(text, "%d x %d - %d bit", ddm->width, ddm->height, ddm->bitCount);
        SendDlgItemMessage(hDlg, IDC_PERFFULLSCREEN, CB_ADDSTRING, 0, (LPARAM)text);
        if (ddm->width == curDdm->width && ddm->height == curDdm->height && ddm->bitCount == curDdm->bitCount) {
            SendDlgItemMessage(hDlg, IDC_PERFFULLSCREEN, CB_SETCURSEL, i, 0);
        }
    }
}

static void getFullscreenResList(HWND hDlg, int* width, int* height, int* bitCount) 
{
    DxDisplayMode* ddm = NULL;
    int index = SendDlgItemMessage(hDlg, IDC_PERFFULLSCREEN, CB_GETCURSEL, 0, 0);

    if (index >= 0) {
        ddm = DirectDrawGetAvailableDisplayMode(index);
    }
    if (ddm == NULL) {
        ddm = DirectDrawGetDisplayMode();
    }
    *width    = ddm->width;
    *height   = ddm->height;
    *bitCount = ddm->bitCount;
}


static BOOL CALLBACK performanceDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;

        /* Init language specific dropdown list data */
        sprintf(pVideoDriver[0], "%s", langEnumVideoDrvDirectDrawHW());
        sprintf(pVideoDriver[1], "%s", langEnumVideoDrvDirectDraw());
        sprintf(pVideoDriver[2], "%s", langEnumVideoDrvGDI());

        sprintf(pVideoFrameSkip[0], "%s", langEnumVideoFrameskip0());
        sprintf(pVideoFrameSkip[1], "%s", langEnumVideoFrameskip1());
        sprintf(pVideoFrameSkip[2], "%s", langEnumVideoFrameskip2());
        sprintf(pVideoFrameSkip[3], "%s", langEnumVideoFrameskip3());
        sprintf(pVideoFrameSkip[4], "%s", langEnumVideoFrameskip4());
        sprintf(pVideoFrameSkip[5], "%s", langEnumVideoFrameskip5());

        sprintf(pSoundDriver[0], "%s", langEnumSoundDrvNone());
        sprintf(pSoundDriver[1], "%s", langEnumSoundDrvWMM());
        sprintf(pSoundDriver[2], "%s", langEnumSoundDrvDirectX());

        sprintf(pEmuSync[0], "%s", langEnumEmuSyncNone());
        sprintf(pEmuSync[1], "%s", langEnumEmuSyncAuto());
        sprintf(pEmuSync[2], "%s", langEnumEmuSync1ms());
        sprintf(pEmuSync[3], "%s", langEnumEmuSyncVblank());
        sprintf(pEmuSync[4], "%s", langEnumEmuAsyncVblank());

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_PERFVIDEODRVGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropPerfVideoDrvGB());
        SendDlgItemMessage(hDlg, IDC_PERFDISPDRVTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfVideoDispDrvText());
        SendDlgItemMessage(hDlg, IDC_PERFFRAMESKIPTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfFrameSkipText());
        SendDlgItemMessage(hDlg, IDC_AUDIODRVGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropPerfAudioDrvGB());
        SendDlgItemMessage(hDlg, IDC_PERFSNDDRVTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfAudioDrvText());
        SendDlgItemMessage(hDlg, IDC_PERFSNDBUFSZTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfAudioBufSzText());
        SendDlgItemMessage(hDlg, IDC_PERFEMUGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropPerfEmuGB());
        SendDlgItemMessage(hDlg, IDC_PERFSYNCMODETEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfSyncModeText());
        SendDlgItemMessage(hDlg, IDC_PERFFULLSCREENTEXT, WM_SETTEXT, 0, (LPARAM)langPropFullscreenResText());
        

        initDropList(hDlg, IDC_SNDDRIVER, pSoundDriver, pProperties->sound.driver);
        {
            int index = 0;
            while (pProperties->sound.bufSize > soundBufSizes[index]) {
                if (soundBufSizes[index] == 350) {
                    break;
                }
                index++;
            }
            initDropList(hDlg, IDC_SNDBUFSZ, pSoundBufferSize, index);
        }
        initDropList(hDlg, IDC_VIDEODRV, pVideoDriver, pProperties->video.driver);
        initDropList(hDlg, IDC_FRAMESKIP, pVideoFrameSkip, pProperties->video.frameSkip);
        initDropList(hDlg, IDC_EMUSYNC, pEmuSync, pProperties->emulation.syncMethod);

        updateFullscreenResList(hDlg);

        return FALSE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }
            
        getFullscreenResList(hDlg, 
                             &pProperties->video.fullscreen.width,
                             &pProperties->video.fullscreen.height,
                             &pProperties->video.fullscreen.bitDepth);
                                
        pProperties->sound.driver           = getDropListIndex(hDlg, IDC_SNDDRIVER, pSoundDriver);
        pProperties->sound.bufSize          = soundBufSizes[getDropListIndex(hDlg, IDC_SNDBUFSZ, pSoundBufferSize)];
        pProperties->video.driver           = getDropListIndex(hDlg, IDC_VIDEODRV, pVideoDriver);
        pProperties->video.frameSkip        = getDropListIndex(hDlg, IDC_FRAMESKIP, pVideoFrameSkip);
        pProperties->emulation.syncMethod   = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);

        DirectDrawSetDisplayMode(pProperties->video.fullscreen.width,
                                pProperties->video.fullscreen.height,
                                pProperties->video.fullscreen.bitDepth);

        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

extern void updateEmuWindow();

static char* strPct(int value) {
    static char buffer[32];
    sprintf(buffer, "%d%%", value);
    return buffer;
}

static char* strDec(int value) {
    static char buffer[32];
    sprintf(buffer, "%d.%.2d", value / 100, value % 100);
    return buffer;
}

static char* strPt(int value) {
    static char buffer[32];
    sprintf(buffer, "%d.%d", value / 2, 5 * (value & 1));
    return buffer;
}

static BOOL CALLBACK videoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;
    static int monitorType;
    static int monitorColor;
    static int oldScanlinesEnable;
    static int oldScanlinesPct;
    static int oldColorGhostingEnable;
    static int oldColorGhostingWidth;
    static int oldHoriz;
    static int oldVert;
    static int oldDeinterlace;
    static int brightness;
    static int saturation;
    static int contrast;
    static int gamma;
    int value;

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;

        /* Init language specific dropdown list data */
        sprintf(pVideoMon[0], "%s", langEnumVideoMonColor());
        sprintf(pVideoMon[1], "%s", langEnumVideoMonGrey());
        sprintf(pVideoMon[2], "%s", langEnumVideoMonGreen());
        sprintf(pVideoMon[3], "%s", langEnumVideoMonAmber());

        sprintf(pVideoVideoType[0], "%s", langEnumVideoTypePAL());
        sprintf(pVideoVideoType[1], "%s", langEnumVideoTypeNTSC());

        sprintf(pVideoPalEmu[0], "%s", langEnumVideoEmuNone());
        sprintf(pVideoPalEmu[1], "%s", langEnumVideoEmuMonitor());
        sprintf(pVideoPalEmu[2], "%s", langEnumVideoEmuYc());
        sprintf(pVideoPalEmu[3], "%s", langEnumVideoEmuYcBlur());
        sprintf(pVideoPalEmu[4], "%s", langEnumVideoEmuComp());
        sprintf(pVideoPalEmu[5], "%s", langEnumVideoEmuCompBlur());
        sprintf(pVideoPalEmu[6], "%s", langEnumVideoEmuScale2x());
        sprintf(pVideoPalEmu[7], "%s", langEnumVideoEmuHq2x());

        sprintf(pVideoMonSize[0], "%s", langEnumVideoSize1x());
        sprintf(pVideoMonSize[1], "%s", langEnumVideoSize2x());
        sprintf(pVideoMonSize[2], "%s", langEnumVideoSizeFullscreen());

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_MONGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropMonMonGB());
        SendDlgItemMessage(hDlg, IDC_MONTYPETEXT, WM_SETTEXT, 0, (LPARAM)langPropMonTypeText());
        SendDlgItemMessage(hDlg, IDC_MONEMUTEXT, WM_SETTEXT, 0, (LPARAM)langPropMonEmuText());
        SendDlgItemMessage(hDlg, IDC_MONVIDEOTYPETEXT, WM_SETTEXT, 0, (LPARAM)langPropVideoTypeText());
        SendDlgItemMessage(hDlg, IDC_MONWINDOWSIZETEXT, WM_SETTEXT, 0, (LPARAM)langPropWindowSizeText());
        SendDlgItemMessage(hDlg, IDC_MONHORIZSTRETCH, WM_SETTEXT, 0, (LPARAM)langPropMonHorizStretch());
        SendDlgItemMessage(hDlg, IDC_MONVERTSTRETCH, WM_SETTEXT, 0, (LPARAM)langPropMonVertStretch());
        SendDlgItemMessage(hDlg, IDC_MONDEINTERLACE, WM_SETTEXT, 0, (LPARAM)langPropMonDeInterlace());
        SendDlgItemMessage(hDlg, IDC_MONBLENDFRAMES, WM_SETTEXT, 0, (LPARAM)langPropMonBlendFrames());
        SendDlgItemMessage(hDlg, IDC_EFFECTSGB, WM_SETTEXT, 0, (LPARAM)langPropMonEffectsGB());

        setButtonCheck(hDlg, IDC_MONHORIZSTRETCH, pProperties->video.horizontalStretch, 1);
        setButtonCheck(hDlg, IDC_MONVERTSTRETCH, pProperties->video.verticalStretch, 1);
        setButtonCheck(hDlg, IDC_MONDEINTERLACE, pProperties->video.deInterlace, 1);
        setButtonCheck(hDlg, IDC_MONBLENDFRAMES, pProperties->video.blendFrames, 1);
        
        /* Init dropdown lists */
        initDropList(hDlg, IDC_MONTYPE, pVideoMon, pProperties->video.monitorColor);
        initDropList(hDlg, IDC_PALEMU, pVideoPalEmu, pProperties->video.monitorType);
        initDropList(hDlg, IDC_MONSIZE, pVideoMonSize, pProperties->video.windowSize);

        monitorType             = pProperties->video.monitorType;
        monitorColor            = pProperties->video.monitorColor;

        oldScanlinesEnable     = pProperties->video.scanlinesEnable;
        oldScanlinesPct        = pProperties->video.scanlinesPct;
        oldColorGhostingEnable = pProperties->video.colorSaturationEnable;
        oldColorGhostingWidth  = pProperties->video.colorSaturationWidth;
        oldHoriz               = pProperties->video.horizontalStretch;
        oldVert                = pProperties->video.verticalStretch;
        oldDeinterlace         = pProperties->video.deInterlace;

        SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSTEXT), WM_SETTEXT, 0, (LPARAM)langPropMonBrightness());
        SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTTEXT), WM_SETTEXT, 0, (LPARAM)langPropMonContrast());
        SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONTEXT), WM_SETTEXT, 0, (LPARAM)langPropMonSaturation());
        SendMessage(GetDlgItem(hDlg, IDC_MONGAMMATEXT), WM_SETTEXT, 0, (LPARAM)langPropMonGamma());
        SendMessage(GetDlgItem(hDlg, IDC_SCANLINESENABLE), WM_SETTEXT, 0, (LPARAM)langPropMonScanlines());
        SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGENABLE), WM_SETTEXT, 0, (LPARAM)langPropMonColorGhosting());
        

        contrast   = pProperties->video.contrast;
        brightness = pProperties->video.brightness;
        saturation = pProperties->video.saturation;
        gamma      = pProperties->video.gamma;

        setButtonCheck(hDlg, IDC_SCANLINESENABLE, pProperties->video.scanlinesEnable, 1);
        setButtonCheck(hDlg, IDC_COLORGHOSTINGENABLE, pProperties->video.colorSaturationEnable, 1);

        EnableWindow(GetDlgItem(hDlg, IDC_SCANLINESSLIDEBAR), oldScanlinesEnable);
        EnableWindow(GetDlgItem(hDlg, IDC_SCANLINESVALUE), oldScanlinesEnable);
        
        EnableWindow(GetDlgItem(hDlg, IDC_COLORGHOSTINGSLIDEBAR), oldColorGhostingEnable);
        EnableWindow(GetDlgItem(hDlg, IDC_COLORGHOSTINGVALUE), oldColorGhostingEnable);

        SendMessage(GetDlgItem(hDlg, IDC_SCANLINESSLIDEBAR), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 100));
        SendMessage(GetDlgItem(hDlg, IDC_SCANLINESSLIDEBAR), TBM_SETPOS,   1, (LPARAM)(100 - oldScanlinesPct));
        SendMessage(GetDlgItem(hDlg, IDC_SCANLINESVALUE), WM_SETTEXT, 0, (LPARAM)strPct(100 - oldScanlinesPct));

        SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGSLIDEBAR), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 4));
        SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGSLIDEBAR), TBM_SETPOS,   1, (LPARAM)oldColorGhostingWidth);
        SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGVALUE), WM_SETTEXT, 0, (LPARAM)strPt(oldColorGhostingWidth));

        SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONSLIDE), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 200));
        SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONSLIDE), TBM_SETPOS,   1, (LPARAM)saturation);
        SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONVALUE), WM_SETTEXT, 0, (LPARAM)strDec(saturation));

        SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSSLIDE), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 200));
        SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSSLIDE), TBM_SETPOS,   1, (LPARAM)brightness);
        SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSVALUE), WM_SETTEXT, 0, (LPARAM)strDec(brightness));

        SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTSLIDE), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 200));
        SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTSLIDE), TBM_SETPOS,   1, (LPARAM)contrast);
        SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTVALUE), WM_SETTEXT, 0, (LPARAM)strDec(contrast));

        SendMessage(GetDlgItem(hDlg, IDC_MONGAMMASLIDE), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 200));
        SendMessage(GetDlgItem(hDlg, IDC_MONGAMMASLIDE), TBM_SETPOS,   1, (LPARAM)gamma);
        SendMessage(GetDlgItem(hDlg, IDC_MONGAMMAVALUE), WM_SETTEXT, 0, (LPARAM)strDec(gamma));

        EnableWindow(GetDlgItem(hDlg, IDC_MONSATURATIONSLIDE), monitorColor == P_VIDEO_COLOR);
        EnableWindow(GetDlgItem(hDlg, IDC_MONSATURATIONVALUE), monitorColor == P_VIDEO_COLOR);
        EnableWindow(GetDlgItem(hDlg, IDC_MONSATURATIONTEXT), monitorColor == P_VIDEO_COLOR);

        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_SCANLINESENABLE:
            pProperties->video.scanlinesEnable = getButtonCheck(hDlg, IDC_SCANLINESENABLE);
            EnableWindow(GetDlgItem(hDlg, IDC_SCANLINESSLIDEBAR), pProperties->video.scanlinesEnable);
            EnableWindow(GetDlgItem(hDlg, IDC_SCANLINESVALUE), pProperties->video.scanlinesEnable);
            
            videoSetScanLines(theVideo, pProperties->video.scanlinesEnable, pProperties->video.scanlinesPct);
            updateEmuWindow();
            break;

        case IDC_COLORGHOSTINGENABLE:
            pProperties->video.colorSaturationEnable = getButtonCheck(hDlg, IDC_COLORGHOSTINGENABLE);
            EnableWindow(GetDlgItem(hDlg, IDC_COLORGHOSTINGSLIDEBAR), pProperties->video.colorSaturationEnable);
            EnableWindow(GetDlgItem(hDlg, IDC_COLORGHOSTINGVALUE), pProperties->video.colorSaturationEnable);



            videoSetColorSaturation(theVideo, pProperties->video.colorSaturationEnable, pProperties->video.colorSaturationWidth);
            updateEmuWindow();
            break;

        case IDC_PALEMU:
            monitorType = getDropListIndex(hDlg, IDC_PALEMU, pVideoPalEmu);
            videoSetPalMode(theVideo, monitorType);
            updateEmuWindow();
            break;

        case IDC_MONTYPE:
            monitorColor = getDropListIndex(hDlg, IDC_MONTYPE, pVideoMon);
            switch (monitorColor) {
            case P_VIDEO_COLOR:
                videoSetColorMode(theVideo, VIDEO_COLOR);
                break;
            case P_VIDEO_BW:
                videoSetColorMode(theVideo, VIDEO_BLACKWHITE);
                break;
            case P_VIDEO_GREEN:
                videoSetColorMode(theVideo, VIDEO_GREEN);
                break;
            case P_VIDEO_AMBER:
                videoSetColorMode(theVideo, VIDEO_AMBER);
                break;
            }
            EnableWindow(GetDlgItem(hDlg, IDC_MONSATURATIONSLIDE), monitorColor == P_VIDEO_COLOR);
            EnableWindow(GetDlgItem(hDlg, IDC_MONSATURATIONVALUE), monitorColor == P_VIDEO_COLOR);
            EnableWindow(GetDlgItem(hDlg, IDC_MONSATURATIONTEXT), monitorColor == P_VIDEO_COLOR);
            updateEmuWindow();
            break;

        case IDC_MONHORIZSTRETCH:
            pProperties->video.horizontalStretch = getButtonCheck(hDlg, IDC_MONHORIZSTRETCH);
            updateEmuWindow();
            break;

        case IDC_MONVERTSTRETCH:
            pProperties->video.verticalStretch   = getButtonCheck(hDlg, IDC_MONVERTSTRETCH);
            updateEmuWindow();
            break;
        case IDC_MONDEINTERLACE:
            pProperties->video.deInterlace   = getButtonCheck(hDlg, IDC_MONDEINTERLACE);
            videoSetDeInterlace(theVideo, pProperties->video.deInterlace);
            updateEmuWindow();
            break;
        case IDC_MONBLENDFRAMES:
            pProperties->video.blendFrames   = getButtonCheck(hDlg, IDC_MONBLENDFRAMES);
            videoSetBlendFrames(theVideo, pProperties->video.blendFrames);
            updateEmuWindow();
            break;
        }
        break;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        switch (wParam) {
        case IDC_SCANLINESSLIDEBAR:
            pProperties->video.scanlinesPct = 100 - SendMessage(GetDlgItem(hDlg, IDC_SCANLINESSLIDEBAR), TBM_GETPOS, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_SCANLINESVALUE), WM_SETTEXT, 0, (LPARAM)strPct(100 - pProperties->video.scanlinesPct));

            videoSetScanLines(theVideo, pProperties->video.scanlinesEnable, pProperties->video.scanlinesPct);

            updateEmuWindow();
            return 0;
            
        case IDC_COLORGHOSTINGSLIDEBAR:
            pProperties->video.colorSaturationWidth = SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGSLIDEBAR), TBM_GETPOS, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGVALUE), WM_SETTEXT, 0, (LPARAM)strPt(pProperties->video.colorSaturationWidth));

            videoSetColorSaturation(theVideo, pProperties->video.colorSaturationEnable, pProperties->video.colorSaturationWidth);
            updateEmuWindow();
            return 0;

        case IDC_MONSATURATIONSLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONSLIDE), TBM_GETPOS, 0, 0);
            if (value != saturation) {
                saturation = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONVALUE), WM_SETTEXT, 0, (LPARAM)strDec(saturation));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            return 0;

        case IDC_MONBRIGHTNESSSLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSSLIDE), TBM_GETPOS, 0, 0);
            if (value != brightness) {
                brightness = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSVALUE), WM_SETTEXT, 0, (LPARAM)strDec(brightness));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            return 0;

        case IDC_MONCONTRASTSLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTSLIDE), TBM_GETPOS, 0, 0);
            if (value != contrast) {
                contrast = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTVALUE), WM_SETTEXT, 0, (LPARAM)strDec(contrast));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }

        case IDC_MONGAMMASLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONGAMMASLIDE), TBM_GETPOS, 0, 0);
            if (value != gamma) {
                gamma = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONGAMMAVALUE), WM_SETTEXT, 0, (LPARAM)strDec(gamma));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            return 0;
        }

        if ((((NMHDR FAR *)lParam)->code) == PSN_QUERYCANCEL) {
            videoSetScanLines(theVideo, pProperties->video.scanlinesEnable, pProperties->video.scanlinesPct);

            videoSetColorSaturation(theVideo, pProperties->video.colorSaturationEnable, pProperties->video.colorSaturationWidth);
            videoSetPalMode(theVideo, pProperties->video.monitorType);
            videoSetColors(theVideo, pProperties->video.saturation, pProperties->video.brightness, 
                           pProperties->video.contrast, pProperties->video.gamma);
            switch (pProperties->video.monitorColor) {
            case P_VIDEO_COLOR:
                videoSetColorMode(theVideo, VIDEO_COLOR);
                break;
            case P_VIDEO_BW:
                videoSetColorMode(theVideo, VIDEO_BLACKWHITE);
                break;
            case P_VIDEO_GREEN:
                videoSetColorMode(theVideo, VIDEO_GREEN);
                break;
            case P_VIDEO_AMBER:
                videoSetColorMode(theVideo, VIDEO_AMBER);
                break;
            }
            pProperties->video.horizontalStretch     = oldHoriz;
            pProperties->video.verticalStretch       = oldVert;
            pProperties->video.deInterlace           = oldDeinterlace;
            pProperties->video.scanlinesEnable       = oldScanlinesEnable;
            pProperties->video.scanlinesPct          = oldScanlinesPct;
            pProperties->video.colorSaturationEnable = oldColorGhostingEnable;
            pProperties->video.colorSaturationWidth  = oldColorGhostingWidth;
            updateEmuWindow();
            return FALSE;
        }
        else if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }

        pProperties->video.monitorColor      = monitorColor;
        pProperties->video.monitorType       = monitorType;
        pProperties->video.contrast          = contrast;
        pProperties->video.brightness        = brightness;
        pProperties->video.saturation        = saturation;
        pProperties->video.gamma             = gamma;
        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

static void getMidiList(HWND hDlg, int id, Properties* pProperties) {
    char buffer[256];
    int idx = SendDlgItemMessage(hDlg, id, CB_GETCURSEL, 0, 0);
    int rv = SendDlgItemMessage(hDlg, id, CB_GETLBTEXT, idx, (LPARAM)buffer);
    int*  midiType;
    char* drvName;
    char* drvDesc;
    int   noSupportFile;

    switch (id) {
    default:
    case IDC_MIDIOUT: 
        midiType = &pProperties->sound.MidiOut.type;
        drvName  = pProperties->sound.MidiOut.name;
        drvDesc  = pProperties->sound.MidiOut.desc;
        noSupportFile = 1;
        break;
    case IDC_MIDIIN: 
        midiType = &pProperties->sound.MidiIn.type;
        drvName  = pProperties->sound.MidiIn.name;
        drvDesc  = pProperties->sound.MidiIn.desc;
        noSupportFile = 1;
        break;
    case IDC_YKIN: 
        midiType = &pProperties->sound.YkIn.type;
        drvName  = pProperties->sound.YkIn.name;
        drvDesc  = pProperties->sound.YkIn.desc;
        noSupportFile = 1;
        break;
    }

    if (idx + noSupportFile < P_MIDI_HOST) {
        *midiType = idx;
    }
    else {
        char* name = buffer;
        // Find the printer name from string
        while (*name && (*name != '-' || name[1] != ' ')) {
            name++;
        }
        
        strcpy(drvName, buffer);
        drvName[name - buffer - 1] = 0;

        if (*name) name++;
        if (*name) name++;

        *midiType = P_COM_HOST;
        strcpy(drvDesc, name);
    }
}

static void updateMidiList(HWND hDlg, int id, Properties* pProperties)
{
    int   midiType;
    char* drvName;
    char* drvDesc;
    int   devNum;
    int   noSupportFile;
    int   i;

    switch (id) {
    default:
    case IDC_MIDIOUT: 
        midiType = pProperties->sound.MidiOut.type;
        drvName  = pProperties->sound.MidiOut.name;
        drvDesc  = pProperties->sound.MidiOut.desc;
        noSupportFile = 1;
        break;
    case IDC_MIDIIN: 
        midiType = pProperties->sound.MidiIn.type;
        drvName  = pProperties->sound.MidiIn.name;
        drvDesc  = pProperties->sound.MidiIn.desc;
        noSupportFile = 1;
        break;
    case IDC_YKIN: 
        midiType = pProperties->sound.YkIn.type;
        drvName  = pProperties->sound.YkIn.name;
        drvDesc  = pProperties->sound.YkIn.desc;
        noSupportFile = 1;
        break;
    }

    while (CB_ERR != SendDlgItemMessage(hDlg, id, CB_DELETESTRING, 0, 0));

    // Add NONE:
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langTextNone());
    SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 0, 0); // Set as default

    // Add FILE
    if (!noSupportFile) {
        SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langTextFile());
        if (midiType == P_MIDI_FILE) {
            SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 1, 0);
        }
    }

    devNum = id == IDC_MIDIOUT ? midiOutGetDeviceCount() : midiInGetDeviceCount();
    for (i = 0; i < devNum; i++) {
        char buf[512];
        const char* name = id == IDC_MIDIOUT ? midiOutGetDeviceIdString(i) : midiInGetDeviceIdString(i);
        const char* desc = id == IDC_MIDIOUT ? midiOutGetDeviceName(i)     : midiInGetDeviceName(i);

        sprintf(buf, "%s - %s", name, desc);

        SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)buf);
        if (midiType == P_MIDI_HOST && 0 == strcmp(drvName, name)) {
            SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 2 - noSupportFile + i, 0);
        }
    }
}

static void getMidiChannelList(HWND hDlg, int id, Properties* pProperties) 
{
    int idx = SendDlgItemMessage(hDlg, id, CB_GETCURSEL, 0, 0);
    pProperties->sound.YkIn.channel = idx;
}

static void updateMidiChannelList(HWND hDlg, int id, Properties* pProperties)
{
    int i;

    while (CB_ERR != SendDlgItemMessage(hDlg, id, CB_DELETESTRING, 0, 0));

    // Add ALL:
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropSndMidiAll());
    SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 0, 0); // Set as default

    for (i = 1; i <= 16; i++) {
        char buf[32];
        sprintf(buf, "%d", i);

        SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)buf);
        if (i == pProperties->sound.YkIn.channel) {
            SendDlgItemMessage(hDlg, id, CB_SETCURSEL, i, 0);
        }
    }
}



static BOOL CALLBACK soundDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;
    int index;

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        hDlgSound = hDlg;

        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;

        SendMessage(GetDlgItem(hDlg, IDC_SNDCHIPEMUGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropSndChipEmuGB());

        SendDlgItemMessage(hDlg, IDC_OVERSAMPLETEXT1, WM_SETTEXT, 0, (LPARAM)langPropSndOversampleText());
        SendDlgItemMessage(hDlg, IDC_OVERSAMPLETEXT2, WM_SETTEXT, 0, (LPARAM)langPropSndOversampleText());
        SendDlgItemMessage(hDlg, IDC_OVERSAMPLETEXT3, WM_SETTEXT, 0, (LPARAM)langPropSndOversampleText());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMSXMUSIC), langPropSndMsxMusic());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMSXAUDIO), langPropSndMsxAudio());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMOONSOUND), langPropSndMoonsound());

        SendDlgItemMessage(hDlg, IDC_YKINGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSndYkInGB());
        SendDlgItemMessage(hDlg, IDC_YKINTEXT, WM_SETTEXT, 0, (LPARAM)langTextDevice());
        SendDlgItemMessage(hDlg, IDC_YKINCHANTEXT, WM_SETTEXT, 0, (LPARAM)langPropSndMidiChannel());
        SendDlgItemMessage(hDlg, IDC_MIDIINGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSndMidiInGB());
        SendDlgItemMessage(hDlg, IDC_MIDIINTEXT, WM_SETTEXT, 0, (LPARAM)langTextDevice());
#if 0
        SendDlgItemMessage(hDlg, IDI_MIDIINFILENAMETEXT, WM_SETTEXT, 0, (LPARAM)langTextFilename());
#endif
        SendDlgItemMessage(hDlg, IDC_MIDIOUTGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSndMidiOutGB());
        SendDlgItemMessage(hDlg, IDC_MIDIOUTTEXT, WM_SETTEXT, 0, (LPARAM)langTextDevice());
#if 0
        SendDlgItemMessage(hDlg, IDI_MIDIOUTFILENAMETEXT, WM_SETTEXT, 0, (LPARAM)langTextFilename());
#endif
        SetWindowText(GetDlgItem(hDlg, IDC_MIDIOUTMT32TOGM), langPropSndMt32ToGm());

        setButtonCheck(hDlg, IDC_ENABLEMSXMUSIC, pProperties->sound.chip.enableYM2413, 1);
        setButtonCheck(hDlg, IDC_ENABLEMSXAUDIO, pProperties->sound.chip.enableY8950, 1);
        setButtonCheck(hDlg, IDC_ENABLEMOONSOUND, pProperties->sound.chip.enableMoonsound, 1);

        {
            int index = 0;
            int oversampling = 1;
            for (; index < 4; index++, oversampling <<= 1) {
                const char* modes[] = { "1 x", "2 x", "4 x", "8 x"} ;
                SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXMUSIC, CB_ADDSTRING, 0, (LPARAM)modes[index]);
                if (oversampling <= pProperties->sound.chip.ym2413Oversampling) {
                    SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXMUSIC, CB_SETCURSEL, index, 0);
                }
                
                SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXAUDIO, CB_ADDSTRING, 0, (LPARAM)modes[index]);
                if (oversampling <= pProperties->sound.chip.y8950Oversampling) {
                    SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXAUDIO, CB_SETCURSEL, index, 0);
                }
                
                SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMOONSOUND, CB_ADDSTRING, 0, (LPARAM)modes[index]);
                if (oversampling <= pProperties->sound.chip.moonsoundOversampling) {
                    SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMOONSOUND, CB_SETCURSEL, index, 0);
                }
            }
        }

        updateMidiList(hDlg, IDC_MIDIOUT, pProperties);
        updateMidiList(hDlg, IDC_MIDIIN,  pProperties);
        updateMidiList(hDlg, IDC_YKIN,  pProperties);
        updateMidiChannelList(hDlg, IDC_YKINCHAN, pProperties);

        setButtonCheck(hDlg, IDC_MIDIOUTMT32TOGM, pProperties->sound.MidiOut.mt32ToGm, 1);

        {
            int idx = SendDlgItemMessage(hDlg, IDC_MIDIOUT, CB_GETCURSEL, 0, 0) + 1;
#if 0
            EnableWindow(GetDlgItem(hDlg, IDC_MIDIOUTFILENAMEBROWSE), idx == P_MIDI_FILE);
            EnableWindow(GetDlgItem(hDlg, IDI_MIDIOUTFILENAME), idx == P_MIDI_FILE);
#endif
            EnableWindow(GetDlgItem(hDlg, IDC_MIDIOUTMT32TOGM), idx >= P_MIDI_HOST);

            idx = SendDlgItemMessage(hDlg, IDC_MIDIIN, CB_GETCURSEL, 0, 0);
#if 0
            EnableWindow(GetDlgItem(hDlg, IDC_MIDIINFILENAMEBROWSE), idx == P_MIDI_FILE);
            EnableWindow(GetDlgItem(hDlg, IDI_MIDIINFILENAME), idx == P_MIDI_FILE);
#endif

            idx = SendDlgItemMessage(hDlg, IDC_YKIN, CB_GETCURSEL, 0, 0) + 1;
            EnableWindow(GetDlgItem(hDlg, IDC_YKINCHANTEXT), idx >= P_MIDI_HOST);
            EnableWindow(GetDlgItem(hDlg, IDC_YKINCHAN), idx >= P_MIDI_HOST);
        }

#if 0
        SetWindowText(GetDlgItem(hDlg, IDI_MIDIOUTFILENAME), pProperties->sound.MidiOut.fileName);
        SetWindowText(GetDlgItem(hDlg, IDI_MIDIINFILENAME),  pProperties->sound.MidiIn.fileName);
#endif
        return FALSE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_MIDIOUT:
            {
                int idx = SendDlgItemMessage(hDlg, IDC_MIDIOUT, CB_GETCURSEL, 0, 0) + 1;
#if 0
                EnableWindow(GetDlgItem(hDlg, IDC_MIDIOUTFILENAMEBROWSE), idx == P_MIDI_FILE);
                EnableWindow(GetDlgItem(hDlg, IDI_MIDIOUTFILENAME), idx == P_MIDI_FILE);
#endif
                EnableWindow(GetDlgItem(hDlg, IDC_MIDIOUTMT32TOGM), idx >= P_MIDI_HOST);
            }
            return TRUE;
        case IDC_MIDIIN:
            {
                int idx = SendDlgItemMessage(hDlg, IDC_MIDIIN, CB_GETCURSEL, 0, 0) + 1;
#if 0
                EnableWindow(GetDlgItem(hDlg, IDC_MIDIINFILENAMEBROWSE), idx == P_MIDI_FILE);
                EnableWindow(GetDlgItem(hDlg, IDI_MIDIINFILENAME), idx == P_MIDI_FILE);
#endif
            }
            return TRUE;
        case IDC_YKIN:
            {
                int idx = SendDlgItemMessage(hDlg, IDC_YKIN, CB_GETCURSEL, 0, 0) + 1;
                EnableWindow(GetDlgItem(hDlg, IDC_YKINCHANTEXT), idx >= P_MIDI_HOST);
                EnableWindow(GetDlgItem(hDlg, IDC_YKINCHAN), idx >= P_MIDI_HOST);
            }
            return TRUE;
#if 0
        case IDC_MIDIOUTFILENAMEBROWSE:
            {
                char fileName[MAX_PATH];
                GetWindowText(GetDlgItem(hDlg, IDI_MIDIOUTFILENAME), fileName, MAX_PATH - 1);
                if (openLogFile(hDlg, fileName)) {
                    SetWindowText(GetDlgItem(hDlg, IDI_MIDIOUTFILENAME), fileName);
                }
            }
            return TRUE;
        case IDC_MIDIINFILENAMEBROWSE:
            {
                char fileName[MAX_PATH];
                GetWindowText(GetDlgItem(hDlg, IDI_MIDIINFILENAME), fileName, MAX_PATH - 1);
                if (openLogFile(hDlg, pProperties->sound.MidiIn.fileName)) {
                    SetWindowText(GetDlgItem(hDlg, IDI_MIDIINFILENAME), fileName);
                }
            }
            return TRUE;
#endif
        case IDC_OVERSAMPLEMSXMUSIC:
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXMUSIC, CB_GETCURSEL, 0, 0);
            boardSetYm2413Oversampling(1 << index);
            break;
        case IDC_OVERSAMPLEMSXAUDIO:
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXAUDIO, CB_GETCURSEL, 0, 0);
            boardSetY8950Oversampling(1 << index);
            break;
        case IDC_OVERSAMPLEMOONSOUND:
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMOONSOUND, CB_GETCURSEL, 0, 0);
            boardSetMoonsoundOversampling(1 << index);
            break;
        }
        break;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        switch (((NMHDR FAR *)lParam)->code) {
        case PSN_APPLY:
            pProperties->sound.chip.enableYM2413 = getButtonCheck(hDlg, IDC_ENABLEMSXMUSIC);
            pProperties->sound.chip.enableY8950 = getButtonCheck(hDlg, IDC_ENABLEMSXAUDIO);
            pProperties->sound.chip.enableMoonsound = getButtonCheck(hDlg, IDC_ENABLEMOONSOUND);
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXMUSIC, CB_GETCURSEL, 0, 0);
            pProperties->sound.chip.ym2413Oversampling = 1 << index;
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXAUDIO, CB_GETCURSEL, 0, 0);
            pProperties->sound.chip.y8950Oversampling = 1 << index;
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMOONSOUND, CB_GETCURSEL, 0, 0);
            pProperties->sound.chip.moonsoundOversampling = 1 << index;

            getMidiList(hDlg, IDC_MIDIOUT, pProperties);
#if 0
            GetWindowText(GetDlgItem(hDlg, IDI_MIDIOUTFILENAME), pProperties->sound.MidiOut.fileName, MAX_PATH - 1);
#endif
            getMidiList(hDlg, IDC_MIDIIN, pProperties);
#if 0
            GetWindowText(GetDlgItem(hDlg, IDI_MIDIINFILENAME), pProperties->sound.MidiIn.fileName, MAX_PATH - 1);
#endif
            pProperties->sound.MidiOut.mt32ToGm = getButtonCheck(hDlg, IDC_MIDIOUTMT32TOGM);

            getMidiList(hDlg, IDC_YKIN, pProperties);
            getMidiChannelList(hDlg, IDC_YKINCHAN, pProperties);

            propModified = 1;
            return TRUE;

        case PSN_QUERYCANCEL:
            emulatorRestartSound();

            boardSetYm2413Oversampling(pProperties->sound.chip.ym2413Oversampling);
            boardSetY8950Oversampling(pProperties->sound.chip.y8950Oversampling);
            boardSetMoonsoundOversampling(pProperties->sound.chip.moonsoundOversampling);

            return FALSE;
        }
        break;
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


static void getPortsLptList(HWND hDlg, int id, Properties* pProperties) {
    char buffer[256];
    int idx = SendDlgItemMessage(hDlg, id, CB_GETCURSEL, 0, 0);
    int rv = SendDlgItemMessage(hDlg, id, CB_GETLBTEXT, idx, (LPARAM)buffer);

    if (idx < P_LPT_HOST) {
        pProperties->ports.Lpt.type = idx;
    }
    else {
        char* prnName = buffer;
        // Find the printer name from string
        while (*prnName && *prnName != '-') {
            prnName++;
        }
        
        strcpy(pProperties->ports.Lpt.portName, buffer);
        pProperties->ports.Lpt.portName[prnName - buffer - 2] = 0;

        if (*prnName) prnName++;
        if (*prnName) prnName++;

        pProperties->ports.Lpt.type = P_LPT_HOST;
        strcpy(pProperties->ports.Lpt.name, prnName);
    }
}

static void getPortsLptEmulList(HWND hDlg, int id, Properties* pProperties) 
{
    int idx = SendDlgItemMessage(hDlg, id, CB_GETCURSEL, 0, 0);

    if (idx >= 0) {
        pProperties->ports.Lpt.emulation = idx;
    }
}

static BOOL updatePortsLptEmulList(HWND hDlg, int id, Properties* pProperties)
{
    while (CB_ERR != SendDlgItemMessage(hDlg, id, CB_DELETESTRING, 0, 0));

    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropPortsNone());
    
    // Add MSX Printer
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)"MSX Printer");
    
    // Add Epson FX-80
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)"Epson FX-80");

    SendDlgItemMessage(hDlg, id, CB_SETCURSEL, pProperties->ports.Lpt.emulation, 0);

    return TRUE;
}

static BOOL updatePortsLptList(HWND hDlg, int id, Properties* pProperties)
{
    LPPRINTER_INFO_2 lpPrinterInfo = NULL;
    TCHAR sBuf[MAX_PATH];
    DWORD dwNeeded;
    DWORD dwReturned;
    DWORD dwItem;

    while (CB_ERR != SendDlgItemMessage(hDlg, id, CB_DELETESTRING, 0, 0));

    // Get buffer size
    EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &dwNeeded, &dwReturned);

    // Allocate memory
    lpPrinterInfo = (LPPRINTER_INFO_2)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeeded);
    if (lpPrinterInfo == NULL)
        return FALSE;

    if (!EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE)lpPrinterInfo, dwNeeded, &dwNeeded, &dwReturned))
        return FALSE;

    // Add NONE:
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropPortsNone());
    SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 0, 0); // Set as default

    // Add SiMPL/COVOX
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropPortsSimplCovox());
    if (pProperties->ports.Lpt.type == P_LPT_SIMPL) 
        SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 1, 0);

    // Add FILE
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropPortsFile());
    if (pProperties->ports.Lpt.type == P_LPT_FILE) 
        SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 2, 0);

    // Add printers 
    for (dwItem = 0; dwItem < dwReturned; dwItem++) {
        if SUCCEEDED(StringCchPrintf(sBuf, MAX_PATH-1, "%s - %s", lpPrinterInfo[dwItem].pPortName, lpPrinterInfo[dwItem].pPrinterName)) {
            SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)sBuf);
            if (pProperties->ports.Lpt.type == P_LPT_HOST && 0 == strcmp(pProperties->ports.Lpt.name, lpPrinterInfo[dwItem].pPrinterName)) 
                SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 3 + dwItem, 0);
        }
    }

    // Free memory
    HeapFree(GetProcessHeap(), 0, lpPrinterInfo);

    return TRUE;
}

static BOOL IsNumeric(LPCTSTR pszString, BOOL bIgnoreColon)
{
    BOOL bNumeric = TRUE;
    size_t cch;
    unsigned int i;

    if (!SUCCEEDED(StringCchLength(pszString, MAX_PATH-1, &cch)))
        return FALSE;
    if (cch == 0)
        return FALSE;

    for (i=0; i<cch && bNumeric; i++) {
        bNumeric = (isdigit(pszString[i]) != 0);
        if (bIgnoreColon && (pszString[i] == ':'))
            bNumeric = TRUE;
    }
    return bNumeric;
}

static void getPortsComList(HWND hDlg, int id, Properties* pProperties) {
    char buffer[256];
    int idx = SendDlgItemMessage(hDlg, id, CB_GETCURSEL, 0, 0);
    int rv = SendDlgItemMessage(hDlg, id, CB_GETLBTEXT, idx, (LPARAM)buffer);

    if (idx < P_COM_HOST) {
        pProperties->ports.Com.type = idx;
    }
    else {
        char* portName = buffer;
        // Find the printer name from string
        while (*portName && *portName != '-') {
            portName++;
        }
        
        strcpy(pProperties->ports.Com.portName, buffer);
        pProperties->ports.Com.portName[portName - buffer - 2] = 0;

        if (*portName) portName++;
        if (*portName) portName++;

        pProperties->ports.Com.type = P_COM_HOST;
        strcpy(pProperties->ports.Com.name, portName);
    }
}

static BOOL updatePortsComList(HWND hDlg, int id, Properties* pProperties)
{
    PORT_INFO_2 *lpPortInfo = NULL;
    TCHAR sBuf[MAX_PATH];
    DWORD dwNeeded;
    DWORD dwReturned;
    DWORD dwItem;

    while (CB_ERR != SendDlgItemMessage(hDlg, id, CB_DELETESTRING, 0, 0));

    // Get buffer size
    EnumPorts(NULL, 2, NULL, 0, &dwNeeded, &dwReturned);

    // Allocate memory
    lpPortInfo = (PORT_INFO_2*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeeded);
    if (lpPortInfo == NULL)
        return FALSE;

    if (!EnumPorts(NULL, 2, (LPBYTE)lpPortInfo, dwNeeded, &dwNeeded, &dwReturned))
        return FALSE;

    // Add NONE:
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropPortsNone());
    SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 0, 0); // Set as default

    // Add FILE
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)langPropPortsComFile());
    if (pProperties->ports.Com.type == P_COM_FILE) 
        SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 1, 0);


    // Add COM ports 
    for (dwItem = 0; dwItem < dwReturned; dwItem++) {
        size_t cch;
        if SUCCEEDED(StringCchLength(lpPortInfo[dwItem].pPortName, MAX_PATH-1, &cch))
            if (cch > 3)
                if ((strncmp(lpPortInfo[dwItem].pPortName, "COM", 3) == 0) && IsNumeric(&lpPortInfo[dwItem].pPortName[3], TRUE))
                    if SUCCEEDED(StringCchPrintf(sBuf, MAX_PATH-1, "%s - %s", lpPortInfo[dwItem].pPortName, lpPortInfo[dwItem].pDescription)) {
                        SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)sBuf);
                        if (pProperties->ports.Com.type == P_COM_HOST && 0 == strcmp(pProperties->ports.Com.name, lpPortInfo[dwItem].pPortName)) 
                            SendDlgItemMessage(hDlg, id, CB_SETCURSEL, 2 + dwItem, 0);
                    }
    }

    // Free memory
    HeapFree(GetProcessHeap(), 0, lpPortInfo);

    return TRUE;
}


static BOOL CALLBACK portsDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }

        SendMessage(GetDlgItem(hDlg, IDC_PORTSLPTGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropPortsLptGB());
        SendMessage(GetDlgItem(hDlg, IDC_PORTSCOMGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropPortsComGB());
        SendMessage(GetDlgItem(hDlg, IDC_PORTSLPTTEXT), WM_SETTEXT, 0, (LPARAM)langPropPortsLptText());
        SendMessage(GetDlgItem(hDlg, IDC_PORTSCOM1TEXT), WM_SETTEXT, 0, (LPARAM)langPropPortsCom1Text());
        SetWindowText(GetDlgItem(GetParent(hDlg), IDOK), langDlgOK());
        SetWindowText(GetDlgItem(GetParent(hDlg), IDCANCEL), langDlgCancel());
        SetWindowText(GetDlgItem(hDlg, IDC_LPTFILENAMETEXT), langTextFilename());
        SetWindowText(GetDlgItem(hDlg, IDC_COM1FILENAMETEXT), langTextFilename());
        SetWindowText(GetDlgItem(hDlg, IDC_LPTEMULATIONTEXT), langPropPortsEmulateMsxPrn());
        

        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;

        updatePortsLptList(hDlg, IDC_PORTSLPT, pProperties);
        updatePortsComList(hDlg, IDC_PORTSCOM1, pProperties);

        updatePortsLptEmulList(hDlg, IDC_LPTEMULATION, pProperties);

        {
            int idx = SendDlgItemMessage(hDlg, IDC_PORTSLPT, CB_GETCURSEL, 0, 0);
            EnableWindow(GetDlgItem(hDlg, IDC_LPTFILENAMEBROWSE), idx == P_LPT_FILE);
            EnableWindow(GetDlgItem(hDlg, IDC_LPTFILENAME), idx == P_LPT_FILE);
            EnableWindow(GetDlgItem(hDlg, IDC_LPTEMULATION), idx >= P_LPT_HOST);

            idx = SendDlgItemMessage(hDlg, IDC_PORTSCOM1, CB_GETCURSEL, 0, 0);
            EnableWindow(GetDlgItem(hDlg, IDC_COM1FILENAMEBROWSE), idx == P_COM_FILE);
            EnableWindow(GetDlgItem(hDlg, IDC_COM1FILENAME), idx == P_COM_FILE);
        }

        SetWindowText(GetDlgItem(hDlg, IDC_LPTFILENAME), pProperties->ports.Lpt.fileName);
        SetWindowText(GetDlgItem(hDlg, IDC_COM1FILENAME), pProperties->ports.Com.fileName);

        return FALSE;
        
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_PORTSLPT:
            {
                int idx = SendDlgItemMessage(hDlg, IDC_PORTSLPT, CB_GETCURSEL, 0, 0);
                EnableWindow(GetDlgItem(hDlg, IDC_LPTFILENAMEBROWSE), idx == P_LPT_FILE);
                EnableWindow(GetDlgItem(hDlg, IDC_LPTFILENAME), idx == P_LPT_FILE);
                EnableWindow(GetDlgItem(hDlg, IDC_LPTEMULATION), idx >= P_LPT_HOST);
            }
            return TRUE;

        case IDC_PORTSCOM1:
            {
                int idx = SendDlgItemMessage(hDlg, IDC_PORTSCOM1, CB_GETCURSEL, 0, 0);
                EnableWindow(GetDlgItem(hDlg, IDC_COM1FILENAMEBROWSE), idx == P_COM_FILE);
                EnableWindow(GetDlgItem(hDlg, IDC_COM1FILENAME), idx == P_COM_FILE);
            }
            return TRUE;

        case IDC_LPTFILENAMEBROWSE:
            if (openLogFile(hDlg, pProperties->ports.Lpt.fileName)) {
                SetWindowText(GetDlgItem(hDlg, IDC_LPTFILENAME), pProperties->ports.Lpt.fileName);
            }
            return TRUE;

        case IDC_COM1FILENAMEBROWSE:
            if (openLogFile(hDlg, pProperties->ports.Com.fileName)) {
                SetWindowText(GetDlgItem(hDlg, IDC_COM1FILENAMEBROWSE), pProperties->ports.Com.fileName);
            }
            return TRUE;
        }
        return FALSE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }

        getPortsLptList(hDlg, IDC_PORTSLPT, pProperties);
        getPortsLptEmulList(hDlg, IDC_LPTEMULATION, pProperties);
        GetWindowText(GetDlgItem(hDlg, IDC_LPTFILENAME), pProperties->ports.Lpt.fileName, MAX_PATH - 1);

        getPortsComList(hDlg, IDC_PORTSCOM1, pProperties);
        GetWindowText(GetDlgItem(hDlg, IDC_COM1FILENAME), pProperties->ports.Com.fileName, MAX_PATH - 1);

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


int showProperties(Properties* pProperties, HWND hwndOwner, PropPage startPage, Mixer* mixer, Video* video) {
	HINSTANCE       hInst = (HINSTANCE)GetModuleHandle(NULL);
    PROPSHEETPAGE   psp[7];
    PROPSHEETHEADER psh;
    Properties oldProp = *pProperties;

    centered = 0;
    hDlgSound = NULL;
    theMixer = mixer;
    theVideo = video;

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[0].hInstance = hInst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_EMULATION);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = emulationDlgProc;
    psp[0].pszTitle = langPropEmulation();
    psp[0].lParam = (LPARAM)pProperties;
    psp[0].pfnCallback = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[1].hInstance = hInst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = videoDlgProc;
    psp[1].pszTitle = langPropVideo();
    psp[1].lParam = (LPARAM)pProperties;
    psp[1].pfnCallback = NULL;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[2].hInstance = hInst;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_SOUND);
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = soundDlgProc;
    psp[2].pszTitle = langPropSound();
    psp[2].lParam = (LPARAM)pProperties;
    psp[2].pfnCallback = NULL;

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[3].hInstance = hInst;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_PERFORMANCE);
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = performanceDlgProc;
    psp[3].pszTitle = langPropPerformance();
    psp[3].lParam = (LPARAM)pProperties;
    psp[3].pfnCallback = NULL;

    psp[4].dwSize = sizeof(PROPSHEETPAGE);
    psp[4].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[4].hInstance = hInst;
    psp[4].pszTemplate = MAKEINTRESOURCE(IDD_SETTINGS);
    psp[4].pszIcon = NULL;
    psp[4].pfnDlgProc = filesDlgProc;
    psp[4].pszTitle = langPropFile();
    psp[4].lParam = (LPARAM)pProperties;
    psp[4].pfnCallback = NULL;

    psp[5].dwSize = sizeof(PROPSHEETPAGE);
    psp[5].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[5].hInstance = hInst;
    psp[5].pszTemplate = MAKEINTRESOURCE(IDD_APEARANCE);
    psp[5].pszIcon = NULL;
    psp[5].pfnDlgProc = settingsDlgProc;
    psp[5].pszTitle = langPropSettings();
    psp[5].lParam = (LPARAM)pProperties;
    psp[5].pfnCallback = NULL;

    psp[6].dwSize = sizeof(PROPSHEETPAGE);
    psp[6].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[6].hInstance = hInst;
    psp[6].pszTemplate = MAKEINTRESOURCE(IDD_PORTS);
    psp[6].pszIcon = NULL;
    psp[6].pfnDlgProc = portsDlgProc;
    psp[6].pszTitle = langPropPorts();
    psp[6].lParam = (LPARAM)pProperties;
    psp[6].pfnCallback = NULL;
    
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
    psh.hwndParent = hwndOwner;
    psh.hInstance = hInst;
    psh.pszIcon = NULL;
    psh.pszCaption = langPropTitle();
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = startPage;
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = NULL;

    propModified = 0;

    PropertySheet(&psh);

    if (propModified) {
        propModified = memcmp(&oldProp, pProperties, sizeof(Properties));
    }

    return propModified;
}

//////////////////////////////////////////////////////////////////////////////
// Host dependent load/save methods, Can be moved to its own file

void getStrValue(char* keyFile, char* keyDir, char* keyStr, char* returnValue) {  
    char defStr[512];
    strcpy(defStr, returnValue);
    GetPrivateProfileString("General", keyStr, defStr, returnValue, 512, keyFile);      
}

void getIntValue(char* keyFile, char* keyDir, char* keyStr, DWORD* returnValue) {  
    DWORD def = *returnValue;
    *returnValue = GetPrivateProfileInt("General", keyStr, def, keyFile);                           
}

void setIntValue(char* keyFile, char* keyDir, char* keyStr, DWORD value) {
    char buf[30];

    sprintf(buf, "%d", value);
    WritePrivateProfileString("General", keyStr, buf, keyFile);
}

void setStrValue(char* keyFile, char* keyDir, char* keyStr, char* value) {
    WritePrivateProfileString("General", keyStr, value, keyFile);
}

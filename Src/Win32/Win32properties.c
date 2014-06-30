/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32properties.c,v $
**
** $Revision: 1.92 $
**
** $Date: 2008-05-19 19:56:59 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#include <windows.h>
#include <math.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "AppConfig.h"

#if _MSC_VER
#define snprintf _snprintf
#endif

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
#include "Win32Cdrom.h"
#include "Win32File.h"


#define WM_UPDATEPROPERTIES  (WM_USER + 0)
#define WM_CANCELUPDATEPROPERTIES (WM_USER + 1)
#define WM_VIDEO_DRIVER_CHANGED (WM_USER + 2)

static HWND hDlgDirectDraw = NULL;
static HWND hDlgDirect3d  = NULL;
static HWND hDlgGdi = NULL;
static HWND hDlgVideoSoftware = NULL;
static HWND hDlgVideoDirect3d = NULL;
static HWND hDlgVideo = NULL;
static HWND hDlgSound = NULL;
static int propModified = 0;
static Mixer* theMixer;
static Video* theVideo;
static int centered = 0;
extern void emulatorRestartSound();
extern void updateEmuWindow();

static const int C_iCropMax = 64;

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

static char pVideoDriverData[4][64];
static char* pVideoDriver[] = {
    pVideoDriverData[0],
    pVideoDriverData[1],
    pVideoDriverData[2],
    pVideoDriverData[3],
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
static char* pEmuGdiSync[] = {
    pEmuSyncData[0],
    pEmuSyncData[1],
    NULL
};

static int soundBufSizes[] = { 10, 25, 50, 75, 100, 150, 200, 250, 300, 350 };

static char* pSoundBufferSize[] = {
    "10 ms",
    "25 ms",
    "50 ms",
    "75 ms",
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
       
        SendDlgItemMessage(hDlg, IDC_SNDCHIPEMUGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSndChipEmuGB());

        SendDlgItemMessage(hDlg, IDC_OVERSAMPLETEXT1, WM_SETTEXT, 0, (LPARAM)langPropSndOversampleText());
        SendDlgItemMessage(hDlg, IDC_OVERSAMPLETEXT2, WM_SETTEXT, 0, (LPARAM)langPropSndOversampleText());
        SendDlgItemMessage(hDlg, IDC_OVERSAMPLETEXT3, WM_SETTEXT, 0, (LPARAM)langPropSndOversampleText());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMSXMUSIC), langPropSndMsxMusic());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMSXAUDIO), langPropSndMsxAudio());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMOONSOUND), langPropSndMoonsound());

        SendMessage(GetDlgItem(hDlg, IDC_EMUGENERALGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropEmuGeneralGB());
        SendMessage(GetDlgItem(hDlg, IDC_EMUFAMILYTEXT), WM_SETTEXT, 0, (LPARAM)langPropEmuFamilyText());
        SendMessage(GetDlgItem(hDlg, IDC_VDPFREQTEXT), WM_SETTEXT, 0, (LPARAM)langPropVideoFreqText());
        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEEDTEXT), WM_SETTEXT, 0, (LPARAM)langPropEmuSpeedText());
        SendMessage(GetDlgItem(hDlg, IDC_EMUSPEEDGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropEmuSpeedGB());
        SendMessage(GetDlgItem(hDlg, IDC_EMUFRONTSWITCHGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropEmuFrontSwitchGB());
        
        SetWindowText(GetDlgItem(hDlg, IDC_EMUFDCTIMING),   langPropEmuFdcTiming());
        SetWindowText(GetDlgItem(hDlg, IDC_NOSPRITELIMITS), langPropEmuNoSpriteLimits());
        SetWindowText(GetDlgItem(hDlg, IDC_ENABLEMSXKEYBOARDQUIRK), langPropEnableMsxKeyboardQuirk());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUFRONTSWITCH), langPropEmuFrontSwitch());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUPAUSESWITCH), langPropEmuPauseSwitch());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUAUDIOSWITCH), langPropEmuAudioSwitch());
        SetWindowText(GetDlgItem(hDlg, IDC_EMUREVERSEPLAY), langPropEmuReversePlay());
        
        setButtonCheck(hDlg, IDC_ENABLEMSXMUSIC, pProperties->sound.chip.enableYM2413, 1);
        setButtonCheck(hDlg, IDC_ENABLEMSXAUDIO, pProperties->sound.chip.enableY8950, 1);
        setButtonCheck(hDlg, IDC_ENABLEMOONSOUND, pProperties->sound.chip.enableMoonsound, 1);

        setButtonCheck(hDlg, IDC_EMUFDCTIMING,   !pProperties->emulation.enableFdcTiming, 1);
        setButtonCheck(hDlg, IDC_NOSPRITELIMITS, pProperties->emulation.noSpriteLimits, 1);
        setButtonCheck(hDlg, IDC_ENABLEMSXKEYBOARDQUIRK, pProperties->keyboard.enableKeyboardQuirk, 1);
        setButtonCheck(hDlg, IDC_EMUFRONTSWITCH, pProperties->emulation.frontSwitch, 1);
        setButtonCheck(hDlg, IDC_EMUPAUSESWITCH, pProperties->emulation.pauseSwitch, 1);
        setButtonCheck(hDlg, IDC_EMUAUDIOSWITCH, pProperties->emulation.audioSwitch, 1);
        setButtonCheck(hDlg, IDC_EMUREVERSEPLAY, pProperties->emulation.reverseEnable, 1);

        curSpeed = pProperties->emulation.speed;

        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_ADDSTRING, 0, (LPARAM)langPropVideoFreqAuto());
        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_ADDSTRING, 0, (LPARAM)"50 Hz");
        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_ADDSTRING, 0, (LPARAM)"60 Hz");

        SendDlgItemMessage(hDlg, IDC_VDPFREQ, CB_SETCURSEL, pProperties->emulation.vdpSyncMode, 0);

        if (CB_ERRSPACE == SendMessage(GetDlgItem(hDlg, IDC_EMUFAMILY), CB_INITSTORAGE, (WPARAM)128, (LPARAM)64))
            MessageBox(NULL, "Error allocating machine names", "blueMSX Error", MB_OK |  MB_ICONERROR);

        machineName[0] = 0;

        {
            int index = 0;
            ArrayList *machineList;
            ArrayListIterator *iterator;

			machineList = arrayListCreate();
			machineFillAvailable(machineList, 1);

            iterator = arrayListCreateIterator(machineList);
            while (arrayListCanIterate(iterator)) {
                char *machineInList = (char *)arrayListIterate(iterator);
                char buffer[128];

                snprintf(buffer, sizeof(buffer) - 1, "%s", machineInList);

                SendDlgItemMessage(hDlg, IDC_EMUFAMILY, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (index == 0 || 0 == strcmp(machineInList, pProperties->emulation.machineName)) {
                    SendDlgItemMessage(hDlg, IDC_EMUFAMILY, CB_SETCURSEL, index, 0);
                    strcpy(machineName, machineInList);
                }
                index++;
            }
            arrayListDestroyIterator(iterator);

            arrayListDestroy(machineList);
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
            int index = 0;
            char buffer[64];
            ArrayList *machineList;
			ArrayListIterator *iterator;

			machineList = arrayListCreate();
            machineFillAvailable(machineList, 1);

            pProperties->sound.chip.enableYM2413 = getButtonCheck(hDlg, IDC_ENABLEMSXMUSIC);
            pProperties->sound.chip.enableY8950 = getButtonCheck(hDlg, IDC_ENABLEMSXAUDIO);
            pProperties->sound.chip.enableMoonsound = getButtonCheck(hDlg, IDC_ENABLEMOONSOUND);
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXMUSIC, CB_GETCURSEL, 0, 0);
            pProperties->sound.chip.ym2413Oversampling = 1 << index;
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMSXAUDIO, CB_GETCURSEL, 0, 0);
            pProperties->sound.chip.y8950Oversampling = 1 << index;
            index = SendDlgItemMessage(hDlg, IDC_OVERSAMPLEMOONSOUND, CB_GETCURSEL, 0, 0);
            pProperties->sound.chip.moonsoundOversampling = 1 << index;

            index = 0;
            pProperties->emulation.enableFdcTiming = !getButtonCheck(hDlg, IDC_EMUFDCTIMING);
            pProperties->emulation.noSpriteLimits = getButtonCheck(hDlg, IDC_NOSPRITELIMITS);
            pProperties->keyboard.enableKeyboardQuirk = getButtonCheck(hDlg, IDC_ENABLEMSXKEYBOARDQUIRK);
            pProperties->emulation.frontSwitch = getButtonCheck(hDlg, IDC_EMUFRONTSWITCH);
            pProperties->emulation.pauseSwitch = getButtonCheck(hDlg, IDC_EMUPAUSESWITCH);
            pProperties->emulation.audioSwitch = getButtonCheck(hDlg, IDC_EMUAUDIOSWITCH);
            pProperties->emulation.reverseEnable = getButtonCheck(hDlg, IDC_EMUREVERSEPLAY);

            pProperties->emulation.vdpSyncMode = SendMessage(GetDlgItem(hDlg, IDC_VDPFREQ), CB_GETCURSEL, 0, 0);

            GetDlgItemText(hDlg, IDC_EMUFAMILY, buffer, 63);
            
            iterator = arrayListCreateIterator(machineList);
            while (arrayListCanIterate(iterator)) {
                char *machineInList = (char *)arrayListIterate(iterator);
                if (0 == strcmp(buffer, machineInList)) {
                    strcpy(pProperties->emulation.machineName, buffer);
                    break;
                }
            }
            arrayListDestroyIterator(iterator);

            arrayListDestroy(machineList);
        }

        pProperties->emulation.speed        = curSpeed;

        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

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
  
#ifndef NO_FILE_HISTORY
        if (appConfigGetInt("filehistory", 1) != 0) {
            SendMessage(GetDlgItem(hDlg, IDC_SETINGSFILEHISTORYGOUPBOX), WM_SETTEXT, 0, (LPARAM)langPropSetFileHistoryGB());
            SendMessage(GetDlgItem(hDlg, IDC_SETINGSHISTORYSIZETEXT), WM_SETTEXT, 0, (LPARAM)langPropSetFileHistorySize());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSHISTORYCLEAR), langPropSetFileHistoryClear());

            {
                char buffer[32];
                sprintf(buffer, "%d", pProperties->filehistory.count);
                SetWindowText(GetDlgItem(hDlg, IDC_SETINGSHISTORYSIZE), buffer);
            }
        }
#endif
        for (i = 0; opendialog_getromtype(i) != ROM_UNKNOWN; i++) {
            SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(opendialog_getromtype(i)));
            if (pProperties->cartridge.defaultType == opendialog_getromtype(i)) {
                SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_SETCURSEL, i, 0);
            }
        }

        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSROMTYPEGB), langPropOpenRomGB());
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSROMTYPETEXT), langPropDefaultRomType());

        SendDlgItemMessage(hDlg, IDC_SETTINGSROMTYPE, CB_ADDSTRING, 0, (LPARAM)langPropGuessRomType());
        if (pProperties->cartridge.defaultType == opendialog_getromtype(i)) {
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
#ifndef NO_FILE_HISTORY
            if (appConfigGetInt("filehistory", 1) != 0) {
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
                    pProperties->filehistory.videocap[0] = 0;

                    EnableWindow(GetDlgItem(hDlg, IDC_SETTINGSHISTORYCLEAR), FALSE);
                }
            }
#endif
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
            pProperties->cartridge.defaultType = opendialog_getromtype(i);
        }

#ifndef NO_FILE_HISTORY
        if (appConfigGetInt("filehistory", 1) != 0) {
            char buffer[64];

            GetDlgItemText(hDlg, IDC_SETINGSHISTORYSIZE, buffer, 63);

            if (isdigit(*buffer)) {
                int count = atoi(buffer);
                if (count < 0)  count = 0;
                if (count > 30) count = 30;

                pProperties->filehistory.count = count;
            }
        }
#endif
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
        SetWindowText(GetDlgItem(hDlg, IDC_SETTINGSEJECTMEDIAONEXIT), langPropEjectMediaOnExit());

        setButtonCheck(hDlg, IDC_SETTINGSFILETYPES, pProperties->emulation.registerFileTypes, 1);
        setButtonCheck(hDlg, IDC_SETTINGSDISABLEWINKEYS, pProperties->emulation.disableWinKeys, 1);
        setButtonCheck(hDlg, IDC_SETTINGSPRIORITYBOOST, pProperties->emulation.priorityBoost, 1);
        setButtonCheck(hDlg, IDC_SETTINGSSCREENSAVER, pProperties->settings.disableScreensaver, 1);
        setButtonCheck(hDlg, IDC_SETTINGSSCREENSHOTPNG, pProperties->settings.usePngScreenshots, 1);
        setButtonCheck(hDlg, IDC_SETTINGSEJECTMEDIAONEXIT, pProperties->emulation.ejectMediaOnExit, 1);
    
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
        pProperties->emulation.ejectMediaOnExit  = getButtonCheck(hDlg, IDC_SETTINGSEJECTMEDIAONEXIT);
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

static Properties* pCurrentProperties;

static BOOL CALLBACK directDraWProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:
        pProperties = pCurrentProperties;

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_PERFFRAMESKIPTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfFrameSkipText());
        SendDlgItemMessage(hDlg, IDC_PERFSETTINGSGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSettings());
        SendDlgItemMessage(hDlg, IDC_PERFSYNCMODETEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfSyncModeText());
        SendDlgItemMessage(hDlg, IDC_PERFFULLSCREENTEXT, WM_SETTEXT, 0, (LPARAM)langPropFullscreenResText());
        
		SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPINGGROUPBOX), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingGB());
        SendDlgItemMessage(hDlg, IDC_MONHORIZSTRETCH, WM_SETTEXT, 0, (LPARAM)langPropMonHorizStretch());
        SendDlgItemMessage(hDlg, IDC_MONVERTSTRETCH, WM_SETTEXT, 0, (LPARAM)langPropMonVertStretch());

        initDropList(hDlg, IDC_FRAMESKIP, pVideoFrameSkip, pProperties->video.frameSkip);
        initDropList(hDlg, IDC_EMUSYNC, pEmuSync, pProperties->emulation.syncMethodDirectX);
        
        setButtonCheck(hDlg, IDC_MONHORIZSTRETCH, pProperties->video.horizontalStretch, 1);
        setButtonCheck(hDlg, IDC_MONVERTSTRETCH, pProperties->video.verticalStretch, 1);

        updateFullscreenResList(hDlg);
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_MONHORIZSTRETCH:
            pProperties->video.horizontalStretch = getButtonCheck(hDlg, IDC_MONHORIZSTRETCH);
            updateEmuWindow();
            break;

        case IDC_MONVERTSTRETCH:
            pProperties->video.verticalStretch   = getButtonCheck(hDlg, IDC_MONVERTSTRETCH);
            updateEmuWindow();
            break;
        }
        return TRUE;

    case WM_UPDATEPROPERTIES:
        getFullscreenResList(hDlg, 
                             &pProperties->video.fullscreen.width,
                             &pProperties->video.fullscreen.height,
                             &pProperties->video.fullscreen.bitDepth);
                                
        pProperties->video.frameSkip        = getDropListIndex(hDlg, IDC_FRAMESKIP, pVideoFrameSkip);
        pProperties->emulation.syncMethod   = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);
        pProperties->emulation.syncMethodDirectX   = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);

        DirectDrawSetDisplayMode(pProperties->video.fullscreen.width,
                                pProperties->video.fullscreen.height,
                                pProperties->video.fullscreen.bitDepth);

        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK gdiProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:
        pProperties = pCurrentProperties;

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_PERFFRAMESKIPTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfFrameSkipText());
        SendDlgItemMessage(hDlg, IDC_PERFSETTINGSGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSettings());
        SendDlgItemMessage(hDlg, IDC_PERFSYNCMODETEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfSyncModeText());
        
        initDropList(hDlg, IDC_FRAMESKIP, pVideoFrameSkip, pProperties->video.frameSkip);
        initDropList(hDlg, IDC_EMUSYNC, pEmuGdiSync, pProperties->emulation.syncMethodGdi);

        return FALSE;

    case WM_UPDATEPROPERTIES:
        pProperties->video.frameSkip        = getDropListIndex(hDlg, IDC_FRAMESKIP, pVideoFrameSkip);
        pProperties->emulation.syncMethodGdi   = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);
        pProperties->emulation.syncMethod      = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);

        return TRUE;
    }

    return FALSE;
}

static void D3DUpdateItems(HWND hDlg, Properties* pProperties)
{
    BOOL b = (pProperties->video.d3d.cropType == P_D3D_CROP_SIZE_CUSTOM) ? TRUE : FALSE;

    SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFT), WM_ENABLE, b, 0); 
    SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHT), WM_ENABLE, b, 0); 
    SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOP), WM_ENABLE, b, 0); 
    SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOM), WM_ENABLE, b, 0); 

    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFTTEXT), b);
    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHTTEXT), b);
    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOPTEXT), b);
    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOMTEXT), b);

    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFTVALUETEXT), b);
    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHTVALUETEXT), b);
    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOPVALUETEXT), b);
    EnableWindow(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOMVALUETEXT), b);
}

static BOOL CALLBACK direct3dProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:
        pProperties = pCurrentProperties;

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_PERFFRAMESKIPTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfFrameSkipText());
        SendDlgItemMessage(hDlg, IDC_PERFSETTINGSGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropSettings());
        SendDlgItemMessage(hDlg, IDC_PERFSYNCMODETEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfSyncModeText());
        SendMessage(GetDlgItem(hDlg, IDC_D3D_EXTENDBORDERCOLOR), WM_SETTEXT, 0, (LPARAM)langPropD3DExtendBorderColorText());
        SendMessage(GetDlgItem(hDlg, IDC_D3D_FORCEHIGHRES), WM_SETTEXT, 0, (LPARAM)langPropD3DForceHighResText());
        
		setButtonCheck(hDlg, IDC_D3D_EXTENDBORDERCOLOR, pProperties->video.d3d.extendBorderColor, 1);
		setButtonCheck(hDlg, IDC_D3D_FORCEHIGHRES, pProperties->video.d3d.forceHighRes, 1);

        initDropList(hDlg, IDC_FRAMESKIP, pVideoFrameSkip, pProperties->video.frameSkip);
        initDropList(hDlg, IDC_EMUSYNC, pEmuSync, pProperties->emulation.syncMethodD3D);


		SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPINGGROUPBOX), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingGB());

		SendDlgItemMessage(hDlg, IDC_D3D_ASPECTRATIO, CB_ADDSTRING, 0, (LPARAM)langEnumD3DARAuto());
        SendDlgItemMessage(hDlg, IDC_D3D_ASPECTRATIO, CB_ADDSTRING, 0, (LPARAM)langEnumD3DARStretch());
        SendDlgItemMessage(hDlg, IDC_D3D_ASPECTRATIO, CB_ADDSTRING, 0, (LPARAM)langEnumD3DARPAL());
        SendDlgItemMessage(hDlg, IDC_D3D_ASPECTRATIO, CB_ADDSTRING, 0, (LPARAM)langEnumD3DARNTSC());
		SendDlgItemMessage(hDlg, IDC_D3D_ASPECTRATIO, CB_ADDSTRING, 0, (LPARAM)langEnumD3DAR11());
        SendDlgItemMessage(hDlg, IDC_D3D_ASPECTRATIO, CB_SETCURSEL, pProperties->video.d3d.aspectRatioType, 0);
		SendMessage(GetDlgItem(hDlg, IDC_D3D_ASPECTRATIOTEXT), WM_SETTEXT, 0, (LPARAM)langPropD3DAspectRatioText());

		SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_ADDSTRING, 0, (LPARAM)langEnumD3DCropNone());
		SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_ADDSTRING, 0, (LPARAM)langEnumD3DCropMSX1());
		SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_ADDSTRING, 0, (LPARAM)langEnumD3DCropMSX1Plus8());
        SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_ADDSTRING, 0, (LPARAM)langEnumD3DCropMSX2());
		SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_ADDSTRING, 0, (LPARAM)langEnumD3DCropMSX2Plus8());
        SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_ADDSTRING, 0, (LPARAM)langEnumD3DCropCustom());
        SendDlgItemMessage(hDlg, IDC_D3D_CROPPING_TYPE, CB_SETCURSEL, pProperties->video.d3d.cropType, 0);
		SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TYPETEXT), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingTypeText());

		SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFTTEXT), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingLeftText());
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHTTEXT), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingRightText());
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOPTEXT), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingTopText());
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOMTEXT), WM_SETTEXT, 0, (LPARAM)langpropD3DCroppingBottomText());

		SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFT), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, C_iCropMax));
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHT), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, C_iCropMax));
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOP), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, C_iCropMax));
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOM), TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, C_iCropMax));

        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFT), TBM_SETPOS, 1,  pProperties->video.d3d.cropLeft);
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHT), TBM_SETPOS, 1, pProperties->video.d3d.cropRight);
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOP), TBM_SETPOS, 1, pProperties->video.d3d.cropTop);
        SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOM), TBM_SETPOS, 1, pProperties->video.d3d.cropBottom);

        D3DUpdateItems(hDlg, pProperties);

        return FALSE;

    case WM_NOTIFY:
        {
            char acBuffer[32];

            if (wParam == IDC_D3D_CROPPING_LEFT) {
                pProperties->video.d3d.cropLeft = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFT), TBM_GETPOS, 0, 0);
                sprintf(acBuffer, "%d", pProperties->video.d3d.cropLeft);
                SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFTVALUETEXT), WM_SETTEXT, 0, (LPARAM)acBuffer);
            }
            if (wParam == IDC_D3D_CROPPING_RIGHT) {
                pProperties->video.d3d.cropRight = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHT), TBM_GETPOS, 0, 0);
                sprintf(acBuffer, "%d", pProperties->video.d3d.cropRight);
                SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHTVALUETEXT), WM_SETTEXT, 0, (LPARAM)acBuffer);
            }
            if (wParam == IDC_D3D_CROPPING_TOP) {
                pProperties->video.d3d.cropTop = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOP), TBM_GETPOS, 0, 0);
                sprintf(acBuffer, "%d", pProperties->video.d3d.cropTop);
                SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOPVALUETEXT), WM_SETTEXT, 0, (LPARAM)acBuffer);
            }
            if (wParam == IDC_D3D_CROPPING_BOTTOM) {
                pProperties->video.d3d.cropBottom = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOM), TBM_GETPOS, 0, 0);
                sprintf(acBuffer, "%d", pProperties->video.d3d.cropBottom);
                SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOMVALUETEXT), WM_SETTEXT, 0, (LPARAM)acBuffer);
            }
        }
        return TRUE;

    case WM_COMMAND:
        pProperties->video.d3d.extendBorderColor = getButtonCheck(hDlg, IDC_D3D_EXTENDBORDERCOLOR);
        pProperties->video.d3d.forceHighRes = getButtonCheck(hDlg, IDC_D3D_FORCEHIGHRES);

        pProperties->video.d3d.aspectRatioType = SendMessage(GetDlgItem(hDlg, IDC_D3D_ASPECTRATIO), CB_GETCURSEL, 0, 0);
        pProperties->video.d3d.cropType = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TYPE), CB_GETCURSEL, 0, 0);

        pProperties->video.d3d.cropBottom = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_BOTTOM), TBM_GETPOS, 0, 0);
        pProperties->video.d3d.cropTop = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_TOP), TBM_GETPOS, 0, 0);
        pProperties->video.d3d.cropRight = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_RIGHT), TBM_GETPOS, 0, 0);
        pProperties->video.d3d.cropLeft = SendMessage(GetDlgItem(hDlg, IDC_D3D_CROPPING_LEFT), TBM_GETPOS, 0, 0);

        D3DUpdateItems(hDlg, pProperties);
        return TRUE;

    case WM_UPDATEPROPERTIES:
        pProperties->video.frameSkip        = getDropListIndex(hDlg, IDC_FRAMESKIP, pVideoFrameSkip);
        pProperties->emulation.syncMethod   = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);
        pProperties->emulation.syncMethodD3D   = getDropListIndex(hDlg, IDC_EMUSYNC, pEmuSync);

        return TRUE;
    }

    return FALSE;
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
        pCurrentProperties = pProperties;

        hDlgDirectDraw = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PERF_DIRECTDRAW), hDlg, directDraWProc);
        SetWindowPos(hDlgDirectDraw,  NULL, 18, 74, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        hDlgGdi = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PERF_GDI), hDlg, gdiProc);
        SetWindowPos(hDlgGdi,  NULL, 18, 74, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        hDlgDirect3d = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PERF_DIRECT3D), hDlg, direct3dProc);
        SetWindowPos(hDlgDirect3d,  NULL, 18, 74, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_PERFVIDEODRVGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropPerfVideoDrvGB());
        SendDlgItemMessage(hDlg, IDC_PERFDISPDRVTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfVideoDispDrvText());
        
        initDropList(hDlg, IDC_VIDEODRV, pVideoDriver, pProperties->video.driver);
        
        ShowWindow(hDlgDirectDraw,  pProperties->video.driver < 2 ? SW_NORMAL : SW_HIDE);
        ShowWindow(hDlgGdi,  pProperties->video.driver == 2 ? SW_NORMAL : SW_HIDE);
        ShowWindow(hDlgDirect3d,  pProperties->video.driver == 3 ? SW_NORMAL : SW_HIDE);

        return FALSE;


    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_VIDEODRV:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                int index = getDropListIndex(hDlg, IDC_VIDEODRV, pVideoDriver);
                ShowWindow(hDlgDirectDraw, index < 2 ? SW_NORMAL : SW_HIDE);
                ShowWindow(hDlgGdi,  index == 2 ? SW_NORMAL : SW_HIDE);
                ShowWindow(hDlgDirect3d,  index == 3 ? SW_NORMAL : SW_HIDE);
                SendMessage(hDlgVideo, WM_VIDEO_DRIVER_CHANGED, index, 0);
                return TRUE;
            }
            break;
        }
        return FALSE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }
        
        pProperties->video.driver           = getDropListIndex(hDlg, IDC_VIDEODRV, pVideoDriver);

        switch (pProperties->video.driver) {
        case 0:
        case 1:
            SendMessage(hDlgDirectDraw,  WM_UPDATEPROPERTIES, 0, 0);
            break;
        case 2:
            SendMessage(hDlgGdi,  WM_UPDATEPROPERTIES, 0, 0);
            break;
        case 3:
            SendMessage(hDlgDirect3d,  WM_UPDATEPROPERTIES, 0, 0);
            break;
        }
            
        propModified = 1;
        
        return TRUE;
    }

    return FALSE;
}

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

static BOOL CALLBACK videoDirect3dDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:    
        pProperties = pCurrentProperties;

        SendMessage(GetDlgItem(hDlg, IDC_D3D_PARAMETERSGROUPBOX), WM_SETTEXT, 0, (LPARAM)langPropD3DParametersGB());
        SendDlgItemMessage(hDlg, IDC_MONDEINTERLACE, WM_SETTEXT, 0, (LPARAM)langPropMonDeInterlace());
        SendDlgItemMessage(hDlg, IDC_MONBLENDFRAMES, WM_SETTEXT, 0, (LPARAM)langPropMonBlendFrames());
		SendMessage(GetDlgItem(hDlg, IDC_D3D_LINEARFILTERING), WM_SETTEXT, 0, (LPARAM)langPropD3DLinearFilteringText());

		setButtonCheck(hDlg, IDC_D3D_LINEARFILTERING, pProperties->video.d3d.linearFiltering, 1);
        
        setButtonCheck(hDlg, IDC_MONDEINTERLACE, pProperties->video.deInterlace, 1);
        setButtonCheck(hDlg, IDC_MONBLENDFRAMES, pProperties->video.blendFrames, 1);

        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_D3D_LINEARFILTERING:
            pProperties->video.d3d.linearFiltering = getButtonCheck(hDlg, IDC_D3D_LINEARFILTERING);
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
        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK videoSoftwareDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
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
        pProperties = pCurrentProperties;

        /* Init language specific dialog items */
        SendDlgItemMessage(hDlg, IDC_MONGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropMonMonGB());
        SendDlgItemMessage(hDlg, IDC_MONTYPETEXT, WM_SETTEXT, 0, (LPARAM)langPropMonTypeText());
        SendDlgItemMessage(hDlg, IDC_MONEMUTEXT, WM_SETTEXT, 0, (LPARAM)langPropMonEmuText());
        SendDlgItemMessage(hDlg, IDC_MONVIDEOTYPETEXT, WM_SETTEXT, 0, (LPARAM)langPropVideoTypeText());
        SendDlgItemMessage(hDlg, IDC_MONWINDOWSIZETEXT, WM_SETTEXT, 0, (LPARAM)langPropWindowSizeText());
        SendDlgItemMessage(hDlg, IDC_MONDEINTERLACE, WM_SETTEXT, 0, (LPARAM)langPropMonDeInterlace());
        SendDlgItemMessage(hDlg, IDC_MONBLENDFRAMES, WM_SETTEXT, 0, (LPARAM)langPropMonBlendFrames());
        SendDlgItemMessage(hDlg, IDC_EFFECTSGB, WM_SETTEXT, 0, (LPARAM)langPropMonEffectsGB());

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
        return TRUE;

    case WM_NOTIFY:
        switch (wParam) {
        case IDC_SCANLINESSLIDEBAR:
            pProperties->video.scanlinesPct = 100 - SendMessage(GetDlgItem(hDlg, IDC_SCANLINESSLIDEBAR), TBM_GETPOS, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_SCANLINESVALUE), WM_SETTEXT, 0, (LPARAM)strPct(100 - pProperties->video.scanlinesPct));

            videoSetScanLines(theVideo, pProperties->video.scanlinesEnable, pProperties->video.scanlinesPct);

            updateEmuWindow();
            break;
            
        case IDC_COLORGHOSTINGSLIDEBAR:
            pProperties->video.colorSaturationWidth = SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGSLIDEBAR), TBM_GETPOS, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_COLORGHOSTINGVALUE), WM_SETTEXT, 0, (LPARAM)strPt(pProperties->video.colorSaturationWidth));

            videoSetColorSaturation(theVideo, pProperties->video.colorSaturationEnable, pProperties->video.colorSaturationWidth);
            updateEmuWindow();
            break;

        case IDC_MONSATURATIONSLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONSLIDE), TBM_GETPOS, 0, 0);
            if (value != saturation) {
                saturation = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONSATURATIONVALUE), WM_SETTEXT, 0, (LPARAM)strDec(saturation));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            break;

        case IDC_MONBRIGHTNESSSLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSSLIDE), TBM_GETPOS, 0, 0);
            if (value != brightness) {
                brightness = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONBRIGHTNESSVALUE), WM_SETTEXT, 0, (LPARAM)strDec(brightness));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            break;

        case IDC_MONCONTRASTSLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTSLIDE), TBM_GETPOS, 0, 0);
            if (value != contrast) {
                contrast = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONCONTRASTVALUE), WM_SETTEXT, 0, (LPARAM)strDec(contrast));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            break;

        case IDC_MONGAMMASLIDE:
            value = SendMessage(GetDlgItem(hDlg, IDC_MONGAMMASLIDE), TBM_GETPOS, 0, 0);
            if (value != gamma) {
                gamma = value;
                SendMessage(GetDlgItem(hDlg, IDC_MONGAMMAVALUE), WM_SETTEXT, 0, (LPARAM)strDec(gamma));
                videoSetColors(theVideo, saturation, brightness, contrast, gamma);
                updateEmuWindow();
            }
            break;
        }
        return TRUE;

    case WM_CANCELUPDATEPROPERTIES:
        pProperties->video.horizontalStretch     = oldHoriz;
        pProperties->video.verticalStretch       = oldVert;
        pProperties->video.deInterlace           = oldDeinterlace;
        pProperties->video.scanlinesEnable       = oldScanlinesEnable;
        pProperties->video.scanlinesPct          = oldScanlinesPct;
        pProperties->video.colorSaturationEnable = oldColorGhostingEnable;
        pProperties->video.colorSaturationWidth  = oldColorGhostingWidth;

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
        updateEmuWindow();
        return TRUE;

    case WM_UPDATEPROPERTIES:
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

static BOOL CALLBACK videoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static Properties* pProperties;

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;
        pCurrentProperties = pProperties;

        hDlgVideo = hDlg;

        hDlgVideoSoftware = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_VIDEO_SOFTWARE), hDlg, videoSoftwareDlgProc);
        SetWindowPos(hDlgVideoSoftware,  NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        hDlgVideoDirect3d = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_VIDEO_DIRECT3D), hDlg, videoDirect3dDlgProc);
        SetWindowPos(hDlgVideoDirect3d,  NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        ShowWindow(hDlgVideoSoftware,  pProperties->video.driver != 3 ? SW_NORMAL : SW_HIDE);
        ShowWindow(hDlgVideoDirect3d,  pProperties->video.driver == 3 ? SW_NORMAL : SW_HIDE);

        return FALSE;

    case WM_VIDEO_DRIVER_CHANGED:
        ShowWindow(hDlgVideoSoftware,  wParam != 3 ? SW_NORMAL : SW_HIDE);
        ShowWindow(hDlgVideoDirect3d,  wParam == 3 ? SW_NORMAL : SW_HIDE);
        return TRUE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            if ((((NMHDR FAR *)lParam)->code) == PSN_QUERYCANCEL) {
               SendMessage(hDlgVideoSoftware,  WM_CANCELUPDATEPROPERTIES, 0, 0);
               SendMessage(hDlgVideoDirect3d,  WM_CANCELUPDATEPROPERTIES, 0, 0);
            }
            return FALSE;
        }
        
        SendMessage(hDlgVideoSoftware,  WM_UPDATEPROPERTIES, 0, 0);
        SendMessage(hDlgVideoDirect3d,  WM_UPDATEPROPERTIES, 0, 0);
            
        propModified = 1;
        return TRUE;
    }

    return FALSE;
}

static void getMidiList(HWND hDlg, int id, Properties* pProperties) {
    char buffer[MAX_PATH];
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

        SendDlgItemMessage(hDlg, IDC_AUDIODRVGROUPBOX, WM_SETTEXT, 0, (LPARAM)langPropPerfAudioDrvGB());
        SendDlgItemMessage(hDlg, IDC_PERFSNDDRVTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfAudioDrvText());
        SendDlgItemMessage(hDlg, IDC_PERFSNDBUFSZTEXT, WM_SETTEXT, 0, (LPARAM)langPropPerfAudioBufSzText());
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
            pProperties->sound.driver           = getDropListIndex(hDlg, IDC_SNDDRIVER, pSoundDriver);
            pProperties->sound.bufSize          = soundBufSizes[getDropListIndex(hDlg, IDC_SNDBUFSZ, pSoundBufferSize)];

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

static void updateCdromListIoctl(HWND hWnd, Properties* pProperties)
{
    int index;
    int drvidx = 0;
    const char* list;
    char str[8];

    SendMessage(hWnd, CB_RESETCONTENT, 0, 0);
    list = cdromGetDriveListIoctl();
    if (list && list[0]) {
        const char* p = list;
        while (*p) {
            sprintf(str, "%c:", *p);
            index = SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)str);
            SendMessage(hWnd, CB_SETITEMDATA, index, (LPARAM)*p);
            if (pProperties->diskdrive.cdromDrive == (int)*p) {
                drvidx = index;
            }
            p++;
        }
    }
    SendMessage(hWnd, CB_SETCURSEL, (WPARAM)drvidx, 0);
}

static void updateCdromListAspi(HWND hWnd, Properties* pProperties)
{
    int index;
    int drvidx = 0;
    const int* tbl = cdromGetDriveTblAspi();

    SendMessage(hWnd, CB_RESETCONTENT, 0, 0);
    if (tbl && *tbl) {
        const char* str;
        do {
            str = cdromGetDriveListAspi(*tbl);
            index = SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)str);
            SendMessage(hWnd, CB_SETITEMDATA, index, (LPARAM)*tbl);
            if (pProperties->diskdrive.cdromDrive == *tbl) {
                drvidx = index;
            }
            tbl++;
        } while(*tbl);
    }
    SendMessage(hWnd, CB_SETCURSEL, (WPARAM)drvidx, 0);
}

static BOOL CALLBACK diskDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    HWND hMethod, hDrive;
    static Properties* pProperties;
    int index, data;
    const char* list;
    const int* tbl;
    int methodIdx[3];
    int method;

    switch (iMsg) {
    case WM_INITDIALOG:
        if (!centered) {
            updateDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES, 0, 1);
            centered = 1;
        }
        pProperties = (Properties*)((PROPSHEETPAGE*)lParam)->lParam;
        SetWindowText(GetDlgItem(hDlg, IDC_CDROMGROUPBOX), langPropCdromGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CDROMMETHODTEXT), langPropCdromMethod());
        SetWindowText(GetDlgItem(hDlg, IDC_CDROMDRIVETEXT), langPropCdromDrive());
        hMethod = GetDlgItem(hDlg, IDC_CDROMMETHODLIST);
        SendMessage(hMethod, CB_ADDSTRING, 0, (LPARAM)langPropCdromMethodNone());
        SendMessage(hMethod, CB_SETITEMDATA, 0, (LPARAM)P_CDROM_DRVNONE);

        memset(methodIdx, 0, sizeof(methodIdx));
        list = cdromGetDriveListIoctl();
        if (list && list[0]) {
            index = SendMessage(hMethod, CB_ADDSTRING, 0, (LPARAM)langPropCdromMethodIoctl());
            SendMessage(hMethod, CB_SETITEMDATA, (WPARAM)index, (LPARAM)P_CDROM_DRVIOCTL);
            methodIdx[P_CDROM_DRVIOCTL] = index;
        }

        tbl = cdromGetDriveTblAspi();
        if (tbl && *tbl) {
            index = SendMessage(hMethod, CB_ADDSTRING, 0, (LPARAM)langPropCdromMethodAspi());
            SendMessage(hMethod, CB_SETITEMDATA, (WPARAM)index, (LPARAM)P_CDROM_DRVASPI);
            methodIdx[P_CDROM_DRVASPI] = index;
        }

        method = pProperties->diskdrive.cdromMethod;
        index = 0;
        if (method == P_CDROM_DRVIOCTL || method == P_CDROM_DRVASPI) {
            index = methodIdx[method];
        }
        SendMessage(hMethod, CB_SETCURSEL, (WPARAM)index, 0);

        hDrive = GetDlgItem(hDlg, IDC_CDROMDRIVELIST);
        switch (method) {
        case P_CDROM_DRVIOCTL:
            updateCdromListIoctl(hDrive, pProperties);
            break;
        case P_CDROM_DRVASPI:
            updateCdromListAspi(hDrive, pProperties);
            break;
        default:
            EnableWindow(hDrive, FALSE);
        }

        if (SendMessage(hMethod, CB_GETCOUNT, 0, 0) < 2) {
            EnableWindow(hMethod, FALSE);
        }

        return FALSE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_CDROMMETHODLIST:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                hMethod = (HWND)lParam;
                index = SendMessage(hMethod, CB_GETCURSEL, 0, 0);
                data  = (int)SendMessage(hMethod, CB_GETITEMDATA, index, 0);
                hDrive = GetDlgItem(hDlg, IDC_CDROMDRIVELIST);
                EnableWindow(hDrive, data > 0);
                switch (data) {
                case P_CDROM_DRVIOCTL:
                    updateCdromListIoctl(hDrive, pProperties);
                    break;
                case P_CDROM_DRVASPI:
                    updateCdromListAspi(hDrive, pProperties);
                    break;
                }
                return TRUE;
            }
        }
        return FALSE;

    case WM_NOTIFY:
        if (((NMHDR FAR*)lParam)->code == PSN_APPLY || ((NMHDR FAR*)lParam)->code == PSN_QUERYCANCEL) {
            saveDialogPos(GetParent(hDlg), DLG_ID_PROPERTIES);
        }

        if ((((NMHDR FAR *)lParam)->code) != PSN_APPLY) {
            return FALSE;
        }

        index = SendDlgItemMessage(hDlg, IDC_CDROMMETHODLIST, CB_GETCURSEL, 0, 0);
        pProperties->diskdrive.cdromMethod = (int)SendDlgItemMessage(hDlg, IDC_CDROMMETHODLIST, CB_GETITEMDATA, index, 0);
        index = SendDlgItemMessage(hDlg, IDC_CDROMDRIVELIST, CB_GETCURSEL, 0, 0);
        pProperties->diskdrive.cdromDrive = (int)SendDlgItemMessage(hDlg, IDC_CDROMDRIVELIST, CB_GETITEMDATA, index, 0);

        propModified = 1;

        return TRUE;
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


static void getPortsLptList(HWND hDlg, int id, Properties* pProperties) {
    char buffer[MAX_PATH];
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
    
    // Add SVI Printer
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM)"SVI Printer");
    
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
    char buffer[MAX_PATH];
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
    HWND hMethod, hDrive;
    int index, data;
    const char* list;
    const int* tbl;
    int methodIdx[3];
    int method;

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

        
        SetWindowText(GetDlgItem(hDlg, IDC_CDROMGROUPBOX), langPropCdromGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CDROMMETHODTEXT), langPropCdromMethod());
        SetWindowText(GetDlgItem(hDlg, IDC_CDROMDRIVETEXT), langPropCdromDrive());
        hMethod = GetDlgItem(hDlg, IDC_CDROMMETHODLIST);
        SendMessage(hMethod, CB_ADDSTRING, 0, (LPARAM)langPropCdromMethodNone());
        SendMessage(hMethod, CB_SETITEMDATA, 0, (LPARAM)P_CDROM_DRVNONE);

        memset(methodIdx, 0, sizeof(methodIdx));
        list = cdromGetDriveListIoctl();
        if (list && list[0]) {
            index = SendMessage(hMethod, CB_ADDSTRING, 0, (LPARAM)langPropCdromMethodIoctl());
            SendMessage(hMethod, CB_SETITEMDATA, (WPARAM)index, (LPARAM)P_CDROM_DRVIOCTL);
            methodIdx[P_CDROM_DRVIOCTL] = index;
        }

        tbl = cdromGetDriveTblAspi();
        if (tbl && *tbl) {
            index = SendMessage(hMethod, CB_ADDSTRING, 0, (LPARAM)langPropCdromMethodAspi());
            SendMessage(hMethod, CB_SETITEMDATA, (WPARAM)index, (LPARAM)P_CDROM_DRVASPI);
            methodIdx[P_CDROM_DRVASPI] = index;
        }

        method = pProperties->diskdrive.cdromMethod;
        index = 0;
        if (method == P_CDROM_DRVIOCTL || method == P_CDROM_DRVASPI) {
            index = methodIdx[method];
        }
        SendMessage(hMethod, CB_SETCURSEL, (WPARAM)index, 0);

        hDrive = GetDlgItem(hDlg, IDC_CDROMDRIVELIST);
        switch (method) {
        case P_CDROM_DRVIOCTL:
            updateCdromListIoctl(hDrive, pProperties);
            break;
        case P_CDROM_DRVASPI:
            updateCdromListAspi(hDrive, pProperties);
            break;
        default:
            EnableWindow(hDrive, FALSE);
        }

        if (SendMessage(hMethod, CB_GETCOUNT, 0, 0) < 2) {
            EnableWindow(hMethod, FALSE);
        }

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
        case IDC_CDROMMETHODLIST:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                hMethod = (HWND)lParam;
                index = SendMessage(hMethod, CB_GETCURSEL, 0, 0);
                data  = (int)SendMessage(hMethod, CB_GETITEMDATA, index, 0);
                hDrive = GetDlgItem(hDlg, IDC_CDROMDRIVELIST);
                EnableWindow(hDrive, data > 0);
                switch (data) {
                case P_CDROM_DRVIOCTL:
                    updateCdromListIoctl(hDrive, pProperties);
                    break;
                case P_CDROM_DRVASPI:
                    updateCdromListAspi(hDrive, pProperties);
                    break;
                }
                return TRUE;
            }
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

        index = SendDlgItemMessage(hDlg, IDC_CDROMMETHODLIST, CB_GETCURSEL, 0, 0);
        pProperties->diskdrive.cdromMethod = (int)SendDlgItemMessage(hDlg, IDC_CDROMMETHODLIST, CB_GETITEMDATA, index, 0);
        index = SendDlgItemMessage(hDlg, IDC_CDROMDRIVELIST, CB_GETCURSEL, 0, 0);
        pProperties->diskdrive.cdromDrive = (int)SendDlgItemMessage(hDlg, IDC_CDROMDRIVELIST, CB_GETITEMDATA, index, 0);

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

int showProperties(Properties* pProperties, HWND hwndOwner, PropPage desiredStartPage, Mixer* mixer, Video* video) {
	HINSTANCE       hInst = (HINSTANCE)GetModuleHandle(NULL);
    PROPSHEETPAGE   psp[9];
    PROPSHEETHEADER psh;
    Properties oldProp = *pProperties;
    UINT startPage = -1;
    UINT curPage = 0;

    centered = 0;
    hDlgSound = NULL;
    theMixer = mixer;
    theVideo = video;
    
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

    sprintf(pVideoDriver[0], "%s", langEnumVideoDrvDirectDrawHW());
    sprintf(pVideoDriver[1], "%s", langEnumVideoDrvDirectDraw());
    sprintf(pVideoDriver[2], "%s", langEnumVideoDrvGDI());
    sprintf(pVideoDriver[3], "%s", langEnumVideoDrvD3D());

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


    if (appConfigGetInt("properties.emulation", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_EMULATION);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = emulationDlgProc;
        psp[curPage].pszTitle = langPropEmulation();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_EMULATION || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

    if (appConfigGetInt("properties.performance", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_PERFORMANCE);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = performanceDlgProc;
        psp[curPage].pszTitle = langPropVideo();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_PERFORMANCE || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

    if (appConfigGetInt("properties.video", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = videoDlgProc;
        psp[curPage].pszTitle = langPropEffects();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_VIDEO || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

    if (appConfigGetInt("properties.sound", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_SOUND);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = soundDlgProc;
        psp[curPage].pszTitle = langPropSound();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_SOUND || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

    if (appConfigGetInt("properties.ports", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_PORTS);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = portsDlgProc;
        psp[curPage].pszTitle = langPropPorts();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_PORTS || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

    if (appConfigGetInt("properties.settings", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_SETTINGS);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = filesDlgProc;
        psp[curPage].pszTitle = langPropFile();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_SETTINGS || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

#if 0
    if (appConfigGetInt("properties.disk", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_DISKEMU);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = diskDlgProc;
        psp[curPage].pszTitle = langPropDisk();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_DISK || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }
#endif

    if (appConfigGetInt("properties.appearance", 1) != 0) {
        psp[curPage].dwSize = sizeof(PROPSHEETPAGE);
        psp[curPage].dwFlags = PSP_USEICONID | PSP_USETITLE;
        psp[curPage].hInstance = hInst;
        psp[curPage].pszTemplate = MAKEINTRESOURCE(IDD_APEARANCE);
        psp[curPage].pszIcon = NULL;
        psp[curPage].pfnDlgProc = settingsDlgProc;
        psp[curPage].pszTitle = langPropSettings();
        psp[curPage].lParam = (LPARAM)pProperties;
        psp[curPage].pfnCallback = NULL;
        if (desiredStartPage == PROP_APEARANCE || startPage == -1) {
            startPage = curPage;
        }
        curPage++;
    }

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
    psh.hwndParent = hwndOwner;
    psh.hInstance = hInst;
    psh.pszIcon = NULL;
    psh.pszCaption = langPropTitle();
    psh.nPages = curPage;
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

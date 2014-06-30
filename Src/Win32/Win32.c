/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32.c,v $
**
** $Revision: 1.205 $
**
** $Date: 2009-07-07 02:38:25 $
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
#define DIRECTINPUT_VERSION     0x0700
#define _WIN32_DCOM

#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <shlobj.h> 
#include <shlwapi.h>
#include <dbt.h>
#include "Win32FileTypes.h"
#include "Win32ThemeClassic.h"
#include "Board.h"
#include "Led.h"
#include "Switches.h"
#include "AudioMixer.h"
#include "VideoRender.h"
#include "CommandLine.h"
#include "Language.h"   
#include "resource.h"
#include "Casette.h"
#include "PrinterIO.h"
#include "UartIO.h"
#include "MidiIO.h"
#include "RomLoader.h"
#include "MediaDb.h"
#include "build_number.h"
#include "FrameBuffer.h"
#include "Win32Midi.h"
#include "Win32Sound.h"
#include "Win32Properties.h"
#include "Win32ToolLoader.h"
#include "Win32joystick.h"
#include "Win32keyboard.h"
#include "Win32Printer.h"
#include "Win32directx.h"
#include "Win32D3D.h"
#include "Win32Avi.h"
#include "FileHistory.h"
#include "Win32Dir.h"
#include "Win32file.h"
#include "Win32Help.h"
#include "Win32Menu.h"
#include "Win32Eth.h"
#include "Win32VideoIn.h"
#include "Win32ScreenShot.h"
#include "Win32MouseEmu.h"
#include "Win32machineConfig.h"
#include "Win32ShortcutsConfig.h"
#include "Win32Cdrom.h"
#include "Actions.h"
#include "LaunchFile.h"
#include "TokenExtract.h"
#include "Emulator.h"
#include "JoystickPort.h"
#include "Theme.h"
#include "VideoRender.h"
#include "ThemeLoader.h"
#include "Win32ThemeClassic.h"
#include "ArchNotifications.h"
#include "ArchEvent.h"
#include "ArchTimer.h"
#include "ArchFile.h"
#include "ArchInput.h"
#include "AppConfig.h"
#include "SlotManager.h"

#pragma warning(disable: 4996)

// PacketFileSystem.h Need to be included after all other includes
#include "PacketFileSystem.h"

void vdpSetDisplayEnable(int enable);


static EmuLanguageType getLangType()
{
    LANGID langId = GetSystemDefaultLangID();

    switch (langId) {
        case 0x0404: return EMU_LANG_CHINESETRAD;
        case 0x0804: return EMU_LANG_CHINESESIMP;
        case 0x0c04: return EMU_LANG_CHINESETRAD;
        case 0x1004: return EMU_LANG_CHINESESIMP;
        case 0x1404: return EMU_LANG_CHINESETRAD;
    }

    switch (langId & 0xff) {
        case 0x04: return EMU_LANG_CHINESESIMP;
        case 0x13: return EMU_LANG_DUTCH;
        case 0x09: return EMU_LANG_ENGLISH;
        case 0x0b: return EMU_LANG_FINNISH;
        case 0x0c: return EMU_LANG_FRENCH;
        case 0x07: return EMU_LANG_GERMAN;
        case 0x10: return EMU_LANG_ITALIAN;
        case 0x11: return EMU_LANG_JAPANESE;
        case 0x12: return EMU_LANG_KOREAN;
        case 0x15: return EMU_LANG_POLISH;
        case 0x16: return EMU_LANG_PORTUGUESE;
        case 0x0a: return EMU_LANG_SPANISH;
        case 0x1d: return EMU_LANG_SWEDISH;
        case 0x19: return EMU_LANG_RUSSIAN;
        case 0x03: return EMU_LANG_CATALAN;
    }

    return EMU_LANG_ENGLISH;
}

int canHandleVblankSyncMode() {
    DWORD t1;
    DWORD t2;
    DWORD cnt = 0;
    int currPri;
    
    currPri= GetThreadPriority(GetCurrentThread());
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    t1 = t2 = archGetSystemUpTime(100);
    while (t2 == t1) {
        t1 = t2;
        t2 = archGetSystemUpTime(100);
    }
    t1 = t2;
    while (t2 == t1) {
        t1 = t2;
        t2 = archGetSystemUpTime(100);
        cnt++;
    }
    SetThreadPriority(GetCurrentThread(), currPri);

    return cnt > 6000;
}

void centerDialog(HWND hwnd, int noActivate) {
    RECT r1;
    RECT r2;
    int x;
    int y;

    GetWindowRect(GetParent(hwnd), &r1);
    GetWindowRect(hwnd, &r2);

    x = r1.left + (r1.right - r1.left - r2.right + r2.left) / 2;
    y = r1.top  + (r1.bottom - r1.top - r2.bottom + r2.top) / 2;
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | (noActivate ? SWP_NOACTIVATE : 0));
}

void updateDialogPos(HWND hwnd, int dialogID, int noMove, int noSize) 
{
    Properties* pProperties = propGetGlobalProperties();

    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT r1;
    int x;
    int y;
    int w;
    int h;

    if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
        noMove = 1;
        noSize = 1;
    }

    GetWindowRect(GetParent(hwnd), &r1);

	x = r1.left + pProperties->settings.windowPos[dialogID].left; 
    y = r1.top  + pProperties->settings.windowPos[dialogID].top; 
    w =           pProperties->settings.windowPos[dialogID].width; 
    h =           pProperties->settings.windowPos[dialogID].height; 

    if (noMove || 
	   (pProperties->settings.windowPos[dialogID].left == 0 && 
	    pProperties->settings.windowPos[dialogID].top == 0)) {
        RECT r2;
        GetWindowRect(hwnd, &r2);
        x = r1.left + (r1.right - r1.left - r2.right + r2.left) / 2;
        y  = r1.top  + (r1.bottom - r1.top - r2.bottom + r2.top) / 2;
    }
    
    if (w == 0 || h == 0) {
        RECT r2;
        GetWindowRect(hwnd, &r2);
        w = r2.right - r2.left;
        h = r2.bottom - r2.top;
    }
    /* Laurent Halter : Removed because window might be on a secondary display*/
    /*
    if (x + w > screenWidth) {
        x = screenWidth - w;
    }

    if (y + h > screenHeight) {
        y = screenHeight - h;
    }

    if (y < 0) {
        y = 0;
    }*/

    SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | (noSize ? SWP_NOSIZE : 0));
}

void saveDialogPos(HWND hwnd, int dialogID)
{
    Properties* pProperties = propGetGlobalProperties();
    RECT r;
    RECT r1;

    if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
        GetWindowRect(hwnd, &r);
        GetWindowRect(GetParent(hwnd), &r1);
        pProperties->settings.windowPos[dialogID].left   = r.left   - r1.left;
        pProperties->settings.windowPos[dialogID].top    = r.top    - r1.top;
        pProperties->settings.windowPos[dialogID].width  = r.right  - r.left;
        pProperties->settings.windowPos[dialogID].height = r.bottom - r.top;
    }
}

///////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK langDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static int* lang = NULL;

    switch (iMsg) {
    case WM_INITDIALOG:
        {
            char buffer[64];
            HIMAGELIST himlSmall;
            LV_COLUMN lvc = {0};
            LV_ITEM lvi = { 0 };
            int i;

            lang = (int*)lParam;

            SetWindowText(hDlg, langDlgLangTitle());
            SendMessage(GetDlgItem(hDlg, IDC_LANGTXT), WM_SETTEXT, 0, (LPARAM)langDlgLangLangText());
            SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
            SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());

            ListView_SetExtendedListViewStyle(GetDlgItem(hDlg, IDC_LANGLIST), LVS_EX_FULLROWSELECT);

            himlSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), TRUE, 1, 1); 

            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_CATALONIA))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_CHINASIMP))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_CHINATRAD))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_NETHERLANDS))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_USA))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_FINLAND))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_FRANCE))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_GERMANY))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_ITALY))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_JAPAN))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_KOREA))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_POLAND))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_BRAZIL))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_RUSSIA))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_SPAIN))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FLAG_SWEDEN))); 
            
            ListView_SetImageList(GetDlgItem(hDlg, IDC_LANGLIST), himlSmall, LVSIL_SMALL);

            SetFocus(GetDlgItem(hDlg, IDC_LANGLIST));

            lvc.mask       = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
            lvc.fmt        = LVCFMT_LEFT;
            lvc.cx         = 185;
            lvc.pszText    = buffer;
	        lvc.cchTextMax = sizeof(buffer);
            sprintf(buffer, "       %s", langMenuPropsLanguage());

            ListView_InsertColumn(GetDlgItem(hDlg, IDC_LANGLIST), 0, &lvc);

            for (i = 0; langGetType(i) != EMU_LANG_UNKNOWN; i++) {
                lvi.mask       = LVIF_IMAGE | LVIF_TEXT;
                lvi.iItem      = i;
                lvi.pszText    = buffer;
	            lvi.cchTextMax = sizeof(buffer);
                lvi.iImage     = i;

                sprintf(buffer, "   %s", langToName(langGetType(i), 1));

                ListView_InsertItem(GetDlgItem(hDlg, IDC_LANGLIST), &lvi);
 
                if (langGetType(i) == *lang) {
                    ListView_SetItemState(GetDlgItem(hDlg, IDC_LANGLIST), i, LVIS_SELECTED, LVIS_SELECTED);
                }
           }

            return FALSE;
        }

    case WM_NOTIFY:
        switch (wParam) {
        case IDC_LANGLIST:
            if ((((NMHDR FAR *)lParam)->code) == LVN_ITEMACTIVATE) {
                if (ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_LANGLIST))) {
                    int index = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LANGLIST), -1, LVNI_SELECTED);
                    if (index != -1) {
                        SendMessage(hDlg, WM_COMMAND, IDOK, 0);
                    }
                }
            }
            return TRUE;
        }
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDOK:
            {
                if (ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_LANGLIST))) {
                    int index = ListView_GetNextItem(GetDlgItem(hDlg, IDC_LANGLIST), -1, LVNI_SELECTED);
                    if (index != -1) {
                        *lang = langGetType(index);
                    }
                }
                EndDialog(hDlg, TRUE);
            }
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
        break;
    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

int langShowDlg(HWND hwnd, int oldLanguage) {
    int lang = oldLanguage;
    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_LANGUAGE), hwnd, langDlgProc, (LPARAM)&lang);
    return lang;
}


///////////////////////////////////////////////////////////////////////////

#define WM_SHOWDSKWIN        (WM_USER + 1248)

#define TIMER_DSKDIALOGSHOW 20

static BOOL CALLBACK dskProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static int show = 0;

    switch (iMsg) {
    case WM_INITDIALOG:
        centerDialog(hDlg, 0);
        return FALSE;

    case WM_SHOWDSKWIN:
        {
            Properties* pProperties = propGetGlobalProperties();
            RECT r1;
            RECT r2;
            int x;
            int y;

            GetWindowRect(GetParent(hDlg), &r1);
            GetWindowRect(hDlg, &r2);

            x = r1.left + (r1.right - r1.left - r2.right + r2.left) / 2;
            y = r1.top  + (r1.bottom - r1.top - r2.bottom + r2.top) / 2;

            SetWindowText(GetDlgItem(hDlg, IDC_DISKIMAGE), 
                          stripPath(*pProperties->media.disks[0].fileNameInZip ? 
                          pProperties->media.disks[0].fileNameInZip : pProperties->media.disks[0].fileName));
            if (!show) {
                enterDialogShow();
                SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                show = 1;
            }
            SetTimer(hDlg, TIMER_DSKDIALOGSHOW, 1000, NULL);
            
            centerDialog(hDlg, 1);
        }
        return TRUE;

    case WM_TIMER:
        ShowWindow(hDlg, SW_HIDE);
        KillTimer(hDlg, TIMER_DSKDIALOGSHOW);
    
        show = 0;
        exitDialogShow();
        return TRUE;
    }
    return FALSE;
}

typedef void* DiskQuickviewWindow;

DiskQuickviewWindow* diskQuickviewWindowCreate(HWND parent) {
    return (DiskQuickviewWindow*)CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DISKIMAGE), parent, dskProc);
}

void diskQuickviewWindowDestroy(DiskQuickviewWindow* dqw)
{
    DestroyWindow((HWND)dqw);
}

void diskQuickviewWindowShow(DiskQuickviewWindow* dqw)
{
    SendMessage((HWND)dqw, WM_SHOWDSKWIN, 0, 0);
}

///////////////////////////////////////////////////////////////////////////

typedef struct {
    char        title[128];
    char        description[128];
    const char* fileList;
    int         fileListCount;
    int         autoReset;
    char        selectFile[512];
    char        zipFileName[512];
    int         selectFileIndex;
    RomType     openRomType;
} ZipFileDlgInfo;


static void updateRomTypeList(HWND hDlg, ZipFileDlgInfo* dlgInfo) {
    char fileName[MAX_PATH];
    int size;
    char* buf = NULL;
    int index;

    index = SendDlgItemMessage(hDlg, IDC_DSKLIST, LB_GETCURSEL, 0, 0);
    SendDlgItemMessage(hDlg, IDC_DSKLIST, LB_GETTEXT, index, (LPARAM)fileName);
    
    if (isFileExtension(fileName, ".rom") || isFileExtension(fileName, ".ri") ||
        isFileExtension(fileName, ".mx1") || isFileExtension(fileName, ".mx2") || 
        isFileExtension(fileName, ".sms") || isFileExtension(fileName, ".col") ||
        isFileExtension(fileName, ".sg") || isFileExtension(fileName, ".sc")) {
        buf = romLoad(dlgInfo->zipFileName, fileName, &size);
    }

    if (buf != NULL) {
        MediaType* mediaType = mediaDbLookupRom(buf, size);
        RomType romType = mediaType != NULL ? mediaDbGetRomType(mediaType) : ROM_UNKNOWN;
        int idx = 0;

        while (opendialog_getromtype(idx) != romType) {
            idx++;
        }

        SendMessage(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), CB_SETCURSEL, idx, 0);

        free(buf);

        dlgInfo->openRomType = romType;

        EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), 1);
        EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTEXT), 1);
    }    
    else {
        dlgInfo->openRomType = ROM_UNKNOWN;
        EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), 0);
        EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTEXT), 0);
        SendMessage(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), CB_SETCURSEL, -1, 0);
    }
}

static BOOL CALLBACK dskZipDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static ZipFileDlgInfo* dlgInfo;

    switch (iMsg) {
    case WM_DESTROY:
        saveDialogPos(hDlg, DLG_ID_ZIPOPEN);
        return 0;

    case WM_INITDIALOG:
        {
            const char* fileList;
            int sel = 0;
            int i;
            
            updateDialogPos(hDlg, DLG_ID_ZIPOPEN, 0, 1);

            dlgInfo = (ZipFileDlgInfo*)lParam;

            dlgInfo->openRomType = ROM_UNKNOWN;

            SetWindowText(hDlg, dlgInfo->title);

            SendMessage(GetDlgItem(hDlg, IDC_DSKLOADTXT), WM_SETTEXT, 0, (LPARAM)dlgInfo->description);
            SetWindowText(GetDlgItem(hDlg, IDC_DSKRESET), langDlgZipReset());
            SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
            SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());
            SetWindowText(GetDlgItem(hDlg, IDC_OPEN_ROMTEXT), langDlgRomType());

            fileList = dlgInfo->fileList;

            for (i = 0; opendialog_getromtype(i) != ROM_UNKNOWN; i++) {
                SendDlgItemMessage(hDlg, IDC_OPEN_ROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(opendialog_getromtype(i)));
                SendDlgItemMessage(hDlg, IDC_ROMTYPE, CB_SETCURSEL, i, 0);
            }
            SendDlgItemMessage(hDlg, IDC_OPEN_ROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(ROM_UNKNOWN));
            EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), 0);
            EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTEXT), 0);

            if (dlgInfo->selectFileIndex != -1) {
                sel = dlgInfo->selectFileIndex;
            }

            for (i = 0; i < dlgInfo->fileListCount; i++) {
                if (dlgInfo->selectFileIndex != -1 && 0 == strcmp(dlgInfo->selectFile, fileList)) {
                    sel = i;
                }
                SendMessage(GetDlgItem(hDlg, IDC_DSKLIST), LB_ADDSTRING, 0, (LPARAM)fileList);
                fileList += strlen(fileList) + 1;
            }

            if (dlgInfo->autoReset == -1) {
                ShowWindow(GetDlgItem(hDlg, IDC_DSKRESET), SW_HIDE);
            }
            else {
                SendMessage(GetDlgItem(hDlg, IDC_DSKRESET), BM_SETCHECK, dlgInfo->autoReset ? BST_CHECKED : BST_UNCHECKED, 0);
            }
            SendDlgItemMessage(hDlg, IDC_DSKLIST, LB_SETCURSEL, sel, 0);
        
            updateRomTypeList(hDlg, dlgInfo);

            return FALSE;
        }

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_OPEN_ROMTYPE:
            if (HIWORD(wParam) == 1 || HIWORD(wParam) == 2) {
                int idx = SendMessage(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), CB_GETCURSEL, 0, 0);

                dlgInfo->openRomType = idx == CB_ERR ? -1 : opendialog_getromtype(idx);
            }
            return 0;
        case IDC_DSKRESET:
            if (dlgInfo->autoReset == 1) {
                SendMessage(GetDlgItem(hDlg, IDC_DSKRESET), BM_SETCHECK, BST_UNCHECKED, 0);
                dlgInfo->autoReset = 0;
            }
            else if (dlgInfo->autoReset == 0) {
                SendMessage(GetDlgItem(hDlg, IDC_DSKRESET), BM_SETCHECK, BST_CHECKED, 1);
                dlgInfo->autoReset = 1;
            }
            break;
        case IDC_DSKLIST:
            switch (HIWORD(wParam)) {
            case LBN_SELCHANGE:
                updateRomTypeList(hDlg, dlgInfo);
                break;
            }

            if (HIWORD(wParam) != LBN_DBLCLK) {
                break;
            }
            // else, fall through
        case IDOK:
            dlgInfo->selectFileIndex = SendMessage(GetDlgItem(hDlg, IDC_DSKLIST), LB_GETCURSEL, 0, 0);
            SendMessage(GetDlgItem(hDlg, IDC_DSKLIST), LB_GETTEXT, dlgInfo->selectFileIndex, (LPARAM)dlgInfo->selectFile);
            EndDialog(hDlg, TRUE);
            return TRUE;
        case IDCANCEL:
            dlgInfo->selectFileIndex = -1;
            dlgInfo->selectFile[0] = '\0';
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
        break;
    case WM_CLOSE:
        dlgInfo->selectFileIndex = -1;
        dlgInfo->selectFile[0] = '\0';
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////

static char* convertTapePos(int tapePos)
{
    static char str[64];
    int pos = tapePos / 128;

    sprintf(str, "%dh %02dm %02ds", pos / 3600, (pos / 60) % 60, pos % 60);
    return str;
}

static void tapeDlgUpdate(HWND hwnd, TapeContent* tc, int tcCount, int showCustomFiles) 
{
    static char* typeNames[] = { "ASCII", "BIN", "BAS", "Custom" };
    int curPos;
    int i;
    int idx = 0;

    ListView_DeleteAllItems(hwnd);

    curPos = tapeGetCurrentPos();

    for (i = 0; i < tcCount; i++) {
        char buffer[64] = {0};
        LV_ITEM lvi = {0};

        if (showCustomFiles || tc[i].type != TAPE_CUSTOM) {
            lvi.mask       = LVIF_TEXT;
            lvi.iItem      = idx;
            lvi.pszText    = buffer;
	        lvi.cchTextMax = 64;
            
            sprintf(buffer, convertTapePos(tc[i].pos));
            ListView_InsertItem(hwnd, &lvi);
            lvi.iSubItem++;
            
            sprintf(buffer, typeNames[tc[i].type]);
            ListView_SetItem(hwnd, &lvi);
            lvi.iSubItem++;
            
            sprintf(buffer, tc[i].fileName);
            ListView_SetItem(hwnd, &lvi);

            if (tc[i].pos <= curPos) {
                SetFocus(hwnd);
                ListView_SetItemState(hwnd, idx, LVIS_SELECTED, LVIS_SELECTED);
            }
            idx++;
        }
    }
}


static BOOL CALLBACK tapePosDlg(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static int currIndex;
    static HWND hwnd;
    static TapeContent* tc;
    static int* showCustomFiles;
    static int tcCount;

    switch (iMsg) {
    case WM_DESTROY:
        saveDialogPos(hDlg, DLG_ID_TAPEPOS);
        return 0;

    case WM_INITDIALOG:
        {
            char buffer[32];
            LV_COLUMN lvc = {0};

            showCustomFiles = (int*)lParam;
         
            updateDialogPos(hDlg, DLG_ID_TAPEPOS, 0, 1);

            currIndex = -1;

            SetWindowText(hDlg, langDlgTapeTitle());

            SendMessage(GetDlgItem(hDlg, IDC_SETTAPEPOSTXT), WM_SETTEXT, 0, (LPARAM)langDlgTapeSetPosText());
            SetWindowText(GetDlgItem(hDlg, IDC_SETTAPECUSTOM), langDlgTapeCustom());
            SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
            SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());

            SendMessage(GetDlgItem(hDlg, IDC_SETTAPECUSTOM), BM_SETCHECK, *showCustomFiles ? BST_CHECKED : BST_UNCHECKED, 0);

            tc = tapeGetContent(&tcCount);
    
            hwnd = GetDlgItem(hDlg, IDC_SETTAPELIST);

            EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);

            ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

            lvc.mask       = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvc.fmt        = LVCFMT_LEFT;
            lvc.cx         = 100;
            lvc.pszText    = buffer;
	        lvc.cchTextMax = 32;

            sprintf(buffer, langDlgTabPosition());
            lvc.cx = 95;
            ListView_InsertColumn(hwnd, 0, &lvc);
            sprintf(buffer, langDlgTabType());
            lvc.cx = 65;
            ListView_InsertColumn(hwnd, 1, &lvc);
            sprintf(buffer, langDlgTabFilename());
            lvc.cx = 105;
            ListView_InsertColumn(hwnd, 2, &lvc);
        }

        tapeDlgUpdate(hwnd, tc, tcCount, *showCustomFiles);
        return FALSE;

    case WM_NOTIFY:
        switch (wParam) {
        case IDC_SETTAPELIST:
            if ((((NMHDR FAR *)lParam)->code) == LVN_ITEMCHANGED) {
                if (ListView_GetSelectedCount(hwnd)) {
                    int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);

                    if (currIndex == -1 && index != -1) {
                        EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
                    }
                    currIndex = index;
                }
                else {
                    if (currIndex != -1) {
                        EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
                    }
                    currIndex = -1;
                }
            }
            if ((((NMHDR FAR *)lParam)->code) == LVN_ITEMACTIVATE) {
                if (ListView_GetSelectedCount(hwnd)) {
                    int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);
                    SendMessage(hDlg, WM_COMMAND, IDOK, 0);
                }
                return TRUE;
            }
        }
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_SETTAPECUSTOM:
            {
                *showCustomFiles = BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_SETTAPECUSTOM), BM_GETCHECK, 0, 0);
                tapeDlgUpdate(GetDlgItem(hDlg, IDC_SETTAPELIST), tc, tcCount, *showCustomFiles);
            }
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return TRUE;

        case IDOK:
            {
                int index = 0;
                int i;

                if (ListView_GetSelectedCount(hwnd)) {
                    currIndex = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);
                }

                for (i = 0; i < tcCount; i++) {
                    if (*showCustomFiles || tc[i].type != TAPE_CUSTOM) {
                        if (currIndex == index) {
                            tapeSetCurrentPos(tc[i].pos);
                        }
                        index++;
                    }
                }
             
                EndDialog(hDlg, TRUE);
            }
            return TRUE;
        }
        return FALSE;

    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

void setTapePosition(HWND parent, Properties* pProperties) {
    if (emulatorGetState() != EMU_STOPPED) {
        emulatorSuspend();
    }
    else {
        tapeSetReadOnly(1);
        boardChangeCassette(0, strlen(pProperties->media.tapes[0].fileName) ? pProperties->media.tapes[0].fileName : NULL, 
                            strlen(pProperties->media.tapes[0].fileNameInZip) ? pProperties->media.tapes[0].fileNameInZip : NULL);
    }

    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTAPEPOS), parent, tapePosDlg, (LPARAM)&pProperties->cassette.showCustomFiles);

    if (emulatorGetState() != EMU_STOPPED) {
        emulatorResume();
    }
    else {
        boardChangeCassette(0, NULL, NULL);
        tapeSetReadOnly(pProperties->cassette.readOnly);
    }
}

///////////////////////////////////////////////////////////////////////////

#define hotkeyEq(hotkey1, hotkey2) (*(DWORD*)&hotkey1 == *(DWORD*)&hotkey2)

static int maxSpeedIsSet = 0;
static int reverseIsSet  = 0;

static void checkKeyDown(Shortcuts* s, ShotcutHotkey key) {
    if (hotkeyEq(key, s->emuSpeedFull)) {
        actionMaxSpeedSet();
        maxSpeedIsSet = 1;
    }
    if (hotkeyEq(key, s->emuPlayReverse)) {
        actionStartPlayReverse();
        reverseIsSet = 1;
    }
}

static void checkKeyUp(Shortcuts* s, ShotcutHotkey key) 
{
    if (maxSpeedIsSet) {
        actionMaxSpeedRelease();
        maxSpeedIsSet = 0;
    }

    if (reverseIsSet) {
        actionStopPlayReverse();
        reverseIsSet = 0;
    }

    if (hotkeyEq(key, s->spritesEnable))                actionToggleSpriteEnable();
    if (hotkeyEq(key, s->fdcTiming))                    actionToggleFdcTiming();
    if (hotkeyEq(key, s->noSpriteLimits))               actionToggleNoSpriteLimits();
    if (hotkeyEq(key, s->msxKeyboardQuirk))             actionToggleMsxKeyboardQuirk();
    if (hotkeyEq(key, s->msxAudioSwitch))               actionToggleMsxAudioSwitch();
    if (hotkeyEq(key, s->frontSwitch))                  actionToggleFrontSwitch();
    if (hotkeyEq(key, s->pauseSwitch))                  actionTogglePauseSwitch();
    if (hotkeyEq(key, s->quit))                         actionQuit();
    if (hotkeyEq(key, s->wavCapture))                   actionToggleWaveCapture();
    if (hotkeyEq(key, s->videoCapLoad))                 actionVideoCaptureLoad();
    if (hotkeyEq(key, s->videoCapPlay))                 actionVideoCapturePlay();
    if (hotkeyEq(key, s->videoCapRec))                  actionVideoCaptureRec();
    if (hotkeyEq(key, s->videoCapStop))                 actionVideoCaptureStop();
    if (hotkeyEq(key, s->videoCapSave))                 actionVideoCaptureSave();
    if (hotkeyEq(key, s->screenCapture))                actionScreenCapture();
    if (hotkeyEq(key, s->screenCaptureUnfilteredSmall)) actionScreenCaptureUnfilteredSmall();
    if (hotkeyEq(key, s->screenCaptureUnfilteredLarge)) actionScreenCaptureUnfilteredLarge();
    if (hotkeyEq(key, s->cpuStateLoad))                 actionLoadState();
    if (hotkeyEq(key, s->cpuStateSave))                 actionSaveState();
    if (hotkeyEq(key, s->cpuStateQuickLoad))            actionQuickLoadState();
    if (hotkeyEq(key, s->cpuStateQuickSave))            actionQuickSaveState();
    if (hotkeyEq(key, s->cpuStateQuickSaveUndo))        actionQuickSaveStateUndo();

    if (hotkeyEq(key, s->cartInsert[0]))                actionCartInsert1();
    if (hotkeyEq(key, s->cartInsert[1]))                actionCartInsert2();
    if (hotkeyEq(key, s->cartSpecialMenu[0]))           actionMenuSpecialCart1(-1, -1);
    if (hotkeyEq(key, s->cartSpecialMenu[1]))           actionMenuSpecialCart2(-1, -1);
    if (hotkeyEq(key, s->cartRemove[0]))                actionCartRemove1();
    if (hotkeyEq(key, s->cartRemove[1]))                actionCartRemove2();
    if (hotkeyEq(key, s->cartAutoReset[0]))             actionToggleCartAutoReset();

    if (hotkeyEq(key, s->diskInsert[0]))                actionDiskInsertA();
    if (hotkeyEq(key, s->diskInsert[1]))                actionDiskInsertB();
    if (hotkeyEq(key, s->diskDirInsert[0]))             actionDiskDirInsertA();
    if (hotkeyEq(key, s->diskDirInsert[1]))             actionDiskDirInsertB();
    if (hotkeyEq(key, s->diskChange[0]))                actionDiskQuickChange();
    if (hotkeyEq(key, s->diskRemove[0]))                actionDiskRemoveA();
    if (hotkeyEq(key, s->diskRemove[1]))                actionDiskRemoveB();
    if (hotkeyEq(key, s->diskAutoReset[0]))             actionToggleDiskAutoReset();

    if (hotkeyEq(key, s->casInsert))                    actionCasInsert();
    if (hotkeyEq(key, s->casRewind))                    actionCasRewind();
    if (hotkeyEq(key, s->casSetPos))                    actionCasSetPosition();
    if (hotkeyEq(key, s->casRemove))                    actionCasRemove();
    if (hotkeyEq(key, s->casToggleReadonly))            actionCasToggleReadonly();
    if (hotkeyEq(key, s->casAutoRewind))                actionToggleCasAutoRewind();
    if (hotkeyEq(key, s->casSave))                      actionCasSave();

    if (hotkeyEq(key, s->prnFormFeed))                  actionPrinterForceFormFeed();
    if (hotkeyEq(key, s->mouseLockToggle))              actionToggleMouseCapture();
    if (hotkeyEq(key, s->emulationRunPause))            actionEmuTogglePause();
    if (hotkeyEq(key, s->emulationStop))                actionEmuStop();
    if (hotkeyEq(key, s->emuSpeedNormal))               actionEmuSpeedNormal();
    if (hotkeyEq(key, s->emuSpeedInc))                  actionEmuSpeedIncrease();
    if (hotkeyEq(key, s->emuSpeedToggle))               actionMaxSpeedToggle();
    if (hotkeyEq(key, s->emuSpeedDec))                  actionEmuSpeedDecrease();
    if (hotkeyEq(key, s->windowSizeSmall))              actionWindowSizeSmall();
    if (hotkeyEq(key, s->windowSizeNormal))             actionWindowSizeNormal();
    if (hotkeyEq(key, s->windowSizeMinimized))          actionWindowSizeMinimized();
    if (hotkeyEq(key, s->windowSizeFullscreen))         actionWindowSizeFullscreen();
    if (hotkeyEq(key, s->windowSizeFullscreenToggle))   actionFullscreenToggle();
    if (hotkeyEq(key, s->resetSoft))                    actionEmuResetSoft();
    if (hotkeyEq(key, s->resetHard))                    actionEmuResetHard();
    if (hotkeyEq(key, s->resetClean))                   actionEmuResetClean();
    if (hotkeyEq(key, s->volumeIncrease))               actionVolumeIncrease();
    if (hotkeyEq(key, s->volumeDecrease))               actionVolumeDecrease();
    if (hotkeyEq(key, s->volumeMute))                   actionMuteToggleMaster();
    if (hotkeyEq(key, s->volumeStereo))                 actionVolumeToggleStereo();
    if (hotkeyEq(key, s->themeSwitch))                  actionNextTheme();
    if (hotkeyEq(key, s->propShowEmulation))            actionPropShowEmulation();
    if (hotkeyEq(key, s->propShowVideo))                actionPropShowVideo();
    if (hotkeyEq(key, s->propShowAudio))                actionPropShowAudio();
    if (hotkeyEq(key, s->propShowSettings))             actionPropShowSettings();
    if (hotkeyEq(key, s->propShowApearance))            actionPropShowApearance();
    if (hotkeyEq(key, s->propShowPorts))                actionPropShowPorts();
    if (hotkeyEq(key, s->propShowEffects))				actionPropShowEffects();
    if (hotkeyEq(key, s->optionsShowLanguage))          actionOptionsShowLanguage();
    if (hotkeyEq(key, s->toolsShowMachineEditor))       actionToolsShowMachineEditor();
    if (hotkeyEq(key, s->toolsShowShorcutEditor))       actionToolsShowShorcutEditor();
    if (hotkeyEq(key, s->toolsShowKeyboardEditor))      actionToolsShowKeyboardEditor();
    if (hotkeyEq(key, s->toolsShowMixer))               actionToolsShowMixer();
    if (hotkeyEq(key, s->toolsShowDebugger))            actionToolsShowDebugger();
    if (hotkeyEq(key, s->toolsShowTrainer))             actionToolsShowTrainer();
    if (hotkeyEq(key, s->helpShowHelp))                 actionHelpShowHelp();
    if (hotkeyEq(key, s->helpShowAbout))                actionHelpShowAbout();
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


#define WM_UPDATE            (WM_USER + 1245)
#define WM_LAUNCHFILE        (WM_USER + 1249)

#define TIMER_STATUSBAR_UPDATE              10
#define TIMER_POLL_INPUT                    11
#define TIMER_POLL_FRAMECOUNT               12
#define TIMER_SCREENUPDATE                  13
#define TIMER_SCREENSHOT                    14
#define TIMER_THEME                         17
#define TIMER_MENUUPDATE                    18
#define TIMER_CLIP_REGION                   19

void  PatchDiskSetBusy(int driveId, int busy);

void updateMenu(int show);

typedef void (*KbdLockFun)(); 

KbdLockFun kbdLockEnable = NULL;
KbdLockFun kbdLockDisable = NULL;

static Properties* pProperties;

typedef struct {
    HWND emuHwnd;
    HWND hwnd;
    HRGN hrgn;
    int      clipAlways;
    int      rgnSize;
    RGNDATA* rgnData;
    int      rgnSizeOrig;
    RGNDATA* rgnDataOrig;
    int      rgnEnable;
    HMENU hMenu;
    int showMenu;
    int trackMenu;
    int showDialog;
    BITMAPINFO bmInfo;
    DiskQuickviewWindow dskWnd;
    
    int active;
    //
    void* bmBitsGDI;
    int frameCount;
    int framesPerSecond;
    char pCurDir[MAX_PATH];
    Video* pVideo;
    int minimized;
    Mixer* mixer;
    int enteringFullscreen;
    Shortcuts* shortcuts;
    DWORD buttonState;

    HANDLE ddrawEvent;
    HANDLE ddrawAckEvent;
    int    diplayUpdated;
    int    diplaySync;
    int    diplayUpdateOnVblank;

    int renderVideo;

    HBITMAP hBitmap;
    HDC hdc;
    ThemePage* themePageActive;
    ThemeCollection** themeList;
    int themeIndex;
    HWND currentHwnd; // Used for arch specific theme events
    POINT currentHwndMouse;
    RECT currentHwndRect;


	//Precalc vars
	int clientWidth;
	int clientHeight;

} WinState;



#define WIDTH  320
#define HEIGHT 240

#define LAUNCH_TEMP_FILE "launch.tmp"

static WinState st;


static void registerFileTypes() {
    registerFileType(".dsk", "blueMSXdsk", "DSK Image", 1);
    registerFileType(".di1", "blueMSXdsk", "DSK Image", 1);
    registerFileType(".di2", "blueMSXdsk", "DSK Image", 1);
    registerFileType(".360", "blueMSXdsk", "DSK Image", 1);
    registerFileType(".720", "blueMSXdsk", "DSK Image", 1);
    registerFileType(".sf7", "blueMSXdsk", "DSK Image", 1);
    registerFileType(".rom", "blueMSXrom", "ROM Image", 2);
    registerFileType(".ri",  "blueMSXrom", "ROM Image", 2);
    registerFileType(".mx1", "blueMSXrom", "ROM Image", 2);
    registerFileType(".mx2", "blueMSXrom", "ROM Image", 2);
    registerFileType(".sms", "blueMSXrom", "Sega ROM Image", 2);
    registerFileType(".col", "blueMSXrom", "Coleco ROM Image", 2);
    registerFileType(".sg",  "blueMSXrom", "Sega ROM Image", 2);
    registerFileType(".sc",  "blueMSXrom", "Sega ROM Image", 2);
    registerFileType(".cas", "blueMSXcas", "CAS Image", 3);
    registerFileType(".sta", "blueMSXsta", "blueMSX State", 4);
    registerFileType(".cap", "blueMSXcap", "blueMSX Video Capture", 4);
}

static void unregisterFileTypes() {
    unregisterFileType(".dsk", "blueMSXdsk", "DSK Image", 1);
    unregisterFileType(".di1", "blueMSXdsk", "DSK Image", 1);
    unregisterFileType(".di2", "blueMSXdsk", "DSK Image", 1);
    unregisterFileType(".360", "blueMSXdsk", "DSK Image", 1);
    unregisterFileType(".720", "blueMSXdsk", "DSK Image", 1);
    unregisterFileType(".sf7", "blueMSXdsk", "DSK Image", 1);
    unregisterFileType(".rom", "blueMSXrom", "ROM Image", 2);
    unregisterFileType(".ri",  "blueMSXrom", "ROM Image", 2);
    unregisterFileType(".mx1", "blueMSXrom", "ROM Image", 2);
    unregisterFileType(".mx2", "blueMSXrom", "ROM Image", 2);
    unregisterFileType(".sms", "blueMSXrom", "Sega ROM Image", 2);
    unregisterFileType(".col", "blueMSXrom", "Coleco ROM Image", 2);
    unregisterFileType(".sg",  "blueMSXrom", "Sega ROM Image", 2);
    unregisterFileType(".sc",  "blueMSXrom", "Sega ROM Image", 2);
    unregisterFileType(".cas", "blueMSXcas", "CAS Image", 3);
    unregisterFileType(".sta", "blueMSXsta", "blueMSX State", 4);
    unregisterFileType(".cap", "blueMSXcap", "blueMSX Video Capture", 4);
}

HWND getMainHwnd()
{
    return st.hwnd;
}

void archShowPropertiesDialog(PropPage  startPane) {
    Properties oldProp = *pProperties;
    int restart = 0;
    int changed;
    int i;

    emulatorSetFrequency(50, NULL);
    enterDialogShow();
    changed = showProperties(pProperties, st.hwnd, startPane, st.mixer, st.pVideo);
    exitDialogShow();
    emulatorSetFrequency(pProperties->emulation.speed, NULL);
    if (!changed) {
        return;
    }

    /* Save properties */
    propSave(pProperties);

    /* Always update video render */
    
    videoUpdateAll(st.pVideo, pProperties);
    
    mediaDbSetDefaultRomType(pProperties->cartridge.defaultType);

    printerIoSetType(pProperties->ports.Lpt.type, pProperties->ports.Lpt.fileName);
    uartIoSetType(pProperties->ports.Com.type, pProperties->ports.Com.fileName);
    midiIoSetMidiOutType(pProperties->sound.MidiOut.type, pProperties->sound.MidiOut.fileName);
    midiIoSetMidiInType(pProperties->sound.MidiIn.type, pProperties->sound.MidiIn.fileName);
    ykIoSetMidiInType(pProperties->sound.YkIn.type, pProperties->sound.YkIn.fileName);
    midiEnableMt32ToGmMapping(pProperties->sound.MidiOut.mt32ToGm);
    midiInSetChannelFilter(pProperties->sound.YkIn.channel);

    /* Update window size only if changed */
    if (pProperties->video.driver != oldProp.video.driver ||
        pProperties->video.fullscreen.width != oldProp.video.fullscreen.width ||
        pProperties->video.fullscreen.height != oldProp.video.fullscreen.height ||
        pProperties->video.fullscreen.bitDepth != oldProp.video.fullscreen.bitDepth ||
        pProperties->video.windowSize != oldProp.video.windowSize ||
        pProperties->video.horizontalStretch != oldProp.video.horizontalStretch ||
        pProperties->video.verticalStretch != oldProp.video.verticalStretch ||
        strcmp(pProperties->settings.themeName, oldProp.settings.themeName))
    {
        archUpdateWindow();
    }

    if (pProperties->cartridge.defaultType != oldProp.cartridge.defaultType) {
        for (i = 0; i < PROP_MAX_CARTS; i++) {
            if (pProperties->media.carts[i].fileName[0]) insertCartridge(pProperties, i, pProperties->media.carts[i].fileName, pProperties->media.carts[i].fileNameInZip, pProperties->media.carts[i].type, -1);
        }
    }
    /* Must restart MSX if Machine configuration changed */
    if (strcmp(oldProp.emulation.machineName, pProperties->emulation.machineName) ||
        oldProp.emulation.syncMethod != pProperties->emulation.syncMethod ||
        oldProp.emulation.vdpSyncMode != pProperties->emulation.vdpSyncMode)
    {
        restart = 1;
    }

    boardSetFdcTimingEnable(pProperties->emulation.enableFdcTiming);
    boardSetNoSpriteLimits(pProperties->emulation.noSpriteLimits);

    /* Update switches */
    switchSetAudio(pProperties->emulation.audioSwitch);
    switchSetFront(pProperties->emulation.frontSwitch);
    switchSetPause(pProperties->emulation.pauseSwitch);
    emulatorSetFrequency(pProperties->emulation.speed, NULL);

    /* Update sound only if changed, Must restart if changed */
    if (oldProp.sound.bufSize              != pProperties->sound.bufSize ||
        oldProp.sound.driver               != pProperties->sound.driver  ||
        oldProp.sound.chip.enableY8950     != pProperties->sound.chip.enableY8950 ||
        oldProp.sound.chip.enableYM2413    != pProperties->sound.chip.enableYM2413 ||
        oldProp.sound.chip.enableMoonsound != pProperties->sound.chip.enableMoonsound ||
        oldProp.sound.stereo               != pProperties->sound.stereo) 
    {
        soundDriverConfig(st.hwnd, pProperties->sound.driver);
        emulatorRestartSound();
    }

    if (oldProp.sound.chip.enableY8950     != pProperties->sound.chip.enableY8950 ||
        oldProp.sound.chip.enableYM2413    != pProperties->sound.chip.enableYM2413 ||
        oldProp.sound.chip.enableMoonsound != pProperties->sound.chip.enableMoonsound)
    {
        boardSetY8950Enable(pProperties->sound.chip.enableY8950);
        boardSetYm2413Enable(pProperties->sound.chip.enableYM2413);
        boardSetMoonsoundEnable(pProperties->sound.chip.enableMoonsound);
        restart = 1;
    }

    if (oldProp.emulation.syncMethod != pProperties->emulation.syncMethod) {
        switch(pProperties->emulation.syncMethod) {
        case P_EMU_SYNCNONE:
            frameBufferSetFrameCount(1);
            break;
        case P_EMU_SYNCTOVBLANK:
        case P_EMU_SYNCTOVBLANKASYNC:
            frameBufferSetFrameCount(4);
            break;
        default:
            frameBufferSetFrameCount(3);
        }
    }

    for (i = 0; i < MIXER_CHANNEL_TYPE_COUNT; i++) {
        mixerSetChannelTypeVolume(st.mixer, i, pProperties->sound.mixerChannel[i].volume);
        mixerSetChannelTypePan(st.mixer, i, pProperties->sound.mixerChannel[i].pan);
        mixerEnableChannelType(st.mixer, i, pProperties->sound.mixerChannel[i].enable);
    }
    
    if(!pProperties->settings.portable) {
        if (pProperties->emulation.registerFileTypes && !oldProp.emulation.registerFileTypes) {
            registerFileTypes();
        }
        else if (!pProperties->emulation.registerFileTypes) {
            unregisterFileTypes();
        }
    }

    if (pProperties->emulation.disableWinKeys && !oldProp.emulation.disableWinKeys) {
        if (kbdLockEnable && emulatorGetState() == EMU_RUNNING && pProperties->emulation.disableWinKeys) {
            kbdLockEnable();
        }
        else if (kbdLockDisable) {
            kbdLockDisable();
        }
    }
    
    if (pProperties->emulation.priorityBoost && !oldProp.emulation.priorityBoost) {
        if (pProperties->emulation.priorityBoost) {
            SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
        }
        else {
            SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
        }
    }

    mixerSetMasterVolume(st.mixer, pProperties->sound.masterVolume);
    mixerEnableMaster(st.mixer, pProperties->sound.masterEnable);

    if (restart) {
        emulatorRestart();
    }

    if (oldProp.settings.disableScreensaver != pProperties->settings.disableScreensaver) {
        POINT pt;
        SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, !pProperties->settings.disableScreensaver, 0, SPIF_SENDWININICHANGE); 
        GetCursorPos(&pt);
        SetCursorPos(pt.x + 1, pt.y);
    }

    updateMenu(0);

    InvalidateRect(st.hwnd, NULL, TRUE);
}


void enterDialogShow() {
    if (pProperties->video.driver != P_VIDEO_DRVGDI) {
        if (emulatorGetState() == EMU_RUNNING) {
            emulatorSuspend();
            DirectXSetGDISurface();
            emulatorResume();
        }
        else {
            DirectXSetGDISurface();
        }
    }
    st.showDialog++;
    if (st.showDialog == 1) {
        emuEnableSynchronousUpdate(0);
        SetTimer(st.hwnd, TIMER_SCREENUPDATE, 20 * (pProperties->video.frameSkip + 1), NULL);
    }
}

void exitDialogShow() {
    st.showDialog--;
    if (st.showDialog == 0) {
        emuEnableSynchronousUpdate(1);
        KillTimer(st.hwnd, TIMER_SCREENUPDATE);
        SetEvent(st.ddrawAckEvent);
    }
}

void updateMenu(int show) {
    int doDelay = show;
    int enableSpecial = 1;
    int emuState = emulatorGetState();

    if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
        show = 1;
    }

    emulatorSuspend();

    if (pProperties->video.driver != P_VIDEO_DRVGDI) {
         if (doDelay) { 
             DirectXSetGDISurface();
         }
    }

    if (boardGetType() != BOARD_MSX) {
        enableSpecial = 0;
    }

    menuUpdate(pProperties, 
               st.shortcuts,
               emuState == EMU_RUNNING, 
               emuState == EMU_STOPPED, 
               mixerIsLogging(st.mixer),
               boardCaptureIsRecording() ? 1 : boardCaptureIsPlaying() ? 2 : 0,
               fileExist(pProperties->filehistory.quicksave, NULL),
               enableSpecial);

    st.showMenu = menuShow(show);

    emulatorResume();

    if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
        mouseEmuActivate(!show);
    }
}

static void setClipRegion(int enable) {
    if (st.rgnEnable == enable) {
        return;
    }
    if ((!enable || st.rgnData == NULL || st.showDialog) && !st.clipAlways) {
        SetWindowRgn(st.hwnd, NULL, TRUE);
        st.rgnEnable = 0;
    }
    else {
        HRGN hrgn = ExtCreateRegion(NULL, st.rgnSize, st.rgnData);
        SetWindowRgn(st.hwnd, hrgn, TRUE);
        st.rgnEnable = 1;
    }
}

static void updateClipRegion() {
    if (st.rgnData != NULL && st.hrgn != NULL) {
        POINT pt;
        RECT r;

        GetCursorPos(&pt);
        GetWindowRect(st.hwnd, &r);
        setClipRegion(!PtInRegion(st.hrgn, pt.x - r.left, pt.y - r.top));
    }
}

static void checkClipRegion() {
    if (st.rgnData != NULL && st.hrgn != NULL && !st.clipAlways) {
        POINT pt;
        RECT r;

        GetCursorPos(&pt);
        GetWindowRect(st.hwnd, &r);
        if (st.rgnEnable == !PtInRegion(st.hrgn, pt.x - r.left, pt.y - r.top)) {
            SetTimer(st.hwnd, TIMER_CLIP_REGION, 500, NULL);
        }
    }
}

static int getZoom() {
    if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN && 
        (pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO || 
        pProperties->video.driver == P_VIDEO_DRVDIRECTX))
    {
        DxDisplayMode* ddm = DirectDrawGetDisplayMode();
        return ddm->width < 640 || ddm->height < 480 ? 1 : 2;
    }
    return pProperties->video.windowSize == P_VIDEO_SIZEX1 ? 1 : 2;
}


void themeSet(char* themeName, int forceMatch) {
    int x  = 0;
    int y  = 0;
    int w  = GetSystemMetrics(SM_CXSCREEN);
    int h  = GetSystemMetrics(SM_CYSCREEN);
    int ex = 0;
    int ey = 0;
    int ew = 0;
    int eh = 0;
    HWND z = 0;
    int index;

    if (themeName == NULL) {
        themeName = "";
    }
    
    index = getThemeListIndex(st.themeList, themeName, forceMatch);
    if (index == -1) {
        return;
    }

    if (st.themePageActive) {
        themePageActivate(st.themePageActive, NULL);
    }

    st.rgnEnable = -1;
    setClipRegion(0);
    st.themeIndex = index;
    strcpy(pProperties->settings.themeName, themeName);
    strcpy(pProperties->settings.themeName, st.themeList[st.themeIndex]->name);

    switch (pProperties->video.windowSize) {
    case P_VIDEO_SIZEX1:
        st.themePageActive = themeGetCurrentPage(st.themeList[st.themeIndex]->little);
        break;
    case P_VIDEO_SIZEX2:
        st.themePageActive = themeGetCurrentPage(st.themeList[st.themeIndex]->normal);
        break;
    case P_VIDEO_SIZEFULLSCREEN:
        st.themePageActive = themeGetCurrentPage(st.themeList[st.themeIndex]->fullscreen);
        break;
    }

    if (st.themePageActive) {
        themePageActivate(st.themePageActive, st.hwnd);
        themePageSetActive(st.themePageActive, NULL, st.active);
    }

    {
        WINDOWPLACEMENT p;
        LONG w, h;

        GetWindowPlacement(st.hwnd, &p);
        w = p.rcNormalPosition.right - p.rcNormalPosition.left;
        h = p.rcNormalPosition.bottom - p.rcNormalPosition.top;

        menuSetInfo(st.themePageActive->menu.color, st.themePageActive->menu.focusColor, 
                    st.themePageActive->menu.textColor, 
                    st.themePageActive->menu.x, st.themePageActive->menu.y,
                    pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN ? w : st.themePageActive->menu.width, 32);
    }

    if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
        x = pProperties->video.windowX;
        y = pProperties->video.windowY;
        w = st.themePageActive->width + 2 * GetSystemMetrics(SM_CXFIXEDFRAME);
        h = st.themePageActive->height + 2 * GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION);
        ex = st.themePageActive->emuWinX;
        ey = st.themePageActive->emuWinY;
        ew = getZoom() * WIDTH;
        eh = getZoom() * HEIGHT;
        z  = HWND_NOTOPMOST;

        if (pProperties->video.windowSize == P_VIDEO_SIZEX2) {
            ew = appConfigGetInt("screen.normal.width", 640);
            eh = appConfigGetInt("screen.normal.height", 480);
        }

        SetWindowPos(st.hwnd, z, x, y, w, h, SWP_SHOWWINDOW);
        SetWindowPos(st.emuHwnd, NULL, ex, ey, ew, eh, SWP_NOZORDER);
    }

    if (st.hBitmap) { DeleteObject(st.hBitmap); st.hBitmap=NULL; }
    if (st.hdc) { ReleaseDC(st.hwnd,st.hdc); st.hdc=NULL; }
    st.hdc=GetDC(st.hwnd);
    if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
        st.hBitmap = CreateCompatibleBitmap(st.hdc, w, h);
    }
    else {
        st.hBitmap = CreateCompatibleBitmap(st.hdc, 640, 480);
    }
    
    if (strcmp(themeName,"Classic")) SetWindowText(st.hwnd, "  blueMSX");

    if (st.rgnData != NULL) {
//        SetWindowRgn(st.hwnd, NULL, TRUE);
        free(st.rgnData);
        st.rgnData = NULL;
    }

    if (st.hrgn != NULL) {
        DeleteObject(st.hrgn);
        st.hrgn = NULL;
    }

    if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
        st.rgnEnable = -1;
        setClipRegion(0);
    } 
    else {
        int clipCount = st.themePageActive->clipPoint.count;
        st.clipAlways = st.themePageActive->noFrame;

        if (clipCount > 0 || st.clipAlways) {
            int i;
            HRGN hrgn;
            POINT pt[512];
            int dx = GetSystemMetrics(SM_CXFIXEDFRAME);
            int dy = GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION);

            if (clipCount == 0) {
                pt[0].x = 0 + dx;
                pt[0].y = 0 + dy;
                pt[1].x = st.themePageActive->width + dx;
                pt[1].y = 0 + dy;
                pt[2].x = st.themePageActive->width + dx;
                pt[2].y = st.themePageActive->height + dy;
                pt[3].x = 0 + dx;
                pt[3].y = st.themePageActive->height + dy;
                clipCount = 4;
            }
            else {
                for (i = 0; i < clipCount; i++) {
                    ClipPoint cp = st.themePageActive->clipPoint.list[i];
                    pt[i].x = cp.x + dx;
                    pt[i].y = cp.y + dy;
                }
            }

            hrgn = CreatePolygonRgn(pt, clipCount, WINDING);
            st.rgnSize = 0;
            if (hrgn != NULL) {
                st.rgnSize = GetRegionData(hrgn, 0, NULL);
                if (st.rgnSize > 0) {
                    st.rgnData = malloc(st.rgnSize);
                    st.rgnSize = GetRegionData(hrgn, st.rgnSize, st.rgnData);
                    if (st.rgnSize == 0) {
                        free(st.rgnData);
                        st.rgnData = NULL;
                    }
                }
                if (st.rgnSize == 0) {
                    st.rgnData = NULL;
                }
                else {
                    st.hrgn = CreateRectRgn(0, 0, w, h);
                    CombineRgn(st.hrgn, st.hrgn, hrgn, RGN_XOR);
                }
                DeleteObject(hrgn);
            }
            else {
                SetWindowRgn(st.hwnd, NULL, TRUE);
            }
        }

        st.rgnEnable = -1;
        setClipRegion(clipCount > 0);

        if (st.rgnData == NULL) {
            KillTimer(st.hwnd, TIMER_CLIP_REGION);
        }
        else {
            SetTimer(st.hwnd, TIMER_CLIP_REGION, 500, NULL);
        }
    }

    InvalidateRect(st.hwnd, NULL, TRUE);
}

void archUpdateWindow() {
    int zoom = getZoom();

    st.enteringFullscreen = 1;
    emulatorSuspend();

    if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D)
        D3DExitFullscreenMode();
    else
        DirectXExitFullscreenMode();

    if (st.bmBitsGDI != NULL) {
        free(st.bmBitsGDI);
        st.bmBitsGDI = NULL;
    }

    if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
        if (pProperties->video.driver == P_VIDEO_DRVGDI) {
            pProperties->video.windowSize = P_VIDEO_SIZEX2;
        }
        else {
            int rv;
            SetWindowLong(st.hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE);

            if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D)
                rv = D3DEnterFullscreenMode(st.emuHwnd, 
                                                pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO, 
                                                pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO);
            else
                rv = DirectXEnterFullscreenMode(st.emuHwnd, 
                                                pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO, 
                                                pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO);

            if (rv != DXE_OK) {
                MessageBox(NULL, langErrorEnterFullscreen(), langErrorTitle(), MB_OK);
                if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D)
                    D3DExitFullscreenMode();
                else
                    DirectXExitFullscreenMode();
                pProperties->video.windowSize = P_VIDEO_SIZEX2;
            }
        }
    }

    if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
        if (GetWindowLong(st.hwnd, GWL_STYLE) & WS_POPUP) {
            mouseEmuActivate(1);
            SetWindowLong(st.hwnd, GWL_STYLE, WS_OVERLAPPED | WS_CLIPCHILDREN | WS_BORDER | WS_DLGFRAME | 
                                WS_SYSMENU | WS_MINIMIZEBOX | (pProperties->video.maximizeIsFullscreen?WS_MAXIMIZEBOX:0));
        }

        if (pProperties->video.driver != P_VIDEO_DRVGDI) {
            int rv;

            if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D)
                rv = D3DEnterWindowedMode(st.emuHwnd, zoom * WIDTH, zoom * HEIGHT, 
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO, 
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO);
            else
                rv = DirectXEnterWindowedMode(st.emuHwnd, zoom * WIDTH, zoom * HEIGHT, 
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO, 
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO);
            if (rv != DXE_OK) {
                MessageBox(NULL, langErrorDirectXFailed(), langErrorTitle(), MB_OK);
                pProperties->video.driver = P_VIDEO_DRVGDI;
            }
        }
    }

    if (st.rgnData != NULL) {
//        SetWindowRgn(st.hwnd, NULL, TRUE);
        free(st.rgnData);
        st.rgnData = NULL;
    }
    st.rgnEnable = -1;
    setClipRegion(0);
    themeSet(pProperties->settings.themeName, 1);
    updateMenu(0);

    {
        RECT r = { 0, 0, zoom * WIDTH, zoom * HEIGHT };
        RECT d = { 0, 0, zoom * WIDTH, zoom * HEIGHT };
        if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
            WINDOWPLACEMENT p;
            GetWindowPlacement(st.hwnd, &p);
            r.right  = p.rcNormalPosition.right - p.rcNormalPosition.left;
            r.bottom = p.rcNormalPosition.bottom - p.rcNormalPosition.top;
        }
        if (!pProperties->video.horizontalStretch) {
            d.left  += zoom * (320 - 272) / 2;
            d.right -= zoom * (320 - 272) / 2;
        }
        
        if (pProperties->video.windowSize == P_VIDEO_SIZEX2) {
            r.right = appConfigGetInt("screen.normal.width", 640);
            r.bottom = appConfigGetInt("screen.normal.height", 480);
        }

        mouseEmuSetCaptureInfo(&r, &d);
    }

    emulatorResume();

    st.enteringFullscreen = 0;

    InvalidateRect(NULL, NULL, TRUE);
}



static void emuWindowDraw(int onlyOnVblank)
{      
    static void* lock = NULL;
    int rv = 0;

    if (lock == NULL) {
        lock = archSemaphoreCreate(1);
    }

    archSemaphoreWait(lock, -1);

    if (!st.enteringFullscreen && 
        (pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO || 
        (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D) || 
        (pProperties->video.driver == P_VIDEO_DRVDIRECTX)))
    {
#define PRINT_RENDERING_TIME 0
#if PRINT_RENDERING_TIME
        LARGE_INTEGER	iCurrentTime;
        double fStartTime;
        if(QueryPerformanceCounter(&iCurrentTime))
            fStartTime = (long double) iCurrentTime.QuadPart;
#endif

        st.diplaySync |= onlyOnVblank;


        if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D) 
            rv = D3DUpdateSurface(st.emuHwnd, st.pVideo, st.diplaySync, &pProperties->video.d3d);
        else
            rv = DirectXUpdateSurface(st.pVideo, 
                                      st.showMenu | st.showDialog || emulatorGetState() != EMU_RUNNING, 
                                      0, 0, getZoom(), 
                                      pProperties->video.horizontalStretch, 
                                      pProperties->video.verticalStretch,
                                      st.diplaySync,
                                      pProperties->video.windowSize == P_VIDEO_SIZEX2);

#if PRINT_RENDERING_TIME
        {
            char output[1024];
            double t;
            LARGE_INTEGER	iFrequency;
            QueryPerformanceFrequency(&iFrequency);
            QueryPerformanceCounter(&iCurrentTime);
            t = (double) (iCurrentTime.QuadPart - fStartTime) / (double) iFrequency.QuadPart;
            sprintf(output, "Rendering time: %fms\n", t*1000.0f);
            OutputDebugString(output);
        }
#endif
        st.diplaySync = 0;
        if (rv) {
            st.frameCount++;
        }
    }
    st.diplayUpdated = rv;

    archSemaphoreSignal(lock);
}

void* createScreenShot(int large, int* bitmapSize, int png)
{
    void* bitmap = NULL;

    int zoom = large ? 2 : 1;

    DWORD* bmBitsDst = malloc(zoom * zoom * WIDTH * HEIGHT * sizeof(UInt32));
    VideoPalMode palMode      = st.pVideo->palMode;
    int scanLinesEnable       = st.pVideo->scanLinesEnable;
    int colorSaturationEnable = st.pVideo->colorSaturationEnable;
    
    FrameBuffer* frameBuffer = frameBufferGetViewFrame();

    if (frameBuffer == NULL || frameBuffer->maxWidth <= 0 || frameBuffer->lines <= 0) {
        return NULL;
    }

    st.pVideo->palMode = VIDEO_PAL_FAST;
    st.pVideo->scanLinesEnable = 0;
    st.pVideo->colorSaturationEnable = 0;

    if (png) {
        videoRender(st.pVideo, frameBufferGetViewFrame(), 32, zoom, 
                    bmBitsDst, 0, zoom * WIDTH * sizeof(DWORD), 0);
    }
    else {
        videoRender(st.pVideo, frameBufferGetViewFrame(), 32, zoom, 
                    bmBitsDst + (zoom * HEIGHT - 1) * zoom * WIDTH, 
                    0, -1 * zoom * WIDTH * sizeof(DWORD), 0);
    }

    st.pVideo->palMode               = palMode;
    st.pVideo->scanLinesEnable       = scanLinesEnable;
    st.pVideo->colorSaturationEnable = colorSaturationEnable;

    if (bitmapSize != NULL) {
        bitmap = ScreenShot2(bmBitsDst, 320 * zoom, frameBuffer->maxWidth * zoom, 240 * zoom, bitmapSize, png);
    }
    else {
        ScreenShot3(pProperties, bmBitsDst, 320 * zoom, frameBuffer->maxWidth * zoom, 240 * zoom, png);
    }

    free(bmBitsDst);

    return bitmap;
}

static LRESULT CALLBACK emuWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
    case WM_CREATE:
        return 0;

    case WM_SETCURSOR:
        return mouseEmuSetCursor();

    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        return SendMessage(GetParent(hwnd), iMsg, wParam, lParam);

	case WM_WINDOWPOSCHANGED :
		if ( pProperties->video.driver == P_VIDEO_DRVGDI  )
		{	
			int zoom = getZoom();
			RECT r;

			GetClientRect ( hwnd, &r );

			st.clientWidth = r.right - r.left;
			st.clientHeight = r.bottom - r.top;
			
            st.bmInfo.bmiHeader.biWidth    = zoom * WIDTH;
            st.bmInfo.bmiHeader.biHeight   = zoom * HEIGHT;
            st.bmInfo.bmiHeader.biBitCount = 32;
		}
    case WM_PAINT:
        if (pProperties->video.driver == P_VIDEO_DRVGDI && emulatorGetState() != EMU_STOPPED) 
		{
            PAINTSTRUCT ps;
            FrameBuffer* frameBuffer;
            int borderWidth;
            HDC hdc;   
            int zoom = getZoom();

            if (st.bmBitsGDI == 0) {
                st.bmBitsGDI = malloc(4096 * 4096 * sizeof(UInt32));
            }

            frameBuffer = frameBufferGetViewFrame();
            if (frameBuffer == NULL) {
                frameBuffer = frameBufferGetWhiteNoiseFrame();
            }
        
			// clear borders
            borderWidth = (320 - frameBuffer->maxWidth) * zoom / 2;

            if (borderWidth > 0) {
                DWORD* ptr  = st.bmBitsGDI;
                int y;
                for (y = 0; y < zoom * HEIGHT; y++) {
                    memset(ptr, 0, borderWidth * sizeof(DWORD));
                    ptr += zoom * WIDTH;
                    memset(ptr - borderWidth, 0, borderWidth * sizeof(DWORD));
                }
            }

			// render image
            videoRender(st.pVideo, 
						frameBuffer, 
						32, 
						zoom, 
                        (char*)st.bmBitsGDI + borderWidth * sizeof(DWORD) + (zoom * HEIGHT - 1) * zoom * WIDTH * sizeof(DWORD), 
                        0, -1 * zoom * WIDTH * sizeof(DWORD), 0);

			// Beginpaint moved because it's only needed to output the framebuffer
			hdc = BeginPaint ( hwnd, &ps );

			StretchDIBits(hdc,
						  0, 0, 
						  st.clientWidth, st.clientHeight, 
						  0, 0, 
						  zoom * WIDTH, zoom * HEIGHT, 
						  st.bmBitsGDI, 
                          &st.bmInfo, 
						  DIB_RGB_COLORS, 
						  SRCCOPY);

            EndPaint(hwnd, &ps);
            st.frameCount++;

        }
        else if (pProperties->video.driver != P_VIDEO_DRVGDI && 
                 (emulatorGetState() == EMU_PAUSED || emulatorGetState() == EMU_SUSPENDED)) {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
		    EndPaint(hwnd, &ps);
            SetEvent(st.ddrawEvent);
        }
        else {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);    
}

static LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
    case WM_CREATE:
        SetTimer(hwnd, TIMER_STATUSBAR_UPDATE, 100, NULL);
        SetTimer(hwnd, TIMER_POLL_INPUT, 50, NULL);
        SetTimer(hwnd, TIMER_POLL_FRAMECOUNT, 1000, NULL);
        DragAcceptFiles(hwnd, TRUE);
        return 0;

    case WM_DROPFILES:
        {
            char fname[MAX_PATH];
            HDROP hDrop;
            DWORD fa;

            hDrop = (HDROP)wParam;
            DragQueryFile(hDrop, 0, fname, 512);
            DragFinish(hDrop);
            
		    fa = GetFileAttributes(fname);
            if (fa & FILE_ATTRIBUTE_DIRECTORY) {
                insertDiskette(pProperties, 0, fname, NULL, 0);
            }
            else {
                tryLaunchUnknownFile(pProperties, fname, 0);
            }
        }
        return 0;

    case WM_LAUNCHFILE:
        {
            char fileName[512];
            FILE* file = fopen(LAUNCH_TEMP_FILE, "r");
            if (file != NULL) {
                int size = fread(fileName, 1, 512, file);
                fclose(file);
                if (size > 0) {
                    char* argument;

                    fileName[size] = 0;                    

                    argument = extractToken(fileName, 0);
                    if (*argument) {
                        int i;

                        emulatorStop();

                        for (i = 0; i < PROP_MAX_CARTS; i++) {
                            pProperties->media.carts[i].fileName[0] = 0;
                            pProperties->media.carts[i].fileNameInZip[0] = 0;
                            pProperties->media.carts[i].type = ROM_UNKNOWN;
                            updateExtendedRomName(i, pProperties->media.carts[i].fileName, pProperties->media.carts[i].fileNameInZip);
                        }

                        for (i = 0; i < PROP_MAX_DISKS; i++) {
                            pProperties->media.disks[i].fileName[0] = 0;
                            pProperties->media.disks[i].fileNameInZip[0] = 0;
                            updateExtendedDiskName(i, pProperties->media.disks[i].fileName, pProperties->media.disks[i].fileNameInZip);
                        }

                        for (i = 0; i < PROP_MAX_TAPES; i++) {
                            pProperties->media.tapes[i].fileName[0] = 0;
                            pProperties->media.tapes[i].fileNameInZip[0] = 0;
                            updateExtendedCasName(i, pProperties->media.tapes[i].fileName, pProperties->media.tapes[i].fileNameInZip);
                        }

                        tryLaunchUnknownFile(pProperties, argument, 1);
                    }

                    SetActiveWindow(hwnd);
                }
            }
        }
        return 0;

    case WM_COMMAND:
        if ( menuCommand(pProperties, LOWORD(wParam))) {
            updateMenu(0);
        }
        break;

    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        {
            ShotcutHotkey key;
            key.type = HOTKEY_TYPE_KEYBOARD;
            key.mods = keyboardGetModifiers();
            key.key  = wParam & 0xff;
            checkKeyDown(st.shortcuts, key);
        }
        return 0;

    case WM_SYSKEYUP:
    case WM_KEYUP:
        {
            ShotcutHotkey key;
            key.type = HOTKEY_TYPE_KEYBOARD;
            key.mods = keyboardGetModifiers();
            key.key  = wParam & 0xff;
            checkKeyUp(st.shortcuts, key);
        }
        return 0;

    case WM_CHAR:
    case WM_SYSCHAR:
        return 0;

    case WM_SYSCOMMAND:
        switch(wParam) {
        case SC_MAXIMIZE:
            vdpSetDisplayEnable(1);
            st.minimized = 0;
            if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
                pProperties->video.windowSize = P_VIDEO_SIZEFULLSCREEN;
                archUpdateWindow();
            }
            return 0;
        case SC_MINIMIZE:
            vdpSetDisplayEnable(0);
            st.minimized = 1;
//            emulatorSuspend();
            break;
        case SC_RESTORE:
            vdpSetDisplayEnable(1);
            st.minimized = 0;
//            emulatorResume();
            break;
        }
        break;

    case WM_NCLBUTTONDBLCLK:
        if (wParam == HTCAPTION && pProperties->video.maximizeIsFullscreen) {
            if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
                pProperties->video.windowSize = P_VIDEO_SIZEFULLSCREEN;
                archUpdateWindow();
            }
            return 0;
        }
        break;

    case WM_ENTERSIZEMOVE:
        emulatorSuspend();
        mouseEmuActivate(0);
        st.showDialog++;
        break;

    case WM_EXITSIZEMOVE:
        st.showDialog--;
        emulatorResume();
        mouseEmuActivate(1);
        break;

    case WM_ENTERMENULOOP:
        emulatorSuspend();
        st.trackMenu = 1;
        SetTimer(st.hwnd, TIMER_MENUUPDATE, 250, NULL);
        mouseEmuActivate(0);
        return 0;

    case WM_EXITMENULOOP:
        {
            int moreMenu;
            
            if (!st.minimized) {
                emuWindowDraw(0);
            }
            moreMenu = menuExitMenuLoop();
            if (!moreMenu) {
                mouseEmuActivate(1);
                //KillTimer(st.hwnd, TIMER_MENUUPDATE);
                emulatorResume();
                updateMenu(0);
                if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
                    PostMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
                    PostMessage(hwnd, WM_LBUTTONUP, 0, 0);
                }
                st.trackMenu = 0;
            }
        }
        return 0;

    case WM_MOVE:
        if (!st.enteringFullscreen && pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
            RECT r;
            GetWindowRect(hwnd, &r);
            pProperties->video.windowX = r.left;
            pProperties->video.windowY = r.top;
        }

    case WM_DISPLAYCHANGE:
        if (pProperties->video.driver != P_VIDEO_DRVGDI) {
            int zoom = getZoom();
            if (st.enteringFullscreen) {
                if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D)
                    D3DUpdateWindowedMode(st.emuHwnd, zoom * WIDTH, zoom * HEIGHT,
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO, 
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO);
                else
                    DirectXUpdateWindowedMode(st.emuHwnd, zoom * WIDTH, zoom * HEIGHT,
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO, 
                                              pProperties->video.driver == P_VIDEO_DRVDIRECTX_VIDEO);
            }
        }
        break;

    case WM_GETMINMAXINFO:
        {
            LRESULT rv = DefWindowProc(hwnd, iMsg, wParam, lParam);
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            mmi->ptMaxSize.x      = 2048;
            mmi->ptMaxSize.y      = 2048;
            mmi->ptMaxTrackSize.x = 2048;
            mmi->ptMaxTrackSize.y = 2048;
            return 0;
        }

    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        break;
        
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE) {
            if (kbdLockDisable != NULL) {
                kbdLockDisable(0);
            }
            else {
                inputReset(hwnd);
            }
            mouseEmuActivate(0);
            actionMaxSpeedRelease();
        }
        else {
            if (kbdLockEnable != NULL && emulatorGetState() == EMU_RUNNING && pProperties->emulation.disableWinKeys) {
                kbdLockEnable();
            }
            mouseEmuActivate(1);
        }
        if (st.themePageActive) {
            HDC hdc = GetDC(hwnd);
            st.active = LOWORD(wParam) != WA_INACTIVE;
            themePageSetActive(st.themePageActive, hdc, st.active);
            ReleaseDC(hwnd, hdc);
        }
        break;

    case WM_NCMOUSEMOVE:
        if (st.themePageActive) {
            checkClipRegion();
        }
        break;

    case WM_MOUSEMOVE:
        if (st.themePageActive) {
            HDC hdc = GetDC(hwnd);
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);
            themePageMouseMove(st.themePageActive, hdc, pt.x, pt.y);
            ReleaseDC(hwnd, hdc);
            checkClipRegion();
        }
        if (pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
            if (HIWORD(lParam) < 2) {
                if (!st.showMenu) {
                    updateMenu(1);
                }
            }
        }
        archWindowMove();
        SetTimer(hwnd, TIMER_THEME, 250, NULL);

        break;

    case WM_LBUTTONDOWN:
        {
            POINT pt;
            RECT r;

            GetCursorPos(&pt);
            GetWindowRect(st.emuHwnd, &r);
            if (!IsWindowVisible(st.emuHwnd) || !PtInRect(&r, pt)) {
                SetCapture(hwnd);
                st.currentHwnd = hwnd;

                if (st.themePageActive) {
                    HDC hdc = GetDC(hwnd);
                    POINT pt;
                    GetCursorPos(&pt);
                    ScreenToClient(hwnd, &pt);
                    themePageMouseButtonDown(st.themePageActive, hdc, pt.x, pt.y);
                    ReleaseDC(hwnd, hdc);
                }
                if (st.showMenu && pProperties->video.windowSize == P_VIDEO_SIZEFULLSCREEN) {
                    updateMenu(0);
                }
            }
        }
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        if (st.themePageActive) {
            HDC hdc = GetDC(hwnd);
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);
            themePageMouseButtonUp(st.themePageActive, hdc, pt.x, pt.y);
            ReleaseDC(hwnd, hdc);
            st.currentHwnd = NULL;
        }

    case WM_ERASEBKGND:
        return 1;

    case WM_TIMER:
        switch (wParam) {
        case TIMER_CLIP_REGION:
            updateClipRegion();
            break;

        case TIMER_STATUSBAR_UPDATE:
            if (!st.minimized) {
                static UInt32 resetCnt = 0;
                HDC hdc = GetDC(hwnd);

                if (emulatorGetState() == EMU_RUNNING) {
                    if ((resetCnt++ & 0x3f) == 0) {
                        mixerIsChannelTypeActive(st.mixer, MIXER_CHANNEL_MOONSOUND, 1);
                        mixerIsChannelTypeActive(st.mixer, MIXER_CHANNEL_YAMAHA_SFG, 1);
                        mixerIsChannelTypeActive(st.mixer, MIXER_CHANNEL_MSXAUDIO, 1);
                        mixerIsChannelTypeActive(st.mixer, MIXER_CHANNEL_MSXMUSIC, 1);
                        mixerIsChannelTypeActive(st.mixer, MIXER_CHANNEL_SCC, 1);
                    }
                }

                if (!strcmp(pProperties->settings.themeName,"Classic")) themeClassicTitlebarUpdate(hwnd);
                themePageUpdate(st.themePageActive, hdc);
                ReleaseDC(hwnd, hdc);

                PatchDiskSetBusy(0, 0);
                PatchDiskSetBusy(1, 0);
                ledSetCas(0);

            }
            break;

        case TIMER_POLL_INPUT:
            {
                DWORD buttonState;
                DWORD buttons;
                ShotcutHotkey key;
                int i;
                HWND hwndFocus = GetFocus();

                keyboardSetFocus(1, hwndFocus == st.hwnd || hwndFocus == st.emuHwnd);
                
                if (emulatorGetState() != EMU_RUNNING) {
                    archPollInput();
                }

                buttonState = joystickGetButtonState();
                
                buttons = buttonState & ~st.buttonState;

                for (i = 1; buttons != 0; i++, buttons >>= 1) {
                    if (buttons & 1) {
                        key.type = HOTKEY_TYPE_JOYSTICK;
                        key.mods = 0;
                        key.key  = i;
                        checkKeyDown(st.shortcuts, key);
                    }
                }
                
                buttons = ~buttonState & st.buttonState;
                for (i = 1; buttons != 0; i++, buttons >>= 1) {
                    if (buttons & 1) {
                        key.type = HOTKEY_TYPE_JOYSTICK;
                        key.mods = 0;
                        key.key  = i;
                        checkKeyUp(st.shortcuts, key);
                    }
                }

                st.buttonState = buttonState;

                mouseEmuSetRunState(emulatorGetState() == EMU_RUNNING);
            }
            break;

        case TIMER_POLL_FRAMECOUNT:
            st.framesPerSecond = st.frameCount;
            st.frameCount = 0;
            break;

        case TIMER_MENUUPDATE:
            if (!st.minimized) {
                emuWindowDraw(0);
            }
            if (!st.trackMenu) {
                KillTimer(st.hwnd, TIMER_MENUUPDATE);
            }
            break;

        case TIMER_SCREENUPDATE:
            if (emulatorGetState() != EMU_STOPPED) {
                DWORD rv = WaitForSingleObject(st.ddrawEvent, 0);
                if (rv == WAIT_OBJECT_0) {
                    if (!st.minimized) {
                        emuWindowDraw(0);
                    }
                    SetEvent(st.ddrawAckEvent);
                }
            }
            break;

        case TIMER_SCREENSHOT:
            {
                RECT r;
                GetWindowRect(st.emuHwnd, &r);
                KillTimer(hwnd, TIMER_SCREENSHOT);
			    ScreenShot(pProperties, st.emuHwnd, r.right - r.left, r.bottom - r.top, 0, 0, pProperties->settings.usePngScreenshots);
            }
            break;
            
        case TIMER_THEME:        
            if (!st.minimized) {
                POINT pt;
                RECT r;
                HDC hdc;

                GetCursorPos(&pt);
                GetWindowRect(hwnd, &r);

                if (!PtInRect(&r, pt)) {
                    KillTimer(hwnd, TIMER_THEME);
                }

                ScreenToClient(hwnd, &pt);

                hdc = GetDC(hwnd);
                themePageMouseMove(st.themePageActive, hdc, pt.x, pt.y);
                ReleaseDC(hwnd, hdc);
            }
            break;
        }
        return 0;

    case WM_UPDATE:
        frameBufferFlipViewFrame(0);
        InvalidateRect(st.emuHwnd, NULL, TRUE);
        return 0;

    case WM_INPUTLANGCHANGE:
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (!st.minimized) {
                HDC hMemDC = CreateCompatibleDC(hdc);
                HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, st.hBitmap);

                 if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
					// the theme is only drawn when not in fullscreen mode
					themePageDraw(st.themePageActive, hMemDC, NULL);
                    BitBlt(hdc, 0, 0, st.themePageActive->width, st.themePageActive->height, hMemDC, 0, 0, SRCCOPY);
                }
                else {
                    RECT r;
                    GetClientRect(hwnd, &r);
					themePageDraw(st.themePageActive, hMemDC, NULL);
                    StretchBlt(hdc, 0, 0, r.right, r.bottom, 
                               hMemDC, 0, 0, st.themePageActive->width, st.themePageActive->height, SRCCOPY);
                }

                SelectObject(hMemDC, hBitmap);
                DeleteDC(hMemDC);                
            }
            EndPaint(hwnd, &ps);
        }
        return 0;
        
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        
        if (pProperties->emulation.ejectMediaOnExit) {
        	actionCartRemove1(); actionCartRemove2();
        	actionDiskRemoveA(); actionDiskRemoveB();
        	actionCasRemove();
        	actionHarddiskRemoveAll();
        }
        
        emulatorStop();
        toolUnLoadAll();
        if (pProperties->video.windowSize != P_VIDEO_SIZEFULLSCREEN) {
            RECT r;
            
            GetWindowRect(hwnd, &r);
            pProperties->video.windowX = r.left;
            pProperties->video.windowY = r.top;
        }
        st.enteringFullscreen = 1;
        if (pProperties->video.driver == P_VIDEO_DRVDIRECTX_D3D)
            D3DExitFullscreenMode();
        else
            DirectXExitFullscreenMode();
        PostQuitMessage(0);
        inputDestroy();
        return 0;

    case WM_DEVICECHANGE:
        {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

            switch(wParam) {
            case DBT_DEVICEARRIVAL:
            case DBT_DEVICEREMOVECOMPLETE:
                if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

                    if (lpdbv->dbcv_flags & DBTF_MEDIA) {
                        cdromOnMediaChange(lpdbv->dbcv_unitmask);
                    }
                }
                break;
            }
        }
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////

static int videoTimeAverage = 0;
static int videoTimeTotal = 0;
static int videoTimeIdle = 1;

UInt32 videoGetCpuUsage() {
    videoTimeAverage = 1000 * (videoTimeTotal - videoTimeIdle) / videoTimeTotal;

    if (videoTimeAverage >= 1000) {
        videoTimeAverage = 0;
    }

    videoTimeIdle  = 0;
    videoTimeTotal = 1;

    return videoTimeAverage;
}

void updateEmuWindow() {
    if (emulatorGetState() != EMU_STOPPED) {
        SetEvent(st.ddrawEvent);
    }
}

int setDefaultPath() {   
    char buffer[512];  
    char buffer2[512];
    char rootDir[512];
    int readOnlyDir;
    DWORD dirattr; 
    FILE* file;
    char* ptr;

    // Set current directory to where the exe is located
    GetModuleFileName((HINSTANCE)GetModuleHandle(NULL), buffer, 512);
    ptr = (char*)stripPath(buffer);
    *ptr = 0;
    chdir(buffer);

    GetCurrentDirectory(MAX_PATH - 1, st.pCurDir);

    readOnlyDir = 0;

    // attribute check first 
    // ( check blueMSX run on the CD-ROM ) 
    dirattr = GetFileAttributes(buffer); 
    if((dirattr == -1) || (dirattr & FILE_ATTRIBUTE_READONLY)){ 
        readOnlyDir = 1; 
    }else{ 
        // Test if current directory is read only 
        // ( check removable disk write protected ) 
        // note: this check with Dialog warning 
        file = fopen("wrtest", "w"); 
        readOnlyDir = file == NULL; 
        if (file != NULL) { 
            fclose(file); 
            DeleteFile("wrtest"); // Delete test file 
        } 
    }

    if (!readOnlyDir) {
        GetCurrentDirectory(MAX_PATH - 1, rootDir); 
    }
    else {
        // Get user's My Documents folder 
        LPITEMIDLIST Root; 
        SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &Root); 
        SHGetPathFromIDList(Root, buffer2); 

        chdir(buffer2); 
        sprintf(buffer, "%s\\blueMSX Temporary Files", buffer2); 
        mkdir(buffer); 
        chdir(buffer); 

        GetCurrentDirectory(MAX_PATH - 1, rootDir); 
        SetCurrentDirectory(st.pCurDir);
    }

    // Set up temp directories
    propertiesSetDirectory(st.pCurDir, rootDir);

    sprintf(buffer, "%s\\Machines", rootDir);
	machineSetDirectory(buffer);

    sprintf(buffer, "%s\\Audio Capture", rootDir);
    mkdir(buffer);
    actionSetAudioCaptureSetDirectory(buffer, "");

    sprintf(buffer, "%s\\Video Capture", rootDir);
    mkdir(buffer);
    actionSetVideoCaptureSetDirectory(buffer, "");

    sprintf(buffer, "%s\\QuickSave", rootDir);
    mkdir(buffer);
    actionSetQuickSaveSetDirectory(buffer, "");

    sprintf(buffer, "%s\\SRAM", rootDir);
    mkdir(buffer);
    boardSetDirectory(buffer);

    sprintf(buffer, "%s\\Keyboard Config", rootDir);
    mkdir(buffer);
    keyboardSetDirectory(buffer);

    sprintf(buffer, "%s\\Screenshots", rootDir);
    mkdir(buffer);
    screenshotSetDirectory(buffer, "");

    sprintf(buffer, "%s\\Casinfo", rootDir);
    mkdir(buffer);
    tapeSetDirectory(buffer, "");

    sprintf(buffer, "%s\\Databases", rootDir);
    mkdir(buffer);
    mediaDbLoad(buffer);

    mediaDbCreateRomdb();

    mediaDbCreateDiskdb();

    mediaDbCreateCasdb();

    return readOnlyDir;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

int emuCheckLanguageArgument(char* cmdLine, int defaultLang){
    int i;
    int lang;
    char* argument;
    
    for (i = 0; argument = extractToken(cmdLine, i); i++) {
        if (strcmp(argument, "/language") == 0) {
            argument = extractToken(cmdLine, ++i);
            if (argument == NULL) return defaultLang;
            lang = langFromName(argument, 0);
            return lang == EMU_LANG_UNKNOWN ? defaultLang : lang;
        }
    }

    return defaultLang;
}

////////////////////////////////////////////////////////////////////

static int getScreenBitDepth()
{
    HDC hdc;
    hdc = GetDC(GetDesktopWindow());
    return GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#ifdef _CONSOLE
int main(int argc, char **argv)
#else
WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR szLine, int iShow)
#endif
{
#ifdef _CONSOLE
    char szLine[8192] = "";
#endif
    static WNDCLASSEX wndClass;
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
    char buffer[512];  
    BOOL screensaverActive;
    int  resetRegistry;
    HWND hwnd;
    int doExit = 0;
    RECT wr;
    MSG msg;
    int i;
    HINSTANCE kbdLockInst;
    int readOnlyDir;
    const char* tempName;
    int scrDepth;

#ifdef _CONSOLE
    for (i = 1; i < argc; i++) {
        strcat(szLine, argv[i]);
        strcat(szLine, " ");
    }
#endif

    scrDepth = getScreenBitDepth();
    if (scrDepth != 16 && scrDepth != 32) {
        MessageBox(NULL, "blueMSX works best in 16 or 32 bits color depth", "blueMSX Info", MB_OK | MB_ICONINFORMATION);
    }

    hwnd = FindWindow("blueMSX", "  blueMSX");
    if (hwnd != NULL && *szLine) {
        char args[2048];
        char* cmdLine = args;

        if (0 == strncmp(szLine, "/onearg ", 8)) {
            char* ptr;
            sprintf(args, "\"%s", szLine + 8);
            ptr = args + strlen(args);
            while(*--ptr == ' ') {
                *ptr = 0; 
            }
            strcat(args, "\"");
        }
        else {
            cmdLine = szLine;
        }
        if (!extractToken(cmdLine, 1)) {
            FILE* file = fopen(LAUNCH_TEMP_FILE, "w");
            if (file != NULL) {
                fwrite(cmdLine, 1, strlen(cmdLine) + 1, file);
                fclose(file);
                SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
                SetForegroundWindow(hwnd);
                PostMessage(hwnd, WM_LAUNCHFILE, 0, 0);
            }
            return 0;
        }
    }

    InitCommonControls(); 

    DirectDrawInitDisplayModes();

    wndClass.cbSize         = sizeof(wndClass);
    wndClass.style          = CS_OWNDC;
    wndClass.lpfnWndProc    = wndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLUEMSX));
    wndClass.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLUEMSX));
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = NULL;
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = "blueMSX";

    RegisterClassEx(&wndClass);

    wndClass.lpfnWndProc    = emuWndProc;
    wndClass.lpszClassName  = "blueMSXemuWindow";
    wndClass.hIcon          = NULL;
    wndClass.hIconSm        = NULL;
    RegisterClassEx(&wndClass);

    {
        // Set current directory to where the exe is located
        char* ptr;
        GetModuleFileName((HINSTANCE)GetModuleHandle(NULL), buffer, 512);
        ptr = (char*)stripPath(buffer);
        *ptr = 0;
        SetCurrentDirectory(buffer);
    }

    pkg_load("Packages/BombaPack.bpk", NULL, 0);

    appConfigLoad();

    kbdLockInst = LoadLibrary("kbdlock.dll");

    if (kbdLockInst != NULL) {
        kbdLockEnable  = (KbdLockFun)GetProcAddress(kbdLockInst, (LPCSTR)2);
        kbdLockDisable = (KbdLockFun)GetProcAddress(kbdLockInst, (LPCSTR)3);
    }

    readOnlyDir = setDefaultPath();

    {
        /* Modify scan code map if nessecary */
        PropKeyboardLanguage kbdLang = P_KBD_EUROPEAN;
        int syncMode = 0;
        char klId[KL_NAMELENGTH];
        if (GetKeyboardLayoutName(klId)) {
            if (0 == strcmp(klId + 4, "0411")) {
                kbdLang = P_KBD_JAPANESE;
            }
        }

        resetRegistry = emuCheckResetArgument(szLine);
        if (resetRegistry) {
            syncMode = canHandleVblankSyncMode() ? 1 : 0;
        }

        {
            char themeName[64];
            if (GetSystemMetrics(SM_CYSCREEN) > 600) {
                strcpy(themeName, "DIGIblue SUITE-X2");
            }
            else {
                strcpy(themeName, "Classic");
            }
            pProperties = propCreate(resetRegistry, getLangType(), kbdLang, syncMode, themeName);
        }

        pProperties->language = emuCheckLanguageArgument(szLine, pProperties->language);
        
        if (resetRegistry == 2) {
            propDestroy(pProperties);
            FreeLibrary(kbdLockInst);

            exit(0);
            return 0;
        }

        emuCheckFullscreenArgument(pProperties, szLine);
    }

    tempName = appConfigGetString("singlemachine", NULL);
    if (tempName != NULL) {
        strcpy(pProperties->emulation.machineName, tempName);
    }

    tempName = appConfigGetString("singletheme", NULL);
    if (tempName != NULL) {
        strcpy(pProperties->settings.themeName, tempName);
    }

    if (readOnlyDir && pProperties->settings.portable) {
        MessageBox(NULL, langErrorPortableReadonly(), langErrorTitle(), MB_OK);
        return 0;
    }

    // Load tools
    sprintf(buffer, "%s\\Tools", st.pCurDir);
    toolLoadAll(buffer, pProperties->language);


    CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE|COINIT_SPEED_OVER_MEMORY);

    videoInInitialize(pProperties);

    switch(pProperties->emulation.syncMethod) {
    case P_EMU_SYNCNONE:
        frameBufferSetFrameCount(1);
        break;
    case P_EMU_SYNCTOVBLANK:
    case P_EMU_SYNCTOVBLANKASYNC:
        frameBufferSetFrameCount(4);
        break;
    default:
        frameBufferSetFrameCount(3);
    }

    midiInitialize();

    st.active = 1;
    st.buttonState = 0;
    st.showDialog = 0;
    st.enteringFullscreen = 1;
    st.frameCount = 0;
    st.framesPerSecond = 0;
    st.minimized = 0;
    st.bmBitsGDI     = NULL;
    st.bmInfo.bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
    st.bmInfo.bmiHeader.biWidth          = WIDTH;
    st.bmInfo.bmiHeader.biHeight         = HEIGHT;
    st.bmInfo.bmiHeader.biPlanes         = 1;
    st.bmInfo.bmiHeader.biBitCount       = 32;
    st.bmInfo.bmiHeader.biCompression    = BI_RGB;
    st.bmInfo.bmiHeader.biClrUsed        = 0;
    st.bmInfo.bmiHeader.biClrImportant   = 0;
    st.ddrawEvent    = CreateEvent(NULL, FALSE, FALSE, NULL);
    st.ddrawAckEvent = CreateEvent(NULL, FALSE, FALSE, NULL);    
	



    st.pVideo = videoCreate();
    videoSetColors(st.pVideo, pProperties->video.saturation, pProperties->video.brightness, 
                  pProperties->video.contrast, pProperties->video.gamma);
    videoSetScanLines(st.pVideo, pProperties->video.scanlinesEnable, pProperties->video.scanlinesPct);
    videoSetColorSaturation(st.pVideo, pProperties->video.colorSaturationEnable, pProperties->video.colorSaturationWidth);
    videoSetBlendFrames(st.pVideo, pProperties->video.blendFrames);

    DirectDrawSetDisplayMode(pProperties->video.fullscreen.width,
                             pProperties->video.fullscreen.height,
                             pProperties->video.fullscreen.bitDepth);

    st.mixer  = mixerCreate();

    emulatorInit(pProperties, st.mixer);
    actionInit(st.pVideo, pProperties, st.mixer);
    langInit();
    tapeSetReadOnly(pProperties->cassette.readOnly);
    
    ethIfInitialize(pProperties);
    cdromInitialize();

    // Initialize shortcuts profile
    if (!shortcutsIsProfileValid(pProperties->emulation.shortcutProfile)) {
        shortcutsGetAnyProfile(pProperties->emulation.shortcutProfile);
    }

    st.shortcuts = shortcutsCreateProfile(pProperties->emulation.shortcutProfile);

    SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &screensaverActive, SPIF_SENDWININICHANGE); 
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, !pProperties->settings.disableScreensaver, 0, SPIF_SENDWININICHANGE); 

    if(!pProperties->settings.portable) {
        if (pProperties->emulation.registerFileTypes) {
            registerFileTypes();
        }
        else {
            unregisterFileTypes();
        }
    }

    pProperties->language = emuCheckLanguageArgument(szLine, pProperties->language);
    langSetLanguage(pProperties->language);

    st.hwnd = CreateWindow("blueMSX", "  blueMSX", 
                            WS_OVERLAPPED | WS_CLIPCHILDREN | WS_BORDER | WS_DLGFRAME | 
                            WS_SYSMENU | WS_MINIMIZEBOX | (pProperties->video.maximizeIsFullscreen?WS_MAXIMIZEBOX:0), 
                            CW_USEDEFAULT, CW_USEDEFAULT, 800, 200, NULL, NULL, hInstance, NULL);

    menuCreate(st.hwnd);

    if (appConfigGetInt("menu.file", 1) != 0) {
        addMenuItem(langMenuFile(), actionMenuFile, 0);
    }
    if (appConfigGetInt("menu.emulation", 1) != 0) {
        addMenuItem(langMenuRun(), actionMenuRun, 1);
    }
    if (appConfigGetInt("menu.window", 1) != 0) {
        addMenuItem(langMenuWindow(), actionMenuZoom, 1);
    }
    if (appConfigGetInt("menu.options", 1) != 0) {
        addMenuItem(langMenuOptions(), actionMenuOptions, 1);
    }
    if (appConfigGetInt("menu.tools", 1) != 0) {
        addMenuItem(langMenuTools(), actionMenuTools, 1);
    }
    if (appConfigGetInt("menu.help", 1) != 0) {
        addMenuItem(langMenuHelp(), actionMenuHelp, 1);
    }

    st.emuHwnd = CreateWindow("blueMSXemuWindow", "", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 0, 0, st.hwnd, NULL, hInstance, NULL);
    ShowWindow(st.emuHwnd, SW_HIDE);

    inputInit();
    inputReset(st.hwnd);
    keyboardLoadConfig(pProperties->keyboard.configFile);
    sprintf(pProperties->keyboard.configFile, keyboardGetCurrentConfig());
    
    mouseEmuInit(st.emuHwnd, 1);
    joystickPortSetType(0, pProperties->joy1.typeId);
    joystickPortSetType(1, pProperties->joy2.typeId);

    printerIoSetType(pProperties->ports.Lpt.type, pProperties->ports.Lpt.fileName);
    uartIoSetType(pProperties->ports.Com.type, pProperties->ports.Com.fileName);
    midiIoSetMidiOutType(pProperties->sound.MidiOut.type, pProperties->sound.MidiOut.fileName);
    midiIoSetMidiInType(pProperties->sound.MidiIn.type, pProperties->sound.MidiIn.fileName);
    ykIoSetMidiInType(pProperties->sound.YkIn.type, pProperties->sound.YkIn.fileName);
    midiEnableMt32ToGmMapping(pProperties->sound.MidiOut.mt32ToGm);
    midiInSetChannelFilter(pProperties->sound.YkIn.channel);

    st.dskWnd = diskQuickviewWindowCreate(st.hwnd);

    if (pProperties->video.windowX < 0 || pProperties->video.windowY < 0) {
        GetWindowRect(st.hwnd, &wr);
        pProperties->video.windowX = wr.left;
        pProperties->video.windowY = wr.top;
    }

    if (pProperties->video.windowX > GetSystemMetrics(SM_CXSCREEN) - 300) {
        pProperties->video.windowX = GetSystemMetrics(SM_CXSCREEN) - 300;
    }

    if (pProperties->video.windowY > GetSystemMetrics(SM_CYSCREEN) - 300) {
        pProperties->video.windowY = GetSystemMetrics(SM_CYSCREEN) - 300;
    }

    SetWindowPos(st.hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOZORDER);

    st.enteringFullscreen = 0;

    soundDriverConfig(st.hwnd, pProperties->sound.driver);
    emulatorRestartSound();

    for (i = 0; i < MIXER_CHANNEL_TYPE_COUNT; i++) {
        mixerSetChannelTypeVolume(st.mixer, i, pProperties->sound.mixerChannel[i].volume);
        mixerSetChannelTypePan(st.mixer, i, pProperties->sound.mixerChannel[i].pan);
        mixerEnableChannelType(st.mixer, i, pProperties->sound.mixerChannel[i].enable);
    }
    
    mixerSetMasterVolume(st.mixer, pProperties->sound.masterVolume);
    mixerEnableMaster(st.mixer, pProperties->sound.masterEnable);

    videoUpdateAll(st.pVideo, pProperties);
    
    mediaDbSetDefaultRomType(pProperties->cartridge.defaultType);

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        if (pProperties->media.carts[i].fileName[0]) insertCartridge(pProperties, i, pProperties->media.carts[i].fileName, pProperties->media.carts[i].fileNameInZip, pProperties->media.carts[i].type, -1);
        updateExtendedRomName(i, pProperties->media.carts[i].fileName, pProperties->media.carts[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_DISKS; i++) {
        if (pProperties->media.disks[i].fileName[0]) insertDiskette(pProperties, i, pProperties->media.disks[i].fileName, pProperties->media.disks[i].fileNameInZip, -1);
        updateExtendedDiskName(i, pProperties->media.disks[i].fileName, pProperties->media.disks[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_TAPES; i++) {
        if (pProperties->media.tapes[i].fileName[0]) insertCassette(pProperties, i, pProperties->media.tapes[i].fileName, pProperties->media.tapes[i].fileNameInZip, 0);
        updateExtendedCasName(i, pProperties->media.tapes[i].fileName, pProperties->media.tapes[i].fileNameInZip);
    }

    // Call initStatistics to get correct ram size and vram size for status bars
    {
        Machine* machine = machineCreate(pProperties->emulation.machineName);
        if (machine != NULL) {
            boardSetMachine(machine);
            machineDestroy(machine);
        }
    }
    boardSetFdcTimingEnable(pProperties->emulation.enableFdcTiming);
    boardSetNoSpriteLimits(pProperties->emulation.noSpriteLimits);
    boardSetY8950Enable(pProperties->sound.chip.enableY8950);
    boardSetYm2413Enable(pProperties->sound.chip.enableYM2413);
    boardSetMoonsoundEnable(pProperties->sound.chip.enableMoonsound);
    boardSetVideoAutodetect(pProperties->video.detectActiveMonitor);

    updateMenu(0);

    if (emuTryStartWithArguments(pProperties, szLine, NULL) < 0) {           
        exit(0);
        return 0;
    }

    st.themePageActive = NULL;
    st.themeList = createThemeList(themeClassicCreate());
    themeSet(emuCheckThemeArgument(szLine), 0);

    archUpdateWindow();
    ShowWindow(st.hwnd, SW_NORMAL);
    UpdateWindow(st.hwnd);

    if (pProperties->emulation.priorityBoost) {
        SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    }

    while (!doExit) {
        DWORD rv = MsgWaitForMultipleObjects(1, &st.ddrawEvent, FALSE, INFINITE, QS_ALLINPUT);    
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                doExit = 1;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (rv == WAIT_OBJECT_0) {
            if (!st.minimized) {
                emuWindowDraw(st.diplayUpdateOnVblank);
            }
            SetEvent(st.ddrawAckEvent);
        }
    }

    emulatorExit();
    sprintf(pProperties->keyboard.configFile, keyboardGetCurrentConfig());
    shortcutsDestroyProfile(st.shortcuts);
    videoDestroy(st.pVideo);
    
    SetCurrentDirectory(st.pCurDir);

    videoInCleanup(pProperties);
    ethIfCleanup(pProperties);
    cdromCleanup();

    pProperties->joy1.typeId = joystickPortGetType(0);
    pProperties->joy2.typeId = joystickPortGetType(1);
    propDestroy(pProperties);

    archSoundDestroy();
    Sleep(300);
    mixerDestroy(st.mixer);
    midiShutdown();

    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, screensaverActive, 0, SPIF_SENDWININICHANGE); 

    if (kbdLockDisable) {
        kbdLockDisable();
    }
    FreeLibrary(kbdLockInst);

    CoUninitialize();

    exit(0);

    return 0;
}



////////////////////////////////////////////////////////////////////////////////////////

void archShowCassettePosDialog()
{
    enterDialogShow();
    setTapePosition(getMainHwnd(), pProperties);
    exitDialogShow();
}

void archShowHelpDialog()
{
    HINSTANCE rv = 0;
    /* NOTE: leaks 10 resource handles everytime ShellExecute is called, XP SP2 */
    if (pProperties->language == EMU_LANG_JAPANESE) {
         rv = ShellExecute(getMainHwnd(), "open", "blueMSXjp.chm", NULL, NULL, SW_SHOWNORMAL);
    }
    if (pProperties->language == EMU_LANG_DUTCH) {
         rv = ShellExecute(getMainHwnd(), "open", "blueMSXnl.chm", NULL, NULL, SW_SHOWNORMAL);
    }
    if (rv <= (HINSTANCE)32) {
        rv = ShellExecute(getMainHwnd(), "open", "blueMSX.chm", NULL, NULL, SW_SHOWNORMAL);
    }
    if (rv <= (HINSTANCE)32) {
        MessageBox(NULL, langErrorNoHelp(), langErrorTitle(), MB_OK);
    }
}

void archShowAboutDialog()
{
    enterDialogShow();
    helpShowAbout(getMainHwnd());
    exitDialogShow();
}

void archShowNoRomInZipDialog() {
    enterDialogShow();
    MessageBox(NULL, langErrorNoRomInZip(), langErrorTitle(), MB_OK);
    exitDialogShow();
}

void archShowNoDiskInZipDialog() {
    enterDialogShow();
    MessageBox(NULL, langErrorNoDskInZip(), langErrorTitle(), MB_OK);
    enterDialogShow();
}

void archShowNoCasInZipDialog() {
    enterDialogShow();
    MessageBox(NULL, langErrorNoCasInZip(), langErrorTitle(), MB_OK);
    enterDialogShow();
}

void archShowStartEmuFailDialog() {
    MessageBox(NULL, langErrorStartEmu(), langErrorTitle(), MB_ICONHAND | MB_OK);
}

void archShowLanguageDialog()
{
    int lang;
    int success;
    int i;

    enterDialogShow();
    lang = langShowDlg(getMainHwnd(), pProperties->language);
    exitDialogShow();
    success = langSetLanguage(lang);
    if (success) {
        pProperties->language = lang;
        if (appConfigGetInt("menu.file", 1) != 0) {
            addMenuItem(langMenuFile(), actionMenuFile, 0);
        }
        if (appConfigGetInt("menu.emulation", 1) != 0) {
            addMenuItem(langMenuRun(), actionMenuRun, 1);
        }
        if (appConfigGetInt("menu.window", 1) != 0) {
            addMenuItem(langMenuWindow(), actionMenuZoom, 1);
        }
        if (appConfigGetInt("menu.options", 1) != 0) {
            addMenuItem(langMenuOptions(), actionMenuOptions, 1);
        }
        if (appConfigGetInt("menu.tools", 1) != 0) {
            addMenuItem(langMenuTools(), actionMenuTools, 1);
        }
        if (appConfigGetInt("menu.help", 1) != 0) {
            addMenuItem(langMenuHelp(), actionMenuHelp, 1);
        }
    }
    
    for (i = 0; i < toolGetCount(); i++) {
        toolInfoSetLanguage(toolInfoGet(i), pProperties->language);
    }

    updateMenu(0);
}

void archShowShortcutsEditor() 
{
    int apply;
    enterDialogShow();
    apply = shortcutsShowDialog(getMainHwnd(), pProperties);
    shortcutsDestroyProfile(st.shortcuts);
    st.shortcuts = shortcutsCreateProfile(pProperties->emulation.shortcutProfile);
    updateMenu(0);
    exitDialogShow();
}

void archShowKeyboardEditor()
{
    static ThemeCollection* tc = NULL;
    
    if (tc == NULL) {
        char themePath[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, themePath);
        strcat(themePath, "\\Keyboard Config\\Theme");
        tc = themeLoad(themePath);
    }

    if (tc == NULL) {
        MessageBox(NULL, "Could not find the Keyboard Editor Theme", langErrorTitle(), MB_ICONERROR | MB_OK);
    }
    else {
        themeCollectionOpenWindow(tc, themeGetNameHash("blueMSX - Input Editor"));
    }
}

void archShowMixer()
{
    static ThemeCollection* tc = NULL;
    
    if (tc == NULL) {
        char themePath[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, themePath);
        strcat(themePath, "\\Properties\\Mixer");
        tc = themeLoad(themePath);
    }

    if (tc == NULL) {
        MessageBox(NULL, "Could not find the Mixer Theme", langErrorTitle(), MB_ICONERROR | MB_OK);
    }
    else {
        themeCollectionOpenWindow(tc, themeGetNameHash("blueMSX - Sound Mixer"));
    }
}

void archShowDebugger()
{
    ToolInfo* ti = toolInfoFind("Debugger");
    if (ti != NULL) {
        toolInfoShowTool(ti);
    }
}

void archShowTrainer()
{
    ToolInfo* ti = toolInfoFind("Trainer");
    if (ti != NULL) {
        toolInfoShowTool(ti);
    }
}

void archShowMachineEditor()
{
    int apply;
    enterDialogShow();
    apply = confShowDialog(getMainHwnd(), pProperties->emulation.machineName);
    exitDialogShow();
    if (apply) {
        actionEmuResetHard();
    }
    updateMenu(0);
}

void* archScreenCapture(ScreenCaptureType type, int* bitmapSize, int onlyBmp)
{
    int png = onlyBmp ? 0 : pProperties->settings.usePngScreenshots;

    if (bitmapSize != NULL) {
        *bitmapSize = 0;
    }
    switch (type) {
    case SC_NORMAL:
        if (png) {
            createScreenShot(1, NULL, png);
        }
        else {
            SetTimer(getMainHwnd(), TIMER_SCREENSHOT, 50, NULL);
        }
        return NULL;
    case SC_SMALL:
        return createScreenShot(0, bitmapSize, png);
    case SC_LARGE:
        return createScreenShot(1, bitmapSize, png);
    }

    return NULL;
}

void archMinimizeMainWindow() {
    ShowWindow(getMainHwnd(), SW_MINIMIZE);
    updateMenu(0);
}

void archDiskQuickChangeNotify() 
{
    diskQuickviewWindowShow(st.dskWnd);
}

////////////////////////////////////////////////////////////////////
// File open/save stuff

static void replaceCharInString(char* str, char oldChar, char newChar) 
{
    while (*str) {
        if (*str == oldChar) {
            *str = newChar;
        }
        str++;
    }
}

char* archFileSave(char* title, char* extensionList, char* defaultDir, char* extensions, int* selectedExtension, char* defExt)
{
    char* fileName;

    enterDialogShow();
    fileName = saveFile(getMainHwnd(), title, extensionList, selectedExtension, defaultDir, defExt);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

const char* archGetCurrentDirectory()
{
    static char pathname[512];
    GetCurrentDirectory(512, pathname);
    return pathname;
}

int archCreateDirectory(const char* pathname)
{
    return mkdir(pathname);
}

void archSetCurrentDirectory(const char* pathname)
{
    SetCurrentDirectory(pathname);
}

char* archDirnameGetOpenDisk(Properties* properties, int drive)
{
    char* title = drive == 1 ? langDlgInsertDiskB() : langDlgInsertDiskA();
    char* defaultDir = properties->media.disks[drive].directory;
    char* filename;

    enterDialogShow();
    filename = openDir(getMainHwnd(), title, defaultDir);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return filename;
}

char* archFileOpen(char* title, char* extensionList, char* defaultDir, char* extensions, int* selectedExtension, char* defautExtension, int createFileSize)
{
    char* fileName;

    enterDialogShow();
    fileName = openFile(getMainHwnd(), title, extensionList, defaultDir, createFileSize, defautExtension, selectedExtension);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetOpenState(Properties* properties)
{
    char* title = langDlgLoadState();
    char extensionList[512];
    char* defaultDir = properties->emulation.statsDefDir;
    char* extensions = ".sta\0";
    int* selectedExtension = NULL;
    char* defautExtension = NULL;
    int createFileSize = -1;
    char* fileName;

    sprintf(extensionList, "%s   (*.sta)#*.sta#", langFileCpuState());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    fileName = openStateFile(getMainHwnd(), title, extensionList, defaultDir, createFileSize, defautExtension, 
                             selectedExtension, &pProperties->settings.showStatePreview);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetOpenCapture(Properties* properties)
{
    char* title = langDlgLoadVideoCapture();
    char extensionList[512];
    char* defaultDir = properties->emulation.statsDefDir;
    char* extensions = ".cap\0";
    int* selectedExtension = NULL;
    char* defautExtension = NULL;
    int createFileSize = -1;
    char* fileName;

    sprintf(extensionList, "%s   (*.cap)#*.cap#", langFileVideoCapture());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    fileName = openStateFile(getMainHwnd(), title, extensionList, defaultDir, createFileSize, defautExtension, 
                             selectedExtension, &pProperties->settings.showStatePreview);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetOpenRom(Properties* properties, int cartSlot, RomType* romType) 
{
    char* defaultDir = properties->cartridge.defDir;
    int* selectedExtension = &properties->media.carts[cartSlot].extensionFilter;
    char* defautExtension = ".rom";
    char* extensions = ".rom\0.ri\0.mx1\0.mx2\0.col\0.sms\0.sg\0.sc\0.zip\0.*\0";
    char* title = cartSlot == 1 ? langDlgInsertRom2() : langDlgInsertRom1();
    char* fileName;
    char extensionList[512];

    // DINK: different defDir depending on machine
    if (strcasestr(properties->emulation.machineName, "Coleco")!=0) {
        defaultDir = properties->cartridge.defDirCOLECO; }
    if (strcasestr(properties->emulation.machineName, "SEGA")!=0) {
        defaultDir = properties->cartridge.defDirSEGA; }
    if ((strcasestr(properties->emulation.machineName, "SVI-318")!=0) ||
        (strcasestr(properties->emulation.machineName, "SVI-328")!=0)) {
        defaultDir = properties->cartridge.defDirSVI; }

    sprintf(extensionList, "%s   (*.rom, *.ri, *.mx1, *.mx2, *.col, *.sms, *.sg, *.sc, *.zip)#*.rom; *.ri; *.mx1; *.mx2; *.col; *.sg; *.sc; *.zip#%s   (*.*)#*.*#", langFileRom(), langFileAll());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    fileName = openRomFile(getMainHwnd(), title, extensionList, defaultDir, 1, defautExtension, selectedExtension, romType);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetOpenCas(Properties* properties)
{
    char* title = langDlgInsertCas();
    char  extensionList[512];
    char* defaultDir = properties->cassette.defDir;
    char* extensions = ".cas\0.zip\0.*\0";
    int* selectedExtension = &properties->media.tapes[0].extensionFilter;
    char* defautExtension = ".cas";
    int createFileSize = 0;
    char* fileName;

    sprintf(extensionList, "%s   (*.cas, *.zip)#*.cas; *.zip#%s   (*.*)#*.*#", langFileCas(), langFileAll());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    fileName = openFile(getMainHwnd(), title, extensionList, defaultDir, createFileSize, defautExtension, selectedExtension);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetOpenDisk(Properties* properties, int drive, int allowCreate)
{
    char* title = drive == 1 ? langDlgInsertDiskB() : langDlgInsertDiskA();
    char  extensionList[512];
    char* defaultDir = properties->diskdrive.defDir;
    char* extensions = ".dsk\0.di1\0.di2\0.360\0.720\0.sf7\0.zip\0";
    int* selectedExtension = &properties->media.disks[drive].extensionFilter;
    char* defautExtension = ".dsk";
    int createFileSize = 720 * 1024;
    char* fileName;

    sprintf(extensionList, "%s   (*.dsk, *.di1, *.di2, *.360, *.720, *.sf7, *.zip)#*.dsk; *.di1; *.di2; *.360; *.720; *.sf7; *.zip#%s   (*.*)#*.*#", langFileDisk(), langFileAll());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    if (allowCreate) {
        fileName = openNewDskFile(getMainHwnd(), title, extensionList, defaultDir, defautExtension, selectedExtension);
    }
    else {
        fileName = openFile(getMainHwnd(), title, extensionList, defaultDir, createFileSize, defautExtension, selectedExtension);
    }
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetOpenHarddisk(Properties* properties, int drive, int allowCreate)
{
    char* title = langDlgInsertHarddisk();
    char  extensionList[512];
    char* defaultDir = properties->diskdrive.defHdDir;
    char* extensions = ".dsk\0.di1\0.di2\0.360\0.720\0.sf7\0.zip\0";
    int* selectedExtension = &properties->media.disks[drive].extensionFilter;
    char* defautExtension = ".dsk";
    char* fileName;

    sprintf(extensionList, "%s   (*.dsk, *.di1, *.di2, *.360, *.720, *.sf7, *.zip)#*.dsk; *.di1; *.di2; *.360; *.720; *.sf7; *.zip#%s   (*.*)#*.*#", langFileDisk(), langFileAll());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    if (allowCreate) {
        fileName = openNewHdFile(getMainHwnd(), title, extensionList, defaultDir, defautExtension, selectedExtension);
    }
    else {
        fileName = openFile(getMainHwnd(), title, extensionList, defaultDir, -1, defautExtension, selectedExtension);
    }
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

char* archFilenameGetSaveCas(Properties* properties, int* type)
{
    char* title = langDlgSaveCassette();
    char  extensionList[512];
    char* defaultDir = properties->cassette.defDir;
    char* extensions = ".cas\0";
    int* selectedExtension = type;

    sprintf(extensionList, "%s - fMSX-DOS     (*.cas)#*.cas#%s - fMSX98/AT   (*.cas)#*.cas#%s - SVI-328         (*.cas)#*.cas#", langFileCas(), langFileCas(), langFileCas());
    replaceCharInString(extensionList, '#', 0);

    return archFileSave(title, extensionList, defaultDir, extensions, selectedExtension, ".cas");
}

char* archFilenameGetSaveState(Properties* properties)
{
    char* title = langDlgSaveState();
    char  extensionList[512];
    char* defaultDir = properties->emulation.statsDefDir;
    char* extensions = ".sta\0";
    int* selectedExtension = NULL;
    char* fileName;

    sprintf(extensionList, "%s   (*.sta)#*.sta#", langFileCpuState());
    replaceCharInString(extensionList, '#', 0);

    enterDialogShow();
    fileName = saveStateFile(getMainHwnd(), title, extensionList, selectedExtension, defaultDir, &pProperties->settings.showStatePreview);
    exitDialogShow();
    SetCurrentDirectory(st.pCurDir);

    return fileName;
}

void archVideoOutputChange()
{
    updateMenu(0);
}

void archUpdateMenu(int show) {
    updateMenu(show);
}

void archQuit() {
    DestroyWindow(getMainHwnd());
}

char* archFilenameGetOpenAnyZip(Properties* properties, const char* fname, const char* fileList, int count, int* autostart, int* romType)
{
    static char filename[512];
    ZipFileDlgInfo dlgInfo;

    sprintf(dlgInfo.title, "%s", langDlgLoadRomDskCas());
    sprintf(dlgInfo.description, "%s", langDlgLoadRomDskCasDesc());
    strcpy(dlgInfo.zipFileName, filename);
    dlgInfo.fileList = fileList;
    dlgInfo.fileListCount = count;
    dlgInfo.autoReset = properties->diskdrive.autostartA || properties->cartridge.autoReset || *autostart;
    dlgInfo.selectFileIndex = -1;
    dlgInfo.selectFile[0] = 0;

    enterDialogShow();
    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ZIPDSK), getMainHwnd(), dskZipDlgProc, (LPARAM)&dlgInfo);
    exitDialogShow();

    if (dlgInfo.selectFile[0] == '\0') {
        return NULL;
    }
    *romType = dlgInfo.openRomType;
    *autostart = dlgInfo.autoReset;
    strcpy(filename, dlgInfo.selectFile);
    return filename;
}

char* archFilenameGetOpenDiskZip(Properties* properties, int drive, const char* fname, const char* fileList, int count, int* autostart)
{
    static char filename[512];
    ZipFileDlgInfo dlgInfo;

    sprintf(dlgInfo.title, "%s", langDlgLoadDsk());
    sprintf(dlgInfo.description, "%s", langDlgLoadDskDesc());
    strcpy(dlgInfo.zipFileName, fname);
    dlgInfo.fileList = fileList;
    dlgInfo.fileListCount = count;
    dlgInfo.autoReset = *autostart;

    dlgInfo.selectFileIndex = -1;
    strcpy(dlgInfo.selectFile, drive == 0 ? properties->media.disks[0].fileNameInZip : properties->media.disks[1].fileNameInZip);

    enterDialogShow();
    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ZIPDSK), getMainHwnd(), dskZipDlgProc, (LPARAM)&dlgInfo);
    exitDialogShow();

    if (dlgInfo.selectFile[0] == '\0') {
        return NULL;
    }
    *autostart = dlgInfo.autoReset;
    strcpy(filename, dlgInfo.selectFile);
    return filename;
}

char* archFilenameGetOpenCasZip(Properties* properties, const char* fname, const char* fileList, int count, int* autostart)
{
    static char filename[512];
    ZipFileDlgInfo dlgInfo;

    sprintf(dlgInfo.title, "%s", langDlgLoadCas());
    sprintf(dlgInfo.description, "%s", langDlgLoadCasDesc());
    strcpy(dlgInfo.zipFileName, fname);
    dlgInfo.fileList = fileList;
    dlgInfo.fileListCount = count;
    dlgInfo.autoReset = *autostart;

    dlgInfo.selectFileIndex = -1;
    strcpy(dlgInfo.selectFile, properties->media.tapes[0].fileNameInZip);

    enterDialogShow();
    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ZIPDSK), getMainHwnd(), dskZipDlgProc, (LPARAM)&dlgInfo);
    exitDialogShow();

    if (dlgInfo.selectFile[0] == '\0') {
        return NULL;
    }
    *autostart = dlgInfo.autoReset;
    strcpy(filename, dlgInfo.selectFile);
    return filename;
}

char* archFilenameGetOpenRomZip(Properties* properties, int cartSlot, const char* fname, const char* fileList, int count, int* autostart, int* romType)
{
    static char filename[512];
    ZipFileDlgInfo dlgInfo;

    sprintf(dlgInfo.title, "%s", langDlgLoadRom());
    sprintf(dlgInfo.description, "%s", langDlgLoadRomDesc());
    strcpy(dlgInfo.zipFileName, fname);
    dlgInfo.fileList = fileList;
    dlgInfo.fileListCount = count;
    dlgInfo.autoReset = *autostart;

    dlgInfo.selectFileIndex = -1;
    strcpy(dlgInfo.selectFile, cartSlot == 0 ? properties->media.carts[0].fileNameInZip : properties->media.carts[1].fileNameInZip);

    enterDialogShow();
    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ZIPDSK), getMainHwnd(), dskZipDlgProc, (LPARAM)&dlgInfo);
    exitDialogShow();

    if (dlgInfo.selectFile[0] == '\0') {
        return NULL;
    }
    *romType = dlgInfo.openRomType;
    *autostart = dlgInfo.autoReset;
    strcpy(filename, dlgInfo.selectFile);
    return filename;
}


void archEmulationStartNotification() {
    if (st.renderVideo) {
        return;
    }
    ShowWindow(st.emuHwnd, SW_NORMAL);

    if (kbdLockEnable != NULL && pProperties->emulation.disableWinKeys) {
        kbdLockEnable();
    }
}

void archEmulationStopNotification()
{
    if (st.renderVideo) {
        return;
    }

    DirectXSetGDISurface();
    ShowWindow(st.emuHwnd, SW_HIDE);

    if (kbdLockDisable != NULL) {
        kbdLockDisable();
    }
}

int archUpdateEmuDisplay(int syncMode) {
    st.diplayUpdateOnVblank = syncMode == 4;
    if (pProperties->video.driver == P_VIDEO_DRVGDI) {
        if (syncMode == 0) {
            PostMessage(getMainHwnd(), WM_UPDATE, 0, 0);
        }
    }
    else if (syncMode == 4) { // VBlank async
        SetEvent(st.ddrawEvent);
    }
    else if (syncMode == 3) { // VBlank sync
        st.diplaySync = 1;
        emuWindowDraw(0);
        return st.diplayUpdated;
    }
    else {
        SetEvent(st.ddrawEvent);
        if (syncMode > 1) {
            WaitForSingleObject(st.ddrawAckEvent, 500);
            return st.diplayUpdated;
        }
    }
    return 0;
}

void archUpdateEmuDisplayConfig() {
    updateEmuWindow();
}

void archThemeSetNext() {
    st.themeIndex++;
    if (st.themeList[st.themeIndex] == NULL) {
        st.themeIndex = 0;
    }
    
    strcpy(pProperties->settings.themeName, st.themeList[st.themeIndex]->name);

    archUpdateWindow();
}

void archThemeUpdate(Theme* theme) {
    if (theme->reference == NULL) {
        themeSet(pProperties->settings.themeName, 0);
    }
    else {
        SendMessage(theme->reference, WM_UPDATE, 0, 0);
    }
}

int archGetFramesPerSecond() {
    return st.framesPerSecond;
}

void archEmulationStartFailure() {
    aviStopRender();
    MessageBox(NULL, langErrorStartEmu(), langErrorTitle(), MB_ICONHAND | MB_OK);
}

int archFileExists(const char* fileName)
{
    return PathFileExists(fileName);
}

int archFileDelete(const char* fileName)
{
    return DeleteFile(fileName);
}

void archMaximizeWindow() {
    if (st.currentHwnd != NULL) {
//        ShowWindow(st.currentHwnd, SW_MAXIMIZE);
        SendMessage(st.currentHwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    }
}

void archMinimizeWindow() {
    if (st.currentHwnd != NULL) {
//        ShowWindow(st.currentHwnd, SW_MINIMIZE);
        SendMessage(st.currentHwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    }
}

void archCloseWindow() {
    if (st.currentHwnd != NULL) {
        SendMessage(st.currentHwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
//        CloseWindow(st.currentHwnd);
    }
}

void archWindowStartMove() {
    if (st.currentHwnd != NULL) {
        GetCursorPos(&st.currentHwndMouse);
        GetWindowRect(st.currentHwnd, &st.currentHwndRect);
        st.currentHwndRect.right = 1; // Used flag to detect window move
    }
}

void archWindowMove() {
    if (st.currentHwnd != NULL && st.currentHwndRect.right) {
        POINT pt;
        int x;
        int y;
        GetCursorPos(&pt);
        x = st.currentHwndRect.left + pt.x - st.currentHwndMouse.x;
        y = st.currentHwndRect.top  + pt.y - st.currentHwndMouse.y;
        SetWindowPos(st.currentHwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }
}

void archWindowEndMove() {
    st.currentHwndRect.right = 0;
}

void archVideoCaptureSave()
{
    actionEmuStop();

    st.renderVideo = 1;
    aviStartRender(getMainHwnd(), propGetGlobalProperties(), st.pVideo);
    st.renderVideo = 0;
}

void SetCurrentWindow(HWND hwnd) {
    st.currentHwnd = hwnd;
}

void archTrap(UInt8 value)
{
    if (appConfigGetInt("trap.quit", 0) != 0) {
        PostMessage(getMainHwnd(), WM_CLOSE, 0, 0);
    }
}


///////////////////////////////////////////////////////////////////////////


static int LoadMemory(const char* fileName, UInt16 address)
{
    FILE* file;

    file = fopen(fileName, "rb");
    if (file != NULL) {
        UInt8 data;
        while (address <= 0xffff && (fread(&data, 1, 1, file) == 1)) {
            slotWrite(NULL, address, data);
            address++;
        }
        fclose(file);
        return 1;
    }
    return 0;
}

static BOOL CALLBACK loadMemorProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static HICON hIconBtBrowse = NULL;

    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(hDlg, langMenuToolsLoadMemory());
        SetWindowText(GetDlgItem(hDlg, IDC_LDMEM_CAPFIL), langConfEditMemFile());
        SetWindowText(GetDlgItem(hDlg, IDC_LDMEM_CAPADR), langConfEditMemAddress());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());

        if (hIconBtBrowse == NULL) {
            hIconBtBrowse = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BROWSE));
        }
        SendMessage(GetDlgItem(hDlg, IDC_LDMEM_BROWSE), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconBtBrowse);

        return FALSE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {

        case IDC_LDMEM_BROWSE: {
                static char  defDir[MAX_PATH] = { 0 };
                char  curDir[MAX_PATH];
                char* fileName;
                char extensionList[512];

                GetCurrentDirectory(MAX_PATH, curDir);
                if (strlen(defDir) == 0) {
                    strcpy(defDir, curDir);
                }
                sprintf(extensionList, "%s   (*.*)#*.*#", langFileAll());
                replaceCharInString(extensionList, '#', 0);

                fileName = openFile(hDlg, langConfOpenRom(), extensionList, defDir, -1, NULL, NULL);
                if (fileName != NULL) {
                   SetWindowText(GetDlgItem(hDlg, IDC_LDMEM_FILENAME), fileName);
                }

                SetFocus(GetDlgItem(hDlg, IDC_LDMEM_ADDRESS));
            }
            return TRUE;

            case IDOK: {
                char fileName[512];
                char data[5];
                int addr, rv;

                GetWindowText(GetDlgItem(hDlg, IDC_LDMEM_FILENAME), fileName, sizeof(fileName));
                GetWindowText(GetDlgItem(hDlg, IDC_LDMEM_ADDRESS), data, sizeof(data));

                rv = sscanf(data, "%x", &addr);
                if (rv == 1) {
//                    emulatorSuspend();
                    LoadMemory(fileName, (UInt16)addr);
//                    emulatorResume();
                }

                EndDialog(hDlg, TRUE);
                return TRUE;
            }
        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

int showLoadMemoryDlg(HWND hwnd)
{
    int rv;

    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_LOAD_MEMORY), hwnd, loadMemorProc);
    if (!rv) {
        return 0;
    }

    return 1;
}

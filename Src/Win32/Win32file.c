/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32file.c,v $
**
** $Revision: 1.42 $
**
** $Date: 2006/06/26 19:35:55 $
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
#include <tchar.h>
#include "Win32file.h"
#include <stdlib.h>
#include <stdio.h>
#include "Resource.h"
#include "MediaDb.h"
#include "RomLoader.h"
#include "ziphelper.h"
#include "Win32Common.h"
#include "Win32ScreenShot.h"
#include "Language.h"

#define WM_DIALOGRESIZE (WM_USER + 1500)

static RomType romTypeList[] = {
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
    ROM_HALNOTE,
    ROM_KONAMISYNTH,
    ROM_KONAMKBDMAS,
    ROM_KONWORDPRO,
    ROM_MAJUTSUSHI,
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
    ROM_TC8566AF,
    ROM_MICROSOL,
    ROM_NATIONALFDC,
    ROM_PHILIPSFDC,
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
    ROM_SVI328,
    ROM_SVI738FDC,
    ROM_COLECO,
    ROM_SG1000,
    ROM_SG1000CASTLE,
    ROM_MICROSOL80,
    ROM_SVI727,
    ROM_SONYHBIV1,
    ROM_FMDAS,
    ROM_YAMAHASFG01,
    ROM_YAMAHASFG05,
    ROM_UNKNOWN,
};

static RomType openRomType;

UINT_PTR CALLBACK hookRomProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
	case WM_DIALOGRESIZE:
        updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 0);
        return 0;

    case WM_INITDIALOG:
        {
            int i;

            for (i = 0; romTypeList[i] != ROM_UNKNOWN; i++) {
                SendDlgItemMessage(hDlg, IDC_OPEN_ROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(romTypeList[i]));
            }
            SendDlgItemMessage(hDlg, IDC_OPEN_ROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(ROM_UNKNOWN));
            SetWindowText(GetDlgItem(hDlg, IDC_OPEN_ROMTEXT), langDlgRomType());
            EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), 0);
        }
        return 0;

    case WM_SIZE:
        {
            RECT r;
            int height;
            int width;
            HWND hwnd;

            GetClientRect(GetParent(hDlg), &r);
            
            height = r.bottom - r.top;
            width  = r.right - r.left;

            hwnd = GetDlgItem(hDlg, IDC_OPEN_ROMTEXT);
            SetWindowPos(hwnd, NULL, 8, height - 29, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            hwnd = GetDlgItem(hDlg, IDC_OPEN_ROMTYPE);
            SetWindowPos(hwnd, NULL, 81, height - 32, width - 187, 12, SWP_NOZORDER);
        }
        return 0;
        
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_OPEN_ROMTYPE:
            if (HIWORD(wParam) == 1 || HIWORD(wParam) == 2) {
                int idx = SendMessage(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), CB_GETCURSEL, 0, 0);

                openRomType = idx == CB_ERR ? -1 : romTypeList[idx];
            }
            return 0;
        }

        return 0;

    case WM_DESTROY:
        saveDialogPos(GetParent(hDlg), DLG_ID_OPEN);
        return 0;
        
    case WM_NOTIFY:
        {
            OFNOTIFY* ofn = (OFNOTIFY*)lParam;
            switch (ofn->hdr.code) {
			case CDN_INITDONE:
				//It is not effective since the second times why. 
				updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 1);
				PostMessage(hDlg, WM_DIALOGRESIZE, 0, 0);
				break;

            case CDN_SELCHANGE:
                {
                    char fileName[MAX_PATH];
                    int fileSize = SendMessage(GetParent(hDlg), CDM_GETFILEPATH, MAX_PATH, (LPARAM)fileName);
                    int size;
                    char* buf = NULL;

                    if (isFileExtension(fileName, ".zip")) {
                        int countRom;
                        int countRi;
                        int countMx1;
                        int countMx2;
                        int countCol;
                        int countSg;
                        int countSc;
                        char* fileListRom = zipGetFileList(fileName, ".rom", &countRom);
                        char* fileListRi  = zipGetFileList(fileName, ".ri",  &countRi);
                        char* fileListMx1 = zipGetFileList(fileName, ".mx1", &countMx1);
                        char* fileListMx2 = zipGetFileList(fileName, ".mx2", &countMx2);
                        char* fileListCol = zipGetFileList(fileName, ".col", &countCol);
                        char* fileListSg  = zipGetFileList(fileName, ".sg", &countSg);
                        char* fileListSc  = zipGetFileList(fileName, ".sc", &countSc);
                        int count = countRom + countRi + countMx1 + countMx2 + countCol + countSg;

                        if (count == 1) {
                            if (countRom == 1) {
                                buf = romLoad(fileName, fileListRom, &size);
                            }
                            if (countRi == 1) {
                                buf = romLoad(fileName, fileListRi, &size);
                            }
                            if (countMx1 == 1) {
                                buf = romLoad(fileName, fileListMx1, &size);
                            }
                            if (countMx2 == 1) {
                                buf = romLoad(fileName, fileListMx2, &size);
                            }
                            if (countCol == 1) {
                                buf = romLoad(fileName, fileListCol, &size);
                            }
                            if (countSg == 1) {
                                buf = romLoad(fileName, fileListSg, &size);
                            }
                            if (countSc == 1) {
                                buf = romLoad(fileName, fileListSc, &size);
                            }
                        }

                        if (fileListRom) free(fileListRom);
                        if (fileListRi)  free(fileListRi);
                        if (fileListMx1) free(fileListMx1);
                        if (fileListMx2) free(fileListMx2);
                        if (fileListCol) free(fileListCol);
                        if (fileListSg)  free(fileListSg);
                        if (fileListSc)  free(fileListSc);
                    }
                    else {
                        buf = romLoad(fileName, NULL, &size);
                    }
            
                    if (buf != NULL) {

                        MediaType* mediaType = mediaDbLookupRom(buf, size);
                        RomType romType = mediaType != NULL ? mediaDbGetRomType(mediaType) : ROM_UNKNOWN;
                        int idx = 0;

                        while (romTypeList[idx] != romType) {
                            idx++;
                        }

                        SendMessage(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), CB_SETCURSEL, idx, 0);

                        free(buf);

                        openRomType = romType;

                        EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), 1);
                    }    
                    else {
                        openRomType = ROM_UNKNOWN;
                        EnableWindow(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), 0);
                        SendMessage(GetDlgItem(hDlg, IDC_OPEN_ROMTYPE), CB_SETCURSEL, -1, 0);
                    }
                }
                break;
            }
        }
        return 0;
    }

    return 0;
}

char* openRomFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, int mustExist, 
                  char* defExt, int* filterIndex, RomType* romType)
{ 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH];
    FILE* file;

    pFileName[0] = 0; 
    *romType = ROM_UNKNOWN;

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = filterIndex ? *filterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_HIDEREADONLY | (mustExist ? OFN_FILEMUSTEXIST : 0); 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookRomProc; 
    ofn.lpTemplateName = MAKEINTRESOURCE(IDD_OPEN_ROMDROPDOWN); 

    rv = GetOpenFileName(&ofn); 

    if (!rv) {
        return NULL; 
    }

    if (filterIndex) {
        *filterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    file = fopen(pFileName, "r");
    if (file != NULL) {
        fclose(file);
    }
    else {
        if (defExt) {
            if (strlen(pFileName) <= strlen(defExt)) {
                strcat(pFileName, defExt);
            }
            else {
                char* pos = pFileName + strlen(pFileName) - strlen(defExt);
                int  len  = strlen(defExt);
                while (len--) {
                    if (toupper(pos[len]) != toupper(defExt[len])) {
                        break;
                    }
                }
                if (len >= 0) {
                    strcat(pFileName, defExt);
                }
            }
        }
        file = fopen(pFileName, "a+");
        if (file != NULL) {
            fclose(file);
        }
    }

    *romType = openRomType;

    return pFileName; 
} 

//////////////////////////////////////////////////////////////////////////////////////

static int doShowPreview = 0;

UINT_PTR CALLBACK hookStateProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBmp = INVALID_HANDLE_VALUE;

    switch (iMsg) {
	case WM_DIALOGRESIZE:
        updateDialogPos(GetParent(hDlg), DLG_ID_OPENSTATE, 0, 0);
        return 0;

    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlg, IDC_PREVIEWBUTTON), langDlgSavePreview());
        SetWindowText(GetDlgItem(hDlg, IDC_PREVIEWDATETEXT), langDlgSaveDate());
        SendDlgItemMessage(hDlg, IDC_PREVIEWBUTTON, BM_SETCHECK, doShowPreview ? BST_CHECKED : BST_UNCHECKED, 0);
        return 0;

	case WM_COMMAND:
        if (LOWORD(wParam) == IDC_PREVIEWBUTTON) {
            int newChecked = BST_CHECKED == SendDlgItemMessage(hDlg, IDC_PREVIEWBUTTON, BM_GETCHECK, 0, 0);
            if (newChecked != doShowPreview) {
                doShowPreview = newChecked;
                InvalidateRect(hDlg, NULL, TRUE);
            }
        }
        return 0;

    case WM_SIZE:
        {
            RECT r;
            int height;
            int width;
            HWND hwnd;

            GetClientRect(GetParent(hDlg), &r);
            
            height = r.bottom - r.top;
            width  = r.right - r.left;

            hwnd = GetDlgItem(hDlg, IDC_PREVIEWBUTTON);
            SetWindowPos(hwnd, NULL, width - 220, 215, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            
            hwnd = GetDlgItem(hDlg, IDC_PREVIEWDATETEXT);
            SetWindowPos(hwnd, NULL, 8, height - 26, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            hwnd = GetDlgItem(hDlg, IDC_PREVIEWDATE);
            SetWindowPos(hwnd, NULL, 81, height - 26, width - 187, 15, SWP_NOZORDER);
        }
        return 0;
        
    case WM_DESTROY:
        if (hBmp != INVALID_HANDLE_VALUE) {
            DeleteObject(hBmp);
            hBmp = INVALID_HANDLE_VALUE;
        }
        saveDialogPos(GetParent(hDlg), DLG_ID_OPENSTATE);
        return 0;
        
    case WM_NOTIFY:
        {
            OFNOTIFY* ofn = (OFNOTIFY*)lParam;
            switch (ofn->hdr.code) {
			case CDN_INITDONE:
				//It is not effective since the second times why. 
				updateDialogPos(GetParent(hDlg), DLG_ID_OPENSTATE, 0, 1);
				PostMessage(hDlg, WM_DIALOGRESIZE, 0, 0);
				break;

            case CDN_SELCHANGE:
                {
                    char fileName[MAX_PATH];
                    void* buffer;
                    Int32 size;

                    int fileSize = SendMessage(GetParent(hDlg), CDM_GETFILEPATH, MAX_PATH, (LPARAM)fileName);

                    if (hBmp != INVALID_HANDLE_VALUE) {
                        DeleteObject(hBmp);
                        hBmp = INVALID_HANDLE_VALUE;
                    }

                    SetWindowText(GetDlgItem(hDlg, IDC_PREVIEWDATE), "");                     
                    buffer = zipLoadFile(fileName, "date.txt", &size);

                    if (buffer != 0) {
                        SetWindowText(GetDlgItem(hDlg, IDC_PREVIEWDATE), buffer);     

                        free(buffer);

                    }

                    if (isFileExtension(fileName, ".sta")) {

                        buffer = zipLoadFile(fileName, "screenshot.bmp", &size);

                        if (buffer != 0) {

                            hBmp = BitmapFromData(buffer);

                            free(buffer);

                        }


                        InvalidateRect(hDlg, NULL, TRUE);
                    }
                }
                break;
            }
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hDlg, &ps);

            if (hBmp != INVALID_HANDLE_VALUE && doShowPreview) {
                BITMAP bmp;

                if(GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) {
                    HDC hMemDC = CreateCompatibleDC(hdc);
                    HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, hBmp);
                    int bmWidth = 200;
                    int bmHeight = bmp.bmHeight * bmWidth / bmp.bmWidth;
                    RECT r;
                    int height;
                    int width;

                    if (bmHeight > 180) {
                        bmHeight = 180;
                        bmWidth = bmp.bmWidth * bmHeight / bmp.bmHeight;
                    }

                    GetClientRect(GetParent(hDlg), &r);
                
                    height = r.bottom - r.top;
                    width  = r.right - r.left;
                    SetStretchBltMode(hdc, HALFTONE);
                    StretchBlt(hdc, width - 220, 30, bmWidth, bmHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
                    SelectObject(hMemDC, hBitmap);
                    DeleteDC(hMemDC);    
                }
            }
            EndPaint(hDlg, &ps);
        }
        return 0;
    }

    return 0;
}

char* openStateFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
                    int newFileSize, char* defExt, int* filterIndex, int* showPreview)
{ 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH];
    FILE* file;

    pFileName[0] = 0; 

    if (showPreview != NULL) {
        doShowPreview = *showPreview;
    }
    else {
        doShowPreview = 1;
    }

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = filterIndex ? *filterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_HIDEREADONLY | (newFileSize < 0 ? OFN_FILEMUSTEXIST : 0); 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookStateProc; 
    ofn.lpTemplateName = MAKEINTRESOURCE(IDD_OPEN_STATEDIALOG); 

    rv = GetOpenFileName(&ofn); 

    if (showPreview != NULL) {
        *showPreview = doShowPreview;
    }

    if (!rv) {
        return NULL; 
    }

    if (filterIndex) {
        *filterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    file = fopen(pFileName, "r");
    if (file != NULL) {
        fclose(file);
    }
    else {
        if (defExt) {
            if (strlen(pFileName) <= strlen(defExt)) {
                strcat(pFileName, defExt);
            }
            else {
                char* pos = pFileName + strlen(pFileName) - strlen(defExt);
                int  len  = strlen(defExt);
                while (len--) {
                    if (toupper(pos[len]) != toupper(defExt[len])) {
                        break;
                    }
                }
                if (len >= 0) {
                    strcat(pFileName, defExt);
                }
            }
        }
        file = fopen(pFileName, "a+");
        if (file != NULL) {
            fclose(file);
        }
    }

    return pFileName; 
} 

char* saveStateFile(HWND hwndOwner, char* pTitle, char* pFilter, int* pFilterIndex, char* pDir, int* showPreview) { 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH]; 
    pFileName[0] = 0; 

    if (showPreview != NULL) {
        doShowPreview = *showPreview;
    }
    else {
        doShowPreview = 1;
    }

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = pFilterIndex ? *pFilterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_HIDEREADONLY | OFN_ENABLEHOOK; 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookStateProc; 
    ofn.lpTemplateName = MAKEINTRESOURCE(IDD_OPEN_STATEDIALOG); 

    rv = GetSaveFileName(&ofn); 

    if (showPreview != NULL) {
        *showPreview = doShowPreview;
    }

    if (!rv) { 
        return NULL; 
    } 

    if (pFilterIndex) {
        *pFilterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    return pFileName; 
}

//////////////////////////////////////////////////////////////////

static int newHdFileSize;

#define ONEMB (1024 * 1024)

static const struct {
    int size;
    char text[8];
} hdFileSizes[] = {
    {   5 * ONEMB, "5 MB" },
    {  10 * ONEMB, "10 MB" },
    {  20 * ONEMB, "20 MB" },
    {  50 * ONEMB, "50 MB" },
    { 100 * ONEMB, "100 MB" },
    { 200 * ONEMB, "200 MB" },
    { 0, "" }
};

UINT_PTR CALLBACK hookHdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
	case WM_DIALOGRESIZE:
        updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 0);
        return 0;

    case WM_INITDIALOG:
        {
            int i;

            for (i = 0; hdFileSizes[i].size; i++) {
                SendDlgItemMessage(hDlg, IDC_OPEN_HDSIZE, CB_ADDSTRING, 0, (LPARAM)hdFileSizes[i].text);
                if (newHdFileSize == hdFileSizes[i].size || i == 0) {
                    SendDlgItemMessage(hDlg, IDC_OPEN_HDSIZE, CB_SETCURSEL, i, 0);
                }
            }
//            SetWindowText(GetDlgItem(hDlg, IDC_OPEN_HDSIZETEXT), langDlgRomType());
        }
        return 0;

    case WM_SIZE:
        {
            RECT r;
            int height;
            int width;
            HWND hwnd;

            GetClientRect(GetParent(hDlg), &r);
            
            height = r.bottom - r.top;
            width  = r.right - r.left;

            hwnd = GetDlgItem(hDlg, IDC_OPEN_HDSIZETEXT);
            SetWindowPos(hwnd, NULL, 8, height - 29, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            hwnd = GetDlgItem(hDlg, IDC_OPEN_HDSIZE);
            SetWindowPos(hwnd, NULL, 81, height - 32, 100, 12, SWP_NOZORDER);
        }
        return 0;

    case WM_DESTROY:
        {
            int idx = SendMessage(GetDlgItem(hDlg, IDC_OPEN_HDSIZE), CB_GETCURSEL, 0, 0);
            if (idx < 0) {
                char buf[128];
                int size;
                GetDlgItemText(hDlg, IDC_OPEN_HDSIZE, buf, 127);
                size = atoi(buf);
                if (size <= 0 && size > 1024) {
                    size = 5;
                }
                newHdFileSize = size * 1024 * 1024;
            }
            else {
                newHdFileSize = hdFileSizes[idx].size;
            }
            saveDialogPos(GetParent(hDlg), DLG_ID_OPEN);
        }
        return 0;
        
    case WM_NOTIFY:
        {
            OFNOTIFY* ofn = (OFNOTIFY*)lParam;
            switch (ofn->hdr.code) {
			case CDN_INITDONE:
				//It is not effective since the second times why. 
				updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 1);
				PostMessage(hDlg, WM_DIALOGRESIZE, 0, 0);
				break;
            }
        }
        return 0;
    }

    return 0;
}

char* openNewHdFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
                    char* defExt, int* filterIndex)
{ 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH];
    FILE* file;
    
    newHdFileSize = 20 * ONEMB;

    pFileName[0] = 0; 

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = filterIndex ? *filterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_HIDEREADONLY; 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookHdProc; 
    ofn.lpTemplateName = MAKEINTRESOURCE(IDD_OPEN_HDSIZEDROPDOWN); 

    rv = GetOpenFileName(&ofn); 

    if (!rv) {
        return NULL; 
    }

    if (filterIndex) {
        *filterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    file = fopen(pFileName, "r");
    if (file != NULL) {
        char langBuffer[200];
        fclose(file);
        sprintf(langBuffer, "%s %s", langWarningOverwriteFile(), pFileName);
        if (IDOK != MessageBox(NULL, langBuffer, langWarningTitle(), MB_OKCANCEL)) {
            return NULL;
        }
    }

    if (defExt) {
        if (strlen(pFileName) <= strlen(defExt)) {
            strcat(pFileName, defExt);
        }
        else {
            char* pos = pFileName + strlen(pFileName) - strlen(defExt);
            int  len  = strlen(defExt);
            while (len--) {
                if (toupper(pos[len]) != toupper(defExt[len])) {
                    break;
                }
            }
            if (len >= 0) {
                strcat(pFileName, defExt);
            }
        }
    }
    file = fopen(pFileName, "w+");
    if (file != NULL) {
        char* data = calloc(1, ONEMB);
        while (newHdFileSize > 0) {
            fwrite(data, 1, ONEMB, file);
            newHdFileSize -= ONEMB;
        }
        free(data);
        fclose(file);
    }

    return pFileName; 
} 
//////////////////////////////////////////////////////////////////

static int newDskFileSize;

#define ONEKB 1024

static const struct {
    int size;
    char* (*translation)();
} dskFileSizes[] = {
    { 720 * ONEKB, langEnumDiskMsx35Dbl9Sect },
    { 640 * ONEKB, langEnumDiskMsx35Dbl8Sect },
    { 360 * ONEKB, langEnumDiskMsx35Sgl9Sect },
    { 320 * ONEKB, langEnumDiskMsx35Sgl8Sect },
    { 338 * ONEKB, langEnumDiskSvi525Dbl },
    { 168 * ONEKB, langEnumDiskSvi525Sgl },
    { 0, NULL }
};

UINT_PTR CALLBACK hookDskProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
	case WM_DIALOGRESIZE:
        updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 0);
        return 0;

    case WM_INITDIALOG:
        {
            int i;

            for (i = 0; dskFileSizes[i].size; i++) {
                SendDlgItemMessage(hDlg, IDC_OPEN_HDSIZE, CB_ADDSTRING, 0, (LPARAM)dskFileSizes[i].translation());
                if (newDskFileSize == dskFileSizes[i].size || i == 0) {
                    SendDlgItemMessage(hDlg, IDC_OPEN_HDSIZE, CB_SETCURSEL, i, 0);
                }
            }
        }
        return 0;

    case WM_SIZE:
        {
            RECT r;
            int height;
            int width;
            HWND hwnd;

            GetClientRect(GetParent(hDlg), &r);
            
            height = r.bottom - r.top;
            width  = r.right - r.left;

            hwnd = GetDlgItem(hDlg, IDC_OPEN_HDSIZETEXT);
            SetWindowPos(hwnd, NULL, 8, height - 29, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            hwnd = GetDlgItem(hDlg, IDC_OPEN_HDSIZE);
            SetWindowPos(hwnd, NULL, 81, height - 32, 233, 12, SWP_NOZORDER);
        }
        return 0;

    case WM_DESTROY:
        {
            int idx = SendMessage(GetDlgItem(hDlg, IDC_OPEN_HDSIZE), CB_GETCURSEL, 0, 0);
            saveDialogPos(GetParent(hDlg), DLG_ID_OPEN);
            newDskFileSize = dskFileSizes[idx].size;
        }
        return 0;
        
    case WM_NOTIFY:
        {
            OFNOTIFY* ofn = (OFNOTIFY*)lParam;
            switch (ofn->hdr.code) {
			case CDN_INITDONE:
				//It is not effective since the second times why. 
				updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 1);
				PostMessage(hDlg, WM_DIALOGRESIZE, 0, 0);
				break;
            }
        }
        return 0;
    }

    return 0;
}

char* openNewDskFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
                    char* defExt, int* filterIndex)
{ 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH];
    FILE* file;
    
    newDskFileSize = 720 * ONEKB;

    pFileName[0] = 0; 

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = filterIndex ? *filterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_HIDEREADONLY; 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookDskProc; 
    ofn.lpTemplateName = MAKEINTRESOURCE(IDD_OPEN_HDSIZEDROPDOWN); 

    rv = GetOpenFileName(&ofn); 

    if (!rv) {
        return NULL; 
    }

    if (filterIndex) {
        *filterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    file = fopen(pFileName, "r");
    if (file != NULL) {
        char langBuffer[200];
        fclose(file);
        sprintf(langBuffer, "%s %s", langWarningOverwriteFile(), pFileName);
        if (IDOK != MessageBox(NULL, langBuffer, langWarningTitle(), MB_OKCANCEL)) {
            return NULL;
        }
    }

    if (defExt) {
        if (strlen(pFileName) <= strlen(defExt)) {
            strcat(pFileName, defExt);
        }
        else {
            char* pos = pFileName + strlen(pFileName) - strlen(defExt);
            int  len  = strlen(defExt);
            while (len--) {
                if (toupper(pos[len]) != toupper(defExt[len])) {
                    break;
                }
            }
            if (len >= 0) {
                strcat(pFileName, defExt);
            }
        }
    }
    file = fopen(pFileName, "w+");
    if (file != NULL) {
        char* data = calloc(1, ONEKB);
        if (newDskFileSize == 338 * ONEKB || newDskFileSize == 168 * ONEKB) {
            memset(data, 0xe5, ONEKB);
        }
        while (newDskFileSize > 0) {
            fwrite(data, 1, ONEKB, file);
            newDskFileSize -= ONEKB;
        }
        free(data);
        fclose(file);
    }

    return pFileName; 
} 

//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////

UINT_PTR CALLBACK hookProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

    switch (iMsg) {
	case WM_DIALOGRESIZE:
        updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 0);
        return 0;

	case WM_NOTIFY:
		{
			OFNOTIFY* ofn = (OFNOTIFY*)lParam;
			if(ofn->hdr.code == CDN_INITDONE){
				//It is not effective since the second times why. 
				updateDialogPos(GetParent(hDlg), DLG_ID_OPEN, 0, 1);
				PostMessage(hDlg, WM_DIALOGRESIZE, 0, 0);
			}
			return 0;
		}

    case WM_DESTROY:
        saveDialogPos(GetParent(hDlg), DLG_ID_OPEN);
        return 0;
    }

    return 0;
}

char* openFile(HWND hwndOwner, char* pTitle, char* pFilter, char* pDir, 
               int newFileSize, char* defExt, int* filterIndex)
{ 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH];
    FILE* file;

    pFileName[0] = 0; 

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = filterIndex ? *filterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLEHOOK | OFN_HIDEREADONLY | (newFileSize < 0 ? OFN_FILEMUSTEXIST : 0); 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookProc; 
    ofn.lpTemplateName = NULL; 

    rv = GetOpenFileName(&ofn); 

    if (!rv) {
        return NULL; 
    }

    if (filterIndex) {
        *filterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    file = fopen(pFileName, "r");
    if (file != NULL) {
        fclose(file);
    }
    else {
        if (defExt) {
            if (strlen(pFileName) <= strlen(defExt)) {
                strcat(pFileName, defExt);
            }
            else {
                char* pos = pFileName + strlen(pFileName) - strlen(defExt);
                int  len  = strlen(defExt);
                while (len--) {
                    if (toupper(pos[len]) != toupper(defExt[len])) {
                        break;
                    }
                }
                if (len >= 0) {
                    strcat(pFileName, defExt);
                }
            }
        }
        file = fopen(pFileName, "a+");
        if (file != NULL) {
            if (newFileSize > 0) {
                char* data = calloc(1, newFileSize);
                fwrite(data, 1, newFileSize, file);
                free(data);
            }
            fclose(file);
        }
    }

    return pFileName; 
} 

char* saveFile(HWND hwndOwner, char* pTitle, char* pFilter, int* pFilterIndex, char* pDir) { 
    OPENFILENAME ofn; 
    BOOL rv; 
    static char pFileName[MAX_PATH]; 
    pFileName[0] = 0; 

    ofn.lStructSize = sizeof(OPENFILENAME); 
    ofn.hwndOwner = hwndOwner; 
    ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL); 
    ofn.lpstrFilter = pFilter ? pFilter : "*.*\0\0"; 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0; 
    ofn.nFilterIndex = pFilterIndex ? *pFilterIndex : 0; 
    ofn.lpstrFile = pFileName; 
    ofn.nMaxFile = 1024; 
    ofn.lpstrFileTitle = NULL; 
    ofn.nMaxFileTitle = 0; 
    ofn.lpstrInitialDir = pDir; 
    ofn.lpstrTitle = pTitle; 
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_ENABLEHOOK; 
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0; 
    ofn.lpstrDefExt = NULL; 
    ofn.lCustData = 0; 
    ofn.lpfnHook = hookProc; 
    ofn.lpTemplateName = NULL; 

    rv = GetSaveFileName(&ofn); 

    if (!rv) { 
        return NULL; 
    } 

    if (pFilterIndex) {
        *pFilterIndex = ofn.nFilterIndex;
    }

    if (pDir != NULL) {
        GetCurrentDirectory(MAX_PATH - 1, pDir);
    }

    return pFileName; 
} 


///////////////////////////////////////////////////////////////////////////

typedef struct {

    char*  title;

    char*  description;

    char** itemList;

    char*  defaultName;

    char*  returnName;

} SaveAsDlgInfo;



static BOOL CALLBACK saveAsProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{

    static SaveAsDlgInfo* sdi;
    char buffer[64];
    int i;

    switch (iMsg) {
    case WM_INITDIALOG:
        sdi = (SaveAsDlgInfo*)lParam;

        SetWindowText(hwnd, sdi->title);
        SetWindowText(GetDlgItem(hwnd, IDC_MACHINENAMETEXT), sdi->description);
        SetWindowText(GetDlgItem(hwnd, IDOK), langDlgSave());
        SetWindowText(GetDlgItem(hwnd, IDCANCEL), langDlgCancel());

        for (i = 0; sdi->itemList[i] != NULL; i++) {
            SendDlgItemMessage(hwnd, IDC_MACHINELIST, LB_ADDSTRING, 0, (LPARAM)sdi->itemList[i]);
            if (0 == strcmpnocase(sdi->itemList[i], sdi->defaultName)) {
                SetWindowText(GetDlgItem(hwnd, IDC_MACHINENAME), sdi->defaultName);
                SendDlgItemMessage(hwnd, IDC_MACHINELIST, LB_SETCURSEL, i, 0);
                EnableWindow(GetDlgItem(hwnd, IDOK), TRUE);
            }
        }
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_MACHINELIST:
            if (HIWORD(wParam) == 1 || HIWORD(wParam) == 2) {
                char buffer[64];
                int index = SendMessage(GetDlgItem(hwnd, IDC_MACHINELIST), LB_GETCURSEL, 0, 0);
                SendMessage(GetDlgItem(hwnd, IDC_MACHINELIST), LB_GETTEXT, index, (LPARAM)buffer);
                SetWindowText(GetDlgItem(hwnd, IDC_MACHINENAME), buffer);
                if (HIWORD(wParam) == 2) {
                    SendMessage(hwnd, WM_COMMAND, IDOK, 0);
                }
            }
            return TRUE;

        case IDC_MACHINENAME:
            GetWindowText(GetDlgItem(hwnd, IDC_MACHINENAME), buffer, 63);

            EnableWindow(GetDlgItem(hwnd, IDOK), strlen(buffer) != 0);      

            SendDlgItemMessage(hwnd, IDC_MACHINELIST, LB_SETCURSEL, -1, 0);

            for (i = 0; sdi->itemList[i] != NULL; i++) {
                if (0 == strcmpnocase(sdi->itemList[i], buffer)) {
                    SendDlgItemMessage(hwnd, IDC_MACHINELIST, LB_SETCURSEL, i, 0);
                }
            }
            return TRUE;
        case IDOK:
            GetWindowText(GetDlgItem(hwnd, IDC_MACHINENAME), sdi->returnName, 63);
            EndDialog(hwnd, TRUE);
            return TRUE;
        case IDCANCEL:
            EndDialog(hwnd, FALSE);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, FALSE);
        return TRUE;
    }

    return FALSE;
}

char* openConfigFile(HWND parent, char* title, char* description,
                     char** itemList, char* defaultName)
{
    static char returnName[MAX_PATH];
    SaveAsDlgInfo* sdi = (SaveAsDlgInfo*)calloc(1, sizeof(SaveAsDlgInfo));
    int rv;

    sdi->title       = title;
    sdi->description = description;
    sdi->itemList    = itemList;
    sdi->defaultName = defaultName;
    sdi->returnName  = returnName;

    rv = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_SAVEAS), parent, saveAsProc, (LPARAM)sdi);
    free(sdi);

    return rv ? returnName : NULL;

}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32ShortcutsConfig.c,v $
**
** $Revision: 1.33 $
**
** $Date: 2008-05-06 17:48:55 $
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
#include <direct.h>
#include <stdio.h>
#include <string.h>
 
#include "Win32ShortcutsConfig.h"
#include "Win32Common.h"
#include "Win32keyboard.h"
#include "IniFileParser.h"
#include "Resource.h"



// PacketFileSystem.h Need to be included after all other includes
#include "PacketFileSystem.h"


#define WM_INITIALIZE (WM_USER + 1634)
#define WM_SET_HOTKEY (WM_USER + 1635)
#define WM_GET_HOTKEY (WM_USER + 1636)

static char virtualKeys[256][32] = {
    "",
    "", //"LBUTTON", 
    "", //"RBUTTON",
    "Cancel",
    "", //"MBUTTON",
    "", //"XBUTTON1",
    "", //"XBUTTON2",
    "",
    "Backspace",
    "Tab",
    "",
    "",
    "Clear",
    "Enter",
    "",
    "",
    "", // SHIFT
    "", // CTRL
    "", // ALT
    "Pause",
    "CapsLk",
    "Kana",
    "",
    "Junja",
    "Final",
    "Kanji",
    "",
    "esc",
    "Conv",
    "NoConv",
    "Accept",
    "ModeCh",
    "Space",
    "PgUp",
    "PgDown",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "Select",
    "Print",
    "Exec",
    "PrScr",
    "Ins",
    "Del",
    "Help",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "LWIN",
    "RWIN",
    "APPS",
    "",
    "Sleep",
    "Num0",
    "Num1",
    "Num2",
    "Num3",
    "Num4",
    "Num5",
    "Num6",
    "Num7",
    "Num8",
    "Num9",
    "Num*",
    "Num+",
    "Num,",
    "Num-",
    "Num.",
    "Num/",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "NumLk",
    "ScrLk",
    "Oem1",
    "Oem2",
    "Oem3",
    "Oem4",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "LShift",
    "RShift",
    "LCTRL",
    "RCONTROL",
    "LALT",
    "RALT",
    "BrBack",
    "BrForward",
    "BrRefresh",
    "BrStop",
    "BrSearch",
    "BrFavorites",
    "BrHome",
    "VolMute",
    "VolDown",
    "VolUp",
    "NextTrk",
    "PrevTrk",
    "MdStop",
    "MdPlay",
    "Mail",
    "MdSelect",
    "App1",
    "App2",
    "",
    "",
    ";",
    "+",
    ",",
    "-",
    ".",
    "?",
    "~",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "[",
    "\\",
    "]",
    "\"",
    "Oem5",
    "",
    "Oem6",
    "Oem7",
    "Oem8",
    "Oem9",
    "Process",
    "Oem10",
    "",
    "",
    "Oem11",
    "Oem12",
    "Oem13",
    "Oem14",
    "Oem15",
    "Oem16",
    "Oem17",
    "Oem18",
    "Oem19",
    "Oem20",
    "Oem21",
    "Oem22",
    "Oem23",
    "Addn",
    "CrSel",
    "ExSel",
    "Ereof",
    "Play",
    "Zoom",
    "",
    "PA1",
    "Clear",
    ""
};

static char       profileDir[] = "Shortcut Profiles";
static char       shortcutProfile[128];
static char       tmpShortcutProfile[128];
static WNDPROC    baseHotkeyCtrlProc = NULL;
static HWND       baseHwnd;
static Shortcuts* shortcuts;
static Shortcuts* shortcutsRef;
static ShotcutHotkey* hotkeyList[128];


static DWORD hotkey2int(ShotcutHotkey hotkey) {
    return (hotkey.key<<16)|(hotkey.mods<<8)|hotkey.type;
}

static ShotcutHotkey int2hotkey(DWORD* hotkey) {
    return *(ShotcutHotkey*)hotkey;
}

static char** getProfileList() 
{
    static char profileArray[128][64];
    static char* profileList[128];
    char fileName[MAX_PATH];
	HANDLE handle;
	WIN32_FIND_DATA wfd;
    int index = 0;
    BOOL cont;
    
    sprintf(fileName, "%s/*.shortcuts", profileDir);

    handle = FindFirstFile(fileName, &wfd);
    
    cont = handle != INVALID_HANDLE_VALUE;
    
    while (cont) {
		DWORD fa = GetFileAttributes(wfd.cFileName);

        if (fa & FILE_ATTRIBUTE_NORMAL) {
            char buffer[128];
            int length = strlen(wfd.cFileName) - 10;
            strcpy(buffer, wfd.cFileName);
            buffer[length] = 0;
            strcpy(profileArray[index], buffer);
            profileList[index] = (char*)profileArray[index];
            index++;
        }   
        cont = FindNextFile(handle, &wfd);
    }
    
    if (handle != INVALID_HANDLE_VALUE) FindClose(handle);
    profileList[index] = NULL;

    return profileList;
}

static BOOL CALLBACK saveProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        {
            char buffer[128];
            SetWindowText(hDlg, langShortcutSaveConfig());

            sprintf(buffer, "%s\n\n    \"%s\" ?", langShortcutOverwriteConfig(), shortcutProfile);

            SetWindowText(GetDlgItem(hDlg, IDC_CONF_SAVEDLG_TEXT), buffer);
            SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
            SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());
        }
        return FALSE;

    case WM_COMMAND:
        switch (wParam) {
        case IDOK:
            EndDialog(hDlg, TRUE);
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

static BOOL CALLBACK closeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        SetWindowText(hDlg, langShortcutExitConfig());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SAVEDLG_TEXT), langShortcutDiscardConfig());

        return FALSE;

    case WM_COMMAND:
        switch (wParam) {
        case IDOK:
            EndDialog(hDlg, TRUE);
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

static BOOL CALLBACK discardProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        SetWindowText(hDlg, "blueMSX - Shortcut Configuration");
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SAVEDLG_TEXT), "Do you want to discard changes to the current configuration?");
        return FALSE;

    case WM_COMMAND:
        switch (wParam) {
        case IDOK:
            EndDialog(hDlg, TRUE);
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

#include "Win32machineConfig.h"

static BOOL CALLBACK saveAsProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        SetWindowText(hDlg, langShortcutSaveConfigAs());
        SetWindowText(GetDlgItem(hDlg, IDC_MACHINENAMETEXT), langShortcutConfigName());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgSave());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());

        {
            char** profileList = getProfileList();
            int index = 0;

            EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
                    
            while (profileList[index] != NULL) {
                SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_ADDSTRING, 0, (LPARAM)profileList[index]);
                if (0 == strcmpnocase(profileList[index], shortcutProfile) && strcmp(shortcutProfile, langShortcutNewProfile())) {
                    SetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), shortcutProfile);
                    SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_SETCURSEL, index, 0);
                    EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
                }
                index++;
            }
        }

        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_MACHINELIST:
            if (HIWORD(wParam) == 1 || HIWORD(wParam) == 2) {
                char buffer[64];
                int index = SendMessage(GetDlgItem(hDlg, IDC_MACHINELIST), LB_GETCURSEL, 0, 0);
                SendMessage(GetDlgItem(hDlg, IDC_MACHINELIST), LB_GETTEXT, index, (LPARAM)buffer);
                SetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), buffer);
                if (HIWORD(wParam) == 2) {
                    SendMessage(hDlg, WM_COMMAND, IDOK, 0);
                }
            }
            return TRUE;

        case IDC_MACHINENAME:
            {
                char sel[64];

                GetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), sel, 63);

                EnableWindow(GetDlgItem(hDlg, IDOK), strlen(sel) != 0);      

                SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_SETCURSEL, -1, 0);

                if (strlen(sel)) {
                    char** profileList = getProfileList();
                    int index = 0;

                    while (profileList[index] != NULL) {
                        if (0 == strcmpnocase(sel, profileList[index])) {
                            SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_SETCURSEL, index, 0);
                        }
                        index++;
                    }
                }
            }
            return TRUE;

        case IDOK:
            GetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), tmpShortcutProfile, 63);
            EndDialog(hDlg, TRUE);
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

static LRESULT CALLBACK hotkeyCtrlProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static DWORD hotkey;
    static int modifiers;
    static int virtKey;
    static int joyButtonState;
    static int joyButton;
    static int keycount;

    switch (iMsg) {
    case WM_INITIALIZE:
        modifiers = 0;
        virtKey = 0;
        keycount = 0;
        joyButton = -1;
        SetTimer(hwnd, 154, 100, NULL);
        joystickUpdate();
        joyButtonState = joystickGetButtonState();
        return 0;

    case WM_SET_HOTKEY:
        {
            ShotcutHotkey hotkey = int2hotkey((DWORD*)&wParam);
            modifiers = 0;
            virtKey   = 0;
            joyButton = -1;
            if (hotkey.type == HOTKEY_TYPE_KEYBOARD) {
                modifiers = hotkey.mods;
                virtKey   = hotkey.key & 0xff;
            }
            else if (hotkey.type == HOTKEY_TYPE_JOYSTICK) {
                joyButton = hotkey.key & 0x1f;
            }
            InvalidateRect(hwnd, NULL, FALSE);
        }

    case WM_GET_HOTKEY:
        {
            ShotcutHotkey hotkey = { HOTKEY_TYPE_NONE, 0, 0 };
            if (joyButton >= 0) {
                hotkey.type = HOTKEY_TYPE_JOYSTICK;
                hotkey.key  = joyButton + 1;
            }
            else if (virtKey != 0) {
                hotkey.type = HOTKEY_TYPE_KEYBOARD;
                hotkey.key  = virtKey;
                hotkey.mods = modifiers;
            }
            return hotkey2int(hotkey);
        }

    case WM_TIMER:
        if (wParam == 154) {
            int i;
            DWORD newState;
            
            joystickUpdate();

            newState = joystickGetButtonState();
            
            if (keycount == 0) {
                for (i = 0; i < 32; i++) {
                    if (newState & ~joyButtonState & (1 << i))  {
                        if (joyButton != i) {
                            modifiers = 0;
                            virtKey = 0;
                            joyButton = i;
                            InvalidateRect(hwnd, NULL, FALSE);
                        }

                        break;
                    }
                }
            }
            joyButtonState = newState;
            return 0;
        }
        break;

    case WM_GETDLGCODE:
        return DLGC_WANTCHARS | DLGC_WANTARROWS;
        
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        joyButton = -1;
        keycount++;
        keyboardUpdate();
        modifiers = keyboardGetModifiers();
        virtKey = wParam & 0xff;
        if (!virtualKeys[virtKey][0]) {
            virtKey = 0;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (keycount > 0) keycount--;
        if (virtKey == 0) {
            virtKey = wParam & 0xff;
            if (!virtualKeys[virtKey][0]) {
                virtKey = 0;
            }
        }
        if (virtKey == 0) {
            keyboardUpdate();
            modifiers = keyboardGetModifiers();
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 154);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            char buf[64] = "";
            RECT r;
            HFONT hFont;

            GetClientRect(hwnd, &r);

            if (modifiers & KBD_LCTRL)  strcat(buf, "LCtrl + ");
            if (modifiers & KBD_RCTRL)  strcat(buf, "RCtrl + ");
            if (modifiers & KBD_LSHIFT) strcat(buf, "LShift + ");
            if (modifiers & KBD_RSHIFT) strcat(buf, "RShift + ");
            if (modifiers & KBD_LALT)   strcat(buf, "LAlt + ");
            if (modifiers & KBD_RALT)   strcat(buf, "RAlt + ");
            if (modifiers & KBD_LWIN)   strcat(buf, "LWin + ");
            if (modifiers & KBD_RWIN)   strcat(buf, "RWin + ");
             
            FillRect(hdc, &r, GetStockObject(WHITE_BRUSH)); 

            strcat(buf, virtualKeys[virtKey]);

            if (buf[0] == 0 && joyButton >= 0) {
                sprintf(buf, "%s %d", "Joystick Button", joyButton + 1);
            }
            hFont = SelectObject(hdc, (HFONT)SendMessage(baseHwnd, WM_GETFONT, 0, 0));
            
            TextOut(hdc, 2, 1, buf, strlen(buf));

            SelectObject(hdc, hFont);
            EndPaint(hwnd, &ps);

            return 0;
        }
    }

    return CallWindowProc(baseHotkeyCtrlProc, hwnd, iMsg, wParam, lParam);
}

#define LOAD_SHORTCUT(iniFile, hotkey)                                          \
do {                                                                            \
    char buffer[32];                                                            \
    DWORD value;                                                                \
    iniFileGetString(iniFile, "Shortcuts", #hotkey, "0", buffer, 32);           \
    if (0 == sscanf(buffer, "%X", &value)) value = 0;                           \
    shortcuts->hotkey = int2hotkey(&value);                                     \
} while(0)

#define SAVE_SHORTCUT(iniFile, hotkey)                                          \
do {                                                                            \
    char buffer[32];                                                            \
    sprintf(buffer, "%.8X", hotkey2int(shortcuts->hotkey));                     \
    iniFileWriteString(iniFile, "Shortcuts", #hotkey, buffer);                  \
} while(0)


static Shortcuts* loadShortcuts(char* profileName)
{
    char fileName[MAX_PATH];
    Shortcuts* shortcuts = (Shortcuts*)malloc(sizeof(Shortcuts));
	IniFile *shortcutFile;

    sprintf(fileName, "%s/%s.shortcuts", profileDir, profileName);

    shortcutFile = iniFileOpen(fileName);

    LOAD_SHORTCUT(shortcutFile, msxAudioSwitch);
    LOAD_SHORTCUT(shortcutFile, spritesEnable);
    LOAD_SHORTCUT(shortcutFile, fdcTiming);
    LOAD_SHORTCUT(shortcutFile, noSpriteLimits);
    LOAD_SHORTCUT(shortcutFile, msxKeyboardQuirk);
    LOAD_SHORTCUT(shortcutFile, frontSwitch);
    LOAD_SHORTCUT(shortcutFile, pauseSwitch);
    LOAD_SHORTCUT(shortcutFile, quit);
    LOAD_SHORTCUT(shortcutFile, wavCapture);
    LOAD_SHORTCUT(shortcutFile, videoCapLoad);
    LOAD_SHORTCUT(shortcutFile, videoCapPlay);
    LOAD_SHORTCUT(shortcutFile, videoCapRec);
    LOAD_SHORTCUT(shortcutFile, videoCapStop);
    LOAD_SHORTCUT(shortcutFile, videoCapSave);
    LOAD_SHORTCUT(shortcutFile, screenCapture);
    LOAD_SHORTCUT(shortcutFile, screenCaptureUnfilteredSmall);
    LOAD_SHORTCUT(shortcutFile, screenCaptureUnfilteredLarge);
    LOAD_SHORTCUT(shortcutFile, cpuStateLoad);
    LOAD_SHORTCUT(shortcutFile, cpuStateSave);
    LOAD_SHORTCUT(shortcutFile, cpuStateQuickLoad);
    LOAD_SHORTCUT(shortcutFile, cpuStateQuickSave);
    LOAD_SHORTCUT(shortcutFile, cpuStateQuickSaveUndo);
    
    LOAD_SHORTCUT(shortcutFile, cartInsert[0]);
    LOAD_SHORTCUT(shortcutFile, cartInsert[1]);
    LOAD_SHORTCUT(shortcutFile, cartSpecialMenu[0]);
    LOAD_SHORTCUT(shortcutFile, cartSpecialMenu[1]);
    LOAD_SHORTCUT(shortcutFile, cartRemove[0]);
    LOAD_SHORTCUT(shortcutFile, cartRemove[1]);
    LOAD_SHORTCUT(shortcutFile, cartAutoReset[0]);
    
    LOAD_SHORTCUT(shortcutFile, diskInsert[0]);
    LOAD_SHORTCUT(shortcutFile, diskInsert[1]);
    LOAD_SHORTCUT(shortcutFile, diskDirInsert[0]);
    LOAD_SHORTCUT(shortcutFile, diskDirInsert[1]);
    LOAD_SHORTCUT(shortcutFile, diskRemove[0]);
    LOAD_SHORTCUT(shortcutFile, diskRemove[1]);
    LOAD_SHORTCUT(shortcutFile, diskChange[0]);
    LOAD_SHORTCUT(shortcutFile, diskAutoReset[0]);

    LOAD_SHORTCUT(shortcutFile, casInsert);
    LOAD_SHORTCUT(shortcutFile, casRewind);
    LOAD_SHORTCUT(shortcutFile, casSetPos);
    LOAD_SHORTCUT(shortcutFile, casRemove);
    
    LOAD_SHORTCUT(shortcutFile, prnFormFeed);
    LOAD_SHORTCUT(shortcutFile, mouseLockToggle);
    LOAD_SHORTCUT(shortcutFile, emulationRunPause);
    LOAD_SHORTCUT(shortcutFile, emulationStop);
    LOAD_SHORTCUT(shortcutFile, emuSpeedFull);
    LOAD_SHORTCUT(shortcutFile, emuPlayReverse);
    LOAD_SHORTCUT(shortcutFile, emuSpeedToggle);
    LOAD_SHORTCUT(shortcutFile, emuSpeedNormal);
    LOAD_SHORTCUT(shortcutFile, emuSpeedInc);
    LOAD_SHORTCUT(shortcutFile, emuSpeedDec);
    LOAD_SHORTCUT(shortcutFile, windowSizeSmall);
    LOAD_SHORTCUT(shortcutFile, windowSizeNormal);
    LOAD_SHORTCUT(shortcutFile, windowSizeFullscreen);
    LOAD_SHORTCUT(shortcutFile, windowSizeMinimized);
    LOAD_SHORTCUT(shortcutFile, windowSizeFullscreenToggle);
    LOAD_SHORTCUT(shortcutFile, resetSoft);
    LOAD_SHORTCUT(shortcutFile, resetHard);
    LOAD_SHORTCUT(shortcutFile, resetClean);
    LOAD_SHORTCUT(shortcutFile, volumeIncrease);
    LOAD_SHORTCUT(shortcutFile, volumeDecrease);
    LOAD_SHORTCUT(shortcutFile, volumeMute);
    LOAD_SHORTCUT(shortcutFile, volumeStereo);
    LOAD_SHORTCUT(shortcutFile, themeSwitch);
    LOAD_SHORTCUT(shortcutFile, casToggleReadonly);
    LOAD_SHORTCUT(shortcutFile, casAutoRewind);
    LOAD_SHORTCUT(shortcutFile, casSave);
    LOAD_SHORTCUT(shortcutFile, propShowEmulation);
    LOAD_SHORTCUT(shortcutFile, propShowVideo);
    LOAD_SHORTCUT(shortcutFile, propShowAudio);
    LOAD_SHORTCUT(shortcutFile, propShowEffects);
    LOAD_SHORTCUT(shortcutFile, propShowSettings);
    LOAD_SHORTCUT(shortcutFile, propShowApearance);
    LOAD_SHORTCUT(shortcutFile, propShowPorts);
    LOAD_SHORTCUT(shortcutFile, optionsShowLanguage);
    LOAD_SHORTCUT(shortcutFile, toolsShowMachineEditor);
    LOAD_SHORTCUT(shortcutFile, toolsShowShorcutEditor);
    LOAD_SHORTCUT(shortcutFile, toolsShowKeyboardEditor);
    LOAD_SHORTCUT(shortcutFile, toolsShowMixer);
    LOAD_SHORTCUT(shortcutFile, toolsShowDebugger);
    LOAD_SHORTCUT(shortcutFile, toolsShowTrainer);
    LOAD_SHORTCUT(shortcutFile, helpShowHelp);
    LOAD_SHORTCUT(shortcutFile, helpShowAbout);

    iniFileClose(shortcutFile);

    return shortcuts;
}

static void saveShortcuts(char* profileName, Shortcuts* shortcuts)
{
    char fileName[MAX_PATH];
	IniFile *shortcutFile;

    sprintf(fileName, "%s/%s.shortcuts", profileDir, profileName);

    shortcutFile = iniFileOpen(fileName);

    SAVE_SHORTCUT(shortcutFile, msxAudioSwitch);
    SAVE_SHORTCUT(shortcutFile, spritesEnable);
    SAVE_SHORTCUT(shortcutFile, fdcTiming);
    SAVE_SHORTCUT(shortcutFile, noSpriteLimits);
    SAVE_SHORTCUT(shortcutFile, msxKeyboardQuirk);
    SAVE_SHORTCUT(shortcutFile, frontSwitch);
    SAVE_SHORTCUT(shortcutFile, pauseSwitch);
    SAVE_SHORTCUT(shortcutFile, quit);
    SAVE_SHORTCUT(shortcutFile, wavCapture);
    SAVE_SHORTCUT(shortcutFile, videoCapLoad);
    SAVE_SHORTCUT(shortcutFile, videoCapPlay);
    SAVE_SHORTCUT(shortcutFile, videoCapRec);
    SAVE_SHORTCUT(shortcutFile, videoCapStop);
    SAVE_SHORTCUT(shortcutFile, videoCapSave);
    SAVE_SHORTCUT(shortcutFile, screenCapture);
    SAVE_SHORTCUT(shortcutFile, screenCaptureUnfilteredSmall);
    SAVE_SHORTCUT(shortcutFile, screenCaptureUnfilteredLarge);
    SAVE_SHORTCUT(shortcutFile, cpuStateLoad);
    SAVE_SHORTCUT(shortcutFile, cpuStateSave);
    SAVE_SHORTCUT(shortcutFile, cpuStateQuickLoad);
    SAVE_SHORTCUT(shortcutFile, cpuStateQuickSave);
    SAVE_SHORTCUT(shortcutFile, cpuStateQuickSaveUndo);

    SAVE_SHORTCUT(shortcutFile, cartInsert[0]);
    SAVE_SHORTCUT(shortcutFile, cartInsert[1]);
    SAVE_SHORTCUT(shortcutFile, cartSpecialMenu[0]);
    SAVE_SHORTCUT(shortcutFile, cartSpecialMenu[1]);
    SAVE_SHORTCUT(shortcutFile, cartRemove[0]);
    SAVE_SHORTCUT(shortcutFile, cartRemove[1]);
    SAVE_SHORTCUT(shortcutFile, cartAutoReset[0]);
    
    SAVE_SHORTCUT(shortcutFile, diskInsert[0]);
    SAVE_SHORTCUT(shortcutFile, diskInsert[1]);
    SAVE_SHORTCUT(shortcutFile, diskDirInsert[0]);
    SAVE_SHORTCUT(shortcutFile, diskDirInsert[1]);
    SAVE_SHORTCUT(shortcutFile, diskRemove[0]);
    SAVE_SHORTCUT(shortcutFile, diskRemove[1]); 
    SAVE_SHORTCUT(shortcutFile, diskChange[0]);   
    SAVE_SHORTCUT(shortcutFile, diskAutoReset[0]);

    SAVE_SHORTCUT(shortcutFile, casInsert);
    SAVE_SHORTCUT(shortcutFile, casRewind);
    SAVE_SHORTCUT(shortcutFile, casSetPos);
    SAVE_SHORTCUT(shortcutFile, casToggleReadonly);
    SAVE_SHORTCUT(shortcutFile, casAutoRewind);
    SAVE_SHORTCUT(shortcutFile, casSave);
    SAVE_SHORTCUT(shortcutFile, casRemove);

    SAVE_SHORTCUT(shortcutFile, prnFormFeed);
    SAVE_SHORTCUT(shortcutFile, mouseLockToggle);
    SAVE_SHORTCUT(shortcutFile, emulationRunPause);
    SAVE_SHORTCUT(shortcutFile, emulationStop);
    SAVE_SHORTCUT(shortcutFile, emuSpeedFull);
    SAVE_SHORTCUT(shortcutFile, emuPlayReverse);
    SAVE_SHORTCUT(shortcutFile, emuSpeedToggle);
    SAVE_SHORTCUT(shortcutFile, emuSpeedNormal);
    SAVE_SHORTCUT(shortcutFile, emuSpeedInc);
    SAVE_SHORTCUT(shortcutFile, emuSpeedDec);
    SAVE_SHORTCUT(shortcutFile, windowSizeSmall);
    SAVE_SHORTCUT(shortcutFile, windowSizeNormal);
    SAVE_SHORTCUT(shortcutFile, windowSizeFullscreen);
    SAVE_SHORTCUT(shortcutFile, windowSizeMinimized);
    SAVE_SHORTCUT(shortcutFile, windowSizeFullscreenToggle);
    SAVE_SHORTCUT(shortcutFile, resetSoft);
    SAVE_SHORTCUT(shortcutFile, resetHard);
    SAVE_SHORTCUT(shortcutFile, resetClean);
    SAVE_SHORTCUT(shortcutFile, volumeIncrease);
    SAVE_SHORTCUT(shortcutFile, volumeDecrease);
    SAVE_SHORTCUT(shortcutFile, volumeMute);
    SAVE_SHORTCUT(shortcutFile, volumeStereo);
    SAVE_SHORTCUT(shortcutFile, themeSwitch);
    SAVE_SHORTCUT(shortcutFile, propShowEmulation);
    SAVE_SHORTCUT(shortcutFile, propShowVideo);
    SAVE_SHORTCUT(shortcutFile, propShowAudio);
    SAVE_SHORTCUT(shortcutFile, propShowEffects);
    SAVE_SHORTCUT(shortcutFile, propShowSettings);
    SAVE_SHORTCUT(shortcutFile, propShowApearance);
    SAVE_SHORTCUT(shortcutFile, propShowPorts);
    SAVE_SHORTCUT(shortcutFile, optionsShowLanguage);
    SAVE_SHORTCUT(shortcutFile, toolsShowMachineEditor);
    SAVE_SHORTCUT(shortcutFile, toolsShowShorcutEditor);
    SAVE_SHORTCUT(shortcutFile, toolsShowKeyboardEditor);
    SAVE_SHORTCUT(shortcutFile, toolsShowMixer);
    SAVE_SHORTCUT(shortcutFile, toolsShowDebugger);
    SAVE_SHORTCUT(shortcutFile, toolsShowTrainer);
    SAVE_SHORTCUT(shortcutFile, helpShowHelp);
    SAVE_SHORTCUT(shortcutFile, helpShowAbout);

    iniFileClose(shortcutFile);
}

static void addShortcutEntry(HWND hwnd, int entry, char* description, ShotcutHotkey hotkey) {
    char buffer[512] = {0};
    LV_ITEM lvi = {0};
    
    lvi.mask       = LVIF_TEXT;
    lvi.iItem      = entry;
    lvi.pszText    = buffer;
	lvi.cchTextMax = 512;
    
    strcpy(buffer, description);

    ListView_InsertItem(hwnd, &lvi);

    lvi.iSubItem++;
    strcpy(buffer, shortcutsToString(hotkey));
    ListView_SetItem(hwnd, &lvi);
}

#define ADD_SHORTCUT(hotkey, destcription)                                      \
do {                                                                            \
    hotkeyList[entry] = &shortcuts->hotkey;                                     \
    addShortcutEntry(hwnd, entry++, destcription, shortcuts->hotkey);           \
} while(0)

#define ADD_SHORTCUTSEPARATOR()                                                 \
do {                                                                            \
    ShotcutHotkey hotkey = {0,0,0};                                             \
    hotkeyList[entry] = NULL;                                                   \
    addShortcutEntry(hwnd, entry++, "", hotkey);                                \
} while(0)

static void updateShortcutEntries(HWND hDlg) 
{
    HWND hwnd = GetDlgItem(hDlg, IDC_SCUTLIST);
    int entry = 0;

    ListView_DeleteAllItems(hwnd);

    ADD_SHORTCUT(cartInsert[0], langShortcutCartInsert1());
    ADD_SHORTCUT(cartRemove[0], langShortcutCartRemove1());
    ADD_SHORTCUT(cartSpecialMenu[0], langShortcutCartSpecialMenu1());
    ADD_SHORTCUT(cartInsert[1], langShortcutCartInsert2());
    ADD_SHORTCUT(cartRemove[1], langShortcutCartRemove2());
    ADD_SHORTCUT(cartSpecialMenu[1], langShortcutCartSpecialMenu2());
    ADD_SHORTCUT(cartAutoReset[0], langShortcutCartAutoReset());

    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(diskInsert[0], langShortcutDiskInsertA());
    ADD_SHORTCUT(diskDirInsert[0], langShortcutDiskDirInsertA());
    ADD_SHORTCUT(diskRemove[0], langShortcutDiskRemoveA());
    ADD_SHORTCUT(diskChange[0], langShortcutDiskChangeA());
    ADD_SHORTCUT(diskAutoReset[0], langShortcutDiskAutoResetA());
    ADD_SHORTCUT(diskInsert[1], langShortcutDiskInsertB());
    ADD_SHORTCUT(diskDirInsert[1], langShortcutDiskDirInsertB());
    ADD_SHORTCUT(diskRemove[1], langShortcutDiskRemoveB());

    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(casInsert, langShortcutCasInsert());
    ADD_SHORTCUT(casRemove, langShortcutCasEject());
    ADD_SHORTCUT(casAutoRewind, langShortcutCasAutorewind());
    ADD_SHORTCUT(casToggleReadonly, langShortcutCasReadOnly());
    ADD_SHORTCUT(casSetPos, langShortcutCasSetPosition());
    ADD_SHORTCUT(casRewind, langShortcutCasRewind());
    ADD_SHORTCUT(casSave, langShortcutCasSave());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(prnFormFeed, langShortcutPrnFormFeed());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(cpuStateLoad, langShortcutCpuStateLoad());
    ADD_SHORTCUT(cpuStateSave, langShortcutCpuStateSave());
    ADD_SHORTCUT(cpuStateQuickLoad, langShortcutCpuStateQload());
    ADD_SHORTCUT(cpuStateQuickSave, langShortcutCpuStateQsave());
    ADD_SHORTCUT(cpuStateQuickSaveUndo, "Quick save CPU state UNDO");
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(wavCapture, langShortcutAudioCapture());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(videoCapLoad, langShortcutVideoLoad());
    ADD_SHORTCUT(videoCapPlay, langShortcutVideoPlay());
    ADD_SHORTCUT(videoCapRec,  langShortcutVideoRecord());
    ADD_SHORTCUT(videoCapStop, langShortcutVideoStop());
    ADD_SHORTCUT(videoCapSave, langShortcutVideoRender());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(screenCapture, langShortcutScreenshotOrig());
    ADD_SHORTCUT(screenCaptureUnfilteredSmall, langShortcutScreenshotSmall());
    ADD_SHORTCUT(screenCaptureUnfilteredLarge, langShortcutScreenshotLarge());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(quit, langShortcutQuit());
    ADD_SHORTCUT(emulationRunPause, langShortcutRunPause());
    ADD_SHORTCUT(emulationStop, langShortcutStop());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(resetHard, langShortcutResetHard());
    ADD_SHORTCUT(resetSoft, langShortcutResetSoft());
    ADD_SHORTCUT(resetClean, langShortcutResetClean());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(windowSizeSmall, langShortcutSizeSmall());
    ADD_SHORTCUT(windowSizeNormal, langShortcutSizeNormal());
    ADD_SHORTCUT(windowSizeFullscreen, langShortcutSizeFullscreen());
    ADD_SHORTCUT(windowSizeMinimized, langShortcutSizeMinimized());
    ADD_SHORTCUT(windowSizeFullscreenToggle, langShortcutToggleFullscren());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(volumeIncrease, langShortcutVolumeIncrease());
    ADD_SHORTCUT(volumeDecrease, langShortcutVolumeDecrease());
    ADD_SHORTCUT(volumeMute, langShortcutVolumeMute());
    ADD_SHORTCUT(volumeStereo, langShortcutVolumeStereo());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(msxAudioSwitch, langShortcutSwitchMsxAudio());
    ADD_SHORTCUT(frontSwitch, langShortcutSwitchFront());
    ADD_SHORTCUT(pauseSwitch, langShortcutSwitchPause());
    ADD_SHORTCUT(mouseLockToggle, langShortcutToggleMouseLock());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(emuSpeedFull, langShortcutEmuSpeedMax());
    ADD_SHORTCUT(emuSpeedToggle, langShortcutEmuSpeedMaxToggle());
    ADD_SHORTCUT(emuSpeedNormal, langShortcutEmuSpeedNormal());
    ADD_SHORTCUT(emuSpeedInc, langShortcutEmuSpeedInc());
    ADD_SHORTCUT(emuSpeedDec, langShortcutEmuSpeedDec());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(emuPlayReverse, langShortcutEmuPlayReverse());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(themeSwitch, langShortcutThemeSwitch());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(propShowEmulation, langShortcutShowEmuProp());
    ADD_SHORTCUT(propShowVideo, langShortcutShowVideoProp());
    ADD_SHORTCUT(propShowAudio, langShortcutShowAudioProp());
    ADD_SHORTCUT(propShowEffects, langShortcutShowEffectsProp());
    ADD_SHORTCUT(propShowSettings, langShortcutShowFiles());
    ADD_SHORTCUT(propShowApearance, langShortcutShowSettProp());
    ADD_SHORTCUT(propShowPorts, langShortcutShowPorts());
    ADD_SHORTCUT(optionsShowLanguage, langShortcutShowLanguage());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(toolsShowMachineEditor, langShortcutShowMachines());
    ADD_SHORTCUT(toolsShowShorcutEditor, langShortcutShowShortcuts());
    ADD_SHORTCUT(toolsShowKeyboardEditor, langShortcutShowKeyboard());
    ADD_SHORTCUT(toolsShowMixer, langShortcutShowMixer());
    ADD_SHORTCUT(toolsShowDebugger, langShortcutShowDebugger());
    ADD_SHORTCUT(toolsShowTrainer, langShortcutShowTrainer());
    
    ADD_SHORTCUTSEPARATOR();

    ADD_SHORTCUT(helpShowHelp, langShortcutShowHelp());
    ADD_SHORTCUT(helpShowAbout, langShortcutShowAbout());
    
    ADD_SHORTCUTSEPARATOR();
    
    ADD_SHORTCUT(spritesEnable, langShortcutToggleSpriteEnable());
    ADD_SHORTCUT(fdcTiming,     langShortcutToggleFdcTiming());
    ADD_SHORTCUT(noSpriteLimits,     langShortcutToggleNoSpriteLimits());
    ADD_SHORTCUT(msxKeyboardQuirk,     langShortcutEnableMsxKeyboardQuirk());
}



static void updateShortcutEntry(HWND hwnd, int entry, ShotcutHotkey hotkey) {
    char buffer[512] = {0};
    LV_ITEM lvi = {0};
    
    lvi.mask       = LVIF_TEXT;
    lvi.iItem      = entry;
    lvi.pszText    = buffer;
	lvi.cchTextMax = 512;
    lvi.iSubItem++;

    strcpy(buffer, shortcutsToString(hotkey));
    ListView_SetItem(hwnd, &lvi);
}

static void updateHotkeys(HWND hDlg, int index, ShotcutHotkey newHotkey)
{
    HWND hwnd = GetDlgItem(hDlg, IDC_SCUTLIST);
    int i;

    for (i = 0; i < sizeof(hotkeyList) / sizeof(void*); i++) {
        if (hotkeyList[i] && hotkey2int(*hotkeyList[i]) == hotkey2int(newHotkey)) {
            hotkeyList[i]->type=hotkeyList[i]->mods=hotkeyList[i]->key=0;
            updateShortcutEntry(hwnd, i, *hotkeyList[i]); 
        }
    }

    if (hotkeyList[index] != NULL) {
        *hotkeyList[index] = newHotkey;
        updateShortcutEntry(hwnd, index, *hotkeyList[index]);
    }
    return;
}

static updateShortcutsList(HWND hDlg) 
{
    char** profileList = getProfileList();
    int index = 0;
    int indexMod = 0;

    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_SCUTCONFIGS, CB_DELETESTRING, 0, 0));

    if (0 == strcmp(shortcutProfile, langShortcutNewProfile())) {
        SendDlgItemMessage(hDlg, IDC_SCUTCONFIGS, CB_ADDSTRING, 0, (LPARAM)langShortcutNewProfile());
        SendDlgItemMessage(hDlg, IDC_SCUTCONFIGS, CB_SETCURSEL, index, 0);
        indexMod = 1;
    }
    
    while (profileList[index]) {
        SendDlgItemMessage(hDlg, IDC_SCUTCONFIGS, CB_ADDSTRING, 0, (LPARAM)profileList[index]);
        
        if (index + indexMod == 0 || 0 == strcmp(profileList[index], shortcutProfile)) {
            SendDlgItemMessage(hDlg, IDC_SCUTCONFIGS, CB_SETCURSEL, index + indexMod, 0);
        }
        index++;
    }
}

static BOOL CALLBACK shortcutsProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static int currIndex;
    static HWND hwnd;

    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(hDlg, langShortcutConfigTitle());
        SetWindowText(GetDlgItem(hDlg, IDC_OK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDC_SAVE), langDlgSave());
        SetWindowText(GetDlgItem(hDlg, IDC_SAVEAS), langDlgSaveAs());
        SetWindowText(GetDlgItem(hDlg, IDC_SCUTASSIGN), langShortcutAssign());
        SetWindowText(GetDlgItem(hDlg, IDC_SCUTHOTKEYTEXT), langShortcutPressText());
        SetWindowText(GetDlgItem(hDlg, IDC_SCUTCONFIGTEXT), langShortcutScheme());
        {
            LV_COLUMN lvc = {0};
            char buffer[32];
            
//            inputReset(hDlg);
            baseHwnd = hDlg;
            baseHotkeyCtrlProc = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, IDC_SCUTHOTKEY), GWL_WNDPROC, (LONG)hotkeyCtrlProc);
            SendDlgItemMessage(hDlg, IDC_SCUTHOTKEY, WM_INITIALIZE, 0, 0);

            currIndex = -1;
            hwnd = GetDlgItem(hDlg, IDC_SCUTLIST);

            ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);
            
            lvc.mask       = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvc.fmt        = LVCFMT_LEFT;
            lvc.cx         = 100;
            lvc.pszText    = buffer;
	        lvc.cchTextMax = 32;
            
            sprintf(buffer, langShortcutKey());
            lvc.cx = 244;
            ListView_InsertColumn(hwnd, 0, &lvc);
            sprintf(buffer, langShortcutDescription());
            lvc.cx = 120;
            ListView_InsertColumn(hwnd, 1, &lvc);

            updateShortcutsList(hDlg);
            updateShortcutEntries(hDlg);
            EnableWindow(GetDlgItem(hDlg, IDC_SCUTHOTKEY), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_SCUTASSIGN), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_SAVE), strcmp(shortcutProfile, langShortcutNewProfile()) &&
                                    memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts)));
        }
        return FALSE;

    case WM_ACTIVATE:
        keyboardSetFocus(4, LOWORD(wParam) != WA_INACTIVE);
        if (LOWORD(wParam) != WA_INACTIVE) {
            inputReset(hwnd);
        }
        break;

    case WM_DESTROY:
        keyboardSetFocus(4, 0);
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_SCUTCONFIGS:
            {
                static volatile int isCheckingConfigs = 0;

                if (isCheckingConfigs == 0 && HIWORD(wParam) == CBN_SELCHANGE) {
                    char profileSel[64];
                    int idx;
                    int rv;

                    isCheckingConfigs = 1;

                    idx = SendMessage(GetDlgItem(hDlg, IDC_SCUTCONFIGS), CB_GETCURSEL, 0, 0);
                    rv = SendMessage(GetDlgItem(hDlg, IDC_SCUTCONFIGS), CB_GETLBTEXT, idx, (LPARAM)profileSel);
                
                    if (rv != CB_ERR) {
                        if (strcmp(profileSel, shortcutProfile)) {
                            int modified = memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts));

                            if (modified) {
                                modified = !DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, discardProc);
                            }

                            if (modified == 0) {
                                strcpy(shortcutProfile, profileSel);
                                free(shortcuts);
                                shortcuts = loadShortcuts(shortcutProfile);
                                memcpy(shortcutsRef, shortcuts, sizeof(Shortcuts));
                                updateShortcutEntries(hDlg);
                                EnableWindow(GetDlgItem(hDlg, IDC_SCUTHOTKEY), FALSE);
                                EnableWindow(GetDlgItem(hDlg, IDC_SCUTASSIGN), FALSE);
                            }

                            updateShortcutsList(hDlg);
                        }
                    }

                    EnableWindow(GetDlgItem(hDlg, IDC_SAVE), strcmp(shortcutProfile, langShortcutNewProfile()) &&
                                            memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts)));

                    isCheckingConfigs = 0;
                }

                return TRUE;
            }

        case IDC_SAVE:
            {
                int rv;
                rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, saveProc);
                if (rv) {
                    saveShortcuts(shortcutProfile, shortcuts);
                    memcpy(shortcutsRef, shortcuts, sizeof(Shortcuts)); 
                }

                EnableWindow(GetDlgItem(hDlg, IDC_SAVE), strcmp(shortcutProfile, langShortcutNewProfile()) &&
                                        memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts)));
            }
            return TRUE;

        case IDC_SAVEAS:
            {
                int rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_SAVEAS), hDlg, saveAsProc);
                if (rv) {
                    FILE* file;
                    char fileName[MAX_PATH];

                    sprintf(fileName, "%s/%s.shortcuts", profileDir, tmpShortcutProfile);
                    file = fopen(fileName, "r");
                    if (file != NULL) {
                        rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, saveProc);
                        fclose(file);
                    }

                    if (rv) {
                        strcpy(shortcutProfile, tmpShortcutProfile);
                        memcpy(shortcutsRef, shortcuts, sizeof(Shortcuts));
                        saveShortcuts(shortcutProfile, shortcuts);
                        updateShortcutsList(hDlg);
                        updateShortcutEntries(hDlg);
                    }
                }
                EnableWindow(GetDlgItem(hDlg, IDC_SAVE), strcmp(shortcutProfile, langShortcutNewProfile()) &&
                                        memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts)));
            }
            return TRUE;

        case IDC_SCUTASSIGN:
            if (currIndex >= 0) {
                DWORD key=SendDlgItemMessage(hDlg, IDC_SCUTHOTKEY, WM_GET_HOTKEY, 0, 0);
                ShotcutHotkey hotkey = int2hotkey(&key);
                updateHotkeys(hDlg, currIndex, hotkey);
                ListView_SetItemState(hwnd, currIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
            }
            EnableWindow(GetDlgItem(hDlg, IDC_SAVE), strcmp(shortcutProfile, langShortcutNewProfile()) &&
                                    memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts)));
            return TRUE;

        case IDC_OK:
            {
                int rv = 1;
                if (memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts))) {
                    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, closeProc);
                }
                if (rv) {
                    EndDialog(hDlg, TRUE);
                }
            }
            return TRUE;
        }
        return FALSE;

    case WM_NOTIFY:
        switch (wParam) {
        case IDC_SCUTLIST:
            if ((((NMHDR FAR *)lParam)->code) == LVN_ITEMCHANGED) {
                if (ListView_GetSelectedCount(hwnd)) {
                    int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);

                    if (currIndex == -1 && index != -1) {
                        EnableWindow(GetDlgItem(hDlg, IDC_SCUTHOTKEY), TRUE);
                        EnableWindow(GetDlgItem(hDlg, IDC_SCUTASSIGN), TRUE);
                        SendDlgItemMessage(hDlg, IDC_SCUTHOTKEY, WM_SET_HOTKEY, hotkeyList[index] ? hotkey2int(*hotkeyList[index]) : 0, 0);
                    }
                    currIndex = index;
                }
                else {
                    if (currIndex != -1) {
                        SendDlgItemMessage(hDlg, IDC_SCUTHOTKEY, WM_SET_HOTKEY, 0, 0);
                        EnableWindow(GetDlgItem(hDlg, IDC_SCUTHOTKEY), FALSE);
                        EnableWindow(GetDlgItem(hDlg, IDC_SCUTASSIGN), FALSE);
                    }
                    currIndex = -1;
                }
            }
        }
        break;

    case WM_CLOSE:
        {
            int rv = 1;

            if (memcmp(shortcutsRef, shortcuts, sizeof(Shortcuts))) {
                rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, closeProc);
            }
            if (rv) {
                EndDialog(hDlg, FALSE);
            }
        }
        return TRUE;
    }

    return FALSE;
}

char* shortcutsToString(ShotcutHotkey hotkey) 
{
    static char buf[64];
        
    buf[0] = 0;

    switch (hotkey.type) {
    case HOTKEY_TYPE_KEYBOARD:
        if (hotkey.mods & KBD_LCTRL)    { strcat(buf, *buf ? "+" : ""); strcat(buf, "LCtrl"); }
        if (hotkey.mods & KBD_RCTRL)    { strcat(buf, *buf ? "+" : ""); strcat(buf, "RCtrl"); }
        if (hotkey.mods & KBD_LSHIFT)   { strcat(buf, *buf ? "+" : ""); strcat(buf, "LShift"); }
        if (hotkey.mods & KBD_RSHIFT)   { strcat(buf, *buf ? "+" : ""); strcat(buf, "RShift"); }
        if (hotkey.mods & KBD_LALT)     { strcat(buf, *buf ? "+" : ""); strcat(buf, "LAlt"); }
        if (hotkey.mods & KBD_RALT)     { strcat(buf, *buf ? "+" : ""); strcat(buf, "RAlt"); }
        if (hotkey.mods & KBD_LWIN)     { strcat(buf, *buf ? "+" : ""); strcat(buf, "LWin"); }
        if (hotkey.mods & KBD_RWIN)     { strcat(buf, *buf ? "+" : ""); strcat(buf, "RWin"); }
        if (virtualKeys[hotkey.key][0]) { strcat(buf, *buf ? "+" : ""); strcat(buf, virtualKeys[hotkey.key]); }
        break;
    case HOTKEY_TYPE_JOYSTICK:
        sprintf(buf, "%s %d", "JoyBt", hotkey.key);
        break;
    }

    return buf;
}


int shortcutsShowDialog(HWND hwnd, Properties* pProperties) {
    BOOL rv;
    FILE* file;
    char fileName[MAX_PATH];

    strcpy(shortcutProfile, pProperties->emulation.shortcutProfile);

    sprintf(fileName, "%s/%s.shortcuts", profileDir, shortcutProfile);
    file = fopen(fileName, "r");
    if (file == NULL) {
        strcpy(shortcutProfile, langShortcutNewProfile());
    }
    else {   
        fclose(file);
    }

    shortcuts = loadShortcuts(shortcutProfile);
    shortcutsRef = calloc(1, sizeof(Shortcuts));
    memcpy(shortcutsRef, shortcuts, sizeof(Shortcuts));

//    inputDestroy();
    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SHORTCUTSCONFIG), hwnd, shortcutsProc);
    if (rv) {
        strcpy(pProperties->emulation.shortcutProfile, shortcutProfile);
    }

//    inputDestroy();

    free(shortcuts);
    free(shortcutsRef);

    SetFocus(hwnd);
//    inputReset(hwnd);
    keyboardUpdate();

    return rv;
}

int shortcutsIsProfileValid(char* profileName) 
{
    FILE* file;
    char fileName[MAX_PATH];

    sprintf(fileName, "%s/%s.shortcuts", profileDir, profileName);
    file = fopen(fileName, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }

    return 0;
}

int shortcutsGetAnyProfile(char* profileName)
{
    profileName[0] = 0;

    if (shortcutsIsProfileValid("blueMSX")) {
        strcat(profileName, "blueMSX");
        return 1;
    }
    else {
        char** profileList = getProfileList();

        if (profileList[0] != NULL) {
            strcat(profileName, profileList[0]);
            return 1;
        }
    }

    return 0;
}

Shortcuts* shortcutsCreateProfile(char* profileName)
{
    return loadShortcuts(profileName);
}

void shortcutsDestroyProfile(Shortcuts* shortcuts)
{
    free(shortcuts);
}

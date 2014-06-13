/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32machineConfig.c,v $
**
** $Revision: 1.50 $
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
#include <math.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#include "Win32machineConfig.h"
#include "Win32Common.h"
#include "Win32file.h"
#include "Resource.h"
#include "Language.h"
#include "Machine.h"
#include "RomLoader.h"
#include "MediaDb.h"
#include "FileHistory.h"

#define WM_UPDATEMAHCINE  (WM_USER + 0)

static HBRUSH hBrush = NULL;
static HWND hDlgSlots  = NULL;
static HWND hDlgMemory = NULL;
static HWND hDlgChips  = NULL;
static HWND hDlgExtras = NULL;
static char tmpMachineName[64];
static char machineName[64];
static int  machineModified;
static Machine* machine;
static Machine* machineRef;
static int foundMachine;
static HWND hDlgMain;

static SlotInfo editSlotInfo;
static int      editRamNormalSize;
static int      editRamMapperSize;
static int      editRamMirroredSize;
static int      editExtRamSize;
static int      editMegaRamSize;

static void setCartSlotDropdown(HWND hDlg, int cart, int dropdownId);

static void replaceCharInString(char* str, char oldChar, char newChar) 
{
    while (*str) {
        if (*str == oldChar) {
            *str = newChar;
        }
        str++;
    }
}

static void updateMachine() {
    machineUpdate(machine);

    SendMessage(hDlgSlots,  WM_UPDATEMAHCINE, 0, 0);
    SendMessage(hDlgMemory, WM_UPDATEMAHCINE, 0, 0);
    SendMessage(hDlgChips,  WM_UPDATEMAHCINE, 0, 0);
    SendMessage(hDlgExtras, WM_UPDATEMAHCINE, 0, 0);

    machineModified = 0 != memcmp(machine, machineRef, sizeof(Machine));

    EnableWindow(GetDlgItem(hDlgMain, IDC_CONFRUN),    machineModified | foundMachine);
    EnableWindow(GetDlgItem(hDlgMain, IDC_CONFSAVE),   machineModified);
    EnableWindow(GetDlgItem(hDlgMain, IDC_CONFSAVEAS), machineModified | foundMachine);
}

static updateMachineList(HWND hDlg) {
    char** machineNames = machineGetAvailable(0);
    int index = 0;

    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_CONF_CONFIGS, CB_DELETESTRING, 0, 0));

    while (*machineNames != NULL) {
        char buffer[128];

        sprintf(buffer, "%s", *machineNames);

        SendDlgItemMessage(hDlg, IDC_CONF_CONFIGS, CB_ADDSTRING, 0, (LPARAM)buffer);

        if (index == 0 || 0 == strcmp(*machineNames, machineName)) {
            SendDlgItemMessage(hDlg, IDC_CONF_CONFIGS, CB_SETCURSEL, index, 0);
            foundMachine = 1;
        }
        machineNames++;
        index++;
    }
    if (!foundMachine) {
        machineName[0] = 0;
    }

    EnableWindow(GetDlgItem(hDlg, IDC_CONFRUN),    foundMachine);
    EnableWindow(GetDlgItem(hDlg, IDC_CONFSAVE),   machineModified);
    EnableWindow(GetDlgItem(hDlg, IDC_CONFSAVEAS), foundMachine);
}

static void setBtCheck(HWND hDlg, int id, int check, int enable) {
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

static int getBtCheck(HWND hDlg, int id) {
    HWND hwnd = GetDlgItem(hDlg, id);

    return BST_CHECKED == SendMessage(hwnd, BM_GETCHECK, 0, 0) ? 1 : 0;
}

static void setSubSlotsEnable(HWND hDlg, int nEnable) {

    if (nEnable) {
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY1,   !machine->slot[0].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED1, machine->slot[0].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY2,   !machine->slot[1].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED2, machine->slot[1].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY3,   !machine->slot[2].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED3, machine->slot[2].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY4,   !machine->slot[3].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED4, machine->slot[3].subslotted, 1);
        setCartSlotDropdown(hDlg, 0, IDC_CONF_SLOTCART1);
        setCartSlotDropdown(hDlg, 1, IDC_CONF_SLOTCART2);
    }
    else {
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY1,    1, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED1, 0, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY2,    1, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED2, 0, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY3,    1, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED3, 0, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY4,    1, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED4, 0, 1);
        machine->slot[0].subslotted = 0;
        machine->slot[1].subslotted = 0;
        machine->slot[2].subslotted = 0;
        machine->slot[3].subslotted = 0;
        setCartSlotDropdown(hDlg, 0, IDC_CONF_SLOTCART1);
        setCartSlotDropdown(hDlg, 1, IDC_CONF_SLOTCART2);
        switch (machine->board.type) {
        case BOARD_SVI:
            SendDlgItemMessage(hDlg, IDC_CONF_SLOTCART1, CB_SETCURSEL, 1, 0);
            SendDlgItemMessage(hDlg, IDC_CONF_SLOTCART2, CB_SETCURSEL, 0, 0);
            break;
        case BOARD_COLECO:
        case BOARD_COLECOADAM:
            SendDlgItemMessage(hDlg, IDC_CONF_SLOTCART1, CB_SETCURSEL, 0, 0);
            SendDlgItemMessage(hDlg, IDC_CONF_SLOTCART2, CB_SETCURSEL, 0, 0);
            break;
        case BOARD_SG1000:
            SendDlgItemMessage(hDlg, IDC_CONF_SLOTCART1, CB_SETCURSEL, 0, 0);
            SendDlgItemMessage(hDlg, IDC_CONF_SLOTCART2, CB_SETCURSEL, 0, 0);
            break;
        }
    }
    EnableWindow(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED1), nEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED2), nEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED3), nEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED4), nEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_CONF_SLOTCART2), nEnable);
}

static void setBoardDropdown(HWND hDlg) {
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_DELETESTRING, 0, 0));

    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"MSX");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"MSX S-3527");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"MSX S-1985");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"MSX T-9769B");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"MSX T-9769C");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"SVI");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"ColecoVision");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"ColecoAdam");
    SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_ADDSTRING, 0, (LPARAM)"SG-1000");

    switch (machine->board.type) {
    default:
    case BOARD_MSX:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 0, 0);
        setSubSlotsEnable(hDlg, 1);
        break;
    case BOARD_MSX_S3527:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 1, 0);
        setSubSlotsEnable(hDlg, 1);
        break;
    case BOARD_MSX_S1985:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 2, 0);
        setSubSlotsEnable(hDlg, 1);
        break;
    case BOARD_MSX_T9769B:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 3, 0);
        setSubSlotsEnable(hDlg, 1);
        break;
    case BOARD_MSX_T9769C:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 4, 0);
        setSubSlotsEnable(hDlg, 1);
        break;
    case BOARD_SVI:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 5, 0);
        setSubSlotsEnable(hDlg, 0);
        break;
    case BOARD_COLECO:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 6, 0);
        setSubSlotsEnable(hDlg, 0);
        break;
    case BOARD_COLECOADAM:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 7, 0);
        setSubSlotsEnable(hDlg, 0);
        break;
    case BOARD_SG1000:
        SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_SETCURSEL, 8, 0);
        setSubSlotsEnable(hDlg, 0);
        break;
    }
}

static int getBoardDropDown(HWND hDlg) {
    int oldType = machine->board.type;
    int idx = SendDlgItemMessage(hDlg, IDC_CONFBOARD, CB_GETCURSEL, 0, 0);

    switch (idx) {
    default:
    case 0:
        machine->board.type = BOARD_MSX;
        setSubSlotsEnable(hDlg, 1);
        break;
    case 1:
        machine->board.type = BOARD_MSX_S3527;
        setSubSlotsEnable(hDlg, 1);
        break;
    case 2:
        machine->board.type = BOARD_MSX_S1985;
        setSubSlotsEnable(hDlg, 1);
        break;
    case 3:
        machine->board.type = BOARD_MSX_T9769B;
        setSubSlotsEnable(hDlg, 1);
        break;
    case 4:
        machine->board.type = BOARD_MSX_T9769C;
        setSubSlotsEnable(hDlg, 1);
        break;
    case 5:
        machine->board.type = BOARD_SVI;
        setSubSlotsEnable(hDlg, 0);
        break;
    case 6:
        machine->board.type = BOARD_COLECO;
        setSubSlotsEnable(hDlg, 0);
        break;
    case 7:
        machine->board.type = BOARD_COLECOADAM;
        setSubSlotsEnable(hDlg, 0);
        break;
    case 8:
        machine->board.type = BOARD_SG1000;
        setSubSlotsEnable(hDlg, 0);
        break;
    }

    return machine->board.type != oldType;
}

static int getCartSlotDropDown(HWND hDlg, int cart, int dropdownId) {
    int slot    = machine->cart[cart].slot;
    int subslot = machine->cart[cart].subslot;

    char selection[64];
    int i;

    int idx = SendMessage(GetDlgItem(hDlg, dropdownId), CB_GETCURSEL, 0, 0);
    int rv = SendMessage(GetDlgItem(hDlg, dropdownId), CB_GETLBTEXT, idx, (LPARAM)selection);

    if (rv == CB_ERR) {
        return 0;
    }

    for (i = 0; i < 4; i++) {
        if (machine->slot[i].subslotted) {
            int j;

            for (j = 0; j < 4; j++) {
                char buffer[128];
                sprintf(buffer, "%s %d-%d", langConfSlot(), i, j);

                if (0 == strcmp(buffer, selection)) {
                    machine->cart[cart].slot = i;
                    machine->cart[cart].subslot = j;
                }
            }
        }
        else {
            char buffer[128];
            sprintf(buffer, "%s %d", langConfSlot(), i);
            if (0 == strcmp(buffer, selection)) {
                machine->cart[cart].slot = i;
                machine->cart[cart].subslot = 0;
            }
        }
    }

    return slot != machine->cart[cart].slot || subslot != machine->cart[cart].subslot;
}

static void setCartSlotDropdown(HWND hDlg, int cart, int dropdownId) {
    int index = 0;
    int i;

    while (CB_ERR != SendDlgItemMessage(hDlg, dropdownId, CB_DELETESTRING, 0, 0));

    for (i = 0; i < 4; i++) {
        if (machine->slot[i].subslotted) {
            int j;

            for (j = 0; j < 4; j++) {
                char buffer[128];
                sprintf(buffer, "%s% d-%d", langConfSlot(), i, j);
                SendDlgItemMessage(hDlg, dropdownId, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (machine->cart[cart].slot == i && machine->cart[cart].subslot == j) {
                    SendDlgItemMessage(hDlg, dropdownId, CB_SETCURSEL, index, 0);
                }
                index++;
            }
        }
        else {
            char buffer[128];
            sprintf(buffer, "%s %d", langConfSlot(), i);
            SendDlgItemMessage(hDlg, dropdownId, CB_ADDSTRING, 0, (LPARAM)buffer);
            if (machine->cart[cart].slot == i) {
                SendDlgItemMessage(hDlg, dropdownId, CB_SETCURSEL, index, 0);
            }
            index++;
        }
    }
}

static BOOL CALLBACK slotProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    char text[32];

    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlg, IDC_CONFSLOTLAYOUTGB), langConfSlotLayoutGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFBOARDGB), langConfBoardGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFEXTSLOTSGB), langConfSlotExtSlotGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFBOARDTEXT), langConfBoardText());
        sprintf(text, "%s 0", langConfSlot());
        SetWindowText(GetDlgItem(hDlg, IDC_SLOT0), text);
        sprintf(text, "%s 1", langConfSlot());
        SetWindowText(GetDlgItem(hDlg, IDC_SLOT1), text);
        sprintf(text, "%s 2", langConfSlot());
        SetWindowText(GetDlgItem(hDlg, IDC_SLOT2), text);
        sprintf(text, "%s 3", langConfSlot());
        SetWindowText(GetDlgItem(hDlg, IDC_SLOT3), text);
        sprintf(text, "%s 1:", langConfCartridge());
        SetWindowText(GetDlgItem(hDlg, IDC_CARTSLOT1), text);
        sprintf(text, "%s 2:", langConfCartridge());
        SetWindowText(GetDlgItem(hDlg, IDC_CARTSLOT2), text);
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTPRIMARY1), langConfSlotPrimary());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED1), langConfSlotExpanded());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTPRIMARY2), langConfSlotPrimary());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED2), langConfSlotExpanded());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTPRIMARY3), langConfSlotPrimary());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED3), langConfSlotExpanded());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTPRIMARY4), langConfSlotPrimary());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SLOTSUBSLOTTED4), langConfSlotExpanded());

        SendMessage(hDlg, WM_UPDATEMAHCINE, 0, 0);
        return FALSE;

    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
        SetBkColor((HDC)wParam, GetSysColor(COLOR_MENU));
        return (BOOL)hBrush;
        
    case WM_ERASEBKGND:
        return TRUE;

    case WM_COMMAND:
        {
            int change = 1;

            switch (LOWORD(wParam)) {
            case IDC_CONF_SLOTPRIMARY1:
                machine->slot[0].subslotted = 0;
                break;
            case IDC_CONF_SLOTSUBSLOTTED1:
                machine->slot[0].subslotted = 1;
                break;
            case IDC_CONF_SLOTPRIMARY2:
                machine->slot[1].subslotted = 0;
                break;
            case IDC_CONF_SLOTSUBSLOTTED2:
                machine->slot[1].subslotted = 1;
                break;
            case IDC_CONF_SLOTPRIMARY3:
                machine->slot[2].subslotted = 0;
                break;
            case IDC_CONF_SLOTSUBSLOTTED3:
                machine->slot[2].subslotted = 1;
                break;
            case IDC_CONF_SLOTPRIMARY4:
                machine->slot[3].subslotted = 0;
                break;
            case IDC_CONF_SLOTSUBSLOTTED4:
                machine->slot[3].subslotted = 1;
                break;
            case IDC_CONF_SLOTCART1:
                change = getCartSlotDropDown(hDlg, 0, IDC_CONF_SLOTCART1);
                break;
            case IDC_CONF_SLOTCART2:
                change = getCartSlotDropDown(hDlg, 1, IDC_CONF_SLOTCART2);
                break;
            case IDC_CONFBOARD:
                change = getBoardDropDown(hDlg);
                break;
            }

            if (change) {
                updateMachine();
            }
        }
        break;

    case WM_UPDATEMAHCINE:
        setCartSlotDropdown(hDlg, 0, IDC_CONF_SLOTCART1);
        setCartSlotDropdown(hDlg, 1, IDC_CONF_SLOTCART2);
        setBoardDropdown(hDlg);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY1,   !machine->slot[0].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED1, machine->slot[0].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY2,   !machine->slot[1].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED2, machine->slot[1].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY3,   !machine->slot[2].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED3, machine->slot[2].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTPRIMARY4,   !machine->slot[3].subslotted, 1);
        setBtCheck(hDlg, IDC_CONF_SLOTSUBSLOTTED4, machine->slot[3].subslotted, 1);
        return TRUE;
    }

    return FALSE;
}

static void getSizeControl(HWND hDlg)
{
    char selection[64];
    int idx = SendMessage(GetDlgItem(hDlg, IDC_ROMSIZE), CB_GETCURSEL, 0, 0);
    int rv = SendMessage(GetDlgItem(hDlg, IDC_ROMSIZE), CB_GETLBTEXT, idx, (LPARAM)selection);
    int value;

    if (rv == CB_ERR) {
        return;
    }

    rv = sscanf(selection, "%d", &value);
    if (rv != 1) {
        return;
    }

    if (editSlotInfo.romType == RAM_NORMAL) {
        if (value == 8 || value == 16 || value == 32 || value == 64) {
            editRamNormalSize = 1024 * value;
            editSlotInfo.startPage = 8 - editRamNormalSize / 0x2000;
        }
    }

    if (editSlotInfo.romType == RAM_1KB_MIRRORED) {
        if (value ==  8 || value == 16 || value == 24 || value == 32 || 
            value == 40 || value == 48 || value == 56 || value == 64) {
            editRamMirroredSize = 1024 * value;
            editSlotInfo.startPage = 8 - editRamMirroredSize / 0x2000;
        }
    }

    if (editSlotInfo.romType == RAM_MAPPER) {
        if (value == 1 || value == 2 || value == 4) {
            editRamMapperSize = 1024 * 1024 * value;
        }
        if (value == 64 || value == 128 || value == 256 || value == 512) {
            editRamMapperSize = 1024 * value;
        }
    }

    if (editSlotInfo.romType == ROM_EXTRAM) {
        if (value == 1 || value == 2 || value == 4) {
            editExtRamSize = 1024 * 1024 * value;
        }
        if (value == 512) {
            editExtRamSize = 1024 * value;
        }
    }

    if (editSlotInfo.romType == ROM_MEGARAM) {
        if (value == 2) {
            editMegaRamSize = 1024 * 1024 * value;
        }
        if (value == 128 || value == 256 || value == 512 || value == 768) {
            editMegaRamSize = 1024 * value;
        }
    }
}

static void getSlotControl(HWND hDlg)
{
    int index = 0;
    int sel;
    int i;
    int j;

    if (editSlotInfo.romType == SRAM_MATSUCHITA || editSlotInfo.romType == ROM_GIDE ||
        editSlotInfo.romType == ROM_TURBORTIMER || editSlotInfo.romType == ROM_TURBORIO ||
        editSlotInfo.romType == SRAM_S1985 || editSlotInfo.romType == ROM_S1990 ||
        editSlotInfo.romType == ROM_F4INVERTED || editSlotInfo.romType == ROM_F4DEVICE ||
        editSlotInfo.romType == ROM_NMS8280DIGI || 
        editSlotInfo.romType == ROM_MOONSOUND || editSlotInfo.romType == ROM_MSXMIDI ||
        editSlotInfo.romType == ROM_KANJI || editSlotInfo.romType == ROM_KANJI12 ||
        editSlotInfo.romType == ROM_JISYO || editSlotInfo.romType == ROM_MSXAUDIODEV ||
        editSlotInfo.romType == ROM_TURBORPCM || editSlotInfo.romType == ROM_SVI328FDC ||
        editSlotInfo.romType == ROM_SVI80COL || editSlotInfo.romType == ROM_SVI328PRN ||
        editSlotInfo.romType == ROM_SVI727 ||
        editSlotInfo.romType == ROM_MSXPRN || editSlotInfo.romType == ROM_SVI328RS232) {
        return;
    }

    sel = SendDlgItemMessage(hDlg, IDC_ROMSLOT, CB_GETCURSEL, 0, 0);
    if (sel == CB_ERR) {
        return;
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < (machine->slot[i].subslotted ? 4 : 1); j++) {
            if (index == sel) {
                editSlotInfo.slot = i;
                editSlotInfo.subslot = j;
            }
            index++;
        }
    }
}

static void getAddressControl(HWND hDlg)
{
    if (editSlotInfo.romType == RAM_NORMAL       || 
        editSlotInfo.romType == RAM_1KB_MIRRORED || 
        editSlotInfo.romType == ROM_NORMAL       || 
        editSlotInfo.romType == ROM_MSXMUSIC     ||
        editSlotInfo.romType == ROM_DISKPATCH    || 
        editSlotInfo.romType == ROM_CASPATCH     ||
        editSlotInfo.romType == ROM_MICROSOL     ||
        editSlotInfo.romType == ROM_NATIONALFDC  ||
        editSlotInfo.romType == ROM_PHILIPSFDC   ||
        editSlotInfo.romType == ROM_SVI738FDC    ||
        editSlotInfo.romType == ROM_BEERIDE      ||
        editSlotInfo.romType == ROM_FMPAC        ||
        editSlotInfo.romType == ROM_MICROSOL80   ||
        editSlotInfo.romType == ROM_BUNSETU) 
    {
        char selection[64];
        int idx = SendMessage(GetDlgItem(hDlg, IDC_ROMADDR), CB_GETCURSEL, 0, 0);
        int rv = SendMessage(GetDlgItem(hDlg, IDC_ROMADDR), CB_GETLBTEXT, idx, (LPARAM)selection);
        int value = 1;

        if (rv == CB_ERR) {
            return;
        }

        rv = sscanf(selection, "%X", &value);
        if (rv != 1 || value & 0x1fff) {
            return;
        }
        editSlotInfo.startPage = value / 0x2000;
    }
}

static void endEditControls(HWND hDlg)
{
    switch (editSlotInfo.romType) {
    case RAM_1KB_MIRRORED:
        strcpy(editSlotInfo.name, "");
        editSlotInfo.pageCount = editRamMirroredSize / 0x2000;
        break;
        
    case RAM_NORMAL:
        strcpy(editSlotInfo.name, "");
        editSlotInfo.pageCount = editRamNormalSize / 0x2000;
        break;
        
    case RAM_MAPPER:
        strcpy(editSlotInfo.name, "");
        editSlotInfo.pageCount = editRamMapperSize / 0x2000;
        editSlotInfo.startPage = 0;
        break;

    case ROM_SNATCHER:
    case ROM_SDSNATCHER:
    case ROM_SCCMIRRORED:
    case ROM_SCCEXTENDED:
    case ROM_SONYHBI55:
        strcpy(editSlotInfo.name, "");
        break;

    case ROM_EXTRAM:
        strcpy(editSlotInfo.name, "");
        editSlotInfo.pageCount = editExtRamSize / 0x2000;
        editSlotInfo.startPage = 0;
        break;

    case ROM_MEGARAM:
        strcpy(editSlotInfo.name, "");
        editSlotInfo.pageCount = editMegaRamSize / 0x2000;
        editSlotInfo.startPage = 0;
        break;

    case ROM_PAC:
    case ROM_FMPAC:
    case ROM_BEERIDE:
    case ROM_MICROSOL80:
        editSlotInfo.pageCount = 2;
        break;

    case ROM_NATIONALFDC:
    case ROM_PHILIPSFDC:
    case ROM_SVI738FDC:
        editSlotInfo.pageCount = 4;
        break;

    case SRAM_S1985:
    case ROM_S1990:
    case SRAM_MATSUCHITA:
    case ROM_F4INVERTED:
    case ROM_F4DEVICE:
    case ROM_NMS8280DIGI:
    case ROM_MSXMIDI:
    case ROM_MSXAUDIODEV:
    case ROM_TURBORPCM:
    case ROM_TURBORTIMER:
    case ROM_TURBORIO:
    case ROM_SVI328FDC:
    case ROM_SVI328PRN:
    case ROM_SVI328RS232:
    case ROM_MSXPRN:
    case ROM_GIDE:
        strcpy(editSlotInfo.name, "");
        editSlotInfo.slot      = 0;
        editSlotInfo.subslot   = 0;
        editSlotInfo.startPage = 0;
        editSlotInfo.pageCount = 0;
        break;

    case ROM_JISYO:
    case ROM_KANJI:
    case ROM_KANJI12:
    case ROM_MOONSOUND:
    case ROM_SVI80COL:
    case ROM_SVI727:
        editSlotInfo.slot      = 0;
        editSlotInfo.subslot   = 0;
        editSlotInfo.startPage = 0;
        editSlotInfo.pageCount = 0;
        break;

    case ROM_MSXDOS2:
    case ROM_ASCII8:
    case ROM_ASCII8SRAM:
    case ROM_ASCII16:
    case ROM_TC8566AF:
    case ROM_ASCII16SRAM:
    case ROM_KONAMI4:
    case ROM_KONAMI5:
    case ROM_SCC:
    case ROM_SCCPLUS:
    case ROM_KOEI:
    case ROM_GAMEMASTER2:
    case ROM_RTYPE:
    case ROM_HALNOTE:
    case ROM_MAJUTSUSHI:
    case ROM_CROSSBLAIM:
    case ROM_HARRYFOX:
    case ROM_KOREAN80:
    case ROM_KOREAN90:
    case ROM_KOREAN126:
    case ROM_HOLYQURAN:
    case ROM_SONYHBIV1:
        editSlotInfo.startPage = 2;
        editSlotInfo.pageCount = 4;
        break;

    case ROM_LODERUNNER:
    case ROM_BASIC:
        editSlotInfo.startPage = 4;
        editSlotInfo.pageCount = 2;
        break;

    case ROM_0x4000:
        editSlotInfo.startPage = 2;
        editSlotInfo.pageCount = 4;
        break;

    case ROM_0xC000:
        editSlotInfo.startPage = 6;
        editSlotInfo.pageCount = 2;
        break;

    case ROM_YAMAHASFG01:
        editSlotInfo.startPage = 0;
        editSlotInfo.pageCount = 2;
        break;

    case ROM_YAMAHASFG05:
        editSlotInfo.startPage = 0;
        editSlotInfo.pageCount = 4;
        break;

    case ROM_MSXAUDIO:
    case ROM_NATIONAL:
    case ROM_PLAIN:
    case ROM_FMDAS:
    case ROM_FMPAK:
    case ROM_PANASONIC16:
    case ROM_SUNRISEIDE:
    case ROM_PANASONIC32:
    case ROM_GAMEREADER:
        editSlotInfo.startPage = 0;
        editSlotInfo.pageCount = 8;
        break;
    default:
        {
            int romSize;
            char* buf = romLoad(editSlotInfo.name, NULL, &romSize);
            if (buf != NULL) {
                free(buf);
            }
            else {
                romSize = 0x2000;
            }
            romSize /= 0x2000;
            editSlotInfo.pageCount = romSize > 8 ? 8 : romSize;
        }
    }
}

static void setEditControls(HWND hDlg)
{
    RomType romType = editSlotInfo.romType;
    char buffer[32];
    int romSize = 0;
    int romPages;
    char* buf;
    int i;

    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_ROMSLOT, CB_DELETESTRING, 0, 0));
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_ROMADDR, CB_DELETESTRING, 0, 0));
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_DELETESTRING, 0, 0));

    EnableWindow(GetDlgItem(hDlg, IDC_ROMSLOT), TRUE);
    EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), TRUE);
    EnableWindow(GetDlgItem(hDlg, IDC_ROMSIZE), TRUE);
    EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), TRUE);
    EnableWindow(GetDlgItem(hDlg, IDC_ROMBROWSE), TRUE);

    // Set rom size
    buf = romLoad(editSlotInfo.name, NULL, &romSize);
    if (buf != NULL) {
        free(buf);
    }
    romPages = romSize / 0x2000;

    if (romType != RAM_1KB_MIRRORED && romType != RAM_NORMAL && romType != RAM_MAPPER && 
        romType != ROM_MEGARAM && romType != ROM_EXTRAM &&
        romType != SRAM_MATSUCHITA && romType != SRAM_S1985 && romType != ROM_S1990 && 
        romType != ROM_F4INVERTED && romType != ROM_F4DEVICE && romType != ROM_NMS8280DIGI && 
        romType != ROM_TURBORTIMER && romType != ROM_TURBORIO && romType != ROM_GIDE &&
        romType != ROM_MSXAUDIODEV && romType != ROM_TURBORPCM && romType != ROM_SVI328FDC &&
        romType != ROM_MSXMIDI && romType != ROM_MSXPRN &&
        romType != ROM_SVI727 && romType != ROM_SVI80COL && romType != ROM_SVI328PRN && romType != ROM_SVI328RS232)
    {
        if (romSize == 0) {
            sprintf(buffer, langTextUnknown());
        }
        else if (romSize / 1024 < 1024) {
            sprintf(buffer, "%d kB", romSize / 1024);
        }
        else {
            sprintf(buffer, "%d MB", romSize / 1024 / 1024);
        }
        SetWindowText(GetDlgItem(hDlg, IDC_ROMSIZE), buffer);
        EnableWindow(GetDlgItem(hDlg, IDC_ROMSIZE), FALSE);
    }

    // Set ram slot
    if (romType == SRAM_MATSUCHITA || romType == SRAM_S1985 || 
        romType == ROM_S1990 || romType == ROM_KANJI ||  romType == ROM_GIDE ||
        romType == ROM_TURBORTIMER || romType == ROM_TURBORIO ||
        romType == ROM_F4INVERTED || romType == ROM_F4DEVICE ||
        romType == ROM_NMS8280DIGI ||
        romType == ROM_MOONSOUND || romType == ROM_MSXMIDI ||
        romType == ROM_MSXAUDIODEV || romType == ROM_TURBORPCM ||
        romType == ROM_KANJI12 || romType == ROM_JISYO ||
        romType == ROM_SVI328FDC || romType == ROM_SVI80COL || romType == ROM_SVI727 ||
        romType == ROM_SVI328PRN || romType == ROM_MSXPRN || romType == ROM_SVI328RS232)
    {
        EnableWindow(GetDlgItem(hDlg, IDC_ROMSLOT), FALSE);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMSLOT), "Unmapped");
    }
    else {
        int index = 0;
        for (i = 0; i < 4; i++) {
            if (machine->slot[i].subslotted) {
                int j;

                for (j = 0; j < 4; j++) {
                    char buffer[128];
                    sprintf(buffer, "%s %d-%d", langConfSlot(), i, j);
                    SendDlgItemMessage(hDlg, IDC_ROMSLOT, CB_ADDSTRING, 0, (LPARAM)buffer);
                    if (editSlotInfo.slot == i && editSlotInfo.subslot == j) {
                        SendDlgItemMessage(hDlg, IDC_ROMSLOT, CB_SETCURSEL, index, 0);
                    }
                    index++;
                }
            }
            else {
                char buffer[128];
                sprintf(buffer, "%s %d", langConfSlot(), i);
                SendDlgItemMessage(hDlg, IDC_ROMSLOT, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (editSlotInfo.slot == i) {
                    SendDlgItemMessage(hDlg, IDC_ROMSLOT, CB_SETCURSEL, index, 0);
                }
                index++;
            }
        }
    }

    // Set address
    if (romType == RAM_NORMAL || romType == RAM_1KB_MIRRORED ||
        romType == ROM_NORMAL || romType == ROM_DISKPATCH || romType == ROM_CASPATCH ||
        romType == ROM_MICROSOL || romType == ROM_NATIONALFDC || romType == ROM_PHILIPSFDC || 
        romType == ROM_SVI738FDC || romType == ROM_MSXMUSIC || romType == ROM_BEERIDE || 
        romType == ROM_FMPAC || romType == ROM_PAC || romType == ROM_BUNSETU || romType == ROM_MICROSOL80)
    {
        int size = romType == RAM_NORMAL ? editRamNormalSize / 0x2000 : 
                   romType == RAM_1KB_MIRRORED ? editRamMirroredSize / 0x2000 : 
                   (romType == ROM_NATIONALFDC || romType == ROM_PHILIPSFDC || romType == ROM_SVI738FDC) ? 4 : 
                   romType == ROM_FMPAC || romType == ROM_PAC ? 2 : 
                   romPages > 8 ? 8 : romPages < 1 ? 1 : romPages;
        int end = 8 - size;
        int start = editSlotInfo.startPage < end ? editSlotInfo.startPage : end;

        for (i = 0; i <= end; i++) {
            char buffer[32];
            sprintf(buffer, "%.4X - %.4X", i * 0x2000, (i + size) * 0x2000 - 1);
            SendDlgItemMessage(hDlg, IDC_ROMADDR, CB_ADDSTRING, 0, (LPARAM)buffer);
            if (i == start) {
                SendDlgItemMessage(hDlg, IDC_ROMADDR, CB_SETCURSEL, i, 0);
            }
        }
    }

    switch (romType) {
    case RAM_1KB_MIRRORED:
        {
            int index = 0;
            for (i = 8; i <= 64; i += 8) {
                sprintf(buffer, "%d kB", i);
                SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (index == 0 || i == editRamMirroredSize / 1024) {
                    SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_SETCURSEL, index, 0);
                }
                index++;
            }

            SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
            EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        }
        break;

    case RAM_NORMAL:
        {
            int index = 0;
            for (i = 8; i <= 64; i *= 2) {
                sprintf(buffer, "%d kB", i);
                SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (index == 0 || i == editRamNormalSize / 1024) {
                    SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_SETCURSEL, index, 0);
                }
                index++;
            }

            SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
            EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        }
        break;

    case RAM_MAPPER:
        {
            int index = 0;

            for (i = 64; i <= 4096; i *= 2) {
                char buffer[32];

                if (i < 1000) {
                    sprintf(buffer, "%d kB", i);
                }
                else {
                    sprintf(buffer, "%d MB", i / 1024);
                }
                SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (index == 0 || i == editRamMapperSize / 1024) {
                    SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_SETCURSEL, index, 0);
                }
                index++;
            }

            EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
            SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0xFFFF");
            SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
            EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        }            
        break;

    case ROM_EXTRAM:
        {
            int index = 0;
            for (i = 512; i <= 4096; i *= 2) {
                int size = i;
                if (size < 1000) {
                    sprintf(buffer, "%d kB", size);
                }
                else {
                    sprintf(buffer, "%d MB", size / 1024);
                }
                SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (index == 0 || size == editExtRamSize / 1024) {
                    SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_SETCURSEL, index, 0);
                }
                index++;
            }

            EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
            SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0xFFFF");
            SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
            EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        }
        break;

    case ROM_MEGARAM:
        {
            int index = 0;
            for (i = 128; i <= 2048; i *= 2) {
                int size = i != 1024 ? i : 768;
                if (size < 1000) {
                    sprintf(buffer, "%d kB", size);
                }
                else {
                    sprintf(buffer, "%d MB", size / 1024);
                }
                SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_ADDSTRING, 0, (LPARAM)buffer);
                if (index == 0 || size == editMegaRamSize / 1024) {
                    SendDlgItemMessage(hDlg, IDC_ROMSIZE, CB_SETCURSEL, index, 0);
                }
                index++;
            }

            EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
            SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0xFFFF");
            SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
            EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        }            
        break;

    case ROM_NORMAL:
    case ROM_DISKPATCH:
    case ROM_CASPATCH:
    case ROM_MICROSOL:
    case ROM_NATIONALFDC:
    case ROM_PHILIPSFDC:
    case ROM_SVI738FDC:
    case ROM_BEERIDE:
    case ROM_FMPAC:
    case ROM_BUNSETU:
    case ROM_MSXMUSIC:
    case ROM_MICROSOL80:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        break;

    case ROM_YAMAHASFG01:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0x3FFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_YAMAHASFG05:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0x7FFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_SUNRISEIDE:
    case ROM_PANASONIC16:
    case ROM_PANASONIC32:
    case ROM_NATIONAL:
    case ROM_PLAIN:
    case ROM_FMPAK:
    case ROM_MSXAUDIO:
    case ROM_FMDAS:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0xFFFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_LODERUNNER:
    case ROM_BASIC:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x8000 - 0xBFFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_0x4000:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        if (romPages > 6) romPages = 6;
        {
            char buffer[32];
            sprintf(buffer, "%.4X - %.4X", 0x4000, 0x4000 + romPages * 0x2000 - 1);
            SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), buffer);
        }
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_0xC000:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0xC000 - 0xFFFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_MSXDOS2:
    case ROM_ASCII8:
    case ROM_ASCII8SRAM:
    case ROM_ASCII16:
    case ROM_TC8566AF:
    case ROM_ASCII16SRAM:
    case ROM_KONAMI4:
    case ROM_KONAMI5:
    case ROM_SCC:
    case ROM_SCCPLUS:
    case ROM_KOEI:
    case ROM_GAMEMASTER2:
    case ROM_RTYPE:
    case ROM_HALNOTE:
    case ROM_CROSSBLAIM:
    case ROM_HARRYFOX:
    case ROM_MAJUTSUSHI:
    case ROM_KOREAN80:
    case ROM_KOREAN90:
    case ROM_KOREAN126:
    case ROM_HOLYQURAN:
    case ROM_SONYHBIV1:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x4000 - 0xBFFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case ROM_GAMEREADER:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x0000 - 0xFFFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;
        
    case ROM_PAC:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x4000 - 0x7FFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        break;

    case ROM_SNATCHER:
    case ROM_SDSNATCHER:
    case ROM_SCCMIRRORED:
    case ROM_SCCEXTENDED:
    case ROM_SONYHBI55:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "0x4000 - 0xBFFF");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        break;

    case ROM_JISYO:
    case ROM_KANJI:
    case ROM_KANJI12:
    case ROM_MOONSOUND:
    case ROM_SVI80COL:
    case ROM_SVI727:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), editSlotInfo.name);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "n/a");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        break;

    case SRAM_S1985:
    case ROM_S1990:
    case SRAM_MATSUCHITA:
    case ROM_F4INVERTED:
    case ROM_F4DEVICE:
    case ROM_NMS8280DIGI:
    case ROM_MSXMIDI:
    case ROM_TURBORTIMER:
    case ROM_TURBORIO:
    case ROM_MSXAUDIODEV:
    case ROM_TURBORPCM:
    case ROM_SVI328FDC:
    case ROM_SVI328PRN:
    case ROM_SVI328RS232:
    case ROM_MSXPRN:
    case ROM_GIDE:
        SetWindowText(GetDlgItem(hDlg, IDC_ROMIMAGE), "");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMIMAGE), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_ROMBROWSE), FALSE);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMADDR), "n/a");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMADDR), FALSE);
        SetWindowText(GetDlgItem(hDlg, IDC_ROMSIZE), "");
        EnableWindow(GetDlgItem(hDlg, IDC_ROMSIZE), FALSE);
        break;
    }
}

static RomType romTypeList[] = {
    ROM_GAMEREADER,
    RAM_NORMAL,
    RAM_1KB_MIRRORED,
    RAM_MAPPER,
    ROM_EXTRAM,
    ROM_MEGARAM,
    ROM_NORMAL,
    ROM_DISKPATCH,
    ROM_CASPATCH,
    ROM_TC8566AF,
    ROM_MICROSOL,
    ROM_NATIONALFDC,
    ROM_PHILIPSFDC,
    ROM_GIDE,
    ROM_SUNRISEIDE,
    ROM_BEERIDE,
    ROM_SVI738FDC,
    ROM_SVI328FDC,
    ROM_SVI328PRN,
    ROM_SVI328RS232,
    ROM_SVI80COL,
    ROM_SVI727,
    ROM_KANJI,
    ROM_KANJI12,
    SRAM_MATSUCHITA,
    SRAM_S1985,
    ROM_S1990,
    ROM_F4DEVICE,
    ROM_NMS8280DIGI,
    ROM_F4INVERTED,
    ROM_TURBORTIMER,
    ROM_TURBORIO,
    ROM_NATIONAL,
    ROM_BUNSETU,
    ROM_JISYO,
    ROM_PANASONIC16,
    ROM_PANASONIC32,
    ROM_PLAIN,
    ROM_BASIC,
    ROM_0x4000,
    ROM_0xC000,
    ROM_MSXDOS2,
    ROM_ASCII8,
    ROM_ASCII8SRAM,
    ROM_ASCII16,
    ROM_ASCII16SRAM,
    ROM_KONAMI4,
    ROM_KONAMI5,
    ROM_SCC,
    ROM_SCCPLUS,
    ROM_KOEI,
    ROM_GAMEMASTER2,
    ROM_RTYPE,
    ROM_CROSSBLAIM,
    ROM_HARRYFOX,
    ROM_LODERUNNER,
    ROM_HALNOTE,
    ROM_KONAMISYNTH,
    ROM_KONAMKBDMAS,
    ROM_KONWORDPRO,
    ROM_MAJUTSUSHI,
    ROM_KOREAN80,
    ROM_KOREAN90,
    ROM_KOREAN126,
    ROM_HOLYQURAN,
    ROM_FMPAC,
    ROM_FMPAK,
    ROM_PAC,
    ROM_MSXMUSIC,
    ROM_MSXAUDIO,
    ROM_MSXPRN,
    ROM_MOONSOUND,
    ROM_MSXMIDI,
    ROM_KONAMI4NF, 
    ROM_ASCII16NF,
    ROM_SNATCHER,
    ROM_SDSNATCHER,
    ROM_SCCMIRRORED,
    ROM_SCCEXTENDED,
    ROM_SONYHBI55,
    ROM_MSXAUDIODEV,
    ROM_TURBORPCM,
    ROM_MICROSOL80,
    ROM_SONYHBIV1,
    ROM_FMDAS,
    ROM_YAMAHASFG01,
    ROM_YAMAHASFG05,
    ROM_UNKNOWN,
};

static void setEditProps(HWND hDlg, char* fileName)
{
    int size;
    int idx = 0;
    char* buf = romLoad(fileName, NULL, &size);

    editSlotInfo.romType = ROM_NORMAL;

    if (buf != NULL) {
        MediaType* mediaType = mediaDbLookupRom(buf, size);
        if (mediaType != NULL && mediaDbGetRomType(mediaType) != ROM_UNKNOWN) {
            editSlotInfo.romType = mediaDbGetRomType(mediaType);
        }

        free(buf);
    }

    while (romTypeList[idx] != editSlotInfo.romType) {
        idx++;
    }

    SendMessage(GetDlgItem(hDlg, IDC_ROMTYPE), CB_SETCURSEL, idx, 0);
}

static BOOL CALLBACK slotEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static HICON hIconBtBrowse = NULL;
    int i;

    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(hDlg, langConfEditMemTitle());
        SetWindowText(GetDlgItem(hDlg, IDC_EDITMEMGB), langConfEditMemGB());
        SetWindowText(GetDlgItem(hDlg, IDC_EDITMEMTYPE), langConfEditMemType());
        SetWindowText(GetDlgItem(hDlg, IDC_EDITMEMFILE), langConfEditMemFile());
        SetWindowText(GetDlgItem(hDlg, IDC_EDITMEMADDRESS), langConfEditMemAddress());
        SetWindowText(GetDlgItem(hDlg, IDC_EDITMEMSIZE), langConfEditMemSize());
        SetWindowText(GetDlgItem(hDlg, IDC_EDITMEMSLOT), langConfEditMemSlot());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());

        if (hIconBtBrowse == NULL) {
            hIconBtBrowse = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BROWSE));
        }

        SendMessage(GetDlgItem(hDlg, IDC_ROMBROWSE),   BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconBtBrowse);

        for (i = 0; romTypeList[i] != ROM_UNKNOWN; i++) {
            SendDlgItemMessage(hDlg, IDC_ROMTYPE, CB_ADDSTRING, 0, (LPARAM)romTypeToString(romTypeList[i]));
            if (romTypeList[i] == editSlotInfo.romType) {
                SendDlgItemMessage(hDlg, IDC_ROMTYPE, CB_SETCURSEL, i, 0);
            }
        }

        editRamNormalSize   = editSlotInfo.romType == RAM_NORMAL       ? editSlotInfo.pageCount * 0x2000 : 0x10000;
        editRamMirroredSize = editSlotInfo.romType == RAM_1KB_MIRRORED ? editSlotInfo.pageCount * 0x2000 : 0x2000;
        editRamMapperSize   = editSlotInfo.romType == RAM_MAPPER       ? editSlotInfo.pageCount * 0x2000 : 0x10000;
        editMegaRamSize     = editSlotInfo.romType == ROM_MEGARAM      ? editSlotInfo.pageCount * 0x2000 : 0xc0000;
        editExtRamSize      = editSlotInfo.romType == ROM_EXTRAM       ? editSlotInfo.pageCount * 0x2000 : 0x400000;

        setEditControls(hDlg);

        return FALSE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_ROMSIZE:
            if (HIWORD(wParam) == 1) {
                getSizeControl(hDlg);
                setEditControls(hDlg);
            }
            return TRUE;

        case IDC_ROMSLOT:
            if (HIWORD(wParam) == 1) {
                getSlotControl(hDlg);
                setEditControls(hDlg);
            }
            return TRUE;

        case IDC_ROMADDR:
            if (HIWORD(wParam) == 1) {
                getAddressControl(hDlg);
                setEditControls(hDlg);
            }
            return TRUE;

        case IDC_ROMBROWSE:
            {
                static char  defDir[MAX_PATH] = { 0 };
                char  curDir[MAX_PATH];
                char* fileName;
                char extensionList[512];

                GetCurrentDirectory(MAX_PATH, curDir);
                if (strlen(defDir) == 0) {
                    strcpy(defDir, curDir);
                }
                sprintf(extensionList, "%s   (*.rom, *.ri, *.mx1, *.mx2, *.col, *.sg, *.sc, *.zip)#*.rom; *.ri; *.mx1; *.mx2; *.col; *.sg; *.sc; *.zip#%s   (*.*)#*.*#", langFileRom(), langFileAll());
                replaceCharInString(extensionList, '#', 0);

                fileName = openFile(hDlg, langConfOpenRom(), extensionList, defDir, -1, NULL, NULL);
                SetCurrentDirectory(curDir);

                if (fileName != NULL) {
                    strcat(curDir, "\\");
                    curDir[0]   = toupper(curDir[0]);
                    fileName[0] = toupper(fileName[0]);
                    if (strlen(fileName) > strlen(curDir)) {
                        if (0 == memcmp(fileName, curDir, strlen(curDir))) {
                            memmove(fileName, fileName + strlen(curDir), MAX_PATH - strlen(curDir));
                        }
                    }

                    strcpy(editSlotInfo.name, fileName);
                    setEditProps(hDlg, fileName);
                    setEditControls(hDlg);
                }

                SetFocus(GetDlgItem(hDlg, IDC_ROMIMAGE));
            }
            return TRUE;

        case IDC_ROMTYPE:
            {
                int idx = SendMessage(GetDlgItem(hDlg, IDC_ROMTYPE), CB_GETCURSEL, 0, 0);

                editSlotInfo.romType = idx == CB_ERR ? -1 : romTypeList[idx];

                setEditControls(hDlg);
            }
            return TRUE;

        case IDOK:
            endEditControls(hDlg);
            EndDialog(hDlg, TRUE);
            return TRUE;

            case IDCANCEL:
                EndDialog(hDlg, FALSE);
                return TRUE;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hDlgSlots);
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

static void addMachineEntry(HWND hwnd, int entry) {
    char buffer[512] = {0};
    LV_ITEM lvi = {0};
    UInt32 start;
    UInt32 end;
    
    lvi.mask       = LVIF_IMAGE;
    lvi.iItem      = entry;
    lvi.pszText    = buffer;
	lvi.cchTextMax = 512;

    lvi.iImage = machine->slotInfo[entry].error;

    machine->slotInfo[entry].error = 0;

    ListView_InsertItem(hwnd, &lvi);
    lvi.mask = LVIF_TEXT;

    lvi.iSubItem++;
    if (machine->slotInfo[entry].pageCount == 0) {
        sprintf(buffer, "");
    }
    else if (machine->slotInfo[entry].subslot || machine->slot[machine->slotInfo[entry].slot].subslotted) {
        sprintf(buffer, "%d - %d", machine->slotInfo[entry].slot, machine->slotInfo[entry].subslot);
    }
    else {
        sprintf(buffer, "%d", machine->slotInfo[entry].slot);
    }
    ListView_SetItem(hwnd, &lvi);

    lvi.iSubItem++;

    if (machine->slotInfo[entry].slot == 0 && 
        machine->slotInfo[entry].subslot == 0 &&
        machine->slotInfo[entry].startPage == 0 &&
        machine->slotInfo[entry].pageCount == 0) 
    {
        sprintf(buffer, "");
    }
    else {
        start = machine->slotInfo[entry].startPage * 0x2000;
        end   = start + machine->slotInfo[entry].pageCount * 0x2000 - 1;
        if (end > 0xffff) end = 0xffff;
        sprintf(buffer, "%.4X-%.4X", start, end);
    }
    ListView_SetItem(hwnd, &lvi);

    lvi.iSubItem++;
    if (machine->slotInfo[entry].romType == RAM_MAPPER ||
        machine->slotInfo[entry].romType == RAM_NORMAL ||
        machine->slotInfo[entry].romType == ROM_EXTRAM ||
        machine->slotInfo[entry].romType == ROM_MEGARAM)
    {
        int size = machine->slotInfo[entry].pageCount * 8;
        if (size < 1024) {
            sprintf(buffer, "%d kB %s", size, romTypeToString(machine->slotInfo[entry].romType));
        }
        else {
            sprintf(buffer, "%d MB %s", size / 1024, romTypeToString(machine->slotInfo[entry].romType));
        }
    }
    else {
        sprintf(buffer, romTypeToString(machine->slotInfo[entry].romType));
    }
    ListView_SetItem(hwnd, &lvi);

    lvi.iSubItem++;
    sprintf(buffer, romTypeToString(machine->slotInfo[entry].romType));
    ListView_InsertItem(hwnd, &lvi);

    sprintf(buffer, stripPath(machine->slotInfo[entry].name));
    ListView_SetItem(hwnd, &lvi);
}

int showListDetailDlg(HWND hwnd, int index) {
    int rv;

    if (index == -1) {
        memset(&editSlotInfo, 0, sizeof(editSlotInfo));
        editSlotInfo.romType = ROM_NORMAL;
    }
    else {
        memcpy(&editSlotInfo, machine->slotInfo + index, sizeof(editSlotInfo));
    }
    
    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_MEMEDIT), hwnd, slotEditProc);
    if (!rv) {
        return 0;
    }

    if (index == -1) {
        index = machine->slotInfoCount;
        machine->slotInfoCount++;
    }

    memcpy(machine->slotInfo + index, &editSlotInfo, sizeof(editSlotInfo));

    machineUpdate(machine);

    return 1;
}

static BOOL CALLBACK memoryProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static int currIndex;
    static HWND hwnd;

    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlg, IDC_CONFADD), langConfMemAdd());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFEDIT), langConfMemEdit());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFREMOVE), langConfMemRemove());

        {
            char buffer[32];
            LV_COLUMN lvc = {0};
            HIMAGELIST himlSmall;
    
            hwnd = GetDlgItem(hDlg, IDC_CONF_SLOTINFO);

            EnableWindow(GetDlgItem(hDlg, IDC_CONFEDIT), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_CONFREMOVE), FALSE);

            currIndex = -1;
            ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

            himlSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), TRUE, 1, 1); 

            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRANSPARENT))); 
            ImageList_AddIcon(himlSmall, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_EXCMARK))); 
            ListView_SetImageList(hwnd, himlSmall, LVSIL_SMALL);


            lvc.mask       = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvc.fmt        = LVCFMT_LEFT;
            lvc.cx         = 100;
            lvc.pszText    = buffer;
	        lvc.cchTextMax = 32;

            sprintf(buffer, "");
            lvc.cx = 20;
            ListView_InsertColumn(hwnd, 0, &lvc);
            sprintf(buffer, langConfMemSlot());
            lvc.cx = 40;
            ListView_InsertColumn(hwnd, 1, &lvc);
            sprintf(buffer, langConfMemAddress());
            lvc.cx = 70;
            ListView_InsertColumn(hwnd, 2, &lvc);
            sprintf(buffer, langConfMemType());
            lvc.cx = 110;
            ListView_InsertColumn(hwnd, 3, &lvc);
            sprintf(buffer, langConfMemRomImage());
            lvc.cx = 150;
            ListView_InsertColumn(hwnd, 4, &lvc);
        }

        SendMessage(hDlg, WM_UPDATEMAHCINE, 0, 0);
        return FALSE;

    case WM_ERASEBKGND:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_CONFADD:
            {
                int change = showListDetailDlg(hDlg, -1);
                if (change) {
                    updateMachine();
                }
            }
            return TRUE;
        case IDC_CONFEDIT:
            {
                if (ListView_GetSelectedCount(hwnd)) {
                    int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);
                    int change = showListDetailDlg(hDlg, index);
                    if (change) {
                        updateMachine();
                    }
                }
            }
            return TRUE;
        case IDC_CONFREMOVE:
            if (ListView_GetSelectedCount(hwnd)) {
                int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);

                if (index != -1) {
                    int i;

                    for (i = index; i < machine->slotInfoCount - 1; i++) {
                        memcpy(machine->slotInfo + i, machine->slotInfo + i + 1, sizeof(SlotInfo));
                    }
                    machine->slotInfoCount--;
                    memset(machine->slotInfo + machine->slotInfoCount, 0, sizeof(SlotInfo));
                    updateMachine();
                    if (index < machine->slotInfoCount) {
                        SetFocus(hwnd);
                        ListView_SetItemState(hwnd, index, LVIS_SELECTED, LVIS_SELECTED);
                    }
                }
            }
            return TRUE;
        }

    case WM_NOTIFY:
        switch (wParam) {
        case IDC_CONF_SLOTINFO:
            if ((((NMHDR FAR *)lParam)->code) == LVN_ITEMCHANGED) {
                if (ListView_GetSelectedCount(hwnd)) {
                    int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);

                    if (currIndex == -1 && index != -1) {
                        EnableWindow(GetDlgItem(hDlg, IDC_CONFEDIT), TRUE);
                        EnableWindow(GetDlgItem(hDlg, IDC_CONFREMOVE), TRUE);
                    }
                    currIndex = index;
                }
                else {
                    if (currIndex != -1) {
                        EnableWindow(GetDlgItem(hDlg, IDC_CONFEDIT), FALSE);
                        EnableWindow(GetDlgItem(hDlg, IDC_CONFREMOVE), FALSE);
                    }
                    currIndex = -1;
                }
            }

            if ((((NMHDR FAR *)lParam)->code) == LVN_ITEMACTIVATE) {
                if (ListView_GetSelectedCount(hwnd)) {
                    int index = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);
                    int change = showListDetailDlg(hDlg, index);
                    if (change) {
                        updateMachine();
                    }
                }
                return TRUE;
            }
        }
        break;

    case WM_UPDATEMAHCINE:
        {
            int i;

            ListView_DeleteAllItems(hwnd);

            for (i = 0; i < machine->slotInfoCount; i++) {
                addMachineEntry(hwnd, i);
            }
        }
        return TRUE;
    }

    return FALSE;
}


static UInt32 cpuFreq[] = {
    21477270 / 1,
    21477270 / 2,
    21477270 / 3,
    21477270 / 4,
    21477270 / 6,
    21477270 / 9,
    21477270 / 15,
    21477270 / 25,
    21477270 / 50,
    21477270 / 100
};

static int getCpuFreqList(HWND hDlg) {
    UInt32 selZ80  = cpuFreq[SendMessage(GetDlgItem(hDlg, IDC_CPUZ80FREQ),  CB_GETCURSEL, 0, 0)];
    UInt32 selR800 = cpuFreq[SendMessage(GetDlgItem(hDlg, IDC_CPUR800FREQ), CB_GETCURSEL, 0, 0)];
    int change = selZ80 != machine->cpu.freqZ80 || selR800 != machine->cpu.freqR800;

    machine->cpu.freqZ80  = selZ80;
    machine->cpu.freqR800 = selR800;

    return change;

}

static void updateCpuFreqList(HWND hDlg) {
    int i;
    UInt32 selZ80  = 3579545;
    UInt32 selR800 = 7159090;
    
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_CPUZ80FREQ, CB_DELETESTRING, 0, 0));
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_CPUR800FREQ, CB_DELETESTRING, 0, 0));

    for (i = 0; i < sizeof(cpuFreq) / sizeof(cpuFreq[0]); i++) {
       char buffer[128];
        sprintf(buffer, "%d.%.3d MHz", cpuFreq[i] / 1000000, (cpuFreq[i] / 1000) % 1000);
        SendDlgItemMessage(hDlg, IDC_CPUZ80FREQ, CB_ADDSTRING, 0, (LPARAM)buffer);
        if (machine->cpu.freqZ80 <= cpuFreq[i]) {
            SendDlgItemMessage(hDlg, IDC_CPUZ80FREQ, CB_SETCURSEL, i, 0);
            selZ80 = cpuFreq[i];
        }
        SendDlgItemMessage(hDlg, IDC_CPUR800FREQ, CB_ADDSTRING, 0, (LPARAM)buffer);
        if (machine->cpu.freqR800 <= cpuFreq[i]) {
            SendDlgItemMessage(hDlg, IDC_CPUR800FREQ, CB_SETCURSEL, i, 0);
            selR800 = cpuFreq[i];
        }
    }
    machine->cpu.freqZ80  = selZ80;
    machine->cpu.freqR800 = selR800;

    EnableWindow(GetDlgItem(hDlg, IDC_CPUR800FREQ), machine->cpu.hasR800);
}

static void updateFdcCountList(HWND hDlg) {
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_DELETESTRING, 0, 0));

    if (machine->fdc.enabled) {
        SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_ADDSTRING, 0, (LPARAM)"1");
        SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_ADDSTRING, 0, (LPARAM)"2");
        if (machine->fdc.count == 1) {
            SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_SETCURSEL, 0, 0);
        }
        else {
            machine->fdc.count = 2;
            SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_SETCURSEL, 1, 0);
        }
    }
    else {
        SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_ADDSTRING, 0, (LPARAM)"0");
        SendDlgItemMessage(hDlg, IDC_FDCNUMDRIVES, CB_SETCURSEL, 0, 0);
    }

    EnableWindow(GetDlgItem(hDlg, IDC_FDCNUMDRIVES), machine->fdc.enabled);
}

static int getFdcCountList(HWND hDlg) {
    int change = 0;
    if (machine->fdc.enabled) {
        int index = 1 + SendMessage(GetDlgItem(hDlg, IDC_FDCNUMDRIVES),  CB_GETCURSEL, 0, 0);
        change = index != machine->fdc.count;
        if (index == 1) {
            machine->fdc.count = 1;
        }
        else {
            machine->fdc.count = 2;
        }
    }
    return change;
}


static BOOL CALLBACK extrasProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlg, IDC_CPUFREQGROUPBOX),  langConfChipCpuFreqGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CPUZ80FREQTEXT),   langConfChipZ80FreqText());
        SetWindowText(GetDlgItem(hDlg, IDC_CPUR800FREQTEXT),  langConfChipR800FreqText());
        SetWindowText(GetDlgItem(hDlg, IDC_FDCNUMGROUPBOX),   langConfChipFdcGB());
        SetWindowText(GetDlgItem(hDlg, IDC_FDCNUMDRIVESTEXT), langConfChipFdcNumDrivesText());

        SetWindowText(GetDlgItem(hDlg, IDC_CMOSGROUPBOX), langConfCmosGB());
        SetWindowText(GetDlgItem(hDlg, IDC_CMOSENABLE), langConfCmosEnableText());
        SetWindowText(GetDlgItem(hDlg, IDC_CMOSBATTERY), langConfCmosBatteryText());
        SendMessage(hDlg, WM_UPDATEMAHCINE, 0, 0);
        return FALSE;

    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
        SetBkColor((HDC)wParam, GetSysColor(COLOR_MENU));
        return (BOOL)hBrush;

    case WM_ERASEBKGND:
        return TRUE;
        
    case WM_COMMAND:
        {
            int change = 0;

            switch (LOWORD(wParam)) {
            case IDC_CPUZ80FREQ:
            case IDC_CPUR800FREQ:
                change = getCpuFreqList(hDlg);
                break;
            case IDC_FDCNUMDRIVES:
                change = getFdcCountList(hDlg);
                break;
            case IDC_CMOSENABLE:
                machine->cmos.enable = getBtCheck(hDlg, IDC_CMOSENABLE);
                EnableWindow(GetDlgItem(hDlg, IDC_CMOSBATTERY), machine->cmos.enable);
                break;
            case IDC_CMOSBATTERY:
                machine->cmos.batteryBacked = getBtCheck(hDlg, IDC_CMOSBATTERY);
                break;
            }

            if (change) {
                updateMachine();
            }
        }
        break;

    case WM_UPDATEMAHCINE:
        updateCpuFreqList(hDlg);
        updateFdcCountList(hDlg);
        
        setBtCheck(hDlg, IDC_CMOSENABLE, machine->cmos.enable, 1);
        setBtCheck(hDlg, IDC_CMOSBATTERY, machine->cmos.batteryBacked, 1);
        EnableWindow(GetDlgItem(hDlg, IDC_CMOSBATTERY), machine->cmos.enable);
        return TRUE;
    }

    return FALSE;
}

static void updateVramList(HWND hDlg) {
    int vram    = 16;
    int maxVram = 16;
    int selVram = machine->video.vramSize;
    int i;
        
    while (CB_ERR != SendDlgItemMessage(hDlg, IDC_CONF_VRAM, CB_DELETESTRING, 0, 0));

    if (machine->video.vdpVersion == VDP_V9938) {
        vram    = 16;
        maxVram = 128;
    }

    if (machine->video.vdpVersion == VDP_V9958) {
        vram    = 128;
        maxVram = 256;
    }

    for (i = 0; vram <= maxVram; i++) {
        char buffer[128];
        if (vram == 256) vram = 192;
        if (vram == 32) vram = 64;
        sprintf(buffer, "%d kB", vram);
        SendDlgItemMessage(hDlg, IDC_CONF_VRAM, CB_ADDSTRING, 0, (LPARAM)buffer);
        if (machine->video.vramSize >= 1024 * vram) {
            SendDlgItemMessage(hDlg, IDC_CONF_VRAM, CB_SETCURSEL, i, 0);
            selVram = 1024 * vram;
        }
        vram *= 2;
    }
   
    machine->video.vramSize = selVram;
}

static int getVramList(HWND hDlg) {
    int vramSize = machine->video.vramSize;
    char vramSel[64];
    int idx = SendMessage(GetDlgItem(hDlg, IDC_CONF_VRAM), CB_GETCURSEL, 0, 0);
    int rv = SendMessage(GetDlgItem(hDlg, IDC_CONF_VRAM), CB_GETLBTEXT, idx, (LPARAM)vramSel);

    if (0 == strcmp(vramSel, "16 kB")) {
        machine->video.vramSize = 16 * 1024;
    }
    if (0 == strcmp(vramSel, "64 kB")) {
        machine->video.vramSize = 64 * 1024;
    }
    if (0 == strcmp(vramSel, "128 kB")) {
        machine->video.vramSize = 128 * 1024;
    }
    if (0 == strcmp(vramSel, "192 kB")) {
        machine->video.vramSize = 192 * 1024;
    }

    return vramSize != machine->video.vramSize;
}

static BOOL CALLBACK chipsProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlg, IDC_VIDEOGROUPBOX), langConfChipVideoGB());
        SetWindowText(GetDlgItem(hDlg, IDC_VIDEOCHIP), langConfChipVideoChip());
        SetWindowText(GetDlgItem(hDlg, IDC_VIDEORAM), langConfChipVideoRam());

        SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_ADDSTRING, 0, (LPARAM)"TMS9929A   (PAL)");
        SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_ADDSTRING, 0, (LPARAM)"TMS99x8A   (NTSC)");
        SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_ADDSTRING, 0, (LPARAM)"V9938");
        SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_ADDSTRING, 0, (LPARAM)"V9958");

        SendMessage(hDlg, WM_UPDATEMAHCINE, 0, 0);

        return FALSE;

    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
        SetBkColor((HDC)wParam, GetSysColor(COLOR_MENU));
        return (BOOL)hBrush;
        
    case WM_ERASEBKGND:
        return TRUE;

    case WM_COMMAND:
        {
            int change = 1;

            switch (LOWORD(wParam)) {
            case IDC_CONF_VIDEOCHIP:
                {
                    char videoSel[64];
                    VdpVersion vdpVersion = machine->video.vdpVersion;
                    int idx = SendMessage(GetDlgItem(hDlg, IDC_CONF_VIDEOCHIP), CB_GETCURSEL, 0, 0);
                    int rv = SendMessage(GetDlgItem(hDlg, IDC_CONF_VIDEOCHIP), CB_GETLBTEXT, idx, (LPARAM)videoSel);

                    if (rv != CB_ERR) {
                        if (0 == strcmp(videoSel, "TMS9929A   (PAL)")) {
                            machine->video.vdpVersion = VDP_TMS9929A;
                        }
                        if (0 == strcmp(videoSel, "TMS99x8A   (NTSC)")) {
                            machine->video.vdpVersion = VDP_TMS99x8A;
                        }
                        if (0 == strcmp(videoSel, "V9938")) {
                            machine->video.vdpVersion = VDP_V9938;
                        }
                        if (0 == strcmp(videoSel, "V9958")) {
                            machine->video.vdpVersion = VDP_V9958;
                        }
                    }
                    change = vdpVersion != machine->video.vdpVersion;
                    if (change) {
                        machine->video.vramSize = 0x20000;
                        updateVramList(hDlg);
                    }
                }
                break;
            case IDC_CONF_VRAM:
                change = getVramList(hDlg);
                break;
            default:
                change = 0;
            }

            if (change) {
                updateMachine();
            }
        }
        break;

    case WM_UPDATEMAHCINE:
        switch (machine->video.vdpVersion) {
        case VDP_TMS9929A:  SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_SETCURSEL, 0, 0); break;
        case VDP_TMS99x8A:  SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_SETCURSEL, 1, 0); break;
        case VDP_V9938: SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_SETCURSEL, 2, 0); break;
        case VDP_V9958: SendDlgItemMessage(hDlg, IDC_CONF_VIDEOCHIP, CB_SETCURSEL, 3, 0); break;
        }
        updateVramList(hDlg);
        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK saveProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        {
            char buffer[128];
            SetWindowText(hDlg, langConfSaveTitle());

            sprintf(buffer, "%s\n\n    \"%s\" ?", langConfSaveText(), tmpMachineName);

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
        DestroyWindow(hDlgSlots);
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK closeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        SetWindowText(hDlg, langConfExitSaveTitle());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SAVEDLG_TEXT), langConfExitSaveText());

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
        DestroyWindow(hDlgSlots);
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK discardProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {        
    case WM_INITDIALOG:
        SetWindowText(hDlg, langConfDiscardTitle());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_SAVEDLG_TEXT), langConfExitSaveText());

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
        DestroyWindow(hDlgSlots);
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK saveAsProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    static char **machineNameList;
    switch (iMsg) {        
    case WM_INITDIALOG:
        SetWindowText(hDlg, langConfSaveAsTitle());
        SetWindowText(GetDlgItem(hDlg, IDC_MACHINENAMETEXT), langConfSaveAsMachineName());
        SetWindowText(GetDlgItem(hDlg, IDOK), langDlgSave());
        SetWindowText(GetDlgItem(hDlg, IDCANCEL), langDlgCancel());

        {
            char** machineNames;
            int index = 0;

            machineNameList = machineGetAvailable(0);
            machineNames = machineNameList;

            EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);

            while (*machineNames != NULL) {
                SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_ADDSTRING, 0, (LPARAM)*machineNames);

                if (0 == strcmpnocase(*machineNames, machineName)) {
                    SetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), machineName);
                    SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_SETCURSEL, index, 0);
                    EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
                }
                machineNames++;
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
                char buffer[64];
                char** machineNames = machineNameList;
                int index = 0;

                GetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), buffer, 63);

                EnableWindow(GetDlgItem(hDlg, IDOK), strlen(buffer) != 0);      

                SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_SETCURSEL, -1, 0);

                while (*machineNames != NULL) {
                    if (0 == strcmpnocase(*machineNames, buffer)) {
                        SendDlgItemMessage(hDlg, IDC_MACHINELIST, LB_SETCURSEL, index, 0);
                    }
                    machineNames++;
                    index++;
                }
            }
            return TRUE;
        case IDOK:
            GetWindowText(GetDlgItem(hDlg, IDC_MACHINENAME), tmpMachineName, 63);
            EndDialog(hDlg, TRUE);
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hDlgSlots);
        EndDialog(hDlg, FALSE);
        return TRUE;
    }

    return FALSE;
}

static BOOL CALLBACK configProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
        
    case WM_INITDIALOG:
        updateDialogPos(hDlg, DLG_ID_MACHINECONFIG, 0, 1);
        hDlgMain = hDlg;

        SetWindowText(hDlg, langConfTitle());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFSAVE), langDlgSave());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFSAVEAS), langDlgSaveAs());
        SetWindowText(GetDlgItem(hDlg, IDC_CONFRUN), langDlgRun());
        SetWindowText(GetDlgItem(hDlg, IDC_CLOSE), langDlgClose());
        SetWindowText(GetDlgItem(hDlg, IDC_CONF_CONFIGTEXT), langConfConfigText());

        hDlgSlots  = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_SLOTS),  GetDlgItem(hDlg, IDC_CONF_TAB), slotProc);
        hDlgMemory = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_MEMORY), GetDlgItem(hDlg, IDC_CONF_TAB), memoryProc);
        hDlgChips  = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_VIDEO),  GetDlgItem(hDlg, IDC_CONF_TAB), chipsProc);
        hDlgExtras = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_EXTRAS), GetDlgItem(hDlg, IDC_CONF_TAB), extrasProc);

        SetWindowPos(hDlgSlots,  NULL, 3, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        SetWindowPos(hDlgMemory, NULL, 3, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        SetWindowPos(hDlgChips,  NULL, 3, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        SetWindowPos(hDlgExtras, NULL, 3, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        
        ShowWindow(hDlgSlots, SW_NORMAL);

        {
            TCITEM tcItem = { TCIF_TEXT, 0, 0, 0, 0, -1, 0 };

            tcItem.pszText = langConfSlotLayout();
            TabCtrl_InsertItem(GetDlgItem(hDlg, IDC_CONF_TAB), 0, &tcItem);

            tcItem.pszText = langConfMemory();
            TabCtrl_InsertItem(GetDlgItem(hDlg, IDC_CONF_TAB), 1, &tcItem);

            tcItem.pszText = langConfChipEmulation();
            TabCtrl_InsertItem(GetDlgItem(hDlg, IDC_CONF_TAB), 2, &tcItem);

            tcItem.pszText = langConfChipExtras();
            TabCtrl_InsertItem(GetDlgItem(hDlg, IDC_CONF_TAB), 3, &tcItem);
        }

        updateMachineList(hDlg);

        return FALSE;

    case WM_NOTIFY:
        switch (wParam) {
        case IDC_CONF_TAB:
            if ((((NMHDR FAR *)lParam)->code) == TCN_SELCHANGE) {
                ShowWindow(hDlgSlots,  SW_HIDE);
                ShowWindow(hDlgMemory, SW_HIDE);
                ShowWindow(hDlgChips,  SW_HIDE);
                ShowWindow(hDlgExtras, SW_HIDE);
                switch(TabCtrl_GetCurSel(GetDlgItem(hDlg, IDC_CONF_TAB))) {
                case 0:
                default:
                    ShowWindow(hDlgSlots, SW_NORMAL);
                    break;
                case 1:
                    ShowWindow(hDlgMemory, SW_NORMAL);
                    break;
                case 2:
                    ShowWindow(hDlgChips, SW_NORMAL);
                    break;
                case 3:
                    ShowWindow(hDlgExtras, SW_NORMAL);
                    break;
                }
            }
            break;
        }
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_CONF_CONFIGS:
            {
                static volatile int isCheckingConfigs = 0;

                if (isCheckingConfigs == 0 && HIWORD(wParam) == CBN_SELCHANGE) {
                    char machineNameSel[64];
                    int idx;
                    int rv;

                    isCheckingConfigs = 1;

                    idx = SendMessage(GetDlgItem(hDlg, IDC_CONF_CONFIGS), CB_GETCURSEL, 0, 0);
                    rv = SendMessage(GetDlgItem(hDlg, IDC_CONF_CONFIGS), CB_GETLBTEXT, idx, (LPARAM)machineNameSel);
                
                    if (rv != CB_ERR) {
                        if (strcmp(machineNameSel, machineName)) {
                            if (machineModified) {
                                int rv;
                                rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, discardProc);
                                if (rv) {
                                    machineModified = 0;
                                }
                            }

                            if (machineModified != 0) {
                                char** machineNames = machineGetAvailable(0);
                                int index = 0;

                                while (*machineNames != NULL) {
                                    if (0 == strcmp(*machineNames, machineName)) {
                                        SendDlgItemMessage(hDlg, IDC_CONF_CONFIGS, CB_SETCURSEL, index, 0);
                                        break;
                                    }
                                    machineNames++;
                                    index++;
                                }
                            }
                            else {
                                Machine* newMachine = machineCreate(machineNameSel);
                                if (newMachine != NULL) {
                                    strcpy(machineName, machineNameSel);
                                    machineDestroy(machine);
                                    machine = newMachine;
                                    machineUpdate(machine);
                                    memcpy(machineRef, machine, sizeof(Machine));
                                }
                            }
                            
                            updateMachine();
                        }
                    }

                    isCheckingConfigs = 0;
                }

                return TRUE;
            }

        case IDC_CONFSAVE:
            {
                int rv;

                strcpy(tmpMachineName, machineName);

                rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, saveProc);
                if (rv) {
                    machineUpdate(machine);
                    memcpy(machineRef, machine, sizeof(Machine));

                    machineSave(machine);

                    updateMachine();
                }
            }
            return TRUE;

        case IDC_CONFSAVEAS:
            {
                int rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONF_SAVEAS), hDlg, saveAsProc);
                if (rv) {
                    Machine* tmpMachine;

                    machineUpdate(machine);
                    memcpy(machineRef, machine, sizeof(Machine));

                    tmpMachine = machineCreate(tmpMachineName);
                    if (tmpMachine != NULL) {
                        rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, saveProc);
                        machineDestroy(tmpMachine);
                    }
            
                    if (rv) {
                        strcpy(machineName, tmpMachineName);
                        strcpy(machine->name, tmpMachineName);
                        machineSave(machine);
                        machineUpdate(machine);
                        memcpy(machineRef, machine, sizeof(Machine));
                        updateMachine();
                        updateMachineList(hDlg);
                    }
                }
            }
            return TRUE;

        case IDC_CONFRUN:
            {
                int rv = 1;

                if (machineModified) {
                    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, closeProc);
                }
                if (rv) {
                    machineModified = 0;
                    EndDialog(hDlg, TRUE);
                }
            }
            return TRUE;

        case IDC_CLOSE:
            {
                int rv = 1;

                if (machineModified) {
                    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, closeProc);
                }
                if (rv) {
                    machineModified = 0;
                    EndDialog(hDlg, FALSE);
                }
            }
            return TRUE;
        }
        return FALSE;

    case WM_CLOSE:
        {
            int rv = 1;

            if (machineModified) {
                rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SAVEDLG), hDlg, closeProc);
            }
            if (rv) {
                DestroyWindow(hDlgSlots);
                EndDialog(hDlg, FALSE);
            }
        }
        return TRUE;

    case WM_DESTROY:
        saveDialogPos(hDlg, DLG_ID_MACHINECONFIG);
        return 0;
    }

    return FALSE;
}



int confShowDialog(HWND hwnd, char* initMachineName) {
    BOOL rv;

    strcpy(machineName, initMachineName);
    machineModified = 0;

    if (hBrush == NULL) {
        hBrush = CreateSolidBrush(GetSysColor(COLOR_MENU));
    }

    machine = machineCreate(machineName);
    machineRef = calloc(1, sizeof(Machine));
    if (machine == NULL) {
        machine = calloc(1, sizeof(Machine));
    }
    machineUpdate(machine);
    memcpy(machineRef, machine, sizeof(Machine));

    rv = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MACHINECONFIG), hwnd, configProc);

    if (rv) {
        strcpy(initMachineName, machineName);
    }

    return rv;

    machineDestroy(machine);
    machineDestroy(machineRef);
}



/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Help.c,v $
**
** $Revision: 1.19 $
**
** $Date: 2006/07/07 00:20:20 $
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
#include "Win32Help.h"
#include "Win32Common.h"
#include "Language.h"
#include "build_number.h"
#include "Resource.h"
#include "version.h"
#include <stdio.h>
 
static BOOL CALLBACK aboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, TRUE);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, TRUE);
        return TRUE;

    case WM_DESTROY:
        saveDialogPos(hDlg, DLG_ID_ABOUT);
        return 0;

    case WM_INITDIALOG:
        {
            char aboutText[4096];
                sprintf(aboutText, "%s\r\n\r\n"
                                     "%s%\t%s\r\n"
                                     "%s%\t%d\r\n"
                                     "%s%\t%s\r\n\r\n"
                                     "%s\r\n\r\n\r\n"
                                     "%s\r\n\r\n"
                              
                                     "Daniel Vik\r\n"
                                     "Benoît Delvaux\r\n"
                                     "Tomas Karlsson\r\n"
                                     "Ray Zero\r\n"
                                     "Ricardo Bittencourt\r\n"
                                     "Johan van Leur\r\n\r\n\r\n"
   
                                     "%s\r\n\r\n"

                                     "Albert Beevendorp\r\n"
                                     "Rudolf Lechleitner\r\n"
                                     "Hiro\r\n"
                                     "BouKiCHi\r\n"
                                     "Vincent van Dam\r\n"
                                     "NYYRIKKI\r\n"
                                     "SaebaRYO\r\n"
                                     "Tobias Keizer\r\n"
                                     "Sandy Pleyte\r\n"
                                     "Marco Casali\r\n"
                                     "UC_ARS\r\n"
                                     "nhlay\r\n"
                                     "Rob Hiep\r\n"
                                     "Fabio Albergaria Dias\r\n"
                                     "SLotman\r\n"
                                     "Tanni\r\n"
                                     "Eduardo Mello\r\n"
                                     "Jon Cortázar Abraido\r\n"
                                     "Hondarer\r\n"
                                     "Wouter Vermaelen\r\n"
                                     "Manuel Bilderbeek\r\n"
                                     "Maarten ter Huurne\r\n"
                                     "Patrick van Arkel\r\n"
                                     "Martijn van Tienen\r\n"
                                     "Laurent Halter\r\n"
                                     "Glafouk\r\n"
                                     "Ulver\r\n"
                                     "Nicolas Beyaert\r\n"
                                     "Víctor Fernández Sánchez\r\n"
                                     "William Ouwehand\r\n"
                                     "Jacek Bogucki (Gucek)\r\n"
                                     "Miikka \"MP83\" Poikela\r\n"
                                     "Jussi Pitkänen\r\n"
                                     "Atarulum\r\n"
                                     "MkII\r\n"
                                     "Kobayashi Michiko\r\n"
                                     "Davide Platania a.k.a. Kruznak\r\n"
                                     "Shimanuki Koshi\r\n"
                                     "Roger Filipe\r\n"
                                     "Amer Dugmag\r\n"
                                     "Alex Wulms\r\n"
                                     "Mitsutaka Okazaki\r\n"
                                     "Tatsuyuki Satoh\r\n"
                                     "Marat Fayzullin\r\n"
                                     "Jarek Burczynski\r\n"
                                     "R. Belmont\r\n"
                                     "O. Galibert\r\n"
                                     "Gilles Vollant\r\n\r\n\r\n"
            
                                     "%s",
                          langDlgAboutAbout(),
                          langDlgAboutVersion(),
                          BLUE_MSX_VERSION,
                          langDlgAboutBuildNumber(),
                          BUILD_NUMBER,
                          langDlgAboutBuildDate(),
                          BUILD_DATE,
                          langDlgAboutCreat(),
                          langDlgAboutDevel(),
                          langDlgAboutThanks(),
                          langDlgAboutLisence());

            updateDialogPos(hDlg, DLG_ID_ABOUT, 0, 1);
            SetWindowText(hDlg, langDlgAboutTitle());
            SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());

            SendMessage(GetDlgItem(hDlg, IDC_ABOUTTEXT), WM_SETTEXT, 0, (LPARAM)aboutText);
        }
        return FALSE;
    }

    return FALSE;
}

void helpShowAbout(HWND hwnd) {
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, aboutDlgProc);
}


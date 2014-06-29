/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Help.c,v $
**
** $Revision: 1.31 $
**
** $Date: 2008-06-08 19:27:46 $
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

    case WM_CTLCOLORSTATIC:
        return (BOOL)GetSysColorBrush(COLOR_WINDOW);
    
    case WM_CLOSE:
        EndDialog(hDlg, TRUE);
        return TRUE;

    case WM_DESTROY:
        saveDialogPos(hDlg, DLG_ID_ABOUT);
        return 0;

    case WM_INITDIALOG:
        {
            char aboutText[4096];

            char buildDate[32] = BUILD_DATE;
            int i;

            for (i = 0; buildDate[i] != ' '; i++);
            buildDate[i] = '/';

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
                                "hap\r\n"
                                "Ricardo Bittencourt\r\n"
                                "white cat\r\n\r\n\r\n"

                                "%s\r\n\r\n"

                                "Tim Brugman\r\n"
                                "Takayoshi Sasano\r\n"
                                "Albert Beevendorp\r\n"
                                "Alex Wulms\r\n"
                                "Alexandre Souza\r\n"
                                "Amer Dugmag\r\n"
                                "Atarulum\r\n"
                                "BouKiCHi\r\n"
                                "David Heremans\r\n"
                                "Davide Platania a.k.a. Kruznak\r\n"
                                "Eduardo Mello\r\n"
                                "Fabio Albergaria Dias\r\n"
                                "Gilles Vollant\r\n"
                                "Glafouk\r\n"
                                "Hiro\r\n"
                                "Hondarer\r\n"
                                "Jacek Bogucki (Gucek)\r\n"
                                "Jarek Burczynski\r\n"
                                "Johan van Leur\r\n"
                                "Jon Cortázar Abraido\r\n"
                                "Jorrith Schaap\r\n"
                                "Jussi Pitkänen\r\n"
                                "Kobayashi Michiko\r\n"
                                "Laurent Halter\r\n"
                                "Luciano Sturaro\r\n"
                                "Maarten ter Huurne\r\n"
                                "Manuel Bilderbeek\r\n"
                                "Manuel Soler\r\n"
                                "Marat Fayzullin\r\n"
                                "Marco Casali\r\n"
                                "Martijn van Tienen\r\n"
                                "Miikka \"MP83\" Poikela\r\n"
                                "Mitsutaka Okazaki\r\n"
                                "MkII\r\n"
                                "nhlay\r\n"
                                "Nicolas Beyaert\r\n"
                                "NYYRIKKI\r\n"
                                "Oleg Malyshev (ALLiGaToR)\r\n"
                                "Olivier Galibert\r\n"
                                "Patrick van Arkel\r\n"
                                "R. Belmont\r\n"
                                "Rob Hiep\r\n"
                                "Roger Filipe\r\n"
                                "Rudolf Lechleitner\r\n"
                                "SaebaRYO\r\n"
                                "Sandy Pleyte\r\n"
                                "Shimanuki Koshi\r\n"
                                "Sjoerd de Vries\r\n"
                                "SLotman\r\n"
                                "Marcelo Tini\r\n"
                                "Tanni\r\n"
                                "Tatsuyuki Satoh\r\n"
                                "Tobias Keizer\r\n"
                                "UC_ARS\r\n"
                                "Ulver\r\n"
                                "Víctor Fernández Sánchez\r\n"
                                "Vincent van Dam\r\n"
                                "William Ouwehand\r\n"
                                "Wouter Vermaelen\r\n\r\n\r\n"
    
                                "%s",
                        langDlgAboutAbout(),
                        langDlgAboutVersion(),
                        BLUE_MSX_VERSION,
                        langDlgAboutBuildNumber(),
                        BUILD_NUMBER,
                        langDlgAboutBuildDate(),
                        buildDate,
                        langDlgAboutCreat(),
                        langDlgAboutDevel(),
                        langDlgAboutThanks(),
                        langDlgAboutLisence());

            updateDialogPos(hDlg, DLG_ID_ABOUT, 0, 1);
            SetWindowText(hDlg, langDlgAboutTitle());
            SetWindowText(GetDlgItem(hDlg, IDOK), langDlgOK());

            SendMessage(GetDlgItem(hDlg, IDC_ABOUTTEXT), WM_SETTEXT, 0, (LPARAM)aboutText);
        }
        return 1;
    }

    return FALSE;
}

void helpShowAbout(HWND hwnd) {
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, aboutDlgProc);
}


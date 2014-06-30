/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32MouseEmu.c,v $
**
** $Revision: 1.8 $
**
** $Date: 2008-03-30 18:38:48 $
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
#include "Win32MouseEmu.h"
#include "ArchInput.h"

static HWND mouseHwnd;
static HCURSOR hCurs;
static int mouseIsRunning = 0;
static int mouseTimerId;
static RECT mouseCapRect;
static RECT mouseDisplayRect;
static int mouseActive;
static int mouseEnable;
static AmEnableMode mouseMode;
static int mouseDX;
static int mouseDY;
static int mouseX;
static int mouseY;
static int mouseLockDX;
static int mouseLockDY;
static int hasMouseLock;
static int mouseForceLock;
static int cursorCnt = 0;

static void mouseShowCursor(BOOL show)
{
    if (show) {
        if (cursorCnt == -1) {
            ShowCursor(TRUE);
            cursorCnt++;
        }
    }
    else {
        if (cursorCnt == 0) {
            ShowCursor(FALSE);
            cursorCnt--;
        }
    }
}

static void CALLBACK mouseEmuTimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT pt;

    if (!mouseIsRunning || !mouseActive) {
        if (hasMouseLock) {
            hasMouseLock = 0;
            mouseShowCursor(TRUE);
            ReleaseCapture();
        }
        return;
    }

    GetCursorPos(&pt);
    ScreenToClient(mouseHwnd, &pt);
    
    if (!(mouseForceLock && mouseMode == AM_ENABLE_MOUSE) && !PtInRect(&mouseCapRect, pt)) {
        if (hasMouseLock) {
            hasMouseLock = 0;
            mouseShowCursor(TRUE);
            ReleaseCapture();
        }
        return;
    }

    if (mouseMode == AM_ENABLE_LASER) {
        if (mouseDisplayRect.right - mouseDisplayRect.left > 0) {
            mouseX = 0x10000*(pt.x - mouseDisplayRect.left) / (mouseDisplayRect.right - mouseDisplayRect.left);
            mouseY = 0x10000*(pt.y - mouseDisplayRect.top)  / (mouseDisplayRect.bottom - mouseDisplayRect.top);
        }
        else {
            mouseX = 0;
            mouseY = 0;
        }
    }

    if (!hasMouseLock) {
        mouseLockDX = 0;
        mouseLockDY = 0;
        mouseDX = 0;
        mouseDY = 0;
        hasMouseLock = 1;
        mouseShowCursor(mouseMode == AM_ENABLE_LASER);
        SetCapture(mouseHwnd);
        if (mouseMode == AM_ENABLE_MOUSE) {
            pt.x = 100;
            pt.y = 100;
            ClientToScreen(mouseHwnd, &pt);
            SetCursorPos(pt.x, pt.y);
        }
    }
    else if (mouseMode == AM_ENABLE_MOUSE) {
        int DX = 100 - pt.x;
        int DY = 100 - pt.y;

        mouseDX += DX;
        mouseDY += DY;
        mouseLockDX -= DX;
        mouseLockDY -= DY;
    
        pt.x = 100;
        pt.y = 100;

        if (!mouseForceLock) {
            pt.x = mouseLockDX < -600 ? mouseCapRect.left   - 7  : mouseLockDX >  600 ? mouseCapRect.right  + 7  : pt.x;
            pt.y = mouseLockDY < -600 ? mouseCapRect.top    - 7  : mouseLockDY >  600 ? mouseCapRect.bottom + 7  : pt.y;

            if (mouseLockDX < -600 || mouseLockDX > 600 || mouseLockDY < -600 || mouseLockDY > 600) {
                hasMouseLock = 0;
                mouseShowCursor(TRUE);
                ReleaseCapture();
            }
        }

        ClientToScreen(mouseHwnd, &pt);
        SetCursorPos(pt.x, pt.y);
    }
}

void archMouseSetForceLock(int lock) {
    if (mouseForceLock == lock) {
        return;
    }

    if (lock) {
        if (!hasMouseLock) {
            POINT pt = { 100, 100 };
            ClientToScreen(mouseHwnd, &pt);
            SetCursorPos(pt.x, pt.y);
        }
    }
    else {
        if (hasMouseLock) {
            POINT pt = { 30, -3 };
            ClientToScreen(mouseHwnd, &pt);
            SetCursorPos(pt.x, pt.y);
        }
    }

    mouseForceLock = lock;
}

int mouseEmuSetCursor()
{
    if (mouseMode == AM_ENABLE_LASER) {
        SetCursor(hCurs);
        return 1;
    }

    return 0;
}

void mouseEmuInit(HWND hwnd, int timerId)
{
    hCurs = LoadCursor(NULL, IDC_CROSS); 

    mouseHwnd = hwnd;
    mouseTimerId = timerId;
    SetTimer(mouseHwnd, mouseTimerId, 10, mouseEmuTimerCallback);
}

void mouseEmuSetCaptureInfo(RECT* captureRect, RECT* displayRect) {
    if (displayRect != NULL) {
        memcpy(&mouseDisplayRect, displayRect, sizeof(RECT));
    }
    else {
        mouseActive = 0;
        memset(&mouseDisplayRect, 0, sizeof(RECT));
    }

    if (captureRect != NULL) {
        memcpy(&mouseCapRect, captureRect, sizeof(RECT));
    }
    else {
        mouseActive = 0;
        memset(&mouseCapRect, 0, sizeof(RECT));
    }
}

void mouseEmuSetRunState(int isRunning) {
    mouseIsRunning = isRunning;
}

void mouseEmuActivate(int activate) {
    mouseActive = activate;
}

void archMouseEmuEnable(AmEnableMode mode) {
    mouseMode = mode;
    if (hasMouseLock) {
        mouseShowCursor(TRUE);
        ReleaseCapture();
        hasMouseLock = 0;
    }
}

void archMouseGetState(int* dx, int* dy) {
    *dx = 0;
    *dy = 0;

    if (hasMouseLock) {
        if (mouseMode == AM_ENABLE_LASER) {
            *dx = mouseX;
            *dy = mouseY;
        }
        else {
            *dx = mouseDX;
            *dy = mouseDY;

            mouseDX = 0;
            mouseDY = 0;
        }
    }
}

int  archMouseGetButtonState(int checkAlways) {
    int buttons = 0;

    if (hasMouseLock || checkAlways) {
        if (GetAsyncKeyState(VK_LBUTTON) > 1UL) {
            buttons |= 1;
        }

        if (GetAsyncKeyState(VK_MBUTTON) > 1UL || GetAsyncKeyState(VK_RBUTTON) > 1UL) {
            buttons |= 2;
        }
    }

    return buttons;
}



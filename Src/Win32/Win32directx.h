/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32directx.h,v $
**
** $Revision: 1.9 $
**
** $Date: 2008-08-31 06:13:13 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik, Laurent Halter
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
#ifndef WIN32_DIRECTX_H
#define WIN32_DIRECTX_H

#include <windows.h>
#include "VideoRender.h"

typedef enum { 
    DXE_OK                    =  0,
    DXE_DIRECTDRAWCREATE      = -1,
    DXE_SETCOOPERATIVELEVEL   = -2,
    DXE_SETDISPLAYMODE        = -3,
    DXE_CREATESURFACE         = -4,
    DXE_CREATEATTACHEDSURFACE = -5,
    DXE_CREATECLIPPER         = -6, 
    DXE_SETHWND               = -7,
    DXE_SETCLIPPER            = -8,
    DXE_CREATEFROMWINDOW      = -9
} DirectXError;

typedef struct {
    int width;
    int height;
    int bitCount;
} DxDisplayMode;

// Fullscreen display mode
void DirectDrawInitDisplayModes();
void DirectDrawSetDisplayMode(int width, int height, int bitCount);
DxDisplayMode* DirectDrawGetDisplayMode();
int DirectDrawGetAvailableDisplayModeCount();
DxDisplayMode* DirectDrawGetAvailableDisplayMode(int index);

int DirectXUpdateSurface(Video* pVideo, int noFlip, 
                         int dstPitchY, int dstOffset, int zoom, 
                         int horizontalStretch, int verticalStretch,
                         int syncVblank, int zoomModeNormal);
void DirectXExitFullscreenMode();
int DirectXEnterFullscreenMode(HWND hwnd, int useVideoBackBuffer, int useSysMemBuffering);
int DirectXEnterWindowedMode(HWND hwnd, int width, int height, 
                             int useVideoBackBuffer, int useSysMemBuffering);
int DirectXUpdateWindowedMode(HWND hwnd, int width, int height, 
                              int useVideoBackBuffer, int useSysMemBuffering);
void DirectXSetGDISurface();

#endif

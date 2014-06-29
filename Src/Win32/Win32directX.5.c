/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32directX.c,v $
**
** $Revision: 1.21 $
**
** $Date: 2008-09-01 21:22:01 $
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
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <prsht.h>
 
#include "Win32directx.h"
#include "VideoRender.h"
#include "FrameBuffer.h"
#include "AppConfig.h"


static LPDIRECTDRAW         lpDD = NULL;            // DirectDraw object
static LPDIRECTDRAW         lpTheDD = NULL;            // DirectDraw object
static LPDIRECTDRAW         lpDDinit = NULL;
static LPDIRECTDRAWSURFACE  lpDDSPrimary = NULL;    // DirectDraw primary surface
static LPDIRECTDRAWSURFACE  lpDDSBack = NULL;       // DirectDraw back surface
static LPDIRECTDRAWSURFACE  lpDDSTemp1 = NULL;       // DirectDraw temp surface
static LPDIRECTDRAWSURFACE  lpDDSTemp2 = NULL;       // DirectDraw temp surface
static LPDIRECTDRAWSURFACE  lpDDSDraw = NULL;       // DirectDraw temp surface
static LPDIRECTDRAWCLIPPER  lpClipper = NULL;       // clipper for primary
static HWND    hwndThis;
static int     MyDevice;
static int     sysMemBuffering = 0;
static int     screenWidth  = 320;
static int     screenHeight = 240;
static char    MyDeviceName[128];
static RECT    MyDeviceRect;
static int     isFullscreen = 0;
static DWORD   allScreenHeight;

#ifndef DDBLTFAST_DONOTWAIT
#define DDBLTFAST_DONOTWAIT 0x00000020
#endif


BOOL CALLBACK OneMonitorCallback(HMONITOR hMonitor, HDC hdc, LPRECT prc, LPARAM lParam) {
    HMONITOR *phMonitorFound = (HMONITOR *)lParam;

    if (*phMonitorFound == 0) {
        *phMonitorFound = hMonitor;
    }
    else {
        *phMonitorFound = (HMONITOR) INVALID_HANDLE_VALUE;
    }

    return TRUE;
}

HMONITOR OneMonitorFromWindow(HWND hwnd) {
    HMONITOR hMonitor = NULL;
    RECT rc;

    if (hwnd) {
        GetClientRect(hwnd, &rc);
        ClientToScreen(hwnd, (LPPOINT)&rc);
        ClientToScreen(hwnd, (LPPOINT)&rc+1);
    }
    else {
        SetRectEmpty(&rc);
    }

    EnumDisplayMonitors(NULL, &rc, OneMonitorCallback, (LPARAM)&hMonitor);
    return hMonitor;
}

int DirectDrawDeviceFromWindow(HWND hwnd, LPSTR szDevice, RECT *prc) {
    HMONITOR hMonitor;

    if (GetSystemMetrics(SM_CMONITORS) <= 1) {
        if (prc) SetRect(prc,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
        if (szDevice) lstrcpy(szDevice, "DISPLAY");
        return -1;
    }

    hMonitor = OneMonitorFromWindow(hwnd);

    if (hMonitor == NULL || hMonitor == INVALID_HANDLE_VALUE) {
        if (prc) SetRectEmpty(prc);
        if (szDevice) *szDevice=0;
        return 0;
    }
    else {
        if (prc != NULL || szDevice != NULL) {
            MONITORINFOEX mi;
            mi.cbSize = sizeof(mi);
            GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi);
            if (prc) *prc = mi.rcMonitor;
            if (szDevice) lstrcpy(szDevice, mi.szDevice);
        }
        return (int)hMonitor;
    }
}

typedef struct {
    LPSTR   szDevice;
    GUID*   lpGUID;
    GUID    GUID;
    BOOL    fFound;
} FindDeviceData;

BOOL CALLBACK FindDeviceCallback(GUID* lpGUID, LPSTR szName, LPSTR szDevice, LPVOID lParam) {
    FindDeviceData *p = (FindDeviceData*)lParam;

    if (lstrcmpi(p->szDevice, szDevice) == 0) {
    if (lpGUID) {
        p->GUID = *lpGUID;
        p->lpGUID = &p->GUID;
    }
    else {
        p->lpGUID = NULL;
    }
    p->fFound = TRUE;
    return FALSE;
    }
    return TRUE;
}

#define MAX_DISPLAY_MODES 128

DxDisplayMode displayModes[MAX_DISPLAY_MODES];
static int displayModeCount = 0;
static DxDisplayMode* currentFullscreenMode = NULL;

static HRESULT WINAPI EnumDisplayModes(LPDDSURFACEDESC desc, LPVOID context) {
    int width     = desc->dwWidth;
    int height    = desc->dwHeight;
    int bitCount  = desc->ddpfPixelFormat.dwRGBBitCount;

#if 1
    if (bitCount != 16 && bitCount != 32) {
        return DDENUMRET_OK;
    }
#else
    if (!(width == 320 && height == 240 && (bitCount == 16 || bitCount == 32))) {
        if (width < 640 || height < 480 || (bitCount != 16 && bitCount != 32)) {
        }
    }
#endif


    if (displayModeCount < MAX_DISPLAY_MODES) {
        displayModes[displayModeCount].width    = width;
        displayModes[displayModeCount].height   = height;
        displayModes[displayModeCount].bitCount = bitCount;

        displayModeCount++;
    }

    return DDENUMRET_OK;
}

int DirectDrawGetAvailableDisplayModeCount() {
    return displayModeCount;
}

DxDisplayMode* DirectDrawGetAvailableDisplayMode(int index) {
    if (index >= displayModeCount) {
        return NULL;
    }
    return displayModes + index;
}

DxDisplayMode* DirectDrawGetDisplayMode() {
    return currentFullscreenMode;
}

void DirectDrawSetDisplayMode(int width, int height, int bitCount) {
    int i;
    for (i = 0; i < displayModeCount; i++) {
        if (displayModes[i].width    == width &&
            displayModes[i].height   == height &&
            displayModes[i].bitCount == bitCount)
        {
            currentFullscreenMode = displayModes + i;
            break;
        }
    }
    allScreenHeight = GetSystemMetrics(SM_CYSCREEN);
}

void DirectDrawInitDisplayModes() {
    LPDIRECTDRAW ddraw;
    displayModeCount = 0;
    DirectDrawCreate(NULL, &ddraw, NULL);
    if (ddraw != NULL) {
        IDirectDraw_EnumDisplayModes(ddraw, 0, NULL, NULL, EnumDisplayModes);
        IDirectDraw_Release(ddraw);
    }
    if (displayModeCount == 0) {
        displayModes[0].width    = 640;
        displayModes[0].height   = 480;
        displayModes[0].bitCount = 32;
        displayModeCount = 1;
    }

    currentFullscreenMode = displayModes;
}


IDirectDraw * DirectDrawCreateFromDevice(LPSTR szDevice) {
    IDirectDraw*    pdd = NULL;
    FindDeviceData  find;

    find.szDevice = szDevice;
    find.fFound   = FALSE;
    DirectDrawEnumerate(FindDeviceCallback, (LPVOID)&find);

    if (find.fFound) {
        DirectDrawCreate(find.lpGUID, &pdd, NULL);
    }

    return pdd;
}

IDirectDraw* DirectDrawCreateFromWindow(HWND hwnd) {
    IDirectDraw *pdd = NULL;
    char szDevice[80];
    OSVERSIONINFO osw;
    int supportsMonitors;

    osw.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx(&osw)) {
        return NULL;
    }

    supportsMonitors = osw.dwMajorVersion > 4 ||
                       (osw.dwMajorVersion == 4 && osw.dwMinorVersion > 0);

    if (supportsMonitors) {
        if (GetSystemMetrics(SM_CMONITORS) > 1) {
            if (DirectDrawDeviceFromWindow(hwnd, szDevice, NULL)) {
                pdd = DirectDrawCreateFromDevice(szDevice);
            }
        }
    }

    if (pdd == NULL) {
        DirectDrawCreate(NULL, &pdd, NULL);
    }

    return pdd;
}

void DirectXExitFullscreenMode()
{
    lpTheDD = NULL;

    if( lpDDSPrimary != NULL ) {
        IDirectDrawSurface_Release(lpDDSPrimary);
        lpDDSPrimary = NULL;
    }
    if( lpDDSTemp1 != NULL ) {
        IDirectDrawSurface_Release(lpDDSTemp1);
        lpDDSTemp1 = NULL;
    }
    if( lpDDSTemp2 != NULL ) {
        IDirectDrawSurface_Release(lpDDSTemp2);
        lpDDSTemp2 = NULL;
    }
    if ( lpDDSDraw != NULL ) {
        IDirectDrawSurface_Release(lpDDSDraw);
        lpDDSDraw = NULL;
    }
    if( lpDD != NULL ) {
        IDirectDraw_SetCooperativeLevel(lpDD, NULL, DDSCL_NORMAL);
        IDirectDraw_Release(lpDD);
        lpDD = NULL;
    }
    if( lpDDinit != NULL ) {
        IDirectDraw_SetCooperativeLevel(lpDDinit, NULL, DDSCL_NORMAL);
        IDirectDraw_RestoreDisplayMode(lpDDinit);
        IDirectDraw_Release(lpDDinit);
        lpDDinit = NULL;
    }

    isFullscreen = 0;

    DirectXSetGDISurface();
}


int DirectXEnterFullscreenMode(HWND hwnd, int useVideoBackBuffer, int useSysMemBuffering)
{
    DDSURFACEDESC   ddsd;
    DDSCAPS     ddscaps;
    HRESULT     ddrval;
    int width  = currentFullscreenMode->width;
    int height = currentFullscreenMode->height;
    int depth  = currentFullscreenMode->bitCount; 

    screenWidth  = width;
    screenHeight = height;

    allScreenHeight = height;

    DirectXExitFullscreenMode();
    
    sysMemBuffering = useSysMemBuffering;
    isFullscreen = 1;

    ddrval = DirectDrawCreate( NULL, &lpDDinit, NULL );
    if( ddrval != DD_OK ) {
        return DXE_DIRECTDRAWCREATE;
    }

    ddrval = IDirectDraw_SetCooperativeLevel(lpDDinit, NULL, DDSCL_NORMAL);
    ddrval = IDirectDraw_QueryInterface(lpDDinit, (GUID *)&IID_IDirectDraw, (LPVOID *)&lpDD);

    // Get exclusive mode
    ddrval = IDirectDraw_SetCooperativeLevel(lpDD, GetParent(hwnd) ? GetParent(hwnd) : hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if( ddrval != DD_OK ) {
        return DXE_SETCOOPERATIVELEVEL;
    }

    ddrval = IDirectDraw_SetDisplayMode(lpDD, width, height, depth);
    if( ddrval != DD_OK ) {
        return DXE_SETDISPLAYMODE;
    }

    // Create the primary surface with 1 back buffer
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSPrimary, NULL );
    if( ddrval != DD_OK ) {
        return DXE_CREATESURFACE;
    }

    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddrval = IDirectDrawSurface_GetAttachedSurface(lpDDSPrimary, &ddscaps, &lpDDSBack);
    if( ddrval != DD_OK ) {
        lpDDSBack = NULL;
    }

    ddrval = IDirectDraw_CreateClipper( lpDD, 0, &lpClipper, NULL );
    if( ddrval != DD_OK ) {
        return DXE_CREATECLIPPER;
    }

    ddrval = IDirectDrawClipper_SetHWnd(lpClipper, 0, hwnd);
    if( ddrval != DD_OK ) {
        return DXE_SETHWND;
    }

    ddrval = IDirectDrawSurface_SetClipper(lpDDSPrimary, lpClipper);
    if( ddrval != DD_OK ) {
        return DXE_SETCLIPPER;
    }

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | (useVideoBackBuffer ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY);
    ddsd.dwWidth = width;
    ddsd.dwHeight = height;
    ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSTemp1, NULL);

    if (ddrval != DD_OK) {
        memset(&ddsd, 0, sizeof(DDSURFACEDESC));
        ddsd.dwSize = sizeof(DDSURFACEDESC);
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        ddsd.dwWidth = width;
        ddsd.dwHeight = height;
        ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSTemp1, NULL);
        if( ddrval != DD_OK ) {
            return DXE_CREATESURFACE;
        }
    }

    if (useSysMemBuffering) {
        memset(&ddsd, 0, sizeof(DDSURFACEDESC));
        ddsd.dwSize = sizeof(DDSURFACEDESC);
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        ddsd.dwWidth = width;
        ddsd.dwHeight = height;
        ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSTemp2, NULL);
        if( ddrval != DD_OK ) {
            return DXE_CREATESURFACE;
        }
    }

    lpTheDD = lpDD;

    return DXE_OK;
}



BOOL DirectXEnterWindowedMode(HWND hwnd, int width, int height, int useVideoBackBuffer, int useSysMemBuffering)
{
    DDSURFACEDESC   ddsd;
    HRESULT     ddrval;

    lpDDSBack = NULL;
    hwndThis = hwnd;
    sysMemBuffering = useSysMemBuffering;

    screenWidth  = width;
    screenHeight = height;

    DirectXExitFullscreenMode();

    allScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    MyDevice = DirectDrawDeviceFromWindow(hwnd, MyDeviceName, &MyDeviceRect);

    lpDD = DirectDrawCreateFromWindow(hwnd);
    if( lpDD == NULL ) {
        return DXE_CREATEFROMWINDOW;
    }

    ddrval = IDirectDraw_SetCooperativeLevel(lpDD, NULL, DDSCL_NORMAL);
    if( ddrval != DD_OK ) {
        return DXE_SETCOOPERATIVELEVEL;
    }

    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddrval = IDirectDraw_CreateSurface(lpDD, &ddsd, &lpDDSPrimary, NULL );
    if( ddrval != DD_OK ) {
        return DXE_CREATESURFACE;
    }

    ddrval = IDirectDraw_CreateClipper( lpDD, 0, &lpClipper, NULL );
    if( ddrval != DD_OK ) {
        return DXE_CREATECLIPPER;
    }

    ddrval = IDirectDrawClipper_SetHWnd(lpClipper, 0, hwnd);
    if( ddrval != DD_OK ) {
        return DXE_SETHWND;
    }

    ddrval = IDirectDrawSurface_SetClipper(lpDDSPrimary, lpClipper );
    if( ddrval != DD_OK ) {
        return DXE_SETCLIPPER;
    }

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | (useVideoBackBuffer ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY);
    ddsd.dwWidth = width;
    ddsd.dwHeight = height;
    ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSTemp1, NULL);

    if (ddrval != DD_OK) {
        memset(&ddsd, 0, sizeof(DDSURFACEDESC));
        ddsd.dwSize = sizeof(DDSURFACEDESC);
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        ddsd.dwWidth = width;
        ddsd.dwHeight = height;
        ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSTemp1, NULL);
        if( ddrval != DD_OK ) {
            return DXE_CREATESURFACE;
        }
    }

    if (useSysMemBuffering) {
        memset(&ddsd, 0, sizeof(DDSURFACEDESC));
        ddsd.dwSize = sizeof(DDSURFACEDESC);
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        ddsd.dwWidth = width;
        ddsd.dwHeight = height;
        ddrval = IDirectDraw2_CreateSurface(lpDD, &ddsd, &lpDDSTemp2, NULL);
        if( ddrval != DD_OK ) {
            return DXE_CREATESURFACE;
        }
    }

    lpTheDD = lpDD;

    return DXE_OK;
}

void DirectXSetGDISurface() {
    if (isFullscreen) {
        IDirectDraw_FlipToGDISurface(lpDD);
    }
}


int DirectXUpdateWindowedMode(HWND hwnd, int width, int height, int useVideoBackBuffer, int useSysMemBuffering) {
    if (!isFullscreen) {
        if (MyDevice != DirectDrawDeviceFromWindow(hwnd, NULL, NULL)) {
            return DirectXEnterWindowedMode(hwnd, width, height, useVideoBackBuffer, useSysMemBuffering);
        }
    }
    return DXE_OK;
}

static DWORD vblankRefOffset = 0;
static DWORD vblankAdjust = 240;
void DirectXAdjustVBlankOffset() {
    DWORD s = 0;
    HRESULT rv = IDirectDraw_GetScanLine(lpTheDD, &s);
    vblankAdjust = (s + allScreenHeight - 10 - vblankRefOffset) % allScreenHeight;
}

int DirectXCheckVBlank()
{
    if (lpTheDD == NULL) {
        return FALSE;
    }

    {
        static int f = 0;
        static DWORD o = 0;
        DWORD s;
        HRESULT rv = IDirectDraw_GetScanLine(lpTheDD, &s);
        if (rv != DD_OK) {
            return FALSE;
        }
#if 0
        vblankRefOffset = s;
        s += vblankAdjust;
        if (s > allScreenHeight) {
            s -= allScreenHeight; 
        }
#endif
        if (s < o) {
            f = 0;
        }
        o = s;

#if 0
        if (f == 0) {
#else
        if (f == 0 && s > allScreenHeight - 240) {
#endif
            f = 1;
            return TRUE;
        }
    }
    return FALSE;
}

static UInt8 lowresOffscreen[640 * 480 * 4];


static int renderNoStretch(Video* pVideo, FrameBuffer* frameBuffer, int bitCount, 
                            int zoom, void* dstBuffer, int dstPitch, int canChangeZoom)
{
    int borderWidth = (320 - frameBuffer->maxWidth) / 2;
    int dstOffset = borderWidth > 0 ? borderWidth * bitCount / 8 : 0;

    zoom = videoRender(pVideo, frameBuffer, bitCount, zoom, 
                       dstBuffer, dstOffset, dstPitch, canChangeZoom);

    if (borderWidth > 0) {
        borderWidth *= zoom;
        if (bitCount == 16) {
            UInt16* ptr  = dstBuffer;                    
            int h = zoom * 240;
            while (h--) {
                memset(ptr, 0, borderWidth * sizeof(UInt16));
                memset(ptr + zoom * 320 - borderWidth, 0, borderWidth * sizeof(UInt16));
                ptr += dstPitch / sizeof(UInt16);
            }
        }
        else if (bitCount == 32) {
            UInt32* ptr  = dstBuffer;                 
            int h = zoom * 240;
            while (h--) {
                memset(ptr, 0, borderWidth * sizeof(UInt32));
                memset(ptr + zoom * 320 - borderWidth, 0, borderWidth * sizeof(UInt32));
                ptr += dstPitch / sizeof(UInt32);
            }
        }
    }

    return zoom;
}


int DirectXUpdateSurface(Video* pVideo, 
                          int noFlip, int dstPitchY, int dstOffset, int zoom, 
                          int horizontalStretch, int verticalStretch, 
                          int syncVblank, int zoomModeNormal) 
{
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE surface = NULL;
    LPDIRECTDRAWSURFACE  lpDDSTemp;
    HRESULT     ddrval;
    FrameBuffer* frameBuffer;
    POINT pt = {0, 0};
    int canChangeZoom;
    RECT destRect = { 0, 0, screenWidth, screenHeight };
    RECT rcRect = { 0, 0, screenWidth, screenHeight };
    void* surfaceBuffer;
    int lowresMode = 0; // Lowres mode is a non stretched lowres mode centering the screen with a black border if needed

    if (screenWidth < 320 || screenHeight < 240) lowresMode = 1;
    if (zoom == 1 && (screenWidth != 320 || screenHeight != 240) && !horizontalStretch) lowresMode = 1;


    if (lpTheDD == NULL) {
        return 0;
    }

    if (lpDDSPrimary == NULL) {
        return 0;
    }

    if (syncVblank && !DirectXCheckVBlank()) {
        return 0;
    }

    ClientToScreen( hwndThis, &pt );

    lpDDSTemp = sysMemBuffering ? lpDDSTemp2 : lpDDSTemp1;

    if (surface == NULL && lpDDSTemp != NULL) {
        ddsd.dwSize = sizeof(ddsd);
        do {
            ddrval = IDirectDrawSurface_Lock(lpDDSTemp, NULL, &ddsd, sysMemBuffering ? 0 : DDLOCK_WAIT, NULL);
            if (ddrval == DDERR_SURFACELOST && IDirectDrawSurface_Restore(lpDDSTemp) != DD_OK) {
                break;
            }
        } while (ddrval == DDERR_SURFACELOST);
        if (ddrval == DD_OK) {
            surface = lpDDSTemp;
        }
    }

    if (surface == NULL) {
        ddsd.dwSize = sizeof(ddsd);
        do {
            ddrval = IDirectDrawSurface_Lock(lpDDSPrimary, NULL, &ddsd, 0, NULL);
            if (ddrval == DDERR_SURFACELOST && IDirectDrawSurface_Restore(lpDDSPrimary) != DD_OK) {
                break;
            }
        } while (ddrval == DDERR_SURFACELOST);

        if (ddrval == DD_OK) {
            surface = lpDDSPrimary;
        }
    }

    if (surface == NULL) {
        return 0;
    }

    surfaceBuffer = ddsd.lpSurface;

    frameBuffer = frameBufferFlipViewFrame(syncVblank);
    if (frameBuffer == NULL) {
        frameBuffer = frameBufferGetWhiteNoiseFrame();
    }

    canChangeZoom = -1; // Allow scale down when possible
    if (isFullscreen && currentFullscreenMode->width >= 3 * 320) {
        canChangeZoom = 1; // Allow scale up in fullscreen
    }

    if (lowresMode) {
        int bytesPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount / 8;
        UInt8* rdrPtr = lowresOffscreen + (((480 - 240) / 2) * 640 + ((640 - 320) / 2)) * bytesPerPixel;
        UInt8* dstPtr = surfaceBuffer;
        UInt8* srcPtr = lowresOffscreen + (((480 - screenHeight) / 2) * 640 + ((640 - screenWidth) / 2)) * bytesPerPixel;
        int y;

        renderNoStretch(pVideo, frameBuffer, ddsd.ddpfPixelFormat.dwRGBBitCount, zoom, 
                        rdrPtr, 640 * bytesPerPixel, -1);

        for (y = 0; y < screenHeight; y++) {
            memcpy(dstPtr, srcPtr, screenWidth * bytesPerPixel);
            srcPtr += 640 * bytesPerPixel;
            dstPtr += ddsd.lPitch;
        }
    }
    else {
        if (horizontalStretch) {
            zoom = videoRender(pVideo, frameBuffer, ddsd.ddpfPixelFormat.dwRGBBitCount, zoom, 
                                surfaceBuffer, 0, ddsd.lPitch, canChangeZoom);
        }
        else {
            zoom = renderNoStretch(pVideo, frameBuffer, ddsd.ddpfPixelFormat.dwRGBBitCount, zoom, 
                                   surfaceBuffer, ddsd.lPitch, canChangeZoom);
        }
    }

    if (IDirectDrawSurface_Unlock(surface, NULL) == DDERR_SURFACELOST) {
        IDirectDrawSurface_Restore(surface);
        IDirectDrawSurface_Unlock(surface, NULL);
    }

    if (sysMemBuffering) {
        do {
            ddrval = IDirectDrawSurface_BltFast(lpDDSTemp1, 0, 0, surface, NULL, DDBLTFAST_DONOTWAIT);

            if (ddrval == DDERR_SURFACELOST) {
                ddrval = IDirectDrawSurface_Restore(lpDDSTemp1);
            }
        } while (ddrval == DDERR_WASSTILLDRAWING);
        surface = lpDDSTemp1;
    }

    destRect.bottom += dstPitchY;
    if (destRect.right  < 64) destRect.right = 64;
    if (destRect.bottom < 64)  destRect.bottom = 64;

    pt.x -= MyDeviceRect.left;
    pt.y -= MyDeviceRect.top;
    pt.y += dstOffset;
    OffsetRect(&destRect, pt.x, pt.y);

    if (!lowresMode) {
        rcRect.top = 0;
        rcRect.left = 0;
        rcRect.bottom = 240 * zoom;
        rcRect.right  = 320 * zoom;

        if (horizontalStretch) {
            rcRect.right -= (320 - frameBuffer->maxWidth) * zoom;
        }
        rcRect.top -= dstPitchY;

        if (verticalStretch) {
            rcRect.top    += 7 * zoom;
            rcRect.bottom -= 7 * zoom;
        }
    }

    if (zoomModeNormal) {
        static int deltaWidth = -9999;
        static int deltaHeight = -9999;
        if (deltaWidth == -9999) {
            deltaWidth  = (640 - appConfigGetInt("screen.normal.width", 640)) / 2;
            deltaHeight = (480 - appConfigGetInt("screen.normal.height", 480)) / 2;
        }
        destRect.right  -= deltaWidth;
        destRect.left   -= deltaWidth;
        destRect.top    -= deltaHeight;
        destRect.bottom -= deltaHeight;
    }

    if (syncVblank) {
        DirectXAdjustVBlankOffset();
    }

    if (lpDDSBack != NULL && !noFlip) {
	    DDSCAPS ddscaps = { DDSCAPS_BACKBUFFER };
        do {
            ddrval = IDirectDrawSurface_Blt(lpDDSBack, &destRect, surface, &rcRect, DDBLT_ASYNC, NULL);
            if (ddrval == DDERR_SURFACELOST) {
                ddrval = IDirectDrawSurface_Restore(lpDDSBack);
            }
        } while (ddrval == DDERR_WASSTILLDRAWING);
        do {
            ddrval = IDirectDrawSurface_Flip(lpDDSPrimary, NULL, 0 );
            if (ddrval == DDERR_SURFACELOST) {
                IDirectDrawSurface_Restore(lpDDSPrimary);
                break;
            }
        } while (ddrval == DDERR_WASSTILLDRAWING);
    	IDirectDrawSurface_GetAttachedSurface(lpDDSPrimary, &ddscaps, &lpDDSBack);
    }
    else {
        ddrval = IDirectDrawSurface_Blt(lpDDSPrimary, &destRect, surface, &rcRect, DDBLT_ASYNC, NULL);

        if (ddrval == DDERR_SURFACELOST) {
            ddrval = IDirectDrawSurface_Restore(lpDDSPrimary);
        }
    }

    return 1;
}

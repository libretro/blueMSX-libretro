
#ifndef WIN32_D3D_H
#define WIN32_D3D_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include "VideoRender.h"
#include "Properties.h"
#include "VDP.h"
#include "Win32Keyboard.h"

void D3DExitFullscreenMode();
int D3DEnterFullscreenMode(HWND hwnd, int useVideoBackBuffer, int useSysMemBuffering);
BOOL D3DEnterWindowedMode(HWND hwnd, int width, int height, int useVideoBackBuffer, int useSysMemBuffering);
int D3DUpdateWindowedMode(HWND hwnd, int width, int height, int useVideoBackBuffer, int useSysMemBuffering);
int D3DUpdateSurface(HWND hWnd, Video* pVideo, int syncVblank, D3DProperties * d3dProperties);


#ifdef __cplusplus
}
#endif
#endif

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32ScreenShot.h,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/05/30 04:10:19 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Laurent Halter
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
#ifndef WIN32_SCREENSHOT_H
#define WIN32_SCREENSHOT_H

#include <windows.h>

#include "Properties.h"

void screenshotSetDirectory(char* directory, char* prefix);
int  ScreenShot(Properties* properties, HWND hwnd, int width, int height, int xOffset, int yOffset, int png);
void* ScreenShot2(void* src, int srcPitch, int width, int height, int* bitmapSize, int png);
void ScreenShot3(Properties* properties, void* src, int srcPitch, int width, int height, int png);
HBITMAP BitmapFromData(void* bmp);

#endif //WIN32_SCREENSHOT_H

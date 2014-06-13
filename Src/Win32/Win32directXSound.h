/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32directXSound.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/06/13 17:40:08 $
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
#ifndef WIN32_DIRECTX_SOUND
#define WIN32_DIRECTX_SOUND

#include <windows.h>
#include "MsxTypes.h"
#include "AudioMixer.h"

typedef struct DxSound DxSound;

DxSound* dxSoundCreate(HWND hwnd, Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels);
void dxSoundDestroy(DxSound* dxSound);
void dxSoundSuspend(DxSound* dxSound);
void dxSoundResume(DxSound* dxSound);

#endif //WIN32_DIRECTX_SOUND

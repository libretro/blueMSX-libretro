/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32snd.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2004/12/06 07:32:03 $
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
#include "Win32Sound.h"
#include "Win32directXSound.h"
#include "Win32wmmSound.h"

#include "ArchSound.h"

static DxSound* dxSound = NULL;
static WmmSound* wmmSound = NULL;

static HWND        cfgHwnd   = NULL;
static SoundDriver cfgDriver = SOUND_DRV_NONE;

void soundDriverConfig(HWND hwnd, SoundDriver driver)
{
    cfgHwnd   = hwnd;
    cfgDriver = driver;
}

void archSoundCreate(Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels)
{
    archSoundDestroy();
 
    switch (cfgDriver) {
    case SOUND_DRV_NONE:
        break;
    case SOUND_DRV_DIRECTX:
        dxSound = dxSoundCreate(cfgHwnd, mixer, sampleRate, bufferSize, channels);
        break;
    case SOUND_DRV_WMM:
        wmmSound = wmmSoundCreate(cfgHwnd, mixer, sampleRate, bufferSize, channels);
        break;
    }
}

void archSoundDestroy(void) 
{
    if (dxSound) {
        dxSoundDestroy(dxSound);
        dxSound = NULL;
    }
    if (wmmSound) {
        wmmSoundDestroy(wmmSound);
        wmmSound = NULL;
    }
}

void archSoundResume(void) 
{
    if (dxSound) {
        dxSoundResume(dxSound);
    }
    if (wmmSound) {
        wmmSoundResume(wmmSound);
    }
}


void archSoundSuspend(void)
{
    if (dxSound) {
        dxSoundSuspend(dxSound);
    }
    if (wmmSound) {
        wmmSoundSuspend(wmmSound);
    }
}

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Linux/blueMSXlite/LinuxSound.c,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/09/25 07:39:07 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vikl, Tomas Karlsson
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
#include "ArchSound.h"
#include "libretro.h"

static retro_audio_sample_batch_t audio_batch_cb;
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }

static Int32 soundWrite(void* dummy, Int16 *buffer, UInt32 count)
{
   audio_batch_cb(buffer, count / 2);
}

void archSoundCreate(Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels) {
    mixerSetStereo(mixer, channels == 2);
    mixerSetWriteCallback(mixer, soundWrite, NULL, 512);
}

void archSoundDestroy(void) {}
void archSoundResume(void) {}
void archSoundSuspend(void) {}

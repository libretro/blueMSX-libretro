/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/KeyClick.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2005/02/08 00:48:08 $
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
#include "KeyClick.h"
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE     10000


static Int32* audioKeyClickSync(void* ref, UInt32 count);

struct AudioKeyClick
{
    Mixer* mixer;
    Int32 handle;

    Int32 sampleVolume;
    Int32 sampleVolumeSum;
    Int32 oldSampleVolume;
    Int32 ctrlVolume;
    Int32 daVolume;
    Int32 count;

    Int32  buffer[BUFFER_SIZE];
};

AudioKeyClick* audioKeyClickCreate(Mixer* mixer)
{
    AudioKeyClick* keyClick = (AudioKeyClick*)calloc(1, sizeof(AudioKeyClick));

    keyClick->mixer = mixer;

    keyClick->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_KEYBOARD, 0, audioKeyClickSync, keyClick);

    return keyClick;
}

void audioKeyClickDestroy(AudioKeyClick* keyClick)
{
    mixerUnregisterChannel(keyClick->mixer, keyClick->handle);
    free(keyClick);
}

void audioKeyClick(AudioKeyClick* keyClick, UInt8 value)
{
    mixerSync(keyClick->mixer);
    keyClick->count++;
    keyClick->sampleVolumeSum += value ? 32000 : 0;
    keyClick->sampleVolume = value ? 32000 : 0;
}

static Int32* audioKeyClickSync(void* ref, UInt32 count)
{
    AudioKeyClick* keyClick = (AudioKeyClick*)ref;
    UInt32 index = 0;

    if (keyClick->count) {
        Int32 sampleVolume = keyClick->sampleVolumeSum / keyClick->count;
        keyClick->count = 0;
        keyClick->sampleVolumeSum = 0;
        keyClick->ctrlVolume = sampleVolume - keyClick->oldSampleVolume + 0x3fe7 * keyClick->ctrlVolume / 0x4000;
        keyClick->oldSampleVolume = sampleVolume;
        keyClick->ctrlVolume = 0x3fe7 * keyClick->ctrlVolume / 0x4000;

        /* Perform simple 1 pole low pass IIR filtering */
        keyClick->daVolume += 2 * (keyClick->ctrlVolume - keyClick->daVolume) / 3;
        keyClick->buffer[index++] = 8 * keyClick->daVolume;
    }

    keyClick->ctrlVolume = keyClick->sampleVolume - keyClick->oldSampleVolume + 0x3fe7 * keyClick->ctrlVolume / 0x4000;
    keyClick->oldSampleVolume = keyClick->sampleVolume;

    for (index; index < count; index++) {
        /* Perform DC offset filtering */
        keyClick->ctrlVolume = 0x3fe7 * keyClick->ctrlVolume / 0x4000;

        /* Perform simple 1 pole low pass IIR filtering */
        keyClick->daVolume += 2 * (keyClick->ctrlVolume - keyClick->daVolume) / 3;
        keyClick->buffer[index] = 7 * keyClick->daVolume;
    }

    return keyClick->buffer;
}

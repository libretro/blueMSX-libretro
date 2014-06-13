/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/DAC.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/01 20:09:00 $
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
#include "DAC.h"
#include "Board.h"
#include <stdlib.h>
#include <string.h>


#define OFFSETOF(s, a) ((int)(&((s*)0)->a))

#define BUFFER_SIZE     20000

static Int32* dacSyncMono(DAC* dac, UInt32 count);
static Int32* dacSyncStereo(DAC* dac, UInt32 count);
static void dacSyncChannel(DAC* dac, UInt32 count, int ch, UInt32 index, UInt32 delta);

struct DAC
{
    Mixer*  mixer;
    Int32   handle;
    DacMode mode;

    Int32   enabled;
    Int32   sampleVolume[2];
    Int32   oldSampleVolume[2];
    Int32   sampleVolumeSum[2];
    Int32   count[2];
    Int32   ctrlVolume[2];
    Int32   daVolume[2];

    Int32   defaultBuffer[BUFFER_SIZE];
    Int32   buffer[BUFFER_SIZE];
};

void dacReset(DAC* dac) {
    dac->oldSampleVolume[DAC_CH_LEFT]  = 0;
    dac->sampleVolume[DAC_CH_LEFT]     = 0;
    dac->ctrlVolume[DAC_CH_LEFT]       = 0;
    dac->daVolume[DAC_CH_LEFT]         = 0;
    dac->oldSampleVolume[DAC_CH_RIGHT] = 0;
    dac->sampleVolume[DAC_CH_RIGHT]    = 0;
    dac->ctrlVolume[DAC_CH_RIGHT]      = 0;
    dac->daVolume[DAC_CH_RIGHT]        = 0;
}

DAC* dacCreate(Mixer* mixer, DacMode mode)
{
    DAC* dac = (DAC*)calloc(1, sizeof(DAC));

    dac->mixer = mixer;
    dac->mode  = mode;

    dacReset(dac);

    if (mode == DAC_MONO) {
        dac->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_PCM, 0, dacSyncMono, dac);
    }
    else {
        dac->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_PCM, 1, dacSyncStereo, dac);
    }
    return dac;
}

void dacDestroy(DAC* dac)
{
    mixerUnregisterChannel(dac->mixer, dac->handle);
    free(dac);
}

void dacWrite(DAC* dac, DacChannel channel, UInt8 value)
{
    if (channel == DAC_CH_LEFT || channel == DAC_CH_RIGHT) {
        Int32 sampleVolume = ((Int32)value - 0x80) * 256;
        mixerSync(dac->mixer);
        dac->sampleVolume[channel]     = sampleVolume;
        dac->sampleVolumeSum[channel] += sampleVolume;
        dac->count[channel]++;
        dac->enabled = 1;
    }
}

static Int32* dacSyncMono(DAC* dac, UInt32 count)
{
    if (!dac->enabled || count == 0) {
        return dac->defaultBuffer;
    }

    dacSyncChannel(dac, count, DAC_CH_MONO, 0, 1);

    dac->enabled = dac->buffer[count - 1] != 0;

    return dac->buffer;
}

static Int32* dacSyncStereo(DAC* dac, UInt32 count)
{
    if (!dac->enabled || count == 0) {
        return dac->defaultBuffer;
    }

    dacSyncChannel(dac, count, DAC_CH_LEFT,  0, 2);
    dacSyncChannel(dac, count, DAC_CH_RIGHT, 1, 2);

    dac->enabled = dac->buffer[2 * count - 1] != 0 ||
                   dac->buffer[2 * count - 2] != 0;

    return dac->buffer;
}

static void dacSyncChannel(DAC* dac, UInt32 count, int ch, UInt32 index, UInt32 delta)
{
    count *= delta;

    if (dac->count[ch] > 0) {
        Int32 sampleVolume = dac->sampleVolumeSum[ch] / dac->count[ch];
        dac->count[ch] = 0;
        dac->sampleVolumeSum[ch] = 0;
        dac->ctrlVolume[ch] = sampleVolume - dac->oldSampleVolume[ch] + 0x3fe7 * dac->ctrlVolume[ch] / 0x4000;
        dac->oldSampleVolume[ch] = sampleVolume;
        dac->ctrlVolume[ch] = 0x3fe7 * dac->ctrlVolume[ch] / 0x4000;

        dac->daVolume[ch] += 2 * (dac->ctrlVolume[ch] - dac->daVolume[ch]) / 3;
        dac->buffer[index] = 6 * 9 * dac->daVolume[ch] / 10;
        index += delta;
    }

    dac->ctrlVolume[ch] = dac->sampleVolume[ch] - dac->oldSampleVolume[ch] + 0x3fe7 * dac->ctrlVolume[ch] / 0x4000;
    dac->oldSampleVolume[ch] = dac->sampleVolume[ch];

    for (; index < count; index += delta) {
        /* Perform DC offset filtering */
        dac->ctrlVolume[ch] = 0x3fe7 * dac->ctrlVolume[ch] / 0x4000;

        /* Perform simple 1 pole low pass IIR filtering */
        dac->daVolume[ch] += 2 * (dac->ctrlVolume[ch] - dac->daVolume[ch]) / 3;
        dac->buffer[index] = 6 * 9 * dac->daVolume[ch] / 10;
    }
}


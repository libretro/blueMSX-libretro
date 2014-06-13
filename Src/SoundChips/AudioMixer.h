/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/AudioMixer.h,v $
**
** $Revision: 1.9 $
**
** $Date: 2006/06/13 17:13:28 $
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
#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include "MsxTypes.h"

/* Type definitions */
typedef struct Mixer Mixer;

typedef enum { 
    MIXER_CHANNEL_PSG = 0,
    MIXER_CHANNEL_SCC,
    MIXER_CHANNEL_MSXMUSIC,
    MIXER_CHANNEL_MSXAUDIO,
    MIXER_CHANNEL_MOONSOUND,
    MIXER_CHANNEL_YAMAHA_SFG,
    MIXER_CHANNEL_KEYBOARD,
    MIXER_CHANNEL_PCM,
    MIXER_CHANNEL_IO,
    MIXER_CHANNEL_MIDI,
    MIXER_CHANNEL_TYPE_COUNT
} MixerAudioType;

typedef enum {
    MIXER_CHANNEL_LEFT = 0,
    MIXER_CHANNEL_RIGHT
} MixerChannelPan;

#define MAX_CHANNELS 16

typedef Int32* (*MixerUpdateCallback)(void*, UInt32);
typedef Int32 (*MixerWriteCallback)(void*, Int16*, UInt32);

/* Constructor and destructor */
Mixer* mixerCreate();
void mixerDestroy(Mixer* mixer);

Mixer* mixerGetGlobalMixer();

Int32 mixerGetMasterVolume(Mixer* mixer, int leftRight);
void mixerSetMasterVolume(Mixer* mixer, Int32 volume);
void mixerEnableMaster(Mixer* mixer, Int32 enable);
void mixerSetStereo(Mixer* mixer, Int32 stereo);

Int32 mixerGetChannelTypeVolume(Mixer* mixer, Int32 channelType, int leftRight);
void mixerSetChannelTypeVolume(Mixer* mixer, Int32 channelType, Int32 volume);
void mixerSetChannelTypePan(Mixer* mixer, Int32 channelType, Int32 pan);
void mixerEnableChannelType(Mixer* mixer, Int32 channelType, Int32 enable);
Int32 mixerIsChannelTypeActive(Mixer* mixer, Int32 channelType, Int32 reset);

/* Write callback registration for audio drivers */
void mixerSetWriteCallback(Mixer* mixer, MixerWriteCallback callback, void*, int);

/* File logging methods */
void mixerStartLog(Mixer* mixer, char* fileName);
int  mixerIsLogging(Mixer* mixer);
void mixerStopLog(Mixer* mixer);

/* Internal interface methods */
void mixerReset(Mixer* mixer);
void mixerSync(Mixer* mixer);

Int32 mixerRegisterChannel(Mixer* mixer, Int32 audioType, Int32 stereo, 
                           MixerUpdateCallback callback, void*param);
void mixerUnregisterChannel(Mixer* mixer, Int32 handle);

void mixerSetBoardFrequency(int CPUFrequency);

#endif


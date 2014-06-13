/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Sdl/SdlSound.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2006/06/24 02:27:08 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
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
#include "SDL/SDL.h"
#include <stdlib.h>

typedef struct SdlSound {
    Mixer* mixer;
    int started;
    UInt32 readPtr;
    UInt32 writePtr;
    UInt32 bytesPerSample;
    UInt32 bufferMask;
    UInt32 bufferSize;
    UInt32 skipCount;
    UInt8* buffer;
} SdlSound;

SdlSound sdlSound;
int oldLen = 0;
void soundCallback(void* userdata, Uint8* stream, int length)
{
    UInt32 avail = (sdlSound.readPtr - sdlSound.writePtr) & sdlSound.bufferMask;
oldLen = length;
    if ((UInt32)length > avail) {
//        sdlSound.readPtr = (sdlSound.readPtr - sdlSound.bufferSize / 2) & sdlSound.bufferMask;
        memset((UInt8*)stream + avail, 0, length - avail);
        length = avail;
    }

    memcpy(stream, sdlSound.buffer + sdlSound.readPtr, length);
    sdlSound.readPtr = (sdlSound.readPtr + length) & sdlSound.bufferMask;
}

static Int32 soundWrite(SdlSound* dummy, Int16 *buffer, UInt32 count)
{
    UInt32 avail;

    if (!sdlSound.started) {
        return 0;
    }

    count *= sdlSound.bytesPerSample;

    if (sdlSound.skipCount > 0) {
        if (count <= sdlSound.skipCount) {
            sdlSound.skipCount -= count;
            return 0;
        }
        count -= sdlSound.skipCount;
        sdlSound.skipCount = 0;
    }

    SDL_LockAudio();

    avail = (sdlSound.writePtr - sdlSound.readPtr) & sdlSound.bufferMask;
    if (count < avail && 0) {
        sdlSound.skipCount = sdlSound.bufferSize / 2;
    }
    else {
        if (sdlSound.writePtr + count > sdlSound.bufferSize) {
            UInt32 count1 = sdlSound.bufferSize - sdlSound.writePtr;
            UInt32 count2 = count - count1;
            memcpy(sdlSound.buffer + sdlSound.writePtr, buffer, count1);
            memcpy(sdlSound.buffer, buffer, count2);
            sdlSound.writePtr = count2;
        }
        else {
            memcpy(sdlSound.buffer + sdlSound.writePtr, buffer, count);
            sdlSound.writePtr += count;
        }
    }

    SDL_UnlockAudio();
    return 0;
}

void archSoundCreate(Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels) 
{
	SDL_AudioSpec desired;
	SDL_AudioSpec audioSpec;
	UInt16 samples = channels;

    memset(&sdlSound, 0, sizeof(sdlSound));

    bufferSize = bufferSize * sampleRate / 1000 * sizeof(Int16) / 4;

    while (samples < bufferSize) samples *= 2;

	desired.freq     = sampleRate;
	desired.samples  = samples;
	desired.channels = (UInt8)channels;
#ifdef LSB_FIRST
	desired.format   = AUDIO_S16LSB;
#else
    desired.format   = AUDIO_S16MSB;
#endif
	desired.callback = soundCallback;
	desired.userdata = NULL;
    
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        return;
    }

	if (SDL_OpenAudio(&desired, &audioSpec) != 0) {
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return;
    }

    sdlSound.bufferSize = 1;
    while (sdlSound.bufferSize < 4 * audioSpec.size) sdlSound.bufferSize *= 2;
    sdlSound.bufferMask = sdlSound.bufferSize - 1;
    sdlSound.buffer = (UInt8*)calloc(1, sdlSound.bufferSize);
    sdlSound.started = 1;
    sdlSound.mixer = mixer;
    sdlSound.bytesPerSample = audioSpec.format == AUDIO_U8 || audioSpec.format == AUDIO_S8 ? 1 : 2;
    
    mixerSetStereo(mixer, channels == 2);
    mixerSetWriteCallback(mixer, soundWrite, NULL, audioSpec.size / sdlSound.bytesPerSample);
    
	SDL_PauseAudio(0);
}

void archSoundDestroy(void) 
{
    if (sdlSound.started) {
        mixerSetWriteCallback(sdlSound.mixer, NULL, NULL, 0);
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
    sdlSound.started = 0;

}
void archSoundResume(void) 
{
	SDL_PauseAudio(0);
}

void archSoundSuspend(void) 
{
	SDL_PauseAudio(1);
}

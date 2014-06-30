/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32directXSound.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2008-05-19 12:41:13 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
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
#include "Win32directXSound.h"
#include "Win32Timer.h"
#include "Properties.h"
#include <stdio.h>
#define DIRECTSOUND_VERSION 0x0500
#include <dsound.h>

typedef enum {
    DX_SOUND_DISABLED,
    DX_SOUND_ENABLED,
    DX_SOUND_RUNNING
} DxState;

struct DxSound {
    Mixer* mixer;
    DxState state;
    UInt32 bufferOffset;
    UInt32 bufferSize;
    UInt32 fragmentCount;
    UInt32 fragmentSize;
    Int16  bytesPerSample;
    Int32  skipCount;
    UInt32 cur_writepos;
    LPDIRECTSOUNDBUFFER primaryBuffer;
    LPDIRECTSOUNDBUFFER secondaryBuffer;
    LPDIRECTSOUND directSound;
};

static void dxClear(DxSound* dxSound)
{
    void*   audioBuffer1;
    UInt32  audioSize1;
    void*   audioBuffer2;
    UInt32  audioSize2;
    HRESULT result;

    result = IDirectSoundBuffer_Lock(dxSound->primaryBuffer, 0, dxSound->bufferSize,
                                     &audioBuffer1, &audioSize1, &audioBuffer2, &audioSize2, 0);
    if (result == DSERR_BUFFERLOST) {
        IDirectSoundBuffer_Restore(dxSound->primaryBuffer);
    } else {
        if (dxSound->bytesPerSample == 2) {
            memset(audioBuffer1, 0, audioSize1);
            if (audioBuffer2) memset(audioBuffer2, 0, audioSize2);
        } else {
            memset(audioBuffer1, 0x80, audioSize1);
            if (audioBuffer2) memset(audioBuffer2, 0x80, audioSize2);
        }
        result = IDirectSoundBuffer_Unlock(dxSound->primaryBuffer, audioBuffer1, audioSize1,
                                           audioBuffer2, audioSize2);
    }
}

static int dxCanWrite(DxSound* dxSound, UInt32 start, UInt32 size) 
{
    UInt32 readPos;
    UInt32 writePos;
    UInt32 end = start + size;
    IDirectSoundBuffer_GetCurrentPosition(dxSound->primaryBuffer, &readPos, &writePos);
    if (writePos < readPos) writePos += dxSound->bufferSize;
    if (start    < readPos) start    += dxSound->bufferSize;
    if (end      < readPos) end      += dxSound->bufferSize;
    
    dxSound->cur_writepos=writePos;
    
    if (start < writePos || end < writePos) {
        return (dxSound->bufferSize - (writePos - readPos)) / 2 - dxSound->fragmentSize;
    }
    return 0;
}

static int dxWriteOne(DxSound* dxSound, Int16 *buffer, UInt32 lockSize)
{
    void*   audioBuffer1;
    UInt32  audioSize1;
    void*   audioBuffer2;
    UInt32  audioSize2;
    HRESULT result;
    UInt32  i;

    do {
        result = IDirectSoundBuffer_Lock(dxSound->primaryBuffer, dxSound->bufferOffset,
                                         lockSize,
                                         &audioBuffer1, &audioSize1, &audioBuffer2,
                                         &audioSize2, 0);

        if (result == DSERR_BUFFERLOST) {
            IDirectSoundBuffer_Restore(dxSound->primaryBuffer);
            result = IDirectSoundBuffer_Lock(dxSound->primaryBuffer, dxSound->bufferOffset,
                                             lockSize, &audioBuffer1,
                                             &audioSize1,
                                             &audioBuffer2, &audioSize2, 0);
        }
    } while ((audioSize1 + audioSize2) < lockSize);

    if (dxSound->bytesPerSample == 2) {
        memcpy(audioBuffer1,buffer,audioSize1);
        if (audioBuffer2)
            memcpy(audioBuffer2,(BYTE *)buffer + audioSize1, audioSize2);
    } else {
        Int16 *copyptr = buffer;
        for (i = 0; i < audioSize1; i++) {
            ((BYTE *)audioBuffer1)[i]=(*(copyptr++) >> 8) + 0x80;
        }
        if (audioBuffer2 != NULL) {
            for (i = 0; i < audioSize2; i++) {
                ((BYTE *)audioBuffer2)[i]=(*(copyptr++) >> 8) + 0x80;
            }
        }
    }

    result = IDirectSoundBuffer_Unlock(dxSound->primaryBuffer, audioBuffer1, audioSize1,
                                       audioBuffer2, audioSize2);
    dxSound->bufferOffset += lockSize;
    dxSound->bufferOffset %= dxSound->bufferSize;

    return 0;
}

static Int32 dxWrite(DxSound* dxSound, Int16 *buffer, UInt32 count)
{
    LONGLONG qpf=win32timer_get_uptime_freq();
    Properties* prop=propGetGlobalProperties();
    
    if (dxSound->state == DX_SOUND_ENABLED) {
        HRESULT result;
        UInt32 readPos;
        UInt32 writePos;

        IDirectSoundBuffer_GetCurrentPosition(dxSound->primaryBuffer, &readPos, &writePos);
        
        dxSound->bufferOffset  = (readPos + dxSound->bufferSize / 2) % dxSound->bufferSize;

        result = IDirectSoundBuffer_Play(dxSound->primaryBuffer, 0, 0, DSBPLAY_LOOPING);
        if (result == DSERR_BUFFERLOST) {
            result = IDirectSoundBuffer_Play(dxSound->primaryBuffer, 0, 0, DSBPLAY_LOOPING);
        }
        dxSound->state = DX_SOUND_RUNNING;
    }

    if (dxSound->state == DX_SOUND_DISABLED) {
        return 0;
    }

    count *= dxSound->bytesPerSample;
    if (dxSound->skipCount > 0) {
        dxSound->skipCount -= count;
        return 0;
    }

    dxSound->skipCount = dxCanWrite(dxSound, dxSound->bufferOffset, count);
    if (dxSound->skipCount > 0) {
        return 0;
    }
    
    if (qpf&&prop&&prop->sound.stabilizeDSoundTiming&&prop->sound.bufSize>=50) {
	UInt32 diff=dxSound->bufferOffset;
	if (diff<dxSound->cur_writepos) diff+=dxSound->bufferSize;
	diff-=dxSound->cur_writepos;
	
	/* force a short delay/speedup to prevent asynchronous timing */
    if (diff<(500UL<<(prop->sound.stereo!=0?1:0))) win32timer_uptime_offset((int)(qpf/333));
	else if (diff>(dxSound->bufferSize>>1)) win32timer_uptime_offset((int)(qpf/-333));
    }
    
    return dxWriteOne(dxSound, buffer, count);
}



DxSound* dxSoundCreate(HWND hwnd, Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels) 
{
    DxSound* dxSound = (DxSound*)calloc(1, sizeof(DxSound));
    HRESULT  result;
    DSBUFFERDESC desc;
    PCMWAVEFORMAT pcmwf;
    DSCAPS  capabilities;
    WAVEFORMATEX    wfex;

    if (dxSound->directSound == NULL) {
        result = DirectSoundCreate(NULL, &dxSound->directSound, NULL);
        if (result != DS_OK) {
            free(dxSound);
            return NULL;
        }

        result = IDirectSound_SetCooperativeLevel(dxSound->directSound, hwnd, DSSCL_EXCLUSIVE);
        if (result != DS_OK) {
            free(dxSound);
            return NULL;
        }
    }

    memset(&capabilities, 0, sizeof(DSCAPS));
    capabilities.dwSize = sizeof(DSCAPS);

    IDirectSound_GetCaps(dxSound->directSound, &capabilities);
    if ((capabilities.dwFlags & DSCAPS_PRIMARY16BIT) || (capabilities.dwFlags & DSCAPS_SECONDARY16BIT)) {
        dxSound->bytesPerSample = 2;
    } 
    else {
        dxSound->bytesPerSample = 1;
    }

    if (!((capabilities.dwFlags & DSCAPS_PRIMARYSTEREO) || (capabilities.dwFlags & DSCAPS_SECONDARYSTEREO))) {
        channels = 1;
    }

    memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
    pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
    pcmwf.wf.nChannels = channels;
    pcmwf.wf.nSamplesPerSec = sampleRate;
    pcmwf.wBitsPerSample = 8 * dxSound->bytesPerSample;
    pcmwf.wf.nBlockAlign = channels * dxSound->bytesPerSample;
    pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;

    memset(&desc, 0, sizeof(DSBUFFERDESC));
    desc.dwSize = sizeof(DSBUFFERDESC);
    desc.dwFlags = DSBCAPS_PRIMARYBUFFER;

    bufferSize = bufferSize * sampleRate / 1000 * channels * dxSound->bytesPerSample;
    dxSound->fragmentSize = 1;
    while (bufferSize / dxSound->fragmentSize >= 32 || dxSound->fragmentSize < 512) {
        dxSound->fragmentSize <<= 1;
    }
    dxSound->fragmentCount = 1 + bufferSize / dxSound->fragmentSize;
    while (dxSound->fragmentCount < 8) {
        dxSound->fragmentCount *= 2;
        dxSound->fragmentSize /= 2;
    }

    dxSound->bufferSize = dxSound->fragmentCount * dxSound->fragmentSize;

    result = IDirectSound_CreateSoundBuffer(dxSound->directSound, &desc, &dxSound->secondaryBuffer, NULL);

    if (result != DS_OK) {
        free(dxSound);
        return NULL;
    }

    memset(&desc, 0, sizeof(DSBUFFERDESC));
    desc.dwSize = sizeof(DSBUFFERDESC);
    desc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2
                   | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN
                   | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS ;

    desc.dwBufferBytes = dxSound->bufferSize;
    desc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf;

    result = IDirectSound_CreateSoundBuffer(dxSound->directSound, &desc, &dxSound->primaryBuffer, NULL);
    if (result != DS_OK) {
        free(dxSound);
        return NULL;
    }

    memset(&wfex, 0, sizeof(WAVEFORMATEX));
    wfex.wFormatTag = WAVE_FORMAT_PCM;
    wfex.nChannels = channels;
    wfex.nSamplesPerSec = sampleRate;
    wfex.wBitsPerSample = 8 * dxSound->bytesPerSample;
    wfex.nBlockAlign = channels * dxSound->bytesPerSample;
    wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

    result=IDirectSoundBuffer_SetFormat(dxSound->secondaryBuffer, &wfex);
    if (result != DS_OK) {
        free(dxSound);
        return NULL;
    }

    dxSound->bufferOffset = dxSound->bufferSize;
    dxClear(dxSound);

    dxSound->skipCount = 0;
    dxSound->state = DX_SOUND_ENABLED;
    dxSound->mixer = mixer;

    mixerSetStereo(mixer, channels == 2);
    mixerSetWriteCallback(mixer, dxWrite, dxSound, dxSound->fragmentSize / dxSound->bytesPerSample);

    return dxSound;
}

void dxSoundDestroy(DxSound* dxSound)
{
    if (dxSound == NULL) {
        return;
    }

    mixerSetWriteCallback(dxSound->mixer, NULL, NULL, 0);

    if (dxSound->directSound == NULL) {
        return;
    }

    IDirectSoundBuffer_Stop(dxSound->primaryBuffer);
    IDirectSoundBuffer_Release(dxSound->primaryBuffer);
    IDirectSound_Release(dxSound->directSound);
    dxSound->primaryBuffer = NULL;
    dxSound->directSound = NULL;
}

void dxSoundSuspend(DxSound* dxSound)
{
    if (dxSound == NULL) {
        return;
    }

    IDirectSoundBuffer_Stop(dxSound->primaryBuffer);
    dxClear(dxSound);
    dxSound->state = DX_SOUND_DISABLED;
}

void dxSoundResume(DxSound* dxSound)
{
    if (dxSound == NULL) {
        return;
    }

    dxSound->state = DX_SOUND_ENABLED;
}


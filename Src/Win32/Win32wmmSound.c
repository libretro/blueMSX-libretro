/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32wmmSound.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2008-03-30 18:38:48 $
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
#include "Win32wmmSound.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
 
#define SND_BITSPERSAMPLE 16
#define SND_BUFSIZE       512
#define SND_BUFFERS       8

struct WmmSound {
    Mixer*       mixer;
    Int16        buffer[32768];
    UInt32       bufferLength;
    UInt32       readIndex;
    UInt32       writeIndex;
    UInt32       sampleRate;
    UInt32       channels;
    HWAVEOUT     hWaveOut;
    WAVEFORMATEX waveFormat;
    DWORD        sndThreadId;
    HANDLE       hThread;
    int          sndEnabled;
};

WmmSound theWmmSound;

static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    LPWAVEHDR pHdr = (LPWAVEHDR)dwParam1;
    Int16* buffer;
    MMRESULT mmrv;
    if (uMsg != WOM_DONE) {
        return;
    }

    mmrv = waveOutUnprepareHeader(theWmmSound.hWaveOut, pHdr, sizeof(WAVEHDR));
    if (pHdr == NULL) {
        return;
    }

    buffer = (Int16*)pHdr->lpData;

    {   
        UInt32 maxIndex  = theWmmSound.bufferLength;
        UInt32 readIndex = theWmmSound.readIndex;
        Int16* srcBuffer = theWmmSound.buffer;
        UInt32 i;
        UInt32 rdWrDiff;

        rdWrDiff = (maxIndex + readIndex - theWmmSound.writeIndex) % maxIndex;
        if (rdWrDiff <= theWmmSound.channels * 1000 || rdWrDiff >= maxIndex - theWmmSound.channels * 1000) {
            readIndex = (readIndex + maxIndex / 2) % maxIndex;
        }

        for (i = 0; i < theWmmSound.channels * SND_BUFSIZE; i++) {
            readIndex = (readIndex + 1) % maxIndex;
            buffer[i] = srcBuffer[readIndex]; 
        }

        theWmmSound.readIndex = readIndex;
    }

    mmrv = waveOutPrepareHeader(theWmmSound.hWaveOut, pHdr, sizeof(WAVEHDR));
    if(mmrv == 0) {
        mmrv = waveOutWrite(theWmmSound.hWaveOut, pHdr, sizeof(WAVEHDR));
    }
}

static DWORD WINAPI soundThread(void* param) 
{
    WmmSound* wmmSound = (WmmSound*)param;
    MMRESULT mmrv;
    LPWAVEHDR pHdr[SND_BUFFERS];
    void* buf[SND_BUFFERS];
    MSG msg;
    int i;
    
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    wmmSound->hWaveOut = NULL;
    mmrv = waveOutOpen(&wmmSound->hWaveOut, WAVE_MAPPER, &wmmSound->waveFormat, (DWORD_PTR)wmmSound->sndThreadId, (DWORD_PTR)NULL, CALLBACK_THREAD);

    for (i = 0; i < SND_BUFFERS ; i++) {
        buf[i] = calloc(theWmmSound.channels * SND_BUFSIZE * SND_BITSPERSAMPLE / 8, 1);

        pHdr[i] = (LPWAVEHDR)malloc(sizeof(WAVEHDR));
        pHdr[i]->lpData = buf[i];
        pHdr[i]->dwBufferLength = theWmmSound.channels * SND_BUFSIZE * SND_BITSPERSAMPLE / 8;
        pHdr[i]->dwFlags = 0;
        
        mmrv = waveOutPrepareHeader(wmmSound->hWaveOut, pHdr[i], sizeof(WAVEHDR));
        if(mmrv == 0) {
            mmrv = waveOutWrite(wmmSound->hWaveOut, pHdr[i], sizeof(WAVEHDR));
        }
    }

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_USER + 1) {
            break;
        }
        if (msg.message == MM_WOM_DONE) {
            waveOutProc(wmmSound->hWaveOut, MM_WOM_DONE, 0, msg.lParam, msg.wParam);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    mmrv = waveOutReset(wmmSound->hWaveOut);

    Sleep(700);
    for (i = 0; i < SND_BUFFERS ; i++) {
        free(buf[i]);
        waveOutUnprepareHeader(wmmSound->hWaveOut, pHdr[i], sizeof(WAVEHDR));
        free(pHdr[i]);
    }

    mmrv = waveOutClose(wmmSound->hWaveOut);

    ExitThread(0);

    return 0;
}


static Int32 wmmSoundWrite(WmmSound* wmmSound, Int16 *pbuf, UInt32 count)
{
    UInt32 writeIndex = wmmSound->writeIndex;

    while (count--) {
        wmmSound->buffer[writeIndex++] = *pbuf++;
        if (writeIndex >= wmmSound->bufferLength) {
            writeIndex = 0;
        }
    }
    wmmSound->writeIndex = writeIndex;
    return 0;
}

WmmSound* wmmSoundCreate(HWND hwnd, Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels)
{
    WmmSound* wmmSound = &theWmmSound;

    wmmSound->waveFormat.wFormatTag      = WAVE_FORMAT_PCM;
    wmmSound->waveFormat.nChannels       = channels;
    wmmSound->waveFormat.cbSize          = 0;
    wmmSound->waveFormat.wBitsPerSample  = SND_BITSPERSAMPLE;
    wmmSound->waveFormat.nSamplesPerSec  = sampleRate;
    wmmSound->waveFormat.nAvgBytesPerSec = wmmSound->waveFormat.nSamplesPerSec * (wmmSound->waveFormat.wBitsPerSample / 8);
    wmmSound->waveFormat.nBlockAlign     = wmmSound->waveFormat.nChannels      * (wmmSound->waveFormat.wBitsPerSample / 8);

    if (bufferSize < 100) bufferSize = 100;
    if (bufferSize > 350) bufferSize = 350;

    wmmSound->mixer = mixer;
    wmmSound->channels = channels;
    wmmSound->sampleRate = sampleRate;
    wmmSound->bufferLength = bufferSize * sampleRate / 1000 * channels;
    wmmSound->readIndex = 0;
    wmmSound->writeIndex = 0;
    memset(wmmSound->buffer, 0, 2 * channels * bufferSize);

    wmmSound->sndEnabled = 1;

    mixerSetStereo(wmmSound->mixer, channels == 2);
    mixerSetWriteCallback(wmmSound->mixer, wmmSoundWrite, wmmSound, 1024);

    return wmmSound;
}

void wmmSoundDestroy(WmmSound* wmmSound)
{
    mixerSetWriteCallback(wmmSound->mixer, NULL, NULL, 0);
    Sleep(200);
    wmmSoundSuspend(wmmSound);
}

void wmmSoundSuspend(WmmSound* wmmSound) 
{
    if(wmmSound->hThread == NULL) {
        return;
    }

    while (0 == PostThreadMessage(wmmSound->sndThreadId, WM_USER + 1, 0, 0)) {
        Sleep(100);
    }

    WaitForSingleObject(wmmSound->hThread, INFINITE);
    CloseHandle(wmmSound->hThread);
    wmmSound->hThread = NULL;
}

void wmmSoundResume(WmmSound* wmmSound) 
{
    if(wmmSound->hThread != NULL || !wmmSound->sndEnabled) {
        return;
    }

    wmmSound->hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)soundThread,
                                 wmmSound, 0, &wmmSound->sndThreadId);
}


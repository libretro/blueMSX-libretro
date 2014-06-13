/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32wmmSound.c,v $
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


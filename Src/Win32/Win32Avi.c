/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Avi.c,v $
**
** $Revision: 1.11 $
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
#include "Win32Avi.h"
#include <Vfw.h>
#include "Actions.h"
#include "Board.h"
#include "Win32Sound.h"
#include "ArchFile.h"
#include "Emulator.h"
#include "Resource.h"
#include "Language.h"

static PAVIFILE     aviFile;
static PAVISTREAM   aviStream;
static PAVISTREAM   aviSndStream;
static PAVISTREAM   aviVidStream;
static int          frameCount;
static int          sampleCount;
static int          aviStatusOk;
static HWND         hwnd;
static Properties*  properties;
static Video*       video;
static int          syncMethod;
static int          emuSpeed;
static int          rendering;
static int          zoom = 2;


int aviOpen(HWND hwndOwner, char* filename, int fps)
{
    AVICOMPRESSOPTIONS options;
    AVICOMPRESSOPTIONS* optionsPtr;
    AVISTREAMINFO steamHdr;
    AVISTREAMINFO soundHdr;
    WAVEFORMATEX wfex;
    BITMAPINFOHEADER bi;

    AVIFileInit();

    frameCount = 0;
    sampleCount = 0;
    aviStatusOk  = 0;

    if (AVIFileOpen(&aviFile, filename, OF_WRITE | OF_CREATE, NULL) != 0) {
        return 0;
    }

	memset(&bi, 0, sizeof(bi));      
	bi.biSize       = 0x28;    
	bi.biPlanes     = 1;
	bi.biBitCount   = 32;
	bi.biWidth      = 320 * zoom;
	bi.biHeight     = 240 * zoom;
	bi.biSizeImage  = bi.biWidth * bi.biHeight * bi.biBitCount / 8;

    memset(&steamHdr, 0, sizeof(steamHdr));
    steamHdr.fccType = streamtypeVIDEO;
    steamHdr.dwScale = 1;
    steamHdr.dwRate  = fps;
    steamHdr.dwSuggestedBufferSize = bi.biSizeImage;

    if (AVIFileCreateStream(aviFile, &aviStream, &steamHdr) != 0) {
        return 0;
    }
      
    memset(&options, 0, sizeof(options));
    optionsPtr = &options;
    if (!AVISaveOptions(hwndOwner, 0, 1, &aviStream, &optionsPtr)) {
        return 0;
    }
  
    if (AVIMakeCompressedStream(&aviVidStream, aviStream, &options, NULL) != 0) {
        return 0;
    }
  
    // setup the video stream format
    if (AVIStreamSetFormat(aviVidStream, 0, &bi, bi.biSize + bi.biClrUsed * sizeof(RGBQUAD)) != 0) {
        return 0;
    }

    memset(&wfex, 0, sizeof(wfex));
    wfex.wFormatTag      = WAVE_FORMAT_PCM;
    wfex.nChannels       = 2;
    wfex.nSamplesPerSec  = 44100;
    wfex.wBitsPerSample  = 8 * sizeof(Int16);
    wfex.nBlockAlign     = wfex.nChannels * wfex.wBitsPerSample / 8;
    wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
    
    memset(&soundHdr, 0, sizeof(soundHdr));
    soundHdr.fccType         = streamtypeAUDIO;
    soundHdr.dwQuality       = (DWORD)-1;
    soundHdr.dwScale         = wfex.nBlockAlign;
    soundHdr.dwInitialFrames = 0;
    soundHdr.dwRate          = wfex.nAvgBytesPerSec;
    soundHdr.dwSampleSize    = wfex.nBlockAlign;

    if (AVIFileCreateStream(aviFile, &aviSndStream, &soundHdr) != 0) {
        return 0;
    }

    if (AVIStreamSetFormat(aviSndStream, 0, (void *)&wfex, sizeof(wfex)) != 0) {
        return 0;
    }

    aviStatusOk = 1;

    return 1;
}

void aviClose()
{
    if (aviSndStream != NULL) {
        AVIStreamClose(aviSndStream);
        aviSndStream = NULL;
    }

    if (aviVidStream != NULL) {
        AVIStreamClose(aviVidStream);
        aviVidStream = NULL;
    }

    if (aviStream != NULL) {
        AVIStreamClose(aviStream);
        aviStream = NULL;
    }

    if (aviFile != NULL) {
        AVIFileClose(aviFile);
        aviFile = NULL;
    }

    AVIFileExit();
}

void aviAddSound(Int16* buffer, int count)
{
    if (!aviStatusOk) {
        return;
    }

    if (AVIStreamWrite(aviSndStream, sampleCount, count, buffer, count * 4, 0, NULL, NULL) != 0) {
        aviStatusOk = 0;
    }

    sampleCount += count;
}

void aviAddFrame(void* buffer, int length)
{
    if (!aviStatusOk) {
        return;
    }

    if (AVIStreamWrite(aviVidStream, frameCount++, 1, buffer, length, AVIIF_KEYFRAME, NULL, NULL) != 0) {
        aviStatusOk = 0;
    }
}



struct AviSound {
    Mixer* mixer;
};

static Int32 aviSoundWrite(void* dummy, Int16 *buffer, UInt32 count)
{
    // Audio stream is stereo, and callback counts individual samples
    count /= 2; 

    aviAddSound(buffer, count);

    return 0;
}

AviSound* aviSoundCreate(HWND hwnd, Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels)
{
    AviSound* aviSound = (AviSound*)calloc(1, sizeof(AviSound));

    aviSound->mixer = mixer;

    // Force stereo
    mixerSetStereo(mixer, 1);
    mixerSetWriteCallback(mixer, aviSoundWrite, NULL, 128);

    return aviSound;
}

void aviSoundDestroy(AviSound* aviSound)
{
    mixerSetWriteCallback(aviSound->mixer, NULL, NULL, 0);
    free(aviSound);
}

void aviSoundSuspend(AviSound* aviSound)
{
}

void aviSoundResume(AviSound* aviSound)
{
}


#define stretchLine(TYPE, dst, dstLen, src, srcLen, M, N, R) do {           \
    TYPE*  d = (TYPE*)dst;                                                  \
    TYPE*  s = (TYPE*)src;                                                  \
    UInt32 a = *s++;                                                        \
    UInt32 b = *s++;                                                        \
    int    n = 0;                                                           \
    int    w;                                                               \
                                                                            \
    for (w = 0; w < dstLen; w++) {                                          \
        int q = R * n / dstLen;                                             \
        int p = R - q;                                                      \
        d[w] = (TYPE)(((((a&M)*p+(b&M)*q)/R)&M)|((((a&N)*p+(b&N)*q)/R)&N)); \
        n += srcLen;                                                        \
        if (n >= dstLen) {                                                  \
            n -= dstLen;                                                    \
            a = b;                                                          \
            b = *s++;                                                       \
        }                                                                   \
    }                                                                       \
} while (0)

void stretchImage(char* dst, int dstWidth, char* src, int srcWidth, int pitch, int height, int bitDepth)
{
    // Assume destination is wider than source
    int h;
    for (h = 0; h < height; h++) {
        if (bitDepth == 16) stretchLine(UInt16, dst, dstWidth, src, srcWidth, 0xf81f, 0x07e0, 32);
        else                stretchLine(UInt32, dst, dstWidth, src, srcWidth, 0x00ff00ff, 0x0000ff00, 256);
        dst += pitch;
        src += pitch;
    }
}

void aviVideoCallback(void* dummy, UInt32 time) 
{
    static char displayData[4 * 640 * (480 + 1)];

    FrameBuffer* frameBuffer;
    int bitDepth = 32;
    int bytesPerPixel = bitDepth / 8;
    char* dpyData = displayData;
    int width  = 320 * zoom;
    int height = 240 * zoom;
    int borderWidth;
    int displayPitch = width * bitDepth / 8;

    frameBuffer = frameBufferFlipViewFrame(1);
    if (frameBuffer == NULL) {
        frameBuffer = frameBufferGetWhiteNoiseFrame();
    }

    borderWidth = (320 - frameBuffer->maxWidth) * zoom / 2;

    if (properties->video.horizontalStretch) {
        if (borderWidth > 0) {
            // Render image one line higher, resizing will move it back to correct location
            videoRender(video, frameBuffer, bitDepth, zoom, 
                        dpyData + (height - 1 + 1) * displayPitch, 0, -1 * displayPitch, -1);
            stretchImage(dpyData, width, dpyData + displayPitch, width - 2 * borderWidth, displayPitch, height, bitDepth);
            borderWidth = 0;
        }
        else {
            videoRender(video, frameBuffer, bitDepth, zoom, 
                        dpyData + (height - 1) * displayPitch, 0, -1 * displayPitch, -1);
        }
    }
    else {
        videoRender(video, frameBuffer, bitDepth, zoom, 
                    dpyData + (height - 1) * displayPitch + borderWidth * bytesPerPixel, 0, -1 * displayPitch, -1);

        if (borderWidth > 0) {
            int h = height;
            while (h--) {
                memset(dpyData, 0, borderWidth * bytesPerPixel);
                memset(dpyData + (width - borderWidth) * bytesPerPixel, 0, borderWidth * bytesPerPixel);
                dpyData += displayPitch;
            }
        }
    }

    aviAddFrame(displayData, width * height * 4);
}


static void replaceCharInString(char* str, char oldChar, char newChar) 
{
    while (*str) {
        if (*str == oldChar) {
            *str = newChar;
        }
        str++;
    }
}

char* aviGetFilename(Properties* properties)
{
    char* title = langDlgSaveVideoClipAs();
    char  extensionList[512];
    char defaultDir[512] = "";
    char* extensions = ".avi\0";
    char* filename;
    int selectedExtension = 0;
    FILE* file;

    sprintf(extensionList, "%s   (*.avi)#*.avi#", langFileAvi());
    replaceCharInString(extensionList, '#', 0);

    filename = archFileSave(title, extensionList, defaultDir, extensions, &selectedExtension, ".avi");

    file = fopen(filename, "r");
    if (file != NULL) {
        char langBuffer[200];
        fclose(file);
        sprintf(langBuffer, "%s %s", langWarningOverwriteFile(), filename);
        if (IDOK != MessageBox(NULL, langBuffer, langWarningTitle(), MB_OKCANCEL)) {
            return NULL;
        }

        file = fopen(filename, "wb");
        fclose(file);
    }

    return filename;
}

static char* progressText()
{
    static char text[128];
    int amount = boardCaptureCompleteAmount();

    sprintf(text, "%s %d.%d%%", langDlgAmountCompleted(), amount / 10, amount % 10);

    return text;
}

static BOOL CALLBACK statusDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (iMsg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, TRUE);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        return TRUE;

    case WM_TIMER:
        SendDlgItemMessage(hDlg, IDC_VIDEOPROGRESSTEXT, WM_SETTEXT, 0, (LPARAM)progressText());
        if (!boardCaptureIsPlaying()) {
            EndDialog(hDlg, TRUE);
        }
        return FALSE;

    case WM_DESTROY:
        KillTimer(hwnd, 2);
        return 0;

    case WM_INITDIALOG:
        SetWindowText(hDlg, langDlgRenderVideoCapture());
        SendDlgItemMessage(hDlg, IDC_VIDEOPROGRESSTEXT, WM_SETTEXT, 0, (LPARAM)progressText());
        SetTimer(hDlg, 2, 250, NULL);
        return FALSE;
    }

    return FALSE;
}



void aviStartRender(HWND hwndOwner, Properties* prop, Video* vid)
{
    char* filename;

    hwnd       = hwndOwner;
    properties = prop;
    video      = vid;
    syncMethod = properties->emulation.syncMethod;
    emuSpeed   = properties->emulation.speed;

    zoom = properties->video.captureSize == 0 ? 1 : 2;

    actionEmuStop();

    filename = aviGetFilename(properties);
    if (filename == NULL) {
        return;
    }

    boardSetPeriodicCallback(aviVideoCallback, NULL, properties->video.captureFps);
    properties->emulation.syncMethod = P_EMU_SYNCIGNORE;
    mixerSetBoardFrequencyFixed(3579545);
    actionEmuSpeedSet(100);
    frameBufferSetFrameCount(4);

    soundDriverConfig(hwnd, SOUND_DRV_AVI);
    emulatorRestartSound();

    aviOpen(hwnd, filename, properties->video.captureFps);

    actionVideoCapturePlay();

    rendering = 1;

    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_RENDERVIDEO), hwnd, statusDlgProc);

    actionEmuStop();
    aviStopRender();
}

void aviStopRender()
{
    if (!rendering) {
        return;
    }
    rendering = 0;

    aviClose();

    // Restore sound driver
    soundDriverConfig(hwnd, properties->sound.driver);
    emulatorRestartSound();

    // Restore sync method
    properties->emulation.syncMethod = syncMethod;
    switch(properties->emulation.syncMethod) {
    case P_EMU_SYNCNONE:
        frameBufferSetFrameCount(1);
        break;
    case P_EMU_SYNCTOVBLANK:
    case P_EMU_SYNCTOVBLANKASYNC:
        frameBufferSetFrameCount(4);
        break;
    default:
        frameBufferSetFrameCount(3);
    }

    // Restore emu speed
    mixerSetBoardFrequencyFixed(0);
    actionEmuSpeedSet(emuSpeed);

    // Remove board timer
    boardSetPeriodicCallback(NULL, NULL, 0);
}

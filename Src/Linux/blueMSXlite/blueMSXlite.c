/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Linux/blueMSXlite/blueMSXlite.c,v $
**
** $Revision: 1.14 $
**
** $Date: 2008-03-31 19:42:21 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Tomas Karlsson
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
#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "CommandLine.h"
#include "Properties.h"
#include "ArchFile.h"
#include "VideoRender.h"
#include "AudioMixer.h"
#include "Casette.h"
#include "PrinterIO.h"
#include "UartIO.h"
#include "MidiIO.h"
#include "Machine.h"
#include "Board.h"
#include "ArchEvent.h"

static Properties* properties;
static Video* video;
static Mixer* mixer;

static Window window;
static Display* display; 
static Screen* screen;
static XImage* ximage;
static int bitDepth;
static int dpyUpdateEvent = 0;
static void* dpyUpdateAckEvent = NULL;

#define WIDTH  640
#define HEIGHT 480

int createX11Window(const char *title, int width, int height, int bitDepth)
{
  XSetWindowAttributes windowAttributes;
  XSizeHints sizeHints;
  XWMHints xvmHints;
  XVisualInfo visualInfo;
  int depth;
  int i;
  int j;

  display = XOpenDisplay(NULL);
  if (!display) {
      return 0;
  }
  screen = DefaultScreenOfDisplay(display);

  windowAttributes.event_mask       = FocusChangeMask|KeyPressMask|KeyReleaseMask|StructureNotifyMask;
  windowAttributes.background_pixel = BlackPixelOfScreen(screen);
  windowAttributes.backing_store    = Always;

  window = XCreateWindow(display, RootWindowOfScreen(screen), 0, 0, width, height, 1,
                         CopyFromParent, CopyFromParent, CopyFromParent,
                         CWBackPixel|CWEventMask|CWBackingStore, &windowAttributes);
  if (!window) {
      return 0;
  }

  sizeHints.flags       = PSize|PMinSize|PMaxSize|PResizeInc;
  sizeHints.min_width   = width;
  sizeHints.max_width   = width;
  sizeHints.base_width  = width;
  sizeHints.width_inc   = sizeof(int);
  sizeHints.min_height  = height;
  sizeHints.max_height  = height;
  sizeHints.base_height = height;
  sizeHints.height_inc  = sizeof(int);
  xvmHints.input        = True;
  xvmHints.flags        = InputHint;

  XSetWMHints(display, window, &xvmHints);
  XSetWMNormalHints(display, window, &sizeHints);
  XStoreName(display, window, title);

  XMapRaised(display, window);
  XClearWindow(display, window);

  
  depth = DefaultDepthOfScreen(screen);
  i = XScreenNumberOfScreen(screen);
  for (j = 7; j >= 0 && !XMatchVisualInfo(display, i, depth, j, &visualInfo); j--);
  if (j < 0) {
      return 0;
  }

    ximage = XCreateImage(display, visualInfo.visual, depth, ZPixmap, 
                          0, NULL, width, height, bitDepth, 0);
    if (!ximage) {
        return 0;
    }

    ximage->data = (char*)malloc(width * height * bitDepth / 8);
  
  return 1;
}

int  archUpdateEmuDisplay(int syncMode) 
{
#ifdef LINUX_TEST
    updateEmuDisplay();
    XSync(display, 0);
    return;
#endif
    dpyUpdateEvent = 1;
    if (properties->emulation.syncMethod == P_EMU_SYNCFRAMES) {
        archEventWait(dpyUpdateAckEvent, 500);
    }
    return 1;
}

int updateEmuDisplay() 
{
    FrameBuffer* frameBuffer;
    int bytesPerPixel = bitDepth / 8;
    char* dpyData  = ximage->data;  
    int borderWidth;
    int dstOffset;

    frameBuffer = frameBufferFlipViewFrame(0);
    if (frameBuffer == NULL) {
        frameBuffer = frameBufferGetWhiteNoiseFrame();
    }

    borderWidth = 320 - frameBuffer->maxWidth;

    videoRender(video, frameBuffer, bitDepth, 2, 
                dpyData + borderWidth * bytesPerPixel, 
                0, WIDTH * bytesPerPixel, -1);

    if (borderWidth > 0) {
        int h = HEIGHT;
        while (h--) {
            memset(dpyData, 0, borderWidth * bytesPerPixel);
            memset(dpyData + (WIDTH - borderWidth) * bytesPerPixel, 0, borderWidth * bytesPerPixel);
            dpyData += WIDTH * bytesPerPixel;
        }
    }

    XPutImage(display, window, DefaultGCOfScreen(screen), ximage, 0, 0, 0, 0, WIDTH,HEIGHT);

    return 0; 
}

void setDefaultPaths(const char* rootDir)
{   
    char buffer[512];  

    propertiesSetDirectory(rootDir, rootDir);

    sprintf(buffer, "%s/Audio Capture", rootDir);
    mkdir(buffer);
    actionSetAudioCaptureSetDirectory(buffer, "");

    sprintf(buffer, "%s/QuickSave", rootDir);
    mkdir(buffer);
    actionSetQuickSaveSetDirectory(buffer, "");

    sprintf(buffer, "%s/SRAM", rootDir);
    mkdir(buffer);
    boardSetDirectory(buffer);

    sprintf(buffer, "%s/Casinfo", rootDir);
    mkdir(buffer);
    tapeSetDirectory(buffer, "");

    sprintf(buffer, "%s/Databases", rootDir);
    mkdir(buffer);
    mediaDbLoad(buffer);
}


int main(int argc, char **argv)
{
    char szLine[8192] = "";
    int resetProperties;
    char path[512] = "";
    int i;

    for (i = 1; i < argc; i++) {
        strcat(szLine, argv[i]);
        strcat(szLine, " ");
    }

    setDefaultPaths(archGetCurrentDirectory());

    resetProperties = emuCheckResetArgument(szLine);
    strcat(path, archGetCurrentDirectory());
    strcat(path, DIR_SEPARATOR "bluemsx.ini");
    properties = propCreate(resetProperties, 0, P_KBD_EUROPEAN, 0, "");
    
    properties->emulation.syncMethod = P_EMU_SYNCTOVBLANKASYNC;

    if (resetProperties == 2) {
        propDestroy(properties);
        return 0;
    }
    
    bitDepth = 32;
    if (!createX11Window("blueMSXlite", WIDTH, HEIGHT, bitDepth)) {
        return 0;
    }
    
    dpyUpdateAckEvent = archEventCreate(0);

    video = videoCreate();
    videoSetColors(video, properties->video.saturation, properties->video.brightness, 
                  properties->video.contrast, properties->video.gamma);
    videoSetScanLines(video, properties->video.scanlinesEnable, properties->video.scanlinesPct);
    videoSetColorSaturation(video, properties->video.colorSaturationEnable, properties->video.colorSaturationWidth);
    
    mixer = mixerCreate();
    
    emulatorInit(properties, mixer);
    actionInit(video, properties, mixer);
    langInit();
    tapeSetReadOnly(properties->cassette.readOnly);
    
    langSetLanguage(properties->language);
    
    joystickPortSetType(0, properties->joy1.type);
    joystickPortSetType(1, properties->joy2.type);

    printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
    printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
    uartIoSetType(properties->ports.Com.type, properties->ports.Com.fileName);
    midiIoSetMidiOutType(properties->sound.MidiOut.type, properties->sound.MidiOut.fileName);
    midiIoSetMidiInType(properties->sound.MidiIn.type, properties->sound.MidiIn.fileName);
    ykIoSetMidiInType(properties->sound.YkIn.type, properties->sound.YkIn.fileName);

    emulatorRestartSound();

    for (i = 0; i < MIXER_CHANNEL_TYPE_COUNT; i++) {
        mixerSetChannelTypeVolume(mixer, i, properties->sound.mixerChannel[i].volume);
        mixerSetChannelTypePan(mixer, i, properties->sound.mixerChannel[i].pan);
        mixerEnableChannelType(mixer, i, properties->sound.mixerChannel[i].enable);
    }
    
    mixerSetMasterVolume(mixer, properties->sound.masterVolume);
    mixerEnableMaster(mixer, properties->sound.masterEnable);

    videoUpdateAll(video, properties);

    mediaDbSetDefaultRomType(properties->cartridge.defaultType);

    for (i = 0; i < PROP_MAX_CARTS; i++) {
        if (properties->media.carts[i].fileName[0]) insertCartridge(properties, i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip, properties->media.carts[i].type, -1);
        updateExtendedRomName(i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_DISKS; i++) {
        if (properties->media.disks[i].fileName[0]) insertDiskette(properties, i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip, -1);
        updateExtendedDiskName(i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip);
    }

    for (i = 0; i < PROP_MAX_TAPES; i++) {
        if (properties->media.tapes[i].fileName[0]) insertCassette(properties, i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip, 0);
        updateExtendedCasName(i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip);
    }

    {
        Machine* machine = machineCreate(properties->emulation.machineName);
        if (machine != NULL) {
            boardSetMachine(machine);
            machineDestroy(machine);
        }
    }
    boardSetFdcTimingEnable(properties->emulation.enableFdcTiming);
    boardSetY8950Enable(properties->sound.chip.enableY8950);
    boardSetYm2413Enable(properties->sound.chip.enableYM2413);
    boardSetMoonsoundEnable(properties->sound.chip.enableMoonsound);
    boardSetVideoAutodetect(properties->video.chipAutodetect);

    XSync(display, 0);
    
    i = emuTryStartWithArguments(properties, szLine, NULL);
    if (i < 0) {
        printf("Failed to parse command line\n");
        return 0;
    }
    
    if (i == 0) {
        emulatorStart(NULL);
    }

    for (i = 0; i < 50000; i++) {
        if (dpyUpdateEvent) {
            updateEmuDisplay();
            XSync(display, 0);
            dpyUpdateEvent = 0;
            archEventSet(dpyUpdateAckEvent);
        }
        archThreadSleep(10);
    }

    
    videoDestroy(video);
    propDestroy(properties);
    archSoundDestroy();
    mixerDestroy(mixer);

    return 0;
}

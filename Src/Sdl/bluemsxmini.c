/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Sdl/bluemsxmini.c,v $
**
** $Revision: 1.4 $
**
** $Date: 2008-03-30 18:38:45 $
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

// The bluemsxmini is an optimized version of bluemsxlite. It supports
// only 272x240 pixel windows, and it bypasses the frame buffers and
// video renderers.
// The goal is to get a faster renderer that can be used for ports to low
// spec systems.
//
// To compile this version instead of the regular bluemsxlite,
// remove the following files from the makefile:
//      - bluemsxlite.c
//      - FrameBuffer.c
//      - hq2x.c
//      - hq3x.c
//      - Scalebit.c
//      - VideoRender.c
//
// Also add the following preprocessor defines to the makefile:
//      - NO_FRAMEBUFFER
//      - MAX_VIDEO_WIDTH_320
//
// The important frame buffer stuff is at the end of the file.
//
// NOTE. All color manipulation is bypassed. The emulator renders
// 555 RGB into the surface buffer. This can of course be changed
// as needed inside the emulator.

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#include "CommandLine.h"
#include "Properties.h"
#include "ArchFile.h"
#include "AudioMixer.h"
#include "Casette.h"
#include "PrinterIO.h"
#include "UartIO.h"
#include "MidiIO.h"
#include "Machine.h"
#include "Board.h"
#include "Emulator.h"
#include "FileHistory.h"
#include "Actions.h"
#include "Language.h"
#include "LaunchFile.h"
#include "ArchEvent.h"
#include "ArchSound.h"
#include "ArchNotifications.h"
#include "JoystickPort.h"
#include "SdlShortcuts.h"
#include "SdlMouse.h"

#ifdef videoGetColor
#undef videoGetColor
#endif

void keyboardSetDirectory(char* directory);
void keyboardInit();
void keyboardSetFocus(int handle, int focus);
void keyboardUpdate();
int keyboardGetModifiers();
static void handleEvent(SDL_Event* event);

static Properties* properties;
static Mixer* mixer;

static Properties* properties;
static Mixer* mixer;
static Shortcuts* shortcuts;
static int doQuit = 0;

static int pendingDisplayEvents = 0;
static void* dpyUpdateAckEvent = NULL;

static SDL_Surface *surface;
static int   bitDepth;
static char* displayData;
static int   displayPitch = 0;

#define WIDTH  272
#define HEIGHT 240

#define EVENT_UPDATE_DISPLAY 2
#define EVENT_UPDATE_WINDOW  3

void archTrap(UInt8 value)
{
}

void createSdlSurface(int width, int height)
{
    int flags = SDL_SWSURFACE;
    int bytepp;

	// try default bpp
	surface = SDL_SetVideoMode(width, height, 0, flags);
	bytepp = (surface ? surface->format->BytesPerPixel : 0);
	if (bytepp != 2 && bytepp != 4) {
		surface = NULL;
	}

    bitDepth = bytepp * 8;

    if (surface != NULL) {
        displayData = (char*)surface->pixels;
        displayPitch = surface->pitch;
    }
}


int createSdlWindow()
{
    const char *title = "blueMSXlite";
    int fullscreen = properties->video.windowSize == P_VIDEO_SIZEFULLSCREEN;

    bitDepth = 32;

    surface = NULL;

    if (surface == NULL) {
        createSdlSurface(WIDTH, HEIGHT);
    }

    // Set the window caption
    SDL_WM_SetCaption( title, NULL );

    // Update mouse capture region
    sdlMouseSetCaptureRect(0, 0, WIDTH, HEIGHT);
    
    return 1;
}

static int updateEmuDisplay(int updateAll) 
{
    if (SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) < 0) {
        return 0;
    }
    SDL_UpdateRect(surface, 0, 0, WIDTH, HEIGHT);
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    return 0; 
}

int  archUpdateEmuDisplay(int syncMode) 
{
    SDL_Event event;

    if (pendingDisplayEvents > 1) {
        return 1;
    }

    pendingDisplayEvents++;

    event.type = SDL_USEREVENT;
    event.user.code = EVENT_UPDATE_DISPLAY;
    event.user.data1 = NULL;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);

#ifndef SINGLE_THREADED
    if (properties->emulation.syncMethod == P_EMU_SYNCFRAMES) {
        archEventWait(dpyUpdateAckEvent, 500);
    }
#endif
    return 1;
}

void archUpdateWindow() 
{
    SDL_Event event;

    event.type = SDL_USEREVENT;
    event.user.code = EVENT_UPDATE_WINDOW;
    event.user.data1 = NULL;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
}

#ifdef SINGLE_THREADED
int archPollEvent()
{
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if( event.type == SDL_QUIT ) {
            doQuit = 1;
        }
        else {
            handleEvent(&event);
        }
    }
    return doQuit;
}
#endif

void archEmulationStartNotification() 
{
}

void archEmulationStopNotification() 
{
#ifdef RUN_EMU_ONCE_ONLY
    doQuit = 1;
#endif
}

void archEmulationStartFailure() 
{
#ifdef RUN_EMU_ONCE_ONLY
    doQuit = 1;
#endif
}


void setDefaultPaths(const char* rootDir)
{   
    char buffer[512];  

    propertiesSetDirectory(rootDir, rootDir);

    sprintf(buffer, "%s/Audio Capture", rootDir);
    archCreateDirectory(buffer);
    actionSetAudioCaptureSetDirectory(buffer, "");

    sprintf(buffer, "%s/Video Capture", rootDir);
    archCreateDirectory(buffer);
    actionSetAudioCaptureSetDirectory(buffer, "");

    sprintf(buffer, "%s/QuickSave", rootDir);
    archCreateDirectory(buffer);
    actionSetQuickSaveSetDirectory(buffer, "");

    sprintf(buffer, "%s/SRAM", rootDir);
    archCreateDirectory(buffer);
    boardSetDirectory(buffer);

    sprintf(buffer, "%s/Casinfo", rootDir);
    archCreateDirectory(buffer);
    tapeSetDirectory(buffer, "");

    sprintf(buffer, "%s/Databases", rootDir);
    archCreateDirectory(buffer);
    mediaDbLoad(buffer);
    
    sprintf(buffer, "%s/Keyboard Config", rootDir);
    archCreateDirectory(buffer);
    keyboardSetDirectory(buffer);

    sprintf(buffer, "%s/Shortcut Profiles", rootDir);
    archCreateDirectory(buffer);
    shortcutsSetDirectory(buffer);
}

static void handleEvent(SDL_Event* event) 
{
    switch (event->type) {
    case SDL_USEREVENT:
        switch (event->user.code) {
        case EVENT_UPDATE_DISPLAY:
            updateEmuDisplay(0);
            archEventSet(dpyUpdateAckEvent);
            pendingDisplayEvents--;
            break;
        case EVENT_UPDATE_WINDOW:
            if (!createSdlWindow()) {
                exit(0);
            }
            break;
        }
        break;
    case SDL_ACTIVEEVENT:
        if (event->active.state & SDL_APPINPUTFOCUS) {
            keyboardSetFocus(1, event->active.gain);
        }
        if (event->active.state == SDL_APPMOUSEFOCUS) {
            sdlMouseSetFocus(event->active.gain);
        }
        break;
    case SDL_KEYDOWN:
        shortcutCheckDown(shortcuts, HOTKEY_TYPE_KEYBOARD, keyboardGetModifiers(), event->key.keysym.sym);
        break;
    case SDL_KEYUP:
        shortcutCheckUp(shortcuts, HOTKEY_TYPE_KEYBOARD, keyboardGetModifiers(), event->key.keysym.sym);
        break;
    case SDL_VIDEOEXPOSE:
        updateEmuDisplay(1);
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        sdlMouseButton(event->button.button, event->button.state);
        break;
    case SDL_MOUSEMOTION:
        sdlMouseMove(event->motion.x, event->motion.y);
        break;
    }
}

int main(int argc, char **argv)
{
    SDL_Event event;
    char szLine[8192] = "";
    int resetProperties;
    char path[512] = "";
    int i;

    SDL_Init( SDL_INIT_EVERYTHING );

    for (i = 1; i < argc; i++) {
        if (strchr(argv[i], ' ') != NULL && argv[i][0] != '\"') {
            strcat(szLine, "\"");
            strcat(szLine, argv[i]);
            strcat(szLine, "\"");
        }
        else {
            strcat(szLine, argv[i]);
        }
        strcat(szLine, " ");
    }

    setDefaultPaths(archGetCurrentDirectory());

    resetProperties = emuCheckResetArgument(szLine);
    strcat(path, archGetCurrentDirectory());
    strcat(path, DIR_SEPARATOR "bluemsx.ini");
    properties = propCreate(resetProperties, 0, P_KBD_EUROPEAN, 0, "");
    
    properties->emulation.syncMethod = P_EMU_SYNCFRAMES;    
//    properties->emulation.syncMethod = P_EMU_SYNCTOVBLANKASYNC;

    if (resetProperties == 2) {
        propDestroy(properties);
        return 0;
    }
    
    bitDepth = 32;
    if (!createSdlWindow()) {
        return 0;
    }
    
    dpyUpdateAckEvent = archEventCreate(0);

    keyboardInit();

    mixer = mixerCreate();
    
    emulatorInit(properties, mixer);
    actionInit(NULL, properties, mixer);
    langInit();
    tapeSetReadOnly(properties->cassette.readOnly);
    
    langSetLanguage(properties->language);
    
    joystickPortSetType(0, properties->joy1.typeId);
    joystickPortSetType(1, properties->joy2.typeId);

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

    shortcuts = shortcutsCreate();

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
    boardSetVideoAutodetect(properties->video.detectActiveMonitor);

    i = emuTryStartWithArguments(properties, szLine, NULL);
    if (i < 0) {
        printf("Failed to parse command line\n");
        return 0;
    }
    
    if (i == 0) {
        emulatorStart(NULL);
    }

    //While the user hasn't quit
    while(!doQuit) {
        SDL_WaitEvent(&event);
        do {
            if( event.type == SDL_QUIT ) {
                doQuit = 1;
            }
            else {
                handleEvent(&event);
            }
        } while(SDL_PollEvent(&event));
    }

	// For stop threads before destroy.
	// Clean up.
	if (SDL_WasInit(SDL_INIT_EVERYTHING)) {
		SDL_Quit(); 
	}

    propDestroy(properties);
    archSoundDestroy();
    mixerDestroy(mixer);
    
    return 0;
}

#if PIXEL_WIDTH==32
Pixel videoGetColor(int R, int G, int B)
{
    return (R << 16) | (G << 8) | B;
}
#else
Pixel videoGetColor(int R, int G, int B)
{
    return ((R >> 3) << 10) | ((G >> 3) << 5) | (B >> 3);
}
#endif

#define NOT_NULL ((void*)1)

// No back buffers implemented, so these methods just need to return something not NULL
// Back buffers could make use of these methods which are called when the MSX switches
// frames
FrameBuffer* frameBufferGetDrawFrame() {return NOT_NULL; } 
FrameBuffer* frameBufferFlipDrawFrame() { return NOT_NULL; }

// This is the important method and it should return a pointer to scan line y (0-239)
Pixel* frameBufferGetLine(FrameBuffer* frameBuffer, int y) { 
    return (Pixel*)(displayData + y * WIDTH * bitDepth / 8); 
}

// Used by gunstick and asciilaser
static int fbScanLine = 0;
void frameBufferSetScanline(int scanline) { fbScanLine = scanline; }
int frameBufferGetScanline() { return fbScanLine; }


////// The following methods are not needed for a fast renderer

// Only one video device supported
FrameBufferData* frameBufferDataCreate(int maxWidth, int maxHeight, int defaultHorizZoom) { return NOT_NULL; }
FrameBufferData* frameBufferGetActive() { return NOT_NULL; }

void frameBufferDataDestroy(FrameBufferData* frameData) {}

void frameBufferSetActive(FrameBufferData* frameData) {}
void frameBufferSetMixMode(FrameBufferMixMode mode, FrameBufferMixMode mask) {}

void   frameBufferSetLineCount(FrameBuffer* frameBuffer, int val) {} // Assume line count is always 240
int    frameBufferGetLineCount(FrameBuffer* frameBuffer) { return HEIGHT; }
int    frameBufferGetMaxWidth(FrameBuffer* frameBuffer) { return WIDTH; }

// Double width is not supported
int    frameBufferGetDoubleWidth(FrameBuffer* frameBuffer, int y) { return 0; }
void   frameBufferSetDoubleWidth(FrameBuffer* frameBuffer, int y, int val) {}

// De interlace not supported
void frameBufferClearDeinterlace() {} 
void   frameBufferSetInterlace(FrameBuffer* frameBuffer, int val) {}

// Another method not needed
void videoUpdateAll(Video* video, Properties* properties) {}

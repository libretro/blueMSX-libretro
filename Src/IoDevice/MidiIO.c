/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/MidiIO.c,v $
**
** $Revision: 1.7 $
**
** $Date: 2006/06/11 07:53:24 $
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
#include <string.h>
#include "MidiIO.h"
#include "ArchUart.h"
#include "DAC.h"
#include "Board.h"
#include "ArchMidi.h"

typedef struct MidiIO {
    MidiType inType;
    FILE* inFile;
    ArchMidi* inHost;
    MidiType outType;
    FILE* outFile;
    ArchMidi* outHost;
    MidiIOCb cb;
    void* ref;
};

static MidiType theMidiInType = MIDI_NONE;
static MidiType theMidiOutType = MIDI_NONE;
static char theInFileName[512] = { 0 };
static char theOutFileName[512] = { 0 };

static MidiIO* theMidiIO = NULL;

static MidiType theYkInType = MIDI_NONE;
static char theYkInFileName[512] = { 0 };
MidiIO* theYkIO = NULL;

static void setOutType(int device, MidiIO* midiIo)
{
    switch (midiIo->outType) {
    case MIDI_HOST:
        midiIo->outHost = archMidiOutCreate(device);
        break;
    case MIDI_FILE:
        midiIo->outFile = fopen(theOutFileName, "w+");
        break;
    }
}

static void midiInCb(MidiIO* midiIO, UInt8* buffer, UInt32 length)
{
    if (midiIO->cb != NULL) {
        midiIO->cb(midiIO->ref, buffer, length);
    }
}

static void setInType(int device, MidiIO* midiIo)
{
    switch (midiIo->inType) {
    case MIDI_HOST:
        midiIo->inHost = archMidiInCreate(device, midiInCb, midiIo);
        break;
    case MIDI_FILE:
        midiIo->inFile = fopen(theInFileName, "w+");
        break;
    }
}

static void removeOutType(MidiIO* midiIo)
{
    switch (midiIo->outType) {
    case MIDI_HOST:
        if (midiIo->outHost != 0) {
            archMidiOutDestroy(midiIo->outHost);
        }
        midiIo->outHost = NULL;
        break;
    case MIDI_FILE:
        fclose(midiIo->outFile);
        break;
    }
}

static void removeInType(MidiIO* midiIo)
{
    switch (midiIo->inType) {
    case MIDI_HOST:
        if (midiIo->inHost != 0) {
            archMidiInDestroy(midiIo->inHost);
        }
        midiIo->inHost = 0;
        break;
    case MIDI_FILE:
        fclose(midiIo->inFile);
        break;
    }
}

void midiIoTransmit(MidiIO* midiIo, UInt8 value)
{
    switch (midiIo->outType) {
    case MIDI_HOST:
        if (midiIo->outHost) {
            archMidiOutTransmit(midiIo->outHost, value);
        }
        break;
    case MIDI_FILE:
        fwrite(&value, 1, 1, midiIo->outFile);
        break;
    }
}

MidiIO* midiIoCreate(MidiIOCb cb, void* ref)
{
    MidiIO* midiIo = calloc(1, sizeof(MidiIO));

    midiIo->cb = cb;
    midiIo->ref = ref;
    midiIo->outType = theMidiOutType;
    midiIo->inType = theMidiInType;
    setOutType(0, midiIo);
    setInType(0, midiIo);

    theMidiIO = midiIo;

    return midiIo;
}

void midiIoDestroy(MidiIO* midiIo)
{
    removeInType(midiIo);
    removeOutType(midiIo);

    free(midiIo);

    theMidiIO = NULL;
}

void midiIoSetMidiOutType(MidiType type, const char* fileName)
{   
    theMidiOutType = type;

    strcpy(theOutFileName, fileName);
    
    if (theMidiIO == NULL) {
        return;
    }

    removeOutType(theMidiIO);
    theMidiIO->outType = theMidiOutType;
    setOutType(0, theMidiIO);
}

void midiIoSetMidiInType(MidiType type, const char* fileName)
{
    theMidiInType = type;

    strcpy(theInFileName, fileName);
    
    if (theMidiIO == NULL) {
        return;
    }

    removeInType(theMidiIO);
    theMidiIO->inType = theMidiInType;
    setInType(0, theMidiIO);
}




MidiIO* ykIoCreate()
{
    MidiIO* ykIo = calloc(1, sizeof(MidiIO));

    ykIo->inType = theYkInType;

    setInType(1, ykIo);

    theYkIO = ykIo;

    return ykIo;
}

void ykIoDestroy(MidiIO* ykIo)
{
    removeInType(ykIo);

    free(ykIo);

    theYkIO = NULL;
}

void ykIoSetMidiInType(MidiType type, const char* fileName)
{
    theYkInType = type;

    strcpy(theYkInFileName, fileName);
    
    if (theYkIO == NULL) {
        return;
    }

    removeInType(theYkIO);
    theYkIO->inType = theYkInType;
    setInType(1, theYkIO);
}

int ykIoGetKeyState(MidiIO* midiIo, int key)
{
    switch (midiIo->inType) {
    case MIDI_HOST:
        if (midiIo->inHost) {
            return archMidiInGetNoteOn(midiIo->inHost, key);
        }
        break;
    }
    return 0;
}
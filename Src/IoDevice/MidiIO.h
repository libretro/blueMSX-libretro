/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/MidiIO.h,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/06/11 07:53:24 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vikl, Tomas Karlsson
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
#ifndef MIDI_IO_H
#define MIDI_IO_H

#include "MsxTypes.h"

typedef struct MidiIO MidiIO;

typedef enum { MIDI_NONE, MIDI_FILE, MIDI_HOST } MidiType;

typedef void (*MidiIOCb)(void*, UInt8*, UInt32);

MidiIO* midiIoCreate(MidiIOCb cb, void* ref);
void midiIoDestroy(MidiIO* midiIo);

void midiIoTransmit(MidiIO* midiIo, UInt8 value);

void midiIoSetMidiInType(MidiType type, const char* fileName);
void midiIoSetMidiOutType(MidiType type, const char* fileName);


MidiIO* ykIoCreate();
void ykIoDestroy(MidiIO* ykIo);

int ykIoGetKeyState(MidiIO* midiIo, int key);

void ykIoSetMidiInType(MidiType type, const char* fileName);


#endif

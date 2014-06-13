/*
 *	Win32 MIDI utility routins for openMSX.
 *
 * Copyright (c) 2003 Reikan.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

// maintain in openMSX: $Id:


#ifndef MIDI_W32
#define MIDI_W32

typedef void (*MidiInCb)(void*, const char*, unsigned);

int w32_midiOutInit();
int w32_midiOutClean();
unsigned w32_midiOutGetVFNsNum();
const char* w32_midiOutGetVFN(unsigned nmb);
const char* w32_midiOutGetRDN(unsigned nmb);
unsigned w32_midiOutOpen(const char* vfn);
int w32_midiOutClose(unsigned idx);
int w32_midiOutPut(unsigned char value, unsigned idx);
unsigned int w32_midiOutGetVolume(unsigned idx);
void w32_midiOutSetVolume(unsigned idx, unsigned int volume);

int w32_midiOutNoteOn(unsigned idx);

void w32_resetHistory();
void w32_midiOutLoadState(unsigned idx);
void w32_midiOutSaveState(unsigned idx);

void w32_midiOutEnableMt32ToGmMapping(unsigned idx, int enable);


int w32_midiInInit();
int w32_midiInClean();
unsigned w32_midiInGetVFNsNum();
const char* w32_midiInGetVFN(unsigned nmb);
const char* w32_midiInGetRDN(unsigned nmb);
unsigned w32_midiInOpen(const char* vfn, MidiInCb midiInCb, void* ref);
int w32_midiInClose(unsigned idx);

#endif // MIDI_W32

/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Midi.c,v $
**
** $Revision: 1.15 $
**
** $Date: 2008-03-31 19:42:24 $
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
#include "Win32Midi.h"
#include "Properties.h"
#include "MsxTypes.h"
#include "ArchMidi.h"
#include "SaveState.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0400

#include <windows.h>
#include <mmsystem.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static UInt8 MT32toGM[128] = {
    0,   1,   2,   4,   4,   5,   5,   3,   16,  16,
    16,  16,  19,  19,  19,  21,  6,   6,   6,   7,
    7,   7,   8,   8,   62,  57,  63,  58,  38,  38,
    39,  39,  88,  33,  52,  35,  97,  100, 38,  39,
    14,  102, 68,  103, 44,  92,  46,  80,  48,  49,
    51,  45,  40,  40,  42,  42,  43,  46,  46,  24,
    25,  28,  27,  104, 32,  32,  34,  33,  36,  37,
    39,  35,  79,  73,  76,  72,  74,  75,  64,  65,
    66,  67,  71,  71,  69,  70,  60,  22,  56,  59,
    57,  63,  60,  60,  58,  61,  61,  11,  11,  99,
    100, 9,   14,  13,  12,  107, 106, 77,  78,  78,
    76,  111, 47,  117, 127, 115, 118, 116, 118, 126,
    121, 121, 55,  124, 120, 125, 126, 127
};



#define MAX_MIDI_SYSMES_LENGTH 4096

#define HISTORY_SIZE 256

typedef struct DevInfo {
	int   id;
    char  idString[32];
	HMIDI handle;
	char  name[MAX_PATH];
    int   recording;
    DWORD origVolume;
    int   refCount;
    int   noteOn;
    ArchMidi* archMidi;

    int   notes[256];
    struct {
        int count;
        int remain;
        int runningStatus;
        UInt8 data[MAX_MIDI_SYSMES_LENGTH + 1];
    } buffer;
} DevInfo;

typedef struct MidiDevices
{
    struct {
        int count;
        DevInfo* dev;
        DevInfo* current[2];
        int noteFilter;
    } in;
    
    struct {
        int count;
        DevInfo* dev;
        int mt32ToGm;
        DWORD volume;
        DevInfo* current[2];
        struct {
            UInt32 data[HISTORY_SIZE];
            UInt32 head;
        } history;
    } out;

    int enabled;
} MidiDevices;


struct ArchMidi {
    DevInfo*     devInfo;
    ArchMidiInCb callback;
    void*        ref;
};

static MidiDevices midi;


void midiInitialize()
{
	MIDIINCAPSA  inCap;
	MIDIOUTCAPSA outCap;
    int num;

    memset(&midi, 0, sizeof(midi));
    midi.enabled = 1;

	num = midiOutGetNumDevs();
    if (num > 0 && midiOutGetDevCapsA(MIDI_MAPPER, &outCap, sizeof(outCap)) == MMSYSERR_NOERROR) {
        int i;

        midi.out.dev = (DevInfo*)calloc(1, (num + 1) * sizeof(DevInfo));

        strcpy(midi.out.dev[0].name, outCap.szPname);
        midi.out.dev[0].id = MIDI_MAPPER;
	    strcpy(midi.out.dev[0].idString, "midi-out");
        midi.out.count++;

	    for (i = 0; i < num; i++) {
		    if (midiOutGetDevCapsA(i, &outCap, sizeof(outCap)) == MMSYSERR_NOERROR) {
                strcpy(midi.out.dev[midi.out.count].name, outCap.szPname);
		        midi.out.dev[midi.out.count].id = i;
		        sprintf(midi.out.dev[midi.out.count].idString, "midi-out-%u", i);
                midi.out.count++;
            }
	    }
    }

	num = midiInGetNumDevs();
    if (num > 0) {
        int i;

        midi.in.dev = (DevInfo*)calloc(1, num * sizeof(DevInfo));

	    for (i = 0; i < num; ++i) {
		    if (midiInGetDevCapsA(i, &inCap, sizeof(inCap)) == MMSYSERR_NOERROR) {
                strcpy(midi.in.dev[midi.in.count].name, inCap.szPname);
		        midi.in.dev[midi.in.count].id = i;
		        sprintf(midi.in.dev[midi.in.count].idString, "midi-in-%u", i);
                midi.in.count++;
            }
	    }
    }
}

void midiShutdown()
{
    free(midi.in.dev);
    free(midi.out.dev);

    memset(&midi, 0, sizeof(midi));
}

int midiInGetDeviceCount()
{
    return midi.in.count;
}

const char* midiInGetDeviceName(int idx)
{
    return midi.in.dev[idx].name;
}

const char* midiInGetDeviceIdString(int idx)
{
    return midi.in.dev[idx].idString;
}

int midiOutGetDeviceCount()
{
    return midi.out.count;
}

const char* midiOutGetDeviceName(int idx)
{
    return midi.out.dev[idx].name;
}

const char* midiOutGetDeviceIdString(int idx)
{
    return midi.out.dev[idx].idString;
}

void midiEnableMt32ToGmMapping(int enable)
{
    midi.out.mt32ToGm = enable;
    if (midi.out.current[0] != NULL) {
        DevInfo* di = midi.out.current[0];
        UInt32 i;
        for (i = 0; i < HISTORY_SIZE; i++) {
            UInt32 msg = midi.out.history.data[(i + midi.out.history.head) & (HISTORY_SIZE - 1)];
            if ((msg & 0xf0) == 0xc0) {
                if (midi.out.mt32ToGm) {
                    msg = (msg & 0xffff80ff) | (MT32toGM[di->buffer.data[1] & 0x7f] << 8);
                }
                midiOutShortMsg((HMIDIOUT)di->handle, msg);
            }
        }
    }
}

void midiInSetChannelFilter(int filter)
{
    int i;

    midi.in.noteFilter = filter;
    for (i = 0; i < 2; i++) {
        if (midi.in.current[i] != NULL) {
            memset(midi.in.current[i]->notes, 0, sizeof(midi.in.current[i]->notes));
        }
    }

}
int  archMidiGetNoteOn()
{
    int noteOn = 0;
    if (midi.out.current[0] != NULL) {
        noteOn = midi.out.current[0]->noteOn;
        midi.out.current[0]->noteOn = 0;
    }
    return noteOn;
}

void archMidiEnable(int enable)
{
    int i;

    midi.enabled = enable;
    if (midi.out.current[0] != NULL) {
        midiOutSetVolume((HMIDIOUT)midi.out.current[0]->handle, midi.out.volume * midi.enabled);
    }
    for (i = 0; i < 2; i++) {
        if (midi.in.current[i] != NULL) {
            if (enable && !midi.in.current[i]->recording) {
                midiInStart((HMIDIIN)midi.in.current[i]->handle);
                midi.in.current[i]->recording = 1;
            }
            else if (!enable && midi.in.current[i]->recording) {
                midiInStop((HMIDIIN)midi.in.current[i]->handle);
                midi.in.current[i]->recording = 0;
            }
        }
    }
}

void archMidiUpdateVolume(int left, int right)
{
    midi.out.volume = MIN(left * 100, 0xffff) | (MIN(right * 100, 0xffff) << 16);
    archMidiEnable(midi.enabled);
}

void archMidiLoadState(void)
{
    SaveState* state = saveStateOpenForRead("ArchMidi");
    char buf[64];
    int i;

    // Get history
    for (i = 0; i < HISTORY_SIZE; i++) {
        char buf[32];
        sprintf(buf, "history_%d", i);
        midi.out.history.data[i] = saveStateGet(state, buf, 0);
    }
    midi.out.history.head = saveStateGet(state, "history_head", 0);

    for (i = 0; i < 4; i++) {
        DevInfo* di = NULL;
        switch (i) {
        case 0: di = midi.out.current[0]; break;
        case 1: di = midi.out.current[1]; break;
        case 2: di = midi.in.current[0]; break;
        case 3: di = midi.in.current[1]; break;
        }
        if (di != NULL) {
            sprintf(buf, "buffer_%d_count", i);
            di->buffer.count = saveStateGet(state, buf, 0);
            sprintf(buf, "buffer_%d_remain", i);
            di->buffer.remain = saveStateGet(state, buf, 0);
            // status byte must be resent from di->buffer.data[0]
            // even if di->buffer.runningStatus was 1 when state saved.
            di->buffer.runningStatus = 0;
            sprintf(buf, "buffer_%d_data", i);
            saveStateGetBuffer(state, buf, di->buffer.data, sizeof(di->buffer.data));
        }
    }
    
    saveStateClose(state);

    midiEnableMt32ToGmMapping(midi.out.mt32ToGm);
}

void archMidiSaveState(void)
{
    SaveState* state = saveStateOpenForWrite("ArchMidi");
    char buf[64];
    int i;

    // Set history
    for (i = 0; i < HISTORY_SIZE; i++) {
        char buf[32];
        sprintf(buf, "history_%d", i);
        saveStateSet(state, buf, midi.out.history.data[i]);
    }
    saveStateSet(state, "history_head", midi.out.history.head);

    for (i = 0; i < 4; i++) {
        DevInfo* di = NULL;
        switch (i) {
        case 0: di = midi.out.current[0]; break;
        case 1: di = midi.out.current[1]; break;
        case 2: di = midi.in.current[0]; break;
        case 3: di = midi.in.current[1]; break;
        }
        if (di != NULL) {
            sprintf(buf, "buffer_%d_count", i);
            saveStateSet(state, buf, di->buffer.count);
            sprintf(buf, "buffer_%d_remain", i);
            saveStateSet(state, buf, di->buffer.remain);
            // no need to save di->buffer.runningStatus
            // (set 0 at archMidiLoadState())
            sprintf(buf, "buffer_%d_data", i);
            saveStateSetBuffer(state, buf, di->buffer.data, sizeof(di->buffer.data));
        }
    }
    
    saveStateClose(state);
}

ArchMidi* archMidiOutCreate(int device) 
{
    Properties* pProperties = propGetGlobalProperties();
    char* propName = device == 0 ? pProperties->sound.MidiOut.name : pProperties->sound.MidiOut.name;
    ArchMidi* archMidi = (ArchMidi*)calloc(1, sizeof(ArchMidi));

    if (device != 0) {
        return archMidi;
    }

    if (midi.out.current[device] == NULL) {
        int i;
        for (i = 0; i < midi.out.count; i++) {
            if (strcmp(midi.out.dev[i].idString, propName) == 0) {
	            if (midiOutOpen((HMIDIOUT*)&midi.out.dev[i].handle, midi.out.dev[i].id, 0, 0 ,0) == MMSYSERR_NOERROR) {
		            midi.out.current[device] = &midi.out.dev[i];
	            }
                break;
            }
        }
        if (midi.out.current[device] == NULL && midi.out.count > 0) {
	        if (midiOutOpen((HMIDIOUT*)&midi.out.dev[0].handle, midi.out.dev[0].id, 0, 0 ,0) == MMSYSERR_NOERROR) {
		        midi.out.current[device] = &midi.out.dev[0];
	        }
            strcpy(propName, midi.out.dev[0].idString);
        }

        if (midi.out.current[device] != NULL) {
            midiOutGetVolume((HMIDIOUT)midi.out.current[device]->handle, &midi.out.current[device]->origVolume);
        }
        archMidiEnable(midi.enabled);

        // Clear history
        memset(midi.out.history.data, 0, sizeof(midi.out.history.data));
    }

    if (midi.out.current[device] != NULL) {
        midi.out.current[device]->refCount++;
    }

    archMidi->devInfo = midi.out.current[device];

    return archMidi;
}

void archMidiOutDestroy(ArchMidi* archMidi)
{
    if (archMidi->devInfo == NULL) {
        free(archMidi);
        return;
    }

    if (--archMidi->devInfo->refCount > 0) {
        free(archMidi);
        return;
    }
    
    midiOutSetVolume((HMIDIOUT)archMidi->devInfo->handle, archMidi->devInfo->origVolume);
    midiOutClose((HMIDIOUT)archMidi->devInfo->handle);

    if (archMidi->devInfo == midi.out.current[0]) midi.out.current[0] = NULL;
    if (archMidi->devInfo == midi.out.current[1]) midi.out.current[1] = NULL;

    free(archMidi);
}

void archMidiOutTransmit(ArchMidi* archMidi, UInt8 value)
{
    DevInfo* di = archMidi->devInfo;

    if (di == NULL) {
        return;
    }

    if (di->buffer.count == 0) {
        UInt8 status;

        if ((value & 0x080) != 0x00) {
            status = value;
        } else {
            // running status
            status = di->buffer.data[0];
            di->buffer.count++;
            di->buffer.runningStatus = 1;
        }

        di->buffer.data[1] = 0;
        di->buffer.data[2] = 0;

		switch (status & 0x0f0) {
		case 0x090:	// Note On
            di->noteOn = 1;
		case 0x080:	// Note Off
		case 0x0a0:	// Key Pressure
		case 0x0b0:	// Control Change
		case 0x0e0:	// Pitch Wheel
            di->buffer.remain = 3;
            break;
		case 0x0c0:	// Program Change
		case 0x0d0:	// After Touch
            di->buffer.remain = 2;
            break;
		case 0x0f0:	// SYSTEM MESSAGE
			switch (value & 0x0f) {
            case 0x00:  // Exclusive
                di->buffer.remain = 0;
                break;
			case 0x02:	// Song Position Pointer
                di->buffer.remain = 3;
				break;
			case 0x01:	// Time Code
			case 0x03:	// Song Select
                di->buffer.remain = 2;
                break;
            default:
                di->buffer.remain = 1;
                break;
            }
            break;
        default:
            di->buffer.remain = 1;
            break;
        }
        if (di->buffer.runningStatus) {
            di->buffer.remain--;
        }
    }

    di->buffer.data[di->buffer.count] = value;

    if (di->buffer.count < MAX_MIDI_SYSMES_LENGTH) {
        di->buffer.count++;
    }
    di->buffer.remain--;

    // End of short message
    if (di->buffer.remain == 0) {
        DWORD msg = (di->buffer.data[0] <<  0) | (di->buffer.data[1] <<  8) | (di->buffer.data[2] << 16);

        if ((di->buffer.data[0] & 0xf0) == 0xc0) {
            midi.out.history.data[midi.out.history.head] = msg;
            midi.out.history.head = (midi.out.history.head + 1) & (HISTORY_SIZE - 1);
            if (midi.out.mt32ToGm) {
                msg = (msg & 0xffff80ff) | (MT32toGM[di->buffer.data[1] & 0x7f] << 8);
            }
        }
        if (di->buffer.runningStatus) {
            // omit status byte
            msg >>= 8;
            di->buffer.runningStatus = 0;
        }
        midiOutShortMsg((HMIDIOUT)di->handle, msg);
        di->buffer.count = 0;
    }

    // End of long message
    if (di->buffer.remain < 0 && value == 0xf7) {
        // Only send message if data was not truncated
        if (di->buffer.count < MAX_MIDI_SYSMES_LENGTH) {
            MIDIHDR header;
            
	        header.lpData         = di->buffer.data;
	        header.dwBufferLength = di->buffer.count;
	        header.dwFlags        = 0;

            if (midiOutPrepareHeader((HMIDIOUT)di->handle, &header, sizeof(header)) == MMSYSERR_NOERROR) {
                int cnt = 10;
        	    if (midiOutLongMsg((HMIDIOUT)di->handle, &header, sizeof(header)) == MMSYSERR_NOERROR) {

	                while (!(header.dwFlags & MHDR_DONE) && --cnt) {
		                Sleep(1);
	                }
                    midiOutUnprepareHeader((HMIDIOUT)di->handle, &header, sizeof(header));
                }
            }
        }
        di->buffer.count = 0;
    }
}

static void CALLBACK midiInCallback(HMIDIIN hMidiIn,  
                                    UINT wMsg,        
                                    DWORD dwInstance, 
                                    DWORD dwParam1,   
                                    DWORD dwParam2)
{
    DWORD id = dwInstance;
    char buffer[4];
    int length = 0;
    MIDIHDR* hdr;
    int i;

    switch (wMsg) {
    case MM_MIM_DATA:
	case MM_MIM_MOREDATA:
        buffer[0] = (char)((dwParam1 >>  0) & 0xff);
        buffer[1] = (char)((dwParam1 >>  8) & 0xff);
        buffer[2] = (char)((dwParam1 >> 16) & 0xff);
        buffer[3] = (char)((dwParam1 >> 24) & 0xff);
		switch (buffer[0] & 0x0f0) {
		case 0x090:	// Note On
		case 0x080:	// Note Off
            if (midi.in.noteFilter == 0 || midi.in.noteFilter == (buffer[0] & 0x0f) + 1) {
                for (i = 0; i < 2; i++) {
                    DevInfo* di = midi.in.current[i];
                    if (di != NULL && di->id == id) {
                        if ((buffer[0] & 0x10) && buffer[2] > 0) {
                            di->notes[buffer[1]]++;
                        }
                        else if (di->notes[buffer[1]] > 0) {
                            di->notes[buffer[1]]--;
                        }
                    }
                }
            }
		case 0x0a0:	// Key Pressure
		case 0x0b0:	// Control Change
		case 0x0e0:	// Pitch Wheel
            length = 3;
            break;
		case 0x0c0:	// Program Change
		case 0x0d0:	// After Touch
            length = 2;
            break;
		case 0x0f0:	// SYSTEM MESSAGE (other than "EXCLUSIVE")
			switch (buffer[0] & 0x0f) {
			case 0x02:	// Song Position Pointer
                length = 3;
				break;
			case 0x01:	// Time Code
			case 0x03:	// Song Select
                length = 2;
                break;
            default:
                length = 1;
                break;
            }
            break;
        default:
            length = 1;
            break;
        }

        for (i = 0; i < 2; i++) {
            DevInfo* di = midi.in.current[i];
            if (di != NULL && di->id == id && di->archMidi->callback != NULL) {
                di->archMidi->callback(di->archMidi->ref, buffer, length);
            }
        }
        break;

    case MM_MIM_LONGDATA:
        hdr = (MIDIHDR*)dwParam1;
        for (i = 0; i < 2; i++) {
            DevInfo* di = midi.in.current[i];
            if (di != NULL && di->id == id && di->archMidi->callback != NULL) {
                di->archMidi->callback(di->archMidi->ref, hdr->lpData, hdr->dwBytesRecorded);
            }
        }
        break;
    }
}

ArchMidi* archMidiInCreate(int device, ArchMidiInCb cb, void* ref)
{
    Properties* pProperties = propGetGlobalProperties();
    char* propName = device == 1 ? pProperties->sound.YkIn.name : pProperties->sound.MidiIn.name;                        
    ArchMidi* archMidi = (ArchMidi*)calloc(1, sizeof(ArchMidi));

    archMidi->callback = cb;
    archMidi->ref      = ref;

    if (device != 0 && device != 1) {
        return archMidi;
    }

    if (midi.in.current[device] == NULL) {
        int i;
        for (i = 0; i < midi.in.count; i++) {
            if (strcmp(midi.in.dev[i].idString, propName) == 0) {
                if (midi.in.dev[i].refCount > 0 ||
                    midiInOpen((HMIDIIN*)&midi.in.dev[i].handle, midi.in.dev[i].id, 
                               (DWORD_PTR)midiInCallback, midi.in.dev[i].id, CALLBACK_FUNCTION) == MMSYSERR_NOERROR) 
                {
		            midi.in.current[device] = &midi.in.dev[i];
	            }
                break;
            }
        }
        if (midi.in.current[device] == NULL && midi.in.count > 0) {
            if (midi.in.dev[0].refCount > 0 ||
	            midiInOpen((HMIDIIN*)&midi.in.dev[0].handle, midi.in.dev[0].id,
                           (DWORD_PTR)midiInCallback, midi.in.dev[0].id, CALLBACK_FUNCTION) == MMSYSERR_NOERROR) 
            {
		        midi.in.current[device] = &midi.in.dev[0];
	        }
            strcpy(propName, midi.in.dev[0].idString);
        }

        if (midi.in.current[device] != NULL) {
            memset(midi.in.current[device]->notes, 0, sizeof(midi.in.current[device]->notes));
        }
        archMidiEnable(midi.enabled);
    }

    if (midi.in.current[device] != NULL) {
        midi.in.current[device]->refCount++;
        midi.in.current[device]->archMidi = archMidi;
    }

    archMidi->devInfo = midi.in.current[device];

    return archMidi;
}

void archMidiInDestroy(ArchMidi* archMidi)
{
    if (archMidi->devInfo == NULL) {
        free(archMidi);
        return;
    }

    if (--archMidi->devInfo->refCount > 0) {
        free(archMidi);
        return;
    }

    if (archMidi->devInfo->recording) {
        midiInStop((HMIDIIN)archMidi->devInfo->handle);
        archMidi->devInfo->recording = 0;
    }
    
    midiInClose((HMIDIIN)archMidi->devInfo->handle);

    if (archMidi->devInfo == midi.in.current[0]) midi.in.current[0] = NULL;
    if (archMidi->devInfo == midi.in.current[1]) midi.in.current[1] = NULL;

    free(archMidi);
}


int archMidiInGetNoteOn(ArchMidi* archMidi, int note)
{
    if (archMidi->devInfo == NULL) {
        return 0;
    }
    if (note >= 256) {
        return 0;
    }
    return archMidi->devInfo->notes[note];
}
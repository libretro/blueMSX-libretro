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
#include "Midi_w32.h"

#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0400

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>

#include "SaveState.h"

#define MAXPATHLEN MAX_PATH

#define assert(x)


#define MIDI_SYSMES_MAXLEN 4096

static void w32_setHistory(unsigned idx);

/*
 * MIDI I/O helper functions for Win32.
 */

typedef struct VfnMidi {
	unsigned idx;
	unsigned devid;
	HMIDI    handle;
	char     vfname[MAXPATHLEN + 1];
	char     devname[MAXPNAMELEN];
} VfnMidi;

typedef struct Outbuf {
	DWORD    shortmes;
	unsigned longmes_cnt;
	char     longmes[MIDI_SYSMES_MAXLEN];
	MIDIHDR  header;
} Outbuf;

typedef struct MidiOut {
    int state;
    VfnMidi vfn;
    Outbuf buf;
    int noteOn;
    int mt32ToGm;
} MidiOut;

static unsigned int midiOutHistory[256];

static MidiOut* midiOut;
static unsigned midiOutNum;

int MT32toGM[128] = {
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



static void w32_midiDevNameConv(char *dst, char *src)
{
	unsigned len = strlen(src);
	unsigned i;
	for (i = 0; i < len; ++i) {
		if ((src[i] < '0') || (src[i] > 'z') ||
		    ((src[i] > '9') && (src[i] < 'A')) ||
		    ((src[i] > 'Z') && (src[i] < 'a'))) {
			dst[i] = '_';
		} else {
			dst[i] = src[i];
		}
	}
	dst[i] = '\0';
}


// MIDI-OUT
static int w32_midiOutFindDev(unsigned *idx, unsigned *dev, const char *vfn)
{
    unsigned i;
	for (i = 0; i < midiOutNum; ++i) {
		if (!strcmp(midiOut[i].vfn.vfname, vfn)) {
			*idx = i;
			*dev = midiOut[i].vfn.devid;
			return 0;
		}
	}
	return -1;
}


int w32_midiOutInit()
{
	MIDIOUTCAPSA cap;
    unsigned num;
    unsigned i;

	midiOutNum = 0;
	num = midiOutGetNumDevs();
	if (!num) {
		return 0;
	}

	if ((midiOut = (MidiOut*)malloc((num + 1) * sizeof(MidiOut))) == NULL) {
		return 1;
	}
	memset(midiOut, 0, (num + 1) * sizeof(MidiOut));

	if (midiOutGetDevCapsA(MIDI_MAPPER, &cap, sizeof(cap)) != MMSYSERR_NOERROR) {
		return 2;
	}
	midiOut[0].vfn.devid = MIDI_MAPPER;
	w32_midiDevNameConv(midiOut[0].vfn.devname, cap.szPname);
	strncpy(midiOut[0].vfn.vfname, "midi-out", MAXPATHLEN + 1);
	midiOutNum++;

	for (i = 0; i < num; ++i) {
		if (midiOutGetDevCapsA(i, &cap, sizeof(cap)) != MMSYSERR_NOERROR) {
			return 0;	// atleast MIDI-MAPPER is available...
		}
		midiOut[i + 1].vfn.devid = i;
		w32_midiDevNameConv(midiOut[i + 1].vfn.devname, cap.szPname);
		sprintf(midiOut[i + 1].vfn.vfname, "midi-out-%u", i);
		midiOutNum++;
	}
	return 0;
}

int w32_midiOutClean()
{
	midiOutNum = 0;
	free(midiOut);
	return 0;
}


unsigned w32_midiOutGetVFNsNum()
{
	return midiOutNum;
}

const char* w32_midiOutGetVFN(unsigned nmb)
{
	assert(nmb < midiOutNum);
	return midiOut[nmb].vfn.vfname;
}

const char* w32_midiOutGetRDN(unsigned nmb)
{
	assert(nmb < midiOutNum);
	return midiOut[nmb].vfn.devname;
}


unsigned w32_midiOutOpen(const char *vfn)
{
	unsigned idx, devid;
	if (w32_midiOutFindDev(&idx, &devid,vfn)) {
		return (unsigned)-1;
	}
	if (midiOutOpen((HMIDIOUT*)&midiOut[idx].vfn.handle, devid, 0, 0 ,0) != MMSYSERR_NOERROR) {
		return (unsigned)-1;
	}
    
    w32_setHistory(idx);

	return idx;
}

int w32_midiOutClose(unsigned idx)
{
	midiOutReset((HMIDIOUT)midiOut[idx].vfn.handle);
	if (midiOutClose((HMIDIOUT)midiOut[idx].vfn.handle) == MMSYSERR_NOERROR) {
		return 0;
	} else {
		return -1;
	}
}


static int w32_midiOutFlushExclusiveMsg(unsigned idx)
{
	int i;
	midiOut[idx].buf.header.lpData = midiOut[idx].buf.longmes;
	midiOut[idx].buf.header.dwBufferLength = midiOut[idx].buf.longmes_cnt;
	midiOut[idx].buf.header.dwFlags = 0;
	if ((i = midiOutPrepareHeader((HMIDIOUT)midiOut[idx].vfn.handle, &midiOut[idx].buf.header, sizeof(midiOut[idx].buf.header))) != MMSYSERR_NOERROR) {
        return -1;
	}
	if ((i = midiOutLongMsg((HMIDIOUT)midiOut[idx].vfn.handle, &midiOut[idx].buf.header, sizeof(midiOut[idx].buf.header))) != MMSYSERR_NOERROR) {
        return -1;
	}
	// Wait sending in driver.
	// This may take long...
	while (!(midiOut[idx].buf.header.dwFlags & MHDR_DONE)) {
		Sleep(1);
	}
	// Sending Exclusive done.
	if ((i = midiOutUnprepareHeader((HMIDIOUT)midiOut[idx].vfn.handle, &midiOut[idx].buf.header, sizeof(midiOut[idx].buf.header))) != MMSYSERR_NOERROR) {
        return -1;
	}
	midiOut[idx].buf.longmes_cnt = 0;
	return 0;
}

int w32_midiOutPut(unsigned char value, unsigned idx)
{
	if ((midiOut[idx].state & 0x1000) || ((value & 0x0ff) == 0x0f0)) {
		if (!(midiOut[idx].state & 0x1000)) {
			// SYSTEM MESSAGE Exclusive start
			midiOut[idx].state |= 0x1000;
		}
		if (midiOut[idx].buf.longmes_cnt >= MIDI_SYSMES_MAXLEN) {
			return -1;
		}
		midiOut[idx].buf.longmes[midiOut[idx].buf.longmes_cnt++] = value;

		if (value == 0x0f7) {
			// SYSTEM MESSAGES Exclusive end
			w32_midiOutFlushExclusiveMsg(idx);
			midiOut[idx].state &= ~0x1000;
		}
	} else {
		switch (midiOut[idx].state) {
		case 0x0000:
			switch (value & 0x0f0) {
			case 0x090:	// Note On
                midiOut[idx].noteOn = 1;
			case 0x080:	// Note Off
			case 0x0a0:	// Key Pressure
			case 0x0b0:	// Control Change
			case 0x0e0:	// Pitch Wheel
				midiOut[idx].state = 0x0082;
				midiOut[idx].buf.shortmes = ((DWORD)value) & 0x0ff;
				break;
			case 0x0c0:	// Program Change
			case 0x0d0:	// After Touch
				midiOut[idx].state = 0x0041;
				midiOut[idx].buf.shortmes = ((DWORD)value) & 0x0ff;
				break;
			case 0x0f0:	// SYSTEM MESSAGE (other than "EXCLUSIVE")
				switch (value &0x0f) {
					case 0x02:	// Song Position Pointer
						midiOut[idx].state = 0x0082;
						midiOut[idx].buf.shortmes = ((DWORD)value) & 0x0ff;
						break;
					case 0x01:	// Time Code
					case 0x03:	// Song Select
						midiOut[idx].state = 0x0041;
						midiOut[idx].buf.shortmes = ((DWORD)value) & 0x0ff;
						break;
					default:	// Timing Clock, Sequencer Start, Sequencer Continue,
							// Sequencer Stop, Cable Check, System Reset, and Unknown...
						midiOut[idx].state = 0;
						midiOut[idx].buf.shortmes = ((DWORD)value) & 0x0ff;
						midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,midiOut[idx].buf.shortmes);
                        midiOutHistory[midiOut[idx].buf.shortmes & 0xff] = midiOut[idx].buf.shortmes;
						break;
				}
				break;
			default:
				midiOut[idx].state = 0;
				midiOut[idx].buf.shortmes = ((DWORD)value) & 0x0ff;
				midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,midiOut[idx].buf.shortmes);
				break;
			}
			break;
		case 0x0041:
            {
                DWORD shortmes = midiOut[idx].buf.shortmes |= ((((DWORD)value) & 0x0ff) << 8);
                midiOutHistory[midiOut[idx].buf.shortmes & 0xff] = shortmes;

                if (midiOut[idx].mt32ToGm && (midiOut[idx].buf.shortmes & 0xf0) == 0xc0) {
                    shortmes = (shortmes & 0x80ff) | (MT32toGM[value & 0x7f] << 8);
                }
			    midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle, shortmes);
			    midiOut[idx].state = 0;
            }
			break;
		case 0x0082:
			midiOut[idx].buf.shortmes |= ((((DWORD)value) & 0x0ff) << 8);
			midiOut[idx].state = 0x0081;
			break;
		case 0x0081:
			midiOut[idx].buf.shortmes |= ((((DWORD)value) & 0x0ff) << 16);
			midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,midiOut[idx].buf.shortmes);
            midiOutHistory[midiOut[idx].buf.shortmes & 0xff] = midiOut[idx].buf.shortmes;
			midiOut[idx].state = 0;
			break;
		default:
			// not reach...
			midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,((DWORD)value) & 0x0ff);
			break;
		}
	}
	return 0;
}

unsigned int w32_midiOutGetVolume(unsigned idx)
{
    DWORD volume;
    midiOutGetVolume((HMIDIOUT)midiOut[idx].vfn.handle, &volume);

    return volume;
}

void w32_midiOutSetVolume(unsigned idx, unsigned int volume)
{
    int rv = midiOutSetVolume((HMIDIOUT)midiOut[idx].vfn.handle, volume);
}

int w32_midiOutNoteOn(unsigned idx)
{
    int on = midiOut[idx].noteOn;
    midiOut[idx].noteOn = 0;
    return on;
}

void w32_midiOutEnableMt32ToGmMapping(unsigned idx, int enable)
{
    int i;
    midiOut[idx].mt32ToGm = enable;

    for (i = 0xc0; i < 0xd0; i++) {
        if (midiOutHistory[i]) {
            DWORD value = midiOutHistory[i];
            if (midiOut[idx].mt32ToGm) {
                value = (value & 0x80ff) | (MT32toGM[(value >> 8) & 0x7f] << 8);
            }
		    midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,value);
        }
    }
}

void w32_resetHistory()
{
    memset(midiOutHistory, 0, sizeof(midiOutHistory));
}

static void w32_setHistory(unsigned idx)
{
    int i;
    for (i = 0; i < 0xf0; i++) {
        if (midiOutHistory[i]) {
            DWORD value = midiOutHistory[i];
            if (midiOut[idx].mt32ToGm && (value & 0xf0) == 0xc0) {
                value = (value & 0x80ff) | (MT32toGM[(value >> 8) & 0x7f] << 8);
            }
		    midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,value);
        }
    }
}

void w32_midiOutLoadState(unsigned idx)
{
    SaveState* state = saveStateOpenForRead("MidiOut");
    int i;

    for (i = 0; i < 256; i++) {
        char buf[32];
        sprintf(buf, "_%d", i);
        midiOutHistory[i] = saveStateGet(state, buf, 0);
    }

    midiOut[idx].state = saveStateGet(state, "state", 0);
    midiOut[idx].buf.shortmes = saveStateGet(state, "shortmes", 0);
    
    saveStateClose(state);

    for (i = 0; i < 0xf0; i++) {
        if (midiOutHistory[i]) {
            DWORD value = midiOutHistory[i];
            if ((value & 0xf0) == 0xc0) {
                value = (value & 0x80ff) | (MT32toGM[(value >> 8) & 0x7f] << 8);
            }
		    midiOutShortMsg((HMIDIOUT)midiOut[idx].vfn.handle,value);
        }
    }
}

void w32_midiOutSaveState(unsigned idx)
{
    SaveState* state = saveStateOpenForWrite("MidiOut");
    int i;

    for (i = 0; i < 256; i++) {
        char buf[32];
        sprintf(buf, "_%d", i);
        saveStateSet(state, buf, midiOutHistory[i]);
    }

    saveStateSet(state, "state", midiOut[idx].state);
    saveStateSet(state, "shortmes", midiOut[idx].buf.shortmes);

    saveStateClose(state);
}



//////////////////////////////////////////////////////
//
// MIDI-IN
//
//////////////////////////////////////////////////////

static struct VfnMidi *vfnt_midiin;
static unsigned vfnt_midiin_num;
static MIDIHDR inhdr;
static char inlongmes[MIDI_SYSMES_MAXLEN];

static int w32_midiInFindDev(unsigned *idx, unsigned *dev, const char *vfn)
{
    unsigned i;
	for (i = 0; i < vfnt_midiin_num; ++i) {
		if (!strcmp(vfnt_midiin[i].vfname, vfn)) {
			*idx = i;
			*dev = vfnt_midiin[i].devid;
			return 0;
		}
	}
	return -1;
}


int w32_midiInInit()
{
    unsigned num;
    unsigned i;

	vfnt_midiin_num = 0;
	num = midiInGetNumDevs();
	if (!num) {
		return 0;
	}
	if ((vfnt_midiin = (struct VfnMidi*)malloc((num + 1) * sizeof(struct VfnMidi))) == NULL) {
		return	1;
	}
	for (i = 0; i < num; ++i) {
		MIDIINCAPSA cap;
		if (midiInGetDevCapsA(i, &cap, sizeof(cap)) != MMSYSERR_NOERROR) {
			return 1;
		}
		vfnt_midiin[i].devid = i;
		w32_midiDevNameConv(vfnt_midiin[i].devname, cap.szPname);
		sprintf(vfnt_midiin[i].vfname, "midi-in-%u", i);
		vfnt_midiin_num++;
	}
	return 0;
}

int w32_midiInClean()
{
	vfnt_midiin_num = 0;
	free(vfnt_midiin);
	return 0;
}


unsigned w32_midiInGetVFNsNum()
{
	return vfnt_midiin_num;
}

const char* w32_midiInGetVFN(unsigned nmb)
{
	assert(nmb < vfnt_midiin_num);
	return vfnt_midiin[nmb].vfname;
}

const char* w32_midiInGetRDN(unsigned nmb)
{
	assert(nmb < vfnt_midiin_num);
	return vfnt_midiin[nmb].devname;
}

typedef struct MidiInCbInfo {
    MidiInCb cb;
    void* ref;
} MidiInCbInfo;

static MidiInCbInfo cbInfo[32];

static void CALLBACK w32_midiInProc(HMIDIIN hMidiIn,  
                                    UINT wMsg,        
                                    DWORD dwInstance, 
                                    DWORD dwParam1,   
                                    DWORD dwParam2)
{
    char buffer[4];
    int length = 0;
    MIDIHDR* hdr;

    switch (wMsg) {
    case MM_MIM_DATA:
        buffer[0] = (char)((dwParam1 >>  0) & 0xff);
        buffer[1] = (char)((dwParam1 >>  8) & 0xff);
        buffer[2] = (char)((dwParam1 >> 16) & 0xff);
        buffer[3] = (char)((dwParam1 >> 24) & 0xff);
		switch (buffer[0] & 0x0f0) {
		case 0x090:	// Note On
		case 0x080:	// Note Off
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
        cbInfo[dwInstance].cb(cbInfo[dwInstance].ref, buffer, length);
        break;

    case MM_MIM_LONGDATA:
        hdr = (MIDIHDR*)dwParam1;
        cbInfo[dwInstance].cb(cbInfo[dwInstance].ref, hdr->lpData, hdr->dwBytesRecorded);
        break;
    }
}

unsigned w32_midiInOpen(const char *vfn, MidiInCb midiInCb, void* ref)
{
	unsigned idx, devid;
	if (w32_midiInFindDev(&idx, &devid, vfn)) {
		return (unsigned)-1;
	}
    cbInfo[idx].cb  = midiInCb;
    cbInfo[idx].ref = ref;

	if (midiInOpen((HMIDIIN*)&vfnt_midiin[idx].handle, devid, (DWORD_PTR)w32_midiInProc, idx, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
		return (unsigned)-1;
	}
	memset(&inhdr, 0, sizeof(MIDIHDR));
	inhdr.lpData = inlongmes;
	inhdr.dwBufferLength = MIDI_SYSMES_MAXLEN;
	if (midiInPrepareHeader((HMIDIIN)vfnt_midiin[idx].handle, (LPMIDIHDR)&inhdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR) {
		return (unsigned)-1;
	}
	if (midiInAddBuffer((HMIDIIN)vfnt_midiin[idx].handle, (LPMIDIHDR)&inhdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR) {
		return (unsigned)-1;
	}
	return idx;
}

int w32_midiInClose(unsigned idx)
{
	midiInStop((HMIDIIN)vfnt_midiin[idx].handle);
	midiInReset((HMIDIIN)vfnt_midiin[idx].handle);
	midiInUnprepareHeader((HMIDIIN)vfnt_midiin[idx].handle, (LPMIDIHDR)&inhdr, sizeof(MIDIHDR));
	if (midiInClose((HMIDIIN)vfnt_midiin[idx].handle) == MMSYSERR_NOERROR) {
		return 0;
	} else {
		return -1;
	}
}


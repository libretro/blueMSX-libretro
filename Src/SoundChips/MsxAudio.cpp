/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/MsxAudio.cpp,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/11 20:08:38 $
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
#include "MsxAudio.h"
#include <string.h>
#include "OpenMsxY8950.h"

extern "C" {
#include "Board.h"
#include "SaveState.h"
#include "IoPort.h"
#include "MediaDb.h"
#include "DeviceManager.h"
}

#define FREQUENCY        3579545
#define SAMPLERATE       44100
#define BUFFER_SIZE      10000
 
extern "C" Int32* msxaudioSync(void* ref, UInt32 count);

#define OFFSETOF(s, a) ((int)(&((s*)0)->a))

struct MsxAudio {
    MsxAudio() :
        timer1(0), timer2(0), timerRef1(-1), timerRef2(-1) {
        memset(defaultBuffer, 0, sizeof(defaultBuffer));
    }

    Mixer* mixer;
    Int32  handle;

    Int32  deviceHandle;
    Y8950* y8950;
    Int32  buffer[BUFFER_SIZE];
    Int32  defaultBuffer[BUFFER_SIZE];
    UInt32 timer1;
    UInt32 counter1;
    UInt8  timerRef1;
    UInt32 timer2;
    UInt32 counter2;
    UInt8  timerRef2;
    UInt8  registerLatch;
 };

MsxAudio* theMsxAudio = NULL;


extern "C" Int32* msxaudioSync(void* ref, UInt32 count) 
{
    MsxAudio* msxaudio = (MsxAudio*)ref;
    Int32* genBuf = NULL;

    genBuf = (Int32*)msxaudio->y8950->updateBuffer(count);
    if (genBuf == NULL) {
        genBuf = msxaudio->defaultBuffer;
    }
    return genBuf;
}

void msxaudioTimerSet(int timer, int count)
{
    if (timer == 1) {
        if (theMsxAudio->counter1 != -1) {
            theMsxAudio->counter1 = count;
        }
        theMsxAudio->timer1 = count;
    }
    else {
        if (theMsxAudio->counter2 != -1) {
            theMsxAudio->counter2 = count;
        }
        theMsxAudio->timer2 = count;
    }
}

void msxaudioTimerStart(int timer, int start, UInt8 ref)
{
    if (timer == 1) {
        theMsxAudio->timerRef1 = ref;
        theMsxAudio->counter1  = start ? theMsxAudio->timer1 : -1;
    }
    else {
        theMsxAudio->timerRef2 = ref;
        theMsxAudio->counter2  = start ? theMsxAudio->timer2 : -1;
    }
}

extern "C" void msxaudioTick2() {
    if (theMsxAudio != NULL) {
        theMsxAudio->y8950->pushTime(boardSystemTime());
    }
}

extern "C" void msxaudioTick(UInt32 elapsedTime) 
{
    if (theMsxAudio != NULL) {
        while (elapsedTime--) {
            if (theMsxAudio->counter1 != -1) {
                if (theMsxAudio->counter1-- == 0) {
                    theMsxAudio->counter1 = theMsxAudio->timer1;
                    theMsxAudio->y8950->callback(theMsxAudio->timerRef1);
                }
            }

            if (theMsxAudio->counter2 != -1) {
                if (theMsxAudio->counter2-- == 0) {
                    theMsxAudio->counter2 = theMsxAudio->timer2;
                    theMsxAudio->y8950->callback(theMsxAudio->timerRef2);
                }
            }
        }
    }
}

extern "C" void msxaudioDestroy(void* rm) {
    MsxAudio* msxaudio = (MsxAudio*)rm;
    deviceManagerUnregister(msxaudio->deviceHandle);

    ioPortUnregister(0xc0);
    ioPortUnregister(0xc1);

    mixerUnregisterChannel(msxaudio->mixer, msxaudio->handle);

    delete msxaudio->y8950;

    theMsxAudio = NULL;
}



extern "C" void msxaudioSaveState(void* rm)
{
}

extern "C" void msxaudioLoadState(void* rm)
{
}

extern "C" UInt8 msxaudioRead(MsxAudio* msxaudio, UInt16 ioPort)
{
	UInt8 result = 0xff;
    UInt32 systemTime = boardSystemTime();

	switch (ioPort & 0x01) {
	case 0:
		result = msxaudio->y8950->readStatus();
		break;
	case 1:
        mixerSync(msxaudio->mixer);
		result = msxaudio->y8950->readReg(msxaudio->registerLatch, systemTime);
		break;
	}
	return result;
}


extern "C" void msxaudioWrite(MsxAudio* msxaudio, UInt16 ioPort, UInt8 value) 
{
    UInt32 systemTime = boardSystemTime();
	switch (ioPort & 0x01) {
	case 0:
		msxaudio->registerLatch = value;
		break;
	case 1:
        mixerSync(msxaudio->mixer);
		msxaudio->y8950->writeReg(msxaudio->registerLatch, value, systemTime);
		break;
	}
}

extern "C" int msxaudioCreate(Mixer* mixer)
{
    DeviceCallbacks callbacks = { msxaudioDestroy, NULL, msxaudioSaveState, msxaudioLoadState };
    MsxAudio* msxaudio = new MsxAudio;
    UInt32 systemTime = boardSystemTime();

    theMsxAudio = msxaudio;

    msxaudio->mixer = mixer;
    msxaudio->counter1 = -1;
    msxaudio->counter2 = -1;
    msxaudio->registerLatch = 0;

    msxaudio->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_MSXAUDIO, 0, msxaudioSync, msxaudio);

    msxaudio->deviceHandle = deviceManagerRegister(ROM_MSXAUDIO, &callbacks, msxaudio);

    msxaudio->y8950 = new Y8950("MsxAudio", 256*1024, systemTime);
    msxaudio->y8950->setSampleRate(SAMPLERATE, boardGetY8950Oversampling());
	msxaudio->y8950->setVolume(32767);

    ioPortRegister(0xc0, (IoPortRead)msxaudioRead, (IoPortWrite)msxaudioWrite, msxaudio);
    ioPortRegister(0xc1, (IoPortRead)msxaudioRead, (IoPortWrite)msxaudioWrite, msxaudio);

    return 1;
}



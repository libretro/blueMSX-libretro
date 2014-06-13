/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/SN76489.c,v $
**
** $Revision: 1.15 $
**
** $Date: 2006/06/14 19:59:52 $
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
#include "SN76489.h"
#include "IoPort.h"
#include "SaveState.h"
#include "DebugDeviceManager.h"
#include "Language.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BASE_PHASE_STEP 0x28959becUL  /* = (1 << 28) * 3579545 / 32 / 44100 */
#define BUFFER_SIZE     10000

static const Int16 voltTable[16] = {
    0x26a9, 0x1eb5, 0x1864, 0x1360, 0x0f64, 0x0c39, 0x09b6, 0x07b6, 
    0x0620, 0x04dd, 0x03dd, 0x0312, 0x0270, 0x01f0, 0x018a, 0x0000
};

static Int32* sn76489Sync(void* ref, UInt32 count);


struct SN76489 {
    Mixer* mixer;
    Int32  handle;
    Int32  debugHandle;

    UInt16 latch;
    UInt32 noiseRand;

    UInt16 regs[8];

    UInt32 tonePhase[4];
    UInt32 toneStep[4];

    UInt8  ampVolume[4];
    Int32  ctrlVolume;
    Int32  oldSampleVolume;
    Int32  daVolume;

    Int32  buffer[BUFFER_SIZE];
};

void sn76489LoadState(SN76489* sn76489)
{
    SaveState* state = saveStateOpenForRead("sn76489");
    char tag[32];
    int i;

    sn76489->latch            = (UInt16)saveStateGet(state, "latch",           0);
    sn76489->noiseRand        =         saveStateGet(state, "noiseRand",       1);
    sn76489->ctrlVolume       =         saveStateGet(state, "ctrlVolume",      0);
    sn76489->oldSampleVolume  =         saveStateGet(state, "oldSampleVolume", 0);
    sn76489->daVolume         =         saveStateGet(state, "daVolume",        0);

    for (i = 0; i < 8; i++) {
        sprintf(tag, "reg%d", i);
        sn76489->regs[i] = (UInt16)saveStateGet(state, tag, 0);
    }

    for (i = 0; i < 4; i++) {
        sprintf(tag, "phase%d", i);
        sn76489->tonePhase[i] = saveStateGet(state, tag, 0);

        sprintf(tag, "toneStep%d", i);
        sn76489->toneStep[i] = saveStateGet(state, tag, 0);

        sprintf(tag, "ampVol%d", i);
        sn76489->ampVolume[i] = (UInt8)saveStateGet(state, tag, 0);
    }

    saveStateClose(state);
}

void sn76489SaveState(SN76489* sn76489)
{
    SaveState* state = saveStateOpenForWrite("sn76489");
    char tag[32];
    int i;

    saveStateSet(state, "latch",           sn76489->latch);
    saveStateSet(state, "noiseRand",       sn76489->noiseRand);
    saveStateSet(state, "ctrlVolume",      sn76489->ctrlVolume);
    saveStateSet(state, "oldSampleVolume", sn76489->oldSampleVolume);
    saveStateSet(state, "daVolume",        sn76489->daVolume);

    for (i = 0; i < 8; i++) {
        sprintf(tag, "reg%d", i);
        saveStateSet(state, tag, sn76489->regs[i]);
    }

    for (i = 0; i < 4; i++) {
        sprintf(tag, "phase%d", i);
        saveStateSet(state, tag, sn76489->tonePhase[i]);

        sprintf(tag, "toneStep%d", i);
        saveStateSet(state, tag, sn76489->toneStep[i]);

        sprintf(tag, "ampVol%d", i);
        saveStateSet(state, tag, sn76489->ampVolume[i]);
    }

    saveStateClose(state);
}

static void setDebugInfo(SN76489* sn76489, DbgDevice* dbgDevice)
{
    DbgRegisterBank* regBank;
    int i;

    regBank = dbgDeviceAddRegisterBank(dbgDevice, langDbgRegs(), 16);

    for (i = 0; i < 16; i++) {
        char reg[4];
        sprintf(reg, "R%d", i);
        dbgRegisterBankAddRegister(regBank,  i, reg, 8, sn76489->regs[i]);
    }
}

SN76489* sn76489Create(Mixer* mixer)
{
    SN76489* sn76489 = (SN76489*)calloc(1, sizeof(SN76489));
    int i;

    sn76489->mixer = mixer;

    sn76489->handle = mixerRegisterChannel(mixer, MIXER_CHANNEL_PSG, 0, sn76489Sync, sn76489);

    sn76489Reset(sn76489);

    {
        double v = 0x26a9;
        for (i = 0; i < 15; i++) {
            v /= 1.258925412;
        }
    }

    return sn76489;
}

void sn76489Reset(SN76489* sn76489)
{
    if (sn76489 != NULL) {
        int i;
    
        for (i = 0; i < 4; i++) {
            sn76489->regs[2 * i] = 0;
            sn76489->regs[2 * i + 1] = 0;
            sn76489->tonePhase[i] = 0;
            sn76489->ampVolume[i] = 0;
            sn76489->toneStep[i]  = 1 << 31;

        }
    }
}

void sn76489Destroy(SN76489* sn76489)
{
    debugDeviceUnregister(sn76489->debugHandle);
    mixerUnregisterChannel(sn76489->mixer, sn76489->handle);
    free(sn76489);
}

void sn76489WriteData(SN76489* sn76489, UInt16 ioPort, UInt8 data)
{
    UInt32 period;

    mixerSync(sn76489->mixer);

	if (data & 0x80) {
		int reg = (data >> 4) & 0x07;
		sn76489->latch = reg;

		sn76489->regs[reg] = (sn76489->regs[reg] & 0x3f0) | (data & 0x0f);

		switch (reg) {
		case 0:
		case 2:
		case 4:
			period = sn76489->regs[reg];
            sn76489->toneStep[reg >> 1] = period > 0 ? BASE_PHASE_STEP / period : 1 << 31;

			if (reg == 4 && (sn76489->regs[6] & 0x03) == 0x03) {
			    period = sn76489->regs[4] * 16;
                sn76489->toneStep[3] = period > 0 ? BASE_PHASE_STEP / period : 1 << 31;
			}
            
			break;

		case 1:
		case 3:
		case 5:
		case 7:
            sn76489->ampVolume[reg >> 1] = data & 0x0f;
			break;

		case 6:
			if ((sn76489->regs[6] & 0x03) == 0x03) {
			    period = sn76489->regs[4] * 16;
                sn76489->toneStep[3] = period > 0 ? BASE_PHASE_STEP / period : 1 << 31;
			}
            else {
		        period = 256 << (sn76489->regs[6] & 0x03);
                sn76489->toneStep[3] = period > 0 ? BASE_PHASE_STEP / period : 1 << 31;
            }

//			sn76489->noiseRand = 0x0f35;
			sn76489->noiseRand = 0x4000;
			break;
		}
	}
	else {
		int reg = sn76489->latch;

		switch (reg) {
		case 0:
		case 2:
		case 4:
			sn76489->regs[reg] = (sn76489->regs[reg] & 0x0f) | ((data & 0x3f) << 4);
            
			period = sn76489->regs[reg];
            sn76489->toneStep[reg >> 1] = period > 0 ? BASE_PHASE_STEP / period : 1 << 31;

            if (reg == 4 && (sn76489->regs[6] & 0x03) == 0x03) {
			    period = sn76489->regs[4] * 16;
                sn76489->toneStep[3] = period > 0 ? BASE_PHASE_STEP / period : 1 << 31;
			}
			break;
		}
	}
}

static Int32* sn76489Sync(void* ref, UInt32 count)
{
    SN76489* sn76489 = (SN76489*)ref;
    Int32   channel;
    UInt32  index;

    for (index = 0; index < count; index++) {
        Int32 sampleVolume = 0;

        UInt32 phaseStep = sn76489->toneStep[3];
        UInt32 tonePhase = sn76489->tonePhase[3];
        UInt32 tone = 0;
        Int32  count = 16;
        
        while (count--) {
            tonePhase += phaseStep;
            while (tonePhase >> 28) {
                tonePhase -= 1 << 28;
//                if (sn76489->noiseRand & 1) {
//                    sn76489->noiseRand ^= (sn76489->regs[6] & 0x04) ? 0x14002 : 0x8000;
//                }
//                sn76489->noiseRand >>= 1;
                sn76489->noiseRand = (sn76489->noiseRand >> 1) | 
                    ((sn76489->regs[6] & 0x04) ? 
                        ((sn76489->noiseRand ^ (sn76489->noiseRand >> 1)) & 1) << 14 : 
                        (sn76489->noiseRand & 1) << 14);
            }
            tone += sn76489->noiseRand & 1;
        }

        /* Store phase */
        sn76489->tonePhase[3] = tonePhase;

        /* Amplify sample using either envelope volume or channel volume */
        sampleVolume += (Int16)tone * voltTable[sn76489->ampVolume[3]] / 16;

        /* Calculate and add channel samples to buffer */
        for (channel = 0; channel < 3; channel++) {
            UInt32 phaseStep = sn76489->toneStep[channel];
            UInt32 tonePhase = sn76489->tonePhase[channel];
            UInt32 tone = 0;
            Int32  count = 16;
            
            /* Perform 16x oversampling */
            while (count--) {
                /* Update phase of tone */
                tonePhase += phaseStep;
     
                /* Calculate if tone is on or off */
                tone += tonePhase >> 31;
            }

            /* Store phase */
            sn76489->tonePhase[channel] = tonePhase;

            /* Amplify sample using either envelope volume or channel volume */
            sampleVolume += (Int16)tone * voltTable[sn76489->ampVolume[channel]] / 16;
        }

        /* Perform DC offset filtering */
        sn76489->ctrlVolume = sampleVolume - sn76489->oldSampleVolume + 0x3fe7 * sn76489->ctrlVolume / 0x4000;
        sn76489->oldSampleVolume = sampleVolume;

        /* Perform simple 1 pole low pass IIR filtering */
        sn76489->daVolume += 2 * (sn76489->ctrlVolume - sn76489->daVolume) / 3;
        
        /* Store calclulated sample value */
        sn76489->buffer[index] = 9 * sn76489->daVolume;
    }

    return sn76489->buffer;
}


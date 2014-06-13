/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Emulator/Emulator.h,v $
**
** $Revision: 1.9 $
**
** $Date: 2006/06/09 20:30:01 $
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
#ifndef EMULATOR_H
#define EMULATOR_H

#include "MsxTypes.h"
#include "Properties.h"
#include "AudioMixer.h"

typedef enum { EMU_RUNNING, EMU_PAUSED, EMU_STOPPED, EMU_SUSPENDED, EMU_STEP } EmuState;

void emulatorInit(Properties* properties, Mixer* mixer);
void emulatorExit();

void emuEnableSynchronousUpdate(int enable);

void emulatorSetFrequency(int logFrequency, int* frequency);
void emulatorRestartSound();
void emulatorSuspend();
void emulatorResume();
void emulatorDoResume();
void emulatorRestart();
void emulatorStart(const char* stateName);
void emulatorStop();
void emulatorSetMaxSpeed(int enable);
int  emulatorGetMaxSpeed();
int emulatorGetCpuOverflow();
int emulatorGetSyncPeriod();
EmuState emulatorGetState();
void emulatorSetState(EmuState state);
void emulatorRunOne();
UInt32 emulatorGetCpuSpeed();
UInt32 emulatorGetCpuUsage();
void emulatorResetMixer();
int emulatorGetCurrentScreenMode();

#endif


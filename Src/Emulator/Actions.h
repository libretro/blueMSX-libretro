/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Emulator/Actions.h,v $
**
** $Revision: 1.34 $
**
** $Date: 2007-03-24 05:20:32 $
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
#ifndef ACTIONS_H
#define ACTIONS_H

#include "Properties.h"
#include "VideoRender.h"
#include "AudioMixer.h"

void actionInit(Video* video, Properties* properties, Mixer* mixer);

void actionCartInsert(int cartNo);
void actionCartRemove(int cartNo);
void actionDiskInsert(int diskNo);
void actionDiskInsertDir(int diskNo);
void actionDiskInsertNew(int diskNo);
void actionDiskRemove(int diskNo);

void actionQuit();
void actionCartInsert1();
void actionCartInsert2();
void actionEmuTogglePause();
void actionEmuStep();
void actionEmuStepBack();
void actionDiskInsertA();
void actionDiskInsertB();
void actionDiskDirInsertA();
void actionDiskDirInsertB();
void actionStartPlayReverse();
void actionStopPlayReverse();
void actionDiskQuickChange();
void actionEmuResetSoft();
void actionEmuResetHard();
void actionEmuResetClean();
void actionDiskRemoveA();
void actionDiskRemoveB();
void actionCartRemove1();
void actionCartRemove2();


void actionPrinterForceFormFeed();

void actionSetCartAutoReset(int value);
void actionSetDiskAutoResetA(int value);
void actionSetCasAutoRewind(int value);
void actionSetSpriteEnable(int value);
void actionSetMsxAudioSwitch(int value);
void actionSetFdcTiming(int value);
void actionSetNoSpriteLimits(int value);
void actionSetFrontSwitch(int value);
void actionSetCasReadonly(int value);

void actionVideoEnableMon1(int value);
void actionVideoEnableMon2(int value);
void actionVideoEnableMon3(int value);

void actionRenshaSetLevel(int value);

#endif


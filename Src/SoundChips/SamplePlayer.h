/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/SamplePlayer.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/06/13 17:40:07 $
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
#ifndef SAMPLE_PLAYER_H
#define SAMPLE_PLAYER_H

#include <stdio.h>

#include "MsxTypes.h"
#include "AudioMixer.h"

/* Type definitions */
typedef struct SamplePlayer SamplePlayer;

/* Constructor and destructor */
SamplePlayer* samplePlayerCreate(Mixer* mixer);
void samplePlayerDestroy(SamplePlayer* samplePlayer);
void samplePlayerReset(SamplePlayer* samplePlayer);

/* Register read/write methods */
void samplePlayerWrite(SamplePlayer* samplePlayer, 
                       const Int16* attackBuffer, UInt32 attackBufferSize, 
                       const Int16* loopBuffer, UInt32 loopBufferSize);
void samplePlayerStopAfter(SamplePlayer* samplePlayer, int loops);
int samplePlayerIsIdle(SamplePlayer* samplePlayer);
int samplePlayerIsLooping(SamplePlayer* samplePlayer);

#endif


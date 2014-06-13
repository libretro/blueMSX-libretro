/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/YM2413.h,v $
**
** $Revision: 1.6 $
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
#ifndef YM2413_H
#define YM2413_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MsxTypes.h"
#include "AudioMixer.h"
#include "DebugDeviceManager.h"

/* Type definitions */
typedef struct YM_2413 YM_2413;

/* Constructor and destructor */
YM_2413* ym2413Create(Mixer* mixer);
void ym2413Destroy(YM_2413* ym2413);
void ym2413WriteAddress(YM_2413* ym2413, UInt8 address);
void ym2413WriteData(YM_2413* ym2413, UInt8 data);
void ym2413Reset(YM_2413* ref);
void ym2413SaveState(YM_2413* ref);
void ym2413LoadState(YM_2413* ref);
void ym2413GetDebugInfo(YM_2413* ym2413, DbgDevice* dbgDevice);

#ifdef __cplusplus
}
#endif

#endif


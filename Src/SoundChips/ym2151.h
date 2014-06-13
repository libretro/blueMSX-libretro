/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/ym2151.h,v $
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
#ifndef YM2151_H
#define YM2151_H

#include "MsxTypes.h"
#include "AudioMixer.h"
#include "DebugDeviceManager.h"

/* Type definitions */
typedef struct YM2151 YM2151;

/* Constructor and destructor */
YM2151* ym2151Create(Mixer* mixer);
void ym2151Destroy(YM2151* ym2151);
void ym2151Reset(YM2151* ym2151);
void ym2151LoadState(YM2151* ym2151);
void ym2151SaveState(YM2151* ym2151);
UInt8 ym2151Peek(YM2151* ym2151, UInt16 ioPort);
UInt8 ym2151Read(YM2151* ym2151, UInt16 ioPort);
void ym2151Write(YM2151* ym2151, UInt16 ioPort, UInt8 value);
void ym2151GetDebugInfo(YM2151* ym2151, DbgDevice* dbgDevice);

#endif

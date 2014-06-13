/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/SCC.h,v $
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
#ifndef SCC_H
#define SCC_H

#include <stdio.h>

#include "MsxTypes.h"
#include "AudioMixer.h"
#include "DebugDeviceManager.h"

/* Type definitions */
typedef struct SCC SCC;

typedef enum { SCC_NONE = 0, SCC_REAL, SCC_COMPATIBLE, SCC_PLUS } SccMode;

/* Constructor and destructor */
SCC* sccCreate(Mixer* mixer);
void sccDestroy(SCC* scc);
void sccReset(SCC* scc);
void sccSetMode(SCC* scc, SccMode newMode);

/* Register read/write methods */
UInt8 sccRead(SCC* scc, UInt8 address);
UInt8 sccPeek(SCC* scc, UInt8 address);
void sccWrite(SCC* scc, UInt8 address, UInt8 value);

void sccGetDebugInfo(SCC* scc, DbgDevice* dbgDevice);

void sccLoadState(SCC* scc);
void sccSaveState(SCC* scc);

#endif


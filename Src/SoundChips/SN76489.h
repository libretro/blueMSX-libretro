/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/SN76489.h,v $
**
** $Revision: 1.3 $
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
#ifndef SN76489_H
#define SN76489_H

#include "MsxTypes.h"
#include "AudioMixer.h"

/* Type definitions */
typedef struct SN76489 SN76489;

/* Constructor and destructor */
SN76489* sn76489Create(Mixer* mixer);
void sn76489Destroy(SN76489* sn76489);

/* Reset chip */
void sn76489Reset(SN76489* sn76489);

/* Register read/write methods */
void sn76489WriteData(SN76489* sn76489, UInt16 port, UInt8 data);

void sn76489LoadState(SN76489* sn76489);
void sn76489SaveState(SN76489* sn76489);

#endif


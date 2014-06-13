/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/VLM5030.h,v $
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
#ifndef VLM5030_H
#define VLM5030_H

#include "MsxTypes.h"
#include "AudioMixer.h"

/* Type definitions */
typedef struct VLM5030 VLM5030;

/* Constructor and destructor */
VLM5030* vlm5030Create(Mixer* mixer, UInt8* voiceData, int length);
void vlm5030Destroy(VLM5030* vlm5030);
void vlm5030Reset(VLM5030* vlm5030);
void vlm5030LoadState(VLM5030* vlm5030);
void vlm5030SaveState(VLM5030* vlm5030);
UInt8 vlm5030Peek(VLM5030* vlm5030, UInt16 ioPort);
UInt8 vlm5030Read(VLM5030* vlm5030, UInt16 ioPort);
void vlm5030Write(VLM5030* vlm5030, UInt16 ioPort, UInt8 value);

#endif

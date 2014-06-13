/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/Y8950.h,v $
**
** $Revision: 1.8 $
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
#ifndef Y8950_H
#define Y8950_H

#include "MsxTypes.h"
#include "AudioMixer.h"
#include "DebugDeviceManager.h"

/* Type definitions */
typedef struct Y8950 Y8950;

/* Constructor and destructor */
Y8950* y8950Create(Mixer* mixer);
void y8950Destroy(Y8950* y8950);
void y8950Reset(Y8950* y8950);
void y8950LoadState(Y8950* y8950);
void y8950SaveState(Y8950* y8950);
UInt8 y8950Peek(Y8950* y8950, UInt16 ioPort);
UInt8 y8950Read(Y8950* y8950, UInt16 ioPort);
void y8950Write(Y8950* y8950, UInt16 ioPort, UInt8 value);
void y8950GetDebugInfo(Y8950* y8950, DbgDevice* dbgDevice);

#endif

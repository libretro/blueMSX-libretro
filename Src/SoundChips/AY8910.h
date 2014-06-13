/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/SoundChips/AY8910.h,v $
**
** $Revision: 1.7 $
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
#ifndef AY8910_H
#define AY8910_H

#include "MsxTypes.h"
#include "AudioMixer.h"

/* Type definitions */
typedef struct AY8910 AY8910;

typedef enum { AY8910_MSX, AY8910_SVI } Ay8910Connector;

typedef enum { PSGTYPE_AY8910, PSGTYPE_YM2149, PSGTYPE_SN76489 } PsgType;

/* Constructor and destructor */
AY8910* ay8910Create(Mixer* mixer, Ay8910Connector connector, PsgType type);
void ay8910Destroy(AY8910* ay8910);

/* Reset chip */
void ay8910Reset(AY8910* ay8910);

/* Register read/write methods */
void ay8910WriteAddress(AY8910* ay8910, UInt16 ioPort, UInt8 address);
UInt8 ay8910PeekData(AY8910* ay8910, UInt16 ioPort);
UInt8 ay8910ReadData(AY8910* ay8910, UInt16 ioPort);
void ay8910WriteData(AY8910* ay8910, UInt16 ioPort, UInt8 data);

typedef UInt8 (*AY8910ReadCb)(void*, UInt16);
typedef void (*AY8910WriteCb)(void*, UInt16, UInt8);

void ay8910SetIoPort(AY8910* ay8910, AY8910ReadCb readCb, AY8910ReadCb pollCb, AY8910WriteCb writeCb, void* arg);

void ay8910LoadState(AY8910* ay8910);
void ay8910SaveState(AY8910* ay8910);

#endif


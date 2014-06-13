/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/FdcAudio.h,v $
**
** $Revision: 1.2 $
**
** $Date: 2006/06/13 17:13:27 $
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
#ifndef FDC_AUDIO_H
#define FDC_AUDIO_H

#include "MsxTypes.h"

typedef enum { FA_WESTERN_DIGITAL, FA_PANASONIC } FdcAudioType;

typedef struct FdcAudio FdcAudio;

FdcAudio* fdcAudioCreate(FdcAudioType type);
void fdcAudioDestroy(FdcAudio* fdcAudio);
void fdcAudioReset(FdcAudio* fdcAudio);
void fdcAudioSetMotor(FdcAudio* fdcAudio, int motorOn);
void fdcAudioSetReadWrite(FdcAudio* fdcAudio);

#endif


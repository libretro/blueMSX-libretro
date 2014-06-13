/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/GameReader.h,v $
**
** $Revision: 1.2 $
**
** $Date: 2006/03/16 07:04:36 $
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
#ifndef GAMER_READER_H
#define GAMER_READER_H

#include "MsxTypes.h"

typedef void* GrHandle;

int gameReaderSupported();

GrHandle* gameReaderCreate(int slot);
void gameReaderDestroy(GrHandle* grHandle);

int gameReaderRead(GrHandle* grHandle, UInt16 address, void* buffer, int length);
int gameReaderWrite(GrHandle* grHandle, UInt16 address, void* buffer, int length);

int gameReaderReadIo(GrHandle* grHandle, UInt16 port, UInt8* value);
int gameReaderWriteIo(GrHandle* grHandle, UInt16 port, UInt8 value);

#endif

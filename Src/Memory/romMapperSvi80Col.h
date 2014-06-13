/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/romMapperSvi80Col.h,v $
**
** $Revision: 1.3 $
**
** $Date: 2006/06/13 17:13:28 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik, Tomas Karlsson
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

#ifndef SVI80_COL_H
#define SVI80_COL_H

#include "MsxTypes.h"

void svi80colMemWrite(UInt16 address, UInt8 value);
UInt8 svi80colMemRead(UInt16 address);
int svi80colMemBankCtrlStatus(void);

int romMapperSvi80ColCreate(int frameRate, UInt8* romData, int size);

#endif

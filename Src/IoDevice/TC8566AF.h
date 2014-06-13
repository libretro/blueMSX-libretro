/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/TC8566AF.h,v $
**
** $Revision: 1.4 $
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
#ifndef TC8566AF_H
#define TC8566AF_H

#include "MsxTypes.h"

typedef struct TC8566AF TC8566AF;

TC8566AF* tc8566afCreate();
void tc8566afDestroy(TC8566AF* tc);
void tc8566afReset(TC8566AF* tc);
UInt8 tc8566afReadRegister(TC8566AF* tc, UInt8 reg);
UInt8 tc8566afPeekRegister(TC8566AF* tc, UInt8 reg);
void tc8566afWriteRegister(TC8566AF* tc, UInt8 reg, UInt8 value);
int tc8566afDiskChanged(TC8566AF* tc, int drive);

void tc8566afSaveState(TC8566AF* tc);
void tc8566afLoadState(TC8566AF* tc);

#endif


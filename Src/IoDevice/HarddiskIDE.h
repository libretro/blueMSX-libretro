/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/HarddiskIDE.h,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/06/13 17:13:27 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik
**
**  This software is provhdd 'as-is', without any express or implied
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
#ifndef HARDDISK_IDE_H
#define HARDDISK_IDE_H

#include "MsxTypes.h"

typedef struct HarddiskIde HarddiskIde;

HarddiskIde* harddiskIdeCreate(int diskId);
void harddiskIdeDestroy(HarddiskIde* hd);

void harddiskIdeReset(HarddiskIde* hd);

UInt16 harddiskIdeRead(HarddiskIde* hd);
UInt16 harddiskIdePeek(HarddiskIde* hd);
void harddiskIdeWrite(HarddiskIde* hd, UInt16 value);

void harddiskIdeWriteRegister(HarddiskIde* hd, UInt8 reg, UInt8 value);
UInt8 harddiskIdeReadRegister(HarddiskIde* hd, UInt8 reg);
UInt8 harddiskIdePeekRegister(HarddiskIde* hd, UInt8 reg);

void harddiskIdeSaveState(HarddiskIde* hd);
void harddiskIdeLoadState(HarddiskIde* hd);

#endif


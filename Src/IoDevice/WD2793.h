/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/WD2793.h,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/30 00:52:53 $
**
** Based on the Mircosol FDC emulation in BRMSX by Ricardo Bittencourt.
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
#ifndef WD2793_H
#define WD2793_H

#include "MsxTypes.h"

typedef struct WD2793 WD2793;

WD2793* wd2793Create();
void    wd2793Destroy(WD2793* tc);
void    wd2793Reset(WD2793* tc);

void    wd2793LoadState(WD2793* tc);
void    wd2793SaveState(WD2793* tc);

int     wd2793GetSide(WD2793* wd);
void    wd2793SetSide(WD2793* wd, int side);
int     wd2793GetDrive(WD2793* wd);
void    wd2793SetDrive(WD2793* wd, int drive);
void    wd2793SetMotor(WD2793* wd, int drive);
int     wd2793DiskChanged(WD2793* wd, int drive);
void    wd2793SetCommandReg(WD2793* wd, UInt8 value);
UInt8   wd2793PeekStatusReg(WD2793* wd);
UInt8   wd2793GetStatusReg(WD2793* wd);
void    wd2793SetDataReg(WD2793* wd, UInt8 value);
UInt8   wd2793PeekDataReg(WD2793* wd);
UInt8   wd2793GetDataReg(WD2793* wd);
UInt8   wd2793PeekSectorReg(WD2793* wd);
UInt8   wd2793GetSectorReg(WD2793* wd);
void    wd2793SetSectorReg(WD2793* wd, UInt8 value);
UInt8   wd2793PeekTrackReg(WD2793* wd);
UInt8   wd2793GetTrackReg(WD2793* wd);
void    wd2793SetTrackReg(WD2793* wd, UInt8 value);
int     wd2793PeekIrq(WD2793* wd);
int     wd2793GetIrq(WD2793* wd);
int     wd2793PeekDataRequest(WD2793* wd);
int     wd2793GetDataRequest(WD2793* wd);
void    wd2793SetSide(WD2793* wd, int side);
void    wd2793SetDensity(WD2793* wd, int density);

#endif


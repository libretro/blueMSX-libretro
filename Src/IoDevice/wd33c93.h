/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/wd33c93.h,v $
**
** $Revision: 1.3 $
**
** $Date: 2006/06/13 17:13:27 $
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
#ifndef WD33C93_H
#define WD33C93_H

#include "MsxTypes.h"

typedef struct WD33C93 WD33C93;

typedef struct {
    UInt32 (*execCommand)(void*, UInt8, UInt8);
    void   (*read)(void*, void*, UInt32);
    void   (*write)(void*, void*, UInt32);
} IoDeviceCallbacks;

WD33C93* wd33c93Create();
void     wd33c93Destroy(WD33C93* wd33c93);

void     wd33c93ReadData(WD33C93* wd33c93, int bytes, UInt8* data);
void     wd33c93WriteData(WD33C93* wd33c93, int bytes, UInt8* data);
void*    wd33c93GetDevice(WD33C93* wd33c93, int id);
int      wd33c93SetDevice(WD33C93* wd33c93, int id, IoDeviceCallbacks* cb, void* ref);
void     wd33c93ClearDma(WD33C93* wd33c93);
int      wd33c93GetDmaCount(WD33C93* wd33c93);
UInt8    wd33c93Read(WD33C93* wd33c93, int offset);
void     wd33c93Write(WD33C93* wd33c93, int offset, unsigned char data);

void     wd33c93LoadState(WD33C93* wd33c93);
void     wd33c93SaveState(WD33C93* wd33c93);

#endif

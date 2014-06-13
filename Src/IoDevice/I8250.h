/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/I8250.h,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/13 17:13:27 $
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
#ifndef I8250_H
#define I8250_H

#include "MsxTypes.h"

typedef int  (*I8250Transmit) (void*, UInt8);
typedef int  (*I8250Signal) (void*);
typedef void (*I8250Set) (void*, int);
typedef int  (*I8250Get) (void*);


typedef struct I8250 I8250;

UInt8 i8250Read(I8250* i8250, UInt16 port);
void i8250Write(I8250* i8250, UInt16 port, UInt8 value);

void i8250Receive(I8250* i8250, UInt8 value);
void i8250RxData(I8250* uart, UInt8 value);

void i8250LoadState(I8250* i8250);
void i8250SaveState(I8250* i8250);

void i8250Reset(I8250* i8250);
void i8250Destroy(I8250* i8250); 

I8250* i8250Create(UInt32 frequency, I8250Transmit transmit,    I8250Signal   signal,
                   I8250Set      setDataBits, I8250Set      setStopBits,
                   I8250Set      setParity,   I8250Set      setRxReady,
                   I8250Set      setDtr,      I8250Set      setRts,
                   I8250Get      getDtr,      I8250Get      getRts,
                   void* ref);

#endif

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/I8251.h,v $
**
** $Revision: 1.5 $
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
#ifndef I8251_H
#define I8251_H

#include "MsxTypes.h"

typedef int  (*I8251Transmit) (void*, UInt8);
typedef int  (*I8251Signal) (void*);
typedef void (*I8251Set) (void*, int);
typedef int  (*I8251Get) (void*);

typedef enum {
    I8251_STOP_INV = 0,
    I8251_STOP_1 = 2,
    I8251_STOP_15 = 3,
    I8251_STOP_2 = 4
} I8251StopBits;

typedef enum {
    I8251_PARITY_NONE,
    I8251_PARITY_EVEN,
    I8251_PARITY_ODD
} I8251Parity;

typedef struct I8251 I8251;

UInt8 i8251Peek(I8251* usart, UInt16 port);
UInt8 i8251Read(I8251* usart, UInt16 port);
void i8251Write(I8251* usart, UInt16 port, UInt8 value);

void i8251RxData(I8251* usart, UInt8 value);

void i8251LoadState(I8251* usart);
void i8251SaveState(I8251* usart);

void i8251Reset(I8251* usart);
void i8251Destroy(I8251* usart); 

I8251* i8251Create(I8251Transmit transmit,    I8251Signal   signal,
                   I8251Set      setDataBits, I8251Set      setStopBits,
                   I8251Set      setParity,   I8251Set      setRxReady,
                   I8251Set      setDtr,      I8251Set      setRts,
                   I8251Get      getDtr,      I8251Get      getRts,
                   void* ref);

#endif

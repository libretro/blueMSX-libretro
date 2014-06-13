/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/I8254.h,v $
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
#ifndef I8254_H
#define I8254_H

#include "MsxTypes.h"

typedef void (*I8254Out) (void*, int);

typedef enum { I8254_COUNTER_1, I8254_COUNTER_2, I8254_COUNTER_3 } I8254Counter;

typedef struct I8254 I8254;

UInt8 i8254Peek(I8254* i8254, UInt16 port);
UInt8 i8254Read(I8254* i8254, UInt16 port);
void i8254Write(I8254* i8254, UInt16 port, UInt8 value);

void i8254LoadState(I8254* i8254);
void i8254SaveState(I8254* i8254);

void i8254Reset(I8254* i8254);
void i8254Destroy(I8254* i8254); 

void i8254SetGate(I8254* i8254, I8254Counter counter, int state);

UInt32 i8254GetFrequency(I8254* i8254, I8254Counter counter);

I8254* i8254Create(UInt32 frequency, I8254Out out1, I8254Out out2, I8254Out out3, void* ref);

#endif

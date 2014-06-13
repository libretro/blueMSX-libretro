/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/UartIO.h,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/05/23 00:08:26 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vikl, Tomas Karlsson
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
#ifndef UART_IO_H
#define UART_IO_H

#include "MsxTypes.h"

typedef struct UartIO UartIO;

typedef enum {UART_NONE, UART_FILE, UART_HOST } UartType;

UartIO* uartIOCreate(void (*recvCallback)(UInt8));
void uartIODestroy(UartIO* uartIO);

void uartIOTransmit(UartIO* uartIO, UInt8 value);
int  uartIOGetStatus(UartIO* uartIO);

void uartIoSetType(UartType type, const char* fileName);

#endif

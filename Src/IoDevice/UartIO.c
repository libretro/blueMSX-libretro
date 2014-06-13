/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/UartIO.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/09/24 00:09:49 $
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
#include <stdlib.h>
#include <string.h>
#include "UartIO.h"
#include "ArchUart.h"
#include "DAC.h"
#include "Board.h"

typedef struct UartIO {
    UartType type;
    FILE* file;
    int  uartReady;

    void (*recvCallback)(UInt8);
};

static UartType theUartType = UART_NONE;
static UartIO* theUartIO = NULL;
static char theFileName[512] = { 0 };


static void setType(UartIO* uartIO)
{
    uartIO->type = theUartType;

    switch (uartIO->type) {
    case UART_HOST:
        uartIO->uartReady = archUartCreate(uartIO->recvCallback);
        break;
    case UART_FILE:
        uartIO->file = fopen(theFileName, "w+");
        break;
    }
}

static void removeType(UartIO* uartIO)
{
    switch (uartIO->type) {
    case UART_HOST:
        archUartDestroy();
        uartIO->uartReady = 0;
        break;
    case UART_FILE:
        fclose(uartIO->file);
        break;
    }
}

void uartIOTransmit(UartIO* uartIO, UInt8 value)
{
    switch (uartIO->type) {
    case UART_HOST:
        archUartTransmit(value);
        break;
    case UART_FILE:
        fwrite(&value, 1, 1, uartIO->file);
        break;
    }
}

int uartIOGetStatus(UartIO* uartIO) 
{
    switch (uartIO->type) {
    case UART_HOST:
        return uartIO->uartReady;
    case UART_FILE:
        return uartIO->file != NULL;
    }
    return 0;
}

UartIO* uartIOCreate(void (*recvCallback)(UInt8))
{
    UartIO* uartIO = calloc(1, sizeof(UartIO));

    setType(uartIO);

    theUartIO = uartIO;

    uartIO->recvCallback = recvCallback;

    return uartIO;
}

void uartIODestroy(UartIO* uartIO)
{
    removeType(uartIO);

    free(uartIO);

    theUartIO = NULL;
}

void uartIoSetType(UartType type, const char* fileName)
{   
    theUartType = type;

    strcpy(theFileName, fileName);
    
    if (theUartIO == NULL) {
        return;
    }

    removeType(theUartIO);
    setType(theUartIO);
}

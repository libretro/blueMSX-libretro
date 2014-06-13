/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/PrinterIO.c,v $
**
** $Revision: 1.10 $
**
** $Date: 2006/06/01 20:09:00 $
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
#include "PrinterIO.h"
#include "ArchPrinter.h"
#include "DAC.h"
#include "Board.h"
#include <stdlib.h>
#include <string.h>

typedef struct PrinterIO {
    PrinterType type;
    DAC* dac;
    FILE* file;
    int  printerReady;
};

static PrinterType thePrinterType = PRN_NONE;
static PrinterIO* thePrinterIO = NULL;
static char theFileName[512] = { 0 };


static void setType(PrinterIO* printerIO)
{
    printerIO->type = thePrinterType;

    switch (printerIO->type) {
    case PRN_HOST:
        printerIO->printerReady = archPrinterCreate();
        break;
    case PRN_FILE:
        printerIO->file = fopen(theFileName, "w+");
        break;
    case PRN_SIMPL:
        printerIO->dac = dacCreate(boardGetMixer(), DAC_MONO);
        break;
    }
}

static void removeType(PrinterIO* printerIO)
{
    switch (printerIO->type) {
    case PRN_HOST:
        archPrinterDestroy();
        printerIO->printerReady = 0;
        break;
    case PRN_FILE:
        fclose(printerIO->file);
        break;
    case PRN_SIMPL:
        dacDestroy(printerIO->dac);
        break;
    }
}

void printerIOWrite(PrinterIO* printerIO, UInt8 value)
{
    switch (printerIO->type) {
    case PRN_HOST:
        archPrinterWrite(value);
        break;
    case PRN_FILE:
        fwrite(&value, 1, 1, printerIO->file);
        break;
    case PRN_SIMPL:
        dacWrite(printerIO->dac, DAC_CH_MONO, value);
        break;
    }
}

int printerIOGetStatus(PrinterIO* printerIO) 
{
    switch (printerIO->type) {
    case PRN_HOST:
        return printerIO->printerReady;
    case PRN_FILE:
        return printerIO->file != NULL;
    case PRN_SIMPL:
        return 1;
    }
    return 0;
}

int  printerIODoStrobe(PrinterIO* printerIO)
{
    return printerIO->type != PRN_SIMPL;
}

PrinterIO* printerIOCreate(void)
{
    PrinterIO* printerIO = calloc(1, sizeof(PrinterIO));

    setType(printerIO);

    thePrinterIO = printerIO;

    return printerIO;
}

void printerIODestroy(PrinterIO* printerIO)
{
    removeType(printerIO);

    free(printerIO);

    thePrinterIO = NULL;
}

void printerIoSetType(PrinterType type, const char* fileName)
{   
    thePrinterType = type;

    strcpy(theFileName, fileName);
    
    if (thePrinterIO == NULL) {
        return;
    }

    removeType(thePrinterIO);
    setType(thePrinterIO);
}

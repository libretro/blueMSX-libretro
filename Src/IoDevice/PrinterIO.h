/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/PrinterIO.h,v $
**
** $Revision: 1.6 $
**
** $Date: 2005/09/09 18:14:15 $
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
#ifndef PRINTER_IO_H
#define PRINTER_IO_H

#include "MsxTypes.h"

typedef struct PrinterIO PrinterIO;

typedef enum {PRN_NONE, PRN_SIMPL, PRN_FILE, PRN_HOST } PrinterType;

PrinterIO* printerIOCreate(void);
void printerIODestroy(PrinterIO* printerIO);

void printerIOWrite(PrinterIO* printerIO, UInt8 value);
int  printerIOGetStatus(PrinterIO* printerIO);
int  printerIODoStrobe(PrinterIO* printerIO);

void printerIoSetType(PrinterType type, const char* fileName);

#endif

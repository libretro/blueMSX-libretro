/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/IoPort.h,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/06/13 17:13:27 $
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
#ifndef IO_PORT_H
#define IO_PORT_H

#include "MsxTypes.h"

typedef UInt8 (*IoPortRead)(void*, UInt16);
typedef void  (*IoPortWrite)(void*, UInt16, UInt8);

void ioPortRegister(int port, IoPortRead read, IoPortWrite write, void* ref);
void ioPortUnregister(int port);

void ioPortRegisterUnused(int idx, IoPortRead read, IoPortWrite write, void* ref);
void ioPortUnregisterUnused(int idx);

void ioPortRegisterSub(int subport, IoPortRead read, IoPortWrite write, void* ref);
void ioPortUnregisterSub(int subport);
int ioPortCheckSub(int subport);

void  ioPortReset();
UInt8 ioPortRead(void* ref, UInt16 port);
void  ioPortWrite(void* ref, UInt16 port, UInt8 value);

#endif

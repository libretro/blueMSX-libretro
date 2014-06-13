/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Utils/SaveState.h,v $
**
** $Revision: 1.3 $
**
** $Date: 2006/06/13 17:13:28 $
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
#ifndef SAVE_STATE_H
#define SAVE_STATE_H
 
#include "MsxTypes.h"
#include <string.h>
#include <stdio.h>

typedef struct SaveState SaveState;

void saveStateCreate(const char* fileName);

SaveState* saveStateOpenForRead(char* fileName);
SaveState* saveStateOpenForWrite(char* fileName);
void saveStateClose(SaveState* state);

UInt32 saveStateGet(SaveState* state, char* tagName, UInt32 defValue);
void saveStateSet(SaveState* state, char* tagName, UInt32 value);

void saveStateGetBuffer(SaveState* state, char* tagName, void* buffer, UInt32 length);
void saveStateSetBuffer(SaveState* state, char* tagName, void* buffer, UInt32 length);

#endif /* SAVE_STATE_H */


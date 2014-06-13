/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/Led.h,v $
**
** $Revision: 1.2 $
**
** $Date: 2004/12/06 07:54:59 $
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
#ifndef MSXLED_H
#define MSXLED_H

void ledSetAll(int enable);

void ledSetCapslock(int enable);
int  ledGetCapslock();

void ledSetKana(int enable);
int  ledGetKana();

void ledSetTurboR(int enable);
int  ledGetTurboR();

void ledSetPause(int enable);
int  ledGetPause();

void ledSetRensha(int enable);
int  ledGetRensha();

void ledSetFdd1(int enable);
int  ledGetFdd1();

void ledSetFdd2(int enable);
int  ledGetFdd2();

#endif


/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/Led.c,v $
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
#include "Led.h"

static int ledCapslock = 0;
static int ledKana     = 0;
static int ledTurboR   = 0;
static int ledPause    = 0;
static int ledRensha   = 0;
static int ledFdd1     = 0;
static int ledFdd2     = 0;

void ledSetAll(int enable) {
    enable = enable ? 1 : 0;
    
    ledCapslock = enable;
    ledKana     = enable;
    ledTurboR   = enable;
    ledPause    = enable;
    ledRensha   = enable;
    ledFdd1     = enable;
    ledFdd2     = enable;
}

void ledSetCapslock(int enable) {
    ledCapslock = enable ? 1 : 0;
}

int ledGetCapslock() {
    return ledCapslock;
}

void ledSetKana(int enable) {
    ledKana = enable ? 1 : 0;
}

int ledGetKana() {
    return ledKana;
}

void ledSetTurboR(int enable) {
    ledTurboR = enable ? 1 : 0;
}

int ledGetTurboR() {
    return ledTurboR;
}

void ledSetPause(int enable) {
    ledPause = enable ? 1 : 0;
}

int ledGetPause() {
    return ledPause;
}

void ledSetRensha(int enable) {
    ledRensha = enable ? 1 : 0;
}

int ledGetRensha() {
    return ledRensha;
}

void ledSetFdd1(int enable) {
    ledFdd1 = enable ? 1 : 0;
}

int ledGetFdd1() {
    return ledFdd1;
}

void ledSetFdd2(int enable) {
    ledFdd2 = enable ? 1 : 0;
}

int ledGetFdd2() {
    return ledFdd2;
}


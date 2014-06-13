/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Utils/TokenExtract.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/02/11 04:30:26 $
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
#include "StrcmpNoCase.h"
#include <string.h>
#include <stdlib.h>

char* extractToken(char* szLine, int argNo) {
    static char argBuf[512];
    int i;

    for (i = 0; i <= argNo; i++) {
        char* arg = argBuf;

        while (*szLine == ' ') szLine++;

        if (*szLine == 0) return NULL;

        if (*szLine == '\"') {
            szLine++;
            while (*szLine != '\"' && *szLine != 0) {
                *arg++ = *szLine++;
            }
            *arg = 0;
            if (*szLine != 0) szLine++;
        }
        else {
            do {
                *arg++ = *szLine++;
            } while (*szLine != ' ' && *szLine != '\t' && *szLine != '\r' && *szLine != '\n' && *szLine != 0);
            *arg = 0;
            if (*szLine != 0) szLine++;
        }
    }
    return argBuf;
}

char* extractTokens(char* szLine, int argNo) {
    static char argBuf[512];
    char* buf;

    argBuf[0] = 0;

    buf = extractToken(szLine, argNo++);

    while (buf != NULL) {
        strcat(argBuf, buf);
        buf = extractToken(szLine, argNo++);
        strcat(argBuf, buf != NULL ? " " : "");
    }

    return argBuf;
}

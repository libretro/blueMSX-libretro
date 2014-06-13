/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Utils/IsFileExtension.c,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/09/24 00:09:50 $
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
#include "IsFileExtension.h"
#include "StrcmpNoCase.h"
#include <string.h>
#include <stdlib.h>

int isFileExtension(const char* fileName, char* extension) {
    int flen = strlen(fileName);
    int elen = strlen(extension);

    if (elen > flen) {
        return 0;
    }

    return 0 == strcmpnocase(fileName + flen - elen, extension);
}
/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Utils/VeryTinyXpath.h,v $
**
** $Revision: 1.1 $
**
** $Date: 2005/09/22 23:04:29 $
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
#ifndef VERY_TINY_XPATH_H
#define VERY_TINY_XPATH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VtXpath VtXpath;

#define VTXPATH_INT_NOT_FOUND 0x789abcde

VtXpath* vtXpathOpenForRead(const char* fileName);
VtXpath* vtXpathOpenForWrite(const char* fileName);
void vtXpathClose(VtXpath* xpath);

int vtXpathGetInt(VtXpath* xpath, int numLevels, const char* first, ...);
const char* vtXpathGetString(VtXpath* xpath, int numLevels, const char* first, ...);

void vtXpathSetInt(VtXpath* xpath, int value, int numLevels, const char* first, ...);
void vtXpathSetString(VtXpath* xpath, const char* value, int numLevels, const char* first, ...);

#ifdef __cplusplus
}
#endif

#endif // VERY_TINY_XPATH_H
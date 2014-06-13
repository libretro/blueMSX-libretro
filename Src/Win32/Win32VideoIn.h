/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32VideoIn.h,v $
**
** $Revision: 1.1 $
**
** $Date: 2006/01/17 08:50:04 $
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
#ifndef WIN32_VIDEO_IN_H
#define WIN32_VIDEO_IN_H

#include <windows.h>
#include <MsxTypes.h>
#include "Properties.h"

void videoInInitialize(Properties* properties);
void videoInCleanup(Properties* properties);

int  videoInGetCount();
char* videoInGetName(int index);
int videoInGetActive();
void videoInSetActive(int index);
int videoInIsActive(int index);

#endif

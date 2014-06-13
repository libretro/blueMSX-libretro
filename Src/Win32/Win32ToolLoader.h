/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32ToolLoader.h,v $
**
** $Revision: 1.3 $
**
** $Date: 2005/06/21 03:22:35 $
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
#ifndef WIN32_TOOL_LOADER_H
#define WIN32_TOOL_LOADER_H

#include <windows.h>

typedef struct ToolInfo ToolInfo;

void toolLoadAll(const char* path, int languageId);
void toolUnLoadAll();

int toolGetCount();

ToolInfo* toolInfoGet(int index);
ToolInfo* toolInfoFind(char* name);

const char* toolInfoGetName(ToolInfo* toolInfo);

void toolInfoShowTool(ToolInfo* toolInfo);
void toolInfoSetLanguage(ToolInfo* toolInfo, int langId);


#endif

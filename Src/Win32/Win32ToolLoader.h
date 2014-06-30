/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32ToolLoader.h,v $
**
** $Revision: 1.6 $
**
** $Date: 2008-03-30 18:38:48 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#ifndef WIN32_TOOL_LOADER_H
#define WIN32_TOOL_LOADER_H

#include <windows.h>

typedef struct ToolInfo ToolInfo;

#ifndef NO_TOOL_SUPPORT

void toolLoadAll(const char* path, int languageId);
void toolUnLoadAll();

int toolGetCount();

ToolInfo* toolInfoGet(int index);
ToolInfo* toolInfoFind(char* name);

const char* toolInfoGetName(ToolInfo* toolInfo);

void toolInfoShowTool(ToolInfo* toolInfo);
void toolInfoSetLanguage(ToolInfo* toolInfo, int langId);

#else

static void toolLoadAll(const char* path, int languageId) {}
static void toolUnLoadAll() {}

static int toolGetCount() { return 0; }

static ToolInfo* toolInfoGet(int index) { return NULL; }
static ToolInfo* toolInfoFind(char* name) { return NULL; }

static const char* toolInfoGetName(ToolInfo* toolInfo) { return ""; }

static void toolInfoShowTool(ToolInfo* toolInfo) {}
static void toolInfoSetLanguage(ToolInfo* toolInfo, int langId) {}

#endif

#endif

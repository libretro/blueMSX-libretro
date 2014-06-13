/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Arch/ArchMenu.h,v $
**
** $Revision: 1.7 $
**
** $Date: 2005/12/20 07:07:06 $
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
#ifndef ARCH_MENU_H
#define ARCH_MENU_H

void archUpdateMenu(int show);

void archShowMenuSpecialCart1(int x, int y);
void archShowMenuSpecialCart2(int x, int y);
void archShowMenuReset(int x, int y);
void archShowMenuHelp(int x, int y);
void archShowMenuRun(int x, int y);
void archShowMenuFile(int x, int y);
void archShowMenuCart1(int x, int y);
void archShowMenuCart2(int x, int y);
void archShowMenuHarddisk(int x, int y);
void archShowMenuDiskA(int x, int y);
void archShowMenuDiskB(int x, int y);
void archShowMenuCassette(int x, int y);
void archShowMenuPrinter(int x, int y);
void archShowMenuZoom(int x, int y);
void archShowMenuOptions(int x, int y);
void archShowMenuTools(int x, int y);
void archShowMenuJoyPort1(int x, int y);
void archShowMenuJoyPort2(int x, int y);

#endif

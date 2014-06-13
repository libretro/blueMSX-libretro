/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/SlotManager.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2006/06/13 17:13:27 $
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
#ifndef SLOT_MANAGER_H
#define SLOT_MANAGER_H

#include "MsxTypes.h"


typedef UInt8 (*SlotRead)(void*, UInt16);
typedef void  (*SlotWrite)(void*, UInt16, UInt8);
typedef void  (*SlotEject)(void*);


void slotManagerCreate();
void slotManagerDestroy();

void slotManagerReset();

void slotLoadState();
void slotSaveState();

void slotWrite(void* ref, UInt16 address, UInt8 value);
UInt8 slotRead(void* ref, UInt16 address);
UInt8 slotPeek(void* ref, UInt16 address);

void slotRegister(int slot, int sslot, int startpage, int pages,
                  SlotRead readCb, SlotRead peekCb, SlotWrite writeCb, SlotEject ejectCb, void* ref);
void slotUnregister(int slot, int sslot, int startpage);

void slotRemove(int slot, int sslot);

void slotRegisterWrite0(SlotWrite writeCb, void* ref);
void slotUnregisterWrite0();

void slotSetRamSlot(int slot, int psl);
void slotMapRamPage(int slot, int sslot, int page);

void slotMapPage(int slot, int sslot, int page, UInt8* pageData, 
                 int readEnable, int writeEnable);
void slotUnmapPage(int slot, int sslot, int page);
void slotUpdatePage(int slot, int sslot, int page, UInt8* pageData, 
                    int readEnable, int writeEnable);

void slotSetSubslotted(int slot, int subslotted);

#endif

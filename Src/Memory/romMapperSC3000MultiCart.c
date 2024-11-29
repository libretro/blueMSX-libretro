/* 

Mapper for Multicard 2MB and Megacart 4MB made by the amazing team: https://sc3000-multicart.com

Multicard
=========

 Information: https://sc3000-multicart.com/section3.htm

 It is a cart with:
    - 2 x 27C801 8MBit EPROMs (2MByte ROM divided in games of 32KB)
    - 32KB SRAM
    - 74LS373 8-bit latch

 To select the 32KB block it uses the 0xE0 IO port, using the following algorithm
 (taken from: https://github.com/ocornut/meka/blob/79f06e87e4ee569845d52d2c51a0097815afe18a/meka/srcs/mappers.cpp#L78 )

    Control Byte format
        Q0..Q4 controls the A15 to A19 address lines on the EPROMs.  
               So these 5 bits let you select a 32KB logical block within one of the EPROMs.  Q0 => A15, Q1 => A16, Q2 => A17, Q3 => A18, Q4 => A19
        Q5     is not connected to anything
        Q6     ROM 0 / ROM 1 select (0 =>ROM 0, 1 => ROM 1)
        Q7     Enables / Disables latch
               If the latch is disabled, then pull up resistors ensure that block 31 in ROM 1 is selected

    so: int game_no = (v & 0x80) ? ((v & 0x1f) | ((v & 0x40) ? 0x20 : 0x00)) : 0x3F; 
    game_no is the offset between 0 and 63

    Menu is at block 63 (last block)

    I had to unregister port 0xe0 port because it was already taken.

Megacard
=========

Information: https://sc3000-multicart.com/megacart.htm

The evolution of the Multicard, with 4MB: Shifted from 2 x 8MBit 27C801 EPROMS to 2 x 16MBit 27C160 EPROMs
But instead of 64 slots of 32KB, we now have 128 slots of 32KB available.
These slots are accessed by flipping the bits on a SN74LS373 8 bit latch
to set the 6 high order bits of the 27C160 EPROMs plus another bit to select between the two 27C160 EPROMS.

 so: int game_no = (value & 0x1f) | (value & 0xc0) >> 1;

 Taken from: 
    - https://github.com/mamedev/mame/blob/f7b31084659475223a979ec8435b7dce4a63bf01/src/devices/bus/sega8/rom.cpp#L1188
    - https://www.smspower.org/forums/17499-SegaSC3000MegacartBinariesForMAMEMEKAEmulators


*/


#include "romMapperSC3000MultiCart.h"
#include "MediaDb.h"
#include "SlotManager.h"
#include "DeviceManager.h"
#include "SaveState.h"
#include "IoPort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int deviceHandle;
    UInt8* romData;
    UInt8 rom[0x8000];
    UInt8 ram[0x8000];
    int slot;
    int sslot;
    int startPage;
    UInt8 offset;
    int is_mega;
} RomMapperSC3000MultiCart;


static UInt8 read_rom(RomMapperSC3000MultiCart* rm, UInt16 address) 
{
    return rm->romData[address];
}

static UInt8 read_ram(RomMapperSC3000MultiCart* rm, UInt16 address) 
{
    return rm->ram[address];
}

static UInt8 write_ram(RomMapperSC3000MultiCart* rm, UInt16 address, UInt8 value)
{
    rm->ram[address] = value;
}


static UInt8 readIo(RomMapperSC3000MultiCart* rm, UInt16 ioPort)
{
    return rm->offset;
}


static UInt8 writeIo(RomMapperSC3000MultiCart* rm, UInt16 ioPort, UInt8 value)
{
    int game_no;

    if (rm->is_mega)
        game_no = (value & 0x1f) | (value & 0xc0) >> 1;
    else
        game_no = (value & 0x80) ? ((value & 0x1f) | ((value & 0x40) ? 0x20 : 0x00)) : 0x3F;

    if (rm->offset != game_no)
    {
        rm->offset = game_no;
        memcpy(rm->rom, rm->romData + rm->offset*32768, 32768);
    }
}


static void saveState(RomMapperSC3000MultiCart* rm)
{
    SaveState* state = saveStateOpenForWrite("mapperSC3000MultiCart");

    saveStateSet(state, "is_mega", rm->is_mega);
    saveStateSet(state, "offset", rm->offset);
    saveStateSetBuffer(state, "rom", rm->rom, 0x8000);
    saveStateSetBuffer(state, "ram", rm->ram, 0x8000);

    saveStateClose(state);
}

static void loadState(RomMapperSC3000MultiCart* rm)
{
    SaveState* state = saveStateOpenForRead("mapperSC3000MultiCart");

    rm->is_mega = saveStateGet(state, "is_mega", 0);
    rm->offset = saveStateGet(state, "offset", 63 + 64*rm->is_mega);
    saveStateGetBuffer(state, "rom", rm->rom, 0x8000);
    saveStateGetBuffer(state, "ram", rm->ram, 0x8000);

    saveStateClose(state);
}


static void destroy(RomMapperSC3000MultiCart* rm)
{
    slotUnregister(rm->slot, rm->sslot, rm->startPage);
    deviceManagerUnregister(rm->deviceHandle);
    free(rm->romData);
    free(rm);
}


int romMapperSC3000MultiCartCreate( const char* filename, UInt8* romData, 
                                    int size, int slot, int sslot, int startPage,
                                    int type) 
{
    int i = 0;
    int pages = 4;

    DeviceCallbacks callbacks = { destroy, NULL, saveState, loadState };

    RomMapperSC3000MultiCart* rm;
    rm = malloc(sizeof(RomMapperSC3000MultiCart));
    rm->deviceHandle = deviceManagerRegister(ROM_SC3000_MULTICART, &callbacks, rm);

    rm->is_mega = (type == ROM_SC3000_MEGACART);
    rm->offset = 63 + 64*rm->is_mega;

    rm->romData = malloc(size);
    memcpy(rm->romData, romData, size);
    memcpy(rm->rom, rm->romData + rm->offset*32768, 32768);
    memset(rm->ram, 0xff, sizeof(rm->ram));

    rm->slot  = slot;
    rm->sslot = sslot;
    rm->startPage  = startPage;

    slotRegister(slot, sslot, startPage, pages, NULL, NULL, NULL, destroy, rm);
    for (i = 0; i < pages; i++) {
        if (i + startPage >= 2) slot = 0;
        slotMapPage(slot, sslot, i + startPage, rm->rom + 0x2000 * i, 1, 0);
    }

    slotRegister(0, 0, 4, 4, read_ram, read_ram, write_ram, destroy, rm);

    ioPortUnregister(0xe0); /* Hack because the port is used by another... thing? SF7000?*/
    ioPortRegister(0xe0, readIo, writeIo, rm);

    return 1;

}
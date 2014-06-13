/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/wd33c93.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2006/05/31 18:05:49 $
**
** Based on the WD233C93 emulation in MESS (www.mess.org).
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik, Tomas Karlsson
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
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "wd33c93.h"
#include "Board.h"
#include "SaveState.h"

enum
{
    SBIC_myid = 0,
    SBIC_cdbsize = 0,
    SBIC_control = 1,
    SBIC_timeo = 2,
    SBIC_cdb1 = 3,
    SBIC_tsecs = 3,
    SBIC_cdb2 = 4,
    SBIC_theads = 4,
    SBIC_cdb3 = 5,
    SBIC_tcyl_hi = 5,
    SBIC_cdb4 = 6,
    SBIC_tcyl_lo = 6,
    SBIC_cdb5 = 7,
    SBIC_addr_hi = 7,
    SBIC_cdb6 = 8,
    SBIC_addr_2 = 8,
    SBIC_cdb7 = 9,
    SBIC_addr_3 = 9,
    SBIC_cdb8 = 10,     
    SBIC_addr_lo = 10,
    SBIC_cdb9 = 11,     
    SBIC_secno = 11,
    SBIC_cdb10 = 12,    
    SBIC_headno = 12,
    SBIC_cdb11 = 13,    
    SBIC_cylno_hi = 13,
    SBIC_cdb12 = 14,    
    SBIC_cylno_lo = 14,
    SBIC_tlun = 15,     
    SBIC_cmd_phase = 16,    
    SBIC_syn = 17,      
    SBIC_count_hi = 18, 
    SBIC_count_med = 19,    
    SBIC_count_lo = 20, 
    SBIC_selid = 21,
    SBIC_rselid = 22,
    SBIC_csr = 23,
    SBIC_cmd = 24,      
    SBIC_data = 25,     
    SBIC_queue_tag = 26,    
    SBIC_aux_status = 27,

    SBIC_num_registers
};

#define SBIC_MyID_Advanced  (0x08)  // in myid register, enable "Advanced Features"

#define SBIC_CSR_Reset      (0x00)  // reset
#define SBIC_CSR_Reset_Adv  (0x01)  // reset w/adv. features
#define SBIC_CSR_Selected   (0x11)  // selection complete, in initiator mode
#define SBIC_CSR_XferComplete   (0x16)  // initiator mode completed
#define SBIC_CSR_MIS_2      (0x88)

#define SBIC_AuxStat_DBR    (0x01)  // data buffer ready
#define SBIC_AuxStat_CIP    (0x10)  // command in progress, chip is busy
#define SBIC_AuxStat_BSY    (0x20)  // Level 2 command in progress
#define SBIC_AuxStat_IRQ    (0x80)  // IRQ pending

typedef struct {
    int enabled;
    IoDeviceCallbacks cb;
    void* ref;
} SCSIDev;

struct WD33C93 
{
    UInt8       latch;
    UInt8       data;
    UInt8       regs[SBIC_num_registers];
    UInt8       lastId;
    BoardTimer* timer;
    UInt32      timeout;
    int         timerRunning;
    SCSIDev     dev[8];
};

void setInt()
{
    // FIXME: Implement ? And what about clearing the int ?
}


static void wd33c93Irq(WD33C93* wd33c93, UInt32 time)
{
    wd33c93->timerRunning = 0;

    // set IRQ flag
    wd33c93->regs[ SBIC_aux_status ] = SBIC_AuxStat_DBR | SBIC_AuxStat_IRQ;
    // clear busy flags
    wd33c93->regs[ SBIC_aux_status ] &= ~(SBIC_AuxStat_CIP | SBIC_AuxStat_BSY);

    // select w/ATN and transfer
    wd33c93->regs[ SBIC_csr ] = SBIC_CSR_XferComplete;
    wd33c93->regs[ SBIC_cmd_phase ] = 0x60; // command successfully completed

    setInt();
}

static void wd33c93ImmediateIrq(WD33C93* wd33c93)
{
    wd33c93->regs[ SBIC_aux_status ] = SBIC_AuxStat_DBR | SBIC_AuxStat_IRQ;
    wd33c93->regs[ SBIC_aux_status ] &= ~(SBIC_AuxStat_CIP | SBIC_AuxStat_BSY);
    setInt();
}

void wd33c93Write(WD33C93* wd33c93, int offset, unsigned char data)
{
    switch( offset )
    {
    case 0:
        wd33c93->latch = data;
        break;
    case 1:
        wd33c93->data = data;
        if( wd33c93->latch <= 0x16 )
        {
            wd33c93->regs[ wd33c93->latch ] = data;
        }
        if( wd33c93->latch == SBIC_cmd )
        {
            wd33c93->regs[ SBIC_aux_status ] |= SBIC_AuxStat_CIP;

            switch( wd33c93->data )
            {
            case 0x00: /* Reset controller */
                // this clears all the registers
                memset(wd33c93->regs, 0, SBIC_num_registers * sizeof(UInt8));

                if( wd33c93->regs[ SBIC_myid ] & SBIC_MyID_Advanced )
                {
                    wd33c93->regs[ SBIC_csr ] = SBIC_CSR_Reset_Adv;
                }
                else
                {
                    wd33c93->regs[ SBIC_csr ] = SBIC_CSR_Reset;
                }

                wd33c93ImmediateIrq(wd33c93);
                break;
            case 0x04: /* Disconnect */
                wd33c93->regs[ SBIC_aux_status ] &= ~(SBIC_AuxStat_CIP | SBIC_AuxStat_BSY);
                break;
            case 0x06: /* Select with ATN */
                wd33c93->lastId = wd33c93->regs[ SBIC_selid ] & 7;

                wd33c93->regs[ SBIC_csr ] = 0x42;   // SEL_TimeOut
                wd33c93->regs[ SBIC_cmd_phase ] = 0x00; // no device selected

                wd33c93ClearDma(wd33c93);   // no DMA

                wd33c93ImmediateIrq(wd33c93);
                break;
            case 0x08: /* Select with ATN and transfer */
                wd33c93->lastId = wd33c93->regs[ SBIC_selid ] & 7;

                if (wd33c93->dev[wd33c93->lastId].cb.execCommand)
                {
                    UInt32 xfercount;

                    xfercount = wd33c93->dev[wd33c93->lastId].cb.execCommand(wd33c93->dev[wd33c93->lastId].ref, 0, wd33c93->regs[3]);

                    wd33c93->regs[ SBIC_count_lo ] = (UInt8)(xfercount & 0xff);
                    wd33c93->regs[ SBIC_count_med ] = (UInt8)((xfercount>>8) & 0xff);
                    wd33c93->regs[ SBIC_count_hi ] = (UInt8)((xfercount>>16) & 0xff);

                    wd33c93->regs[ SBIC_rselid ] |= wd33c93->regs[ SBIC_selid ] & 7;
                    
                    wd33c93->timeout = boardSystemTime() + boardFrequency() / 16384;
                    wd33c93->timerRunning = 1;
                    boardTimerAdd(wd33c93->timer, wd33c93->timeout);
                }
                else
                {
                    wd33c93->regs[ SBIC_csr ] = 0x42;   // SEL_TimeOut
                    wd33c93->regs[ SBIC_cmd_phase ] = 0x00; // no device selected

                    wd33c93ClearDma(wd33c93);   // no DMA

                    wd33c93ImmediateIrq(wd33c93);
                }
                break;
            default:
                break;
            }
        }

        wd33c93->latch++;
        break;
    default:
        break;
    }
}

UInt8 wd33c93Read(WD33C93* wd33c93, int offset)
{
    UInt8 rv = 0;

    switch( offset )
    {
    case 0:
        rv = wd33c93->regs[ SBIC_aux_status ];
        break;
    case 1:
        // ack IRQ on status read
        if (wd33c93->latch == SBIC_csr)
        {
            wd33c93->regs[ SBIC_aux_status ] &= ~SBIC_AuxStat_IRQ;
            setInt();
        }

        rv = wd33c93->regs[ wd33c93->latch ];
        wd33c93->latch++;
        break;
    default:
        break;
    }

    return rv;
}

WD33C93* wd33c93Create()
{
    WD33C93* wd33c93 = (WD33C93*) calloc(1, sizeof(WD33C93));

    wd33c93->timer = boardTimerCreate(wd33c93Irq, wd33c93);
    return wd33c93;
}

void  wd33c93Destroy(WD33C93* wd33c93)
{
    boardTimerDestroy(wd33c93->timer);
    free(wd33c93);
}

void  wd33c93LoadState(WD33C93* wd33c93)
{
    SaveState* state = saveStateOpenForRead("wd33c93");

    wd33c93->latch        = (UInt8)saveStateGet(state, "latch",         0);
    wd33c93->data         = (UInt8)saveStateGet(state, "data",          0);
    wd33c93->lastId       = (UInt8)saveStateGet(state, "lastId",        0);
    wd33c93->timeout      =        saveStateGet(state, "timeout",       0);
    wd33c93->timerRunning =        saveStateGet(state, "timerRunning",  0);

    saveStateGetBuffer(state, "regs", wd33c93->regs, sizeof(wd33c93->regs));

    saveStateClose(state);
    
    if (wd33c93->timerRunning) {
        boardTimerAdd(wd33c93->timer, wd33c93->timeout);
    }
}

void  wd33c93SaveState(WD33C93* wd33c93)
{
    SaveState* state = saveStateOpenForWrite("wd33c93");

    saveStateSet(state, "latch",        wd33c93->latch);
    saveStateSet(state, "data",         wd33c93->data);
    saveStateSet(state, "lastId",       wd33c93->lastId);
    saveStateGet(state, "timeout",      wd33c93->timeout);
    saveStateGet(state, "timerRunning", wd33c93->timerRunning);

    saveStateSetBuffer(state, "regs", wd33c93->regs, sizeof(wd33c93->regs));

    saveStateClose(state);
}

void wd33c93ReadData(WD33C93* wd33c93, int bytes, UInt8 *data)
{
    if (wd33c93->dev[wd33c93->lastId].cb.read) {
        wd33c93->dev[wd33c93->lastId].cb.read(wd33c93->dev[wd33c93->lastId].ref, data, bytes);
    }
}

void wd33c93WriteData(WD33C93* wd33c93, int bytes, UInt8 *data)
{
    if (wd33c93->dev[wd33c93->lastId].cb.write) {
        wd33c93->dev[wd33c93->lastId].cb.write(wd33c93->dev[wd33c93->lastId].ref, data, bytes);
    }
}

void *wd33c93GetDevice(WD33C93* wd33c93, int id)
{
    return wd33c93->dev[wd33c93->lastId].ref;
}

int wd33c93SetDevice(WD33C93* wd33c93, int id, IoDeviceCallbacks* cb, void* ref)
{
    if (wd33c93->dev[wd33c93->lastId].enabled) {
        return 0;
    }

    if (cb == NULL) {
        memset(&wd33c93->dev[wd33c93->lastId], 0, sizeof(wd33c93->dev[wd33c93->lastId]));
        return 1;
    }

    memcpy(&wd33c93->dev[wd33c93->lastId].cb, cb, sizeof(&wd33c93->dev[wd33c93->lastId].cb));
    wd33c93->dev[wd33c93->lastId].ref     = ref;
    wd33c93->dev[wd33c93->lastId].enabled = 1;
    return 1;
}

void wd33c93ClearDma(WD33C93* wd33c93)
{
    // indicate DMA completed by clearing the transfer count
    wd33c93->regs[ SBIC_count_lo ] = 0;
    wd33c93->regs[ SBIC_count_med ] = 0;
    wd33c93->regs[ SBIC_count_hi ] = 0;
}

int wd33c93GetDmaCount(WD33C93* wd33c93)
{
    return wd33c93->regs[ SBIC_count_lo ] | wd33c93->regs[ SBIC_count_med ]<<8 | wd33c93->regs[ SBIC_count_hi ]<<16;
}

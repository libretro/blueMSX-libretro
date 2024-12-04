#ifndef ROMMAPPER_SC3000_MULTICART_H
#define ROMMAPPER_SC3000_MULTICART_H

#include "MsxTypes.h"

int romMapperSC3000MultiCartCreate(
    const char* filename,
    UInt8* romData,
    int size,
    int slot,
    int sslot,
    int startPage,
    int type
);

#endif

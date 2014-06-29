#ifndef ROMMAPPER_MEGA_FLASH_ROM_SCC_PLUS_H
#define ROMMAPPER_MEGA_FLASH_ROM_SCC_PLUS_H
 
#include "MsxTypes.h"

typedef enum {
    EN_NONE = 0,
    EN_SCC,
    EN_SCCPLUS
} SccEnable;

int romMapperMegaFlashRomSccPlusCreate(const char* filename, UInt8* romData, 
                                       int size, int slot, int sslot, int startPage,
                                       SccEnable sccEnable, UInt32 writeProtectMask);

#endif

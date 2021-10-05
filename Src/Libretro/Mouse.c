#include "MsxTypes.h"
#include "ArchInput.h"


void archMouseGetState(int* dx, int* dy) 
{ 
    *dx = 0;
    *dy = 0;
}

int  archMouseGetButtonState(int checkAlways) 
{ 
   return 0;
}

void  archMouseEmuEnable(AmEnableMode mode) 
{
}

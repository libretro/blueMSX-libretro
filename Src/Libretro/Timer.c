#include "ArchTimer.h"

#ifdef PSP
//#include <pspkernel.h>
#include <psptypes.h>
#include <sys/time.h>
#include <psprtc.h>

UInt32 archGetSystemUpTime(UInt32 frequency)
{
   u64 rtc_tick;
   sceRtcGetCurrentTick(&rtc_tick);
   return (rtc_tick / frequency);
}

#else

#include <stdlib.h>
#include <sys/time.h>

UInt32 archGetSystemUpTime(UInt32 frequency)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec * frequency + tv.tv_usec / (1000000 / frequency);
}
#endif


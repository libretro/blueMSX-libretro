#include <stdint.h>
#include "ArchTimer.h"

#if defined(_WIN32) && defined(_XBOX)
#include <xtl.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

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

#elif defined(__PS3__)

#include <ppu_intrinsics.h>


UInt32 archGetSystemUpTime(UInt32 frequency)
{
   uint64_t rtc_tick = __mftb();
   return (rtc_tick / frequency);
}

#elif defined(__PS3__) && !defined(__PSL1GHT__)
#include <sys/sys_time.h>


UInt32 archGetSystemUpTime(UInt32 frequency)
{
   uint64_t rtc_tick;
   asm volatile ("mftb %0" : "=r"(rtc_tick));
   return (rtc_tick / frequency);
}

#elif defined(_WIN32)
static LONGLONG uptime_hfFrequency = 0;
static signed long long uptime_offset = 0;

UInt32 archGetSystemUpTime(UInt32 frequency)
{
   LARGE_INTEGER li;

   if (!uptime_hfFrequency) {
      if (QueryPerformanceFrequency(&li)) {
         uptime_hfFrequency = li.QuadPart;
      }
      else {
         return 0;
      }
   }

   QueryPerformanceCounter(&li);

   return (DWORD)((li.QuadPart + uptime_offset) * frequency / uptime_hfFrequency);
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


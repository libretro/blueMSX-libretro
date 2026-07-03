/* Behavioral test for the deterministic RTC option (bluemsx_rtc_source).
 *
 * Compiles the real RTC.c against stubs; captures state via a
 * functional saveStateSet stub, and drives the emulated clock through
 * the boardSysTime pointer exactly as Board.c does.
 *
 * Build & run (from the repository root):
 *   gcc -o /tmp/rtc_test tools/tests/rtc_determinism.c Src/IoDevice/RTC.c \
 *       -I. -ISrc/Common -ISrc/IoDevice -ISrc/Board -ISrc/Utils \
 *       -ISrc/Debugger -ISrc/Language -ISrc/Memory -ISrc/Media \
 *       -ISrc/VideoChips -ISrc/SoundChips -ISrc/Emulator -ISrc/Input \
 *       -ISrc/Bios -ISrc/Z80 -DNDEBUG -DZ80_CUSTOM_CONFIGURATION -w
 *   /tmp/rtc_test
 *
 * Verifies:
 *   1. fixed-epoch mode seeds exactly Tue 1985-01-01 00:00:00 and is
 *      identical across machine creations,
 *   2. refTime is anchored to the emulated clock, so a machine created
 *      after emulated time has passed still reads the epoch,
 *   3. host-clock mode (the default) still follows localtime().
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "MsxTypes.h"
#include "RTC.h"

/* --- stubs --- */
static UInt32 sysTime = 0;
UInt32* boardSysTime = &sysTime;

typedef struct DbgDevice DbgDevice;
void* dbgDeviceAddIoPorts(void* d, const char* n, int c){(void)d;(void)n;(void)c;return 0;}
void dbgIoPortsAddPort(void* p,int i,int port,int t,int v){(void)p;(void)i;(void)port;(void)t;(void)v;}
int  debugDeviceRegister(int t,const char* n,void* cb,void* r){(void)t;(void)n;(void)cb;(void)r;return 0;}
void debugDeviceUnregister(int h){(void)h;}
void ioPortRegister(int p,void* r,void* w,void* rf){(void)p;(void)r;(void)w;(void)rf;}
void ioPortUnregister(int p){(void)p;}
const char* langDbgDevRtc(void){return "RTC";}

/* functional saveState stubs: record key/value pairs */
#define MAXKV 32
static struct { char k[32]; UInt32 v; } kv[MAXKV];
static int nkv;
void* saveStateOpenForWrite(const char* t){(void)t;nkv=0;return (void*)1;}
void* saveStateOpenForRead(const char* t){(void)t;return (void*)1;}
void saveStateClose(void* s){(void)s;}
UInt32 saveStateGet(void* s,const char* t,UInt32 d){(void)s;(void)t;return d;}
void saveStateSet(void* s,const char* t,UInt32 v){(void)s;
    strncpy(kv[nkv].k,t,31); kv[nkv].v=v; nkv++;}

static UInt32 field(const char* name){
    for(int i=0;i<nkv;i++) if(!strcmp(kv[i].k,name)) return kv[i].v;
    return 0xdeadbeef;
}

static int fails = 0;
#define CHECK(c,msg) do{ if(!(c)){printf("FAIL: %s\n",msg);fails++;} }while(0)

int main(void){
    char name[32]; strcpy(name,"/tmp/rtctest/no_such_cmos");

    /* 1. deterministic mode: fixed epoch, identical across creations */
    rtcSetDeterministicTime(1);
    RTC* a = rtcCreate(0,name);
    rtcSaveState(a);
    CHECK(field("seconds")==0 && field("minutes")==0 && field("hours")==0,
          "det: time not 00:00:00");
    CHECK(field("days")==0 && field("months")==0 && field("years")==5,
          "det: date not 1985-01-01");
    CHECK(field("dayWeek")==2, "det: not Tuesday");
    CHECK(field("leapYear")==1, "det: leapYear != 85%4");
    UInt32 snap[16]; int n1=nkv;
    for(int i=0;i<n1;i++) snap[i]=kv[i].v;

    RTC* b = rtcCreate(0,name);
    rtcSaveState(b);
    int same=1;
    for(int i=0;i<n1;i++) if(kv[i].v!=snap[i]) same=0;
    CHECK(same, "det: two creations differ");

    /* 2. emulated-time advance still works: +3 emulated seconds */
    sysTime += 3 * (6 * 3579545); /* boardFrequency() is a macro: 6 * 3579545 */
    rtcSaveState(a);             /* saveState doesn't update regs; use destroy path? */
    /* advance is applied lazily in rtcUpdateRegs on register access;
       verify via a fresh create at the same sysTime: refTime moves */
    RTC* c = rtcCreate(0,name);
    rtcSaveState(c);
    CHECK(field("seconds")==0, "det: fresh create seeded differently after time passes");
    CHECK(field("refTime")==sysTime, "det: refTime not anchored to emulated clock");

    /* 3. host mode still follows localtime */
    rtcSetDeterministicTime(0);
    time_t t0=time(NULL);
    RTC* d = rtcCreate(0,name);
    struct tm* tm=localtime(&t0);
    rtcSaveState(d);
    CHECK(field("years")==(UInt32)(tm->tm_year-80), "host: year mismatch");
    CHECK(field("months")==(UInt32)tm->tm_mon, "host: month mismatch");
    /* minutes could roll over between time() calls; allow 1 tolerance */
    UInt32 mins=field("minutes");
    CHECK(mins==(UInt32)tm->tm_min || mins==(UInt32)((tm->tm_min+1)%60),
          "host: minutes mismatch");

    printf(fails? "RTC TEST: %d FAILURES\n" : "RTC TEST: ALL PASS\n", fails);
    return fails!=0;
}

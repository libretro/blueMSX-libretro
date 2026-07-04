#!/bin/sh
# Determinism test battery for bluemsx_libretro.
#
# Run from the repository root after building the core:
#   sh tools/tests/run_replay_tests.sh [frames]
#
# What it verifies (see tools/tests/replay_harness.c):
#   1. run-to-run:   two identical runs produce identical A/V digests
#   2. wall clock:   with bluemsx_rtc_source="fixed epoch", the digest is
#                    independent of the host date (checked with faketime
#                    if available); a host-clock control run at a
#                    different date must DIFFER, proving the test
#                    cartridge really reads the RTC
#   3. compiler:     a clang build of the core matches (if clang found)
#   4. architecture: an aarch64 cross build run under qemu-user matches
#                    (if aarch64-linux-gnu-gcc and qemu-aarch64 found)
#
# The test cartridge (testcart.asm/.rom) drives the VDP, PSG and RTC in
# a fixed input-independent loop; regenerate it with:
#   pasmo tools/tests/testcart.asm tools/tests/testcart.rom
#   (then pad to 16384 bytes with 0xff)

set -u
FRAMES=${1:-600}
SYS=./system/bluemsx
ROM=tools/tests/testcart.rom
HARNESS=tools/tests/replay_harness.c
FAILED=0

say()  { printf '%s\n' "$*"; }
fail() { say "FAIL: $*"; FAILED=1; }

digest() { # $1 runner-prefix  $2 core  $3 rtc-mode  [$4 faketime-date]
    if [ "${4:-}" != "" ] && command -v faketime >/dev/null; then
        $1 faketime "$4" /tmp/replay_native "$2" "$SYS" "$ROM" "$FRAMES" "--rtc=$3" 2>/dev/null | tail -1
    else
        $1 /tmp/replay_native "$2" "$SYS" "$ROM" "$FRAMES" "--rtc=$3" 2>/dev/null | tail -1
    fi
}

[ -f bluemsx_libretro.so ] || { say "build the core first (make)"; exit 2; }
cc -O2 -o /tmp/replay_native "$HARNESS" -ldl || exit 2

say "== 1. run-to-run reproducibility =="
A=$(digest "" ./bluemsx_libretro.so fixed)
B=$(digest "" ./bluemsx_libretro.so fixed)
say "  run A: $A"
say "  run B: $B"
[ "$A" = "$B" ] || fail "two identical runs differ"

if command -v faketime >/dev/null; then
    say "== 2. wall-clock independence (fixed epoch) =="
    C=$(digest "" ./bluemsx_libretro.so fixed "1999-12-31 23:59:00")
    say "  fixed @1999: $C"
    [ "$A" = "$C" ] || fail "fixed-epoch digest depends on host date"

    say "== 2b. host-clock control (must differ across dates) =="
    D=$(digest "" ./bluemsx_libretro.so host "1999-12-31 23:59:00")
    E=$(digest "" ./bluemsx_libretro.so host "2007-06-15 12:00:00")
    say "  host @1999: $D"
    say "  host @2007: $E"
    [ "$D" != "$E" ] || fail "host-clock control did not diverge (cart not reading RTC?)"
else
    say "== 2. skipped: faketime not installed =="
fi

if command -v clang >/dev/null; then
    say "== 3. cross-compiler (clang) =="
    make clean >/dev/null 2>&1
    make -j"$(nproc)" \
        CC="clang -Wno-error=incompatible-function-pointer-types -Wno-error=incompatible-pointer-types -Wno-error=int-conversion" \
        CXX=clang++ >/tmp/clang_build.log 2>&1 || fail "clang build failed"
    if [ -f bluemsx_libretro.so ]; then
        mv bluemsx_libretro.so /tmp/core_clang.so
        F=$(digest "" /tmp/core_clang.so fixed)
        say "  clang: $F"
        [ "$A" = "$F" ] || fail "clang core digest differs"
    fi
    make clean >/dev/null 2>&1
    make -j"$(nproc)" >/dev/null 2>&1   # restore native core
else
    say "== 3. skipped: clang not installed =="
fi

if command -v aarch64-linux-gnu-gcc >/dev/null && command -v qemu-aarch64 >/dev/null; then
    say "== 4. cross-architecture (aarch64 under qemu) =="
    make clean >/dev/null 2>&1
    make -j"$(nproc)" platform=unix \
        CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++ >/tmp/arm_build.log 2>&1 \
        || fail "aarch64 build failed"
    if [ -f bluemsx_libretro.so ]; then
        mv bluemsx_libretro.so /tmp/core_aarch64.so
        aarch64-linux-gnu-gcc -O2 -static -o /tmp/replay_aarch64 "$HARNESS" -ldl 2>/dev/null
        G=$(qemu-aarch64 -L /usr/aarch64-linux-gnu /tmp/replay_aarch64 \
            /tmp/core_aarch64.so "$SYS" "$ROM" "$FRAMES" --rtc=fixed 2>/dev/null | tail -1)
        say "  aarch64: $G"
        [ "$A" = "$G" ] || fail "aarch64 core digest differs"
    fi
    make clean >/dev/null 2>&1
    make -j"$(nproc)" >/dev/null 2>&1   # restore native core
else
    say "== 4. skipped: aarch64 cross toolchain / qemu-aarch64 not installed =="
fi

say "== 5. savestate round-trip (bit-exact save -> load -> run) =="
SS=$((FRAMES / 2))
R1=$(/tmp/replay_native ./bluemsx_libretro.so "$SYS" "$ROM" "$FRAMES" --rtc=fixed \
     "--savestate-test=$SS" 2>/dev/null | tail -1)
say "  plain mapper: $R1"
[ "$R1" = "SAVESTATE MATCH" ] || fail "savestate round-trip diverged (plain)"
R2=$(/tmp/replay_native ./bluemsx_libretro.so "$SYS" "$ROM" "$FRAMES" --rtc=fixed \
     --mapper=KonamiSCC "--savestate-test=$SS" 2>/dev/null | tail -1)
say "  KonamiSCC mapper (SCC audio active): $R2"
[ "$R2" = "SAVESTATE MATCH" ] || fail "savestate round-trip diverged (SCC)"

if [ "$FAILED" = 0 ]; then
    say "REPLAY TESTS: ALL PASS"
else
    say "REPLAY TESTS: FAILURES PRESENT"
fi
exit "$FAILED"

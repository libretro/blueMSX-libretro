# blueMSX-libretro

Libretro core for MSX-family and related Z80 home systems, based on
[blueMSX](http://www.bluemsx.com/).

**Emulated systems:** MSX, MSX2, MSX2+, MSX turbo R, Spectravideo
SVI-318/328, ColecoVision (and SVI-603), SEGA SG-1000, SC-3000 and
SF-7000.

## System files

The core needs the `Machines/` and `Databases/` folders inside the
frontend's **system directory** (they hold machine configurations and
the ROM database). If no system directory is set, the core looks for
them next to the loaded content instead.

The bundled **C-BIOS** machines (`MSX - C-BIOS`, `MSX2 - C-BIOS`,
`MSX2+ - C-BIOS`) include freely redistributable BIOS ROMs, so most
cartridge games run without any copyrighted system ROMs. All other
machine configurations require the corresponding BIOS ROMs inside their
`Machines/<machine name>/` folder.

## Content

Supported extensions: `rom` `ri` `mx1` `mx2` `dsk` `col` `sg` `sc`
`sf` `cas` `m3u`

* **Disks** (`.dsk`): multi-disk games can be loaded through an `.m3u`
  playlist; disks are switched at runtime with the frontend's disk
  control interface.
* **Cassettes** (`.cas`): see the *Auto Rewind Cassette* option.
* **Hard disk**: with *SunriseIDE Enable* set, disk images are attached
  as IDE hard drives instead (requires `SUNRISEIDE.rom`).

## Core options

| Option | What it does |
|---|---|
| Machine Type (Restart) | Machine to emulate: `Auto`, `MSX`, `MSX2`, `MSX2+`, `MSXturboR`, the SEGA and SVI machines, or `ColecoVision`. Any machine name from `Machines/` is also accepted (e.g. `MSX2 - C-BIOS`). |
| Cart Mapper Type (Restart) | Force a cartridge mapper for ROMs the database does not know. |
| Additional MSX Sound Cartridge (Restart) | Insert an extra sound cartridge (SCC, SCC+, Snatcher/SD-Snatcher variants) into the first free slot. |
| Sound YM2413 Enable (Restart) | Enable the MSX-MUSIC (FM) sound chip. |
| VDP Sync Type (Restart) | `Auto`, `50Hz` or `60Hz` region timing. |
| Crop Overscan | Crop overscanned frames. |
| No Sprite Limit | Lift the per-scanline sprite limit to reduce flicker. |
| Real-Time Clock Source (Restart) | `host clock` (default) seeds the emulated RTC from your computer's clock; `fixed epoch` always starts at 1985-01-01 00:00:00, making runs bit-identical for netplay, run-ahead and input replay. |
| Auto Rewind Cassette | Rewind the tape automatically. |
| I/O Sound Enable | Floppy drive access sounds. |
| SunriseIDE Enable (Restart) | Load disk images as IDE hard drives. |
| Colecovision Keyboard Mapping | Additionally use the host keyboard for the Coleco 0-9/#/* buttons (P1: number row, `-`, `=`; P2: keypad, `/`, `*`). |
| Patch Colecovision ROM Fast Boot | Patch the NTSC ColecoVision BIOS for fast (3.3 s) booting. |
| Use F12 to hard reset | F12 hard-resets and restarts emulation, like blueMSX standalone. |

## Input

Each port accepts three device types: **RetroPad** (joystick),
**RetroKeyboard** (full host keyboard passthrough for machines with a
keyboard) and **RetroPad Keyboard Map** (RetroPad buttons mapped to
keyboard keys).

RetroPad mapping:

| RetroPad | MSX / Coleco |
|---|---|
| D-Pad | Joystick directions |
| A / B | Button 1 / Button 2 |
| Y / X | Key 3 / Key 4 (Coleco 2 / 1) |
| L / R | Key 5 / Key 6 (Coleco 4 / 3) |
| L2 / R2 | Coleco 6 / 5 |
| L3 / R3 | Coleco 8 / 7 |
| Start / Select | Coleco # / * |
| Right analog | Coleco 9/0 and Super Action buttons 3/4 |

### Virtual keyboard

Machines with a keyboard (MSX, MSX2, MSX2+, MSXturboR, SVI, SC-3000,
SF-7000) include an on-screen keyboard using the standard MSX
international layout:

| Button | Action |
|---|---|
| SELECT | Toggle the virtual keyboard |
| D-Pad | Navigate between keys |
| A | Press the highlighted key; hold to repeat |
| B | Clear all sticky modifiers |

Modifier keys (SHIFT, CTRL, GRAPH, CODE, CAPS) are *sticky*: they stay
active (shown in green) until the next normal key press. CAPS is a true
toggle, like the physical key. Shifted labels are shown at the top of
each key.

## Determinism and savestates

The core contains no floating-point arithmetic and no other
platform-dependent behaviour: with the *fixed epoch* RTC option,
emulation is bit-identical across runs, operating systems, compilers
and CPU architectures, and save → load → run is bit-identical to an
uninterrupted run — the properties run-ahead, netplay rollback and
input replay (TAS) rely on. Serialized states are byte-stable, so they
can be hashed for netplay verification.

See [DETERMINISM.md](DETERMINISM.md) for how this is achieved and
verified. The test battery can be run from the repository root after
building:

```
sh tools/tests/run_replay_tests.sh
```

## Building

```
make
```

C sources must remain C89-compatible (no declarations after
statements); the buildbot still targets MSVC 2005. A quick local check:

```
make CC="gcc -Werror=declaration-after-statement"
```

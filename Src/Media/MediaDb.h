/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Media/MediaDb.h,v $
**
** $Revision: 1.23 $
**
** $Date: 2006/06/30 22:41:59 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik
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
#ifndef MEDIA_DB_H
#define MEDIA_DB_H

typedef int RomType;
enum  {
    ROM_UNKNOWN     = 0,
    ROM_STANDARD    = 1,
    ROM_MSXDOS2     = 2,
    ROM_KONAMI5     = 3,
    ROM_KONAMI4     = 4,
    ROM_ASCII8      = 5,
    ROM_ASCII16     = 6,
    ROM_GAMEMASTER2 = 7,
    ROM_ASCII8SRAM  = 8,
    ROM_ASCII16SRAM = 9,
    ROM_RTYPE       = 10,
    ROM_CROSSBLAIM  = 11,
    ROM_HARRYFOX    = 12,
    ROM_KOREAN80    = 13,
    ROM_KOREAN126   = 14,
    ROM_SCCEXTENDED = 15,
    ROM_FMPAC       = 16,
    ROM_KONAMI4NF   = 17,
    ROM_ASCII16NF   = 18,
    ROM_PLAIN       = 19,
    ROM_NORMAL      = 20,
    ROM_DISKPATCH   = 21,
    RAM_MAPPER      = 22,
    RAM_NORMAL      = 23,
    ROM_KANJI       = 24,
    ROM_HOLYQURAN   = 25,
    SRAM_MATSUCHITA = 26,
    ROM_PANASONIC16 = 27,
    ROM_BUNSETU     = 28,
    ROM_JISYO       = 29,
    ROM_KANJI12     = 30,
    ROM_NATIONAL    = 31,
    SRAM_S1985      = 32,
    ROM_F4DEVICE    = 33,
    ROM_F4INVERTED  = 34,
    ROM_KOEI        = 38,
    ROM_BASIC       = 39,
    ROM_HALNOTE     = 40,
    ROM_LODERUNNER  = 41,
    ROM_0x4000      = 42,
    ROM_PAC         = 43,
    ROM_MEGARAM     = 44,
    ROM_MEGARAM128  = 45,
    ROM_MEGARAM256  = 46,
    ROM_MEGARAM512  = 47,
    ROM_MEGARAM768  = 48,
    ROM_MEGARAM2M   = 49,
    ROM_MSXAUDIO    = 50,
    ROM_KOREAN90    = 51,
    ROM_SNATCHER    = 52,
    ROM_SDSNATCHER  = 53,
    ROM_SCCMIRRORED = 54,
    ROM_SCC         = 55,
    ROM_SCCPLUS     = 56,
    ROM_TC8566AF    = 57,
    ROM_S1990       = 58,
    ROM_TURBORTIMER = 59,
    ROM_TURBORPCM   = 60,
    ROM_KONAMISYNTH = 61,
    ROM_MAJUTSUSHI  = 62,
    ROM_MICROSOL    = 63,
    ROM_NATIONALFDC = 64,
    ROM_PHILIPSFDC  = 65,
    ROM_CASPATCH    = 66,
    ROM_SVI738FDC   = 67,
    ROM_PANASONIC32 = 68,
    ROM_EXTRAM      = 69,
    ROM_EXTRAM512KB = 70,
    ROM_EXTRAM1MB   = 71,
    ROM_EXTRAM2MB   = 72,
    ROM_EXTRAM4MB   = 73,
    ROM_SVI328      = 74,
    ROM_SVI328FDC   = 75,
    ROM_COLECO      = 76,
    ROM_SONYHBI55   = 77,
    ROM_MSXMUSIC    = 78,
    ROM_MOONSOUND   = 79,
    ROM_MSXAUDIODEV = 80,
    ROM_V9958       = 81,
    ROM_SVI80COL    = 82,
    ROM_SVI328PRN   = 83,
    ROM_MSXPRN      = 84,
    ROM_SVI328RS232 = 85,
    ROM_0xC000      = 86,
    ROM_FMPAK       = 87,
    ROM_MSXMIDI     = 88,
    ROM_MSXRS232    = 89,
    ROM_TURBORIO    = 90,
    ROM_KONAMKBDMAS = 91,
    ROM_GAMEREADER  = 92,
    RAM_1KB_MIRRORED= 93,
    ROM_SG1000      = 94,
    ROM_SG1000CASTLE= 95,
    ROM_SUNRISEIDE  = 96,
    ROM_GIDE        = 97,
    ROM_BEERIDE     = 98,
    ROM_KONWORDPRO  = 99,
    ROM_MICROSOL80  = 100,
    ROM_NMS8280DIGI = 101,
    ROM_SONYHBIV1   = 102,
    ROM_SVI727      = 103,
    ROM_FMDAS       = 104,
    ROM_YAMAHASFG05 = 105,
    ROM_YAMAHASFG01 = 106,
    ROM_MAXROMID    = 106
};

typedef struct MediaType MediaType;
typedef struct MediaDb MediaDb;

typedef enum { FORMAT_ROM, FORMAT_DISK, FORMAT_CAS } OldFormat;

void mediaDbAddFromOldFile(MediaDb* mediaDb, const char* fileName, 
                           OldFormat format);
void mediaDbAddFromXmlFile(MediaDb* mediaDb, const char* fileName, 
                           const char* rootTag, const char* elemTag);

MediaType* mediaDbLookup(MediaDb* mediaDb, const void *buffer, int size);


void mediaDbLoad(const char* directory);

void mediaDbCreateRomdb(const char* oldFileName);
void mediaDbCreateDiskdb(const char* oldFileName);
void mediaDbCreateCasdb(const char* oldFileName);


MediaType* mediaDbLookupRom(const void *buffer, int size);
MediaType* mediaDbGuessRom(const void *buffer, int size);
MediaType* mediaDbLookupDisk(const void *buffer, int size);
MediaType* mediaDbLookupCas(const void *buffer, int size);

RomType     mediaDbGetRomType(MediaType* mediaType);
const char* mediaDbGetTitle(MediaType* mediaType);
const char* mediaDbGetYear(MediaType* mediaType);
const char* mediaDbGetCompany(MediaType* mediaType);
const char* mediaDbGetRemark(MediaType* mediaType);
const char* mediaDbGetPrettyString(MediaType* mediaType);

void mediaDbSetDefaultRomType(RomType romType);
RomType mediaDbOldStringToType(const char* romName);
const char* romTypeToString(RomType romType);
const char* romTypeToShortString(RomType romType);

int romTypeIsRom(RomType romType);
int romTypeIsMegaRom(RomType romType);
int romTypeIsMegaRam(RomType romType);
int romTypeIsFmPac(RomType romType);

#endif /*MEDIA_DB_H*/

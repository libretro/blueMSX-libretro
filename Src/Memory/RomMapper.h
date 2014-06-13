/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/RomMapper.h,v $
**
** $Revision: 1.13 $
**
** $Date: 2005/04/11 20:47:38 $
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
#ifndef ROM_MAPPER_H
#define ROM_MAPPER_H

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
    AUDIO_MOONSOUND = 35,
    AUDIO_Y8950     = 36,
    AUDIO_YM2413    = 37,
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
    ROM_MSXRS232    = 86,
    ROM_MAXROMID    = 86
};

static char* romTypeToString(RomType romType) {
    switch (romType) {    
    case ROM_STANDARD:    return "Standard";
    case ROM_MSXDOS2:     return "MSXDOS 2";
    case ROM_KONAMI5:     return "Konami 5";
    case ROM_KONAMI4:     return "Konami 4";
    case ROM_ASCII8:      return "ASCII 8";
    case ROM_ASCII16:     return "ASCII 16";
    case ROM_GAMEMASTER2: return "Game Master 2 (SRAM)";
    case ROM_ASCII8SRAM:  return "ASCII 8 (SRAM)";
    case ROM_ASCII16SRAM: return "ASCII 16 (SRAM)";
    case ROM_RTYPE:       return "R-Type";
    case ROM_CROSSBLAIM:  return "Cross Blaim";
    case ROM_HARRYFOX:    return "Harry Fox";
    case ROM_MAJUTSUSHI:  return "Konami Majutsushi";
    case ROM_KOREAN80:    return "Korean 80 in 1";
    case ROM_KOREAN126:   return "Korean 126 in 1";
    case ROM_SCC:         return "SCC";
    case ROM_SCCPLUS:     return "SCC+";
    case ROM_SNATCHER:    return "The Snatcher";
    case ROM_SDSNATCHER:  return "SD Snatcher";
    case ROM_SCCMIRRORED: return "SCC mirrored";
    case ROM_SCCEXTENDED: return "SCC extended";
    case ROM_FMPAC:       return "FMPAC (SRAM)";
    case ROM_KONAMI4NF:   return "Konami4 alt.";
    case ROM_ASCII16NF:   return "ASCII 16 alt.";
    case ROM_PLAIN:       return "Mirrored ROM";
    case ROM_NORMAL:      return "Normal ROM";
    case ROM_DISKPATCH:   return "Normal + Disk Patch";
    case ROM_CASPATCH:    return "Normal + Cassette Patch";
    case ROM_TC8566AF:    return "TC8566AF Disk Controller";
    case ROM_MICROSOL:    return "Microsol Disk Controller";
    case ROM_NATIONALFDC: return "National Disk Controller";
    case ROM_PHILIPSFDC:  return "Philips Disk Controller";
    case ROM_SVI738FDC:   return "SVI-738 Disk Controller";
    case RAM_MAPPER:      return "Mapped RAM";
    case RAM_NORMAL:      return "Normal RAM";
    case ROM_KANJI:       return "Kanji";
    case ROM_HOLYQURAN:   return "Holy Quran";
    case SRAM_MATSUCHITA: return "Matsushita SRAM";
    case ROM_PANASONIC16: return "Panasonic 16kB SRAM";
    case ROM_PANASONIC32: return "Panasonic 32kB SRAM";
    case ROM_BUNSETU:     return "Bunsetsu";
    case ROM_JISYO:       return "Jisyo";
    case ROM_KANJI12:     return "Kanji12";
    case ROM_NATIONAL:    return "National (SRAM)";
    case SRAM_S1985:      return "S1985";
    case ROM_S1990:       return "S1990";
    case ROM_F4DEVICE:    return "F4 Device Normal";
    case ROM_F4INVERTED:  return "F4 Device Inverted";
    case ROM_TURBORTIMER: return "Turbo-R Timer";
    case AUDIO_MOONSOUND: return "Moonsound Audio";
    case AUDIO_Y8950:     return "Y8950 Audio";
    case AUDIO_YM2413:    return "YM2413 Audio";
    case ROM_KOEI:        return "Koei (SRAM)";
    case ROM_BASIC:       return "Basic ROM";
    case ROM_HALNOTE:     return "Halnote";
    case ROM_LODERUNNER:  return "Lode Runner";
    case ROM_0x4000:      return "Normal 0x4000";
    case ROM_KONAMISYNTH: return "Konami Synthesizer";
    case ROM_PAC:         return "PAC (SRAM)";
    case ROM_MEGARAM:     return "MegaRAM";
    case ROM_MEGARAM128:  return "128kB MegaRAM";
    case ROM_MEGARAM256:  return "256kB MegaRAM";
    case ROM_MEGARAM512:  return "512kB MegaRAM";
    case ROM_MEGARAM768:  return "768kB MegaRAM";
    case ROM_MEGARAM2M:   return "2MB MegaRAM";
    case ROM_EXTRAM:      return "External RAM";
    case ROM_EXTRAM512KB: return "512kB External RAM";
    case ROM_EXTRAM1MB:   return "1MB External RAM";
    case ROM_EXTRAM2MB:   return "2MB External RAM";
    case ROM_EXTRAM4MB:   return "4MB External RAM";
    case ROM_MSXMUSIC:    return "MSX Music";
    case ROM_MSXAUDIO:    return "MSX Audio";
    case ROM_MSXRS232:    return "MSX RS-232";
    case ROM_MOONSOUND:   return "Moonsound";
    case ROM_KOREAN90:    return "Korean 90 in 1";
    case ROM_SVI328:      return "SVI-328 Cartridge";
    case ROM_SVI328FDC:   return "SVI-328 Disk Controller";
    case ROM_SVI328PRN:   return "SVI-328 Printer";
    case ROM_SVI328RS232: return "SVI-328 Serial Port";
    case ROM_SVI80COL:    return "SVI 80 Column Card";
    case ROM_COLECO:      return "Coleco Cartridge";
    case ROM_SONYHBI55:   return "Sony HBI-55";
    case ROM_MSXAUDIODEV: return "MSX Audio Chip";
    case ROM_MSXPRN:      return "MSX Printer";
    case ROM_TURBORPCM:   return "Turbo-R PCM Chip";

    case ROM_UNKNOWN:     return "Unknown";
    }

    return "unknown";
}

static char* romTypeToShortString(RomType romType) {
    switch (romType) {    
    case ROM_STANDARD:    return "STANDARD";
    case ROM_MSXDOS2:     return "MSXDOS2";
    case ROM_KONAMI5:     return "KONAMI5";
    case ROM_KONAMI4:     return "KONAMI4";
    case ROM_ASCII8:      return "ASCII8";
    case ROM_ASCII16:     return "ASCII16";
    case ROM_GAMEMASTER2: return "GMASTER2";
    case ROM_ASCII8SRAM:  return "ASCII8SRAM";
    case ROM_ASCII16SRAM: return "ASCII16SRAM";
    case ROM_RTYPE:       return "R-TYPE";
    case ROM_CROSSBLAIM:  return "CROSSBLAIM";
    case ROM_HARRYFOX:    return "HARRYFOX";
    case ROM_KOREAN80:    return "80IN1";
    case ROM_KOREAN126:   return "126IN1";
    case ROM_SCC:         return "SCC";
    case ROM_SCCPLUS:     return "SCC+";
    case ROM_SNATCHER:    return "SNATCHER";
    case ROM_SDSNATCHER:  return "SDSNATCHER";
    case ROM_SCCMIRRORED: return "SCCMIRRORED";
    case ROM_SCCEXTENDED: return "SCCEXTENDED";
    case ROM_FMPAC:       return "FMPAC";
    case ROM_KONAMI4NF:   return "KONAMI4ALT";
    case ROM_ASCII16NF:   return "ASCII16ALT";
    case ROM_PLAIN:       return "MIRRORED";
    case ROM_NORMAL:      return "NORMAL";
    case ROM_DISKPATCH:   return "DISKPATCH";
    case ROM_CASPATCH:    return "CASPATCH";
    case ROM_TC8566AF:    return "TC8566AF";
    case ROM_MICROSOL:    return "MICROSOL";
    case ROM_NATIONALFDC: return "NATNL FDC";
    case ROM_PHILIPSFDC:  return "PHILIPSFDC";
    case ROM_SVI738FDC:   return "SVI738 FDC";
    case RAM_MAPPER:      return "MAPPED RAM";
    case RAM_NORMAL:      return "NORMAL RAM";
    case ROM_KANJI:       return "KANJI";
    case ROM_HOLYQURAN:   return "HOLYQURAN";
    case SRAM_MATSUCHITA: return "MATSUSHITA";
    case ROM_PANASONIC16: return "PANASON 16";
    case ROM_PANASONIC32: return "PANASON 32";
    case ROM_BUNSETU:     return "BUNSETSU";
    case ROM_JISYO:       return "JISYO";
    case ROM_KANJI12:     return "KANJI12";
    case ROM_NATIONAL:    return "NATIONAL";
    case SRAM_S1985:      return "S1985";
    case ROM_S1990:       return "S1990";
    case ROM_F4DEVICE:    return "F4NORMAL";
    case ROM_F4INVERTED:  return "F4INV";
    case ROM_TURBORTIMER: return "TURBORTMR";
    case AUDIO_MOONSOUND: return "MOONSOUND";
    case AUDIO_Y8950:     return "Y8950";
    case AUDIO_YM2413:    return "YM2413";
    case ROM_KOEI:        return "KOEI";
    case ROM_BASIC:       return "BASIC";
    case ROM_HALNOTE:     return "HALNOTE";
    case ROM_LODERUNNER:  return "LODERUNNER";
    case ROM_0x4000:      return "0x4000";
    case ROM_KONAMISYNTH: return "KONSYNTH";
    case ROM_MAJUTSUSHI:  return "MAJUTSUSHI";
    case ROM_PAC:         return "PAC";
    case ROM_MEGARAM:     return "MEGARAM";
    case ROM_MEGARAM128:  return "MEGARAM128";
    case ROM_MEGARAM256:  return "MEGARAM256";
    case ROM_MEGARAM512:  return "MEGARAM512";
    case ROM_MEGARAM768:  return "MEGARAM768";
    case ROM_MEGARAM2M:   return "MEGARAM2MB";
    case ROM_EXTRAM:      return "EXTERN RAM";
    case ROM_EXTRAM512KB: return "EXTRAM 512";
    case ROM_EXTRAM1MB:   return "EXTRAM 2MB";
    case ROM_EXTRAM2MB:   return "EXTRAM 1MB";
    case ROM_EXTRAM4MB:   return "EXTRAM 4MB";
    case ROM_MSXMUSIC:    return "MSXMUSIC";
    case ROM_MSXAUDIO:    return "MSXAUDIO";
    case ROM_MSXRS232:    return "MSXRS232";
    case ROM_MOONSOUND:   return "MOONSOUND";
    case ROM_KOREAN90:    return "90IN1";
    case ROM_SVI328:      return "SVI328";
    case ROM_SVI328FDC:   return "SVI328FDC";
    case ROM_SVI328PRN:   return "SVI328PRN";
    case ROM_SVI328RS232: return "SVI328RS232";
    case ROM_SVI80COL:    return "SVI80COL";
    case ROM_COLECO:      return "COLECO";
    case ROM_SONYHBI55:   return "HBI-55";
    case ROM_MSXAUDIODEV: return "MSXAUDIO";
    case ROM_MSXPRN:      return "MSXPRN";
    case ROM_TURBORPCM:   return "TURBOR PCM";

    case ROM_UNKNOWN:     return "UNKNOWN";
    }

    return "UNKNOWN";
}

extern RomType romMapperGuessRom(const void *buffer, int size, int guess, char* extendedName);
extern RomType romMapperTypeFromString(const char* name);
extern void    romMapperSetRomdbFilename(const char* filename);
extern void    romMapperSetDiskdbFilename(const char* filename);
extern void    romMapperSetCasdbFilename(const char* filename);
extern void    romMapperSetDefaultType(RomType romType);
extern void    romMapperGetDiskInfo(const void *data, int size, char* extendedName);
extern void    romMapperGetCasInfo(const void *data, int size, char* extendedName);

#endif /*ROM_MAPPER_H*/

/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Memory/MegaromCartridge.c,v $
**
** $Revision: 1.31 $
**
** $Date: 2006/06/26 19:35:55 $
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
#include "MegaromCartridge.h"
#include "MediaDb.h"
#include "RomLoader.h"
#include "SlotManager.h"

#include "ramMapper.h"
#include "romMapperStandard.h"
#include "romMapperMsxDos2.h"
#include "romMapperKonami5.h"
#include "romMapperNormal.h"
#include "romMapperKonami4.h"
#include "romMapperASCII8.h"
#include "romMapperASCII16.h"
#include "romMapperGameMaster2.h"
#include "romMapperASCII8sram.h"
#include "romMapperASCII16sram.h"
#include "romMapperKoei.h"
#include "romMapperASCII16nf.h"
#include "romMapperKonami4nf.h"
#include "romMapperKanji.h"
#include "romMapperKanji12.h"
#include "romMapperHolyQuran.h"
#include "romMapperMegaRAM.h"
#include "romMapperMsxAudio.h"
#include "romMapperPlain.h"
#include "romMapperBasic.h"
#include "romMapperHarryFox.h"
#include "romMapperHalnote.h"
#include "romMapperRType.h"
#include "romMapperCrossBlaim.h"
#include "romMapperLodeRunner.h"
#include "romMapperKorean80.h"
#include "romMapperKorean90.h"
#include "romMapperKorean126.h"
#include "romMapperKonamiSynth.h"
#include "romMapperKonamiKeyboardMaster.h"
#include "romMapperKonamiWordPro.h"
#include "romMapperMajutsushi.h"
#include "romMapperPAC.h"
#include "romMapperFMPAC.h"
#include "romMapperFMPAK.h"
#include "romMapperSCCplus.h"
#include "romMapperDisk.h"
#include "romMapperTC8566AF.h"
#include "romMapperMicrosol.h"
#include "romMapperNationalFdc.h"
#include "romMapperPhilipsFdc.h"
#include "romMapperSvi738Fdc.h"
#include "romMapperSonyHBI55.h"
#include "romMapperMoonsound.h"
#ifdef WIN32
#include "romMapperGameReader.h"
#endif
#include "romMapperSunriseIDE.h"
#include "romMapperBeerIDE.h"
#include "romMapperGIDE.h"
#include "romMapperSg1000Castle.h"
#include "romMapperMicrosolVmx80.h"
#include "romMapperSvi727.h"
#include "romMapperSonyHBIV1.h"
#include "romMapperFmDas.h"
#include "romMapperSfg05.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    struct {
        int slot;
        int sslot;
    } cart[2];
} CartridgeInfo;


CartridgeInfo cartridgeInfo;


void cartridgeSetSlotInfo(int cartNo, int slot, int sslot) 
{
    cartridgeInfo.cart[cartNo].slot  = slot;
    cartridgeInfo.cart[cartNo].sslot = sslot;
}

void cartridgeInsert(int cartNo, RomType romType, char* cart, char* cartZip) 
{
    char* romName = cartZip != NULL ? cartZip : cart;
    UInt8* buf;
    int size;
    int slot  = cartridgeInfo.cart[cartNo].slot;
    int sslot = cartridgeInfo.cart[cartNo].sslot;

    // Delete old cartridge
    slotRemove(slot, sslot);

    if (cart == NULL) {
        return;
    }

    switch (romType) {
    case ROM_EXTRAM512KB:
        ramMapperCreate(0x80000, slot, sslot, 0, NULL, NULL);
        break;
        
    case ROM_EXTRAM1MB:
        ramMapperCreate(0x100000, slot, sslot, 0, NULL, NULL);
        break;
        
    case ROM_EXTRAM2MB:
        ramMapperCreate(0x200000, slot, sslot, 0, NULL, NULL);
        break;
        
    case ROM_EXTRAM4MB:
        ramMapperCreate(0x400000, slot, sslot, 0, NULL, NULL);
        break;

    case ROM_MEGARAM128:
        romMapperMegaRAMCreate(0x20000, slot, sslot, 0);
        break;

    case ROM_MEGARAM256:
        romMapperMegaRAMCreate(0x40000, slot, sslot, 0);
        break;

    case ROM_MEGARAM512:
        romMapperMegaRAMCreate(0x80000, slot, sslot, 0);
        break;

    case ROM_MEGARAM768:
        romMapperMegaRAMCreate(0xc0000, slot, sslot, 0);
        break;

    case ROM_MEGARAM2M:
        romMapperMegaRAMCreate(0x200000, slot, sslot, 0);
        break;

    case ROM_PAC:
        romMapperPACCreate("PacA.rom", NULL, 0, slot, sslot, 2);
        break;

#ifdef WIN32
    case ROM_GAMEREADER:
        romMapperGameReaderCreate(cartNo, slot, sslot);
        break;
#endif

    case ROM_GIDE:
        romMapperGIdeCreate(cartNo);
        break;

    case ROM_FMPAC:
        if (cart[strlen(cart) - 4] != '.') {
            buf = romLoad("Machines/Shared Roms/FMPAC.rom", "", &size);
            if (buf != NULL) {
                romMapperFMPACCreate("FmPacA.rom", buf, size, slot, sslot, 2);
                free(buf);
            }
            else {
                static UInt8 romFMPAC[0x10000] = {
                    0x41, 0x42, 0x00, 0x00, 0x82, 0x40, 0x00, 0x00, 
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x50, 0x41, 0x43, 0x32, 0x4f, 0x50, 0x4c, 0x4c
                };
                romMapperFMPACCreate("FmPacA.rom", romFMPAC, 0x10000, slot, sslot, 2);
            }
            break;
        }
        // Fall through.. 
    default:
        // Load roms for Special Carts
        if (strcmp(cart, "Sunrise IDE") == 0) {
            buf = romLoad("Machines/Shared Roms/sunriseide.rom", cartZip, &size);
            if (buf == 0) {
                romMapperSunriseIdeCreate(cartNo, romName, NULL, 0, slot, sslot, 0);
                break;
            }
        }
        // Load roms for Special Carts
        else if (strcmp(cart, "Beer IDE") == 0) {
            buf = romLoad("Machines/Shared Roms/beeride.rom", cartZip, &size);
            if (buf == 0) {
                romMapperBeerIdeCreate(cartNo, romName, NULL, 0, slot, sslot, 0);
                break;
            }
        }
        else {
            buf = romLoad(cart, cartZip, &size);
        }
        if (buf == NULL) {
            switch (romType) {
            case ROM_SNATCHER:
                romMapperSCCplusCreate(NULL, NULL, 0, slot, sslot, 2, SCC_SNATCHER);
                break;

            case ROM_SDSNATCHER:
                romMapperSCCplusCreate(NULL, NULL, 0, slot, sslot, 2, SCC_SDSNATCHER);
                break;

            case ROM_SCCMIRRORED:
                romMapperSCCplusCreate(NULL, NULL, 0, slot, sslot, 2, SCC_MIRRORED);
                break;

            case ROM_SCCEXTENDED:
                romMapperSCCplusCreate(NULL, NULL, 0, slot, sslot, 2, SCC_EXTENDED);
                break;

            case ROM_SCC:
                romMapperSCCplusCreate(romName, NULL, 0, slot, sslot, 2, SCC_EXTENDED);
                break;

            case ROM_SCCPLUS:
                romMapperSCCplusCreate(romName, NULL, 0, slot, sslot, 2, SCCP_EXTENDED);
                break;

            case ROM_SONYHBI55:
                romMapperSonyHBI55Create();
                break;
            }
            break;
        }
        
        if (romType == ROM_UNKNOWN) {
            MediaType* mediaType = mediaDbGuessRom(buf, size);
            romType =  mediaDbGetRomType(mediaType);
        }

        switch (romType) {
        case ROM_0x4000:
            romMapperNormalCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_0xC000:
            romMapperNormalCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_BASIC:
            romMapperBasicCreate(romName, buf, size, slot, sslot, 4);
            break;

        case ROM_FMDAS:
            romMapperFmDasCreate(romName, buf, size, slot, sslot, 0);
            break;

        case ROM_PLAIN:
            romMapperPlainCreate(romName, buf, size, slot, sslot, 0);
            break;

        case ROM_FMPAK:
            romMapperFMPAKCreate(romName, buf, size, slot, sslot, 0);
            break;

        case ROM_STANDARD:
            romMapperStandardCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_MSXDOS2:
            romMapperMsxDos2Create(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_KONAMI5:
            romMapperKonami5Create(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_SNATCHER:
            romMapperSCCplusCreate(NULL, buf, size, slot, sslot, 2, SCC_SNATCHER);
            break;

        case ROM_SDSNATCHER:
            romMapperSCCplusCreate(NULL, buf, size, slot, sslot, 2, SCC_SDSNATCHER);
            break;

        case ROM_SCCMIRRORED:
            romMapperSCCplusCreate(NULL, buf, size, slot, sslot, 2, SCC_MIRRORED);
            break;

        case ROM_SCCEXTENDED:
            romMapperSCCplusCreate(NULL, buf, size, slot, sslot, 2, SCC_EXTENDED);
            break;

        case ROM_SCC:
            romMapperSCCplusCreate(romName, buf, size, slot, sslot, 2, SCC_EXTENDED);
            break;

        case ROM_SCCPLUS:
            romMapperSCCplusCreate(romName, buf, size, slot, sslot, 2, SCCP_EXTENDED);
            break;

        case ROM_MOONSOUND:
            romMapperMoonsoundCreate(romName, buf, size, 640);
            break;
            
        case ROM_KONAMI4:
            romMapperKonami4Create(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_MAJUTSUSHI:
            romMapperMajutsushiCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_HOLYQURAN:
            romMapperHolyQuranCreate(romName, buf, size, slot, sslot, 2);
            break;

		case ROM_KONAMISYNTH:
            romMapperKonamiSynthCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_KONWORDPRO:
            romMapperKonamiWordProCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_KONAMKBDMAS:
            {
                // Try to load voice rom before creating the rom mapper
                char voiceName[512];
                int voiceSize = 0;
                UInt8* voiceData;
                int i;

                strcpy(voiceName, cart);
                for (i = strlen(voiceName); i > 0 && voiceName[i] != '.'; i--);
                voiceName[i] = 0;
                strcat(voiceName, "_voice.rom");
                    
                voiceData = romLoad(voiceName, NULL, &voiceSize);
                romMapperKonamiKeyboardMasterCreate(romName, buf, size, slot, sslot, 2, voiceData, voiceSize);
                if (voiceData != NULL) {
                    free(voiceData);
                }
            }
            break;
            
        case ROM_ASCII8:
            romMapperASCII8Create(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_ASCII16:
            romMapperASCII16Create(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_ASCII8SRAM:
            romMapperASCII8sramCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_ASCII16SRAM:
            romMapperASCII16sramCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_KOEI:
            romMapperKoeiCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_KONAMI4NF:
            romMapperKonami4nfCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_ASCII16NF:
            romMapperASCII16nfCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_GAMEMASTER2:
            romMapperGameMaster2Create(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_HARRYFOX:
            romMapperHarryFoxCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_HALNOTE:
            romMapperHalnoteCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_RTYPE:
            romMapperRTypeCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_LODERUNNER:
            romMapperLodeRunnerCreate(romName, buf, size, slot, sslot, 4);
            break;
            
        case ROM_CROSSBLAIM:
            romMapperCrossBlaimCreate(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_KOREAN80:
            romMapperKorean80Create(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_KOREAN90:
            romMapperKorean90Create(romName, buf, size, slot, sslot, 2);
            break;
            
        case ROM_KOREAN126:
            romMapperKorean126Create(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_MSXAUDIO:
            romMapperMsxAudioCreate(romName, buf, size, slot, sslot, 0);
            break;

        case ROM_YAMAHASFG01:
            romMapperSfg05Create(romName, buf, size, slot, sslot, 0);
            break;

        case ROM_YAMAHASFG05:
            romMapperSfg05Create(romName, buf, size, slot, sslot, 0);
            break;

        case ROM_DISKPATCH:
            romMapperDiskCreate(romName, buf, size, slot, sslot, 2);
           break;

        case ROM_TC8566AF:
            romMapperTC8566AFCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_MICROSOL:
            romMapperMicrosolCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_NATIONALFDC:
            romMapperNationalFdcCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_PHILIPSFDC:
            romMapperPhilipsFdcCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_SVI738FDC:
            romMapperSvi738FdcCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_SVI328:
            romMapperNormalCreate(romName, buf, size, 1, 0, 0);
            break;

        case ROM_COLECO:
            romMapperNormalCreate(romName, buf, size, 0, 0, 4);
            break;

        case ROM_SG1000:
            romMapperNormalCreate(romName, buf, size, 0, 0, 0);
            break;

        case ROM_SG1000CASTLE:
            romMapperSg1000CastleCreate(romName, buf, size, 0, 0, 0);
            break;

        case ROM_KANJI:
            romMapperKanjiCreate(buf, size);
            break;

        case ROM_KANJI12:
            romMapperKanji12Create(buf, size);
            break;
            
        case ROM_FMPAC:
            romMapperFMPACCreate(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_SUNRISEIDE:
            romMapperSunriseIdeCreate(cartNo, romName, buf, size, slot, sslot, 0);
            break;

        case ROM_BEERIDE:
            romMapperBeerIdeCreate(cartNo, romName, buf, size, slot, sslot, 2);
            break;

        case ROM_SONYHBIV1:
            romMapperSonyHbiV1Create(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_SVI727:
            romMapperSvi727Create(romName, buf, size, slot, sslot, 2);
            break;

        case ROM_MICROSOL80:
            {
                // Try to load character rom before creating the rom mapper
                char charName[512];
                int charSize = 0;
                UInt8* charData;
                int i;

                strcpy(charName, cart);
                for (i = strlen(charName); i > 0 && charName[i] != '.'; i--);
                charName[i] = 0;
                strcat(charName, "_char.rom");
                    
                charData = romLoad(charName, NULL, &charSize);
                romMapperMicrosolVmx80Create(romName, buf, size, slot, sslot, 2, charData, charSize);
                if (charData != NULL) {
                    free(charData);
                }
            }
            break;
        }

        free(buf);
    }
}


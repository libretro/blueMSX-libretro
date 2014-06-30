####################################################
# MakeFile and blueMSX.mak created by Roger Filipe #
#                                                  #
# 02/01/2004 - v1.0 - Initial Version              #
# 20/01/2004 - v1.1 - Improved many things         #
####################################################

# Win32VideoIn uses c language (old) version

#
# Comment out if verbose comilation is wanted
#
SILENT = @

#
# Directories
#
ROOT_DIR   = ../..
OUTPUT_DIR = objs

#
# Tools
#
CC    = $(SILENT)gcc
CXX   = $(SILENT)g++
RC    = $(SILENT)windres -O COFF
LD    = $(SILENT)g++ 
RM    = $(SILENT)-rm -f
RMDIR = $(SILENT)-rm -rf
MKDIR = $(SILENT)-mkdir
ECHO  = @echo

#
# Flags
#
CFLAGS   = -g -w -O2 -D_MBCS -D__WINCRYPT_H__ -DWIN32 -D_WINDOWS -DWINDOWS -DLSB_FIRST -DNARROW -DSOUND -DDISK -DFMSX -DNO_ASM -D_WIN32_IE=0x0500 -DPROPERTIES_DEFAULTS_ALT_1 -DPROPERTIES_NO_REGISTRY -DPROPERTIES_LOCAL_INI_FILE -DMINGW
# Optimization sample
#CFLAGS   = -w -O2 -march=pentium3 -mmmx -msse -mfpmath=sse -DNDEBUG -D_MBCS -D__WINCRYPT_H__ -DWIN32 -D_WINDOWS -DWINDOWS -DLSB_FIRST -DNARROW -DSOUND -DDISK -DFMSX -D_WIN32_IE=0x0500 -DPROPERTIES_DEFAULTS_ALT_1 -DPROPERTIES_NO_REGISTRY -DPROPERTIES_LOCAL_INI_FILE -fomit-frame-pointer -finline-functions -fforce-addr -fforce-mem -funroll-loops
CPPFLAGS = $(CFLAGS)
RCFLAGS  =
LDFLAGS  = -static -Wl,--subsystem,windows
TARGET   = blueMSX.exe
LIBS     = -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -lshlwapi -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -ldxguid -lwinmm -lComCtl32 -lddraw -ldsound -ldinput -lmsvcrt -lvfw32 -ld3d9 -ld3dx9

SRCS        = $(SOURCE_FILES)
OBJS        = $(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))
OUTPUT_OBJS = $(addprefix $(OUTPUT_DIR)/, $(OBJS))

#
# Include paths
#
INCLUDE = 
INCLUDE += -I$(ROOT_DIR)/Src/Arch
INCLUDE += -I$(ROOT_DIR)/Src/Bios
INCLUDE += -I$(ROOT_DIR)/Src/Board
INCLUDE += -I$(ROOT_DIR)/Src/BuildInfo
INCLUDE += -I$(ROOT_DIR)/Src/Common
INCLUDE += -I$(ROOT_DIR)/Src/Debugger
INCLUDE += -I$(ROOT_DIR)/Src/Emulator
INCLUDE += -I$(ROOT_DIR)/Src/Input
INCLUDE += -I$(ROOT_DIR)/Src/IoDevice
INCLUDE += -I$(ROOT_DIR)/Src/Language
INCLUDE += -I$(ROOT_DIR)/Src/Media
INCLUDE += -I$(ROOT_DIR)/Src/Memory
INCLUDE += -I$(ROOT_DIR)/Src/Resources
INCLUDE += -I$(ROOT_DIR)/Src/SoundChips
INCLUDE += -I$(ROOT_DIR)/Src/Theme
INCLUDE += -I$(ROOT_DIR)/Src/TinyXML
INCLUDE += -I$(ROOT_DIR)/Src/Unzip
INCLUDE += -I$(ROOT_DIR)/Src/Utils
INCLUDE += -I$(ROOT_DIR)/Src/VideoChips
INCLUDE += -I$(ROOT_DIR)/Src/VideoRender
INCLUDE += -I$(ROOT_DIR)/Src/Win32
INCLUDE += -I$(ROOT_DIR)/Src/Z80
#INCLUDE += -I$(ROOT_DIR)/Src/ThirdParty/DirectShowSamples
INCLUDE += -I$(ROOT_DIR)/Src/ThirdParty/WinPCap

vpath % $(ROOT_DIR)/Src/Arch
vpath % $(ROOT_DIR)/Src/Bios
vpath % $(ROOT_DIR)/Src/Board
vpath % $(ROOT_DIR)/Src/BuildInfo
vpath % $(ROOT_DIR)/Src/Common
vpath % $(ROOT_DIR)/Src/Debugger
vpath % $(ROOT_DIR)/Src/Emulator
vpath % $(ROOT_DIR)/Src/Input
vpath % $(ROOT_DIR)/Src/IoDevice
vpath % $(ROOT_DIR)/Src/Language
vpath % $(ROOT_DIR)/Src/Media
vpath % $(ROOT_DIR)/Src/Memory
vpath % $(ROOT_DIR)/Src/Resources
vpath % $(ROOT_DIR)/Src/SoundChips
vpath % $(ROOT_DIR)/Src/Theme
vpath % $(ROOT_DIR)/Src/TinyXML
vpath % $(ROOT_DIR)/Src/Unzip
vpath % $(ROOT_DIR)/Src/Utils
vpath % $(ROOT_DIR)/Src/VideoChips
vpath % $(ROOT_DIR)/Src/VideoRender
vpath % $(ROOT_DIR)/Src/Win32
vpath % $(ROOT_DIR)/Src/Z80

#
# Source files
#
SOURCE_FILES  =
SOURCE_FILES += Win32.c 
SOURCE_FILES += Win32Avi.c
SOURCE_FILES += Win32Bitmap.c 
SOURCE_FILES += Win32Cdrom.c 
SOURCE_FILES += Win32Dir.cpp 
SOURCE_FILES += Win32D3D.cpp 
#SOURCE_FILES += Win32DirectShow.cpp
SOURCE_FILES += Win32directX.c 
SOURCE_FILES += Win32directXSound.c 
SOURCE_FILES += Win32Eth.c
SOURCE_FILES += Win32Event.c 
SOURCE_FILES += Win32file.c 
SOURCE_FILES += Win32FileTypes.c
SOURCE_FILES += Win32Glob.c
SOURCE_FILES += Win32Help.c 
SOURCE_FILES += Win32joystick.c 
SOURCE_FILES += Win32keyboard.c 
SOURCE_FILES += Win32machineConfig.c 
SOURCE_FILES += Win32Menu.c 
SOURCE_FILES += Win32MouseEmu.c 
SOURCE_FILES += Win32Printer.c 
SOURCE_FILES += Win32properties.c 
SOURCE_FILES += Win32ScreenShot.c 
SOURCE_FILES += Win32ShortcutsConfig.c 
SOURCE_FILES += Win32snd.c 
SOURCE_FILES += Win32ThemeClassic.c 
SOURCE_FILES += Win32Thread.c 
SOURCE_FILES += Win32Timer.c 
SOURCE_FILES += Win32ToolLoader.c
SOURCE_FILES += Win32Uart.c
SOURCE_FILES += Win32VideoIn.c
#SOURCE_FILES += Win32VideoIn.cpp
SOURCE_FILES += Win32Window.c
SOURCE_FILES += Win32wmmSound.c 
SOURCE_FILES += Win32Midi.c
SOURCE_FILES += Midi_w32.c

SOURCE_FILES += ArrayList.c
SOURCE_FILES += DebugMemory.c

SOURCE_FILES += Patch.c 

SOURCE_FILES += adler32.c
SOURCE_FILES += compress.c
SOURCE_FILES += crc32.c
SOURCE_FILES += gzio.c
SOURCE_FILES += uncompr.c
SOURCE_FILES += deflate.c
SOURCE_FILES += trees.c
SOURCE_FILES += zutil.c
SOURCE_FILES += inflate.c
SOURCE_FILES += infback.c
SOURCE_FILES += inftrees.c
SOURCE_FILES += inffast.c

SOURCE_FILES += unzip.c
SOURCE_FILES += ioapi.c
SOURCE_FILES += zip.c
SOURCE_FILES += iowin32.c

SOURCE_FILES += AtmelPerom.c
SOURCE_FILES += AmdFlash.c
SOURCE_FILES += DeviceManager.c
SOURCE_FILES += IoPort.c
SOURCE_FILES += MegaromCartridge.c
SOURCE_FILES += MegaSCSIsub.c
SOURCE_FILES += ram1kBMirrored.c
SOURCE_FILES += ramMapper.c
SOURCE_FILES += ramMapperIo.c
SOURCE_FILES += ramNormal.c
SOURCE_FILES += RomLoader.c
SOURCE_FILES += romMapperA1FM.c
SOURCE_FILES += romMapperA1FMModem.c
SOURCE_FILES += romMapperActivisionPcb.c
SOURCE_FILES += romMapperArc.c
SOURCE_FILES += romMapperASCII16.c
SOURCE_FILES += romMapperASCII16nf.c
SOURCE_FILES += romMapperASCII16sram.c
SOURCE_FILES += romMapperASCII8.c
SOURCE_FILES += romMapperASCII8sram.c
SOURCE_FILES += romMapperBasic.c
SOURCE_FILES += romMapperBeerIDE.c
SOURCE_FILES += romMapperBunsetu.c
SOURCE_FILES += romMapperCasette.c
SOURCE_FILES += romMapperCrossBlaim.c
SOURCE_FILES += romMapperCvMegaCart.c
SOURCE_FILES += romMapperDisk.c
SOURCE_FILES += romMapperDooly.c
SOURCE_FILES += romMapperDRAM.c
SOURCE_FILES += romMapperDumas.c
SOURCE_FILES += romMapperF4device.c
SOURCE_FILES += romMapperFMPAC.c
SOURCE_FILES += romMapperFMPAK.c
SOURCE_FILES += romMapperPAC.c
SOURCE_FILES += romMapperFmDas.c
SOURCE_FILES += romMapperForteII.c
SOURCE_FILES += romMapperGIDE.c
SOURCE_FILES += romMapperGoudaSCSI.c
SOURCE_FILES += romMapperGameMaster2.c
SOURCE_FILES += romMapperGameReader.c
SOURCE_FILES += romMapperHalnote.c
SOURCE_FILES += romMapperHarryFox.c
SOURCE_FILES += romMapperHolyQuran.c
SOURCE_FILES += romMapperJoyrexPsg.c
SOURCE_FILES += romMapperKanji.c
SOURCE_FILES += romMapperKanji12.c
SOURCE_FILES += romMapperKoei.c
SOURCE_FILES += romMapperKonami4.c
SOURCE_FILES += romMapperKonami4nf.c
SOURCE_FILES += romMapperKonami5.c
SOURCE_FILES += romMapperKonamiKeyboardMaster.c
SOURCE_FILES += romMapperKonamiSynth.c
SOURCE_FILES += romMapperKonamiWordPro.c
SOURCE_FILES += romMapperKorean126.c
SOURCE_FILES += romMapperKorean80.c
SOURCE_FILES += romMapperKorean90.c
SOURCE_FILES += romMapperLodeRunner.c
SOURCE_FILES += romMapperMajutsushi.c
SOURCE_FILES += romMapperMatraINK.c
SOURCE_FILES += romMapperMegaRAM.c
SOURCE_FILES += romMapperMegaFlashRomScc.c
SOURCE_FILES += romMapperMicrosol.c
SOURCE_FILES += romMapperMicrosolVmx80.c
SOURCE_FILES += romMapperMoonsound.c
SOURCE_FILES += romMapperMsxAudio.c
SOURCE_FILES += romMapperMsxDos2.c
SOURCE_FILES += romMapperMsxMusic.c
SOURCE_FILES += romMapperMsxPrn.c
SOURCE_FILES += romMapperMsxRs232.c
SOURCE_FILES += romMapperMuPack.c
SOURCE_FILES += romMapperNational.c
SOURCE_FILES += romMapperNationalFdc.c
SOURCE_FILES += romMapperNet.c
SOURCE_FILES += romMapperNettouYakyuu.c
SOURCE_FILES += romMapperNms1210Rs232.c
SOURCE_FILES += romMapperNms8280VideoDa.c
SOURCE_FILES += romMapperNormal.c
SOURCE_FILES += romMapperNoWind.cpp
SOURCE_FILES += romMapperObsonet.c
SOURCE_FILES += romMapperOpcodeBios.c
SOURCE_FILES += romMapperOpcodeMegaRam.c
SOURCE_FILES += romMapperOpcodeModule.c
SOURCE_FILES += romMapperOpcodePsg.c
SOURCE_FILES += romMapperOpcodeSaveRam.c
SOURCE_FILES += romMapperOpcodeSlotManager.c
SOURCE_FILES += romMapperPanasonic.c
SOURCE_FILES += romMapperPhilipsFdc.c
SOURCE_FILES += romMapperPlain.c
SOURCE_FILES += romMapperPlayBall.c
SOURCE_FILES += romMapperRType.c
SOURCE_FILES += romMapperS1990.c
SOURCE_FILES += romMapperSCCplus.c
SOURCE_FILES += romMapperSegaBasic.c
SOURCE_FILES += romMapperSf7000Ipl.c
SOURCE_FILES += romMapperSfg05.c
SOURCE_FILES += romMapperSg1000.c
SOURCE_FILES += romMapperSg1000Castle.c
SOURCE_FILES += romMapperSg1000RamExpander.c
SOURCE_FILES += romMapperSonyHBI55.c
SOURCE_FILES += romMapperSonyHBIV1.c
SOURCE_FILES += romMapperStandard.c
SOURCE_FILES += romMapperSunriseIDE.c
SOURCE_FILES += romMapperSvi328Fdc.c
SOURCE_FILES += romMapperSvi328Prn.c
SOURCE_FILES += romMapperSvi328Rs232.c
SOURCE_FILES += romMapperSvi328RsIDE.c
SOURCE_FILES += romMapperSvi727.c
SOURCE_FILES += romMapperSvi707Fdc.c
SOURCE_FILES += romMapperSvi738Fdc.c
SOURCE_FILES += romMapperSvi80Col.c
SOURCE_FILES += romMapperTC8566AF.c
SOURCE_FILES += romMapperTurboRPcm.c
SOURCE_FILES += romMapperTurboRTimer.c
SOURCE_FILES += SlotManager.c
SOURCE_FILES += sramMapperMatsuchita.c
SOURCE_FILES += sramMapperS1985.c
SOURCE_FILES += sramMapperEseSCC.c
SOURCE_FILES += sramMapperMegaSCSI.c
SOURCE_FILES += sramLoader.c

SOURCE_FILES += DebugDeviceManager.c
SOURCE_FILES += Debugger.c 

SOURCE_FILES += Crc32Calc.c
SOURCE_FILES += MediaDb.cpp
SOURCE_FILES += Sha1.cpp 

SOURCE_FILES += CRTC6845.c 
SOURCE_FILES += FrameBuffer.c 
SOURCE_FILES += VDP.c 
SOURCE_FILES += V9938.c 
SOURCE_FILES += VideoManager.c 

SOURCE_FILES += hq2x.c 
SOURCE_FILES += hq3x.c 
SOURCE_FILES += Scalebit.c 
SOURCE_FILES += VideoRender.c

SOURCE_FILES += R800.c 
SOURCE_FILES += R800Dasm.c 
SOURCE_FILES += R800Debug.c
SOURCE_FILES += R800SaveState.c 

SOURCE_FILES += Casette.c 
SOURCE_FILES += DirAsDisk.c
SOURCE_FILES += Disk.c
SOURCE_FILES += FdcAudio.c
SOURCE_FILES += ft245.c
SOURCE_FILES += GameReader.cpp
SOURCE_FILES += HarddiskIDE.c
SOURCE_FILES += I8250.c
SOURCE_FILES += I8251.c
SOURCE_FILES += I8254.c
SOURCE_FILES += I8255.c
#SOURCE_FILES += JoystickIO.c
SOURCE_FILES += Led.c
SOURCE_FILES += MB89352.c
SOURCE_FILES += Microchip24x00.c
SOURCE_FILES += Microwire93Cx6.c
SOURCE_FILES += MidiIO.c
SOURCE_FILES += msxgr.cpp
SOURCE_FILES += MSXMidi.c
SOURCE_FILES += MsxPPI.c
SOURCE_FILES += NEC765.c
SOURCE_FILES += PrinterIO.c
SOURCE_FILES += RTC.c
SOURCE_FILES += rtl8019.c
SOURCE_FILES += Sc3000PPI.c
SOURCE_FILES += ScsiDevice.c
SOURCE_FILES += Sf7000PPI.c
SOURCE_FILES += sl811hs.c
SOURCE_FILES += SunriseIDE.c
SOURCE_FILES += SviPPI.c
SOURCE_FILES += Switches.c
SOURCE_FILES += TC8566AF.c
SOURCE_FILES += TurboRIO.c
SOURCE_FILES += UartIO.c
SOURCE_FILES += WD2793.c
SOURCE_FILES += wd33c93.c
SOURCE_FILES += WDCRC.c
SOURCE_FILES += Z8530.c

SOURCE_FILES += CoinDevice.c
SOURCE_FILES += ColecoJoystick.c
SOURCE_FILES += ColecoSteeringWheel.c
SOURCE_FILES += ColecoSuperAction.c
SOURCE_FILES += InputEvent.c
SOURCE_FILES += JoystickPort.c
SOURCE_FILES += MagicKeyDongle.c
SOURCE_FILES += MsxArkanoidPad.c
SOURCE_FILES += MsxAsciiLaser.c
SOURCE_FILES += MsxGunstick.c
SOURCE_FILES += MsxJoystick.c
SOURCE_FILES += MsxMouse.c
SOURCE_FILES += MsxTetrisDongle.c
SOURCE_FILES += Sg1000JoyIo.c
SOURCE_FILES += Sg1000Joystick.c
SOURCE_FILES += SviJoyIo.c
SOURCE_FILES += SviJoystick.c

SOURCE_FILES += Language.c

SOURCE_FILES += tinystr.cpp 
SOURCE_FILES += tinyxml.cpp 
SOURCE_FILES += tinyxmlerror.cpp 
SOURCE_FILES += tinyxmlparser.cpp 
 
SOURCE_FILES += AY8910.c
SOURCE_FILES += AudioMixer.c
SOURCE_FILES += DAC.c
SOURCE_FILES += Fmopl.c
SOURCE_FILES += KeyClick.c
SOURCE_FILES += MameVLM5030.c
SOURCE_FILES += MameYM2151.c
SOURCE_FILES += Moonsound.cpp
SOURCE_FILES += MsxAudio.cpp
SOURCE_FILES += MsxPsg.c
SOURCE_FILES += OpenMsxY8950.cpp
SOURCE_FILES += OpenMsxY8950Adpcm.cpp
SOURCE_FILES += OpenMsxYM2413.cpp
SOURCE_FILES += OpenMsxYM2413_2.cpp
SOURCE_FILES += OpenMsxYMF262.cpp
SOURCE_FILES += OpenMsxYMF278.cpp
SOURCE_FILES += SCC.c
SOURCE_FILES += SN76489.c
SOURCE_FILES += SamplePlayer.c
SOURCE_FILES += VLM5030.c
SOURCE_FILES += Y8950.c
SOURCE_FILES += YM2413.cpp
SOURCE_FILES += Ymdeltat.c
SOURCE_FILES += ym2151.c

SOURCE_FILES += Actions.c 
SOURCE_FILES += Appconfig.c 
SOURCE_FILES += CommandLine.c 
SOURCE_FILES += Emulator.c 
#SOURCE_FILES += Keyboard.c
SOURCE_FILES += FileHistory.c 
SOURCE_FILES += LaunchFile.c 
SOURCE_FILES += Properties.c 

SOURCE_FILES += blowfish.c 
SOURCE_FILES += IniFileParser.c
SOURCE_FILES += IsFileExtension.c 
SOURCE_FILES += PacketFileSystem.c 
SOURCE_FILES += SaveState.c 
SOURCE_FILES += StrcmpNoCase.c 
SOURCE_FILES += TokenExtract.c 
SOURCE_FILES += ZipFromMem.c
SOURCE_FILES += ziphelper.c 

SOURCE_FILES += Theme.c 
SOURCE_FILES += ThemeTriggers.c
SOURCE_FILES += ThemeControls.c 
SOURCE_FILES += ThemeLoader.c 

SOURCE_FILES += Board.c
SOURCE_FILES += Machine.c
SOURCE_FILES += MSX.c
SOURCE_FILES += Adam.c
SOURCE_FILES += Coleco.c
SOURCE_FILES += SG1000.c
SOURCE_FILES += SVI.c

SOURCE_FILES += blueMSX.rc

HEADER_FILES  =


#
# Rules
#
all: $(OUTPUT_DIR) $(TARGET)

clean: clean_$(TARGET)	


$(TARGET): $(OUTPUT_OBJS)
	$(ECHO) Linking $@...
	$(LD) $(LDFLAGS) -o $@ $(OUTPUT_OBJS) $(LIBS)

clean_$(TARGET):
	$(ECHO) Cleaning files ...
	$(RMDIR) -rf $(OUTPUT_DIR)
	$(RM) -f $(TARGET)

$(OUTPUT_DIR):
	$(ECHO) Creating directory $@...
	$(MKDIR) $(OUTPUT_DIR)

$(OUTPUT_DIR)/%.o: %.c  $(HEADER_FILES)
	$(ECHO) Compiling $<...
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

$(OUTPUT_DIR)/%.o: %.cc  $(HEADER_FILES)
	$(ECHO) Compiling $<...
	$(CXX) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

$(OUTPUT_DIR)/%.o: %.cpp  $(HEADER_FILES)
	$(ECHO) Compiling $<...
	$(CXX) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

$(OUTPUT_DIR)/%.o: %.cxx  $(HEADER_FILES)
	$(ECHO) Compiling $<...
	$(CXX) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

$(OUTPUT_DIR)/%.res: %.rc $(HEADER_FILES)
	$(ECHO) Compiling $<...
	$(RC) $(RCFLAGS) $(INCLUDE) -o $@ -i $<

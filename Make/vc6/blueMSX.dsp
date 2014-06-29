# Microsoft Developer Studio Project File - Name="blueMSX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=blueMSX - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blueMSX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blueMSX.mak" CFG="blueMSX - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blueMSX - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "blueMSX - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "blueMSX - Win32 Final" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "blueMSX - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\Src\Arch" /I "..\..\Src\Bios" /I "..\..\Src\Board" /I "..\..\Src\BuildInfo" /I "..\..\Src\Common" /I "..\..\Src\Debugger" /I "..\..\Src\Emulator" /I "..\..\Src\Input" /I "..\..\Src\IODevice" /I "..\..\Src\Language" /I "..\..\Src\Media" /I "..\..\Src\Memory" /I "..\..\Src\Resources" /I "..\..\Src\SoundChips" /I "..\..\Src\TinyXML" /I "..\..\Src\VideoChips" /I "..\..\Src\Utils" /I "..\..\Src\VideoRender" /I "..\..\Src\Win32" /I "..\..\Src\Z80" /I "..\..\Src\Unzip" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "WINDOWS" /D "LSB_FIRST" /D "SOUND" /D "DISK" /D "FMSX" /D "PROPERTIES_LOCAL_INI_FILE" /D WINVER=0x0500 /FR /YX /FD /Zm800 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib winmm.lib ComCtl32.Lib ddraw.lib dsound.lib dinput.lib ..\..\Src\Unzip\zlibstat.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "blueMSX - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\Src\Arch" /I "..\..\Src\Bios" /I "..\..\Src\Board" /I "..\..\Src\BuildInfo" /I "..\..\Src\Common" /I "..\..\Src\Debugger" /I "..\..\Src\Emulator" /I "..\..\Src\Input" /I "..\..\Src\IODevice" /I "..\..\Src\Language" /I "..\..\Src\Media" /I "..\..\Src\Memory" /I "..\..\Src\Resources" /I "..\..\Src\SoundChips" /I "..\..\Src\TinyXML" /I "..\..\Src\VideoChips" /I "..\..\Src\Utils" /I "..\..\Src\VideoRender" /I "..\..\Src\Win32" /I "..\..\Src\Z80" /I "..\..\Src\Unzip" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WINDOWS" /D "_MBCS" /D "LSB_FIRST" /D "SOUND" /D "DISK" /D "FMSX" /D "PROPERTIES_LOCAL_INI_FILE" /D WINVER=0x0500 /FR /YX /FD /GZ /Zm800 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib winmm.lib ComCtl32.Lib ddraw.lib dsound.lib dinput.lib ..\..\Src\Unzip\zlibstat.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "blueMSX - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "blueMSX___Win32_Final"
# PROP BASE Intermediate_Dir "blueMSX___Win32_Final"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Final"
# PROP Intermediate_Dir "Final"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "Bios" /I "BuildInfo" /I "Common" /I "IODevice" /I "Language" /I "Memory" /I "Resources" /I "SoundChips" /I "Unzip" /I "VideoRender" /I "VideoChips" /I "Win32" /I "Z80" /I "Zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "WINDOWS" /D "LSB_FIRST" /D "SOUND" /D "DISK" /D "FMSX" /D "PROPERTIES_LOCAL_INI_FILE" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\Src\Arch" /I "..\..\Src\Bios" /I "..\..\Src\Board" /I "..\..\Src\BuildInfo" /I "..\..\Src\Common" /I "..\..\Src\Debugger" /I "..\..\Src\Emulator" /I "..\..\Src\Input" /I "..\..\Src\IODevice" /I "..\..\Src\Language" /I "..\..\Src\Media" /I "..\..\Src\Memory" /I "..\..\Src\Resources" /I "..\..\Src\SoundChips" /I "..\..\Src\TinyXML" /I "..\..\Src\VideoChips" /I "..\..\Src\Utils" /I "..\..\Src\VideoRender" /I "..\..\Src\Win32" /I "..\..\Src\Z80" /I "..\..\Src\Unzip" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "WINDOWS" /D "LSB_FIRST" /D "SOUND" /D "DISK" /D "FMSX" /D "PROPERTIES_LOCAL_INI_FILE" /D WINVER=0x0500 /FD /Zm800 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib winmm.lib ComCtl32.Lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib winmm.lib ComCtl32.Lib ddraw.lib dsound.lib dinput.lib ..\..\Src\Unzip\zlibstat.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
OutDir=.\Final
TargetName=blueMSX
SOURCE="$(InputPath)"
PostBuild_Desc=Pack executable
PostBuild_Cmds=upx --best $(OutDir)\$(TargetName).exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "blueMSX - Win32 Release"
# Name "blueMSX - Win32 Debug"
# Name "blueMSX - Win32 Final"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Arch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Arch\ArchBitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchCdrom.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchControls.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchEth.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchGlob.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchInput.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchKeyStorage.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchMidi.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchNotifications.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchPrinter.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchSound.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchText.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchThread.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchUart.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Arch\ArchVideoIn.h
# End Source File
# End Group
# Begin Group "Bios"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Bios\Patch.c
# End Source File
# End Group
# Begin Group "Board"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Board\Adam.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Adam.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Board.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Board.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Coleco.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Coleco.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Machine.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\Machine.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\MSX.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\MSX.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\SG1000.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\SG1000.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\SVI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Board\SVI.h
# End Source File
# End Group
# Begin Group "BuildInfo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\BuildInfo\build_number.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\BuildInfo\version.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Common\DebugMemory.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Common\MsxTypes.h
# End Source File
# End Group
# Begin Group "Debugger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Debugger\DebugDeviceManager.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Debugger\DebugDeviceManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Debugger\Debugger.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Debugger\Debugger.h
# End Source File
# End Group
# Begin Group "Emulator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Emulator\Actions.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\Actions.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\AppConfig.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\AppConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\CommandLine.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\CommandLine.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\Emulator.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\Emulator.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\FileHistory.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\FileHistory.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\LaunchFile.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\LaunchFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\Properties.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Emulator\Properties.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Input\CoinDevice.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\CoinDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoJoystick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoJoystick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoJoystickDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoSteeringWheel.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoSteeringWheel.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoSuperAction.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\ColecoSuperAction.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\InputEvent.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\InputEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\JoystickPort.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\JoystickPort.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MagicKeyDongle.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MagicKeyDongle.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxArkanoidPad.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxArkanoidPad.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxAsciiLaser.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxAsciiLaser.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxGunstick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxGunstick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxJoystick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxJoystick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxJoystickDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxMouse.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxMouse.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxTetrisDongle.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\MsxTetrisDongle.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\Sg1000JoyIo.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\Sg1000JoyIo.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\Sg1000Joystick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\Sg1000Joystick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\Sg1000JoystickDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\SviJoyIo.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\SviJoyIo.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\SviJoystick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\SviJoystick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Input\SviJoystickDevice.h
# End Source File
# End Group
# Begin Group "IoDevice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\IoDevice\Casette.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Casette.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\DirAsDisk.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\DirAsDisk.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Disk.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Disk.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\DiskUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\FdcAudio.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\FdcAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\ft245.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\ft245.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\GameReader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\GameReader.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\HarddiskIDE.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\HarddiskIDE.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8250.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8250.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8251.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8251.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8254.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8254.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8255.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\I8255.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\JoystickIO.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\JoystickIO.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Led.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Led.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MB89352.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MB89352.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Microwire93Cx6.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Microwire93Cx6.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MidiIO.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MidiIO.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\msxgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\msxgr.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MSXMidi.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MSXMidi.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MsxPPI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\MsxPPI.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\NEC765.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\NEC765.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\PrinterIO.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\PrinterIO.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\RTC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\RTC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\rtl8019.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\rtl8019.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Sc3000PPI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Sc3000PPI.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\ScsiDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\scsidev.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\ScsiDevice.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\ScsiDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Sf7000PPI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Sf7000PPI.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\sl811hs.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\sl811hs.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\SunriseIDE.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\SunriseIDE.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\SviPPI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\SviPPI.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Switches.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Switches.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\TC8566AF.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\TC8566AF.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\TurboRIO.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\TurboRIO.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\UartIO.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\UartIO.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\WD2793.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\WD2793.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\wd33c93.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\wd33c93.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\WDCRC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\WDCRC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Z8530.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\IoDevice\Z8530.h
# End Source File
# End Group
# Begin Group "Language"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Language\Language.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\Language.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageCatalan.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageChineseSimplified.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageChineseTraditional.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageDutch.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageEnglish.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageFinnish.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageFrench.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageGerman.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageItalian.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageJapanese.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageKorean.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageMinimal.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguagePolish.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguagePortuguese.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageRussian.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageSpannish.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageStrings.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Language\LanguageSwedish.h
# End Source File
# End Group
# Begin Group "Linux"

# PROP Default_Filter ""
# End Group
# Begin Group "Media"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Media\Crc32Calc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Media\Crc32Calc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Media\MediaDb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Media\MediaDb.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Media\Sha1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Media\Sha1.h
# End Source File
# End Group
# Begin Group "Memory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Memory\AmdFlash.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\AmdFlash.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\AtmelPerom.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\AtmelPerom.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\DeviceManager.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\DeviceManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\IoPort.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\IoPort.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\MegaromCartridge.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\MegaromCartridge.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\MegaSCSIsub.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\MegaSCSIsub.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\NettouYakyuuSamples.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\PlayballSamples.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ram1kBMirrored.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ram1kBMirrored.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ramMapper.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ramMapper.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ramMapperIo.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ramMapperIo.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ramNormal.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\ramNormal.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\RomLoader.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\RomLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\RomMapper.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\RomMapper.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperA1FM.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperA1FM.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperA1FMModem.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperA1FMModem.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperArc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperArc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII16.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII16.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII16nf.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII16nf.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII16sram.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII16sram.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII8.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII8.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII8sram.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperASCII8sram.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperBasic.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperBeerIDE.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperBeerIDE.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperBunsetu.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperBunsetu.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperCasette.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperCasette.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperCrossBlaim.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperCrossBlaim.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperCvMegaCart.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperCvMegaCart.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperDisk.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperDisk.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperDRAM.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperDRAM.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperDumas.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperDumas.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperF4device.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperF4device.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperFmDas.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperFmDas.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperFMPAC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperFMPAC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperFMPAK.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperFMPAK.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperForteII.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperForteII.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGameMaster2.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGameMaster2.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGameReader.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGameReader.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGIDE.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGIDE.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGoudaSCSI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperGoudaSCSI.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperHalnote.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperHalnote.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperHarryFox.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperHarryFox.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperHolyQuran.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperHolyQuran.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperJoyrexPsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperJoyrexPsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKanji.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKanji.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKanji12.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKanji12.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKoei.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKoei.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonami4.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonami4.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonami4nf.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonami4nf.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonami5.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonami5.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonamiKeyboardMaster.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonamiKeyboardMaster.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonamiSynth.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonamiSynth.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonamiWordPro.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKonamiWordPro.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKorean126.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKorean126.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKorean80.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKorean80.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKorean90.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperKorean90.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperLodeRunner.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperLodeRunner.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMajutsushi.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMajutsushi.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMatraINK.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMatraINK.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMegaFlashRomScc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMegaFlashRomScc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMegaRAM.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMegaRAM.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMicrosol.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMicrosol.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMicrosolVmx80.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMicrosolVmx80.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMoonsound.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMoonsound.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxAudio.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxDos2.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxDos2.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxMusic.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxMusic.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxPrn.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxPrn.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxRs232.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperMsxRs232.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNational.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNational.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNationalFdc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNationalFdc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNet.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNet.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNettouYakyuu.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNettouYakyuu.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNms1210Rs232.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNms1210Rs232.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNms8280VideoDa.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNms8280VideoDa.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNormal.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNormal.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNoWind.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperNoWind.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperObsonet.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperObsonet.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeBios.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeBios.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeMegaRam.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeMegaRam.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodePsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodePsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeSaveRam.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeSaveRam.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeSlotManager.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperOpcodeSlotManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPAC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPAC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPanasonic.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPanasonic.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPhilipsFdc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPhilipsFdc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPlain.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPlain.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPlayBall.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperPlayBall.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperRType.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperRType.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperS1990.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperS1990.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSCCplus.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSCCplus.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSegaBasic.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSegaBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSf7000Ipl.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSf7000Ipl.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSfg05.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSfg05.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSg1000.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSg1000.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSg1000Castle.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSg1000Castle.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSonyHBI55.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSonyHBI55.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSonyHBIV1.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSonyHBIV1.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperStandard.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperStandard.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSunriseIDE.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSunriseIDE.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi328Fdc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi328Fdc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi328Prn.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi328Prn.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi328Rs232.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi328Rs232.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi727.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi727.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi738Fdc.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi738Fdc.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi80Col.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperSvi80Col.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperTC8566AF.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperTC8566AF.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperTurboRPcm.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperTurboRPcm.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperTurboRTimer.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\romMapperTurboRTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\SlotManager.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\SlotManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramLoader.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperEseSCC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperEseSCC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperMatsuchita.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperMatsuchita.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperMegaSCSI.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperMegaSCSI.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperS1985.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Memory\sramMapperS1985.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Resources\blueMSX.rc
# End Source File
# Begin Source File

SOURCE=..\..\Src\Resources\Resource.h
# End Source File
# End Group
# Begin Group "Sdl"

# PROP Default_Filter ""
# End Group
# Begin Group "SoundChips"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\SoundChips\AudioMixer.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\AudioMixer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\AY8910.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\AY8910.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\DAC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\DAC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Fmopl.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Fmopl.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\KeyClick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\KeyClick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MameVLM5030.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MameVLM5030.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MameYM2151.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MameYM2151.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Moonsound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Moonsound.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MsxAudio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MsxAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MsxPsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\MsxPsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxY8950.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxY8950.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxY8950Adpcm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxY8950Adpcm.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYM2413.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYM2413.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYM2413_2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYM2413_2.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYMF262.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYMF262.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYMF278.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\OpenMsxYMF278.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\SamplePlayer.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\SamplePlayer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\SCC.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\SCC.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\SN76489.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\SN76489.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\VLM5030.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\VLM5030.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\VLM5030VoiceData.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Y8950.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Y8950.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\ym2151.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\ym2151.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\YM2413.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\YM2413.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Ymdeltat.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\SoundChips\Ymdeltat.h
# End Source File
# End Group
# Begin Group "Theme"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Theme\Theme.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\Theme.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\ThemeControls.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\ThemeControls.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\ThemeLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\ThemeLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\ThemeTriggers.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Theme\ThemeTriggers.h
# End Source File
# End Group
# Begin Group "ThirdParty"

# PROP Default_Filter ""
# End Group
# Begin Group "TinyXML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\TinyXML\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TinyXML\TinyStr.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\TinyXML\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TinyXML\tinyxml.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\TinyXML\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TinyXML\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "Unzip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Unzip\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\crc32.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\crypt.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\infback.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inflate.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\ioapi.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\ioapi.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\iowin32.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\iowin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\unzip.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\unzip.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\zip.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\zip.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\zutil.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Unzip\zutil.h
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Utils\authkey.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\blowfish.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\blowfish.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\IniFileParser.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\IniFileParser.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\IsFileExtension.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\IsFileExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\PacketFileSystem.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\PacketFileSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\SaveState.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\SaveState.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\StrcmpNoCase.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\StrcmpNoCase.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\TokenExtract.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\TokenExtract.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\VeryTinyXpath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\VeryTinyXpath.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\ZipFromMem.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\ZipFromMem.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\ziphelper.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Utils\ziphelper.h
# End Source File
# End Group
# Begin Group "VideoChips"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\VideoChips\Common.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\CRTC6845.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\CRTC6845.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\FrameBuffer.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\FrameBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\SpriteLine.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\SpriteLine.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\V9938.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\V9938.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\VDP.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\VDP.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\VideoManager.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoChips\VideoManager.h
# End Source File
# End Group
# Begin Group "VideoRender"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\VideoRender\hq2x.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\hq2x.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\hq3x.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\hq3x.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\Scale2x.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\Scale3x.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\Scalebit.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\Scalebit.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\VideoRender.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\VideoRender\VideoRender.h
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Win32\BlueMSXToolInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Midi_w32.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Midi_w32.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Avi.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Avi.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Bitmap.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Cdrom.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Cdrom.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Common.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Dir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Dir.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32DirectShow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32DirectShow.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32directX.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32directx.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32directXSound.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32directXSound.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Eth.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Eth.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Event.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32file.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32file.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32FileTypes.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32FileTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Glob.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Help.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Help.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32joystick.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32joystick.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32keyboard.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32keyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32machineConfig.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32machineConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Menu.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Menu.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Midi.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Midi.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32MouseEmu.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32MouseEmu.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Printer.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Printer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32properties.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Properties.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ScreenShot.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ScreenShot.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ShortcutsConfig.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ShortcutsConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32snd.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32SystemTime.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ThemeClassic.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ThemeClassic.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Thread.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Timer.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Timer.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ToolLoader.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32ToolLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Uart.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Uart.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32VideoIn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32VideoIn.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Window.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32Window.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32wmmSound.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Win32\Win32wmmSound.h
# End Source File
# End Group
# Begin Group "Z80"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Z80\R800.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800Dasm.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800Dasm.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800Debug.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800SaveState.c
# End Source File
# Begin Source File

SOURCE=..\..\Src\Z80\R800SaveState.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

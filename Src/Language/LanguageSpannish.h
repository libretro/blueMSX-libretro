/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageSpannish.h,v $
**
** $Revision: 1.32 $
**
** $Date: 2006/06/27 15:43:09 $
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
#ifndef LANGUAGE_SPANISH_H
#define LANGUAGE_SPANISH_H

#include "LanguageStrings.h"

void langInitSpanish(LanguageStrings* ls)
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "Chino simplificado";
    ls->langChineseTraditional  = "Chino tradicional";
    ls->langDutch               = "Holandés";
    ls->langEnglish             = "Inglés";
    ls->langFinnish             = "Finlandés";
    ls->langFrench              = "Francés";
    ls->langGerman              = "Alemán";
    ls->langItalian             = "Italiano";
    ls->langJapanese            = "Japonés";
    ls->langKorean              = "Coreano";
    ls->langPolish              = "Polaco";
    ls->langPortuguese          = "Portugués";
    ls->langSpanish             = "Español";
    ls->langSwedish             = "Sueco";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "Tipo:"; 
    ls->textFilename            = "Nombre:"; 
    ls->textFile                = "Archivo"; 
    ls->textNone                = "Ninguno"; 
    ls->textUnknown             = "Desconocido";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - Advertencia";
    ls->warningDiscardChanges   = "Quieres descartar cambios de la configuración actual?"; 
    ls->warningOverwriteFile    = "Usted desea sobreescribir este archivo?:"; // New in 2.5
    ls->errorTitle              = "blueMSX - Error";
    ls->errorEnterFullscreen    = "Error al intentar Modo Pantalla Completa             \n";
    ls->errorDirectXFailed      = "Error al crear objetos DirectX.           \n.\nComprueba configuración de Vídeo.";
    ls->errorNoRomInZip         = "No hay archivo .rom en el archivo zip.";
    ls->errorNoDskInZip         = "No hay archivo .dsk en el archivo zip.";
    ls->errorNoCasInZip         = "No hay archivo .cas en el archivo zip.";
    ls->errorNoHelp             = "Imposible encontrar archivo de ayuda de BlueMSX.";
    ls->errorStartEmu           = "Error al iniciar MSX emulator.";
    ls->errorPortableReadonly   = "El dispositivo portable es sólo lectura";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM image"; 
    ls->fileAll                 = "Todos los archivos"; 
    ls->fileCpuState            = "Carga CPU"; 
    ls->fileDisk                = "Disk Image"; 
    ls->fileCas                 = "Tape Image"; 


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- No hay archivos recientes -";
    ls->menuInsert              = "Insertar";
    ls->menuEject               = "Sacar";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCCPlus         = "Cartucho SCC+";
    ls->menuCartSCC             = "Cartucho SCC";
    ls->menuCartFMPac           = "Cartucho FM-PAC";
    ls->menuCartPac             = "Cartucho PAC";
    ls->menuCartHBI55           = "Cartucho Sony HBI-55"; 
    ls->menuCartInsertSpecial   = "Insertar Especial";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "RAM externo"; 

    ls->menuDiskInsertNew       = "Insertar nueva imagen de Disco";              // New in 2.5
    ls->menuDiskDirInsert       = "Insertar directorio"; 
    ls->menuDiskAutoStart       = "Reinicio Tras Insertar";
    ls->menuCartAutoReset       = "Reinicio Tras Insertar/Sacar";

    ls->menuCasRewindAfterInsert = "Rebovinar tras Insertar";
    ls->menuCasUseReadOnly       = "Usar Imagen de Cassette Sólo Lectura";
    ls->lmenuCasSaveAs           = "Salvar Imagen de Cassette Como...";
    ls->menuCasSetPosition      = "Posicionar";
    ls->menuCasRewind           = "Rebovinar";

    ls->menuPrnFormfeed         = "Página siguiente"; 

    ls->menuZoomNormal          = "Tamaño Pequeña";
    ls->menuZoomDouble          = "Tamaño Normal";
    ls->menuZoomFullscreen      = "Pantalla Completa";

    ls->menuPropsEmulation      = "Emulación";
    ls->menuPropsVideo          = "Vídeo";
    ls->menuPropsSound          = "Sonido";
    ls->menuPropsControls       = "Controles";
    ls->menuPropsPerformance    = "Rendimiento";
    ls->menuPropsSettings        = "Configuraciones";
    ls->menuPropsFile           = "Archivo";
    ls->menuPropsLanguage       = "Idioma";
    ls->menuPropsPorts          = "Puertos"; 

    ls->menuVideoSource         = "Salida Vídeo";                   // New in 2.5
    ls->menuVideoSourceDefault  = "Salida Vídeo no conectada";      // New in 2.5
    ls->menuVideoChipAutodetect = "Detección automática";     
    ls->menuVideoInSource       = "Entrada vídeo";                    // New in 2.5
    ls->menuVideoInBitmap       = "Archivo Bitmap";                        // New in 2.5

    ls->menuHelpHelp            = "Ayuda";
    ls->menuHelpAbout           = "Acerca De blueMSX";

    ls->menuFileCart            = "Cartucho Slot";
    ls->menuFileDisk            = "Unidad Disco";
    ls->menuFileCas             = "Cassette";
    ls->menuFilePrn             = "Impresora"; 
    ls->menuFileLoadState       = "Estado Carga CPU";
    ls->menuFileSaveState       = "Grabar Estado CPU";
    ls->menuFileQLoadState      = "Carga rápida Estado";
    ls->menuFileQSaveState      = "Grabación Rápida Estado";
    ls->menuFileCaptureAudio    = "Captura Audio";
    ls->menuFileScreenShot      = "Grabar Pantalla";
    ls->menuFileExit            = "Salir";

    ls->menuFileHarddisk        = "Disco duro";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "No hay controladores";             // New in 2.5

    ls->menuRunRun              = "Ejecutar";
    ls->menuRunPause            = "Pausar";
    ls->menuRunStop             = "Parada";
    ls->menuRunSoftReset        = "Reinicio Software";
    ls->menuRunHardReset        = "Reinicio Hardware";
    ls->menuRunCleanReset       = "Reinicio Completo";

    ls->menuToolsMachine         = "Editor de Máquina";
    ls->menuToolsShortcuts      = "Editor de Atajos";
    ls->menuToolsKeyboard       = "Editor de controladores/teclado"; 
    ls->menuToolsMixer          = "Mezclador de Audio"; 
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "Archivo";
    ls->menuRun                 = "Emulación";
    ls->menuWindow              = "Ventana";
    ls->menuOptions             = "Opciones";
    ls->menuTools                = "Herramientas";
    ls->menuHelp                = "Ayuda";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Open";
    ls->dlgCancel               = "Cancelar";
    ls->dlgSave                 = "Guardar";
    ls->dlgSaveAs               = "Guardar Como...";
    ls->dlgRun                  = "Ejecutar";
    ls->dlgClose                = "Cerrar";

    ls->dlgLoadRom              = "blueMSX - Seleccionar imagen Rom a cargar";
    ls->dlgLoadDsk              = "blueMSX - Seleccionar imagen Dsk a cargar";
    ls->dlgLoadCas              = "blueMSX - Seleccionar imagen Cas a cargar";
    ls->dlgLoadRomDskCas        = "blueMSX - Seleccionar un archivo Rom, Dsk, or Cas a cargar";
    ls->dlgLoadRomDesc          = "Seleccionar imagen Rom a cargar:";
    ls->dlgLoadDskDesc          = "Seleccionar imagen de disco a cargar:";
    ls->dlgLoadCasDesc          = "Seleccionar imagen de cinta a cargar:";
    ls->dlgLoadRomDskCasDesc    = "Seleccionar imagen Rom, Disco, o Cinta a cargar:";
    ls->dlgLoadState            = "Cargar Estado CPU";
    ls->dlgSaveState            = "Salvar Estado CPU";
    ls->dlgSaveCassette          = "blueMSX - Salvar Imagen de Cinta";
    ls->dlgInsertRom1           = "Insertar Cartucho ROM en slot 1";
    ls->dlgInsertRom2           = "Insertar Cartucho ROM en slot 2";
    ls->dlgInsertDiskA          = "Insertar imagen de Disco en Unidad A";
    ls->dlgInsertDiskB          = "Insertar imagen de Disco en Unidad B";
    ls->dlgInsertHarddisk       = "Insertar disco duro";                   // New in 2.5
    ls->dlgInsertCas            = "Insertar Cinsta de cassette";
    ls->dlgRomType              = "Tipo rom:"; 

    ls->dlgTapeTitle            = "blueMSX - Posición de la Cinta";
    ls->dlgTapeFrameText        = "Posición de la Cinta";
    ls->dlgTapeCurrentPos       = "Posición Actual";
    ls->dlgTapeTotalTime        = "Tiempo Total";
    ls->dlgTapeSetPosText        = "Posición de la Cinta:";
    ls->dlgTapeCustom            = "Mostrar Archivos Personalizados";
    ls->dlgTabPosition           = "Posición";
    ls->dlgTabType               = "Tipo";
    ls->dlgTabFilename           = "Nombre Archivo";
    ls->dlgZipReset             = "Reiniciar Tras Insertar";

    ls->dlgAboutTitle           = "blueMSX - Acerca de";

    ls->dlgLangLangText         = "Seleccionar Idioma";
    ls->dlgLangLangTitle        = "blueMSX - Idioma";

    ls->dlgAboutAbout           = "Acerca de\r\n====";
    ls->dlgAboutVersion         = "Versión:";
    ls->dlgAboutBuildNumber     = "Compilación:";
    ls->dlgAboutBuildDate       = "Fecha:";
    ls->dlgAboutCreat           = "Desarrollado por Daniel Vik";
    ls->dlgAboutDevel           = "PROGRAMADORES\r\n========";
    ls->dlgAboutThanks          = "MUCHAS GRACIAS A\r\n============";
    ls->dlgAboutLisence         = "LICENCIA\r\n"
                                  "======\r\n\r\n"
                                  "Este software se proporciona tal y como es, sin ninguna garantía."
                                  "En ningún caso el/los autores serán responsablesny de posibles daños "
                                  "producidos por el uso de este software.\r\n\r\n"
                                  "Para más detalles, visita la web www.bluemsx.com.";

    ls->dlgSavePreview          = "Imaginar"; 
    ls->dlgSaveDate             = "Fecha:"; 


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Propiedades";
    ls->propEmulation           = "Emulación";
    ls->propVideo               = "Video";
    ls->propSound               = "Sonido";
    ls->propControls            = "Controles";
    ls->propPerformance         = "Rendimiento";
    ls->propSettings             = "Configuraciones";
    ls->propFile                = "Archivo"; 
    ls->propPorts               = "Puertos";

    ls->propEmuGeneralGB        = "General ";
    ls->propEmuFamilyText       = "Familia MSX:";
    ls->propEmuMemoryGB         = "Memoria ";
    ls->propEmuRamSizeText      = "Tamaño RAM:";
    ls->propEmuVramSizeText     = "Tamaño VRAM:";
    ls->propEmuSpeedGB          = "Velocidad de la Emulación ";
    ls->propEmuSpeedText        = "Velocidad de la Emulación:";
    ls->propEmuFrontSwitchGB     = "Botones Panasonic "; 
    ls->propEmuFrontSwitch       = " Botón Frontal"; 
    ls->propEmuFdcTiming        = "No sincronizar unidade disco"; 
    ls->propEmuPauseSwitch      = " Botón Pause"; 
    ls->propEmuAudioSwitch       = " Botón cartucho MSX-AUDIO"; 
    ls->propVideoFreqText       = "Frecuencia video:"; 
    ls->propVideoFreqAuto       = "Auto"; 
    ls->propSndOversampleText   = "Oversample:"; 
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI In "; 
    ls->propSndMidiOutGB        = "MIDI Out "; 
    ls->propSndMidiChannel      = "Canal MIDI:";                      // New in 2.5
    ls->propSndMidiAll          = "Todos";                                // New in 2.5

    ls->propMonMonGB            = "Monitor ";
    ls->propMonTypeText         = "Tipo de Monitor:";
    ls->propMonEmuText          = "Emulación del Monitor:";
    ls->propVideoTypeText       = "Tipo de Video:";
    ls->propWindowSizeText      = "Tamaño de Ventana:";
    ls->propMonHorizStretch      = " Estiramiento Horizontal";
    ls->propMonVertStretch       = " Estiramiento Vertical";
    ls->propMonDeInterlace      = " De-entrelace";
    ls->propBlendFrames         = " Mezclar frames consecutivas";           // New in 2.5
    ls->propMonBrightness       = "Brillo:";
    ls->propMonContrast         = "Contraste:";
    ls->propMonSaturation       = "Saturación:";
    ls->propMonGamma            = "Gamma:";
    ls->propMonScanlines        = "Scanlines:";
    ls->propMonColorGhosting    = "Modulador RF"; 
    ls->propMonEffectsGB        = "Efectos  "; 

    ls->propPerfVideoDrvGB      = "Controlador de Video ";
    ls->propPerfVideoDispDrvText= "Controlador de Pantalla:";
    ls->propPerfFrameSkipText   = "Omisión de Frames:";
    ls->propPerfAudioDrvGB      = "Controlador de Audio ";
    ls->propPerfAudioDrvText    = "Controlador de Sonido:";
    ls->propPerfAudioBufSzText  = "Tamaño del Buffer de sonido:";
    ls->propPerfEmuGB           = "Emulación ";
    ls->propPerfSyncModeText    = "Modo SYNC:";
    ls->propFullscreenResText   = "Resolución pantalla completa:"; 

    ls->propSndChipEmuGB        = "Emulación Chip de Sonido ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " Mapa instrumentos MT-32 a General MIDI"; 

    ls->propPortsLptGB          = "Puerto paralelo "; 
    ls->propPortsComGB          = "Puertos seriales "; 
    ls->propPortsLptText        = "Puerto:"; 
    ls->propPortsCom1Text       = "Puerto 1:"; 
    ls->propPortsNone           = "Ninguno";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC"; 
    ls->propPortsFile           = "Imprimir en archivo"; 
    ls->propPortsComFile        = "Enviar hacia archivo";
    ls->propPortsOpenLogFile    = "Abrir un archivo de datos"; 
    ls->propPortsEmulateMsxPrn  = "Emulación:"; 

    ls->propSetFileHistoryGB     = "Histórico de Archivos ";
    ls->propSetFileHistorySize   = "Número de items en Histórico de Archivos:";
    ls->propSetFileHistoryClear  = "Borrar Hostórico";
    ls->propFileTypes            = " Asociar algunos tipos de archivo con blueMSX";
    ls->propWindowsEnvGB         = "Ambiente Windows";
    ls->propSetScreenSaver       = " Deshabilitar Salvapantallas mientras blueMSX esté en ejecución";
    ls->propDisableWinKeys       = " Función MSX automática para las teclas Windows"; 
    ls->propPriorityBoost       = " Dar a blueMSX una elevada prioridad";
    ls->propScreenshotPng       = " Utilizar el formato PNG para las capturas de pantalla";  // New in 2.5
    ls->propClearHistory         = "¿Desean realmente borrar hostórico ?";
    ls->propOpenRomGB           = "Abrir archivo rom"; 
    ls->propDefaultRomType      = "Tipo por defecto:"; 
    ls->propGuessRomType        = "Conjeturar tipo"; 

    ls->propSettDefSlotGB       = "Deslizar e depositar "; 
    ls->propSettDefSlots        = "Insertar cartucho en:"; 
    ls->propSettDefSlot         = " Slot"; 
    ls->propSettDefDrives       = "Insertar disco en:"; 
    ls->propSettDefDrive        = " Unidad"; 

    ls->propThemeGB             = "Tema ";
    ls->propTheme               = "Tema";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Color";
    ls->enumVideoMonGrey        = "Blanco y Negro";
    ls->enumVideoMonGreen       = "Verde";
    ls->enumVideoMonAmber       = "Ambar"; 

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "Ninguno";
    ls->enumVideoEmuYc          = "Cable Y/C";
    ls->enumVideoEmuMonitor     = "Monitor"; 
    ls->enumVideoEmuYcBlur      = "Cable Y/C ruidoso";
    ls->enumVideoEmuComp        = "Compuesto";
    ls->enumVideoEmuCompBlur    = "Compuesto Ruidoso";
    ls->enumVideoEmuScale2x     = "Escala 2x";
    ls->enumVideoEmuHq2x        = "Hq2x"; 

    ls->enumVideoSize1x         = "Normal - 320x200";
    ls->enumVideoSize2x         = "Doble - 640x400";
    ls->enumVideoSizeFullscreen = "Pantalla Completa";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw HW acel.";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Ninguno";
    ls->enumVideoFrameskip1     = "1 frame";
    ls->enumVideoFrameskip2     = "2 frames";
    ls->enumVideoFrameskip3     = "3 frames";
    ls->enumVideoFrameskip4     = "4 frames";
    ls->enumVideoFrameskip5     = "5 frames";

    ls->enumSoundDrvNone        = "Sin Sonido";
    ls->enumSoundDrvWMM         = "Controlador WMM";
    ls->enumSoundDrvDirectX     = "Controlador DirectX";

    ls->enumEmuSync1ms          = "Sinc sobre MSX refresh"; 
    ls->enumEmuSyncAuto         = "Auto (rápido)"; 
    ls->enumEmuSyncNone         = "Ninguno"; 
    ls->enumEmuSyncVblank       = "Sinc sobre PC Vertical Blank"; 
    ls->enumEmuAsyncVblank      = "Asincrónico PC Vblank";             // New in 2.5

    ls->enumControlsJoyNone     = "Ninguno";
    ls->enumControlsJoyMouse    = "Ratón";
    ls->enumControlsJoyTetris2Dongle = "Tetris 2 Dongle"; 
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey Dongle";             // New in 2.5
    ls->enumControlsJoy2Button = "2-button Joystick";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision Joystick";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 doble cara, 9 sectores";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 doble cara, 8 sectores";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 simple cara, 9 sectores";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 simple cara, 8 sectores";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 doble cara";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 simple cara";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle                = "blueMSX - Editor de Configuración de Máquina";
    ls->confConfigText           = "Configuración";
    ls->confSlotLayout           = "Esquema del Slot";
    ls->confMemory               = "Memoria";
    ls->confChipEmulation        = "Emulación de Chip";
    ls->confChipExtras          = "Extras"; 

    ls->confOpenRom             = "Abrirse ROM image"; 
    ls->confSaveTitle            = "blueMSX - Guardar Configuración";
    ls->confSaveText             = "Usted desea sobreescribir la configuración de máquina?:";
    ls->confSaveAsTitle         = "Guardar Como..."; 
    ls->confSaveAsMachineName    = "Nombre de Máquina:";
    ls->confDiscardTitle         = "blueMSX - Configuración ";
    ls->confExitSaveTitle        = "blueMSX - Salir del Editor de Configuración";
    ls->confExitSaveText         = "Quieres descartar cambios de la configuración actual?";

    ls->confSlotLayoutGB         = "Esquema del Slot ";
    ls->confSlotExtSlotGB        = "Slots Externos ";
    ls->confBoardGB             = "Sistema "; 
    ls->confBoardText           = "Sistema tipo:"; 
    ls->confSlotPrimary          = "Primario";
    ls->confSlotExpanded         = "Expandido (cuator subslots)";

    ls->confSlotCart             = "Cartucho";
    ls->confSlot                = "Slot"; 
    ls->confSubslot             = "Subslot"; 

    ls->confMemAdd               = "Añadir...";
    ls->confMemEdit              = "Editar...";
    ls->confMemRemove            = "Borrar";
    ls->confMemSlot              = "Slot";
    ls->confMemAddresss          = "Dirección";
    ls->confMemType              = "Tipo";
    ls->confMemRomImage          = "Imagen Rom";
    
    ls->confChipVideoGB          = "Video ";
    ls->confChipVideoChip        = "Chip Video:";
    ls->confChipVideoRam         = "RAM Video:";
    ls->confChipSoundGB          = "Sonido ";

    ls->confCmosGB               = "CMOS "; 
    ls->confCmosEnable           = " Activar el CMOS"; 
    ls->confCmosBattery          = " Utilizar una Batería Cargada";

    ls->confCpuFreqGB            = "Frecuencia CPU "; 
    ls->confZ80FreqText          = "Frecuencia Z80:"; 
    ls->confR800FreqText         = "Frecuencia R800:"; 
    ls->confFdcGB                = "Regulador De Diskette "; 
    ls->confCFdcNumDrivesText    = "Número de unidades:"; 

    ls->confEditMemTitle         = "blueMSX - Editar Mapa";
    ls->confEditMemGB            = "Detalles Mapa ";
    ls->confEditMemType          = "Tipo:";
    ls->confEditMemFile          = "Archivo:";
    ls->confEditMemAddress       = "Dirección";
    ls->confEditMemSize          = "Tamaño";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Tecla rápida"; 
    ls->shortcutDescription     = "Atajo"; 

    ls->shortcutSaveConfig      = "blueMSX -  - Guardar Configuración";
    ls->shortcutOverwriteConfig = "Usted desea sobreescribir la configuración del atajo?:";
    ls->shortcutExitConfig      = "blueMSX - Salir del Editor de Atajos";
    ls->shortcutDiscardConfig   = "Quieres descartar cambios de la configuración actual?";
    ls->shortcutSaveConfigAs    = "blueMSX - Guardar Configuración de Atajos como...";
    ls->shortcutConfigName      = "Nombre de la configuración:";
    ls->shortcutNewProfile      = "< Nuevo Perfil >";
    ls->shortcutConfigTitle     = "blueMSX - Editor de la tarjeta de los atajos";
    ls->shortcutAssign          = "Asigne";
    ls->shortcutPressText       = "Apoyar en la(s) tecla(s) del atajo";
    ls->shortcutScheme          = "Disposición:";
    ls->shortcutCartInsert1     = "Insertar Cartucho ROM en slot 1";
    ls->shortcutCartRemove1     = "Sacar Cartucho ROM en slot 1";
    ls->shortcutCartInsert2     = "Insertar Cartucho ROM en slot 2";
    ls->shortcutCartRemove2     = "Sacar Cartucho ROM en slot 2";
    ls->shortcutSpecialMenu1    = "Mostrar el menú especial para cartucho 1 ROM en slot 1";
    ls->shortcutSpecialMenu2    = "Mostrar el menú especial para cartucho 1 ROM en slot 2";
    ls->shortcutCartAutoReset   = "Reiniciar Tras Insertar Cartucho ROM";
    ls->shortcutDiskInsertA     = "Insertar imagen de Disco en Unidad A";
    ls->shortcutDiskDirInsertA  = "Insertar un directorio como disco A"; 
    ls->shortcutDiskRemoveA     = "Sacar imagen de Disco en Unidad A";
    ls->shortcutDiskChangeA     = "Cambiar rápidamente de Disco en Unidad A";
    ls->shortcutDiskAutoResetA  = "Reiniciar Tras Insertar Disco en Unidad A";
    ls->shortcutDiskInsertB     = "Insertar imagen de Disco en Unidad B";
    ls->shortcutDiskDirInsertB  = "Insertar un directorio como disco B";
    ls->shortcutDiskRemoveB     = "Sacar imagen de Disco en Unidad B";
    ls->shortcutCasInsert       = "Insertar Cinsta de cassette";
    ls->shortcutCasEject        = "Sacar Cinsta de cassette";
    ls->shortcutCasAutorewind   = "Rebobinado Automático o no de Cassette";
    ls->shortcutCasReadOnly     = "Cassette en método sólo Lectura o no";
    ls->shortcutCasSetPosition  = "Posicionar Cassette";
    ls->shortcutCasRewind       = "Rebovinar Cassette";
    ls->shortcutCasSave         = "Salvar Imagen de Cassette";
    ls->shortcutPrnFormFeed     = "Paso a la página siguiente"; 
    ls->shortcutCpuStateLoad    = "Cargar Estado CPU";
    ls->shortcutCpuStateSave    = "Grabar Estado CPU";
    ls->shortcutCpuStateQload   = "Cargar rápida Estado CPU";
    ls->shortcutCpuStateQsave   = "Grabar rápida Estado CPU";
    ls->shortcutAudioCapture    = "Iniciar/Pausar captura audio";
    ls->shortcutScreenshotOrig  = "Grabar Pantalla";
    ls->shortcutScreenshotSmall = "Pequeña captura de la pantalla sin filtro";
    ls->shortcutScreenshotLarge = "Gran captura de la pantalla sin filtro";
    ls->shortcutQuit            = "Salir blueMSX";
    ls->shortcutRunPause        = "Ejecutar/Pausar emulación";
    ls->shortcutStop            = "Parada emulación";
    ls->shortcutResetHard       = "Reinicio Hardware";
    ls->shortcutResetSoft       = "Reinicio Software";
    ls->shortcutResetClean      = "Reinicio Completo";
    ls->shortcutSizeSmall       = "Pasar en método ventana tamaño pequeña";
    ls->shortcutSizeNormal      = "Pasar en método ventana tamaño normal";
    ls->shortcutSizeFullscreen  = "Pasar en método pantalla completa";
    ls->shortcutSizeMinimized   = "Reducir la ventana"; 
    ls->shortcutToggleFullscren = "Balanza método ventana/método pantalla completa";
    ls->shortcutVolumeIncrease  = "Aumentar el volumen sonoro";
    ls->shortcutVolumeDecrease  = "Disminuir el volumen sonoro";
    ls->shortcutVolumeMute      = "Parada el sonido";
    ls->shortcutVolumeStereo    = "Balanza mono/stereo";
    ls->shortcutSwitchMsxAudio  = "Impulsar el botón cartucho MSX-AUDIO";
    ls->shortcutSwitchFront     = "Impulsar el botón frontal Panasonic";
    ls->shortcutSwitchPause     = "Impulsar el botón Pause"; 
    ls->shortcutToggleMouseLock = "Activar/desactivar el bloqueo del ratón";
    ls->shortcutEmuSpeedMax     = "Velocidad máxima de la emulación";
    ls->shortcutEmuSpeedToggle  = "Balanza velocitad máxima de la emulación"; 
    ls->shortcutEmuSpeedNormal  = "Velocidad normal de la emulación";
    ls->shortcutEmuSpeedInc     = "Aumentar la velocidad de la emulación";
    ls->shortcutEmuSpeedDec     = "Disminuir la velocidad de la emulación";
    ls->shortcutThemeSwitch     = "Cambiar de tema";
    ls->shortcutShowEmuProp     = "Mostrar las propiedades de la emulación";
    ls->shortcutShowVideoProp   = "Mostraz las propiedades video";
    ls->shortcutShowAudioProp   = "Mostrar las propiedades de sonido";
    ls->shortcutShowCtrlProp    = "Mostrar las propiedades de los controles";
    ls->shortcutShowPerfProp    = "Mostrar las propiedades de redimiento";
    ls->shortcutShowSettProp    = "Mostrar las propiedades de los ajustes";
    ls->shortcutShowPorts       = "Mostrar las propriedades de los puertos";
    ls->shortcutShowLanguage    = "Mostrar diálogo de la idioma";
    ls->shortcutShowMachines    = "Mostrar editor de la máquina";
    ls->shortcutShowShortcuts   = "Mostrar editor de atajos";
    ls->shortcutShowKeyboard    = "Mostrar editor de controladores/teclado"; 
    ls->shortcutShowMixer       = "Mostrar Mezclador de Audio"; 
    ls->shortcutShowDebugger    = "Mostrar Debugger"; 
    ls->shortcutShowTrainer     = "Mostrar Trainer"; 
    ls->shortcutShowHelp        = "Ver la ayuda";
    ls->shortcutShowAbout       = "Ver la rúbrica acerca de blueMSX";
    ls->shortcutShowFiles       = "Mostrar las propiedades de los archivos";
    ls->shortcutToggleSpriteEnable = "Mostrar/ocultar los sprites";
    ls->shortcutToggleFdcTiming = "Sincronizar o no unidade disco"; 
    ls->shortcutToggleCpuTrace  = "Activar/Desactivar Rastro de la CPU"; 


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Tecla MSX:"; 
    ls->keyconfigMappedTo       = "Tecla PC :"; 
    ls->keyconfigMappingScheme  = "Configuración del teclado:"; 

    
    //----------------------
    // Rom type lines
    //----------------------

    ls->romTypeStandard         = "Standard";
    ls->romTypeMsxdos2          = "MSXDOS 2";
    ls->romTypeKonamiScc        = "Konami SCC";
    ls->romTypeKonami           = "Konami";
    ls->romTypeAscii8           = "ASCII 8";
    ls->romTypeAscii16          = "ASCII 16";
    ls->romTypeGameMaster2      = "Game Master 2 (SRAM)";
    ls->romTypeAscii8Sram       = "ASCII 8 (SRAM)";
    ls->romTypeAscii16Sram      = "ASCII 16 (SRAM)";
    ls->romTypeRtype            = "R-Type";
    ls->romTypeCrossblaim       = "Cross Blaim";
    ls->romTypeHarryFox         = "Harry Fox";
    ls->romTypeMajutsushi       = "Konami Majutsushi";
    ls->romTypeZenima80         = "Zemina 80 in 1";
    ls->romTypeZenima90         = "Zemina 90 in 1";
    ls->romTypeZenima126        = "Zemina 126 in 1";
    ls->romTypeScc              = "SCC";
    ls->romTypeSccPlus          = "SCC+";
    ls->romTypeSnatcher         = "The Snatcher";
    ls->romTypeSdSnatcher       = "SD Snatcher";
    ls->romTypeSccMirrored      = "SCC mirrored";
    ls->romTypeSccExtended      = "SCC extended";
    ls->romTypeFmpac            = "FMPAC (SRAM)";
    ls->romTypeFmpak            = "FMPAK";
    ls->romTypeKonamiGeneric    = "Konami Generic";
    ls->romTypeSuperPierrot     = "Super Pierrot";
    ls->romTypeMirrored         = "Mirrored ROM";
    ls->romTypeNormal           = "Normal ROM";
    ls->romTypeDiskPatch        = "Normal + Disk Patch";
    ls->romTypeCasPatch         = "Normal + Cassette Patch";
    ls->romTypeTc8566afFdc      = "TC8566AF Disk Controller";
    ls->romTypeMicrosolFdc      = "Microsol Disk Controller";
    ls->romTypeNationalFdc      = "National Disk Controller";
    ls->romTypePhilipsFdc       = "Philips Disk Controller";
    ls->romTypeSvi738Fdc        = "SVI-738 Disk Controller";
    ls->romTypeMappedRam        = "Mapped RAM";
    ls->romTypeMirroredRam1k    = "1kB Mirrored RAM";
    ls->romTypeNormalRam        = "Normal RAM";
    ls->romTypeKanji            = "Kanji";
    ls->romTypeHolyQuran        = "Holy Quran";
    ls->romTypeMatsushitaSram   = "Matsushita SRAM";
    ls->romTypePanasonic16      = "Panasonic 16kB SRAM";
    ls->romTypePanasonic32      = "Panasonic 32kB SRAM";
    ls->romTypeBunsetsu         = "Bunsetsu";
    ls->romTypeJisyo            = "Jisyo";
    ls->romTypeKanji12          = "Kanji12";
    ls->romTypeNationalSram     = "National (SRAM)";
    ls->romTypeS1985            = "S1985";
    ls->romTypeS1990            = "S1990";
    ls->romTypeTurborPause      = "Turbo-R Pause";
    ls->romTypeF4deviceNormal   = "F4 Device Normal";
    ls->romTypeF4deviceInvert   = "F4 Device Inverted";
    ls->romTypeMsxMidi          = "MSX-MIDI";
    ls->romTypeTurborTimer      = "Turbo-R Timer";
    ls->romTypeKoei             = "Koei (SRAM)";
    ls->romTypeBasic            = "Basic ROM";
    ls->romTypeHalnote          = "Halnote";
    ls->romTypeLodeRunner       = "Lode Runner";
    ls->romTypeNormal4000       = "Normal 4000h";
    ls->romTypeNormalC000       = "Normal C000h";
    ls->romTypeKonamiSynth      = "Konami Synthesizer";
    ls->romTypeKonamiKbdMast    = "Konami Keyboard Master";
    ls->romTypeKonamiWordPro    = "Konami Word Pro";
    ls->romTypePac              = "PAC (SRAM)";
    ls->romTypeMegaRam          = "MegaRAM";
    ls->romTypeMegaRam128       = "128kB MegaRAM";
    ls->romTypeMegaRam256       = "256kB MegaRAM";
    ls->romTypeMegaRam512       = "512kB MegaRAM";
    ls->romTypeMegaRam768       = "768kB MegaRAM";
    ls->romTypeMegaRam2mb       = "2MB MegaRAM";
    ls->romTypeExtRam           = "External RAM";
    ls->romTypeExtRam512        = "512kB External RAM";
    ls->romTypeExtRam1mb        = "1MB External RAM";
    ls->romTypeExtRam2mb        = "2MB External RAM";
    ls->romTypeExtRam4mb        = "4MB External RAM";
    ls->romTypeMsxMusic         = "MSX Music";
    ls->romTypeMsxAudio         = "MSX Audio";
    ls->romTypeY8950            = "Y8950";
    ls->romTypeMoonsound        = "Moonsound";
    ls->romTypeSvi328Cart       = "SVI-328 Cartridge";
    ls->romTypeSvi328Fdc        = "SVI-328 Disk Controller";
    ls->romTypeSvi328Prn        = "SVI-328 Printer";
    ls->romTypeSvi328Uart       = "SVI-328 Serial Port";
    ls->romTypeSvi328col80      = "SVI-328 80 Column Card";
    ls->romTypeSvi727col80      = "SVI-727 80 Column Card";
    ls->romTypeColecoCart       = "Coleco Cartridge";
    ls->romTypeSg1000Cart       = "SG-1000 Cartridge";
    ls->romTypeTheCastle        = "The Castle";
    ls->romTypeSonyHbi55        = "Sony HBI-55";
    ls->romTypeMsxPrinter       = "MSX Printer";
    ls->romTypeTurborPcm        = "Turbo-R PCM Chip";
    ls->romTypeGameReader       = "Sunrise GameReader";
    ls->romTypeSunriseIde       = "Sunrise IDE";
    ls->romTypeBeerIde          = "Beer IDE";
    ls->romTypeGide             = "GIDE";
    ls->romTypeVmx80            = "Microsol VMX-80";
    ls->romTypeNms8280Digitiz   = "Philips NMS-8280 Digitizer";
    ls->romTypeHbiV1Digitiz     = "Sony HBI-V1 Digitizer";
    ls->romTypeFmdas            = "F&M Direct Assembler System";
    ls->romTypeSfg01            = "Yamaha SFG-01";
    ls->romTypeSfg05            = "Yamaha SFG-05";
    
    
    //----------------------
    // Debug type lines
    // Note: Only needs translation if debugger is translated
    //----------------------

    ls->dbgMemVisible           = "Visible Memory";
    ls->dbgMemRamNormal         = "Normal";
    ls->dbgMemRamMapped         = "Mapped";
    ls->dbgMemVram              = "VRAM";
    ls->dbgMemYmf278            = "YMF278 Sample RAM";
    ls->dbgMemAy8950            = "AY8950 Sample RAM";
    ls->dbgMemScc               = "Memory";

    ls->dbgCallstack            = "Callstack";

    ls->dbgRegs                 = "Registers";
    ls->dbgRegsCpu              = "CPU Registers";
    ls->dbgRegsYmf262           = "YMF262 Registers";
    ls->dbgRegsYmf278           = "YMF278 Registers";
    ls->dbgRegsAy8950           = "AY8950 Registers";
    ls->dbgRegsYm2413           = "YM2413 Registers";

    ls->dbgDevRamMapper         = "RAM Mapper";
    ls->dbgDevRam               = "RAM";
    ls->dbgDevIdeBeer           = "Beer IDE";
    ls->dbgDevIdeGide           = "GIDE";
    ls->dbgDevF4Device          = "F4 Device";
    ls->dbgDevFmpac             = "FMPAC";
    ls->dbgDevFmpak             = "FMPAK";
    ls->dbgDevKanji             = "Kanji";
    ls->dbgDevKanji12           = "Kanji 12";
    ls->dbgDevKonamiKbd         = "Konami Keyboard Master";
    ls->dbgDevKorean80          = "Korean 80";
    ls->dbgDevKorean90          = "Korean 90";
    ls->dbgDevKorean128         = "Korean 128";
    ls->dbgDevMegaRam           = "Mega RAM";
    ls->dbgDevFdcMicrosol       = "Microsol FDC";
    ls->dbgDevMoonsound         = "Moonsound";
    ls->dbgDevMsxAudio          = "MSX Audio";
    ls->dbgDevMsxAudioMidi      = "MSX Audio MIDI";
    ls->dbgDevMsxMusic          = "MSX Music";
    ls->dbgDevPrinter           = "Printer";
    ls->dbgDevRs232             = "RS232";
    ls->dbgDevS1990             = "S1990";
    ls->dbgDevSfg05             = "Yamaha SFG-05";
    ls->dbgDevHbi55             = "Sony HBI-55";
    ls->dbgDevSviFdc            = "SVI FDC";
    ls->dbgDevSviPrn            = "SVI Printer";
    ls->dbgDevSvi80Col          = "SVI 80 Column";
    ls->dbgDevPcm               = "PCM";
    ls->dbgDevMatsushita        = "Matsushita";
    ls->dbgDevS1985             = "S1985";
    ls->dbgDevCrtc6845          = "CRTC6845";
    ls->dbgDevTms9929A          = "TMS9929A";
    ls->dbgDevTms99x8A          = "TMS99x8A";
    ls->dbgDevV9938             = "V9938";
    ls->dbgDevV9958             = "V9958";
    ls->dbgDevZ80               = "Z80";
    ls->dbgDevMsxMidi           = "MSX MIDI";
    ls->dbgDevPpi               = "PPI";
    ls->dbgDevRtc               = "RTC";
    ls->dbgDevTrPause           = "TR Pause";
    ls->dbgDevAy8910            = "AY8910 PSG";
    ls->dbgDevScc               = "SCC";


    //----------------------
    // Debug type lines
    // Note: Can only be translated to european languages
    //----------------------

    ls->aboutScrollThanksTo     = "Gracias especiales a: ";
    ls->aboutScrollAndYou       = "y USTED !!!!";
};

#endif

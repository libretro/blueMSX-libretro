/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguagePortuguese.h,v $
**
** $Revision: 1.34 $
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
#ifndef LANGUAGE_PORTUGUESE_H
#define LANGUAGE_PORTUGUESE_H

#include "LanguageStrings.h"

void langInitPortuguese(LanguageStrings* ls) 
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "Chinês simplificado";
    ls->langChineseTraditional  = "Chinês tradicional";
    ls->langDutch               = "Holandês";
    ls->langEnglish             = "Inglês";
    ls->langFinnish             = "Finlandês";
    ls->langFrench              = "Francês";
    ls->langGerman              = "Alemão";
    ls->langItalian             = "Italiano";
    ls->langJapanese            = "Japonês";
    ls->langKorean              = "Coreano";
    ls->langPolish              = "Polaco";
    ls->langPortuguese          = "Português";
    ls->langSpanish             = "Espanhol";
    ls->langSwedish             = "Sueco";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "Tipo:"; 
    ls->textFilename            = "Nome:"; 
    ls->textFile                = "Arquivo"; 
    ls->textNone                = "Nenhum"; 
    ls->textUnknown             = "Desconhecido";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - Advertência";
    ls->warningDiscardChanges   = "Você quer desfazer as alterações da configuração atual?"; 
    ls->warningOverwriteFile    = "Você quer substituir esto arquivo? "; // New in 2.5
    ls->errorTitle              = "blueMSX - Erro";
    ls->errorEnterFullscreen    = "Falha ao entrar em Tela Cheia.             \n";
    ls->errorDirectXFailed      = "Falha ao Criar Objetos DirectX.             \nUsando GDI ao invés.\nCheque as propriedades Video.";
    ls->errorNoRomInZip         = "Nao foi localizado arquivo .ROM dentro do arquivo zip.";
    ls->errorNoDskInZip         = "Nao foi localizado arquivo .DSK dentro do arquivo zip.";
    ls->errorNoCasInZip         = "Nao foi localizado arquivo .CAS dentro do arquivo zip.";
    ls->errorNoHelp             = "Nao foi localizado arquivo ajuda do blueMSX";
    ls->errorStartEmu           = "Falha ao Iniciar o emulador MSX.";
    ls->errorPortableReadonly   = "O dispositivo portátil é somente leitura";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM image"; 
    ls->fileAll                 = "Todos os arquivos"; 
    ls->fileCpuState            = "Status da CPU"; 
    ls->fileDisk                = "Disk Image"; 
    ls->fileCas                 = "Tape Image"; 


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- Sem Arquivos Recentes -";
    ls->menuInsert              = "Inserir";
    ls->menuEject               = "Ejetar";

    ls->menuCartGameReader      = "Game Reader";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "Cartucho SCC";
    ls->menuCartSCCPlus         = "Cartucho SCC+";
    ls->menuCartFMPac           = "Cartucho FM-PAC";
    ls->menuCartPac             = "Cartucho PAC";
    ls->menuCartHBI55           = "Cartucho Sony HBI-55"; 
    ls->menuCartInsertSpecial   = "Inserir Especial";                     // New in 2.5
    ls->menuCartMegaRam         = "MegaRAM";                            // New in 2.5
    ls->menuCartExternalRam     = "RAM esterno";

    ls->menuDiskInsertNew       = "Inserir novo disco";              // New in 2.5
    ls->menuDiskDirInsert       = "Inserir diretório"; 
    ls->menuDiskAutoStart       = "Reset após inserir";
    ls->menuCartAutoReset       = "Reset após inserir/remover";
    
    ls->menuCasRewindAfterInsert = "Rebobinar Apos Inserir";
    ls->menuCasUseReadOnly       = "Usar Cassete Como Somente Leitura";
    ls->lmenuCasSaveAs           = "Salvar Imagem do Cassete Como...";
    ls->menuCasSetPosition      = "Ajustar Posição";
    ls->menuCasRewind           = "Rebobinar";

    ls->menuPrnFormfeed         = "Página seguinte"; 

    ls->menuZoomNormal          = "Tamanho Pequeno";
    ls->menuZoomDouble          = "Tamanho Normal";
    ls->menuZoomFullscreen      = "Tela Cheia";
    
    ls->menuPropsEmulation      = "Emulação";
    ls->menuPropsVideo          = "Video";
    ls->menuPropsSound          = "Som";
    ls->menuPropsControls       = "Controles";
    ls->menuPropsPerformance    = "Performance";
    ls->menuPropsSettings       = "Ajustes";
    ls->menuPropsFile           = "Arquivo"; 
    ls->menuPropsLanguage       = "Linguagem";
    ls->menuPropsPorts          = "Portas"; 
    
    ls->menuVideoSource         = "Fonte vídeo";                   // New in 2.5
    ls->menuVideoSourceDefault  = "Fonte vídeo não conectada";      // New in 2.5
    ls->menuVideoInSource       = "Entrada vídeo";                    // New in 2.5
    ls->menuVideoInBitmap       = "Arquivo Bitmap";                        // New in 2.5
    ls->menuVideoChipAutodetect = "Detecção automática"; 

    ls->menuHelpHelp            = "Tópicos Ajuda";
    ls->menuHelpAbout           = "Sobre blueMSX";

    ls->menuFileCart            = "Cartucho Slot";
    ls->menuFileDisk            = "Disco drive";
    ls->menuFileCas             = "Cassette";
    ls->menuFilePrn             = "Impressora"; 
    ls->menuFileLoadState       = "Carregar Status da CPU";
    ls->menuFileSaveState       = "Salvar Status da CPU";
    ls->menuFileQLoadState      = "Carregar rápido Status";
    ls->menuFileQSaveState      = "Salvar rápido Status";
    ls->menuFileCaptureAudio    = "Capturar áudio";
    ls->menuFileScreenShot      = "Salvar Tela";
    ls->menuFileExit            = "Sair";

    ls->menuFileHarddisk        = "Disco duro";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "Não controladores";             // New in 2.5

    ls->menuRunRun              = "Executar";
    ls->menuRunPause            = "Pausa";
    ls->menuRunStop             = "Parar";
    ls->menuRunSoftReset        = "Reset Software";
    ls->menuRunHardReset        = "Reset Hardware";
    ls->menuRunCleanReset       = "Reset Completo";

    ls->menuToolsMachine         = "Configuração da Máquina";
    ls->menuToolsShortcuts      = "Configuração de Atalhos";
    ls->menuToolsKeyboard       = "Configuração de Controladores/Teclado"; 
    ls->menuToolsMixer          = "Mixer de Audio"; 
    ls->menuToolsDebugger       = "Debugger";               // New in 2.5
    ls->menuToolsTrainer        = "Trainer";                // New in 2.5
    ls->menuToolsTraceLogger    = "Trace Logger";           // New in 2.5

    ls->menuFile                = "Arquivo";
    ls->menuRun                 = "Emulação";
    ls->menuWindow              = "Janela";
    ls->menuOptions             = "Opções";
    ls->menuTools                = "Ferramentas";
    ls->menuHelp                = "Ajuda";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                 = "Abrir";
    ls->dlgCancel               = "Cancelar";
    ls->dlgSave                 = "Salvar";
    ls->dlgSaveAs               = "Salvar Como";
    ls->dlgRun                  = "Executar";
    ls->dlgClose                = "Fechar";

    ls->dlgLoadRom              = "blueMSX - Selecione uma ROM para carregar";
    ls->dlgLoadDsk              = "blueMSX - Selecione uma DSK para carregar";
    ls->dlgLoadCas              = "blueMSX - Selecione uma CAS para carregar";
    ls->dlgLoadRomDskCas        = "blueMSX - Selecione uma ROM, DSK, ou CAS para carregar";
    ls->dlgLoadRomDesc          = "Escolha uma ROM para carregar:";
    ls->dlgLoadDskDesc          = "Escolha um disco para carregar:";
    ls->dlgLoadCasDesc          = "Escolha um tape para carregar:";
    ls->dlgLoadRomDskCasDesc    = "Escolha uma ROM, disco, ou tape para carregar:";
    ls->dlgLoadState            = "Carregar Status da CPU";
    ls->dlgSaveState            = "Salvar Status da CPU";
    ls->dlgSaveCassette          = "blueMSX - Salvar Imagem Tape";
    ls->dlgInsertRom1           = "Insira cartucho ROM no slot 1";
    ls->dlgInsertRom2           = "Insira cartucho ROM no slot 2";
    ls->dlgInsertDiskA          = "Insira disco no drive A";
    ls->dlgInsertDiskB          = "Insira disco no drive B";
    ls->dlgInsertCas            = "Insira cassette tape";
    ls->dlgInsertHarddisk       = "Insira disco duro";                   // New in 2.5
    ls->dlgRomType              = "Tipo rom:"; 

    ls->dlgTapeTitle            = "blueMSX - Posição do Tape";
    ls->dlgTapeFrameText        = "Posição do Tape";
    ls->dlgTapeCurrentPos       = "Posição Atual";
    ls->dlgTapeTotalTime        = "Tempo Total";
    ls->dlgTapeCustom            = "Mostrar Arquivos Personalizados";
    ls->dlgTapeSetPosText        = "Posição do Cassete:";
    ls->dlgTabPosition           = "Posição";
    ls->dlgTabType               = "Tipo";
    ls->dlgTabFilename           = "Nome do Arquivo";
    ls->dlgZipReset             = "Reset após inserir";

    ls->dlgAboutTitle           = "blueMSX - Sobre";

    ls->dlgLangLangText         = "Escolha uma linguagem que blueMSX irá usar";
    ls->dlgLangLangTitle        = "blueMSX - Linguagem";

    ls->dlgAboutAbout           = "Sobre\r\n====";
    ls->dlgAboutVersion         = "Versão:";
    ls->dlgAboutBuildNumber     = "Compilação:";
    ls->dlgAboutBuildDate       = "Data:";
    ls->dlgAboutCreat           = "Criado Por Daniel Vik";
    ls->dlgAboutDevel           = "DESENVOLVEDORES\r\n========";
    ls->dlgAboutThanks          = "MUITO OBRIGADO PARA\r\n============";
    ls->dlgAboutLisence         = "LICENÇA\r\n"
                                  "======\r\n\r\n"
                                  "Este software é provido 'como-está', sem Garantia expressa ou implicada."
                                  "Em Nenhuma circunstancia os autores serão responsaveis por qualquer "
                                  "Problema ou DANO proveniente do uso deste software.\r\n\r\n"
                                  "Visite www.bluemsx.com para mais detalhes.";

    ls->dlgSavePreview          = "Visualizar"; 
    ls->dlgSaveDate             = "Data:"; 


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - Propriedades";
    ls->propEmulation           = "Emulação";
    ls->propVideo               = "Video";
    ls->propSound               = "Som";
    ls->propControls            = "Controles";
    ls->propPerformance         = "Performance";
    ls->propSettings             = "Ajustes";
    ls->propFile                = "Arquivo"; 
    ls->propPorts               = "Portas";
    
    ls->propEmuGeneralGB        = "Geral ";
    ls->propEmuFamilyText       = "MSX :";
    ls->propEmuMemoryGB         = "Memória ";
    ls->propEmuRamSizeText      = "Tamanho RAM :";
    ls->propEmuVramSizeText     = "Tamanho VRAM :";
    ls->propEmuSpeedGB          = "Velocidade da Emulação ";
    ls->propEmuSpeedText        = "Velocidade da Emulação:";
    ls->propEmuFrontSwitchGB    = "Botão Panasonic "; 
    ls->propEmuFrontSwitch      = " Botão Frontal"; 
    ls->propEmuFdcTiming        = " Não sincronizar disco do drive"; 
    ls->propEmuPauseSwitch      = " Botão Pause "; 
    ls->propEmuAudioSwitch      = " Botão Cartucho MSX-AUDIO"; 
    ls->propVideoFreqText       = "Freqüência video:"; 
    ls->propVideoFreqAuto       = "Auto"; 
    ls->propSndOversampleText   = "Oversample:"; 
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 In ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI In "; 
    ls->propSndMidiOutGB        = "MIDI Out "; 
    ls->propSndMidiChannel      = "Canal MIDI:";                      // New in 2.5
    ls->propSndMidiAll          = "Todos";                                // New in 2.5

    ls->propMonMonGB            = "Monitor:";
    ls->propMonTypeText         = "Tipo de Monitor:";
    ls->propMonEmuText          = "Emulacão de Monitor:";
    ls->propVideoTypeText       = "Tipo de Video:";
    ls->propWindowSizeText      = "Tamanho Janela:";
    ls->propMonHorizStretch     = " Estiramento Horizontal"; 
    ls->propMonVertStretch      = " Estiramento Vertical"; 
    ls->propMonDeInterlace      = " De-entrelaçar";
    ls->propBlendFrames         = " Misturar frames consecutivas";           // New in 2.5
    ls->propMonBrightness       = "Brilho:";
    ls->propMonContrast         = "Contraste:";
    ls->propMonSaturation       = "Saturação:";
    ls->propMonGamma            = "Gama:";
    ls->propMonScanlines        = " Scanlines:";
    ls->propMonColorGhosting    = " Modulador RF"; 
    ls->propMonEffectsGB        = "Efeitos "; 

    ls->propPerfVideoDrvGB      = "Driver de Video:";
    ls->propPerfVideoDispDrvText= "Driver do Display:";
    ls->propPerfFrameSkipText   = "Frame skipping:";
    ls->propPerfAudioDrvGB      = "Driver de Audio:";
    ls->propPerfAudioDrvText    = "Driver de Som:";
    ls->propPerfAudioBufSzText  = "Tamanho buffer Som:";
    ls->propPerfEmuGB           = "Emulação:";
    ls->propPerfSyncModeText    = "Modo Sincronismo:";
    ls->propFullscreenResText   = "Resolução tela cheia:"; 

    ls->propSndChipEmuGB        = "Emulação do Chip de Som";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " Mapa instrumentos MT-32 a General MIDI"; 

    ls->propPortsLptGB          = "Porta paralela "; 
    ls->propPortsComGB          = "Portas seriais "; 
    ls->propPortsLptText        = "Porta:"; 
    ls->propPortsCom1Text       = "Porta 1:"; 
    ls->propPortsNone           = "Nenhum";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC"; 
    ls->propPortsFile           = "Imprimir em arquivo"; 
    ls->propPortsComFile        = "Enviar para arquivo";
    ls->propPortsOpenLogFile    = "Abrir um arquivo de dados"; 
    ls->propPortsEmulateMsxPrn  = "Emulação:"; 

    ls->propSetFileHistoryGB    = "Histórico ";
    ls->propSetFileHistorySize  = "Numero de itens no Histórico:";
    ls->propSetFileHistoryClear = "Limpar Histórico";
    ls->propFileTypes           = " Associar certos tipos de arquivo com blueMSX";
    ls->propWindowsEnvGB        = "Ambiente Windows";
    ls->propSetScreenSaver      = " Desabilitar Proteção de Tela Quando blueMSX estiver ativo";
    ls->propDisableWinKeys      = " Função MSX automática para as teclas Windows"; 
    ls->propPriorityBoost       = " Dar ao blueMSX uma prioridade elevada";
    ls->propScreenshotPng       = " Utilizar o formato PNG para as capturas da tela";  // New in 2.5
    ls->propClearHistory        = "Deseja realmente limpar histórico ?";
    ls->propOpenRomGB           = "Abrir arquivo rom"; 
    ls->propDefaultRomType      = "Tipo de ROM Padrão:"; 
    ls->propGuessRomType        = "Adivinhar tipo"; 

    ls->propSettDefSlotGB       = "Deslizar e depositar "; 
    ls->propSettDefSlots        = "Inserir cartucho no:"; 
    ls->propSettDefSlot         = " Slot"; 
    ls->propSettDefDrives       = "Inserir disco no:"; 
    ls->propSettDefDrive        = " Drive"; 

    ls->propThemeGB             = "Tema ";
    ls->propTheme               = "Tema";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "Cor";
    ls->enumVideoMonGrey        = "Branco e Preto";
    ls->enumVideoMonGreen       = "Verde";
    ls->enumVideoMonAmber       = "Âmbar"; 

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC e PAL-M";

    ls->enumVideoEmuNone        = "Nenhum";
    ls->enumVideoEmuYc          = "Y/C cable (sharp)";
    ls->enumVideoEmuMonitor     = "Monitor"; 
    ls->enumVideoEmuYcBlur      = "Noisy Y/C cable (sharp)";
    ls->enumVideoEmuComp        = "Composite (blurry)";
    ls->enumVideoEmuCompBlur    = "Noisy Composite (blurry)";
    ls->enumVideoEmuScale2x     = "Escala 2x";
    ls->enumVideoEmuHq2x        = "Hq2x"; 

    ls->enumVideoSize1x         = "Normal - 320x200";
    ls->enumVideoSize2x         = "Duplo  - 640x400";
    ls->enumVideoSizeFullscreen = "Tela Cheia";

    ls->enumVideoDrvDirectDrawHW= "DirectDraw HW acel.";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "Nenhum";
    ls->enumVideoFrameskip1     = "1 frame";
    ls->enumVideoFrameskip2     = "2 frames";
    ls->enumVideoFrameskip3     = "3 frames";
    ls->enumVideoFrameskip4     = "4 frames";
    ls->enumVideoFrameskip5     = "5 frames";

    ls->enumSoundDrvNone        = "Sem Som";
    ls->enumSoundDrvWMM         = "Driver WMM";
    ls->enumSoundDrvDirectX     = "Driver DirectX";

    ls->enumEmuSync1ms          = "Sinc sobre MSX refresh"; 
    ls->enumEmuSyncAuto         = "Auto (rapido)"; 
    ls->enumEmuSyncNone         = "Nenhum"; 
    ls->enumEmuSyncVblank       = "Sinc sobre PC Vertical Blank"; 
    ls->enumEmuAsyncVblank      = "AssíncronoPC Vblank";             // New in 2.5

    ls->enumControlsJoyNone     = "Nenhum";
    ls->enumControlsJoyMouse    = "Mouse";
    ls->enumControlsJoyTetris2Dongle = "Tetris 2 Dongle"; 
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey Dongle";             // New in 2.5
    ls->enumControlsJoy2Button = "2-button Joystick";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "Gun Stick";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X Terminator Laser";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision Joystick";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 duplo face, 9 sectores";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 duplo face, 8 sectores";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 simples face, 9 sectores";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 simples face, 8 sectores";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 duplo face";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 simples face";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle               = "blueMSX - Editor de Configuração da Maquina";
    ls->confConfigText          = "Configuração";
    ls->confSlotLayout          = "Slot Layout";
    ls->confMemory              = "Memória";
    ls->confChipEmulation       = "Emulação do Chip";
    ls->confChipExtras          = "Extras"; 

    ls->confOpenRom             = "Abrir ROM image"; 
    ls->confSaveTitle           = "blueMSX - Salvar Configuração";
    ls->confSaveText            = "Você quer substituir esta configuração da máquina? :";
    ls->confSaveAsTitle         = "Salvar Como"; 
    ls->confSaveAsMachineName   = "Nome da Maquina:";
    ls->confDiscardTitle        = "blueMSX - Configuração";
    ls->confExitSaveTitle       = "blueMSX - Sair do Editor de Configuração";
    ls->confExitSaveText        = "Voce quer desfazer as alterações da Configuração atual ?";

    ls->confSlotLayoutGB        = "Layout do Slot ";
    ls->confSlotExtSlotGB       = "Slots Externos";
    ls->confBoardGB             = "Sistema "; 
    ls->confBoardText           = "Sistema tipo:"; 
    ls->confSlotPrimary         = "Primário";
    ls->confSlotExpanded        = "Expandido (Quatro sub-slots)";

    ls->confSlotCart            = "Cartucho";
    ls->confSlot                = "Slot"; 
    ls->confSubslot             = "Sub-slot"; 

    ls->confMemAdd               = "Adicionar...";
    ls->confMemEdit              = "Editar...";
    ls->confMemRemove            = "Remover";
    ls->confMemSlot              = "Slot";
    ls->confMemAddresss          = "Endereço";
    ls->confMemType              = "Tipo";
    ls->confMemRomImage          = "Imagem Rom ";
    
    ls->confChipVideoGB          = "Video ";
    ls->confChipVideoChip        = "Video Chip:";
    ls->confChipVideoRam         = "Video RAM:";
    ls->confChipSoundGB          = "Som ";

    ls->confCmosGB                = "CMOS "; 
    ls->confCmosEnable            = " Ativar CMOS"; 
    ls->confCmosBattery           = " Utilizar uma Bateria Carregada";

    ls->confCpuFreqGB            = "Freqüência CPU "; 
    ls->confZ80FreqText          = "Freqüência Z80:"; 
    ls->confR800FreqText         = "Freqüência R800:"; 
    ls->confFdcGB                = "Controlador do Drive "; 
    ls->confCFdcNumDrivesText    = "Número das Drives:"; 

    ls->confEditMemTitle         = "blueMSX - Editar Mapper";
    ls->confEditMemGB            = "Detalhes do Mapper ";
    ls->confEditMemType          = "Tipo:";
    ls->confEditMemFile          = "Arquivo:";
    ls->confEditMemAddress       = "Endereço";
    ls->confEditMemSize          = "Tamanho";
    ls->confEditMemSlot          = "Slot";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "Hotkey"; 
    ls->shortcutDescription     = "Shortcut"; 

    ls->shortcutSaveConfig      = "blueMSX - Salvar Configuração";
    ls->shortcutOverwriteConfig = "Você quer  substituir esta configuração da máquina?:";
    ls->shortcutExitConfig      = "blueMSX - Sair do Editor de Atalhos";
    ls->shortcutDiscardConfig   = "Voce quer desfazer as alterações da Configuração atual ?";
    ls->shortcutSaveConfigAs    = "blueMSX - Salvar Configuração de Atalhos Como...";
    ls->shortcutConfigName      = "Nome da Configuração:";
    ls->shortcutNewProfile      = "< Novo Perfil >";
    ls->shortcutConfigTitle     = "blueMSX - Editor do Mapa dos Atalhos";
    ls->shortcutAssign          = "Atribuir";
    ls->shortcutPressText       = "Apoiar sobre Tecla(s) :";
    ls->shortcutScheme          = "Disposição:";
    ls->shortcutCartInsert1     = "Inserir Cartucho no Slot 1";
    ls->shortcutCartRemove1     = "Remover Cartucho no Slot 1";
    ls->shortcutCartInsert2     = "Inserir Cartucho no Slot 2";
    ls->shortcutCartRemove2     = "Remover Cartucho no Slot 2";
    ls->shortcutSpecialMenu1    = "Mostrar Menu Especial para Cartucho no Slot 1";
    ls->shortcutSpecialMenu2    = "Mostrar Menu Especial para Cartucho no Slot 2";
    ls->shortcutCartAutoReset   = "Reset após inserir cartucho";
    ls->shortcutDiskInsertA     = "Inserir Disco no Drive A";
    ls->shortcutDiskDirInsertA  = "Inserir um diretório como disco A"; 
    ls->shortcutDiskRemoveA     = "Ejetar  Disco no Drive A ";
    ls->shortcutDiskChangeA     = "Alterar rapidamente Disco no Drive A";
    ls->shortcutDiskAutoResetA  = "Reset após inserir Disco no Drive A ";
    ls->shortcutDiskInsertB     = "Insira Disco no Drive B";
    ls->shortcutDiskDirInsertB  = "Inserir um diretório como disco B"; 
    ls->shortcutDiskRemoveB     = "Ejetar Disco no Drive B";
    ls->shortcutCasInsert       = "Inserir Cassette";
    ls->shortcutCasEject        = "Ejetar Cassette";
    ls->shortcutCasAutorewind   = "Rebobinação automática ou não do cassete";
    ls->shortcutCasReadOnly     = "Cassete em modo leitura somente ou não";
    ls->shortcutCasSetPosition  = "Ajustar Posição";
    ls->shortcutCasRewind       = "Rebobinar Cassette";
    ls->shortcutCasSave         = "Salvar Imagem do Cassette Como...";
    ls->shortcutPrnFormFeed     = "Passagem para a página seguinte"; 
    ls->shortcutCpuStateLoad    = "Carregar Status da CPU";
    ls->shortcutCpuStateSave    = "Salvar Status da CPU";
    ls->shortcutCpuStateQload   = "Carregar rapido Status da CPU";
    ls->shortcutCpuStateQsave   = "Salvar rapido Status da CPU";
    ls->shortcutAudioCapture    = "Começar/parar a captura áudio";
    ls->shortcutScreenshotOrig  = "Salvar Tela";
    ls->shortcutScreenshotSmall = "Pequena captura da tela sem filtro";
    ls->shortcutScreenshotLarge = "Grande captura da tela sem filtro";
    ls->shortcutQuit            = "Sair blueMSX";
    ls->shortcutRunPause        = "Executar/Pausa Emulação ";
    ls->shortcutStop            = "Parar Emulação";
    ls->shortcutResetHard       = "Reset Hardware";
    ls->shortcutResetSoft       = "Reset Software";
    ls->shortcutResetClean      = "Reset Completo";
    ls->shortcutSizeSmall       = "Passar em modo Janela Tamanho Pequeno";
    ls->shortcutSizeNormal      = "Passar em modo Janela Tamanho Normal";
    ls->shortcutSizeFullscreen  = "Passar em modo Tela Cheia";
    ls->shortcutSizeMinimized   = "Reduzir a Janela"; 
    ls->shortcutToggleFullscren = "Troca de modo Janela/Tela Cheia";
    ls->shortcutVolumeIncrease  = "Aumentar Volume sonoro";
    ls->shortcutVolumeDecrease  = "Diminuir Volume sonoro";
    ls->shortcutVolumeMute      = "Parar Som";
    ls->shortcutVolumeStereo    = "Troca Mono/Estereo";
    ls->shortcutSwitchMsxAudio  = "Impulsionar Botão cartucho MSX-AUDIO";
    ls->shortcutSwitchFront     = "Impulsionar Botão frontal Panasonic";
    ls->shortcutSwitchPause     = "Impulsionar Botao Pause"; 
    ls->shortcutToggleMouseLock = "Ativar/Desativar Bloqueio do Mouse";
    ls->shortcutEmuSpeedMax     = "Velocidade Máxima da Emulação";
    ls->shortcutEmuSpeedToggle  = "Troca de Velocidade Máxima da Emulação"; 
    ls->shortcutEmuSpeedNormal  = "Velocidade Normal da Emulação";
    ls->shortcutEmuSpeedInc     = "Aumentar Velocidade da Emulação";
    ls->shortcutEmuSpeedDec     = "Diminuir Velocidade da Emulação";
    ls->shortcutThemeSwitch     = "Troque o tema";
    ls->shortcutShowEmuProp     = "Mostrar Propriedades da Emulação";
    ls->shortcutShowVideoProp   = "Mostrar Propriedades Video";
    ls->shortcutShowAudioProp   = "Mostrar Propriedades de Som";
    ls->shortcutShowCtrlProp    = "Mostrar Propriedades dos Controles";
    ls->shortcutShowPerfProp    = "Mostrar Propriedades Performance";
    ls->shortcutShowSettProp    = "Mostrar Propriedades dos Ajustes";
    ls->shortcutShowPorts       = "Mostrar Propriedades das Portas";
    ls->shortcutShowLanguage    = "Mostrar Diálogo da Língua";
    ls->shortcutShowMachines    = "Mostrar Configuração da Máquina";
    ls->shortcutShowShortcuts   = "Mostrar Configuração de Atalhos";
    ls->shortcutShowKeyboard    = "Mostrar Configuração de Controladores/Teclado"; 
    ls->shortcutShowMixer       = "Mostre Mixer de Audio"; 
    ls->shortcutShowDebugger    = "Mostrar Debugger"; 
    ls->shortcutShowTrainer     = "Mostrar Trainer"; 
    ls->shortcutShowHelp        = "Ver Rubricas da Ajuda";
    ls->shortcutShowAbout       = "Ver Rubrica Sobre blueMSX";
    ls->shortcutShowFiles       = "Mostrar Propriedades de Arquivos";
    ls->shortcutToggleSpriteEnable = "Mostrar/esconder os sprites";
    ls->shortcutToggleFdcTiming = "sincronizar ou não disco drive"; 
    ls->shortcutToggleCpuTrace  = "Ativar/Desativar Traço da CPU"; 


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "Tecla MSX:"; 
    ls->keyconfigMappedTo       = "Tecla PC :"; 
    ls->keyconfigMappingScheme  = "Configuração do teclado:"; 

    
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

    ls->aboutScrollThanksTo     = "Agradecimentos especiais a: ";
    ls->aboutScrollAndYou       = "e VOCÊ !!!!";
};

#endif

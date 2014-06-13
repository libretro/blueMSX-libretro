/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageKorean.h,v $
**
** $Revision: 1.33 $ + additions 2005/03/03
**
** $Date: 2006/06/27 15:43:08 $
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
#ifndef LANGUAGE_KOREAN_H
#define LANGUAGE_KOREAN_H

#include "LanguageStrings.h"
 
void langInitKorean(LanguageStrings* ls)
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "중국어 간체";
    ls->langChineseTraditional  = "중국어 번체";
    ls->langDutch               = "네덜란드어";
    ls->langEnglish             = "영어";
    ls->langFinnish             = "핀란드어";
    ls->langFrench              = "프랑스어";
    ls->langGerman              = "독일어";
    ls->langItalian             = "이탈리아어";
    ls->langJapanese            = "일본어";
    ls->langKorean              = "한국어";
    ls->langPolish              = "폴란드어";
    ls->langPortuguese          = "포르투갈어";
    ls->langSpanish             = "스페인어";
    ls->langSwedish             = "스웨덴어";


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "장치:";
    ls->textFilename            = "파일 이름:";
    ls->textFile                = "파일";
    ls->textNone                = "없음";
    ls->textUnknown             = "알려지지 않음";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle             = "blueMSX - 경고";
    ls->warningDiscardChanges   = "변경한 설정이 적용되기 위해선 저장을 하셔야 합니다만 현재 설정에는 아무런 영향도 주지않고 그냥 나가겠습니까?";
    ls->warningOverwriteFile    = "파일을 덮어 쓰시겠습니까:"; // New in 2.5
    ls->errorTitle              = "blueMSX - 에러";
    ls->errorEnterFullscreen    = "전체 화면모드 진입 실패.           \n";
    ls->errorDirectXFailed      = "DirectX 오브젝트 만들기 실패.           \nGDI로 대체해서 사용합니다.\n그래픽카드 등록정보를 확인하세요.";
    ls->errorNoRomInZip         = "zip파일 내부의 .rom을 찾을 수 없습니다.";
    ls->errorNoDskInZip         = "zip파일 내부의 .dsk를 찾을 수 없습니다.";
    ls->errorNoCasInZip         = "zip파일 내부의 .cas를 찾을 수 없습니다.";
    ls->errorNoHelp             = "blueMSX 도움말을 찾을 수 없습니다.";
    ls->errorStartEmu           = "MSX 에뮬레이터를 시작할 수 없습니다.";
    ls->errorPortableReadonly   = "이동 장치는 읽기 전용입니다.";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "롬 이미지";
    ls->fileAll                 = "모든 파일";
    ls->fileCpuState            = "CPU 상태";
    ls->fileDisk                = "디스크 이미지";
    ls->fileCas                 = "테이프 이미지";


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "- 열어본 파일 없음 -";
    ls->menuInsert              = "삽입";
    ls->menuEject               = "꺼내기";

    ls->menuCartGameReader      = "게임 리더";                        // New in 2.5
    ls->menuCartIde             = "IDE";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise";                            // New in 2.5
    ls->menuCartSCC             = "SCC 카트리지";
    ls->menuCartSCCPlus         = "SCC+ 카트리지";
    ls->menuCartFMPac           = "FM-PAC 카트리지";
    ls->menuCartPac             = "PAC 카트리지";
    ls->menuCartHBI55           = "Sony HBI-55 카트리지";
    ls->menuCartInsertSpecial   = "특수 카트리지 삽입";                     // New in 2.5
    ls->menuCartMegaRam         = "메가램";                            // New in 2.5
    ls->menuCartExternalRam     = "외장램";

    ls->menuDiskInsertNew       = "새로운 디스크 이미지 삽입";              // New in 2.5
    ls->menuDiskDirInsert       = "디렉토리 삽입";
    ls->menuDiskAutoStart       = "삽입후에 재시작";
    ls->menuCartAutoReset       = "삽입/제거 후에 재시작";

    ls->menuCasRewindAfterInsert = "삽입후에 되감기";
    ls->menuCasUseReadOnly       = "읽기전용";
    ls->lmenuCasSaveAs           = "다른 이름으로 저장...";
    ls->menuCasSetPosition      = "위치 설정";
    ls->menuCasRewind           = "되감기";

    ls->menuPrnFormfeed         = "폼피드";

    ls->menuZoomNormal          = "보통 크기";
    ls->menuZoomDouble          = "두배 크기";
    ls->menuZoomFullscreen      = "전체 화면";
    
    ls->menuPropsEmulation      = "에뮬레이션";
    ls->menuPropsVideo          = "비디오";
    ls->menuPropsSound          = "사운드";
    ls->menuPropsControls       = "컨트롤";
    ls->menuPropsPerformance    = "성능";
    ls->menuPropsSettings        = "외부 설정";
    ls->menuPropsFile           = "파일";
    ls->menuPropsLanguage       = "언어";
    ls->menuPropsPorts          = "포트"; 
    
    ls->menuVideoSource         = "비디오 출력 소스";                   // New in 2.5
    ls->menuVideoSourceDefault  = "비디오 출력 소스 연결안됨";      // New in 2.5
    ls->menuVideoChipAutodetect = "비디오 칩 자동감지";    
    ls->menuVideoInSource       = "비디오 입력 소스";                    // New in 2.5
    ls->menuVideoInBitmap       = "비트맵 파일";                        // New in 2.5

    ls->menuHelpHelp            = "도움말 항목";
    ls->menuHelpAbout           = "blueMSX에 대하여";

    ls->menuFileCart            = "카트리지 슬롯";
    ls->menuFileDisk            = "디스크 드라이브";
    ls->menuFileCas             = "카세트";
    ls->menuFilePrn             = "프린터";
    ls->menuFileLoadState       = "CPU 상태 불러오기";
    ls->menuFileSaveState       = "CPU 상태 저장";
    ls->menuFileQLoadState      = "상태 바로 불러오기";
    ls->menuFileQSaveState      = "상태 바로 저장";
    ls->menuFileCaptureAudio    = "소리 저장";
    ls->menuFileScreenShot      = "화면 저장";
    ls->menuFileExit            = "끝내기";

    ls->menuFileHarddisk        = "하드 디스크";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "컨트롤러 존재하지 않음";             // New in 2.5

    ls->menuRunRun              = "실행";
    ls->menuRunPause            = "일시 정지";
    ls->menuRunStop             = "중지";
    ls->menuRunSoftReset        = "소프트 리셋";
    ls->menuRunHardReset        = "하드 리셋";
    ls->menuRunCleanReset       = "전체 리셋";

    ls->menuTools                = "도구";
    ls->menuToolsMachine         = "머신 편집기";
    ls->menuToolsShortcuts      = "단축키 편집기";
    ls->menuToolsKeyboard       = "키보드 편집기";
    ls->menuToolsDebugger       = "디버거";               // New in 2.5
    ls->menuToolsTrainer        = "트레이너";                // New in 2.5
    ls->menuToolsTraceLogger    = "트레이스 기록";           // New in 2.5

    ls->menuFile                = "파일";
    ls->menuRun                 = "실행";
    ls->menuWindow              = "윈도우";
    ls->menuToolsMixer          = "믹서";
    ls->menuOptions             = "옵션";
    ls->menuHelp                = "도움말";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "확인";
    ls->dlgOpen                 = "열기";
    ls->dlgCancel               = "취소";
    ls->dlgSave                 = "저장";
    ls->dlgSaveAs               = "다른 이름으로...";
    ls->dlgRun                  = "실행";
    ls->dlgClose                = "닫기";

    ls->dlgLoadRom              = "blueMSX - 카트리지에 삽입할 rom 이미지 선택";
    ls->dlgLoadDsk              = "blueMSX - 드라이브에 삽입할 dsk 이미지 선택";
    ls->dlgLoadCas              = "blueMSX - 카세트 플레이어에 넣을 cas 이미지 선택";
    ls->dlgLoadRomDskCas        = "blueMSX - 읽어 들일 rom, dsk, cas 이미지 선택";
    ls->dlgLoadRomDesc          = "카트리지에 삽입할 롬 이미지를 선택해 주세요:";
    ls->dlgLoadDskDesc          = "드라이브에 삽입할 첫번째 장 디스크 또는 디스크 이미지를 선택해 주세요(교환은 ALT+F9. 파일명과 숫자가 일정해야 됩니다):";
    ls->dlgLoadCasDesc          = "카세트 플레이어에 넣을 테잎 이미지를 선택해 주세요:";
    ls->dlgLoadRomDskCasDesc    = "읽어 들일 롬,디스크,또는 테잎 이미지를 선택해 주세요:";
    ls->dlgLoadState            = "CPU 상태 불러오기";
    ls->dlgSaveState            = "CPU 상태 저장";
    ls->dlgSaveCassette          = "blueMSX - 테잎 이미지 저장";
    ls->dlgInsertRom1           = "슬롯 1에 ROM 카트리지 삽입";
    ls->dlgInsertRom2           = "슬롯 2에 ROM 카트리지 삽입";
    ls->dlgInsertDiskA          = "드라이브 A에 디스크 삽입";
    ls->dlgInsertDiskB          = "드라이브 B에 디스크 삽입";
    ls->dlgInsertHarddisk       = "하드 디스크 삽입";                   // New in 2.5
    ls->dlgInsertCas            = "카세트 테잎 삽입";
    ls->dlgRomType              = "롬 형식:";

    ls->dlgTapeTitle            = "blueMSX - 테잎 위치";
    ls->dlgTapeFrameText        = "테잎 위치";
    ls->dlgTapeCurrentPos       = "현재 위치";
    ls->dlgTapeTotalTime        = "총 시간";
    ls->dlgTapeSetPosText        = "테잎 위치:";
    ls->dlgTapeCustom            = "사용자 지정 파일 보기";
    ls->dlgTabPosition           = "위치";
    ls->dlgTabType               = "형식";
    ls->dlgTabFilename           = "파일 이름";
    ls->dlgZipReset             = "삽입후 재시작";

    ls->dlgAboutTitle           = "blueMSX에 대하여";

    ls->dlgLangLangText         = "blueMSX에 사용할 언어 선택";
    ls->dlgLangLangTitle        = "blueMSX - 언어";

    ls->dlgAboutAbout           = "대하여\r\n=====";
    ls->dlgAboutVersion         = "버전:";
    ls->dlgAboutBuildNumber     = "빌드:";
    ls->dlgAboutBuildDate       = "날짜:";
    ls->dlgAboutCreat           = "제작자: Daniel Vik";
    ls->dlgAboutDevel           = "개발자 분들\r\n========";
    ls->dlgAboutThanks          = "도움을 주신 분들\r\n===========";
    ls->dlgAboutLisence         = "라이센스\r\n"
                                  "======\r\n\r\n"
				  "이 소프트웨어는 원본 그대로 배포되어야 하며, 명시적이든 암묵적이든 "
				  "어떤 종류의 보증도 하지 않습니다.\r\n이 소프트웨어의 사용으로 일어나는 "
				  "어떠한 문제에도 제작자에게는 책임이 없습니다.\r\n\r\n"
                                  "더 자세한 것은 www.bluemsx.com을 방문해 주세요.";

    ls->dlgSavePreview          = "미리 보기";
    ls->dlgSaveDate             = "시간 저장됨:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - 속성";
    ls->propEmulation           = "에뮬레이션";
    ls->propVideo               = "비디오";
    ls->propSound               = "사운드";
    ls->propControls            = "콘트롤";
    ls->propPerformance         = "성능";
    ls->propSettings             = "외부 설정";
    ls->propFile                = "파일";
    ls->propPorts               = "포트";
    
    ls->propEmuGeneralGB        = "일반 ";
    ls->propEmuFamilyText       = "MSX 머신:";
    ls->propEmuMemoryGB         = "메모리 ";
    ls->propEmuRamSizeText      = "램 크기:";
    ls->propEmuVramSizeText     = "비디오램 크기:";
    ls->propEmuSpeedGB          = "에뮬레이션 속도 ";
    ls->propEmuSpeedText        = "에뮬레이션 속도:";
    ls->propEmuFrontSwitchGB     = "파나소닉 스위치 ";
    ls->propEmuFrontSwitch       = " 프론트 스위치";
    ls->propEmuFdcTiming        = "플로피 드라이브 타이밍 사용 않함";
    ls->propEmuPauseSwitch      = " 일시 정지 스위치";
    ls->propEmuAudioSwitch       = " MSX-AUDIO 카트리지 스위치";
    ls->propVideoFreqText       = "비디오 주파수:";
    ls->propVideoFreqAuto       = "자동";
    ls->propSndOversampleText   = "오버샘플:";
    ls->propSndYkInGB           = "YK-01/YK-10/YK-20 입력 ";                // New in 2.5
    ls->propSndMidiInGB         = "MIDI 입력 ";
    ls->propSndMidiOutGB        = "MIDI 출력 ";
    ls->propSndMidiChannel      = "MIDI 채널:";                      // New in 2.5
    ls->propSndMidiAll          = "전부";                                // New in 2.5

    ls->propMonMonGB            = "모니터 ";
    ls->propMonTypeText         = "모니터 유형:";
    ls->propMonEmuText          = "모니터 에뮬레이션:";
    ls->propVideoTypeText       = "비디오 유형:";
    ls->propWindowSizeText      = "윈도우 크기:";
    ls->propMonHorizStretch      = " 수평 스트레치";
    ls->propMonVertStretch       = " 수직 스트레치";
    ls->propMonDeInterlace      = " 디인터레이스";
    ls->propBlendFrames         = " 인접한 프레임 색상혼합";           // New in 2.5
    ls->propMonColorGhosting    = "RF모듈레이터";
    ls->propMonBrightness       = "밝기:";
    ls->propMonContrast         = "대비:";
    ls->propMonSaturation       = "채도:";
    ls->propMonGamma            = "감마:";
    ls->propMonScanlines        = "스캔라인:";
    ls->propMonEffectsGB        = "효과";

    ls->propPerfVideoDrvGB      = "비디오 드라이버 ";
    ls->propPerfVideoDispDrvText= "화면 드라이버:";
    ls->propPerfFrameSkipText   = "프레임 건너띄기:";
    ls->propPerfAudioDrvGB      = "오디오 드라이버 ";
    ls->propPerfAudioDrvText    = "사운드 드라이버:";
    ls->propPerfAudioBufSzText  = "사운드 버퍼 사이즈:";
    ls->propPerfEmuGB           = "클럭 에뮬레이션 ";
    ls->propPerfSyncModeText    = "동기화 모드:";
    ls->propFullscreenResText   = "전체 화면 해상도:";

    ls->propSndChipEmuGB        = "사운드 칩 에뮬레이션 ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " GM에 MT-32 악기배열";

    ls->propPortsLptGB          = "패러럴 포트 "; 
    ls->propPortsComGB          = "시리얼 포트 "; 
    ls->propPortsLptText        = "포트:"; 
    ls->propPortsCom1Text       = "포트 1:";
    ls->propPortsNone           = "없음";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC";
    ls->propPortsFile           = "파일에 출력하기";
    ls->propPortsComFile        = "파일에 보내기";
    ls->propPortsOpenLogFile    = "로그파일 열기";
    ls->propPortsEmulateMsxPrn  = "흉내내기:";

    ls->propSetFileHistoryGB     = "열어본 파일 목록 ";
    ls->propSetFileHistorySize   = "파일 보관 크기:";
    ls->propSetFileHistoryClear  = "목록 지우기";
    ls->propFileTypes            = " blueMSX에 파일형식(.rom .dsk .cas .sta) 연결";
    ls->propWindowsEnvGB         = "윈도우즈 환경 "; 
    ls->propSetScreenSaver       = " blueMSX 실행중에는 화면 보호기 기능 중지";
    ls->propDisableWinKeys       = " blueMSX 실행중에는 왼/오른쪽 윈도우 키 사용 않함"; 
    ls->propPriorityBoost        = " blueMSX에 작업 우선권 주기";
    ls->propScreenshotPng       = " Portable Network Graphics (.png) 스크린샷 사용";  // New in 2.5
    ls->propClearHistory         = "열어본 파일 목록을 지울까요?";
    ls->propOpenRomGB           = "열때 항상 현재 설정 사용 ";
    ls->propDefaultRomType      = "기본 롬 형식:";
    ls->propGuessRomType        = "자동 판단";

    ls->propSettDefSlotGB       = "마우스로 끌어서 놓을때 ";
    ls->propSettDefSlots        = "현재 슬롯에 롬 삽입:";
    ls->propSettDefSlot         = " 슬롯";
    ls->propSettDefDrives       = "현재 드라이브에 디스켓 삽입:";
    ls->propSettDefDrive       = " 드라이브";

    ls->propThemeGB             = "테마 ";
    ls->propTheme               = "기본 테마:";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "컬러";
    ls->enumVideoMonGrey        = "검정과 흰색";
    ls->enumVideoMonGreen       = "녹색";
    ls->enumVideoMonAmber       = "호박색"; 

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "없음";
    ls->enumVideoEmuYc          = "Y/C 케이블 (선명)";
    ls->enumVideoEmuMonitor     = "모니터";
    ls->enumVideoEmuYcBlur      = "잡음있는 Y/C 케이블 (선명)";
    ls->enumVideoEmuComp        = "컴포지트 (흐릿)";
    ls->enumVideoEmuCompBlur    = "잡음있는 컴포지트 (흐릿)";
    ls->enumVideoEmuScale2x     = "Scale 2x";
    ls->enumVideoEmuHq2x        = "Hq2x";

    ls->enumVideoSize1x         = "보통 - 320x200";
    ls->enumVideoSize2x         = "두배 - 640x400";
    ls->enumVideoSizeFullscreen = "전체 화면";

    ls->enumVideoDrvDirectDrawHW = "DirectDraw HW 가속";
    ls->enumVideoDrvDirectDraw  = "DirectDraw";
    ls->enumVideoDrvGDI         = "GDI";

    ls->enumVideoFrameskip0     = "없음";
    ls->enumVideoFrameskip1     = "1 프레임";
    ls->enumVideoFrameskip2     = "2 프레임";
    ls->enumVideoFrameskip3     = "3 프레임";
    ls->enumVideoFrameskip4     = "4 프레임";
    ls->enumVideoFrameskip5     = "5 프레임";

    ls->enumSoundDrvNone        = "소리없음";
    ls->enumSoundDrvWMM         = "WMM 드라이버";
    ls->enumSoundDrvDirectX     = "DirectX 드라이버";

    ls->enumEmuSync1ms          = "MSX의 속도에 동기";
    ls->enumEmuSyncAuto         = "자동 (빠름)";
    ls->enumEmuSyncNone         = "하지않음";
    ls->enumEmuSyncVblank       = "PC의 수직주파수에 동기";
    ls->enumEmuAsyncVblank      = "PC의 수직주파수에 비동기";             // New in 2.5

    ls->enumControlsJoyNone     = "없음";
    ls->enumControlsJoyMouse    = "마우스";
    ls->enumControlsJoyTetris2Dongle = "테트리스 2 동글";
    ls->enumControlsJoyTMagicKeyDongle = "매직키 동글";             // New in 2.5
    ls->enumControlsJoy2Button = "2버튼 조이스틱";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "건 스틱";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="아스키 플러스-X 터미네이터 레이저";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision 조이스틱";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5 양면, 9 섹터";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5 양면, 8 섹터";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5 단면, 9 섹터";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5 단면, 8 섹터";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25 양면";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25 단면";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle                = "blueMSX - 머신 설정 편집기";
    ls->confConfigText           = "머신 설정";
    ls->confSlotLayout           = "슬롯 배치";
    ls->confMemory               = "메모리";
    ls->confChipEmulation        = "칩 에뮬레이션";
    ls->confChipExtras          = "기타";

    ls->confOpenRom             = "롬 이미지 열기";
    ls->confSaveTitle            = "blueMSX - 설정 저장";
    ls->confSaveText             = "이미 파일이 있습니다. 기존 파일을 이 파일로 바꾸시겠습니까?";
    ls->confSaveAsTitle         = "다른 이름으로 설정 저장...";
    ls->confSaveAsMachineName    = "머신 이름:";
    ls->confDiscardTitle         = "blueMSX - 설정";
    ls->confExitSaveTitle        = "blueMSX - 머신 설정 편집기 나가기";
    ls->confExitSaveText         = "변경한 설정이 적용되기 위해선 저장을 하셔야 합니다만 현재 설정에는 아무런 영향도 주지않고 그냥 나가겠습니까?";

    ls->confSlotLayoutGB         = "슬롯 배치 ";
    ls->confSlotExtSlotGB        = "외부 슬롯 ";
    ls->confBoardGB             = "시스템 ";
    ls->confBoardText           = "시스템 형식:";
    ls->confSlotPrimary          = "기본";
    ls->confSlotExpanded         = "확장 (4개의 서브슬롯)";

    ls->confSlotCart            = "카트리지";
    ls->confSlot                = "슬롯";
    ls->confSubslot             = "서브 슬롯";

    ls->confMemAdd               = "추가...";
    ls->confMemEdit              = "편집...";
    ls->confMemRemove            = "제거";
    ls->confMemSlot              = "슬롯";
    ls->confMemAddresss          = "주소";
    ls->confMemType              = "형식";
    ls->confMemRomImage          = "롬 이미지";
    
    ls->confChipVideoGB          = "비디오 ";
    ls->confChipVideoChip        = "비디오 칩:";
    ls->confChipVideoRam         = "비디오 램:";
    ls->confChipSoundGB          = "사운드 ";

    ls->confCmosGB                = "CMOS ";
    ls->confCmosEnable            = " CMOS 사용";
    ls->confCmosBattery           = " 충전지 사용";

    ls->confCpuFreqGB            = "CPU 주파수 ";
    ls->confZ80FreqText          = "Z80 주파수:";
    ls->confR800FreqText         = "R800 주파수:";
    ls->confFdcGB                = "플로피 디스크 컨트롤러 ";
    ls->confCFdcNumDrivesText    = "드라이브의 갯수:";

    ls->confEditMemTitle         = "blueMSX - 맵퍼 편집";
    ls->confEditMemGB            = "맵퍼 항목 ";
    ls->confEditMemType          = "형식:";
    ls->confEditMemFile          = "파일:";
    ls->confEditMemAddress       = "주소";
    ls->confEditMemSize          = "크기";
    ls->confEditMemSlot          = "슬롯";


    //----------------------
    // Shortcut lines
    //----------------------

	ls->shortcutKey             = "핫키";
    ls->shortcutDescription     = "단축키";

    ls->shortcutSaveConfig      = "blueMSX - 설정 저장";
    ls->shortcutOverwriteConfig = "이미 파일이 있습니다. 기존 파일을 이 파일로 바꾸시겠습니까?";
    ls->shortcutExitConfig      = "blueMSX - 단축키 편집기 나가기";
    ls->shortcutDiscardConfig   = "변경한 설정이 적용되기 위해선 저장을 하셔야 합니다만 현재 설정에는 아무런 영향도 주지않고 그냥 나가겠습니까?";
    ls->shortcutSaveConfigAs    = "blueMSX - 다른이름으로 단축키 설정 저장...";
    ls->shortcutConfigName      = "설정 이름:";
    ls->shortcutNewProfile      = "< 새로운 프로필 >";
    ls->shortcutConfigTitle     = "blueMSX - 단축키 매핑 편집기";
    ls->shortcutAssign          = "적용";
    ls->shortcutPressText       = "단축키 새로 설정:";
    ls->shortcutScheme          = "단축키 스타일:";
    ls->shortcutCartInsert1     = "카트리지 1 삽입";
    ls->shortcutCartRemove1     = "카트리지 1 제거";
    ls->shortcutCartInsert2     = "카트리지 2 삽입";
    ls->shortcutCartRemove2     = "카트리지 2 제거";
    ls->shortcutSpecialMenu1    = "카트리지 1 - 사용자 지정 카트리지";
    ls->shortcutSpecialMenu2    = "카트리지 2 - 사용자 지정 카트리지";
    ls->shortcutCartAutoReset   = "카트리지 삽입후 에뮬레이터 재시작";
    ls->shortcutDiskInsertA     = "디스켓 A 삽입";
    ls->shortcutDiskDirInsertA  = "디스켓 A로 디렉토리 삽입";
    ls->shortcutDiskRemoveA     = "디스켓 A 꺼내기";
    ls->shortcutDiskChangeA     = "디스켓 A 빠른 교환(zip압축 파일)";
    ls->shortcutDiskAutoResetA  = "디스켓 A 삽입후 에뮬레이터 재시작";
    ls->shortcutDiskInsertB     = "디스켓 B 삽입";
    ls->shortcutDiskDirInsertB  = "디스켓 B로 디렉토리 삽입";
    ls->shortcutDiskRemoveB     = "디스켓 B 꺼내기";
    ls->shortcutCasInsert       = "카세트 삽입";
    ls->shortcutCasEject        = "카세트 꺼내기";
    ls->shortcutCasAutorewind   = "카세트 자동 되감기 기능 변환";
    ls->shortcutCasReadOnly     = "카세트 읽기전용 변환";
    ls->shortcutCasSetPosition  = "테잎 위치 설정";
    ls->shortcutCasRewind       = "카세트 되감기";
    ls->shortcutCasSave         = "카세트 이미지 저장";
    ls->shortcutPrnFormFeed     = "프린터 폼피드";
    ls->shortcutCpuStateLoad    = "CPU 상태 불러오기";
    ls->shortcutCpuStateSave    = "CPU 상태 저장";
    ls->shortcutCpuStateQload   = "상태 바로 불러오기";
    ls->shortcutCpuStateQsave   = "상태 바로 저장";
    ls->shortcutAudioCapture    = "소리 저장 시작/중지";
    ls->shortcutScreenshotOrig  = "화면 저장";
    ls->shortcutScreenshotSmall = "작은 크기로 화면 저장(원본)";
    ls->shortcutScreenshotLarge = "큰 크기로 화면 저장(원본)";
    ls->shortcutQuit            = "blueMSX 끝내기";
    ls->shortcutRunPause        = "에뮬레이션 시작/일시 정지";
    ls->shortcutStop            = "에뮬레이션 중지";
    ls->shortcutResetHard       = "하드 리셋";
    ls->shortcutResetSoft       = "소프트 리셋";
    ls->shortcutResetClean      = "전체 리셋";
    ls->shortcutSizeSmall       = "작은 윈도우 크기로 설정";
    ls->shortcutSizeNormal      = "보통 윈도우 크기로 설정";
    ls->shortcutSizeFullscreen  = "전체 화면으로 설정";
    ls->shortcutSizeMinimized   = "윈도우 최소화";
    ls->shortcutToggleFullscren = "전체 화면 변환";
    ls->shortcutVolumeIncrease  = "볼륨 증가";
    ls->shortcutVolumeDecrease  = "볼륨 감소";
    ls->shortcutVolumeMute      = "음소거";
    ls->shortcutVolumeStereo    = "모노/스테레오 변환";
    ls->shortcutSwitchMsxAudio  = "MSX-AUDIO 스위치 변환";
    ls->shortcutSwitchFront     = "파나소닉 프론트 스위치 변환";
    ls->shortcutSwitchPause     = "일시 정지 스위치";
    ls->shortcutToggleMouseLock = "마우스 고정 변환";
    ls->shortcutEmuSpeedMax     = "최대 에뮬레이션 속도";
    ls->shortcutEmuSpeedToggle  = "에뮬레이션 속도 최대화 변환";
    ls->shortcutEmuSpeedNormal  = "보통 에뮬레이션 속도";
    ls->shortcutEmuSpeedInc     = "에뮬레이션 속도 증가";
    ls->shortcutEmuSpeedDec     = "에뮬레이션 속도 감소";
    ls->shortcutThemeSwitch     = "테마 변환";
    ls->shortcutShowEmuProp     = "에뮬레이션 속성 보기";
    ls->shortcutShowVideoProp   = "비디오 속성 보기";
    ls->shortcutShowAudioProp   = "오디오 속성 보기";
    ls->shortcutShowCtrlProp    = "콘트롤 속성 보기";
    ls->shortcutShowPerfProp    = "성능 속성 보기";
    ls->shortcutShowSettProp    = "외부 설정 속성 보기";
    ls->shortcutShowPorts       = "포트 속성 보기";
    ls->shortcutShowLanguage    = "언어 보기";
    ls->shortcutShowMachines    = "머신 편집기 보기";
    ls->shortcutShowShortcuts   = "단축키 편집기 보기";
    ls->shortcutShowKeyboard    = "키보드 편집기 보기";
    ls->shortcutShowDebugger    = "디버거 보기"; 
    ls->shortcutShowTrainer     = "트레이너 보기"; 
    ls->shortcutShowMixer       = "믹서 보기";
    ls->shortcutShowHelp        = "도움말 보기";
    ls->shortcutShowAbout       = "blueMSX에 대하여 보기";
    ls->shortcutShowFiles       = "파일 속성 보기";
    ls->shortcutToggleSpriteEnable = "스프라이트 보이기/숨기기 변환";
    ls->shortcutToggleFdcTiming = "플로피 드라이브 타이밍 사용/않함 변환";
    ls->shortcutToggleCpuTrace  = "CPU 트레이스 변환";


    //----------------------
    // Keyboard config lines
    //----------------------

    ls->keyconfigSelectedKey    = "선택된 키:";
    ls->keyconfigMappedTo       = "대응된 키:";
    ls->keyconfigMappingScheme  = "매핑 스타일:";

    
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
    ls->aboutScrollThanksTo     = "Special thanks to: ";
    ls->aboutScrollAndYou       = "and YOU !!!!";
};

#endif

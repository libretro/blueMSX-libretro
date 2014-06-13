/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Language/LanguageJapanese.h,v $
**
** $Revision: 1.37 $
**
** $Date: 2006/07/07 00:20:19 $
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
#ifndef LANGUAGE_JAPANESE_H
#define LANGUAGE_JAPANESE_H

#include "LanguageStrings.h"

void langInitJapanese(LanguageStrings* ls) 
{
    //----------------------
    // Language lines
    //----------------------

    ls->langChineseSimplified   = "中国語(簡体字)";         // New in 2.5
    ls->langChineseTraditional  = "中国語(繁体字)";        // New in 2.5
    ls->langDutch               = "オランダ語";                      // New in 2.5
    ls->langEnglish             = "英語";                    // New in 2.5
    ls->langFinnish             = "フィンランド語";                    // New in 2.5
    ls->langFrench              = "フランス語";                     // New in 2.5
    ls->langGerman              = "ドイツ語";                     // New in 2.5
    ls->langItalian             = "イタリア語";                    // New in 2.5
    ls->langJapanese            = "日本語";                   // New in 2.5
    ls->langKorean              = "韓国語";                     // New in 2.5
    ls->langPolish              = "ポーランド語";                     // New in 2.5
    ls->langPortuguese          = "ポルトガル語";                 // New in 2.5
    ls->langSpanish             = "スペイン語";                    // New in 2.5
    ls->langSwedish             = "スウェーデン語";                    // New in 2.5


    //----------------------
    // Generic lines
    //----------------------

    ls->textDevice              = "デバイス:"; 
    ls->textFilename            = "ファイル名:"; 
    ls->textFile                = "ファイル"; 
    ls->textNone                = "なし"; 
    ls->textUnknown             = "不明";                            // New in 2.5


    //----------------------
    // Warning and Error lines
    //----------------------

    ls->warningTitle            = "blueMSX - 警告";
    ls->warningDiscardChanges   = "構\成が失われますが、よろしいですか?";
    ls->warningOverwriteFile    = "このファイルに上書きしてもよろしいですか:"; // New in 2.5
    ls->errorTitle              = "blueMSX - エラー";
    ls->errorEnterFullscreen    = "フルスクリーンモードにできません。          \n";
    ls->errorDirectXFailed      = "DirectX オブジェクトを作成できません。          \nGDIを代わりに使用します。\nグラフィックカードの設定を確認してください。";
    ls->errorNoRomInZip         = "zipファイルの中に.romファイルが見つかりません。";
    ls->errorNoDskInZip         = "zipファイルの中に.dskファイルが見つかりません。";
    ls->errorNoCasInZip         = "zipファイルの中に.casファイルが見つかりません。";
    ls->errorNoHelp             = "blueMSX のヘルプファイルが見つかりません。";
    ls->errorStartEmu           = "blueMSX の開始に失敗しました。";
    ls->errorPortableReadonly   = "ポータブルデバイスは読込専用です。";        // New in 2.5


    //----------------------
    // File related lines
    //----------------------

    ls->fileRom                 = "ROM イメージ"; 
    ls->fileAll                 = "全てのファイル"; 
    ls->fileCpuState            = "CPU ステート"; 
    ls->fileDisk                = "ディスクイメージ"; 
    ls->fileCas                 = "テープイメージ"; 


    //----------------------
    // Menu related lines
    //----------------------

    ls->menuNoRecentFiles       = "(使用ファイルなし)";
    ls->menuInsert              = "挿入";
    ls->menuEject               = "取り出し";

    ls->menuCartGameReader      = "ゲームリーダーに接続";                        // New in 2.5
    ls->menuCartIde             = "IDE カートリッジ";                                // New in 2.5
    ls->menuCartBeerIde         = "Beer IDE";                               // New in 2.5
    ls->menuCartGIde            = "GIDE";                               // New in 2.5
    ls->menuCartSunriseIde      = "Sunrise IDE";                            // New in 2.5
    ls->menuCartSCC              = "SCC カートリッジ";
    ls->menuCartSCCPlus          = "SCC+ カートリッジ";
    ls->menuCartFMPac           = "FMPAC+ カートリッジ";
    ls->menuCartPac             = "PAC+ カートリッジ";
    ls->menuCartHBI55           = "Sony HBI-55 カートリッジ";
    ls->menuCartInsertSpecial   = "特殊カートリッジ";                     // New in 2.5
    ls->menuCartMegaRam         = "メガRAM カートリッジ";                            // New in 2.5
    ls->menuCartExternalRam     = "外部RAM カートリッジ"; 

    ls->menuDiskInsertNew       = "新規ディスクイメージを挿入";              // New in 2.5
    ls->menuDiskDirInsert       = "ディレクトリ挿入"; 
    ls->menuDiskAutoStart       = "挿入/取り出し後リセット";
    ls->menuCartAutoReset       = "挿入/取り出し後リセット";

    ls->menuCasRewindAfterInsert = "挿入後巻き戻す";
    ls->menuCasUseReadOnly       = "テープを読み込み専用にする";
    ls->lmenuCasSaveAs           = "テープを別名で保存...";
    ls->menuCasSetPosition      = "テープ位置セット";
    ls->menuCasRewind           = "巻き戻し";
    
    ls->menuPrnFormfeed         = "紙送り"; 

    ls->menuZoomNormal          = "標準サイズ";
    ls->menuZoomDouble          = "2倍サイズ";
    ls->menuZoomFullscreen      = "フルスクリーン";
    
    ls->menuPropsEmulation      = "エミュレーション";
    ls->menuPropsVideo          = "ビデオ";
    ls->menuPropsSound          = "サウンド";
    ls->menuPropsControls       = "コントロール";
    ls->menuPropsPerformance    = "パフォーマンス";
    ls->menuPropsSettings        = "その他";
    ls->menuPropsFile           = "ファイル";
    ls->menuPropsLanguage       = "言語";
    ls->menuPropsPorts          = "ポート";
    
    ls->menuVideoChipAutodetect = "ビデオチップの自動認識";
    ls->menuVideoSource         = "ビデオ出力";                   // New in 2.5
    ls->menuVideoSourceDefault  = "ビデオ出力が見つかりません";      // New in 2.5    
    ls->menuVideoInSource       = "ビデオ入力";                    // New in 2.5
    ls->menuVideoInBitmap       = "ビットマップファイル";                        // New in 2.5

    ls->menuHelpHelp            = "ヘルプ目次";
    ls->menuHelpAbout           = "blueMSXについて";

    ls->menuFileCart            = "ROMスロット";
    ls->menuFileDisk            = "ディスクドライブ";
    ls->menuFileCas             = "カセットテープ";
    ls->menuFilePrn             = "プリンター"; 
    ls->menuFileLoadState       = "CPUステート読込";
    ls->menuFileSaveState       = "CPUステート保存";
    ls->menuFileQLoadState      = "クイックロード";
    ls->menuFileQSaveState      = "クイックセーブ";
    ls->menuFileCaptureAudio    = "録音";
    ls->menuFileScreenShot      = "スクリーンショット";
    ls->menuFileExit            = "終了";

    ls->menuFileHarddisk        = "ハードディスク";                          // New in 2.5
    ls->menuFileHarddiskNoPesent= "ハードディスクは無効です";             // New in 2.5

    ls->menuRunRun              = "実行";
    ls->menuRunPause            = "中断";
    ls->menuRunStop             = "停止";
    ls->menuRunSoftReset        = "ソ\フトリセット";
    ls->menuRunHardReset        = "ハードリセット";
    ls->menuRunCleanReset       = "クリーンリセット";

    ls->menuToolsMachine         = "マシン構\成編集";
    ls->menuToolsShortcuts      = "ショートカット構\成編集";
    ls->menuToolsKeyboard       = "キーボード構\成編集";
    ls->menuToolsMixer          = "ミキサー"; 
    ls->menuToolsDebugger       = "デバッガー";               // New in 2.5
    ls->menuToolsTrainer        = "トレーナー";                // New in 2.5
    ls->menuToolsTraceLogger    = "トレースロガー";           // New in 2.5

    ls->menuFile                = "ファイル";
    ls->menuRun                 = "実行";
    ls->menuWindow              = "ウィンドウ";
    ls->menuOptions             = "オプション";
    ls->menuTools                = "ツール";
    ls->menuHelp                = "ヘルプ";


    //----------------------
    // Dialog related lines
    //----------------------

    ls->dlgOK                   = "OK";
    ls->dlgOpen                  = "開く";
    ls->dlgCancel               = "キャンセル";
    ls->dlgSave                 = "保存";
    ls->dlgSaveAs               = "別名保存...";
    ls->dlgRun                  = "起動";
    ls->dlgClose                = "閉じる";

    ls->dlgLoadRom              = "blueMSX - romイメージを選択";
    ls->dlgLoadDsk              = "blueMSX - dskイメージを選択";
    ls->dlgLoadCas              = "blueMSX - casイメージを選択";
    ls->dlgLoadRomDskCas        = "blueMSX - rom, dsk, またはcasイメージを選択";
    ls->dlgLoadRomDesc          = "読み込むromイメージ:";
    ls->dlgLoadDskDesc          = "読み込むdskイメージ:";
    ls->dlgLoadCasDesc          = "読み込むcasイメージ:";
    ls->dlgLoadRomDskCasDesc    = "読み込むrom, dsk, casイメージ:";
    ls->dlgLoadState            = "CPUステート 読み込み";
    ls->dlgSaveState            = "CPUステート 保存";
    ls->dlgSaveCassette          = "blueMSX - テープを保存";
    ls->dlgInsertRom1           = "ROM イメージをスロット1に挿入";
    ls->dlgInsertRom2           = "ROM イメージをスロット2に挿入";
    ls->dlgInsertDiskA          = "ディスクイメージをドライブAに挿入";
    ls->dlgInsertDiskB          = "ディスクイメージをドライブBに挿入";
    ls->dlgInsertHarddisk       = "ハードディスクを挿入";                   // New in 2.5
    ls->dlgInsertCas            = "カセットテープを挿入";
    ls->dlgRomType              = "ROM タイプ:"; 

    ls->dlgTapeTitle            = "blueMSX - カセットテープ";
    ls->dlgTapeFrameText        = "テープ位置";
    ls->dlgTapeCurrentPos       = "現在位置";
    ls->dlgTapeTotalTime        = "トータルタイム";
    ls->dlgTapeSetPosText        = "テープ位置:";
    ls->dlgTapeCustom            = "カスタムファイルを表\示";
    ls->dlgTabPosition           = "位置";
    ls->dlgTabType               = "タイプ";
    ls->dlgTabFilename           = "ファイル名";
    ls->dlgZipReset             = "挿入後リセット";

    ls->dlgAboutTitle           = "blueMSX について";

    ls->dlgLangLangText         = "blueMSX で使用する言語を選択してください";
    ls->dlgLangLangTitle        = "blueMSX - 言語";

    ls->dlgAboutAbout           = "このソ\フトについて\r\n===========";
    ls->dlgAboutVersion         = "バージョン:";
    ls->dlgAboutBuildNumber     = "ビルド:";
    ls->dlgAboutBuildDate       = "ビルド日付:";
    ls->dlgAboutCreat           = "製作:\tDaniel Vik";
    ls->dlgAboutDevel           = "開発者\r\n=====";
    ls->dlgAboutThanks          = "ご協力いただいた方々\r\n=============";
    ls->dlgAboutLisence         = "ライセンス\r\n"
                                  "======\r\n\r\n"
                                  "このソ\フトウェアは現状をそのままの状態で配布するものであり、明示的または"
                                  "黙示的を問わず一切の保証はありません。また、このソ\フトウェアの使用の結果生じた"
                                  "いかなる損害についても、作者は責任を負いません。\r\n\r\n"
                                  "詳しい説明については、www.bluemsx.com をご覧ください。";

    ls->dlgSavePreview          = "プレビュー表\示";
    ls->dlgSaveDate             = "セーブ時刻:";


    //----------------------
    // Properties related lines
    //----------------------

    ls->propTitle               = "blueMSX - 設定";
    ls->propEmulation           = "エミュレーション";
    ls->propVideo               = "ビデオ";
    ls->propSound               = "サウンド";
    ls->propControls            = "コントロール";
    ls->propPerformance         = "パフォーマンス";
    ls->propSettings             = "その他";
    ls->propFile                = "ファイル"; 
    ls->propPorts               = "ポート";
    
    ls->propEmuGeneralGB        = " 一般 ";
    ls->propEmuFamilyText       = "マシン構\成:";
    ls->propEmuMemoryGB         = " メモリ ";
    ls->propEmuRamSizeText      = "メインRAM:";
    ls->propEmuVramSizeText     = "VRAM:";
    ls->propEmuSpeedGB          = " スピード ";
    ls->propEmuSpeedText        = "スピード:";
    ls->propEmuFrontSwitchGB     = " パナソ\ニックスイッチ "; 
    ls->propEmuFrontSwitch       = " 前面[内蔵ソ\フト]スイッチ"; 
    ls->propEmuFdcTiming        = " FDDドライブのタイミングを無効化";
    ls->propEmuPauseSwitch      = " ポーズスイッチ";
    ls->propEmuAudioSwitch       = " MSX-AUDIO カートリッジスイッチ"; 
    ls->propVideoFreqText       = "ビデオ周波数:";
    ls->propVideoFreqAuto       = "自動";
    ls->propSndOversampleText   = "サンプリング:";
    ls->propSndYkInGB           = " YK-01/YK-10/YK-20 イン ";                // New in 2.5
    ls->propSndMidiInGB         = " MIDI イン "; 
    ls->propSndMidiOutGB        = " MIDI アウト "; 
    ls->propSndMidiChannel      = "MIDI チャンネル:";                      // New in 2.5
    ls->propSndMidiAll          = "全て";                                // New in 2.5

    ls->propMonMonGB            = " モニター ";
    ls->propMonTypeText         = "モニターの種類:";
    ls->propMonEmuText          = "モニターエミュレーション:";
    ls->propVideoTypeText       = "映像方式:";
    ls->propWindowSizeText      = "ウィンドウサイズ:";
    ls->propMonHorizStretch      = " 水平方向に拡大";  
    ls->propMonVertStretch       = " 垂直方向に拡大";
    ls->propMonDeInterlace      = " インターレース除去";
    ls->propBlendFrames         = " 残像を残す";           // New in 2.5
    ls->propMonBrightness       = "明るさ:";
    ls->propMonContrast         = "コントラスト:";
    ls->propMonSaturation       = "彩度:";
    ls->propMonGamma            = "ガンマ:";
    ls->propMonScanlines        = " スキャンライン:";
    ls->propMonColorGhosting    = " RFモジュレータ:";
    ls->propMonEffectsGB        = " エフェクト "; 

    ls->propPerfVideoDrvGB      = " ビデオドライバ ";
    ls->propPerfVideoDispDrvText= "ディスプレイドライバ:";
    ls->propPerfFrameSkipText   = "フレームスキップ:";
    ls->propPerfAudioDrvGB      = " サウンドドライバ ";
    ls->propPerfAudioDrvText    = "サウンドドライバ:";
    ls->propPerfAudioBufSzText  = "サウンドバッファ:";
    ls->propPerfEmuGB           = " ビデオのエミュレーション ";
    ls->propPerfSyncModeText    = "同期モード:";
    ls->propFullscreenResText   = "フルスクリーンの解像度:";

    ls->propSndChipEmuGB        = " サウンドのエミュレーション ";
    ls->propSndMsxMusic         = " MSX-MUSIC";
    ls->propSndMsxAudio         = " MSX-AUDIO";
    ls->propSndMoonsound         = " Moonsound";
    ls->propSndMt32ToGm         = " MT-32の音色をGM音源に割り当て"; 

    ls->propPortsLptGB          = " パラレルポート ";
    ls->propPortsComGB          = " シリアルポート ";
    ls->propPortsLptText        = "ポート:";
    ls->propPortsCom1Text       = "ポート 1:";
    ls->propPortsNone           = "なし";
    ls->propPortsSimplCovox     = "SiMPL / Covox DAC";
    ls->propPortsFile           = "ログファイルへ出力"; 
    ls->propPortsComFile        = "ログファイルへ出力"; 
    ls->propPortsOpenLogFile    = "ログファイルを開く"; 
    ls->propPortsEmulateMsxPrn  = "エミュレート:"; 

    ls->propSetFileHistoryGB     = " ファイルヒストリ ";
    ls->propSetFileHistorySize   = "ファイルヒストリの個数:";
    ls->propSetFileHistoryClear  = "ヒストリの消去";
    ls->propFileTypes            = " blueMSX で使用するファイル (rom, dsk, cas, sta) を関連付け";
    ls->propWindowsEnvGB         = " Windows ";
    ls->propSetScreenSaver       = " blueMSX の動作中はスクリーンセーバを無効にする";
    ls->propDisableWinKeys       = " Windows キー及び漢字キーを blueMSX に割り当てる"; 
    ls->propPriorityBoost        = " Windows で blueMSX の優先度を高くする";
    ls->propScreenshotPng       = " スクリーンショットを PNG 形式で取得する";  // New in 2.5
    ls->propClearHistory         = "ヒストリの消去を実行してよろしいですか?"; 
    ls->propOpenRomGB           = " ROM イメージ "; 
    ls->propDefaultRomType      = "デフォルトROMタイプ:"; 
    ls->propGuessRomType        = "ROMタイプを自動判定";

    ls->propSettDefSlotGB       = " ドラッグ＆ドロップ "; 
    ls->propSettDefSlots        = "ROM を挿入するスロット:"; 
    ls->propSettDefSlot         = " スロット"; 
    ls->propSettDefDrives       = "ディスクを挿入するドライブ:"; 
    ls->propSettDefDrive        = " ドライブ"; 

    ls->propThemeGB             = " テーマ ";
    ls->propTheme               = "テーマ:";


    //----------------------
    // Dropdown related lines
    //----------------------

    ls->enumVideoMonColor       = "カラー";
    ls->enumVideoMonGrey        = "モノクロ";
    ls->enumVideoMonGreen       = "グリーン";
    ls->enumVideoMonAmber       = "アンバー";

    ls->enumVideoTypePAL        = "PAL";
    ls->enumVideoTypeNTSC       = "NTSC";

    ls->enumVideoEmuNone        = "なし";
    ls->enumVideoEmuYc          = "Y/Cケーブル (鮮明)";
    ls->enumVideoEmuMonitor     = "モニター";
    ls->enumVideoEmuYcBlur      = "Y/Cケーブル＋ノイズ (鮮明)";
    ls->enumVideoEmuComp        = "コンポジット (不鮮明)";
    ls->enumVideoEmuCompBlur    = "コンポジット＋ノイズ (不鮮明)";
    ls->enumVideoEmuScale2x     = "Scale 2x";
    ls->enumVideoEmuHq2x        = "Hq2x";

    ls->enumVideoSize1x         = "標準サイズ - 320x200";
    ls->enumVideoSize2x         = "2倍サイズ - 640x400";
    ls->enumVideoSizeFullscreen = "フルスクリーン";

    ls->enumVideoDrvDirectDrawHW= "DirectDraw ハードウェア";           // New in 2.5
    ls->enumVideoDrvDirectDraw  = "DirectDraw ソ\フトウェア";                     // New in 2.5
    ls->enumVideoDrvGDI         = "GDI";                            // New in 2.5

    ls->enumVideoFrameskip0     = "なし";
    ls->enumVideoFrameskip1     = "1フレーム";
    ls->enumVideoFrameskip2     = "2フレーム";
    ls->enumVideoFrameskip3     = "3フレーム";
    ls->enumVideoFrameskip4     = "4フレーム";
    ls->enumVideoFrameskip5     = "5フレーム";

    ls->enumSoundDrvNone        = "なし";
    ls->enumSoundDrvWMM         = "WMM";
    ls->enumSoundDrvDirectX     = "DirectX";
    
    ls->enumEmuSync1ms          = "MSXに同期"; 
    ls->enumEmuSyncAuto         = "オート(高速)"; 
    ls->enumEmuSyncNone         = "なし"; 
    ls->enumEmuSyncVblank       = "Windowsに同期"; 
    ls->enumEmuAsyncVblank      = "非同期";             // New in 2.5

    ls->enumControlsJoyNone     = "なし";
    ls->enumControlsJoyMouse    = "マウス";
    ls->enumControlsJoyTetris2Dongle = "テトリス2 ドングル"; 
    ls->enumControlsJoyTMagicKeyDongle = "MagicKey ドングル";             // New in 2.5
    ls->enumControlsJoy2Button = "2ボタン ジョイスティック";                   // New in 2.5
    ls->enumControlsJoyGunstick  = "ガンスティック";                         // New in 2.5
    ls->enumControlsJoyAsciiLaser="ASCII Plus-X ターミネーターレーザー";      // New in 2.5
    ls->enumControlsJoyColeco = "ColecoVision ジョイスティック";                // New in 2.5

    ls->enumDiskMsx35Dbl9Sect    = "MSX 3.5インチ 2DD (9セクタ)";     // New in 2.5
    ls->enumDiskMsx35Dbl8Sect    = "MSX 3.5インチ 2DD (8セクタ)";     // New in 2.5
    ls->enumDiskMsx35Sgl9Sect    = "MSX 3.5インチ 2D (9セクタ)";     // New in 2.5
    ls->enumDiskMsx35Sgl8Sect    = "MSX 3.5インチ 2D (8セクタ)";     // New in 2.5
    ls->enumDiskSvi525Dbl        = "SVI-328 5.25インチ 2DD";           // New in 2.5
    ls->enumDiskSvi525Sgl        = "SVI-328 5.25インチ 2D";           // New in 2.5


    //----------------------
    // Configuration related lines
    //----------------------

    ls->confTitle                = "blueMSX - マシン構\成編集";
    ls->confConfigText           = "マシン構\成:";
    ls->confSlotLayout           = "スロット";
    ls->confMemory               = "メモリ";
    ls->confChipEmulation        = "チップ";
    ls->confChipExtras          = "その他";

    ls->confOpenRom             = "ROMイメージを開く"; 
    ls->confSaveTitle            = "blueMSX - マシン構\成編集の終了";
    ls->confSaveText             = "現在の構\成が失われますが、よろしいですか?";    
    ls->confSaveAsTitle         = "別名で保存...";
    ls->confSaveAsMachineName    = "構\成名:";
    ls->confDiscardTitle         = "blueMSX - マシン構\成編集";
    ls->confExitSaveTitle        = "blueMSX - マシン構\成編集の終了";
    ls->confExitSaveText         = "現在の構\成が失われますが、よろしいですか?";

    ls->confSlotLayoutGB         = " スロット構\成 ";
    ls->confSlotExtSlotGB        = " 外部スロット ";
    ls->confBoardGB             = " 基盤 ";
    ls->confBoardText           = "基盤タイプ:";
    ls->confSlotPrimary          = "基本";
    ls->confSlotExpanded         = "拡張(4つの拡張スロット)";

    ls->confSlotCart             = "カートリッジ";
    ls->confSlot                = "スロット"; 
    ls->confSubslot             = "拡張スロット"; 

    ls->confMemAdd               = "追加...";
    ls->confMemEdit              = "編集...";
    ls->confMemRemove            = "削除";
    ls->confMemSlot              = "スロット";
    ls->confMemAddresss          = "アドレス";
    ls->confMemType              = "タイプ";
    ls->confMemRomImage          = "ROM イメージ";
    
    ls->confChipVideoGB          = " ビデオ ";
    ls->confChipVideoChip        = "ビデオチップ:";
    ls->confChipVideoRam         = "ビデオ RAM:";
    ls->confChipSoundGB          = " サウンド ";

    ls->confCmosGB                = " CMOS "; 
    ls->confCmosEnable            = " CMOS有効"; 
    ls->confCmosBattery           = " 内蔵バッテリー有効";

    ls->confCpuFreqGB            = " CPU 周波数 ";
    ls->confZ80FreqText          = "Z80 周波数:";
    ls->confR800FreqText         = "R800 周波数:";
    ls->confFdcGB                = " フロッピーディスクコントローラー ";
    ls->confCFdcNumDrivesText    = "ドライブの数:";

    ls->confEditMemTitle         = "blueMSX - マッパの編集";
    ls->confEditMemGB            = " マッパの詳細 ";
    ls->confEditMemType          = "タイプ:";
    ls->confEditMemFile          = "ファイル:";
    ls->confEditMemAddress       = "アドレス:";
    ls->confEditMemSize          = "サイズ:";
    ls->confEditMemSlot          = "スロット:";


    //----------------------
    // Shortcut lines
    //----------------------

    ls->shortcutKey             = "機能\"; 
    ls->shortcutDescription     = "ショートカット"; 

    ls->shortcutSaveConfig      = "blueMSX - 構\成の保存";
    ls->shortcutOverwriteConfig = "ショートカット構\成を上書きしてよろしいですか?:";
    ls->shortcutExitConfig      = "blueMSX - ショートカット構\成編集の終了";
    ls->shortcutDiscardConfig   = "現在の構\成が失われますが、よろしいですか?";
    ls->shortcutSaveConfigAs    = "blueMSX - ショートカット構\成を別名で保存";
    ls->shortcutConfigName      = "構\成名:";
    ls->shortcutNewProfile      = "< 新規プロファイル >";
    ls->shortcutConfigTitle     = "blueMSX - ショートカット構\成編集";
    ls->shortcutAssign          = "割り当て";
    ls->shortcutPressText       = "ショートカットキー入力欄:";
    ls->shortcutScheme          = "構\成名:";
    ls->shortcutCartInsert1     = "カートリッジ1 挿入";
    ls->shortcutCartRemove1     = "カートリッジ1 取り出し";
    ls->shortcutCartInsert2     = "カートリッジ2 挿入";
    ls->shortcutCartRemove2     = "カートリッジ2 取り出し";
    ls->shortcutSpecialMenu1    = "カートリッジ1のその他メニューの表\示";
    ls->shortcutSpecialMenu2    = "カートリッジ2のその他メニューの表\示";
    ls->shortcutCartAutoReset   = "カートリッジ 挿入/取り出し後リセット";
    ls->shortcutDiskInsertA     = "ディスクA 挿入";
    ls->shortcutDiskDirInsertA  = "ディレクトリをディスクAとして挿入"; 
    ls->shortcutDiskRemoveA     = "ディスクA 取り出し";
    ls->shortcutDiskChangeA     = "ディスクA 入れ替え(v-DRIVE)";
    ls->shortcutDiskAutoResetA  = "ディスクA 挿入/取り出し後リセット";
    ls->shortcutDiskInsertB     = "ディスクB 挿入";
    ls->shortcutDiskDirInsertB  = "ディレクトリをディスクBとして挿入";
    ls->shortcutDiskRemoveB     = "ディスクB 取り出し";
    ls->shortcutCasInsert       = "テープ 挿入";
    ls->shortcutCasEject        = "テープ 取り出し";
    ls->shortcutCasAutorewind   = "テープ 挿入後巻き戻す";
    ls->shortcutCasReadOnly     = "テープを読み込み専用にする";
    ls->shortcutCasSetPosition  = "テープ位置セット";
    ls->shortcutCasRewind       = "テープ巻き戻し";
    ls->shortcutCasSave         = "テープイメージ保存";
    ls->shortcutPrnFormFeed     = "プリンター紙送り"; 
    ls->shortcutCpuStateLoad    = "CPUステート読み込み";
    ls->shortcutCpuStateSave    = "CPUステート保存";
    ls->shortcutCpuStateQload   = "クイックロード";
    ls->shortcutCpuStateQsave   = "クリックセーブ";
    ls->shortcutAudioCapture    = "サウンド録音の実行/停止";
    ls->shortcutScreenshotOrig  = "スクリーンショット";
    ls->shortcutScreenshotSmall = "無フィルター標準サイズ スクリーンショット";
    ls->shortcutScreenshotLarge = "無フィルター2倍サイズ スクリーンショット";
    ls->shortcutQuit            = "blueMSX の終了";
    ls->shortcutRunPause        = "エミュレーションの実行/中断";
    ls->shortcutStop            = "停止";
    ls->shortcutResetHard       = "ハードリセット";
    ls->shortcutResetSoft       = "ソ\フトリセット";
    ls->shortcutResetClean      = "クリーンリセット";
    ls->shortcutSizeSmall       = "標準サイズ";
    ls->shortcutSizeNormal      = "2倍サイズ";
    ls->shortcutSizeFullscreen  = "フルスクリーン";
    ls->shortcutSizeMinimized   = "ウィンドウ 最小化";
    ls->shortcutToggleFullscren = "フルスクリーン化";
    ls->shortcutVolumeIncrease  = "ボリュームを上げる";
    ls->shortcutVolumeDecrease  = "ボリュームを下げる";
    ls->shortcutVolumeMute      = "ミュート";
    ls->shortcutVolumeStereo    = "モノラル/ステレオの切り替え";
    ls->shortcutSwitchMsxAudio  = "MSX-AUDIO カートリッジスイッチ";
    ls->shortcutSwitchFront     = "パナソ\ニック系 前面[内蔵ソ\フト]スイッチ";
    ls->shortcutSwitchPause     = "パナソ\ニック系 ポーズスイッチ";
    ls->shortcutToggleMouseLock = "マウスロック";
    ls->shortcutEmuSpeedMax     = "エミュレーション実行速度を最速化";
    ls->shortcutEmuSpeedToggle  = "最速動作の有無";
    ls->shortcutEmuSpeedNormal  = "エミュレーション実行速度を標準化";
    ls->shortcutEmuSpeedInc     = "エミュレーション速度を加速";
    ls->shortcutEmuSpeedDec     = "エミュレーション速度の減速";
    ls->shortcutThemeSwitch     = "テーマの切り替え";
    ls->shortcutShowEmuProp     = "エミュレーション設定画面の表\示";
    ls->shortcutShowVideoProp   = "ビデオ設定画面の表\示";
    ls->shortcutShowAudioProp   = "サウンド設定画面の表\示";
    ls->shortcutShowCtrlProp    = "コントロール設定画面の表\示";
    ls->shortcutShowPerfProp    = "パフォーマンス設定画面の表\示";
    ls->shortcutShowSettProp    = "その他設定画面の表\示";
    ls->shortcutShowPorts       = "ポート設定画面の表\示";
    ls->shortcutShowLanguage    = "言語ダイアログの表\示";
    ls->shortcutShowMachines    = "マシン構\成編集の表\示";
    ls->shortcutShowShortcuts   = "ショートカットエディターの表\示";
    ls->shortcutShowKeyboard    = "キーボード構\成編集の表\示";
    ls->shortcutShowMixer       = "ミキサーの表\示"; 
	ls->shortcutShowDebugger    = "デバッガー表\示";
    ls->shortcutShowTrainer     = "トレーナー表\示";
    ls->shortcutShowHelp        = "ヘルプダイアログの表\示";
    ls->shortcutShowAbout       = "blueMSX についての表\示";
    ls->shortcutShowFiles       = "ファイルダイアログを表\示";
    ls->shortcutToggleSpriteEnable = "スプライトの表\示/非表\示";
    ls->shortcutToggleFdcTiming = "FDDドライブのタイミングの有効/無効";
    ls->shortcutToggleCpuTrace  = "CPUトレースの有効/無効";


    //----------------------
    // Keyboard config lines
    //----------------------
    
    ls->keyconfigSelectedKey    = "選択キー:";
    ls->keyconfigMappedTo       = "対象キー:";
    ls->keyconfigMappingScheme  = "構\成名:";


    
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

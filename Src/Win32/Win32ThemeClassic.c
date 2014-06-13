/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32ThemeClassic.c,v $
**
** $Revision: 1.12 $
**
** $Date: 2005/11/11 05:15:01 $
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
#include "Theme.h"
#include "Actions.h"
#include "Win32ThemeClassic.h"
#include "ArchBitmap.h"
#include "ArchText.h"
#include "Resource.h"
#include <stdlib.h>
#include <string.h>

static ThemePage* themeCreateSmall() 
{
    ThemePage* theme = themePageCreate("small",
                                   328,                 // Width
                                   318,                 // Hidth
                                   4,                   // Emu pos x
                                   51,                  // Emu pos y
                                   320,                 // Emu width
                               240,                 // Emu height
                               0, 
                               0, 
                               320,
                               archRGB(219, 221, 224),
                               archRGB(128, 128, 255),
                               archRGB(0, 0, 0),
                               0,
                               0,
                               NULL);

    themePageAddImage(theme, activeImageCreate(0, 0, 1, archBitmapCreateFromId(IDB_CLASSIC_BGSMALL), 1),    THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);

    themePageAddText(theme, activeTextCreate(283, 278, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT2), 0, 256, 5, 0, 0, 0), THEME_TRIGGER_TEXT_VERSION, THEME_TRIGGER_NONE);

    themePageAddImage(theme, activeImageCreate(204, 300, 2, archBitmapCreateFromId(IDB_CLASSIC_DISKA), 2),  THEME_TRIGGER_IMG_DISKA, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(222, 300, 2, archBitmapCreateFromId(IDB_CLASSIC_DISKB), 2),  THEME_TRIGGER_IMG_DISKB, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(240, 300, 2, archBitmapCreateFromId(IDB_CLASSIC_CAS), 2),    THEME_TRIGGER_IMG_CAS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(269, 302, 2, archBitmapCreateFromId(IDB_CLASSIC_CAPS), 2),   THEME_TRIGGER_IMG_CAPS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(284, 302, 2, archBitmapCreateFromId(IDB_CLASSIC_KANA), 2),   THEME_TRIGGER_IMG_KANA, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(299, 302, 2, archBitmapCreateFromId(IDB_CLASSIC_FS), 2),     THEME_TRIGGER_IMG_FS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(314, 302, 2, archBitmapCreateFromId(IDB_CLASSIC_AS), 2),     THEME_TRIGGER_IMG_AS, THEME_TRIGGER_NONE);

    themePageAddImage(theme, activeImageCreate(68, 298, 2, archBitmapCreateFromId(IDB_CLASSIC_FPS), 2), THEME_TRIGGER_IMG_NOT_STOPPED, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(177, 298, 2, archBitmapCreateFromId(IDB_CLASSIC_FREQ), 2), THEME_TRIGGER_IMG_NOT_STOPPED, THEME_TRIGGER_NONE);

    themePageAddText(theme, activeTextCreate(143, 299, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 5, 0, 0, 1),  THEME_TRIGGER_TEXT_FREQ, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(55,  299, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 2, 0, 0, 1),  THEME_TRIGGER_TEXT_FPS, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(96, 299, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 5, 0, 0, 0),   THEME_TRIGGER_TEXT_CPU, THEME_TRIGGER_NONE);

    themePageAddDualButton(theme, activeDualButtonCreate(8, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_RESET),
                                              (ButtonEvent)actionEmuResetHard, 0, 0, 
                                              (ButtonEvent)actionMenuReset, 8, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(52, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_CART1),
                                              (ButtonEvent)actionCartInsert1, 0, 0, 
                                              (ButtonEvent)actionMenuCart1, 52, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(90, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_CART2),
                                              (ButtonEvent)actionCartInsert2, 0, 0, 
                                              (ButtonEvent)actionMenuCart2, 90, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(128, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTDISKA),
                                              (ButtonEvent)actionDiskInsertA, 0, 0, 
                                              (ButtonEvent)actionMenuDiskA, 128, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(166, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTDISKB),
                                              (ButtonEvent)actionDiskInsertB, 0, 0, 
                                              (ButtonEvent)actionMenuDiskB, 166, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(204, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTCAS),
                                              (ButtonEvent)actionCasInsert, 0, 0, 
                                              (ButtonEvent)actionMenuCassette, 204, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(251, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTSIZE),
                                              (ButtonEvent)actionWindowSizeNormal, 0, 0, 
                                              (ButtonEvent)actionMenuZoom, 251, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(289, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTOPTIONS),
                                              (ButtonEvent)actionPropShowEmulation, 0, 0, 
                                              (ButtonEvent)actionMenuOptions, 289, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);

    return theme;
}


static ThemePage* themeCreateNormal() 
{
    ThemePage* theme = themePageCreate("normal",
                               648,                 // Width
                               558,                 // Hidth
                               4,                   // Emu pos x
                               51,                  // Emu pos y
                               640,                 // Emu width
                               480,                 // Emu height
                               0, 
                               0, 
                               640,
                               archRGB(219, 221, 224),
                               archRGB(128, 128, 255),
                               archRGB(0, 0, 0),
                               0,
                               0,
                               NULL);

    themePageAddImage(theme, activeImageCreate(0, 0, 1, archBitmapCreateFromId(IDB_CLASSIC_BG), 1), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);

    themePageAddText(theme, activeTextCreate(538, 514, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT2), 0, 256, 5, 0, 0, 0), THEME_TRIGGER_TEXT_VERSION, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(606, 514, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT2), 0, 256, 5, 0, 0, 0), THEME_TRIGGER_TEXT_BUILDNUMBER, THEME_TRIGGER_NONE);

    themePageAddImage(theme, activeImageCreate(439, 540, 2, archBitmapCreateFromId(IDB_CLASSIC_DISKA), 2), THEME_TRIGGER_IMG_DISKA, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(457, 540, 2, archBitmapCreateFromId(IDB_CLASSIC_DISKB), 2), THEME_TRIGGER_IMG_DISKB, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(475, 540, 2, archBitmapCreateFromId(IDB_CLASSIC_CAS), 2), THEME_TRIGGER_IMG_CAS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(530, 542, 2, archBitmapCreateFromId(IDB_CLASSIC_CAPS), 2), THEME_TRIGGER_IMG_CAPS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(571, 542, 2, archBitmapCreateFromId(IDB_CLASSIC_KANA), 2), THEME_TRIGGER_IMG_KANA, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(602, 542, 2, archBitmapCreateFromId(IDB_CLASSIC_FS), 2), THEME_TRIGGER_IMG_FS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(633, 542, 2, archBitmapCreateFromId(IDB_CLASSIC_AS), 2), THEME_TRIGGER_IMG_AS, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(238, 538, 2, archBitmapCreateFromId(IDB_CLASSIC_FPS), 2), THEME_TRIGGER_IMG_NOT_STOPPED, THEME_TRIGGER_NONE);
    themePageAddImage(theme, activeImageCreate(342, 538, 2, archBitmapCreateFromId(IDB_CLASSIC_FREQ), 2), THEME_TRIGGER_IMG_NOT_STOPPED, THEME_TRIGGER_NONE);

    themePageAddText(theme, activeTextCreate(370, 539, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 9, 0, 0, 0),  THEME_TRIGGER_TEXT_SCREEN, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(310, 539, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 5, 0, 0, 1),  THEME_TRIGGER_TEXT_FREQ, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(225, 539, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 2, 0, 0, 1),  THEME_TRIGGER_TEXT_FPS, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(268, 539, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT), 0, 256, 5, 0, 0, 0),  THEME_TRIGGER_TEXT_CPU, THEME_TRIGGER_NONE);

    themePageAddDualButton(theme, activeDualButtonCreate(8, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_RESET),
                                          (ButtonEvent)actionEmuResetHard, 0, 0, 
                                          (ButtonEvent)actionMenuReset, 8, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddButton(theme, activeButtonCreate(52, 22, 4, 0, archBitmapCreateFromId(IDB_CLASSIC_PLAY),
                                          (ButtonEvent)actionEmuTogglePause, 0, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_IMG_NOT_RUNNING, THEME_TRIGGER_NONE);
    themePageAddButton(theme, activeButtonCreate(78, 22, 4, 0, archBitmapCreateFromId(IDB_CLASSIC_PAUSE),
                                          (ButtonEvent)actionEmuTogglePause, 0, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_IMG_RUNNING, THEME_TRIGGER_NONE);
    themePageAddButton(theme, activeButtonCreate(104, 22, 4, 0, archBitmapCreateFromId(IDB_CLASSIC_STOP),
                                          (ButtonEvent)actionEmuStop, 0, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_IMG_NOT_STOPPED, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(136, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_CART1),
                                          (ButtonEvent)actionCartInsert1, 0, 0, 
                                          (ButtonEvent)actionMenuCart1, 136, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(174, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_CART2),
                                          (ButtonEvent)actionCartInsert2, 0, 0, 
                                          (ButtonEvent)actionMenuCart2, 174, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(212, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTDISKA),
                                          (ButtonEvent)actionDiskInsertA, 0, 0, 
                                          (ButtonEvent)actionMenuDiskA, 212, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(250, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTDISKB),
                                          (ButtonEvent)actionDiskInsertB, 0, 0, 
                                          (ButtonEvent)actionMenuDiskB, 250, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(288, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTCAS),
                                          (ButtonEvent)actionCasInsert, 0, 0, 
                                          (ButtonEvent)actionMenuCassette, 288, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(335, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTSIZE),
                                          (ButtonEvent)actionWindowSizeSmall, 0, 0, 
                                          (ButtonEvent)actionMenuZoom, 335, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddDualButton(theme, activeDualButtonCreate(373, 22, 5, 0, archBitmapCreateFromId(IDB_CLASSIC_BTOPTIONS),
                                          (ButtonEvent)actionPropShowEmulation, 0, 0, 
                                          (ButtonEvent)actionMenuOptions, 373, 47, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);
    themePageAddButton(theme, activeButtonCreate(420, 22, 4, 0, archBitmapCreateFromId(IDB_CLASSIC_BTHELP),
                                          (ButtonEvent)actionHelpShowHelp, 0, 0), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);

    return theme;
}

static ThemePage* themeCreateFullscreen() 
{
    ThemePage* theme = themePageCreate("fullscreen",
                               640,                 // Width
                               480,                 // Hidth
                               0,                   // Emu pos x
                               0,                   // Emu pos y
                               640,                 // Emu width
                               480,                 // Emu height
                               0, 
                               0, 
                               640,
                               archRGB(219, 221, 224),
                               archRGB(128, 128, 255),
                               archRGB(0, 0, 0),
                               0,
                               0,
                               NULL);

    themePageAddImage(theme, activeImageCreate(-4, -51, 1, archBitmapCreateFromId(IDB_CLASSIC_BG), 1), THEME_TRIGGER_NONE, THEME_TRIGGER_NONE);

    themePageAddText(theme, activeTextCreate(534, 463, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT2), 0, 256, 5, 0, 0, 0), THEME_TRIGGER_TEXT_VERSION, THEME_TRIGGER_NONE);
    themePageAddText(theme, activeTextCreate(602, 463, 256, archBitmapCreateFromId(IDB_CLASSIC_FONT2), 0, 256, 5, 0, 0, 0), THEME_TRIGGER_TEXT_BUILDNUMBER, THEME_TRIGGER_NONE);

    return theme;
}

ThemeCollection* themeClassicCreate() 
{
    ThemeCollection* themeCollection = themeCollectionCreate();

    strcpy(themeCollection->name, "Classic");

    themeCollection->little          = themeCreate("little");
    themeCollection->normal          = themeCreate("normal");
    themeCollection->fullscreen      = themeCreate("fullscreen");
    
    themeAddPage(themeCollection->little,          themeCreateSmall());
    themeAddPage(themeCollection->normal,          themeCreateNormal());
    themeAddPage(themeCollection->fullscreen,      themeCreateFullscreen());

    return themeCollection;
}

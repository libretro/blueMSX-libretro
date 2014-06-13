/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Theme/Theme.h,v $
**
** $Revision: 1.15 $
**
** $Date: 2005/08/09 08:16:41 $
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
#ifndef THEME_H
#define THEME_H

#include "ThemeControls.h"
#include "ThemeTriggers.h"

typedef struct Theme Theme;
typedef struct ThemePage ThemePage;
typedef struct ThemeItem ThemeItem;

typedef struct {
    int x;
    int y;
} ClipPoint;

#define THEME_MAX_WINDOWS 64
#define THEME_MAX_PAGES   64

typedef struct ThemeCollection {
    char   name[64];
    Theme* little;
    Theme* normal;
    Theme* fullscreen;
    Theme* theme[THEME_MAX_WINDOWS];
} ThemeCollection;

struct ThemePage {
// public:
    char name[64];
    int width;
    int height;
    int emuWinX;
    int emuWinY;
    int emuWinW;
    int emuWinH;
    struct {
        unsigned long color;
        unsigned long focusColor;
        unsigned long textColor;
        int x;
        int y;
        int width;
    } menu;

    struct {
        int count;
        ClipPoint* list;
    } clipPoint;
    int noFrame;

// private:
    ThemeItem*        itemList;
    ThemeItem*        activeItem;
    ActiveButton*     btDown;
    ActiveDualButton* btDualDown;
    int               btPressed;
    int               active;
};

typedef enum { TH_NORMAL, TH_KBDCONFIG, TH_PROPERTIES } ThemeHandler;

struct Theme {
    char   name[64];
    void*  reference;
    int currentPage;
    int pageCount;
    ThemeHandler themeHandler;
    ThemePage* pages[THEME_MAX_PAGES];
};

ThemePage* themePageCreate(const char* name,
                           int width,
                           int height,
                           int emuPosX,
                           int emuPosY,
                           int emuWidth,
                           int emuHeight,
                           int menuPosX,
                           int menuPosY,
                           int menuWidth,
                           unsigned long menuColor,
                           unsigned long menuFocusColor,
                           unsigned long menuTextColor,
                           int        noFrame,
                           int        clipPointCount,
                           ClipPoint* clipPointList);

unsigned long themeGetNameHash(const char* name);


ThemeCollection* themeCollectionCreate();
void themeCollectionDestroy(ThemeCollection* tc);
void themeCollectionAddWindow(ThemeCollection* tc, Theme* theme);
void themeCollectionOpenWindow(ThemeCollection* themeCollection, unsigned long hash);

Theme*     themeCreate(const char* name);
void       themeDestroy(Theme* theme);
void       themeAddPage(Theme* theme, ThemePage* page);
void       themeSetHandler(Theme* theme, ThemeHandler themeHandler);
ThemePage* themeGetCurrentPage(Theme* theme);
int        themeGetCurrentPageIndex(Theme* theme);
int        themeGetPageCount(Theme* theme);
char*      themeGetPageName(Theme* theme, int index);
void       themeSetPageFromHash(Theme* theme, unsigned long hash);
char**     themeGetPageNames(Theme* theme);

void themePageDestroy(ThemePage* theme);
void themePageAddImage(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible);
void themePageAddGrabImage(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible);
void themePageAddText(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible);
void themePageAddButton(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible, ThemeTrigger pressed);
void themePageAddDualButton(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible, ThemeTrigger pressed);
void themePageAddToggleButton(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible, ThemeTrigger pressed);
void themePageAddMeter(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible);
void themePageAddSlider(ThemePage* theme, void* object, ThemeTrigger trigger, ThemeTrigger visible);
void themePageAddObject(ThemePage* theme, void* object, ThemeTrigger visible);
void themePageActivate(ThemePage* theme, void* window);
void themePageUpdate(ThemePage* theme, void* dc);
void themePageDraw(ThemePage* theme, void* dc, ThemeItem* startItem);
void themePageMouseMove(ThemePage* theme, void* dc, int x, int y);
void themePageMouseButtonUp(ThemePage* theme, void* dc, int x, int y);
void themePageMouseButtonDown(ThemePage* theme, void* dc, int x, int y);
void themePageSetActive(ThemePage* theme, void* dc, int active);

#endif //WIN32_THEME_CLASSIC_H

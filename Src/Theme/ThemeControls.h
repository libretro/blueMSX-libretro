/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Theme/ThemeControls.h,v $
**
** $Revision: 1.12 $
**
** $Date: 2005/08/24 04:35:33 $
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
#ifndef THEME_CONTROLS_H
#define THEME_CONTROLS_H

#include "ArchBitmap.h"

typedef void (*ButtonEvent)(int, int);

typedef struct ActiveRect {
    int x;
    int y;
    int width;
    int height;
} ActiveRect;

//ActiveItem is the base class for all active objects
typedef struct ActiveItem ActiveItem;

void activeItemGetRect(void* activeItem, ActiveRect* rect);



typedef struct ActiveImage ActiveImage;

ActiveImage* activeImageCreate(int x, int y, int cols, ArchBitmap* bitmap, int count);
void activeImageDestroy(ActiveImage* activeImage);
void activeImageSetPosition(ActiveImage* activeImage, int x, int y);
int activeImageGetWidth(ActiveImage* activeImage);
int activeImageGetHeight(ActiveImage* activeImage);
int activeImageSetImage(ActiveImage* activeImage, int index);
int activeImageShow(ActiveImage* activeImage, int show);
void activeImageDraw(ActiveImage* activeImage, void* dc, ActiveRect* rect);

typedef struct ActiveButton ActiveButton;

ActiveButton* activeButtonCreate(int x, int y, int cols, int activeNotify, ArchBitmap* bitmap, ButtonEvent event, int arg1, int arg2);
void activeButtonDestroy(ActiveButton* activeButton);
int activeButtonMouseMove(ActiveButton* activeButton, int x, int y);
int activeButtonDown(ActiveButton* activeButton, int x, int y);
int activeButtonUp(ActiveButton* activeButton, int x, int y);
int activeButtonShow(ActiveButton* activeButton, int show);
void activeButtonDraw(ActiveButton* activeButton, void* dc, ActiveRect* rect);
int activeButtonForcePushed(ActiveButton* activeButton, int pushed);
int activeButtonActivate(ActiveButton* activeButton, int active);


typedef struct ActiveToggleButton ActiveToggleButton;

ActiveToggleButton* activeToggleButtonCreate(int x, int y, int cols, int activeNotify, ArchBitmap* bitmap, ButtonEvent event, int arg1, int arg2);
void activeToggleButtonDestroy(ActiveToggleButton* activeButton);
int activeToggleButtonMouseMove(ActiveToggleButton* activeButton, int x, int y);
int activeToggleButtonDown(ActiveToggleButton* activeButton, int x, int y);
int activeToggleButtonUp(ActiveToggleButton* activeButton, int x, int y);
int activeToggleButtonShow(ActiveToggleButton* activeButton, int show);
void activeToggleButtonDraw(ActiveToggleButton* activeButton, void* dc, ActiveRect* rect);
int activeToggleButtonForcePushed(ActiveToggleButton* activeButton, int pushed);
int activeToggleButtonSetToggled(ActiveToggleButton* activeButton, int toggled);
int activeToggleButtonActivate(ActiveToggleButton* activeButton, int active);


typedef struct ActiveDualButton ActiveDualButton;

ActiveDualButton* activeDualButtonCreate(int x, int y, int cols, int activeNotify, ArchBitmap* bitmap, 
                                          ButtonEvent eventA, int argA1, int argA2, 
                                          ButtonEvent eventB, int argB1, int argB2, int vertical);
void activeDualButtonDestroy(ActiveDualButton* activeButton);
int activeDualButtonMouseMove(ActiveDualButton* activeButton, int x, int y);
int activeDualButtonDown(ActiveDualButton* activeButton, int x, int y);
int activeDualButtonUp(ActiveDualButton* activeButton, int x, int y);
void activeDualButtonDraw(ActiveDualButton* activeButton, void* dc, ActiveRect* rect);
int activeDualButtonShow(ActiveDualButton* activeButton, int show);
int activeDualButtonForcePushed(ActiveDualButton* activeButton, int pushed);
int activeDualButtonActivate(ActiveDualButton* activeButton, int active);



typedef struct ActiveText ActiveText;

ActiveText* activeTextCreate(int x, int y, int cols, ArchBitmap* bitmap, int startChar, int charCount, int width, int type, int color, int rightAligned);
void activeTextDestroy(ActiveText* activeText);
int activeTextSetText(ActiveText* activeText, const char* string);
void activeTextDraw(ActiveText* activeText, void* dc, ActiveRect* rect);
int activeTextShow(ActiveText* activeText, int show);


typedef struct ActiveGrabImage ActiveGrabImage;

ActiveGrabImage* activeGrabImageCreate(int x, int y, int cols, ArchBitmap* bitmap, int count);
void activeGrabImageDestroy(ActiveGrabImage* activeImage);
int activeGrabImageMouseMove(ActiveGrabImage* activeImage, int x, int y);
int activeGrabImageDown(ActiveGrabImage* activeImage, int x, int y);
int activeGrabImageUp(ActiveGrabImage* activeImage, int x, int y);
int activeGrabImageSetImage(ActiveGrabImage* activeImage, int index);
int activeGrabImageShow(ActiveGrabImage* activeImage, int show);
void activeGrabImageDraw(ActiveGrabImage* activeImage, void* dc, ActiveRect* rect);
int activeGrabImageActivate(ActiveGrabImage* activeImage, int active);


typedef struct ActiveMeter ActiveMeter;

ActiveMeter* activeMeterCreate(int x, int y, int cols, ArchBitmap* bitmap, int count);
void activeMeterDestroy(ActiveMeter* activeMeter);
int activeMeterSetImage(ActiveMeter* activeMeter, int index);
void activeMeterDraw(ActiveMeter* activeMeter, void* dc, ActiveRect* rect);
int activeMeterShow(ActiveMeter* activeMeter, int show);


typedef void (*SliderEvent)(int);
typedef struct ActiveSlider ActiveSlider;
typedef enum { AS_HORIZONTAL, AS_VERTICAL, AS_BOTH } AsDirection;

ActiveSlider* activeSliderCreate(int x, int y, int cols, ArchBitmap* bitmap, SliderEvent event, int count,
                                 AsDirection direction, int sensitivity,
                                 ButtonEvent upEvent, int arg1, int arg2);
void activeSliderDestroy(ActiveSlider* activeSlider);
void activeSliderDraw(ActiveSlider* activeSlider, void* dc, ActiveRect* rect);
int activeSliderShow(ActiveSlider* activeSlider, int show);
int activeSliderSetImage(ActiveSlider* activeSlider, int index);
int activeSliderDown(ActiveSlider* activeSlider, int x, int y);
int activeSliderUp(ActiveSlider* activeSlider, int x, int y);
int activeSliderMouseMove(ActiveSlider* activeSlider, int x, int y);

typedef struct ActiveObject ActiveObject;

ActiveObject* activeObjectCreate(int x, int y, int width, int height, const char* id, int arg1, int arg2);
void activeObjectDestroy(ActiveObject* activeObject);
void activeObjectActivate(ActiveObject* activeObject, void* window);

#endif

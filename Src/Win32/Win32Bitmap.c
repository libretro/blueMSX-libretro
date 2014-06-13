/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Bitmap.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2005/09/24 00:09:50 $
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
#include "ArchBitmap.h"
#include "ArchText.h"
#include <windows.h>

struct ArchText {
    HFONT hFont;
    int height;
    int right;
    int color;
};

int archRGB(int r, int g, int b)
{
    return RGB(r, g, b);
}

ArchText* archTextCreate(int height, int color, int rightAligned)
{
    ArchText* text = malloc(sizeof(ArchText));
    LOGFONT lf = { 0 };

    lf.lfHeight  = height;
    lf.lfCharSet = DEFAULT_CHARSET;

    text->height = height;
    text->color  = color;
    text->right  = rightAligned;
    text->hFont  = CreateFontIndirect(&lf);

    return text;
}

void archTextDestroy(ArchText* text)
{
    DeleteObject(text->hFont);
    free(text);
}

void archTextDraw(ArchText* text, void* dcDest, int xDest, int yDest, int width, int height, char* string)
{
    HDC hOldFont;

    SetBkMode(dcDest, TRANSPARENT);
    SetTextColor(dcDest, text->color);
    
    if (hOldFont = SelectObject(dcDest, text->hFont)) {
        TEXTMETRIC tm;
        if (GetTextMetrics(dcDest, &tm)) {
            text->height = tm.tmHeight;
        }
        {
            RECT rect = { xDest, yDest, xDest + width, yDest + text->height };
            DrawText(dcDest, string, strlen(string), &rect, text->right ? DT_RIGHT : DT_LEFT);
            SelectObject(dcDest, hOldFont); 
        }
    }
}

struct ArchBitmap {
    HBITMAP hBitmapOrig;
    HBITMAP hBitmap;
    HDC hMemDC;
    int width;
    int height;
};

static ArchBitmap* bitmapCreate(void* bitmap) 
{
    ArchBitmap* bm = malloc(sizeof(ArchBitmap));
    BITMAP bmp;
    
    bm->hBitmap = bitmap;

    bm->hMemDC      = CreateCompatibleDC(GetWindowDC(NULL));
    bm->hBitmapOrig = (HBITMAP)SelectObject(bm->hMemDC, bm->hBitmap);
    GetObject(bm->hBitmap, sizeof(BITMAP), (PSTR)&bmp);
    bm->width  = bmp.bmWidth;
    bm->height = bmp.bmHeight;

    return bm;
}

ArchBitmap* archBitmapCreate(int width, int height)
{
    HBITMAP hBitmap = CreateCompatibleBitmap(GetWindowDC(NULL), width, height);
    return bitmapCreate(hBitmap);
}

ArchBitmap* archBitmapCreateFromFile(const char* filename)
{
    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0,
                               LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

    return bitmapCreate(hBitmap);
}

ArchBitmap* archBitmapCreateFromId(int id)
{
    HBITMAP hBitmap = (HBITMAP)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(id));

    return bitmapCreate(hBitmap);
}

void archBitmapDestroy(ArchBitmap* bm)
{
    DeleteObject(SelectObject(bm->hMemDC, bm->hBitmapOrig));
    DeleteDC(bm->hMemDC);
    free(bm);
}

int archBitmapGetWidth(ArchBitmap* bm)
{
    return bm->width;
}

int archBitmapGetHeight(ArchBitmap* bm)
{
    return bm->height;
}

void archBitmapDraw(ArchBitmap* bm, void* dcDest, int xDest, int yDest, int xSrc, int ySrc, int width, int height)
{
    BitBlt(dcDest, xDest, yDest, width, height, bm->hMemDC, xSrc, ySrc, SRCCOPY);
}

void archBitmapCopy(ArchBitmap* dst, int xDest, int yDest, ArchBitmap* src, int xSrc, int ySrc, int width, int height)
{
    archBitmapDraw(src, dst->hMemDC, xDest, yDest, xSrc, ySrc, width, height);
}


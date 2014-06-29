/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Bitmap.c,v $
**
** $Revision: 1.8 $
**
** $Date: 2008-03-30 18:38:48 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#include "ArchBitmap.h"
#include "ArchText.h"
#include <windows.h>
#include <stdio.h>

// PacketFileSystem.h Need to be included after all other includes
#include "PacketFileSystem.h"

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
    HBITMAP hBitmap;

    if (pkg_file_exists(filename)) {
        // TODO: Make better bitmap loader for bitmaps in packages
        char* bitmap = NULL;
        int size = 0;
        FILE* f = fopen(filename, "rb");
        if (f != NULL) {
            fseek(f, 0, SEEK_END);
            size = ftell(f);
            fseek(f, 0, SEEK_SET);
            
            if (size > 0) {
                bitmap = malloc(size);
                if (bitmap != NULL) {
                    fread(bitmap, 1, size, f);
                }
            }
            fclose(f);
        }
        if (bitmap != NULL) {
            FILE* f = fopen("tmp.bmp", "wb");
            if (f != NULL) {
                fwrite(bitmap, 1, size, f);
                fclose(f);
            }
            free(bitmap);
        }
        hBitmap = (HBITMAP)LoadImage(NULL, "tmp.bmp", IMAGE_BITMAP, 0, 0,
                                     LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    }
    else {
        hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0,
                                     LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    }

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


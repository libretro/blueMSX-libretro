/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32Printer.c,v $
**
** $Revision: 1.31 $
**
** $Date: 2006/06/24 02:23:59 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Rudolf Lechleitner, Daniel Vik, Tomas Karlsson
**
** This code is ported from RuMSX and modified to fit into the blueMSX
** framework.
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
#include <windows.h>
#include "Properties.h"
#include "Win32Printer.h"
#include "ArchPrinter.h"



static int printerType = P_LPT_MSXPRN;

static void ResetEmulatedPrinter(void);

#define PIXEL_WIDTH 8

#define MSX_FONT_SIZE (256 * 8)

#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#define MAX(x, y)  ((x) > (y) ? (x) : (y))

static void eraseBackground(HDC hdc)
{
    int  w = GetDeviceCaps(hdc, PHYSICALWIDTH);
    int  h = GetDeviceCaps(hdc, PHYSICALHEIGHT);
    RECT r = { 0, 0, w, h };

    FillRect(hdc, &r, GetStockObject(WHITE_BRUSH)); 
}


//////////////////////////////////////////////////////////////////////////////////
/// Raw printer emulation
//////////////////////////////////////////////////////////////////////////////////

static HANDLE hFile = INVALID_HANDLE_VALUE;

static void printerRawDestroy(void)
{
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
}

static void printerRawWrite(BYTE value)
{
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwWritten;
        WriteFile(hFile, &value, 1, &dwWritten, NULL);
    }
}

static int printerRawCreate(void)
{
    Properties* pProperties = propGetGlobalProperties();

    hFile = CreateFile(pProperties->ports.Lpt.portName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);

    return hFile != INVALID_HANDLE_VALUE;
}


///////////////////////////////////////////////////////////////
/// Emulated printer, from RuMSX
///////////////////////////////////////////////////////////////

//#define USE_REAL_FONT

static BYTE EpsonFontRom[] = {
    0x8b, 0x04, 0x0a, 0x20, 0x8a, 0x60, 0x0a, 0x20, 0x1c, 0x02, 0x00, 0x00, /*   0 */
    0x8b, 0x1c, 0x22, 0x08, 0xa2, 0x48, 0x22, 0x08, 0x22, 0x18, 0x00, 0x00, /*   1 */
    0x9b, 0x00, 0x3c, 0x00, 0x82, 0x40, 0x02, 0x00, 0x3c, 0x02, 0x00, 0x00, /*   2 */
    0x9a, 0x00, 0x1c, 0x22, 0x80, 0x62, 0x00, 0x22, 0x1c, 0x00, 0x00, 0x00, /*   3 */
    0x96, 0x00, 0x12, 0x80, 0x5e, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, /*   4 */
    0xa7, 0x00, 0x00, 0x40, 0xa0, 0x00, 0xa0, 0x40, 0x00, 0x00, 0x00, 0x00, /*   5 */
    0x8b, 0x12, 0x00, 0x7e, 0x80, 0x12, 0x80, 0x02, 0x80, 0x42, 0x00, 0x00, /*   6 */
    0xc8, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*   7 */
    0x8b, 0x06, 0x00, 0x09, 0x00, 0x51, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, /*   8 */
    0x8b, 0x5e, 0x80, 0x10, 0x80, 0x08, 0x40, 0x04, 0x40, 0x9e, 0x00, 0x00, /*   9 */
    0x8a, 0x40, 0x9e, 0x00, 0x90, 0x40, 0x10, 0x4e, 0x80, 0x00, 0x00, 0x00, /*  10 */
    0x8b, 0x92, 0x28, 0x44, 0x00, 0x44, 0x00, 0x44, 0x28, 0x92, 0x00, 0x00, /*  11 */
    0x8b, 0xfe, 0x00, 0xa0, 0x00, 0x48, 0x00, 0x1e, 0x00, 0x0a, 0x00, 0x00, /*  12 */
    0x8b, 0x06, 0x08, 0x54, 0xa0, 0x04, 0xa0, 0x54, 0x08, 0x06, 0x00, 0x00, /*  13 */
    0x8b, 0x04, 0x0a, 0x20, 0x0a, 0xa0, 0x0a, 0x20, 0x1c, 0x02, 0x00, 0x00, /*  14 */
    0x0a, 0x38, 0x44, 0x01, 0x44, 0x01, 0x46, 0x00, 0x44, 0x00, 0x00, 0x00, /*  15 */
    0x9a, 0x00, 0x50, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x14, 0x00, 0x00, 0x00, /*  16 */
    0x8a, 0x7e, 0x80, 0x00, 0x80, 0x12, 0x80, 0x12, 0x6c, 0x00, 0x00, 0x00, /*  17 */
    0x8b, 0x3e, 0x40, 0x90, 0x00, 0xfe, 0x00, 0x92, 0x00, 0x92, 0x00, 0x00, /*  18 */
    0x8b, 0x2c, 0x02, 0x28, 0x02, 0x1c, 0x20, 0x0a, 0x20, 0x1a, 0x00, 0x00, /*  19 */
    0x8b, 0x3a, 0x44, 0x00, 0x8a, 0x10, 0xa2, 0x00, 0x44, 0xb8, 0x00, 0x00, /*  20 */
    0x8b, 0x02, 0x08, 0x14, 0x22, 0x08, 0x22, 0x14, 0x08, 0x20, 0x00, 0x00, /*  21 */
    0xa9, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, /*  22 */
    0x8b, 0x06, 0x88, 0x14, 0x20, 0x44, 0x20, 0x14, 0x88, 0x06, 0x00, 0x00, /*  23 */
    0x8b, 0x1c, 0xa2, 0x00, 0x22, 0x00, 0x22, 0x00, 0xa2, 0x1c, 0x00, 0x00, /*  24 */
    0x8b, 0x3c, 0x82, 0x00, 0x02, 0x00, 0x02, 0x00, 0x82, 0x3c, 0x00, 0x00, /*  25 */
    0x8b, 0x04, 0x0a, 0xa0, 0x0a, 0x20, 0x0a, 0xa0, 0x1c, 0x02, 0x00, 0x00, /*  26 */
    0x9a, 0x00, 0x1c, 0xa2, 0x00, 0x22, 0x00, 0xa2, 0x1c, 0x00, 0x00, 0x00, /*  27 */
    0x8a, 0x3c, 0x80, 0x02, 0x00, 0x02, 0x80, 0x3c, 0x02, 0x00, 0x00, 0x00, /*  28 */
    0x8b, 0x3e, 0x00, 0x2a, 0x00, 0x6a, 0x80, 0x2a, 0x00, 0x22, 0x00, 0x00, /*  29 */
    0x8b, 0x1c, 0x22, 0x08, 0x22, 0x48, 0xa2, 0x08, 0x22, 0x18, 0x00, 0x00, /*  30 */
    0x8b, 0xa8, 0x00, 0x68, 0x00, 0x3e, 0x00, 0x68, 0x00, 0xa8, 0x00, 0x00, /*  31 */
    0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  32 */
    0xc8, 0x00, 0x00, 0x00, 0x00, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  33 */
    0xa9, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, /*  34 */
    0x8b, 0x28, 0x00, 0xfe, 0x00, 0x28, 0x00, 0xfe, 0x00, 0x28, 0x00, 0x00, /*  35 */
    0x8b, 0x24, 0x00, 0x54, 0x00, 0xfe, 0x00, 0x54, 0x00, 0x48, 0x00, 0x00, /*  36 */
    0x8b, 0xc0, 0x02, 0xc4, 0x08, 0x10, 0x20, 0x46, 0x80, 0x06, 0x00, 0x00, /*  37 */
    0x8b, 0x4c, 0xa0, 0x12, 0xa0, 0x4a, 0x00, 0x04, 0x08, 0x12, 0x00, 0x00, /*  38 */
    0xc8, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, /*  39 */
    0xc9, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x00, 0x00, 0x00, 0x00, /*  40 */
    0xa7, 0x00, 0x00, 0x82, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  41 */
    0x8b, 0x10, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x10, 0x00, 0x00, /*  42 */
    0x8b, 0x10, 0x00, 0x10, 0x00, 0x7c, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, /*  43 */
    0x39, 0x00, 0x00, 0x00, 0x0c, 0x01, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, /*  44 */
    0x8b, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, /*  45 */
    0xa8, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  46 */
    0x9a, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, /*  47 */
    0x8b, 0x38, 0x44, 0x00, 0x82, 0x00, 0x82, 0x00, 0x44, 0x38, 0x00, 0x00, /*  48 */
    0xa9, 0x00, 0x00, 0x42, 0x00, 0xfe, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, /*  49 */
    0x8b, 0x42, 0x80, 0x06, 0x80, 0x0a, 0x80, 0x12, 0x80, 0x62, 0x00, 0x00, /*  50 */
    0x8b, 0x84, 0x00, 0x82, 0x00, 0xa2, 0x00, 0xd2, 0x00, 0x8c, 0x00, 0x00, /*  51 */
    0x8b, 0x08, 0x10, 0x28, 0x40, 0x88, 0x00, 0xfe, 0x00, 0x08, 0x00, 0x00, /*  52 */
    0x8b, 0xe4, 0x02, 0xa0, 0x02, 0xa0, 0x02, 0xa0, 0x02, 0x9c, 0x00, 0x00, /*  53 */
    0x8b, 0x0c, 0x12, 0x20, 0x52, 0x80, 0x12, 0x00, 0x12, 0x0c, 0x00, 0x00, /*  54 */
    0x8b, 0x80, 0x00, 0x82, 0x04, 0x88, 0x10, 0xa0, 0x40, 0x80, 0x00, 0x00, /*  55 */
    0x8b, 0x6c, 0x92, 0x00, 0x92, 0x00, 0x92, 0x00, 0x92, 0x6c, 0x00, 0x00, /*  56 */
    0x8b, 0x60, 0x90, 0x00, 0x90, 0x02, 0x94, 0x08, 0x90, 0x60, 0x00, 0x00, /*  57 */
    0xa7, 0x00, 0x00, 0x36, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  58 */
    0x27, 0x00, 0x00, 0x6d, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  59 */
    0x89, 0x10, 0x00, 0x28, 0x00, 0x44, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, /*  60 */
    0x8b, 0x28, 0x00, 0x28, 0x00, 0x28, 0x00, 0x28, 0x00, 0x28, 0x00, 0x00, /*  61 */
    0xab, 0x00, 0x00, 0x82, 0x00, 0x44, 0x00, 0x28, 0x00, 0x10, 0x00, 0x00, /*  62 */
    0x8b, 0x40, 0x80, 0x00, 0x80, 0x0a, 0x80, 0x10, 0x80, 0x60, 0x00, 0x00, /*  63 */
    0x8b, 0x38, 0x44, 0x82, 0x10, 0xaa, 0x00, 0xaa, 0x00, 0x7a, 0x00, 0x00, /*  64 */
    0x8b, 0x1e, 0x20, 0x48, 0x80, 0x08, 0x80, 0x48, 0x20, 0x1e, 0x00, 0x00, /*  65 */
    0x8b, 0x82, 0x7c, 0x82, 0x10, 0x82, 0x10, 0x82, 0x10, 0x6c, 0x00, 0x00, /*  66 */
    0x8b, 0x7c, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x82, 0x44, 0x00, 0x00, /*  67 */
    0x8b, 0x82, 0x7c, 0x82, 0x00, 0x82, 0x00, 0x82, 0x44, 0x38, 0x00, 0x00, /*  68 */
    0x8b, 0xfe, 0x00, 0x92, 0x00, 0x92, 0x00, 0x92, 0x00, 0x82, 0x00, 0x00, /*  69 */
    0x8b, 0xfe, 0x00, 0x90, 0x00, 0x90, 0x00, 0x90, 0x00, 0x80, 0x00, 0x00, /*  70 */
    0x8b, 0x7c, 0x82, 0x00, 0x82, 0x10, 0x82, 0x10, 0x82, 0x5c, 0x00, 0x00, /*  71 */
    0x8b, 0xfe, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0xfe, 0x00, 0x00, /*  72 */
    0xa9, 0x00, 0x00, 0x82, 0x00, 0xfe, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, /*  73 */
    0x8a, 0x0c, 0x02, 0x00, 0x82, 0x00, 0x82, 0x7c, 0x80, 0x00, 0x00, 0x00, /*  74 */
    0x8b, 0xfe, 0x00, 0x10, 0x00, 0x28, 0x00, 0x44, 0x00, 0x82, 0x00, 0x00, /*  75 */
    0x8b, 0xfe, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, /*  76 */
    0x8b, 0xfe, 0x00, 0x40, 0x20, 0x10, 0x20, 0x40, 0x00, 0xfe, 0x00, 0x00, /*  77 */
    0x8b, 0xfe, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0xfe, 0x00, 0x00, /*  78 */
    0x8b, 0x7c, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x82, 0x7c, 0x00, 0x00, /*  79 */
    0x8b, 0xfe, 0x00, 0x90, 0x00, 0x90, 0x00, 0x90, 0x00, 0x60, 0x00, 0x00, /*  80 */
    0x8b, 0x7c, 0x82, 0x00, 0x82, 0x08, 0x82, 0x04, 0x80, 0x7a, 0x00, 0x00, /*  81 */
    0x8b, 0xfe, 0x00, 0x90, 0x00, 0x90, 0x00, 0x98, 0x04, 0x62, 0x00, 0x00, /*  82 */
    0x8b, 0x64, 0x92, 0x00, 0x92, 0x00, 0x92, 0x00, 0x92, 0x4c, 0x00, 0x00, /*  83 */
    0x8b, 0x80, 0x00, 0x80, 0x00, 0xfe, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, /*  84 */
    0x8b, 0xfc, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0xfc, 0x00, 0x00, /*  85 */
    0x8b, 0xe0, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0xe0, 0x00, 0x00, /*  86 */
    0x8b, 0xfc, 0x02, 0x04, 0x08, 0x30, 0x08, 0x04, 0x02, 0xfc, 0x00, 0x00, /*  87 */
    0x9a, 0x00, 0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82, 0x00, 0x00, 0x00, /*  88 */
    0x8b, 0x80, 0x40, 0x20, 0x10, 0x0e, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, /*  89 */
    0x9a, 0x00, 0x82, 0x04, 0x8a, 0x10, 0xa2, 0x40, 0x82, 0x00, 0x00, 0x00, /*  90 */
    0xa9, 0x00, 0x00, 0xfe, 0x00, 0x82, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, /*  91 */
    0x9a, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, /*  92 */
    0xa9, 0x00, 0x00, 0x82, 0x00, 0x82, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, /*  93 */
    0x8b, 0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00, 0x00, /*  94 */
    0x0b, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, /*  95 */
    0xb7, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*  96 */
    0x8b, 0x04, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x1c, 0x02, 0x00, 0x00, /*  97 */
    0x8a, 0xfe, 0x00, 0x22, 0x00, 0x22, 0x00, 0x22, 0x1c, 0x00, 0x00, 0x00, /*  98 */
    0x8a, 0x1c, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0x00, 0x00, /*  99 */
    0x8a, 0x1c, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0xfe, 0x00, 0x00, 0x00, /* 100 */
    0x8b, 0x1c, 0x22, 0x08, 0x22, 0x08, 0x22, 0x08, 0x22, 0x18, 0x00, 0x00, /* 101 */
    0x89, 0x10, 0x00, 0x10, 0x7e, 0x90, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, /* 102 */
    0x0a, 0x38, 0x44, 0x01, 0x44, 0x01, 0x44, 0x01, 0x7e, 0x00, 0x00, 0x00, /* 103 */
    0x8a, 0xfe, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x1e, 0x00, 0x00, 0x00, /* 104 */
    0x98, 0x00, 0x22, 0x00, 0xbe, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, /* 105 */
    0x99, 0x00, 0x01, 0x00, 0x01, 0x20, 0x01, 0xbe, 0x00, 0x00, 0x00, 0x00, /* 106 */
    0x9a, 0x00, 0xfe, 0x00, 0x08, 0x00, 0x14, 0x00, 0x22, 0x00, 0x00, 0x00, /* 107 */
    0x98, 0x00, 0x82, 0x00, 0xfe, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, /* 108 */
    0x8b, 0x1e, 0x20, 0x00, 0x20, 0x1e, 0x20, 0x00, 0x20, 0x1e, 0x00, 0x00, /* 109 */
    0x8a, 0x3e, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x1e, 0x00, 0x00, 0x00, /* 110 */
    0x8b, 0x1c, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0x22, 0x1c, 0x00, 0x00, /* 111 */
    0x0a, 0x7f, 0x00, 0x44, 0x00, 0x44, 0x00, 0x44, 0x38, 0x00, 0x00, 0x00, /* 112 */
    0x1b, 0x00, 0x38, 0x44, 0x00, 0x44, 0x00, 0x44, 0x00, 0x7e, 0x00, 0x00, /* 113 */
    0x8a, 0x3e, 0x00, 0x10, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, /* 114 */
    0x8b, 0x10, 0x2a, 0x00, 0x2a, 0x00, 0x2a, 0x00, 0x2a, 0x04, 0x00, 0x00, /* 115 */
    0x8a, 0x20, 0x00, 0x7c, 0x02, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, /* 116 */
    0x8b, 0x3c, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x3c, 0x02, 0x00, 0x00, /* 117 */
    0x8b, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, /* 118 */
    0x8b, 0x3c, 0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02, 0x3c, 0x00, 0x00, /* 119 */
    0x89, 0x22, 0x14, 0x00, 0x08, 0x00, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00, /* 120 */
    0x0b, 0x40, 0x20, 0x11, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, /* 121 */
    0x89, 0x22, 0x04, 0x22, 0x08, 0x22, 0x10, 0x22, 0x00, 0x00, 0x00, 0x00, /* 122 */
    0xaa, 0x00, 0x00, 0x10, 0x00, 0x6c, 0x82, 0x00, 0x82, 0x00, 0x00, 0x00, /* 123 */
    0xc7, 0x00, 0x00, 0x00, 0x00, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 124 */
    0xaa, 0x00, 0x82, 0x00, 0x82, 0x6c, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, /* 125 */
    0x8b, 0x40, 0x80, 0x00, 0x80, 0x40, 0x20, 0x00, 0x20, 0x40, 0x00, 0x00, /* 126 */
    0x8b, 0x7c, 0x82, 0x04, 0x8a, 0x10, 0xa2, 0x40, 0x82, 0x7c, 0x00, 0x00, /* 127 */
    0x8a, 0x04, 0x0a, 0x80, 0x2a, 0x60, 0x0a, 0x24, 0x1a, 0x00, 0x00, 0x00, /* 128 */
    0x8a, 0x0c, 0x12, 0x28, 0x82, 0x68, 0x02, 0x28, 0x10, 0x00, 0x00, 0x00, /* 129 */
    0x8a, 0x0c, 0x32, 0x00, 0x82, 0x40, 0x02, 0x0c, 0x32, 0x00, 0x00, 0x00, /* 130 */
    0x8a, 0x0c, 0x12, 0x00, 0xa0, 0x42, 0x00, 0x24, 0x18, 0x00, 0x00, 0x00, /* 131 */
    0x98, 0x00, 0x02, 0x00, 0x16, 0x88, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, /* 132 */
    0xa9, 0x00, 0x00, 0x40, 0xa0, 0x00, 0xa0, 0x40, 0x00, 0x00, 0x00, 0x00, /* 133 */
    0x8b, 0x12, 0x00, 0x1e, 0x60, 0x12, 0x80, 0x12, 0x80, 0x40, 0x00, 0x00, /* 134 */
    0x9a, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x80, 0x00, 0x00, 0x00, /* 135 */
    0x8a, 0x06, 0x01, 0x08, 0x01, 0x10, 0x21, 0x80, 0x02, 0x00, 0x00, 0x00, /* 136 */
    0x8b, 0x06, 0x58, 0x80, 0x08, 0x84, 0x40, 0x06, 0x58, 0x80, 0x00, 0x00, /* 137 */
    0x8b, 0x12, 0x4c, 0x80, 0x10, 0x80, 0x50, 0x02, 0x4c, 0x80, 0x00, 0x00, /* 138 */
    0x8b, 0x02, 0x18, 0x24, 0x80, 0x44, 0x02, 0x48, 0x30, 0x80, 0x00, 0x00, /* 139 */
    0x8b, 0x06, 0x38, 0xc0, 0x20, 0x88, 0x26, 0xd8, 0x02, 0x08, 0x00, 0x00, /* 140 */
    0x8b, 0x02, 0x04, 0x08, 0x14, 0x40, 0xa4, 0x00, 0xbe, 0x40, 0x00, 0x00, /* 141 */
    0x8a, 0x04, 0x0a, 0x20, 0x0a, 0x20, 0x8a, 0x24, 0x1a, 0x00, 0x00, 0x00, /* 142 */
    0x1b, 0x00, 0x18, 0x21, 0x04, 0x41, 0x06, 0x40, 0x04, 0x40, 0x00, 0x00, /* 143 */
    0x8b, 0x02, 0x10, 0x6a, 0x00, 0xaa, 0x00, 0xac, 0x10, 0x80, 0x00, 0x00, /* 144 */
    0x8a, 0x06, 0x18, 0x60, 0x00, 0x82, 0x10, 0x82, 0x6c, 0x00, 0x00, 0x00, /* 145 */
    0x8b, 0x0e, 0x30, 0x40, 0x90, 0x0e, 0x70, 0x82, 0x10, 0x82, 0x00, 0x00, /* 146 */
    0x8b, 0x04, 0x22, 0x08, 0x22, 0x1c, 0x22, 0x08, 0x22, 0x10, 0x00, 0x00, /* 147 */
    0x8b, 0x1a, 0x24, 0x42, 0x08, 0x92, 0x20, 0x84, 0x48, 0xb0, 0x00, 0x00, /* 148 */
    0x8a, 0x0c, 0x11, 0x02, 0x2c, 0x12, 0x20, 0x44, 0x18, 0x00, 0x00, 0x00, /* 149 */
    0xa9, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, /* 150 */
    0x8b, 0x02, 0x04, 0x08, 0x14, 0x80, 0x24, 0x00, 0x3e, 0x80, 0x00, 0x00, /* 151 */
    0x8b, 0x0c, 0x12, 0x00, 0xa2, 0x00, 0x22, 0x00, 0xa4, 0x18, 0x00, 0x00, /* 152 */
    0x8b, 0x0c, 0x32, 0x00, 0x82, 0x00, 0x02, 0x00, 0x8c, 0x30, 0x00, 0x00, /* 153 */
    0x8a, 0x04, 0x0a, 0x20, 0x8a, 0x20, 0x0a, 0x24, 0x9a, 0x00, 0x00, 0x00, /* 154 */
    0x8a, 0x0c, 0x12, 0x00, 0xa0, 0x02, 0x00, 0x24, 0x98, 0x00, 0x00, 0x00, /* 155 */
    0x8b, 0x0c, 0x32, 0x80, 0x02, 0x00, 0x02, 0x0c, 0xb2, 0x00, 0x00, 0x00, /* 156 */
    0x8b, 0x06, 0x18, 0x22, 0x08, 0x22, 0x48, 0x22, 0x80, 0x20, 0x00, 0x00, /* 157 */
    0x8a, 0x0c, 0x12, 0x28, 0x02, 0x68, 0x02, 0xa8, 0x10, 0x00, 0x00, 0x00, /* 158 */
    0x8b, 0x08, 0x20, 0x88, 0x66, 0x18, 0x20, 0x48, 0x20, 0x80, 0x00, 0x00, /* 159 */
    0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 160 */
    0x9a, 0x00, 0x02, 0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, /* 161 */
    0x9a, 0x00, 0x20, 0x40, 0x80, 0x00, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, /* 162 */
    0x8b, 0x28, 0x06, 0x38, 0xc0, 0x28, 0x06, 0x38, 0xc0, 0x28, 0x00, 0x00, /* 163 */
    0x8a, 0x00, 0x24, 0x10, 0x46, 0x38, 0xc4, 0x10, 0x48, 0x00, 0x00, 0x00, /* 164 */
    0x8b, 0x40, 0x82, 0x44, 0x88, 0x10, 0x22, 0x44, 0x82, 0x04, 0x00, 0x00, /* 165 */
    0x8b, 0x0c, 0x10, 0x42, 0xa0, 0x12, 0xa8, 0x44, 0x0a, 0x10, 0x00, 0x00, /* 166 */
    0xc8, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, /* 167 */
    0xba, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, /* 168 */
    0x98, 0x00, 0x02, 0x00, 0x04, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, /* 169 */
    0x8b, 0x10, 0x04, 0x50, 0x28, 0x10, 0x28, 0x14, 0x40, 0x10, 0x00, 0x00, /* 170 */
    0x8b, 0x10, 0x00, 0x14, 0x08, 0x10, 0x20, 0x50, 0x00, 0x10, 0x00, 0x00, /* 171 */
    0x29, 0x00, 0x00, 0x01, 0x04, 0x0a, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, /* 172 */
    0x8b, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, /* 173 */
    0xa8, 0x00, 0x00, 0x02, 0x04, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, /* 174 */
    0x9a, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, /* 175 */
    0x8b, 0x1c, 0x20, 0x42, 0x00, 0x82, 0x00, 0x84, 0x08, 0x70, 0x00, 0x00, /* 176 */
    0x99, 0x00, 0x02, 0x00, 0x46, 0x18, 0x62, 0x80, 0x00, 0x00, 0x00, 0x00, /* 177 */
    0x8b, 0x02, 0x40, 0x06, 0x80, 0x0a, 0x80, 0x12, 0x80, 0x60, 0x00, 0x00, /* 178 */
    0x8b, 0x04, 0x00, 0x82, 0x00, 0x92, 0x00, 0xb2, 0x4c, 0x80, 0x00, 0x00, /* 179 */
    0x8b, 0x08, 0x10, 0x08, 0x20, 0x08, 0x46, 0x38, 0xc0, 0x08, 0x00, 0x00, /* 180 */
    0x8b, 0x04, 0x60, 0x82, 0x20, 0x82, 0x20, 0x84, 0x18, 0x80, 0x00, 0x00, /* 181 */
    0x8a, 0x0c, 0x10, 0x22, 0x10, 0x42, 0x10, 0x82, 0x0c, 0x00, 0x00, 0x00, /* 182 */
    0x8b, 0x80, 0x02, 0x84, 0x08, 0x90, 0x20, 0x80, 0x40, 0x80, 0x00, 0x00, /* 183 */
    0x8b, 0x0c, 0x62, 0x10, 0x82, 0x10, 0x82, 0x10, 0x8c, 0x60, 0x00, 0x00, /* 184 */
    0x8a, 0x60, 0x02, 0x90, 0x04, 0x90, 0x08, 0x90, 0x60, 0x00, 0x00, 0x00, /* 185 */
    0xa9, 0x00, 0x00, 0x02, 0x14, 0x22, 0x14, 0x20, 0x00, 0x00, 0x00, 0x00, /* 186 */
    0x2a, 0x00, 0x00, 0x01, 0x04, 0x2a, 0x44, 0x28, 0x40, 0x00, 0x00, 0x00, /* 187 */
    0x9a, 0x00, 0x10, 0x08, 0x24, 0x02, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, /* 188 */
    0x8a, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x00, 0x00, 0x00, /* 189 */
    0x9a, 0x00, 0x02, 0x00, 0x04, 0x80, 0x48, 0x20, 0x10, 0x00, 0x00, 0x00, /* 190 */
    0x8a, 0x48, 0x02, 0x80, 0x08, 0x80, 0x10, 0x80, 0x60, 0x00, 0x00, 0x00, /* 191 */
    0x8b, 0x1c, 0x20, 0x42, 0x80, 0x12, 0x88, 0x22, 0x88, 0x70, 0x00, 0x00, /* 192 */
    0x8b, 0x02, 0x04, 0x08, 0x10, 0x28, 0x40, 0x88, 0x00, 0xfe, 0x00, 0x00, /* 193 */
    0x8b, 0x06, 0x98, 0x62, 0x80, 0x12, 0x80, 0x12, 0x8c, 0x60, 0x00, 0x00, /* 194 */
    0x8b, 0x1c, 0x22, 0x40, 0x82, 0x00, 0x82, 0x00, 0x84, 0x40, 0x00, 0x00, /* 195 */
    0x8b, 0x06, 0x98, 0x62, 0x80, 0x02, 0x80, 0x04, 0x88, 0x70, 0x00, 0x00, /* 196 */
    0x8b, 0x06, 0x38, 0xc2, 0x10, 0x82, 0x10, 0x82, 0x00, 0x80, 0x00, 0x00, /* 197 */
    0x8b, 0x06, 0x38, 0xc0, 0x10, 0x80, 0x10, 0x80, 0x00, 0x80, 0x00, 0x00, /* 198 */
    0x8b, 0x1c, 0x22, 0x40, 0x82, 0x00, 0x92, 0x04, 0x98, 0x40, 0x00, 0x00, /* 199 */
    0x8b, 0x06, 0x38, 0xc0, 0x10, 0x00, 0x10, 0x06, 0x38, 0xc0, 0x00, 0x00, /* 200 */
    0x92, 0x00, 0x02, 0x00, 0x86, 0x18, 0xe2, 0x00, 0x80, 0x00, 0x00, 0x00, /* 201 */
    0x8b, 0x0c, 0x02, 0x00, 0x02, 0x80, 0x04, 0x98, 0x60, 0x80, 0x00, 0x00, /* 202 */
    0x8b, 0x06, 0x38, 0xc0, 0x10, 0x20, 0x08, 0x44, 0x02, 0x80, 0x00, 0x00, /* 203 */
    0x9a, 0x00, 0x06, 0x18, 0x62, 0x80, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, /* 204 */
    0x8b, 0x06, 0x38, 0xc0, 0x00, 0x30, 0x00, 0x46, 0x38, 0xc0, 0x00, 0x00, /* 205 */
    0x8b, 0x06, 0x38, 0xc0, 0x20, 0x10, 0x08, 0x06, 0x38, 0xc0, 0x00, 0x00, /* 206 */
    0x8b, 0x0c, 0x32, 0x40, 0x82, 0x00, 0x82, 0x04, 0x98, 0x60, 0x00, 0x00, /* 207 */
    0x8b, 0x06, 0x18, 0x60, 0x90, 0x00, 0x90, 0x00, 0x90, 0x60, 0x00, 0x00, /* 208 */
    0x8b, 0x1c, 0x20, 0x42, 0x00, 0x8a, 0x00, 0x84, 0x0a, 0x70, 0x00, 0x00, /* 209 */
    0x8b, 0x06, 0x18, 0x60, 0x80, 0x10, 0x88, 0x14, 0x82, 0x60, 0x00, 0x00, /* 210 */
    0x8b, 0x04, 0x62, 0x00, 0x92, 0x00, 0x92, 0x00, 0x8c, 0x40, 0x00, 0x00, /* 211 */
    0x8b, 0x80, 0x00, 0x86, 0x18, 0xe0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, /* 212 */
    0x8b, 0x0c, 0x32, 0xc0, 0x02, 0x00, 0x02, 0x0c, 0x30, 0xc0, 0x00, 0x00, /* 213 */
    0x9b, 0x00, 0xfe, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, /* 214 */
    0x8b, 0x06, 0x38, 0xc4, 0x08, 0x10, 0x08, 0x06, 0x38, 0xc0, 0x00, 0x00, /* 215 */
    0x8b, 0x02, 0x84, 0x48, 0x20, 0x18, 0x24, 0x02, 0x40, 0x80, 0x00, 0x00, /* 216 */
    0x8b, 0x80, 0x40, 0x26, 0x18, 0x00, 0x20, 0x00, 0x40, 0x80, 0x00, 0x00, /* 217 */
    0x8b, 0x02, 0x04, 0x8a, 0x00, 0x92, 0x00, 0xa2, 0x40, 0x80, 0x00, 0x00, /* 218 */
    0x9b, 0x00, 0x06, 0x18, 0x62, 0x80, 0x02, 0x80, 0x00, 0x80, 0x00, 0x00, /* 219 */
    0xa8, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, /* 220 */
    0x8a, 0x02, 0x00, 0x02, 0x80, 0x06, 0x98, 0x60, 0x80, 0x00, 0x00, 0x00, /* 221 */
    0x9a, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 0x40, 0x20, 0x00, 0x00, 0x00, /* 222 */
    0x0b, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, /* 223 */
    0xb7, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 224 */
    0x8a, 0x04, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x24, 0x1a, 0x00, 0x00, 0x00, /* 225 */
    0x8a, 0x06, 0x18, 0xe2, 0x00, 0x22, 0x00, 0x24, 0x18, 0x00, 0x00, 0x00, /* 226 */
    0x8a, 0x0c, 0x10, 0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, /* 227 */
    0x8b, 0x0c, 0x10, 0x02, 0x20, 0x02, 0x20, 0x06, 0x38, 0xc0, 0x00, 0x00, /* 228 */
    0x8a, 0x0c, 0x12, 0x28, 0x02, 0x28, 0x02, 0x28, 0x10, 0x00, 0x00, 0x00, /* 229 */
    0x8b, 0x20, 0x00, 0x26, 0x18, 0x60, 0x00, 0xa0, 0x00, 0x80, 0x00, 0x00, /* 230 */
    0x1b, 0x00, 0x18, 0x25, 0x00, 0x45, 0x00, 0x46, 0x18, 0x60, 0x00, 0x00, /* 231 */
    0x8a, 0x06, 0x18, 0xe0, 0x00, 0x20, 0x00, 0x26, 0x18, 0x00, 0x00, 0x00, /* 232 */
    0x99, 0x00, 0x02, 0x00, 0x26, 0x18, 0x22, 0x80, 0x00, 0x00, 0x00, 0x00, /* 233 */
    0x89, 0x01, 0x00, 0x01, 0x00, 0x26, 0x18, 0xa0, 0x00, 0x00, 0x00, 0x00, /* 234 */
    0x8a, 0x06, 0x18, 0x60, 0x88, 0x04, 0x12, 0x00, 0x20, 0x00, 0x00, 0x00, /* 235 */
    0x99, 0x00, 0x02, 0x00, 0x06, 0x98, 0x62, 0x80, 0x00, 0x00, 0x00, 0x00, /* 236 */
    0x8a, 0x26, 0x18, 0x20, 0x06, 0x38, 0x00, 0x26, 0x18, 0x00, 0x00, 0x00, /* 237 */
    0x89, 0x26, 0x18, 0x20, 0x00, 0x20, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, /* 238 */
    0x8a, 0x0c, 0x12, 0x00, 0x20, 0x02, 0x00, 0x24, 0x18, 0x00, 0x00, 0x00, /* 239 */
    0x0a, 0x03, 0x1c, 0x60, 0x04, 0x40, 0x04, 0x48, 0x30, 0x00, 0x00, 0x00, /* 240 */
    0x1b, 0x00, 0x18, 0x24, 0x00, 0x44, 0x00, 0x47, 0x18, 0x60, 0x00, 0x00, /* 241 */
    0x89, 0x06, 0x38, 0x00, 0x10, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, /* 242 */
    0x8a, 0x02, 0x10, 0x02, 0x28, 0x02, 0x28, 0x04, 0x20, 0x00, 0x00, 0x00, /* 243 */
    0x9a, 0x00, 0x20, 0x0c, 0x32, 0xc0, 0x22, 0x00, 0x20, 0x00, 0x00, 0x00, /* 244 */
    0x8a, 0x0c, 0x32, 0x00, 0x02, 0x00, 0x02, 0x0c, 0x32, 0x00, 0x00, 0x00, /* 245 */
    0x9a, 0x00, 0x3e, 0x00, 0x04, 0x00, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00, /* 246 */
    0x8b, 0x0e, 0x30, 0x04, 0x00, 0x18, 0x04, 0x00, 0x06, 0x38, 0x00, 0x00, /* 247 */
    0x8b, 0x02, 0x00, 0x24, 0x10, 0x08, 0x04, 0x12, 0x00, 0x20, 0x00, 0x00, /* 248 */
    0x1b, 0x00, 0x40, 0x21, 0x12, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, /* 249 */
    0x8b, 0x02, 0x00, 0x26, 0x00, 0x2a, 0x00, 0x32, 0x00, 0x20, 0x00, 0x00, /* 250 */
    0x9a, 0x00, 0x10, 0x04, 0x1a, 0x60, 0x82, 0x00, 0x80, 0x00, 0x00, 0x00, /* 251 */
    0x99, 0x00, 0x02, 0x04, 0x08, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, /* 252 */
    0x9a, 0x00, 0x02, 0x00, 0x82, 0x0c, 0xb0, 0x40, 0x10, 0x00, 0x00, 0x00, /* 253 */
    0x8b, 0x40, 0x80, 0x00, 0x80, 0x40, 0x20, 0x00, 0x20, 0x40, 0x00, 0x00, /* 254 */
    0x8b, 0x1a, 0x24, 0x42, 0x08, 0x92, 0x20, 0x84, 0x48, 0xb0, 0x00, 0x00  /* 255 */
};

static BYTE MSXFont[256 * 9];
static BYTE MSXFontRaw[MSX_FONT_SIZE];


typedef enum { 
    CC_USA              = 0,
    CC_FRANCE           = 1,
    CC_GERMANY          = 2,
    CC_UNITED_KINGDOM   = 3,
    CC_DENMARK          = 4,
    CC_SWEDEN           = 5,
    CC_ITALY            = 6,
    CC_SPAIN            = 7,
    CC_JAPAN            = 8
} CountryCodes;

#define MAX_ESC_CMDSIZE  8 
#define MAX_FONT_WIDTH   12

typedef struct {
    BYTE rom[256 * MAX_FONT_WIDTH];
    BYTE ram[256 * MAX_FONT_WIDTH];
    BOOL useRam;
    UINT charWidth;
    double pixelDelta;
} FontInfo;

typedef struct {
    BOOL    fLetterQuality;
    BOOL    fBold;
    BOOL    fProportional;
    BOOL    fItalic;
    BOOL    fSuperscript;
    BOOL    fSubscript;
    BOOL    fDoubleWidth;
    BOOL    fUnderline;
    BOOL    fDoubleStrike;
    BOOL    fEscSequence;
    BOOL    fAlternateChar;
    BOOL    fDetectPaperOut;
    BOOL    fJapanese;
    BOOL    fNormalAfterLine;
    BOOL    fNinePinGraphics;
    BOOL    fLeftToRight;
    BOOL    fGraphicsHiLo;
    BOOL    fElite;
    BOOL    fCompressed;
    BOOL    fNoHighEscapeCodes;
    int     uiEightBit;
    UINT    uiPerforationSkip;
    UINT    uiLeftBorder;
    UINT    uiRightBorder;
    UINT    uiTotalWidth;
    UINT    uiTotalHeight;
    UINT    uiFontWidth;
    UINT    uiRemainingCommandBytes;
    size_t  sizeEscPos;
    BYTE    abEscSeq[MAX_ESC_CMDSIZE];
    size_t  sizeRemainingDataBytes;
    UINT    ramLoadOffset;
    UINT    ramLoadEnd;
    double  uiGraphDensity;
    double  uiFontDensity;
    double  uiHpos;
    double  uiVpos;
    double  uiPageTop;
    double  uiLineFeed;
    double  uiPageHeight;
    UINT    uiLines;
    UINT    uiPrintAreaTop;
    UINT    uiPrintAreaBottom;
    double  uiPixelSizeX;
    double  uiPixelSizeY;
    CountryCodes countryCode;
    FontInfo fontInfo;
} PRINTERRAM;

PRINTERRAM   stPrtRam;
BOOL         fPrintDataOnPage;
ULONG        ulPrintPage = 1;
HDC          hdcPrinter = NULL;
HDC          hdcMem = NULL;
HBITMAP      hBitmap = NULL;

static char  printDir[MAX_PATH];
static BYTE  abGrphImage[512*2][sizeof(DWORD) * PIXEL_WIDTH * 9];
static TCHAR szDocTitle[MAX_PATH];

static struct {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[2];
} bmiGrph;

void FlushEmulatedPrinter(void)
{
    if (hdcPrinter && hdcMem) {
        if (fPrintDataOnPage && stPrtRam.uiPrintAreaBottom > stPrtRam.uiPrintAreaTop) {
            int w = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH);
            int h = (int)(stPrtRam.uiPrintAreaBottom - stPrtRam.uiPrintAreaTop);
            BitBlt(hdcPrinter, 0, stPrtRam.uiPrintAreaTop, w, h, hdcMem, 0, 0, SRCCOPY);
            EndPage( hdcPrinter );
            EndDoc( hdcPrinter );
            eraseBackground(hdcMem);
            stPrtRam.uiPrintAreaTop    = (UINT)-1;
            stPrtRam.uiPrintAreaBottom = 0;
            ulPrintPage++;
        }
        else
            AbortDoc( hdcPrinter );
    }

    stPrtRam.uiHpos = stPrtRam.uiLeftBorder;
    stPrtRam.uiVpos = stPrtRam.uiPageTop;

    fPrintDataOnPage=FALSE;
}

static void SeekPrinterHeadRelative(double iOffset)
{
    stPrtRam.uiHpos += iOffset;

    if ((UINT)stPrtRam.uiHpos > stPrtRam.uiRightBorder) {
        stPrtRam.uiHpos = stPrtRam.uiLeftBorder;

        stPrtRam.uiVpos += stPrtRam.uiLineFeed;
        if (stPrtRam.uiVpos >= stPrtRam.uiPageHeight) {
            stPrtRam.fDoubleWidth ^= stPrtRam.fNormalAfterLine;
            FlushEmulatedPrinter();
        }
    }
}

void MsxPrnSetFont(BYTE* msxBits)
{
    // Convert MSX printer font to Epson printer font
    BYTE* font = MSXFont;
    int i;

    if (printerType != P_LPT_MSXPRN) {
        return;
    }

    for (i = 0; i < 256; i++) {
        BYTE oneBits = 0;
        int start = -1;
        int end = 0;
        int j;

        // Rotate MSX character
        for (j = 0; j < 8; j++) {
            BYTE charBits = 0;
            int k;
            
            for (k = 0; k < 8; k++) {
                charBits |= ((msxBits[7 - k] >> (7 - j)) & 1) << k;
            }
            
            oneBits |= charBits;
            if (oneBits  != 0 && start < 0) start = j;
            if (charBits != 0) end = j;

            font[j + 1] = charBits;
        }

        end = end + 1;
        if (start < 0 || start >= 7) start = 0;
        if (end   == 1) end   = 5;
        if (end   >= 7) end   = 7;

        font[0] = (start << 4) | end;

        font    += 9;
        msxBits += 8;
    }
}

void EnsurePrintPage(void)
{
    DOCINFO di;
    TCHAR   szDocName[MAX_PATH];

    if (!fPrintDataOnPage) {
        if (hdcPrinter) {
            char originalDir[MAX_PATH];

            ZeroMemory( &di, sizeof(di) );
            di.cbSize = sizeof(di);
            di.lpszDocName = szDocName;

            wsprintf( szDocName, TEXT("%s (%lu)"),
                szDocTitle, ulPrintPage );

            // We need to restore the current directory after
            // StartDoc is called in case we use a virtual
            // printer that writes to file and open a dialog
            // box. Also set current dir to the dir of last
            // access to this method before calling StartPage

            GetCurrentDirectory(MAX_PATH - 1, originalDir);
            if (*printDir) {
                SetCurrentDirectory(printDir);
            }
            StartDoc( hdcPrinter, &di );
            StartPage( hdcPrinter );
            
            GetCurrentDirectory(MAX_PATH - 1, printDir);
            SetCurrentDirectory(originalDir);
        }

        fPrintDataOnPage=TRUE;
    }
}

void PrintVisibleCharacter(BYTE bChar)
{
    EnsurePrintPage();

    if (hdcMem) {
        double iHeight = stPrtRam.uiPixelSizeY * 9; // Font Height
        double iYPos = 0;
        BYTE *charBitmap = (stPrtRam.fontInfo.useRam ? stPrtRam.fontInfo.ram : stPrtRam.fontInfo.rom) 
                           + stPrtRam.fontInfo.charWidth * (UINT)bChar;
        BYTE attribute   = charBitmap[0];
        UINT start       = (attribute >> 4) & 0x07;
        UINT end         = attribute & 0x0f;
        UINT topBits     = attribute >> 7;
        double headRelative;
        double hPos;
        UINT script = stPrtRam.fSuperscript || stPrtRam.fSubscript ? 1 : 0;
        UINT destY;
        UINT destHeight;
        UINT dblStrikeOffset;
        UINT i;

        if (!stPrtRam.fProportional) {
            start = 0;  // Fixed width font
            end   = stPrtRam.fontInfo.charWidth - 1;
        }

        if (stPrtRam.fSubscript) {
            iYPos /=2;
            iYPos += stPrtRam.uiPixelSizeY * 4.5;
        }
        if (script) {
            iYPos -= stPrtRam.uiPixelSizeY * 4.5;
        }

        hPos = stPrtRam.uiHpos;

        headRelative = (stPrtRam.fDoubleWidth ? 2 : 1) * stPrtRam.fontInfo.pixelDelta / stPrtRam.uiFontDensity;
        
        destY      = (UINT)(stPrtRam.uiVpos * stPrtRam.uiPixelSizeY + iYPos);
        destHeight = (UINT)(stPrtRam.uiPixelSizeY * 9);
        dblStrikeOffset = (UINT)(stPrtRam.fDoubleStrike ? stPrtRam.uiPixelSizeY / 2.5 : 0);
        stPrtRam.uiPrintAreaTop    = MIN(stPrtRam.uiPrintAreaTop, destY);
        stPrtRam.uiPrintAreaBottom = MAX(stPrtRam.uiPrintAreaBottom, destY + destHeight + dblStrikeOffset);

        for (i = start; i < end; i++) {
            UINT charBits = (UINT)charBitmap[i + 1] << topBits;
            int y, b, d;

            if (stPrtRam.fUnderline) {
                charBits |= 2;
            }
            
            for (d = 0; d <= stPrtRam.fDoubleWidth; d++) {
                for (b = 0; b <= stPrtRam.fBold; b++) {
                    for (y = 0; y <= stPrtRam.fDoubleStrike; y++) {
                        UINT destX = (UINT)((hPos + (d + b / 2.) / stPrtRam.uiFontDensity) * stPrtRam.uiPixelSizeX);
                        StretchDIBits(hdcMem,
                            destX, destY + y * dblStrikeOffset,
                            (UINT)(stPrtRam.uiPixelSizeX),  destHeight,
                            0, 0, 1 * PIXEL_WIDTH, 9 * PIXEL_WIDTH,
                            abGrphImage[512 * script + charBits], 
                            (LPBITMAPINFO)&bmiGrph, DIB_RGB_COLORS, SRCAND);
                    }
                }
            }

            hPos += headRelative;
        }

        SeekPrinterHeadRelative((1 + end - start) * headRelative);
    }
}

#define SWAP_BITS_8(v) (((v&0x80)>>7)|((v&0x40)>>5)|((v&0x20)>>3)|((v&0x10)>>1)|((v&0x08)<<1)|((v&0x04)<<3)|((v&0x02)<<5)|((v&0x01)<<7))

void PrintGraphicByte(BYTE bByte, BOOL fMsxPrinter)
{
    EnsurePrintPage();

    if (hdcMem) {
        UINT destY      = (UINT)(stPrtRam.uiVpos * stPrtRam.uiPixelSizeY);
        UINT destHeight = (UINT)(stPrtRam.uiPixelSizeY * 9);
        UINT charBits;

        if (fMsxPrinter) {
            bByte = SWAP_BITS_8(bByte);
        }

        // Print Data to high 8 bits
        charBits = (UINT)bByte << 1;

        stPrtRam.uiPrintAreaTop    = MIN(stPrtRam.uiPrintAreaTop, destY);
        stPrtRam.uiPrintAreaBottom = MAX(stPrtRam.uiPrintAreaBottom, destY + destHeight);

        // Print bit-mask
        StretchDIBits(hdcMem,
            (UINT)(stPrtRam.uiHpos*stPrtRam.uiPixelSizeX), destY,
            (UINT)(stPrtRam.uiPixelSizeX),                 destHeight,
            0, 0, 1 * PIXEL_WIDTH, 9 * PIXEL_WIDTH,
            abGrphImage[charBits], (LPBITMAPINFO)&bmiGrph,
            DIB_RGB_COLORS, SRCAND );

        // Move print-position...
        SeekPrinterHeadRelative(1./stPrtRam.uiGraphDensity);
    }

    return;
}

////////////////////////////////////////////////////
// MSX Printer specific methods
////////////////////////////////////////////////////

static void MsxPrnResetSettings(void)
{
    stPrtRam.uiLineFeed     = 12.0;
    stPrtRam.uiLeftBorder   = 48;
    stPrtRam.uiRightBorder  = 800;
    stPrtRam.uiGraphDensity = 1.0;
    stPrtRam.uiFontDensity  = 1.0;
    stPrtRam.uiPageTop      = 48;
    stPrtRam.uiLines        = 72;
    stPrtRam.uiTotalWidth   = 825;
    stPrtRam.uiTotalHeight  = 825;
    stPrtRam.uiFontWidth    = 8;
    stPrtRam.fGraphicsHiLo  = TRUE;
    stPrtRam.uiEightBit     = -1;
}

static UINT MsxPrnParseNumber(size_t sizeStart, size_t sizeChars)
{
    UINT uiValue = 0;
    BYTE bChar;

    while (sizeChars--) {
        uiValue=uiValue*10;
        bChar=stPrtRam.abEscSeq[sizeStart++];
        if (bChar>='0' && bChar<='9')
            uiValue+=(UINT)(bChar-'0');
    }

    return uiValue;
}

static size_t MsxPrnCalcEscSequenceLength(BYTE character) 
{
    switch (character) {
    case 'A':
    case 'C':
        return 1;
    case 'T':
    case 'Z':
        return 2;
    case 'O':
    case '\\':
    case 'L':
    case '/':
        return 3;
    case 'S':
        return 4;
    case 'G':
        return 7;
    }

    return 0;
}

static void MsxPrnProcessEscSequence(void)
{
    switch (stPrtRam.abEscSeq[0]) {
    case 'N':
        stPrtRam.fProportional = FALSE; 
        stPrtRam.uiFontDensity = 1.0;
        break;

    case 'E':
        stPrtRam.fProportional = FALSE; 
        stPrtRam.uiFontDensity = 1.40;
        break;

    case 'Q':
        stPrtRam.fProportional = FALSE; 
        stPrtRam.uiFontDensity = 1.72;
        break;

    case 'P':
        stPrtRam.fProportional = TRUE;
        stPrtRam.uiFontDensity = 0.90;
        break;

    case '!':
        stPrtRam.fLetterQuality=TRUE;
        break;

    case '\"':
        stPrtRam.fLetterQuality=FALSE;
        break;

    case 'C':
        switch (stPrtRam.abEscSeq[1])
        {
        case 'D':
            stPrtRam.fDoubleStrike=TRUE;
            break;

        case 'd':
            stPrtRam.fDoubleStrike=FALSE;
            break;

        case 'I':
            stPrtRam.fItalic=TRUE;
            break;

        case 'i':
            stPrtRam.fItalic=FALSE;
            break;

        case 'B':
            stPrtRam.fBold=TRUE;
            break;

        case 'b':
            stPrtRam.fBold=FALSE;
            break;

        case 'S':
            stPrtRam.fSuperscript=TRUE;
            break;

        case 's':
            stPrtRam.fSuperscript=FALSE;
            break;

        case 'U':
            stPrtRam.fSubscript=TRUE;
            break;

        case 'u':
            stPrtRam.fSubscript=FALSE;
            break;
        }
        break;

    case '(':
        // ???: Set a horizontal tab position
        break;

    case ')':
        // ???: Partially delete a horizontal tab position
        break;

    case '2':
        // ???: Clear horizontal tabs
        break;

    case 'O':
        switch (stPrtRam.abEscSeq[1])
        {
        case 'S':
            stPrtRam.uiPerforationSkip=MsxPrnParseNumber( 2, 2 );
            break;

        case 'I':
            // ???: Set page-height(inches)
            break;

        default:
            // ???: Set page-height (lines)
            break;
        }
        break;

    case '/':  //Right margin
        break;

    case 'L':
        stPrtRam.uiLeftBorder=MsxPrnParseNumber( 1, 3 );
        break;

    case 'A':
      // ???: Line-feed 1/6"
        stPrtRam.uiLineFeed = 12.0;
        break;

    case 'B':
        // ???: Line-feed 1/9"
        stPrtRam.uiLineFeed = 8.0;
        break;

    case 'T':
        // ???: Line-feed nn/144"
        stPrtRam.uiLineFeed = MsxPrnParseNumber( 1, 2 ) / 2.0;
        break;

    case 'Z':
        // ???: Line-feed nn/216"
        stPrtRam.uiLineFeed = MsxPrnParseNumber( 1, 2 ) / 3.0;
        break;

    case '[':
        // ???: Uni-directional printing
        break;

    case ']':
        // ???: Bi-directional printing
        break;

    case 'p':
        stPrtRam.fDetectPaperOut=TRUE;
        break;

    case 'q':
        stPrtRam.fDetectPaperOut=FALSE;
        break;

    case 13:
        // (ESC, CR) Move printer-head to end-position
        break;

    case '@':
        ResetEmulatedPrinter();
        break;

    case '\\':
        stPrtRam.uiRightBorder=MsxPrnParseNumber( 1, 3 );
        break;

    case 'G':
        stPrtRam.uiGraphDensity=MsxPrnParseNumber( 1, 3 ) / 100.0;
        if (stPrtRam.uiGraphDensity < 0.1) {
            stPrtRam.uiGraphDensity = 0.1;
        }
        stPrtRam.sizeRemainingDataBytes=MsxPrnParseNumber( 4, 4 );
        break;

    case 'S':
        // Print graphics, density depending on font
        stPrtRam.sizeRemainingDataBytes=MsxPrnParseNumber( 1, 3 );
        break;

    case 'X':
        stPrtRam.fUnderline=TRUE;
        break;

    case 'Y':
        stPrtRam.fUnderline=FALSE;
        break;

    case '&':
    case '$':
        stPrtRam.fJapanese=!stPrtRam.fJapanese;
        break;

    case 'f':
        // ???: Scroll paper forward
        break;

    case 'r':
        // ???: Scroll paper back
        break;
    }
}

static void MsxPrnProcessCharacter(BYTE bChar)
{
    if (stPrtRam.fAlternateChar)
    {
        // Print SOH-preceded character
        PrintVisibleCharacter( (BYTE)(bChar & 0x1F) );
        stPrtRam.fAlternateChar=FALSE;
    }
    else {
        switch (bChar) {
        case 1:                                 // SOH: A symbolcode preceding code
            stPrtRam.fAlternateChar=TRUE;
            break;

        case 7:                                 // BEL: Audible beep (buzzer, 0.3s)
            break;

        case 8:                                 // BS: Backstep (1 Character)
            // ToDo: fix for other font-sizes ***
            stPrtRam.uiHpos -= 8;
            if (stPrtRam.uiHpos < stPrtRam.uiLeftBorder)
                stPrtRam.uiHpos=stPrtRam.uiLeftBorder;
            break;

        case 9:                                 // HAT: Horizontal tabulator
            {
                // ToDo: fix for other font-sizes ***
                UINT            uiNewPos;
                uiNewPos = ((UINT)stPrtRam.uiHpos + 64
                    - stPrtRam.uiLeftBorder) & ~63;
                uiNewPos += stPrtRam.uiLeftBorder;

                if (uiNewPos >= stPrtRam.uiHpos)
                    break;
                // else fall thru: CR/LF
            }

        case 10:                                // LF: Carriage return + Line feed
        case 11:                                // VT: Vertical tabulator (like LF)
//            stPrtRam.uiHpos = stPrtRam.uiLeftBorder;
            stPrtRam.uiVpos += stPrtRam.uiLineFeed;
            if (stPrtRam.uiVpos >= stPrtRam.uiPageHeight)
                FlushEmulatedPrinter();
            break;

        case 12:                                // FF: Form feed
            EnsurePrintPage();
            FlushEmulatedPrinter();
            break;

        case 13:                                // CR: Carriage return
            stPrtRam.uiHpos = stPrtRam.uiLeftBorder;
            break;

        case 14:                                // SO: Double character-width on
            stPrtRam.fDoubleWidth=TRUE;
            break;

        case 15:                                // SI: Double character-width off
            stPrtRam.fDoubleWidth=FALSE;
            break;

        case 27:
            stPrtRam.fEscSequence=TRUE;
            break;

        default:
            if (bChar<0 || bChar>=32)
            {
                // Yes, we can print it!
                PrintVisibleCharacter( bChar );
            }
            break;
        }
    }
}

////////////////////////////////////////////////////
// Epson FX80 specific methods
////////////////////////////////////////////////////

static UINT EpsonFx80ParseNumber(size_t sizeStart, size_t sizeChars)
{
    UINT uiValue = 0;

    sizeStart += sizeChars;

    while (sizeChars--) {
        uiValue = uiValue * 256;
        uiValue += stPrtRam.abEscSeq[--sizeStart];
    }

    return uiValue;
}

static void EpsonFx80ResetSettings(void)
{
    stPrtRam.uiLineFeed     = 12.0;
    stPrtRam.uiLeftBorder   = 48;
    stPrtRam.uiRightBorder  = stPrtRam.uiLeftBorder + 480;
    stPrtRam.uiGraphDensity = 1.0;
    stPrtRam.uiFontDensity  = 1.0;
    stPrtRam.uiPageTop      = 48;
    stPrtRam.uiLines        = 72;
    stPrtRam.uiTotalWidth   = 610;
    stPrtRam.uiTotalHeight  = 825;
    stPrtRam.uiFontWidth    = 6;
    stPrtRam.fGraphicsHiLo  = FALSE;
    stPrtRam.uiEightBit     = -1;
}

static size_t EpsonFx80CalcEscSequenceLength(BYTE character) 
{
    character &= 127;

    switch (character) {
    case '!':
    case '-':
    case '/':
    case '3':
    case 'A':
    case 'J':
    case 'N':
    case 'Q':
    case 'R':
    case 'S':
    case 'U':
    case 'W':
    case 'b':
    case 'i':
    case 'j':
    case 'l':
    case 'p':
    case 's':
        return 1;
    case '%':
    case '?':
    case 'K':
    case 'L':
    case 'Z':
    case '^':
        return 2;
    case '*':
    case ':':
    case '&':
        return 3;

    case 'B': // Set tabs, variable length (up to 16 tabs)
        return 0;
    case 'C': // Set form length, variable length (2 or 3)
        return 0;
    case 'D': // Set tabs, variable length (up to 32 tabs)
        return 0;
    }
    return 0;
}

static void EpsonFx80ProcessEscSequence(void)
{
    BYTE character = stPrtRam.abEscSeq[0] & 127;

    switch (character) {
    case '!':  // Master Print Mode Select
        {
            UINT masterSelect      = EpsonFx80ParseNumber(1, 1);
            stPrtRam.fElite        = (masterSelect & 1) != 0;
            stPrtRam.fCompressed   = (masterSelect & 4) != 0;
            stPrtRam.fBold         = (masterSelect & 8) != 0;
            stPrtRam.fDoubleStrike = (masterSelect & 16) != 0;
            stPrtRam.fDoubleWidth  = (masterSelect & 32) != 0;

            if (stPrtRam.fElite) {
                stPrtRam.uiFontDensity = 1.20;
            }
            else if (stPrtRam.fCompressed) {
                stPrtRam.uiFontDensity = 1.72;
            }
            else {
                stPrtRam.uiFontDensity = 1.00;
            }
        }
        break;

    case '#':  // Accept Eight Bit as-is
        break;

    case '%':  // Activates Character Set
        stPrtRam.fontInfo.useRam = EpsonFx80ParseNumber(1, 1) & 1;
        break;

    case '&': // Custom character set, variable length
        stPrtRam.ramLoadOffset = 12 * (UINT)EpsonFx80ParseNumber(2, 1);
        stPrtRam.ramLoadEnd    = 12 * (UINT)EpsonFx80ParseNumber(3, 1) + 12;
        if (stPrtRam.ramLoadEnd <= stPrtRam.ramLoadOffset) {
            stPrtRam.ramLoadEnd = stPrtRam.ramLoadOffset;
        }
        break;

    case '*':  // Turn Graphics Mode ON
        stPrtRam.fNinePinGraphics = FALSE;
        switch(EpsonFx80ParseNumber(1, 1)) {
        default:
        case 0:
            stPrtRam.uiGraphDensity = 1.0;
            break;
        case 1:
        case 2:
            stPrtRam.uiGraphDensity = 2.0;
            break;
        case 3:
            stPrtRam.uiGraphDensity = 4.0;
            break;
        case 4:
            stPrtRam.uiGraphDensity = 1.33;
            break;
        case 5:
            stPrtRam.uiGraphDensity = 1.2;
            break;
        case 6:
            stPrtRam.uiGraphDensity = 1.5;
            break;
        }
        stPrtRam.sizeRemainingDataBytes = EpsonFx80ParseNumber(2, 2);
        break;

    case '-':  // Turn Underline Mode ON/OFF
        stPrtRam.fUnderline = EpsonFx80ParseNumber(1, 1);
        break;

    case '/':  // Selects Vertical Tab Channel
        break;

    case '0':  // Sets Line Spacing to 1/8 inch
        stPrtRam.uiLineFeed = 9.0;
        break;

    case '1':  // Sets Line Spacing to 7/72 inch
        stPrtRam.uiLineFeed = 7.0;
        break;

    case '2':  // Sets Line Spacing to 1/6 inch
        stPrtRam.uiLineFeed = 12.0;
        break;

    case '3':  // Sets Line Spacing to n/216 inch
        stPrtRam.uiLineFeed = (EpsonFx80ParseNumber(1, 1) & 127) / 3.0;
        break;

    case '4':  // Turn Italic Mode ON
        stPrtRam.fItalic = TRUE;
        break;

    case '5':  // Turn Italic Mode OFF
        stPrtRam.fItalic = FALSE;
        break;

    case '6':  // Turn Printing of International Italic characters ON
        stPrtRam.fNoHighEscapeCodes = TRUE;
        break;

    case '7':  // Turn Printing of International Italic characters OFF
        stPrtRam.fNoHighEscapeCodes = FALSE;
        break;

    case '8':  // Turn Paper Out Sensor ON
        stPrtRam.fDetectPaperOut = TRUE;
        break;

    case '9':  // Turn Paper Out Sensor OFF
        stPrtRam.fDetectPaperOut = FALSE;
        break;

    case ':':  // Copies Rom Character set to RAM
        memcpy(stPrtRam.fontInfo.ram, stPrtRam.fontInfo.rom, sizeof(stPrtRam.fontInfo.ram));
        break;

    case '<':  // Turn Uni-directional printing ON (left to right)
        stPrtRam.fLeftToRight = TRUE;
        break;

    case '=':  // Sets eight bit to 0
        stPrtRam.uiEightBit = 0;
        break;

    case '>':  // Sets eight bit to 1
        stPrtRam.uiEightBit = 1;
        break;

    case '?':  // Redefines Graphics Codes
        break;

    case '@':  // Reset
        stPrtRam.uiEightBit = -1;
        stPrtRam.fNinePinGraphics = FALSE;
        stPrtRam.uiGraphDensity = 1.0;
        stPrtRam.uiFontDensity  = 1.0;
        stPrtRam.fUnderline = FALSE;
        stPrtRam.uiLineFeed = 12.0;
        stPrtRam.fUnderline = FALSE;
        stPrtRam.fItalic = FALSE;
        stPrtRam.fDetectPaperOut = FALSE;
        stPrtRam.fLeftToRight = FALSE;
        stPrtRam.fDoubleStrike = FALSE;
        stPrtRam.fElite = FALSE;
        stPrtRam.fCompressed = FALSE;
        stPrtRam.uiRightBorder = 6 * 78;
        stPrtRam.fSubscript =   FALSE;
        stPrtRam.fSuperscript = FALSE;
        stPrtRam.fLeftToRight = FALSE;
        stPrtRam.fDoubleWidth = FALSE;
        stPrtRam.fBold = FALSE;
        stPrtRam.fProportional = FALSE;
        stPrtRam.fontInfo.useRam = FALSE;
        stPrtRam.fNoHighEscapeCodes = FALSE;
        stPrtRam.fAlternateChar = FALSE;
        stPrtRam.countryCode = CC_USA;
        break;

    case 'A':  // Sets Line Spacing to n/72 inch
        stPrtRam.uiLineFeed = EpsonFx80ParseNumber(1, 1) & 127;
        break;

    case 'B':  // Set tabs, variable length (up to 16 tabs)
        break;

    case 'C': // Set form length, variable length (2 or 3)
        break;

    case 'D': // Set tabs, variable length (up to 32 tabs)
        break;

    case 'E':  // Turn Emphasized Mode ON
        stPrtRam.fBold = TRUE;
        break;

    case 'F':  // Turn Emphasized Mode OFF
        stPrtRam.fBold = FALSE;
        break;

    case 'G':  // Turn Double Strike Mode ON
        stPrtRam.fDoubleStrike = TRUE;
        break;

    case 'H':  // Turn Double Strike Mode OFF
        stPrtRam.fDoubleStrike = FALSE;
        break;

    case 'I':  // Enables printing of chars 1-31
        stPrtRam.fAlternateChar = EpsonFx80ParseNumber(1, 1) & 1;
        break;

    case 'J':  // Forces Line Feed with n/216 inch
        stPrtRam.uiVpos += (EpsonFx80ParseNumber(1, 1) & 127) / 3.0;
        if (stPrtRam.uiVpos >= stPrtRam.uiPageHeight)
            FlushEmulatedPrinter();
        break;

    case 'K':  // Turn Single Density Graphics on (480 dot mode)
        stPrtRam.uiGraphDensity = 1.0;
        stPrtRam.fNinePinGraphics = FALSE;
        stPrtRam.sizeRemainingDataBytes = EpsonFx80ParseNumber(1, 2);
        break;

    case 'L':  // Turn Double Density Graphics on (960 dot mode)
        stPrtRam.uiGraphDensity = 2.0;
        stPrtRam.fNinePinGraphics = FALSE;
        stPrtRam.sizeRemainingDataBytes = EpsonFx80ParseNumber(1, 2);
        break;

    case 'M':  // Turn Elite mode ON
        stPrtRam.fElite = TRUE;        
        stPrtRam.uiFontDensity = 1.20;
        break;

    case 'N':  // Turn Skip Over Perforation ON
        break;

    case 'O':  // Turn Skip Over Perforation OFF
        break;

    case 'P':  // Turn Elite mode OFF
        stPrtRam.fElite = FALSE;
        if (stPrtRam.fCompressed) {
            stPrtRam.uiFontDensity = 1.72;
        }
        else {
            stPrtRam.uiFontDensity = 1.00;
        }
        break;

    case 'Q':  // Set Right Margin
        {
            int width = EpsonFx80ParseNumber(1, 2);
            if (width > 78) width = 78; // FIXME Font dependent !!
            stPrtRam.uiRightBorder = 6 * width;
        }
        break;

    case 'R':  // Select International Character Set
        stPrtRam.countryCode = EpsonFx80ParseNumber(1, 1);
        if (stPrtRam.countryCode > CC_JAPAN) {
            stPrtRam.countryCode = CC_USA;
        }
        break;

    case 'S':  // Turn Script Mode ON
        {
            int script = EpsonFx80ParseNumber(1, 1) & 1;
            stPrtRam.fSuperscript = script == 0;
            stPrtRam.fSubscript   = script == 1;
        }
        break;

    case 'T':  // Turn Script Mode OFF
        stPrtRam.fSubscript =   FALSE;
        stPrtRam.fSuperscript = FALSE;
        break;

    case 'U':  // Turn Uni-directional mode ON/OFF
        stPrtRam.fLeftToRight = EpsonFx80ParseNumber(1, 1);
        break;

    case 'W':  // Turn Expanded Mode ON/OFF
        stPrtRam.fNormalAfterLine = FALSE;
        stPrtRam.fDoubleWidth = EpsonFx80ParseNumber(1, 1);
        break;

    case 'Y':  // Turn High Speed Double Density Graphics ON
        break;

    case 'Z':  // Turns Quadruple Density Graphics ON
        stPrtRam.uiGraphDensity = 4.0;
        stPrtRam.fNinePinGraphics = FALSE;
        stPrtRam.sizeRemainingDataBytes = EpsonFx80ParseNumber(1, 2);
        break;

    case '^':  // Turn Nine Pin Graphics Mode ON
        stPrtRam.uiGraphDensity = EpsonFx80ParseNumber(1, 1) ? 2.0 : 1.0;
        stPrtRam.fNinePinGraphics = TRUE;
        stPrtRam.sizeRemainingDataBytes = 2 * EpsonFx80ParseNumber(2, 2);
        break;

    case 'b':  // Set Vertical Tab
        break;

    case 'i':  // Turn Immediate Mode ON/OFF
        break;

    case 'j':  // Immediate Reverse Line Feed
        stPrtRam.uiVpos -= (EpsonFx80ParseNumber(1, 1) & 127) / 3.0;
        if (stPrtRam.uiVpos < stPrtRam.uiPageTop)
            stPrtRam.uiVpos = stPrtRam.uiPageTop;
        break;

    case 'l':  // Set Left Margin
        break;

    case 'p':  // Turn Proportional Mode ON/OFF
        stPrtRam.fProportional = EpsonFx80ParseNumber(1, 1);
        break;

    case 's':  // Set Print Speed
        break;

    case 127: // Deletes Last Character in Buffer
        break;
    }
}

// International character code translation for the Epson FX-80 printer
//                              US   FR   DE   GB   DK   SE   IT   SP   JP
static BYTE intlChar35[9]  = {  35,  35,  35,   6,  35,  35,  35,  12,  35 };
static BYTE intlChar36[9]  = {  36,  36,  36,  36,  36,  11,  36,  36,  36 };
static BYTE intlChar64[9]  = {  64,   0,  16,  64,  64,  29,  64,  64,  64 };
static BYTE intlChar91[9]  = {  91,   5,  23,  91,  18,  23,   5,   7,  91 };
static BYTE intlChar92[9]  = {  92,  15,  24,  92,  20,  24,  92,   9,  31 };
static BYTE intlChar93[9]  = {  93,  16,  25,  93,  13,  13,  30,   8,  93 };
static BYTE intlChar94[9]  = {  94,  94,  94,  94,  94,  25,  94,  94,  94 };
static BYTE intlChar96[9]  = {  96,  96,  96,  96,  96,  30,   2,  96,  96 };
static BYTE intlChar123[9] = { 123,  30,  26, 123,  19,  26,   0,  22, 123 };
static BYTE intlChar124[9] = { 124,   2,  27, 124,  21,  27,   3,  10, 124 };
static BYTE intlChar125[9] = { 125,   1,  28, 125,  14,  14,   1, 125, 125 };
static BYTE intlChar126[9] = { 126,  22,  17, 126, 126,  28,   4, 126, 126 };

static void EpsonFx80ProcessCharacter(BYTE bChar)
{
    if (bChar >= 32) {
        if (stPrtRam.fItalic) {
            bChar |= 128;
        }
        else {
            bChar &= 127;
        }
    }

    if (!stPrtRam.fNoHighEscapeCodes && bChar >= 128 && bChar < 160) {
        bChar &= 31;
    }

    // Convert international characters
    switch (bChar & 0x7f) {
        case 35:  bChar = (bChar & 0x80) | intlChar35 [stPrtRam.countryCode]; break;
        case 36:  bChar = (bChar & 0x80) | intlChar36 [stPrtRam.countryCode]; break;
        case 64:  bChar = (bChar & 0x80) | intlChar64 [stPrtRam.countryCode]; break;
        case 91:  bChar = (bChar & 0x80) | intlChar91 [stPrtRam.countryCode]; break;
        case 92:  bChar = (bChar & 0x80) | intlChar92 [stPrtRam.countryCode]; break;
        case 93:  bChar = (bChar & 0x80) | intlChar93 [stPrtRam.countryCode]; break;
        case 94:  bChar = (bChar & 0x80) | intlChar94 [stPrtRam.countryCode]; break;
        case 96:  bChar = (bChar & 0x80) | intlChar96 [stPrtRam.countryCode]; break;
        case 123: bChar = (bChar & 0x80) | intlChar123[stPrtRam.countryCode]; break;
        case 124: bChar = (bChar & 0x80) | intlChar124[stPrtRam.countryCode]; break;
        case 125: bChar = (bChar & 0x80) | intlChar125[stPrtRam.countryCode]; break;
        case 126: bChar = (bChar & 0x80) | intlChar126[stPrtRam.countryCode]; break;
    }

    if (bChar >= 32) {
        PrintVisibleCharacter(bChar);
        return;
    }

    switch (bChar) {
    case 0:  // Terminates horizontal and vertical TAB setting
        break;
    case 7:  // Sound beeper
        break;
    case 8:  // Backspace
        // ToDo: fix for other font-sizes ***
        stPrtRam.uiHpos -= 8;
        if (stPrtRam.uiHpos < stPrtRam.uiLeftBorder)
            stPrtRam.uiHpos=stPrtRam.uiLeftBorder;
        break;
    case 9:  // Horizontal TAB
        {
            // ToDo: fix for other font-sizes ***
            UINT            uiNewPos;
            uiNewPos = ((UINT)stPrtRam.uiHpos + 64 - stPrtRam.uiLeftBorder) & ~63;
            uiNewPos += stPrtRam.uiLeftBorder;

            if (uiNewPos >= stPrtRam.uiHpos)
                break;
            // else fall thru: CR/LF
        }
        break;
    case 10: // Line Feed
    case 11: // Vertical TAB
        stPrtRam.uiVpos += stPrtRam.uiLineFeed;
        if (stPrtRam.uiVpos >= stPrtRam.uiPageHeight)
            FlushEmulatedPrinter();
        break;
    case 12: // Form Feed
        EnsurePrintPage();
        FlushEmulatedPrinter();
        break;
    case 13: // Carrige return
        stPrtRam.uiHpos = stPrtRam.uiLeftBorder;
        break;
    case 14: // Turns expanded mode ON
        stPrtRam.fDoubleWidth = TRUE;
        stPrtRam.fNormalAfterLine = TRUE;
        break;
    case 15: // Shift in. Emties buffer, turns compressed mode ON (17.16 cpi)
        stPrtRam.fCompressed = TRUE;
        if (!stPrtRam.fElite) {
            stPrtRam.uiFontDensity = 1.72;
        }
        break;
    case 17: // Device Control 1: 
        break;
    case 18: // Device Control 2: turns compressed mode OFF
        stPrtRam.fCompressed = FALSE;
        stPrtRam.uiFontDensity = 1.00;
        break;
    case 19: // Device Control 3: 
        break;
    case 20: // Device Control 4: Turns expanded mode OFF
        stPrtRam.fDoubleWidth = FALSE;
        break;
    case 24: // Cancels all text in the print buffer
        break;
    case 27: // Escape
        stPrtRam.fEscSequence = TRUE;
        break;
    default:
        if (stPrtRam.fAlternateChar) {
            PrintVisibleCharacter(bChar);
        }
        break;
    }
}

////////////////////////////////////////////////////
// Generic Character processing
////////////////////////////////////////////////////

static void updateRamFont(void)
{
    switch (printerType) {
    case P_LPT_MSXPRN:
        memcpy(stPrtRam.fontInfo.rom, MSXFont, sizeof(stPrtRam.fontInfo.rom));
        stPrtRam.fontInfo.charWidth = 9;
        stPrtRam.fontInfo.pixelDelta = 1.0;
        stPrtRam.fontInfo.useRam = FALSE;
        break;
    case P_LPT_EPSONFX80:
        memcpy(stPrtRam.fontInfo.rom, EpsonFontRom, sizeof(stPrtRam.fontInfo.rom));
        stPrtRam.fontInfo.charWidth = 12;
        stPrtRam.fontInfo.pixelDelta = 0.5;
        stPrtRam.fontInfo.useRam = FALSE;
        break;
    }
}

static void ResetEmulatedPrinter(void)
{
    ZeroMemory(&stPrtRam, sizeof(stPrtRam));

    switch (printerType) {
    case P_LPT_MSXPRN:
        MsxPrnResetSettings();
        break;
    case P_LPT_EPSONFX80:
        EpsonFx80ResetSettings();
        break;
    }

    stPrtRam.uiPageHeight   = stPrtRam.uiPageTop + stPrtRam.uiLines * stPrtRam.uiLineFeed;
    stPrtRam.uiHpos         = stPrtRam.uiLeftBorder;
    stPrtRam.uiVpos         = stPrtRam.uiPageTop;

    fPrintDataOnPage = FALSE;

    if (hdcPrinter != NULL) {
        stPrtRam.uiPixelSizeX = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH) / stPrtRam.uiTotalWidth;
        stPrtRam.uiPixelSizeY = GetDeviceCaps(hdcPrinter, PHYSICALHEIGHT) / stPrtRam.uiTotalHeight;
    }
    
    updateRamFont();

    return;
}

static size_t CalcEscSequenceLength(BYTE character) 
{
    size_t rv = 0;

    switch (printerType) {
    case P_LPT_MSXPRN:
        rv = MsxPrnCalcEscSequenceLength( character );
        break;
    case P_LPT_EPSONFX80:
        rv = EpsonFx80CalcEscSequenceLength( character );
        break;
    }
    return rv;
}

static void ProcessCharacter(BYTE bChar)
{
    switch (printerType) {
    case P_LPT_MSXPRN:
        MsxPrnProcessCharacter( bChar );
        break;
    case P_LPT_EPSONFX80:
        EpsonFx80ProcessCharacter( bChar );
        break;
    }
}

static void ProcessEscSequence(void)
{
    switch (printerType) {
    case P_LPT_MSXPRN:
        MsxPrnProcessEscSequence();
        break;
    case P_LPT_EPSONFX80:
        EpsonFx80ProcessEscSequence();
        break;
    }
}

void PrintToMSX(BYTE bData)
{
    if (stPrtRam.ramLoadOffset < stPrtRam.ramLoadEnd) {
        stPrtRam.fontInfo.ram[stPrtRam.ramLoadOffset++] = bData;
    }
    else if (stPrtRam.sizeRemainingDataBytes) {
        if (stPrtRam.uiEightBit == 0) {
            bData &= 0x80;
        }
        if (stPrtRam.uiEightBit == 1) {
            bData |= 0x80;
        }
        PrintGraphicByte(bData, stPrtRam.fGraphicsHiLo);
        stPrtRam.sizeRemainingDataBytes--;
    }
    else if (stPrtRam.fEscSequence) {
        stPrtRam.fEscSequence=FALSE;

        memset( &(stPrtRam.abEscSeq), 0, sizeof(stPrtRam.abEscSeq) );
        *(stPrtRam.abEscSeq)=bData;
        stPrtRam.sizeEscPos=1;

        stPrtRam.uiRemainingCommandBytes=CalcEscSequenceLength(bData);

        if (!stPrtRam.uiRemainingCommandBytes) {
            ProcessEscSequence();
        }
    }
    else if (stPrtRam.uiRemainingCommandBytes) {
        stPrtRam.abEscSeq[stPrtRam.sizeEscPos++]=bData;

        if (!--stPrtRam.uiRemainingCommandBytes) {
            ProcessEscSequence();
        }
    }
    else {
        ProcessCharacter( bData );
    }
}

static void initPixelBitmaps(void)
{
    UINT bByte;
    
    ZeroMemory( &bmiGrph, sizeof(bmiGrph) );
    bmiGrph.bmiHeader.biSize         = sizeof(bmiGrph.bmiHeader);
    bmiGrph.bmiHeader.biWidth        = PIXEL_WIDTH * 1;
    bmiGrph.bmiHeader.biHeight       = PIXEL_WIDTH * 9;
    bmiGrph.bmiHeader.biPlanes       = 1;
    bmiGrph.bmiHeader.biBitCount     = 1;
    bmiGrph.bmiHeader.biCompression  = BI_RGB;
    bmiGrph.bmiColors[0].rgbBlue     = 0xFF;
    bmiGrph.bmiColors[0].rgbGreen    = 0xFF;
    bmiGrph.bmiColors[0].rgbRed      = 0xFF;
    bmiGrph.bmiColors[0].rgbReserved = 0x00;
    bmiGrph.bmiColors[1].rgbBlue     = 0x00;
    bmiGrph.bmiColors[1].rgbGreen    = 0x00;
    bmiGrph.bmiColors[1].rgbRed      = 0x00;
    bmiGrph.bmiColors[1].rgbReserved = 0x00;

    ZeroMemory(abGrphImage, sizeof(abGrphImage));

    for (bByte = 0; bByte < 512; bByte++) {
        int  iPixel;
        UINT bMask = 0x01;

        for (iPixel=0; iPixel<9; iPixel++) {
            if (bByte & bMask) {
#if PIXEL_WIDTH == 1
                abGrphImage[iPixel<<2] = 0x80;
#else
                static BYTE singleStrikeBitmap[] = {
                    0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c
                };
                int j;
                for (j = 0; j < PIXEL_WIDTH; j++) {
                    abGrphImage[bByte][(PIXEL_WIDTH * iPixel + j) << 2] = singleStrikeBitmap[j];
                    abGrphImage[512+bByte][(PIXEL_WIDTH * iPixel / 2 + j) << 2] |= singleStrikeBitmap[j];
                }
#endif
            }
            bMask <<= 1;
        }
    }
}

static int printerCreate(void)
{
    Properties* pProperties = propGetGlobalProperties();
    int width;
    int height;

    initPixelBitmaps();

    lstrcpy(szDocTitle, "blueMSX");

    MsxPrnSetFont(MSXFontRaw);

    hdcPrinter = CreateDC(NULL, TEXT(pProperties->ports.Lpt.name), NULL, NULL);
    if (hdcPrinter == NULL) {
        return 0;
    }

    width  = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH);
    height = GetDeviceCaps(hdcPrinter, PHYSICALHEIGHT);
    hdcMem  = CreateCompatibleDC(hdcPrinter);
    hBitmap = CreateCompatibleBitmap(hdcPrinter, width, height);
    DeleteObject(SelectObject(hdcMem, hBitmap));

    SetBkMode(hdcMem, TRANSPARENT);

    eraseBackground(hdcMem);
    stPrtRam.uiPrintAreaTop    = (UINT)-1;
    stPrtRam.uiPrintAreaBottom = 0;

    ResetEmulatedPrinter();

    return 1;
}

static void printerDestroy(void)
{
    if (hdcPrinter != NULL) {
        FlushEmulatedPrinter();
        ResetEmulatedPrinter();

        DeleteObject(SelectObject(hdcPrinter, GetStockObject(SYSTEM_FONT)));

        DeleteDC(hdcPrinter);
        DeleteDC(hdcMem);
        DeleteObject(hBitmap);

        hdcPrinter = NULL;
        hdcMem     = NULL;
    }
}

static void printerWrite(BYTE value)
{
    if (hdcPrinter != NULL) {
        PrintToMSX(value);
    }
}

/////////////////////////////////////////////////////////////////////
/// Generic Printer methods
/////////////////////////////////////////////////////////////////////

void archPrinterWrite(BYTE value)
{
    if (printerType == P_LPT_RAW) {
        printerRawWrite(value);
    }
    else {
        printerWrite(value);
    }
}

int archPrinterCreate(void)
{
    Properties* pProperties = propGetGlobalProperties();
    printerType = pProperties->ports.Lpt.emulation;

    if (printerType == P_LPT_RAW) {
        return printerRawCreate();
    }
    else {
        return printerCreate();
    }
}

void archPrinterDestroy(void)
{
    if (printerType == P_LPT_RAW) {
        printerRawDestroy();
    }
    else {
        printerDestroy();
    }
}

void archForceFormFeed(void)
{
    if (printerType == P_LPT_RAW) {
    }
    else {
        FlushEmulatedPrinter();
    }
}

void archPrinterSetMsxFont(void* romData, UInt32 size)
{
    BYTE* rom = (BYTE*)romData;

    UINT offset = rom[4] | ((UINT)(rom[5])) << 8;

    if (offset + MSX_FONT_SIZE > size) {
        return;
    }

    memcpy(MSXFontRaw, rom + offset, MSX_FONT_SIZE);

    if (printerType == P_LPT_MSXPRN) {
        MsxPrnSetFont(MSXFontRaw);
        updateRamFont();
    }
}

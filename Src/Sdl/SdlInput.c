/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Sdl/SdlInput.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2006/06/24 02:27:08 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
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
#include "ArchInput.h"


#include "Language.h"
#include "InputEvent.h"
#include "IniFileParser.h"
#include <stdio.h>
#include "SDL/SDL.h"

#define MAX_JOYSTICKS 8


#define KBD_TABLE_LEN 512

#define KBD_TABLE_NUM 3
static int kbdTable[KBD_TABLE_NUM][KBD_TABLE_LEN];
static int kbdTableBackup[KBD_TABLE_NUM][KBD_TABLE_LEN];
static int keyStatus[KBD_TABLE_NUM][KBD_TABLE_LEN];
static int selectedKey;
static int selectedDikKey;
static int selectedTable;
static int editEnabled;
static int kbdModifiers;
static UInt32 buttonState = 0;
static int hasFocus = 1;

static char keyboardConfigDir[512];

static char DefaultConfigName[] = "blueMSX Default";

static char currentConfigFile[512];



#define SDLK_JOY1_BUTTON1  (SDLK_LAST + 1)
#define SDLK_JOY1_BUTTON2  (SDLK_LAST + 2)
#define SDLK_JOY1_UP       (SDLK_LAST + 3)
#define SDLK_JOY1_DOWN     (SDLK_LAST + 4)
#define SDLK_JOY1_LEFT     (SDLK_LAST + 5)
#define SDLK_JOY1_RIGHT    (SDLK_LAST + 6)

#define SDLK_JOY2_BUTTON1  (SDLK_LAST + 7)
#define SDLK_JOY2_BUTTON2  (SDLK_LAST + 8)
#define SDLK_JOY2_UP       (SDLK_LAST + 9)
#define SDLK_JOY2_DOWN     (SDLK_LAST + 10)
#define SDLK_JOY2_LEFT     (SDLK_LAST + 11)
#define SDLK_JOY2_RIGHT    (SDLK_LAST + 12)


// initKbdTable initializes the keyboard table with default keys
static void initKbdTable()
{
    memset (kbdTable[0], 0, sizeof(kbdTable));
    memset (kbdTable[1], 0, sizeof(kbdTable));
    memset (kbdTable[2], 0, sizeof(kbdTable));

    kbdTable[0][SDLK_0          ] = EC_0;
    kbdTable[0][SDLK_1          ] = EC_1;
    kbdTable[0][SDLK_2          ] = EC_2;
    kbdTable[0][SDLK_3          ] = EC_3;
    kbdTable[0][SDLK_4          ] = EC_4;
    kbdTable[0][SDLK_5          ] = EC_5;
    kbdTable[0][SDLK_6          ] = EC_6;
    kbdTable[0][SDLK_7          ] = EC_7;
    kbdTable[0][SDLK_8          ] = EC_8;
    kbdTable[0][SDLK_9          ] = EC_9;
    
    kbdTable[0][SDLK_MINUS      ] = EC_NEG;
    kbdTable[0][SDLK_EQUALS     ] = EC_CIRCFLX;
    kbdTable[0][SDLK_BACKSLASH  ] = EC_BKSLASH;
    kbdTable[0][SDLK_LEFTBRACKET   ] = EC_AT;
    kbdTable[0][SDLK_RIGHTBRACKET  ] = EC_LBRACK;
    kbdTable[0][SDLK_SEMICOLON  ] = EC_SEMICOL;
    kbdTable[0][SDLK_ASTERISK   ] = EC_COLON;
    kbdTable[0][SDLK_CARET      ] = EC_RBRACK;
    kbdTable[0][SDLK_COMMA      ] = EC_COMMA;
    kbdTable[0][SDLK_PERIOD     ] = EC_PERIOD;
    kbdTable[0][SDLK_SLASH      ] = EC_DIV;
    kbdTable[0][SDLK_RCTRL      ] = EC_UNDSCRE;

    kbdTable[0][SDLK_a          ] = EC_A;
    kbdTable[0][SDLK_b          ] = EC_B;
    kbdTable[0][SDLK_c          ] = EC_C;
    kbdTable[0][SDLK_d          ] = EC_D;
    kbdTable[0][SDLK_e          ] = EC_E;
    kbdTable[0][SDLK_f          ] = EC_F;
    kbdTable[0][SDLK_g          ] = EC_G;
    kbdTable[0][SDLK_h          ] = EC_H;
    kbdTable[0][SDLK_i          ] = EC_I;
    kbdTable[0][SDLK_j          ] = EC_J;
    kbdTable[0][SDLK_k          ] = EC_K;
    kbdTable[0][SDLK_l          ] = EC_L;
    kbdTable[0][SDLK_m          ] = EC_M;
    kbdTable[0][SDLK_n          ] = EC_N;
    kbdTable[0][SDLK_o          ] = EC_O;
    kbdTable[0][SDLK_p          ] = EC_P;
    kbdTable[0][SDLK_q          ] = EC_Q;
    kbdTable[0][SDLK_r          ] = EC_R;
    kbdTable[0][SDLK_s          ] = EC_S;
    kbdTable[0][SDLK_t          ] = EC_T;
    kbdTable[0][SDLK_u          ] = EC_U;
    kbdTable[0][SDLK_v          ] = EC_V;
    kbdTable[0][SDLK_w          ] = EC_W;
    kbdTable[0][SDLK_x          ] = EC_X;
    kbdTable[0][SDLK_y          ] = EC_Y;
    kbdTable[0][SDLK_z          ] = EC_Z;

    kbdTable[0][SDLK_F1         ] = EC_F1;
    kbdTable[0][SDLK_F2         ] = EC_F2;
    kbdTable[0][SDLK_F3         ] = EC_F3;
    kbdTable[0][SDLK_F4         ] = EC_F4;
    kbdTable[0][SDLK_F5         ] = EC_F5;
    kbdTable[0][SDLK_ESCAPE     ] = EC_ESC;
    kbdTable[0][SDLK_TAB        ] = EC_TAB;
    kbdTable[0][SDLK_PAGEUP     ] = EC_STOP;
    kbdTable[0][SDLK_BACKSPACE  ] = EC_BKSPACE;
    kbdTable[0][SDLK_END        ] = EC_SELECT;
    kbdTable[0][SDLK_RETURN     ] = EC_RETURN;
    kbdTable[0][SDLK_SPACE      ] = EC_SPACE;
    kbdTable[0][SDLK_HOME       ] = EC_CLS;
    kbdTable[0][SDLK_INSERT     ] = EC_INS;
    kbdTable[0][SDLK_DELETE     ] = EC_DEL;
    kbdTable[0][SDLK_LEFT       ] = EC_LEFT;
    kbdTable[0][SDLK_UP         ] = EC_UP;
    kbdTable[0][SDLK_RIGHT      ] = EC_RIGHT;
    kbdTable[0][SDLK_DOWN       ] = EC_DOWN;

    kbdTable[0][SDLK_KP_MULTIPLY   ] = EC_NUMMUL;
    kbdTable[0][SDLK_KP_PLUS       ] = EC_NUMADD;
    kbdTable[0][SDLK_KP_DIVIDE     ] = EC_NUMDIV;
    kbdTable[0][SDLK_KP_MINUS      ] = EC_NUMSUB;
    kbdTable[0][SDLK_KP_PERIOD     ] = EC_NUMPER;
    kbdTable[0][SDLK_PAGEDOWN      ] = EC_NUMCOM;
    kbdTable[0][SDLK_KP0    ] = EC_NUM0;
    kbdTable[0][SDLK_KP1    ] = EC_NUM1;
    kbdTable[0][SDLK_KP2    ] = EC_NUM2;
    kbdTable[0][SDLK_KP3    ] = EC_NUM3;
    kbdTable[0][SDLK_KP4    ] = EC_NUM4;
    kbdTable[0][SDLK_KP5    ] = EC_NUM5;
    kbdTable[0][SDLK_KP6    ] = EC_NUM6;
    kbdTable[0][SDLK_KP7    ] = EC_NUM7;
    kbdTable[0][SDLK_KP8    ] = EC_NUM8;
    kbdTable[0][SDLK_KP9    ] = EC_NUM9;

    kbdTable[0][SDLK_LSUPER     ] = EC_TORIKE;
    kbdTable[0][SDLK_RSUPER     ] = EC_JIKKOU;
    kbdTable[0][SDLK_LSHIFT     ] = EC_LSHIFT;
    kbdTable[0][SDLK_RSHIFT     ] = EC_RSHIFT;
    kbdTable[0][SDLK_LCTRL      ] = EC_CTRL;
    kbdTable[0][SDLK_LALT       ] = EC_GRAPH;
    kbdTable[0][SDLK_RALT       ] = EC_CODE;
    kbdTable[0][SDLK_CAPSLOCK   ] = EC_CAPS;
    kbdTable[0][SDLK_KP_ENTER   ] = EC_PAUSE;
    kbdTable[0][SDLK_SYSREQ     ] = EC_PRINT;


    kbdTable[1][SDLK_JOY1_BUTTON1] = EC_JOY1_BUTTON1;
    kbdTable[1][SDLK_JOY1_BUTTON2] = EC_JOY1_BUTTON2;
    kbdTable[1][SDLK_JOY1_UP     ] = EC_JOY1_UP;
    kbdTable[1][SDLK_JOY1_DOWN   ] = EC_JOY1_DOWN;
    kbdTable[1][SDLK_JOY1_LEFT   ] = EC_JOY1_LEFT;
    kbdTable[1][SDLK_JOY1_RIGHT  ] = EC_JOY1_RIGHT;
    kbdTable[1][SDLK_0           ] = EC_COLECO1_0;
    kbdTable[1][SDLK_1           ] = EC_COLECO1_1;
    kbdTable[1][SDLK_2           ] = EC_COLECO1_2;
    kbdTable[1][SDLK_3           ] = EC_COLECO1_3;
    kbdTable[1][SDLK_4           ] = EC_COLECO1_4;
    kbdTable[1][SDLK_5           ] = EC_COLECO1_5;
    kbdTable[1][SDLK_6           ] = EC_COLECO1_6;
    kbdTable[1][SDLK_7           ] = EC_COLECO1_7;
    kbdTable[1][SDLK_8           ] = EC_COLECO1_8;
    kbdTable[1][SDLK_9           ] = EC_COLECO1_9;
    kbdTable[1][SDLK_MINUS       ] = EC_COLECO1_STAR;
    kbdTable[1][SDLK_EQUALS      ] = EC_COLECO1_HASH;
    

    kbdTable[2][SDLK_JOY2_BUTTON1] = EC_JOY2_BUTTON1;
    kbdTable[2][SDLK_JOY2_BUTTON2] = EC_JOY2_BUTTON2;
    kbdTable[2][SDLK_JOY2_UP     ] = EC_JOY2_UP;
    kbdTable[2][SDLK_JOY2_DOWN   ] = EC_JOY2_DOWN;
    kbdTable[2][SDLK_JOY2_LEFT   ] = EC_JOY2_LEFT;
    kbdTable[2][SDLK_JOY2_RIGHT  ] = EC_JOY2_RIGHT;
    kbdTable[2][SDLK_KP0     ] = EC_COLECO2_0;
    kbdTable[2][SDLK_KP1     ] = EC_COLECO2_1;
    kbdTable[2][SDLK_KP2     ] = EC_COLECO2_2;
    kbdTable[2][SDLK_KP3     ] = EC_COLECO2_3;
    kbdTable[2][SDLK_KP4     ] = EC_COLECO2_4;
    kbdTable[2][SDLK_KP5     ] = EC_COLECO2_5;
    kbdTable[2][SDLK_KP6     ] = EC_COLECO2_6;
    kbdTable[2][SDLK_KP7     ] = EC_COLECO2_7;
    kbdTable[2][SDLK_KP8     ] = EC_COLECO2_8;
    kbdTable[2][SDLK_KP9     ] = EC_COLECO2_9;
    kbdTable[2][SDLK_KP_MULTIPLY    ] = EC_COLECO2_STAR;
    kbdTable[2][SDLK_KP_DIVIDE      ] = EC_COLECO2_HASH;

//    keyboardSaveConfig(DefaultConfigName);
}

void keyboardSetDirectory(char* directory)
{
    strcpy(keyboardConfigDir, directory);
}

void keyboardInit()
{
    char fileName[512];
    FILE* file;

    initKbdTable();

    inputEventReset();

    kbdModifiers = 0;
    
    sprintf(fileName, "%s/%s.config", keyboardConfigDir, DefaultConfigName);
    file = fopen(fileName, "r");
    if (file == NULL) {
//        keyboardSaveConfig(DefaultConfigName);
        return;
    }
    sprintf(currentConfigFile, DefaultConfigName);
    fclose(file);
}

void keyboardSetFocus(int handle, int focus) 
{
    if (focus) {
        hasFocus |= handle;
    }
    else {
        hasFocus &= ~handle;
    }
}

static void keyboardHanldeKeypress(int code, int pressed) 
{
    int n;

    for (n = 0; n < KBD_TABLE_NUM; n++) {
        int wasPressed = keyStatus[n][code];
        int keyCode = kbdTable[n][code];
        int isEditing = selectedTable == n && editEnabled && selectedKey != 0;
        keyStatus[n][code] = pressed;

        if (pressed == wasPressed || (keyCode == 0 && !isEditing)) {
            continue;
        }

        if (pressed) {
            if (isEditing) {
                int i;
                for (i = 0; i < KBD_TABLE_LEN; i++) {
                    if (kbdTable[n][i] == selectedKey) {
                        kbdTable[n][i] = 0;
                    }
                }

                kbdTable[n][code] = selectedKey;
                selectedDikKey = code;

                inputEventUnset(keyCode);
            }
            else {
                inputEventSet(keyCode);
            }
        }
        else {
            inputEventUnset(keyCode);
        }
    }
}

static void keyboardResetKbd() 
{
    int n;
    for (n = 0; n < KBD_TABLE_NUM; n++) {
        int i;
        for (i = 0; i < KBD_TABLE_LEN; i++) {
            kbdModifiers = 0;
            keyboardHanldeKeypress(kbdTable[n][i], 0);
        }
    }
    inputEventReset();
    buttonState = 0;
}

void keyboardUpdate() 
{ 
    Uint8* keyBuf;
    int    keyNum;

    if (!hasFocus) {
        keyboardResetKbd();
        return;
    }

    keyBuf = SDL_GetKeyState(&keyNum);

    if (keyBuf == NULL) {
        return;
    }

    kbdModifiers = ((keyBuf[SDLK_LSHIFT] ? 1 : 0) << 0) | ((keyBuf[SDLK_RSHIFT] ? 1 : 0) << 1) | 
                   ((keyBuf[SDLK_LCTRL]  ? 1 : 0) << 2) | ((keyBuf[SDLK_RCTRL]  ? 1 : 0) << 3) | 
                   ((keyBuf[SDLK_LALT]   ? 1 : 0) << 4) | ((keyBuf[SDLK_RALT]   ? 1 : 0) << 5) | 
                   ((keyBuf[SDLK_LSUPER] ? 1 : 0) << 6) | ((keyBuf[SDLK_RSUPER] ? 1 : 0) << 7);

    if (kbdModifiers &&
            (keyBuf[SDLK_F6] | keyBuf[SDLK_F7]  | keyBuf[SDLK_F8]  | keyBuf[SDLK_F9]  | 
            keyBuf[SDLK_F10] | keyBuf[SDLK_F11] | keyBuf[SDLK_F12]))
    {
        int i;
        for (i = 0; i < keyNum; i++) {
            keyboardHanldeKeypress(i, 0);
        }
    }
    else {
        int i;
        for (i = 0; i < keyNum; i++) {
            keyboardHanldeKeypress(i, keyBuf[i]);
        }
    }
} 


int keyboardGetModifiers()
{
    return kbdModifiers;
}



void  archUpdateJoystick() {}

UInt8 archJoystickGetState(int joystickNo) { return 0; }
int  archJoystickGetCount() { return 0; }
char* archJoystickGetName(int index) { return ""; }
void archMouseSetForceLock(int lock) { }

void  archPollInput() 
{
    keyboardUpdate();
}
void  archKeyboardSetSelectedKey(int keyCode) {}
char* archGetSelectedKey() { return ""; }
char* archGetMappedKey() { return ""; }
int   archKeyboardIsKeyConfigured(int msxKeyCode) { return 0; }
int   archKeyboardIsKeySelected(int msxKeyCode) { return 0; }
char* archKeyconfigSelectedKeyTitle() { return ""; }
char* archKeyconfigMappedToTitle() { return ""; }
char* archKeyconfigMappingSchemeTitle() { return ""; }


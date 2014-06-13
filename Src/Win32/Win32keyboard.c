/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32keyboard.c,v $
**
** $Revision: 1.31 $
**
** $Date: 2006/06/03 17:55:54 $
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
#define DIRECTINPUT_VERSION     0x0500
#include "Win32keyboard.h"
#include "Language.h"
#include "InputEvent.h"
#include "IniFileParser.h"
#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <dinput.h>


#define MAX_JOYSTICKS 8


#define KBD_TABLE_LEN 512

#define KBD_TABLE_NUM 3
static int kbdTable[KBD_TABLE_NUM][KBD_TABLE_LEN];
static int kbdTableBackup[KBD_TABLE_NUM][KBD_TABLE_LEN];
static int keyStatus[KBD_TABLE_NUM][KBD_TABLE_LEN];
static char dikStrings[KBD_TABLE_LEN][256];
static int selectedKey;
static int selectedDikKey;
static int selectedTable;
static int editEnabled;

static char keyboardConfigDir[MAX_PATH];

static char DefaultConfigName[] = "blueMSX Default";

static char currentConfigFile[MAX_PATH];

#define KEY_CODE_BUTTON1  256
#define KEY_CODE_JOYUP    (256 + 28)
#define KEY_CODE_JOYDOWN  (256 + 29)
#define KEY_CODE_JOYLEFT  (256 + 30)
#define KEY_CODE_JOYRIGHT (256 + 31)

#define DIK_JOY1_BUTTON1  256
#define DIK_JOY1_BUTTON2  257
#define DIK_JOY1_UP       284
#define DIK_JOY1_DOWN     285
#define DIK_JOY1_LEFT     286
#define DIK_JOY1_RIGHT    287

#define DIK_JOY2_BUTTON1  288
#define DIK_JOY2_BUTTON2  289
#define DIK_JOY2_UP       316
#define DIK_JOY2_DOWN     317
#define DIK_JOY2_LEFT     318
#define DIK_JOY2_RIGHT    319

#define INIT_DIK(val) strcpy(dikStrings[DIK_##val], #val)

static void initDikStr()
{
    int i, j;

    INIT_DIK(0);
    INIT_DIK(1);
    INIT_DIK(2);
    INIT_DIK(3);
    INIT_DIK(4);
    INIT_DIK(5);
    INIT_DIK(6);
    INIT_DIK(7);
    INIT_DIK(8);
    INIT_DIK(9);
    INIT_DIK(A);
    INIT_DIK(ABNT_C1);
    INIT_DIK(ABNT_C2);
    INIT_DIK(ADD);
    INIT_DIK(APOSTROPHE);
    INIT_DIK(APPS);
    INIT_DIK(AT);
    INIT_DIK(AX);
    INIT_DIK(B);
    INIT_DIK(BACK);
    INIT_DIK(BACKSLASH);
    INIT_DIK(C);
    INIT_DIK(CALCULATOR);
    INIT_DIK(CAPITAL);
    INIT_DIK(COLON);
    INIT_DIK(COMMA);
    INIT_DIK(CONVERT);
    INIT_DIK(D);
    INIT_DIK(DECIMAL);
    INIT_DIK(DELETE);
    INIT_DIK(DIVIDE);
    INIT_DIK(DOWN);
    INIT_DIK(E);
    INIT_DIK(END);
    INIT_DIK(EQUALS);
    INIT_DIK(ESCAPE);
    INIT_DIK(F);
    INIT_DIK(F1);
    INIT_DIK(F2);
    INIT_DIK(F3);
    INIT_DIK(F4);
    INIT_DIK(F5);
    INIT_DIK(F6);
    INIT_DIK(F7);
    INIT_DIK(F8);
    INIT_DIK(F9);
    INIT_DIK(F10);
    INIT_DIK(F11);
    INIT_DIK(F12);
    INIT_DIK(F13);
    INIT_DIK(F14);
    INIT_DIK(F15);
    INIT_DIK(G);
    INIT_DIK(GRAVE);
    INIT_DIK(H);
    INIT_DIK(HOME);
    INIT_DIK(I);
    INIT_DIK(INSERT);
    INIT_DIK(J);
    INIT_DIK(K);
    INIT_DIK(KANA);
    INIT_DIK(KANJI);
    INIT_DIK(L);
    INIT_DIK(LBRACKET);
    INIT_DIK(LCONTROL);
    INIT_DIK(LEFT);
    INIT_DIK(LMENU);
    INIT_DIK(LSHIFT);
    INIT_DIK(LWIN);
    INIT_DIK(M);
    INIT_DIK(MAIL);
    INIT_DIK(MEDIASELECT);
    INIT_DIK(MEDIASTOP);
    INIT_DIK(MINUS);
    INIT_DIK(MULTIPLY);
    INIT_DIK(MUTE);
    INIT_DIK(MYCOMPUTER);
    INIT_DIK(N);
    INIT_DIK(NEXT);
    INIT_DIK(NEXTTRACK);
    INIT_DIK(NOCONVERT);
    INIT_DIK(NUMLOCK);
    INIT_DIK(NUMPAD0);
    INIT_DIK(NUMPAD1);
    INIT_DIK(NUMPAD2);
    INIT_DIK(NUMPAD3);
    INIT_DIK(NUMPAD4);
    INIT_DIK(NUMPAD5);
    INIT_DIK(NUMPAD6);
    INIT_DIK(NUMPAD7);
    INIT_DIK(NUMPAD8);
    INIT_DIK(NUMPAD9);
    INIT_DIK(NUMPADCOMMA);
    INIT_DIK(NUMPADENTER);
    INIT_DIK(NUMPADEQUALS);
    INIT_DIK(O);
    INIT_DIK(OEM_102);
    INIT_DIK(P);
    INIT_DIK(PAUSE);
    INIT_DIK(PERIOD);
    INIT_DIK(PLAYPAUSE);
    INIT_DIK(POWER);
    INIT_DIK(PREVTRACK);
    INIT_DIK(PRIOR);
    INIT_DIK(Q);
    INIT_DIK(R);
    INIT_DIK(RBRACKET);
    INIT_DIK(RCONTROL);
    INIT_DIK(RETURN);
    INIT_DIK(RIGHT);
    INIT_DIK(RMENU);
    INIT_DIK(RSHIFT);
    INIT_DIK(RWIN);
    INIT_DIK(S);
    INIT_DIK(SCROLL);
    INIT_DIK(SEMICOLON);
    INIT_DIK(SLASH);
    INIT_DIK(SLEEP);
    INIT_DIK(SPACE);
    INIT_DIK(STOP);
    INIT_DIK(SUBTRACT);
    INIT_DIK(SYSRQ);
    INIT_DIK(T);
    INIT_DIK(TAB);
    INIT_DIK(U);
    INIT_DIK(UNDERLINE);
    INIT_DIK(UNLABELED);
    INIT_DIK(UP);
    INIT_DIK(V);
    INIT_DIK(VOLUMEDOWN);
    INIT_DIK(VOLUMEUP);
    INIT_DIK(W);
    INIT_DIK(WAKE);
    INIT_DIK(WEBBACK);
    INIT_DIK(WEBFAVORITES);
    INIT_DIK(WEBFORWARD);
    INIT_DIK(WEBHOME);
    INIT_DIK(WEBREFRESH);
    INIT_DIK(WEBSEARCH);
    INIT_DIK(WEBSTOP);
    INIT_DIK(X);
    INIT_DIK(Y);
    INIT_DIK(YEN);
    INIT_DIK(Z);

    for (j = 0; j < MAX_JOYSTICKS; j++) {
        for (i = 0; i < 28; i++) {
            sprintf(dikStrings[KEY_CODE_BUTTON1 + i + 32 * j], "J%d BT %d", j + 1, i + 1);
        }

        sprintf(dikStrings[KEY_CODE_JOYLEFT  + 32 * j], "J%d LEFT", j + 1);
        sprintf(dikStrings[KEY_CODE_JOYRIGHT + 32 * j], "J%d RIGHT", j + 1);
        sprintf(dikStrings[KEY_CODE_JOYUP    + 32 * j], "J%d UP", j + 1);
        sprintf(dikStrings[KEY_CODE_JOYDOWN  + 32 * j], "J%d DOWN", j + 1);
    }
}

char* dik2str(int dikKey) 
{
    if (dikKey < 0 || dikKey >= KBD_TABLE_LEN) {
        return "";
    }
    return dikStrings[dikKey];
}

int str2dik(char* dikString) 
{
    int i;
    for (i = 0; i < KBD_TABLE_LEN; i++) {
        if (0 == strcmp(dikString, dikStrings[i])) {
            return i;
        }
    }
    return 0;
}

// initKbdTable initializes the keyboard table with default keys
static void initKbdTable()
{
    memset (kbdTable[0], 0, sizeof(kbdTable));
    memset (kbdTable[1], 0, sizeof(kbdTable));
    memset (kbdTable[2], 0, sizeof(kbdTable));

    kbdTable[0][DIK_0          ] = EC_0;
    kbdTable[0][DIK_1          ] = EC_1;
    kbdTable[0][DIK_2          ] = EC_2;
    kbdTable[0][DIK_3          ] = EC_3;
    kbdTable[0][DIK_4          ] = EC_4;
    kbdTable[0][DIK_5          ] = EC_5;
    kbdTable[0][DIK_6          ] = EC_6;
    kbdTable[0][DIK_7          ] = EC_7;
    kbdTable[0][DIK_8          ] = EC_8;
    kbdTable[0][DIK_9          ] = EC_9;
    
    kbdTable[0][DIK_MINUS      ] = EC_NEG;
    kbdTable[0][DIK_EQUALS     ] = EC_CIRCFLX;
    kbdTable[0][DIK_BACKSLASH  ] = EC_BKSLASH;
    kbdTable[0][DIK_LBRACKET   ] = EC_AT;
    kbdTable[0][DIK_RBRACKET   ] = EC_LBRACK;
    kbdTable[0][DIK_SEMICOLON  ] = EC_SEMICOL;
    kbdTable[0][DIK_APOSTROPHE ] = EC_COLON;
    kbdTable[0][DIK_GRAVE      ] = EC_RBRACK;
    kbdTable[0][DIK_COMMA      ] = EC_COMMA;
    kbdTable[0][DIK_PERIOD     ] = EC_PERIOD;
    kbdTable[0][DIK_SLASH      ] = EC_DIV;
    kbdTable[0][DIK_RCONTROL   ] = EC_UNDSCRE;
    
    kbdTable[0][DIK_A          ] = EC_A;
    kbdTable[0][DIK_B          ] = EC_B;
    kbdTable[0][DIK_C          ] = EC_C;
    kbdTable[0][DIK_D          ] = EC_D;
    kbdTable[0][DIK_E          ] = EC_E;
    kbdTable[0][DIK_F          ] = EC_F;
    kbdTable[0][DIK_G          ] = EC_G;
    kbdTable[0][DIK_H          ] = EC_H;
    kbdTable[0][DIK_I          ] = EC_I;
    kbdTable[0][DIK_J          ] = EC_J;
    kbdTable[0][DIK_K          ] = EC_K;
    kbdTable[0][DIK_L          ] = EC_L;
    kbdTable[0][DIK_M          ] = EC_M;
    kbdTable[0][DIK_N          ] = EC_N;
    kbdTable[0][DIK_O          ] = EC_O;
    kbdTable[0][DIK_P          ] = EC_P;
    kbdTable[0][DIK_Q          ] = EC_Q;
    kbdTable[0][DIK_R          ] = EC_R;
    kbdTable[0][DIK_S          ] = EC_S;
    kbdTable[0][DIK_T          ] = EC_T;
    kbdTable[0][DIK_U          ] = EC_U;
    kbdTable[0][DIK_V          ] = EC_V;
    kbdTable[0][DIK_W          ] = EC_W;
    kbdTable[0][DIK_X          ] = EC_X;
    kbdTable[0][DIK_Y          ] = EC_Y;
    kbdTable[0][DIK_Z          ] = EC_Z;

    kbdTable[0][DIK_F1         ] = EC_F1;
    kbdTable[0][DIK_F2         ] = EC_F2;
    kbdTable[0][DIK_F3         ] = EC_F3;
    kbdTable[0][DIK_F4         ] = EC_F4;
    kbdTable[0][DIK_F5         ] = EC_F5;
    kbdTable[0][DIK_ESCAPE     ] = EC_ESC;
    kbdTable[0][DIK_TAB        ] = EC_TAB;
    kbdTable[0][DIK_PRIOR      ] = EC_STOP;
    kbdTable[0][DIK_BACK       ] = EC_BKSPACE;
    kbdTable[0][DIK_END        ] = EC_SELECT;
    kbdTable[0][DIK_RETURN     ] = EC_RETURN;
    kbdTable[0][DIK_SPACE      ] = EC_SPACE;
    kbdTable[0][DIK_HOME       ] = EC_CLS;
    kbdTable[0][DIK_INSERT     ] = EC_INS;
    kbdTable[0][DIK_DELETE     ] = EC_DEL;
    kbdTable[0][DIK_LEFT       ] = EC_LEFT;
    kbdTable[0][DIK_UP         ] = EC_UP;
    kbdTable[0][DIK_RIGHT      ] = EC_RIGHT;
    kbdTable[0][DIK_DOWN       ] = EC_DOWN;

    kbdTable[0][DIK_MULTIPLY   ] = EC_NUMMUL;
    kbdTable[0][DIK_ADD        ] = EC_NUMADD;
    kbdTable[0][DIK_DIVIDE     ] = EC_NUMDIV;
    kbdTable[0][DIK_SUBTRACT   ] = EC_NUMSUB;
    kbdTable[0][DIK_DECIMAL    ] = EC_NUMPER;
    kbdTable[0][DIK_NEXT       ] = EC_NUMCOM;
    kbdTable[0][DIK_NUMPAD0    ] = EC_NUM0;
    kbdTable[0][DIK_NUMPAD1    ] = EC_NUM1;
    kbdTable[0][DIK_NUMPAD2    ] = EC_NUM2;
    kbdTable[0][DIK_NUMPAD3    ] = EC_NUM3;
    kbdTable[0][DIK_NUMPAD4    ] = EC_NUM4;
    kbdTable[0][DIK_NUMPAD5    ] = EC_NUM5;
    kbdTable[0][DIK_NUMPAD6    ] = EC_NUM6;
    kbdTable[0][DIK_NUMPAD7    ] = EC_NUM7;
    kbdTable[0][DIK_NUMPAD8    ] = EC_NUM8;
    kbdTable[0][DIK_NUMPAD9    ] = EC_NUM9;

    kbdTable[0][DIK_LWIN       ] = EC_TORIKE;
    kbdTable[0][DIK_RWIN       ] = EC_JIKKOU;
    kbdTable[0][DIK_LSHIFT     ] = EC_LSHIFT;
    kbdTable[0][DIK_RSHIFT     ] = EC_RSHIFT;
    kbdTable[0][DIK_LCONTROL   ] = EC_CTRL;
    kbdTable[0][DIK_LMENU      ] = EC_GRAPH;
    kbdTable[0][DIK_RMENU      ] = EC_CODE;
    kbdTable[0][DIK_CAPITAL    ] = EC_CAPS;
    kbdTable[0][DIK_NUMPADENTER] = EC_PAUSE;
    kbdTable[0][DIK_SYSRQ      ] = EC_PRINT;


    kbdTable[1][DIK_JOY1_BUTTON1] = EC_JOY1_BUTTON1;
    kbdTable[1][DIK_JOY1_BUTTON2] = EC_JOY1_BUTTON2;
    kbdTable[1][DIK_JOY1_UP     ] = EC_JOY1_UP;
    kbdTable[1][DIK_JOY1_DOWN   ] = EC_JOY1_DOWN;
    kbdTable[1][DIK_JOY1_LEFT   ] = EC_JOY1_LEFT;
    kbdTable[1][DIK_JOY1_RIGHT  ] = EC_JOY1_RIGHT;
    kbdTable[1][DIK_0           ] = EC_COLECO1_0;
    kbdTable[1][DIK_1           ] = EC_COLECO1_1;
    kbdTable[1][DIK_2           ] = EC_COLECO1_2;
    kbdTable[1][DIK_3           ] = EC_COLECO1_3;
    kbdTable[1][DIK_4           ] = EC_COLECO1_4;
    kbdTable[1][DIK_5           ] = EC_COLECO1_5;
    kbdTable[1][DIK_6           ] = EC_COLECO1_6;
    kbdTable[1][DIK_7           ] = EC_COLECO1_7;
    kbdTable[1][DIK_8           ] = EC_COLECO1_8;
    kbdTable[1][DIK_9           ] = EC_COLECO1_9;
    kbdTable[1][DIK_MINUS       ] = EC_COLECO1_STAR;
    kbdTable[1][DIK_EQUALS      ] = EC_COLECO1_HASH;
    

    kbdTable[2][DIK_JOY2_BUTTON1] = EC_JOY2_BUTTON1;
    kbdTable[2][DIK_JOY2_BUTTON2] = EC_JOY2_BUTTON2;
    kbdTable[2][DIK_JOY2_UP     ] = EC_JOY2_UP;
    kbdTable[2][DIK_JOY2_DOWN   ] = EC_JOY2_DOWN;
    kbdTable[2][DIK_JOY2_LEFT   ] = EC_JOY2_LEFT;
    kbdTable[2][DIK_JOY2_RIGHT  ] = EC_JOY2_RIGHT;
    kbdTable[2][DIK_NUMPAD0     ] = EC_COLECO2_0;
    kbdTable[2][DIK_NUMPAD1     ] = EC_COLECO2_1;
    kbdTable[2][DIK_NUMPAD2     ] = EC_COLECO2_2;
    kbdTable[2][DIK_NUMPAD3     ] = EC_COLECO2_3;
    kbdTable[2][DIK_NUMPAD4     ] = EC_COLECO2_4;
    kbdTable[2][DIK_NUMPAD5     ] = EC_COLECO2_5;
    kbdTable[2][DIK_NUMPAD6     ] = EC_COLECO2_6;
    kbdTable[2][DIK_NUMPAD7     ] = EC_COLECO2_7;
    kbdTable[2][DIK_NUMPAD8     ] = EC_COLECO2_8;
    kbdTable[2][DIK_NUMPAD9     ] = EC_COLECO2_9;
    kbdTable[2][DIK_MULTIPLY    ] = EC_COLECO2_STAR;
    kbdTable[2][DIK_DIVIDE      ] = EC_COLECO2_HASH;

//    keyboardSaveConfig(DefaultConfigName);
}





static LPDIRECTINPUT        dinput;
static int                  dinputVersion;
static LPDIRECTINPUTDEVICE  kbdDevice = NULL;
static LPDIRECTINPUTDEVICE2 kbdDevice2 = NULL;
static HWND                 dinputWindow;
static int                  kbdModifiers;
struct JoyInfo {
    LPDIRECTINPUTDEVICE  diDevice;
    LPDIRECTINPUTDEVICE2 diDevice2;
    int                  numButtons;
    int                  buttonA;
    int                  buttonB;
};
static struct JoyInfo joyInfo[MAX_JOYSTICKS];

static int joyCount;



#define STRUCTSIZE(x) ((dinputVersion == 0x0300) ? sizeof(x##_DX3) : sizeof(x))

static int foundInputDevices = 0;
static int tryBackground = 1;
static int useBackgroundInput = 0;

static BOOL CALLBACK enumKeyboards(LPCDIDEVICEINSTANCE devInst, LPVOID ref)
{
    DIDEVCAPS kbdCaps;
    HRESULT rv;

    if (kbdDevice != NULL) {
        return DIENUM_CONTINUE;
    }

    rv = IDirectInput_CreateDevice(dinput, &devInst->guidInstance, &kbdDevice, NULL);
    if (rv != DI_OK) {
        return DIENUM_CONTINUE;
    }

    IDirectInputDevice_QueryInterface(kbdDevice, &IID_IDirectInputDevice2, (void **)&kbdDevice2);

    kbdCaps.dwSize = STRUCTSIZE(DIDEVCAPS);
    rv = IDirectInputDevice_GetCapabilities(kbdDevice, &kbdCaps);
    if (rv == DI_OK) {
        rv = IDirectInputDevice_SetDataFormat(kbdDevice, &c_dfDIKeyboard);
        if (rv == DI_OK) {
            rv = IDirectInputDevice_SetCooperativeLevel(kbdDevice, dinputWindow,
                (tryBackground ? DISCL_BACKGROUND : DISCL_FOREGROUND) | DISCL_NONEXCLUSIVE);
            if (rv != DI_OK) {
                if (kbdDevice2 != NULL) {
                    IDirectInputDevice_Release(kbdDevice2);
                }
                IDirectInputDevice_Release(kbdDevice);
                kbdDevice = NULL;
                kbdDevice2 = NULL;
            }
        }
    }

    foundInputDevices = 1;

    return DIENUM_CONTINUE;
}

static BOOL CALLBACK enumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext)
{
    DIPROPRANGE diprg;
    HRESULT rv;

    diprg.diph.dwSize       = sizeof(DIPROPRANGE);
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprg.diph.dwHow        = DIPH_BYOFFSET;
    diprg.diph.dwObj        = pdidoi->dwOfs;
    diprg.lMin              = -100;
    diprg.lMax              = +100;
    
    rv = IDirectInputDevice_SetProperty(joyInfo[joyCount].diDevice, DIPROP_RANGE, &diprg.diph);
    
    if (rv != DI_OK) {
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

static BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
    DIDEVCAPS diDevCaps;
    HRESULT rv;
    int i;

    rv = IDirectInput_CreateDevice(dinput, &pdidInstance->guidInstance, &joyInfo[joyCount].diDevice, NULL);
    if (rv != DI_OK) {
        return DIENUM_CONTINUE;
    }
  
    joyInfo[joyCount].diDevice2 = NULL;
    rv = IDirectInputDevice_QueryInterface(joyInfo[joyCount].diDevice, &IID_IDirectInputDevice2, (void **)&joyInfo[joyCount].diDevice2);

	rv = IDirectInputDevice_SetDataFormat(joyInfo[joyCount].diDevice, &c_dfDIJoystick);
    if (rv != DI_OK) {
        IDirectInputDevice_Release(joyInfo[joyCount].diDevice);
        return DIENUM_CONTINUE;
    }
    
    rv = IDirectInputDevice_SetCooperativeLevel(joyInfo[joyCount].diDevice, dinputWindow, 
            DISCL_NONEXCLUSIVE | (tryBackground ? DISCL_BACKGROUND : DISCL_FOREGROUND));
    if (rv != DI_OK) {
        IDirectInputDevice_Release(joyInfo[joyCount].diDevice);
        return DIENUM_CONTINUE;
    }

	rv = IDirectInputDevice_EnumObjects(joyInfo[joyCount].diDevice, enumAxesCallback, (void*)dinputWindow, DIDFT_AXIS);
    if (rv != DI_OK) {
        IDirectInputDevice_Release(joyInfo[joyCount].diDevice);
        return DIENUM_CONTINUE;
    }

    diDevCaps.dwSize = sizeof(diDevCaps);
    rv = IDirectInputDevice_GetCapabilities(joyInfo[joyCount].diDevice, &diDevCaps);
    if (rv != DI_OK) {
        IDirectInputDevice_Release(joyInfo[joyCount].diDevice);
        return DIENUM_CONTINUE;
    }
    joyInfo[joyCount].numButtons = diDevCaps.dwButtons;

    sprintf(dikStrings[KEY_CODE_JOYLEFT  + 32 * joyCount], "%s : left",  pdidInstance->tszInstanceName);
    sprintf(dikStrings[KEY_CODE_JOYRIGHT + 32 * joyCount], "%s : right", pdidInstance->tszInstanceName);
    sprintf(dikStrings[KEY_CODE_JOYUP    + 32 * joyCount], "%s : up",    pdidInstance->tszInstanceName);
    sprintf(dikStrings[KEY_CODE_JOYDOWN  + 32 * joyCount], "%s : down",  pdidInstance->tszInstanceName);

    for (i = diDevCaps.dwButtons - 1; i >= 0; i--) {
        sprintf(dikStrings[KEY_CODE_BUTTON1 + i + 32 * joyCount], "%s : button %d",  pdidInstance->tszInstanceName, i + 1);
    }

    foundInputDevices = 1;

    if (++joyCount == MAX_JOYSTICKS) {
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

int inputReset(HWND hwnd)
{
    DIPROPDWORD dipdw = { { sizeof(DIPROPDWORD), sizeof(DIPROPHEADER), 0, DIPH_DEVICE }, 256 };
    HRESULT rv   = 234;
    int i;

    if (useBackgroundInput) {
        return 1;
    }

    joyCount     = 0;
    kbdModifiers = 0;

    foundInputDevices = 0;
    tryBackground = 1;

    for (i = 0; i < 2; i++) {
        dinputWindow = hwnd;
        dinputVersion = DIRECTINPUT_VERSION;
	    rv = DirectInputCreate(GetModuleHandle(NULL), dinputVersion, &dinput, NULL);
        if (rv != DI_OK) {
            dinputVersion = 0x0300;
    	    rv = DirectInputCreate(GetModuleHandle(NULL), dinputVersion, &dinput, NULL);
            if (rv != DI_OK) {
                printf("Failed to initialize DirectInput\n");
                return 0;
            }
        }

	    rv = IDirectInput_EnumDevices(dinput, DIDEVTYPE_KEYBOARD, enumKeyboards, 0, DIEDFL_ATTACHEDONLY);
        if (rv != DI_OK) {
            IDirectInput_Release(dinput);
            printf("Failed to find DirectInput device\n");
            return 0;
        }

        if (kbdDevice == NULL) {
            printf("Failed to create DirectInput device\n");
            return 0;
        }

	    rv = IDirectInputDevice_SetProperty(kbdDevice, DIPROP_BUFFERSIZE,&dipdw.diph);

        rv = IDirectInput_EnumDevices(dinput, DIDEVTYPE_JOYSTICK, enumJoysticksCallback, 0, DIEDFL_ATTACHEDONLY);

        if (foundInputDevices) {
            // We found input devices that supports background input so lets not
            // do anymore tests in the future
            useBackgroundInput = 1;
            break;
        }
        tryBackground = 0;
    }

    return 1;
}

void inputDestroy(void)
{
    if (kbdDevice) {
        IDirectInputDevice_Release(kbdDevice);
        kbdDevice = NULL;
    }
    
    if (kbdDevice2) {
        IDirectInputDevice_Release(kbdDevice2);
        kbdDevice2 = NULL;
    }

    while (--joyCount >= 0) {
        if (joyInfo[joyCount].diDevice) {
    	    IDirectInputDevice_Unacquire(joyInfo[joyCount].diDevice);
            IDirectInputDevice_Release(joyInfo[joyCount].diDevice);
        }
        joyInfo[joyCount].diDevice = NULL;
        if (joyInfo[joyCount].diDevice2) {
            IDirectInputDevice_Release(joyInfo[joyCount].diDevice2);
            joyInfo[joyCount].diDevice2 = NULL;
        }
    }

    if (dinput) {
        IDirectInput_Release(dinput);
    }

    dinput = NULL;
}

static int joystickUpdateState(int index,  DWORD* buttonMask) {
    DIJOYSTATE js;
    HRESULT rv;
    int state = 0;
    DWORD bMask = 0;
    int i;

    *buttonMask = 0;
    if (index >= joyCount) {
        return 0;
    }

    if (joyInfo[index].diDevice2) {
        IDirectInputDevice2_Poll(joyInfo[index].diDevice2);
    }

    rv = IDirectInputDevice_GetDeviceState(joyInfo[index].diDevice, sizeof(DIJOYSTATE), &js);
    if (rv == DIERR_INPUTLOST || rv == DIERR_NOTACQUIRED) {
    	rv = IDirectInputDevice_Acquire(joyInfo[index].diDevice);
    }

    if (rv != DI_OK) {
        return 0;
    }

    if (js.lX < -50) state |= 0x04; 
    if (js.lX >  50) state |= 0x08;
    if (js.lY < -50) state |= 0x01; 
    if (js.lY >  50) state |= 0x02; 
    if (js.rgbButtons[joyInfo[index].buttonA]) state |= 0x10;
    if (js.rgbButtons[joyInfo[index].buttonB]) state |= 0x20;

    for (i = 0; i < joyInfo[index].numButtons; i++) {
        if (js.rgbButtons[i]) bMask |= 1 << i;
    }

    *buttonMask = bMask;

    return state;
}

int joystickNumButtons(int index) {
    return joyInfo[index].numButtons;
}

void joystickSetButtons(int index, int buttonA, int buttonB)
{
    joyInfo[index].buttonA = buttonA;
    joyInfo[index].buttonB = buttonB;
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

static DWORD buttonState = 0;
static int hasFocus = 0;

void keyboardSetFocus(int handle, int focus) 
{
    if (focus) {
        hasFocus |= handle;
    }
    else {
        hasFocus &= ~handle;
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

DWORD joystickGetButtonState()
{
    return buttonState;
}

void joystickUpdate()
{
    int i, j;
    DWORD joyMask = 0;

    buttonState = 0;

    for (i = 0; i < joyCount; i++) {
        DWORD mask;
        int state = joystickUpdateState(i, &mask);
        joyMask |= state;
        buttonState |= mask;
        
        for (j = 0; j < 28; j++) {
            keyboardHanldeKeypress(KEY_CODE_BUTTON1 + j + i * 32, mask & 1);
            mask >>= 1;
        }
        
        for (j = 0; j < 4; j++) {
            keyboardHanldeKeypress(KEY_CODE_JOYUP + j + i * 32, state & 1);
            state >>= 1;
        }
    }
}

void keyboardEnable(int enable)
{
    int n;
    for (n = 0; n < KBD_TABLE_NUM; n++) {
        int i;
        for (i = 0; i < KBD_TABLE_LEN; i++) {
            keyboardHanldeKeypress(kbdTable[n][i], 0);
        }
    }

    if (kbdDevice != NULL) {
        IDirectInputDevice_Unacquire(kbdDevice);
        if (enable) {
            IDirectInputDevice_Acquire(kbdDevice);
        }
    }
}

int keyboardGetModifiers()
{
    return kbdModifiers;
}

void keyboardUpdate() 
{ 
    if (!hasFocus) {
        keyboardResetKbd();
        return;
    }

    if (kbdDevice != NULL) {
        char buffer[256]; 
        HRESULT  rv; 
        
        if (kbdDevice2) {
			IDirectInputDevice2_Poll(kbdDevice2);
        }

        rv = IDirectInputDevice_GetDeviceState(kbdDevice, sizeof(buffer), (LPVOID)&buffer); 
        if (rv == DIERR_INPUTLOST || rv == DIERR_NOTACQUIRED) {
            rv = IDirectInputDevice_Acquire(kbdDevice);
            if (rv == DI_OK) {
                rv = IDirectInputDevice_GetDeviceState(kbdDevice, sizeof(buffer), (LPVOID)&buffer); 
            }
        }

        if (rv >= 0) { 
            kbdModifiers = ((buffer[DIK_LSHIFT]   & 0x80) >> 7) | ((buffer[DIK_RSHIFT]   & 0x80) >> 6) | 
                           ((buffer[DIK_LCONTROL] & 0x80) >> 5) | ((buffer[DIK_RCONTROL] & 0x80) >> 4) | 
                           ((buffer[DIK_LALT]     & 0x80) >> 3) | ((buffer[DIK_RALT]     & 0x80) >> 2) | 
                           ((buffer[DIK_LWIN]     & 0x80) >> 1) | ((buffer[DIK_RWIN]     & 0x80) >> 0);

            if (kbdModifiers &&
                  ((buffer[DIK_F6]  | buffer[DIK_F7]  | buffer[DIK_F8]  | buffer[DIK_F9]  | 
                    buffer[DIK_F10] | buffer[DIK_F11] | buffer[DIK_F12]) >> 7))
            {
                int i;
                for (i = 0; i < 256; i++) {
                    keyboardHanldeKeypress(i, 0);
                }
            }
            else {
                int i;
                for (i = 0; i < 256; i++) {
                    keyboardHanldeKeypress(i, buffer[i] >> 7);
                }
            }

            return;
        }
    }

    kbdModifiers = (GetAsyncKeyState(VK_LMENU)   > 1UL ? KBD_LALT     : 0) |
                   (GetAsyncKeyState(VK_MENU)    > 1UL ? KBD_LALT     : 0) |
                   (GetAsyncKeyState(VK_SHIFT)   > 1UL ? KBD_LSHIFT   : 0) |
                   (GetAsyncKeyState(VK_CONTROL) > 1UL ? KBD_LCTRL    : 0) |
                   (GetAsyncKeyState(VK_LWIN)    > 1UL ? KBD_LWIN     : 0) |
                   (GetAsyncKeyState(VK_RWIN)    > 1UL ? KBD_RWIN     : 0);
} 

char** keyboardGetConfigs()
{
    static char* keyboardNames[256];
    char         fileName[MAX_PATH];
    static char  keyboardArray[256][64];
	HANDLE       handle;
	WIN32_FIND_DATA wfd;
    int index = 0;
    BOOL cont = TRUE;

    sprintf(fileName, "%s/*.config", keyboardConfigDir);

    handle = FindFirstFile(fileName, &wfd);

    if (handle == INVALID_HANDLE_VALUE) {
        keyboardNames[0] = NULL;
        return keyboardNames;
    }

    while (cont) {
		DWORD fa = GetFileAttributes(wfd.cFileName);
        if (fa & FILE_ATTRIBUTE_DIRECTORY) {
            char buffer[128];
            int length = strlen(wfd.cFileName) - 7;
            strcpy(buffer, wfd.cFileName);
            buffer[length] = 0;
            strcpy(keyboardArray[index], buffer);
            keyboardNames[index] = keyboardArray[index];
            index++;
        }
        cont = FindNextFile(handle, &wfd);
    }

	FindClose(handle);
    
    keyboardNames[index] = NULL;

    return keyboardNames;
}

int keyboardLoadConfig(char* configName)
{
    char fileName[MAX_PATH];
    FILE* file;
    int i;
    int n;

    keyboardResetKbd();

    if (configName[0] == 0) {
        sprintf(fileName, "%s/%s.config", keyboardConfigDir, DefaultConfigName);
    }
    else {
        sprintf(fileName, "%s/%s.config", keyboardConfigDir, configName);
    }

    file = fopen(fileName, "r");
    if (file == NULL) {
        if (kbdTable[0][DIK_NUMPADENTER] == 0) {
            kbdTable[0][DIK_NUMPADENTER] = kbdTable[0][DIK_RETURN];
        }
        return 0;
    }
    fclose(file);

    sprintf(currentConfigFile, *configName ? configName : DefaultConfigName);

    iniFileOpen(fileName);

    for (n = 0; n < KBD_TABLE_NUM; n++) {
        char profString[32];
        sprintf(profString, "Keymapping-%d", n);
        for (i = 0; i < EC_KEYCOUNT; i++) {
            const char* keyCode = inputEventCodeToString(i);
            if (keyCode != NULL && *keyCode != 0) {
                char dikName[256];
                int dikKey;
                char key[32] = { 0 };
                strcat(key, keyCode);
                strcat(key, " ");
                iniFileGetString(profString, key, "", dikName, sizeof(dikName));
//                GetPrivateProfileString(profString, keyCode, "", dikName, sizeof(dikName), fileName);
                dikKey = str2dik(dikName);
                if (dikKey > 0) {
                    int j;
                    for (j = 0; j < KBD_TABLE_LEN; j++) {
                        if (kbdTable[n][j] == i) {
                            kbdTable[n][j] = 0;
                        }
                    }
                    kbdTable[n][dikKey] = i;
                }
            }
        }
    }
    iniFileClose();

    if (kbdTable[0][DIK_NUMPADENTER] == 0) {
        kbdTable[0][DIK_NUMPADENTER] = kbdTable[0][DIK_RETURN];
    }

    return 1;
}

void keyboardSaveConfig(char* configName)
{
    char fileName[MAX_PATH];
    int i, n;
    
    if (configName[0] == 0) {
        return;
    }

    sprintf(fileName, "%s/%s.config", keyboardConfigDir, configName);
    
    iniFileOpen(fileName);
    for (n = 0; n < KBD_TABLE_NUM; n++) {
        char profString[32];
        sprintf(profString, "Keymapping-%d", n);
        for (i = 0; i < EC_KEYCOUNT; i++) {
            const char* keyCode = inputEventCodeToString(i);
            const char* dikName = "";
            int j;
            for (j = 0; j < KBD_TABLE_LEN; j++) {
                if (kbdTable[n][j] == i) {
                    dikName = dik2str(j);
                    break;
                }
            }
            if (keyCode != NULL) {
                char key[32] = { 0 };
                strcat(key, keyCode);
                strcat(key, " ");
                iniFileWriteString(profString, key, (char*)dikName);
//            WritePrivateProfileString(profString, keyCode, dikName, fileName);
            }
        }
    
        memcpy(kbdTableBackup[n], kbdTable[n], sizeof(kbdTableBackup[n]));
    }
    sprintf(currentConfigFile, configName);
    iniFileClose();
}

void keyboardSetDirectory(char* directory)
{
    strcpy(keyboardConfigDir, directory);
}

char* keyboardGetCurrentConfig()
{
    return currentConfigFile;
}

int keyboardIsCurrentConfigDefault() 
{
    return strcmp(currentConfigFile, DefaultConfigName) == 0;
}

void inputInit()
{
    char fileName[MAX_PATH];
    FILE* file;

    initDikStr();
    initKbdTable();

    inputEventReset();
    
    sprintf(fileName, "%s/%s.config", keyboardConfigDir, DefaultConfigName);
    file = fopen(fileName, "r");
    if (file == NULL) {
        keyboardSaveConfig(DefaultConfigName);
        return;
    }
    sprintf(currentConfigFile, DefaultConfigName);
    fclose(file);
}

char* archGetSelectedKey()
{
    if (selectedKey != 0) {
        char* keyCode = (char*)inputEventCodeToString(selectedKey);
        if (keyCode != NULL) {
            return keyCode;
        }
    }
    return "";
}

char* archGetMappedKey()
{
    if (selectedKey != 0) {
        return dik2str(selectedDikKey);
    }
    return "";
}

void archKeyboardSetSelectedKey(int msxKeyCode) {
    int i;
    selectedKey = msxKeyCode;
    selectedDikKey = 0;
    selectedTable = 0;

    if (inputEventIsJoystick1(msxKeyCode)) {
        selectedTable = 1;
    }

    if (inputEventIsJoystick2(msxKeyCode)) {
        selectedTable = 2;
    }

    for (i = 0; i < KBD_TABLE_LEN; i++) {
        if (kbdTable[selectedTable][i] == selectedKey) {
            selectedDikKey = i;
        }
    }
}

void archPollInput() {
    keyboardUpdate();
    joystickUpdate();
}

int archKeyboardIsKeySelected(int msxKeyCode)
{
    return editEnabled && (msxKeyCode == selectedKey);
}

int archKeyboardIsKeyConfigured(int msxKeyCode)
{
    int i, n;

    if (!editEnabled) {
        return 1;
    }

    for (n = 0; n < KBD_TABLE_NUM; n++) {
        for (i = 0; i < KBD_TABLE_LEN; i++) {
            if (kbdTable[n][i] == msxKeyCode) {
                return 1;
            }
        }
    }
    return 0;
}

void keybardEnableEdit(int enable)
{
    if (enable) {
        if (kbdTable[0][DIK_NUMPADENTER] == kbdTable[0][DIK_RETURN]) {
            kbdTable[0][DIK_NUMPADENTER] = 0;
        }
    }
    else {
        if (kbdTable[0][DIK_NUMPADENTER] == 0) {
            kbdTable[0][DIK_NUMPADENTER] = kbdTable[0][DIK_RETURN];
        }
    }

    editEnabled = enable;
    selectedKey = 0;
    selectedDikKey = 0;
}

void keyboardStartConfig() 
{
    int n;
    for (n = 0; n < KBD_TABLE_NUM; n++) {
        memcpy(kbdTableBackup[n], kbdTable[n], sizeof(kbdTableBackup[n]));
    }
}

void keyboardCancelConfig() 
{
    int n;
    for (n = 0; n < KBD_TABLE_NUM; n++) {
        memcpy(kbdTable[n], kbdTableBackup[n], sizeof(kbdTableBackup[n]));
    }
}

int  keyboardConfigIsModified()
{
    int n;
    int diff = 0;
    for (n = 0; n < KBD_TABLE_NUM; n++) {
        diff |= memcmp(kbdTable[n], kbdTableBackup[n], sizeof(kbdTableBackup[n]));
    }
    return diff;
}



char* archKeyconfigSelectedKeyTitle() {
    return langKeyconfigSelectedKey();
}

char* archKeyconfigMappedToTitle() {
    return langKeyconfigMappedTo();
}

char* archKeyconfigMappingSchemeTitle() {
    return langKeyconfigMappingScheme();
}


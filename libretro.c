#include "libretro.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include <retro_miscellaneous.h>

#include "CommandLine.h"
#include "Properties.h"
#include "ArchFile.h"
#include "VideoRender.h"
#include "AudioMixer.h"
#include "Casette.h"
#include "PrinterIO.h"
#include "UartIO.h"
#include "MidiIO.h"
#include "Machine.h"
#include "Board.h"
#include "Emulator.h"
#include "FileHistory.h"
#include "Actions.h"
#include "Language.h"
#include "LaunchFile.h"
#include "ArchEvent.h"
#include "ArchSound.h"
#include "ArchNotifications.h"
#include "JoystickPort.h"
#include "InputEvent.h"
#include "R800.h"

#include "ziphelper.c"



retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;

static Properties* properties;
static Video* video;
static Mixer* mixer;

static uint16_t* image_buffer;
static unsigned image_buffer_base_width;
static unsigned image_buffer_current_width;
static unsigned image_buffer_height;
static unsigned width = 272;
static unsigned height = 240;
static int double_width;


static char msx_type[256];
static char msx_cartmapper[256];
static bool mapper_auto;
bool is_coleco, is_sega, is_spectra, is_auto, auto_rewind_cas;
static unsigned msx_vdp_synctype;
static bool msx_ym2413_enable;
static bool use_overscan = true;
int msx2_dif = 0;


void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

#ifdef LOG_PERFORMANCE
static struct retro_perf_callback perf_cb;
#define RETRO_PERFORMANCE_INIT(name) static struct retro_perf_counter name = {#name}; if (!name.registered) perf_cb.perf_register(&(name))
#define RETRO_PERFORMANCE_START(name) perf_cb.perf_start(&(name))
#define RETRO_PERFORMANCE_STOP(name) perf_cb.perf_stop(&(name))
#else
#define RETRO_PERFORMANCE_INIT(name)
#define RETRO_PERFORMANCE_START(name)
#define RETRO_PERFORMANCE_STOP(name)
#endif

#define RETRO_DEVICE_MAPPER RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 1)
#define EC_KEYBOARD_KEYCOUNT  94

#ifdef _WIN32
#define SLASH '\\'
#else
#define SLASH '/'
#endif

#ifndef PATH_MAX
#define PATH_MAX  4096
#endif

static char base_dir[PATH_MAX];

/* .dsk support */
enum{
   MEDIA_TYPE_CART = 0,
   MEDIA_TYPE_TAPE,
   MEDIA_TYPE_DISK,
   MEDIA_TYPE_DISK_BUNDLE,
   MEDIA_TYPE_OTHER
};

void lower_string(char* str)
{
   int i;
   for (i=0; str[i]; i++)
   {
      str[i] = tolower(str[i]);
   }
}

int get_media_type(const char* filename)
{
   char workram[PATH_MAX];
   const char *extension = NULL;

   strcpy(workram, filename);
   lower_string(workram);
   extension = workram + strlen(workram) - 4;

   if(strcmp(extension, ".dsk") == 0){
      if (is_auto){
         strcpy(msx_type, "MSX2+");
      }
      return MEDIA_TYPE_DISK;
   }
   else if(strcmp(extension, ".m3u") == 0){
      if (is_auto){
         strcpy(msx_type, "MSX2+");
      }
      return MEDIA_TYPE_DISK_BUNDLE;
   }
   else if(strcmp(extension, ".cas") == 0){
      if (is_auto){
         strcpy(msx_type, "MSX2+");
      }
      return MEDIA_TYPE_TAPE;
   }
   else if(strcmp(extension, ".rom") == 0){
      if (is_auto){
         strcpy(msx_type, "MSX2+");
      }
      return MEDIA_TYPE_CART;
   }
   else if(strcmp(extension, ".mx1") == 0){
      if (is_auto){
         strcpy(msx_type, "MSX2+");
      }
      return MEDIA_TYPE_CART;
   }
   else if(strcmp(extension, ".mx2") == 0){
      if (is_auto){
         strcpy(msx_type, "MSX2+");
      }
      return MEDIA_TYPE_CART;
   }
   else if(strcmp(extension, ".col") == 0){
      if (is_auto){
         is_coleco = true;
         strcpy(msx_type, "COL - ColecoVision");
      }
      return MEDIA_TYPE_CART;
   }
   else if(strcmp(extension, ".sg") == 0){
      if (is_auto){
         is_sega = true;
         strcpy(msx_type, "SEGA - SC-3000");
      }
      return MEDIA_TYPE_CART;
   }

   return MEDIA_TYPE_OTHER;
}

/* end .dsk support */
/* .dsk swap support */
struct retro_disk_control_callback dskcb;
unsigned disk_index = 0;
unsigned disk_images = 0;
char disk_paths[10][PATH_MAX];
bool disk_inserted = false;

bool set_eject_state(bool ejected)
{
   disk_inserted = !ejected;
   return true;
}

bool get_eject_state(void)
{
   return !disk_inserted;
}

unsigned get_image_index(void)
{
   return disk_index;
}

bool set_image_index(unsigned index)
{
   disk_index = index;
   
   if(disk_index == disk_images)
   {
      //retroarch is trying to set "no disk in tray"
      return true;
   }
   
   emulatorSuspend();
   insertDiskette(properties, 0 /*drive*/, disk_paths[disk_index] /*fname*/, NULL /*inZipFile*/, -1 /*forceAutostart, -1 is force no autostart*/);
   emulatorResume();
   
   return true;
}

unsigned get_num_images(void)
{
   return disk_images;
}

bool add_image_index(void)
{
   if (disk_images >= 10)
      return false;
   
   disk_images++;
   return true;
}

bool replace_image_index(unsigned index, const struct retro_game_info *info)
{
   if(get_media_type(info->path) != MEDIA_TYPE_DISK)
       return false;//cant swap a cart or tape into a disk slot
    
   strcpy(disk_paths[index], info->path);
   return true;
}

void attach_disk_swap_interface(void)
{
   /* these functions are unused */
   dskcb.set_eject_state = set_eject_state;
   dskcb.get_eject_state = get_eject_state;
   dskcb.set_image_index = set_image_index;
   dskcb.get_image_index = get_image_index;
   dskcb.get_num_images  = get_num_images;
   dskcb.add_image_index = add_image_index;
   dskcb.replace_image_index = replace_image_index;

   environ_cb(RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE, &dskcb);
}
/* end .dsk swap support */

static bool read_m3u(const char *file)
{
   char line[PATH_MAX];
   char name[PATH_MAX];
   FILE *f = fopen(file, "r");
   
   if (!f)
      return false;

   while (fgets(line, sizeof(line), f) && disk_images < sizeof(disk_paths) / sizeof(disk_paths[0])) 
   {
      char *carriage_return = NULL;
      char *newline         = NULL;

      if (line[0] == '#')
         continue;

      carriage_return = strchr(line, '\r');
      if (carriage_return)
         *carriage_return = '\0';
      
      newline = strchr(line, '\n');
      if (newline)
         *newline = '\0';

      if (line[0] != '\0')
      {
         snprintf(name, sizeof(name), "%s%c%s", base_dir, SLASH, line);
         strcpy(disk_paths[disk_images], name);
         disk_images++;
      }
   }

   fclose(f);
   return (disk_images != 0);
}

extern BoardInfo boardInfo;

#define MAX_PADS 2
static unsigned input_devices[MAX_PADS];

extern int eventMap[256];

static unsigned btn_map[EC_KEYCOUNT] =
{
   RETROK_UNKNOWN,         //EC_NONE      0

   // ROW 0
   RETROK_F1,              //EC_F1        1
   RETROK_F2,              //EC_F2        2
   RETROK_F3,              //EC_F3        3
   RETROK_F4,              //EC_F4        4
   RETROK_F5,              //EC_F5        5
   RETROK_END,             //EC_STOP      6
   RETROK_HOME,            //EC_CLS       7
   RETROK_PAGEUP,          //EC_SELECT    8
   RETROK_INSERT,          //EC_INS       9
   RETROK_DELETE,          //EC_DEL      10

   // ROW 1
   RETROK_ESCAPE,          //EC_ESC      11
   RETROK_1,               //EC_1        12
   RETROK_2,               //EC_2        13
   RETROK_3,               //EC_3        14
   RETROK_4,               //EC_4        15
   RETROK_5,               //EC_5        16
   RETROK_6,               //EC_6        17
   RETROK_7,               //EC_7        18
   RETROK_8,               //EC_8        19
   RETROK_9,               //EC_9        20
   RETROK_0,               //EC_0        21
   RETROK_MINUS,           //EC_NEG      22
   RETROK_EQUALS,          //EC_CIRCFLX  23
   RETROK_BACKSLASH,       //EC_BKSLASH  24 (YEN)
   RETROK_BACKSPACE,       //EC_BKSPACE  25

   // ROW 2
   RETROK_TAB,             //EC_TAB      26
   RETROK_q,               //EC_Q        27
   RETROK_w,               //EC_W        28
   RETROK_e,               //EC_E        29
   RETROK_r,               //EC_R        30
   RETROK_t,               //EC_T        31
   RETROK_y,               //EC_Y        32
   RETROK_u,               //EC_U        33
   RETROK_i,               //EC_I        34
   RETROK_o,               //EC_O        35
   RETROK_p,               //EC_P        36
   RETROK_BACKQUOTE,       //EC_AT       37
   RETROK_LEFTBRACKET,     //EC_LBRACK   38
   RETROK_RETURN,          //EC_RETURN   39

   // ROW 3
   RETROK_LCTRL,           //EC_CTRL     40
   RETROK_a,               //EC_A        41
   RETROK_s,               //EC_S        42
   RETROK_d,               //EC_D        43
   RETROK_f,               //EC_F        44
   RETROK_g,               //EC_G        45
   RETROK_h,               //EC_H        46
   RETROK_j,               //EC_J        47
   RETROK_k,               //EC_K        48
   RETROK_l,               //EC_L        49
   RETROK_SEMICOLON,       //EC_SEMICOL  50
   RETROK_QUOTE,           //EC_COLON    51
   RETROK_RIGHTBRACKET,    //EC_RBRACK   52

   // ROW 4
   RETROK_LSHIFT,          //EC_LSHIFT   53
   RETROK_z,               //EC_Z        54
   RETROK_x,               //EC_X        55
   RETROK_c,               //EC_C        56
   RETROK_v,               //EC_V        57
   RETROK_b,               //EC_B        58
   RETROK_n,               //EC_N        59
   RETROK_m,               //EC_M        60
   RETROK_COMMA,           //EC_COMMA    61
   RETROK_PERIOD,          //EC_PERIOD   62
   RETROK_SLASH,           //EC_DIV      63
   RETROK_0,               //EC_UNDSCRE  64 (as Shift+0)
   RETROK_RSHIFT,          //EC_RSHIFT   65

   // ROW 5
   RETROK_CAPSLOCK,        //EC_CAPS     66
   RETROK_LALT,            //EC_GRAPH    67
   RETROK_UNKNOWN,         //EC_TORIKE   68
   RETROK_SPACE,           //EC_SPACE    69
   RETROK_UNKNOWN,         //EC_JIKKOU   70
   RETROK_UNKNOWN,         //EC_CODE     71
   RETROK_PAUSE,           //EC_PAUSE    72

   // ARROWS
   RETROK_LEFT,            //EC_LEFT     73
   RETROK_UP,              //EC_UP       74
   RETROK_DOWN,            //EC_DOWN     75
   RETROK_RIGHT,           //EC_RIGHT    76

   // NUMERIC KEYBOARD
   RETROK_KP7,             //EC_NUM7     77
   RETROK_KP8,             //EC_NUM8     78
   RETROK_KP9,             //EC_NUM9     79
   RETROK_KP_DIVIDE,       //EC_NUMDIV   80
   RETROK_KP4,             //EC_NUM4     81
   RETROK_KP5,             //EC_NUM5     82
   RETROK_KP6,             //EC_NUM6     83
   RETROK_KP_MULTIPLY,     //EC_NUMMUL   84
   RETROK_KP1,             //EC_NUM1     85
   RETROK_KP2,             //EC_NUM2     86
   RETROK_KP3,             //EC_NUM3     87
   RETROK_KP_MINUS,        //EC_NUMSUB   88
   RETROK_KP0,             //EC_NUM0     89
   RETROK_KP_ENTER,        //EC_NUMPER   90
   RETROK_KP_PERIOD,       //EC_NUMCOM   91
   RETROK_KP_PLUS,         //EC_NUMADD   92

   // SVI SPECIFIC KEYS
   RETROK_PRINT,           //EC_PRINT    93

   RETROK_UNKNOWN,
   RETROK_UNKNOWN,
   RETROK_UNKNOWN,
   RETROK_UNKNOWN,
   RETROK_UNKNOWN,
   RETROK_UNKNOWN,

   RETRO_DEVICE_ID_JOYPAD_UP,       //EC_JOY1_UP      100
   RETRO_DEVICE_ID_JOYPAD_DOWN,     //EC_JOY1_DOWN    101
   RETRO_DEVICE_ID_JOYPAD_LEFT,     //EC_JOY1_LEFT    102
   RETRO_DEVICE_ID_JOYPAD_RIGHT,    //EC_JOY1_RIGHT   103
   RETRO_DEVICE_ID_JOYPAD_A,        //EC_JOY1_BUTTON1 104
   RETRO_DEVICE_ID_JOYPAD_B,        //EC_JOY1_BUTTON2 105
   RETRO_DEVICE_ID_JOYPAD_Y,        //EC_JOY1_BUTTON3 106
   RETRO_DEVICE_ID_JOYPAD_X,        //EC_JOY1_BUTTON4 107
   RETRO_DEVICE_ID_JOYPAD_START,    //EC_JOY1_BUTTON5 108
   RETRO_DEVICE_ID_JOYPAD_SELECT,   //EC_JOY1_BUTTON6 109

   RETRO_DEVICE_ID_JOYPAD_UP,       //EC_JOY2_UP      110
   RETRO_DEVICE_ID_JOYPAD_DOWN,     //EC_JOY2_DOWN    111
   RETRO_DEVICE_ID_JOYPAD_LEFT,     //EC_JOY2_LEFT    112
   RETRO_DEVICE_ID_JOYPAD_RIGHT,    //EC_JOY2_RIGHT   113
   RETRO_DEVICE_ID_JOYPAD_A,        //EC_JOY2_BUTTON1 114
   RETRO_DEVICE_ID_JOYPAD_B,        //EC_JOY2_BUTTON2 115
   RETRO_DEVICE_ID_JOYPAD_Y,        //EC_JOY2_BUTTON3 116
   RETRO_DEVICE_ID_JOYPAD_X,        //EC_JOY2_BUTTON4 117
   RETRO_DEVICE_ID_JOYPAD_START,    //EC_JOY2_BUTTON5 118
   RETRO_DEVICE_ID_JOYPAD_SELECT,   //EC_JOY2_BUTTON6 119

   RETROK_UNKNOWN,                  //EC_COLECO1_0    120
   RETROK_UNKNOWN,                  //EC_COLECO1_1    121
   RETROK_UNKNOWN,                  //EC_COLECO1_2    122
   RETROK_UNKNOWN,                  //EC_COLECO1_3    123
   RETROK_UNKNOWN,                  //EC_COLECO1_4    124
   RETROK_UNKNOWN,                  //EC_COLECO1_5    125
   RETROK_UNKNOWN,                  //EC_COLECO1_6    126
   RETROK_UNKNOWN,                  //EC_COLECO1_7    127
   RETROK_UNKNOWN,                  //EC_COLECO1_8    128
   RETROK_UNKNOWN,                  //EC_COLECO1_9    129
   RETROK_UNKNOWN,                  //EC_COLECO1_STAR 130
   RETROK_UNKNOWN,                  //EC_COLECO1_HASH 131

   RETROK_UNKNOWN,                  //EC_COLECO2_0    140
   RETROK_UNKNOWN,                  //EC_COLECO2_1    141
   RETROK_UNKNOWN,                  //EC_COLECO2_2    142
   RETROK_UNKNOWN,                  //EC_COLECO2_3    143
   RETROK_UNKNOWN,                  //EC_COLECO2_4    144
   RETROK_UNKNOWN,                  //EC_COLECO2_5    145
   RETROK_UNKNOWN,                  //EC_COLECO2_6    146
   RETROK_UNKNOWN,                  //EC_COLECO2_7    147
   RETROK_UNKNOWN,                  //EC_COLECO2_8    148
   RETROK_UNKNOWN,                  //EC_COLECO2_9    149
   RETROK_UNKNOWN,                  //EC_COLECO2_STAR 150
   RETROK_UNKNOWN,                  //EC_COLECO2_HASH 151
};

void retro_get_system_info(struct retro_system_info *info)
{
   info->library_name = "blueMSX";
#ifdef GIT_VERSION
   info->library_version = "git" GIT_VERSION;
#else
   info->library_version = "svn";
#endif
   info->need_fullpath = true;
   info->block_extract = false;
   info->valid_extensions = "rom|ri|mx1|mx2|dsk|col|sg|sc|cas|m3u";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    width  = use_overscan ? 272 : (272 - 16);
    height = use_overscan ? 240 : (240 - 48 + (msx2_dif * 2));
   info->geometry.base_width = width ;
   info->geometry.base_height = height ;
   info->geometry.max_width = FB_MAX_LINE_WIDTH ;
   info->geometry.max_height = FB_MAX_LINES ;
   info->geometry.aspect_ratio = 0;
   info->timing.fps = 60.0;
   info->timing.sample_rate = 44100.0;
}

void init_input_descriptors(unsigned device){
   struct retro_input_descriptor desc_retropad[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Joy Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Joy Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Joy Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Joy Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "1 / coleco but.1" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "2 / coleco but.2" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "3 / coleco 2" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "4 / coleco 1" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "coleco #" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"coleco *" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "5 / coleco 4" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "6 / coleco 3" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "coleco 6" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "coleco 5" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3,    "coleco 8" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3,    "coleco 7" },

      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Joy Left" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Joy Up" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Joy Down" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Joy Right" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "1 / coleco but.1" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "2 / coleco but.2" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "3 / coleco 2" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "4 / coleco 1" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "coleco #" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"coleco *" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "5 / coleco 4" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "6 / coleco 3" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "coleco 6" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "coleco 5" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3,    "coleco 8" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3,    "coleco 7" },

      { 0, 0, 0, 0, NULL }
   };


   struct retro_input_descriptor desc[] = {
      { 0, 0, 0, 0, NULL }
   };
   /* Todo: RetroPad Keymapper binds */

   if (device == RETRO_DEVICE_JOYPAD)
      environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc_retropad);
   else
      environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}

void retro_init(void)
{
   int i;
   struct retro_log_callback log;

   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else
      log_cb = NULL;

#ifdef LOG_PERFORMANCE
   environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb);
#endif
   memZipFileSystemCreate(1);

}

void retro_deinit(void)
{
#ifdef LOG_PERFORMANCE
   perf_cb.perf_log();
#endif
}

void retro_set_environment(retro_environment_t cb)
{
   static const struct retro_variable vars[] = {
      { "bluemsx_msxtype", "Machine Type (Restart); Auto|MSX|MSXturboR|MSX2|MSX2+|SEGA - SG-1000|SEGA - SC-3000|SEGA - SF-7000|SVI - Spectravideo SVI-318|SVI - Spectravideo SVI-328|SVI - Spectravideo SVI-328 MK2|ColecoVision|Coleco (Spectravideo SVI-603)" },
      { "bluemsx_overscan", "Crop Overscan; disabled|enabled|MSX2" },
      { "bluemsx_vdp_synctype", "VDP Sync Type (Restart); Auto|50Hz|60Hz" },
      { "bluemsx_nospritelimits", "No Sprite Limit; OFF|ON" },
      { "bluemsx_ym2413_enable", "Sound YM2413 Enable (Restart); enabled|disabled" },
      { "bluemsx_cartmapper", "Cart Mapper Type (Restart); Auto|Normal|mirrored|basic|0x4000|0xC000|ascii8|ascii8sram|ascii16|ascii16sram|ascii16nf|konami4|konami4nf|konami5|konamisynth|korean80|korean90|korean126|MegaFlashRomScc|MegaFlashRomSccPlus|msxdos2|scc|sccexpanded|sccmirrored|sccplus|snatcher|sdsnatcher|SegaBasic|SG1000|SG1000Castle|SG1000RamA|SG1000RamB|SC3000" },
      { "bluemsx_auto_rewind_cas", "Auto Rewind Cassette; ON|OFF" },
      { NULL, NULL },
   };

   static const struct retro_controller_description port[] = {
      { "RetroPad",              RETRO_DEVICE_JOYPAD },
      { "RetroKeyboard",         RETRO_DEVICE_KEYBOARD },
      { "RetroPad Keyboard Map", RETRO_DEVICE_MAPPER },
      { 0 },
   };

   static const struct retro_controller_info ports[] = {
      { port, 3 },
      { port, 2 },
      { NULL, 0 },
   };

   environ_cb = cb;

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   /*there was a buffer overflow when retroarch called this function with a value greater than 1
   corrupting the framebuffer pointer and crashing on first render(MAX_PADS == 2 so any index that is not [0,1] is an overflow)*/
   if(port < MAX_PADS)
   {
      switch (device)
      {
         case RETRO_DEVICE_JOYPAD:
            input_devices[port] = RETRO_DEVICE_JOYPAD;
            init_input_descriptors(RETRO_DEVICE_JOYPAD);
            break;
         case RETRO_DEVICE_MAPPER:
            input_devices[port] = RETRO_DEVICE_MAPPER;
            init_input_descriptors(RETRO_DEVICE_MAPPER);
            break;
         case RETRO_DEVICE_KEYBOARD:
            input_devices[port] = RETRO_DEVICE_KEYBOARD;
            init_input_descriptors(RETRO_DEVICE_KEYBOARD);
            break;
         default:
            if (log_cb)
               log_cb(RETRO_LOG_ERROR, "%s\n", "[libretro]: Invalid device, setting type to RETRO_DEVICE_JOYPAD ...");
            input_devices[port] = RETRO_DEVICE_JOYPAD;
      }
   }
}

void retro_reset(void)
{
   actionEmuResetSoft();

   /* Apply mapper override on reset, force mapper detection again */
   if (properties->media.carts[0].fileName[0]){
      if (!mapper_auto)
         insertCartridge(properties, 0, properties->media.carts[0].fileName, properties->media.carts[0].fileNameInZip, mediaDbStringToType(msx_cartmapper), -1);
      else
         insertCartridge(properties, 0, properties->media.carts[0].fileName, properties->media.carts[0].fileNameInZip, 0, -1);
   }
}

static void extract_directory(char *buf, const char *path, size_t size)
{
   char *base = NULL;

   strncpy(buf, path, size - 1);
   buf[size - 1] = '\0';

   base = strrchr(buf, '/');
   if (!base)
      base = strrchr(buf, '\\');

   if (base)
      *base = '\0';
   else
      buf[0] = '\0';
}

static void check_variables(void)
{
   struct retro_system_av_info av_info;
   bool geometry_update = false;
   struct retro_variable var;
   
   var.key = "bluemsx_msxtype";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "ColecoVision"))
      {
         is_coleco = true;
         strcpy(msx_type, "COL - ColecoVision");
      }
      else if (!strcmp(var.value, "Coleco (Spectravideo SVI-603)"))
      {
         is_coleco = true;
         strcpy(msx_type, "COL - Spectravideo SVI-603 Coleco");
      }
      else if (strcmp(var.value, "Auto"))
      {
         is_coleco = false;
         strcpy(msx_type, var.value);
         
         if (!strncmp(var.value, "SEGA", 4))
            is_sega = true;
         if (!strncmp(var.value, "SVI", 3))
            is_spectra = true;
      }
      else
      {
         is_auto = true;
         strcpy(msx_type, "SEGA - SC-3000");
      }
   }
   else
   {
      is_auto = true;
      // Sega machines don't work if not set right from the start
      strcpy(msx_type, "SEGA - SC-3000");
   }

   var.key = "bluemsx_overscan";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "disabled"));
      int msx2_dif_old = msx2_dif;
      
      if (!strcmp(var.value, "MSX2"))
         msx2_dif = 10;
      else
         msx2_dif = 0;
      
      if (msx2_dif_old != msx2_dif)
         geometry_update = true;
         
      if (newval != use_overscan)
      {
         use_overscan = newval;
         geometry_update = true;
      }
   }
   
   var.key = "bluemsx_vdp_synctype";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "Auto"))
         msx_vdp_synctype = P_VDP_SYNCAUTO;
      else if (!strcmp(var.value, "50Hz"))
         msx_vdp_synctype = P_VDP_SYNC50HZ;
      else if (!strcmp(var.value, "60Hz"))
         msx_vdp_synctype = P_VDP_SYNC60HZ;
   }
   else
      msx_vdp_synctype = P_VDP_SYNCAUTO;

   var.key = "bluemsx_nospritelimits";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "OFF"))
         vdpSetNoSpriteLimits(0);
      else
         vdpSetNoSpriteLimits(1);
   }
   else
      vdpSetNoSpriteLimits(0);

   var.key = "bluemsx_ym2413_enable";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "disabled"))
         msx_ym2413_enable = false;
      else if (!strcmp(var.value, "enabled"))
         msx_ym2413_enable = true;
   }
   else
      msx_ym2413_enable = true;

   var.key = "bluemsx_cartmapper";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "Auto"))
         mapper_auto = true;
      else 
      {
         mapper_auto = false;
         strcpy(msx_cartmapper, var.value);
      }
   }

   var.key = "bluemsx_auto_rewind_cas";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "OFF"))
         auto_rewind_cas = false;
      else
         auto_rewind_cas = true;
   }
   else
      auto_rewind_cas = true;

   if (geometry_update)
   {
      retro_get_system_av_info(&av_info);
      environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
   }
}

bool retro_load_game(const struct retro_game_info *info)
{
   const char *save_dir = NULL;
   int i, media_type;
   char properties_dir[256], machines_dir[256], mediadb_dir[256];
   const char *dir = NULL;
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;

   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      if (log_cb)
         log_cb(RETRO_LOG_INFO, "%s\n", "RGB565 is not supported.");
      return false;
   }

   if (!info)
      return false;

   image_buffer               =  malloc(FB_MAX_LINE_WIDTH*FB_MAX_LINES*sizeof(uint16_t));
   image_buffer_base_width    =  272;
   image_buffer_current_width =  image_buffer_base_width;
   image_buffer_height        =  240;
   double_width = 0;

   for (i = 0; i < MAX_PADS; i++)
      input_devices[i] = RETRO_DEVICE_JOYPAD;

   disk_index = 0;
   disk_images = 0;
   disk_inserted = false;
   extract_directory(base_dir, info->path, sizeof(base_dir));
   
   check_variables();

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
      strcpy(properties_dir, dir);
   else /* Fallback */
      extract_directory(properties_dir, info->path, sizeof(properties_dir));

   snprintf(machines_dir, sizeof(machines_dir), "%s%c%s", properties_dir, SLASH, "Machines");
   snprintf(mediadb_dir, sizeof(mediadb_dir), "%s%c%s", properties_dir, SLASH, "Databases");

   propertiesSetDirectory(properties_dir, properties_dir);
   machineSetDirectory(machines_dir);
   
   if(environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save_dir) && save_dir)
      boardSetDirectory(save_dir);
   
#if 0
   boardSetDirectory(buffer);
#endif
   mediaDbLoad(mediadb_dir);
#if 0
   mediaDbCreateRomdb();
   mediaDbCreateDiskdb();
   mediaDbCreateCasdb();
#endif

   properties = propCreate(1, EMU_LANG_ENGLISH, P_KBD_EUROPEAN, P_EMU_SYNCNONE, "");

   media_type = get_media_type(info->path);

   if (is_coleco)
   {
      strcpy(properties->joy1.type, "coleco joystick");
      properties->joy1.typeId            = JOYSTICK_PORT_COLECOJOYSTICK;
      properties->joy1.autofire          = 0;

      strcpy(properties->joy2.type, "coleco joystick");
      properties->joy2.typeId            = JOYSTICK_PORT_COLECOJOYSTICK;
      properties->joy2.autofire          = 0;
   }
   else
   {
      strcpy(properties->joy1.type, "joystick");
      properties->joy1.typeId            = JOYSTICK_PORT_JOYSTICK;
      properties->joy1.autofire          = 0;

      strcpy(properties->joy2.type, "joystick");
      properties->joy2.typeId            = JOYSTICK_PORT_JOYSTICK;
      properties->joy2.autofire          = 0;
   }

   properties->emulation.vdpSyncMode       = msx_vdp_synctype;
   properties->video.monitorType           = P_VIDEO_PALNONE;
   strcpy(properties->emulation.machineName, msx_type);

   properties->sound.chip.enableYM2413 = msx_ym2413_enable;

   mixer = mixerCreate();

   emulatorInit(properties, mixer);
   actionInit(video, properties, mixer);
   langInit();
   tapeSetReadOnly(properties->cassette.readOnly);
   if (auto_rewind_cas)
      tapeRewindNextInsert();

   langSetLanguage(properties->language);

   joystickPortSetType(0, properties->joy1.typeId);
   joystickPortSetType(1, properties->joy2.typeId);

#if 0
   printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
   uartIoSetType(properties->ports.Com.type, properties->ports.Com.fileName);
   midiIoSetMidiOutType(properties->sound.MidiOut.type, properties->sound.MidiOut.fileName);
   midiIoSetMidiInType(properties->sound.MidiIn.type, properties->sound.MidiIn.fileName);
   ykIoSetMidiInType(properties->sound.YkIn.type, properties->sound.YkIn.fileName);
#endif

   emulatorRestartSound();

   for (i = 0; i < MIXER_CHANNEL_TYPE_COUNT; i++)
   {
      mixerSetChannelTypeVolume(mixer, i, properties->sound.mixerChannel[i].volume);
      mixerSetChannelTypePan(mixer, i, properties->sound.mixerChannel[i].pan);
      mixerEnableChannelType(mixer, i, properties->sound.mixerChannel[i].enable);
   }

   mixerSetMasterVolume(mixer, properties->sound.masterVolume);
   mixerEnableMaster(mixer, properties->sound.masterEnable);


   if (mapper_auto)
      mediaDbSetDefaultRomType(properties->cartridge.defaultType);
   else
      mediaDbSetDefaultRomType(mediaDbStringToType(msx_cartmapper));

   switch(media_type)
   {
      case MEDIA_TYPE_DISK:
         strcpy(disk_paths[0] , info->path);
         strcpy(properties->media.disks[0].fileName , info->path);
         disk_inserted = true;
         attach_disk_swap_interface();
         break;
      case MEDIA_TYPE_DISK_BUNDLE:
         if (!read_m3u(info->path))
         {
            if (log_cb)
               log_cb(RETRO_LOG_ERROR, "%s\n", "[libretro]: failed to read m3u file ...");
            return false;
         }
         for (i = 0; (i <= disk_images) && (i <= 1); i++)
         {
            strcpy(properties->media.disks[i].fileName , disk_paths[i]);
         }
         disk_inserted = true;
         attach_disk_swap_interface();
         break;
      case MEDIA_TYPE_TAPE:
         strcpy(properties->media.tapes[0].fileName , info->path);
         break;
      case MEDIA_TYPE_CART:
      case MEDIA_TYPE_OTHER:
      default:
         strcpy(properties->media.carts[0].fileName , info->path);
         break;
   }

   for (i = 0; i < PROP_MAX_CARTS; i++)
   {
   /*    Breaks database detection
      if (properties->media.carts[i].fileName[0] && mapper_auto)
         insertCartridge(properties, i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip, properties->media.carts[i].type, -1);
   */
      if (properties->media.carts[i].fileName[0] && !mapper_auto)
         insertCartridge(properties, i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip, mediaDbStringToType(msx_cartmapper), -1);

      updateExtendedRomName(i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip);
   }

   for (i = 0; i < PROP_MAX_DISKS; i++)
   {
      if (properties->media.disks[i].fileName[0])
         insertDiskette(properties, i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip, -1);
      updateExtendedDiskName(i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip);
   }

   for (i = 0; i < PROP_MAX_TAPES; i++)
   {
      if (properties->media.tapes[i].fileName[0])
         insertCassette(properties, i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip, 0);
      updateExtendedCasName(i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip);
   }

   {
      Machine* machine = machineCreate(properties->emulation.machineName);
      if (machine != NULL)
      {
         boardSetMachine(machine);
         machineDestroy(machine);
      }
      else
         return false;
   }
   
   boardSetFdcTimingEnable(properties->emulation.enableFdcTiming);
   boardSetY8950Enable(properties->sound.chip.enableY8950);
   boardSetYm2413Enable(properties->sound.chip.enableYM2413);
   boardSetMoonsoundEnable(properties->sound.chip.enableMoonsound);
   boardSetVideoAutodetect(properties->video.detectActiveMonitor);

   emulatorStart(NULL);
   return true;
}

void timerCallback_global(void* timer);

UInt8 archJoystickGetState(int joystickNo)
{
   return ((eventMap[EC_JOY1_UP]    << 0) |
         (eventMap[EC_JOY1_DOWN]    << 1) |
         (eventMap[EC_JOY1_LEFT]    << 2) |
         (eventMap[EC_JOY1_RIGHT]   << 3) |
         (eventMap[EC_JOY1_BUTTON1] << 4) |
         (eventMap[EC_JOY1_BUTTON2] << 5) |
         (eventMap[EC_JOY1_BUTTON3] << 6) |
         (eventMap[EC_JOY1_BUTTON4] << 7));
}

void retro_run(void)
{
   int i,j;
   bool updated = false;
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();

   RETRO_PERFORMANCE_INIT(core_retro_run);
   RETRO_PERFORMANCE_START(core_retro_run);

   input_poll_cb();

   if (is_coleco)
   {
      /* ColecoVision Input Part */
      for (i = 0; i < MAX_PADS; i++)
      {
         switch (input_devices[i])
         {
            case RETRO_DEVICE_JOYPAD:
               if (i == 0){
                  for (j = EC_JOY1_UP; j <= (EC_JOY1_BUTTON2); j++)
                     eventMap[j] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, btn_map[j]) ? 1 : 0;
               }else if (i == 1){
                  for (j = EC_JOY2_UP; j <= (EC_JOY2_BUTTON2); j++)
                     eventMap[j] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, btn_map[j]) ? 1 : 0;
               }
               break;
         }
      }
      eventMap[EC_COLECO1_1]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)      ? 1 : 0;
      eventMap[EC_COLECO1_2]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)      ? 1 : 0;
      eventMap[EC_COLECO1_3]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R)      ? 1 : 0;
      eventMap[EC_COLECO1_4]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L)      ? 1 : 0;
      eventMap[EC_COLECO1_5]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2)     ? 1 : 0;
      eventMap[EC_COLECO1_6]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2)     ? 1 : 0;
      eventMap[EC_COLECO1_7]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3)     ? 1 : 0;
      eventMap[EC_COLECO1_8]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3)     ? 1 : 0;
      eventMap[EC_COLECO1_STAR]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT) ? 1 : 0;
      eventMap[EC_COLECO1_HASH]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)  ? 1 : 0;
      eventMap[EC_COLECO1_0]     = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_1)                    ? 1 : 0;
      eventMap[EC_COLECO1_9]     = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_2)                    ? 1 : 0;

      eventMap[EC_COLECO2_1]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)      ? 1 : 0;
      eventMap[EC_COLECO2_2]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)      ? 1 : 0;
      eventMap[EC_COLECO2_3]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R)      ? 1 : 0;
      eventMap[EC_COLECO2_4]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L)      ? 1 : 0;
      eventMap[EC_COLECO2_5]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2)     ? 1 : 0;
      eventMap[EC_COLECO2_6]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2)     ? 1 : 0;
      eventMap[EC_COLECO2_7]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3)     ? 1 : 0;
      eventMap[EC_COLECO2_8]     = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3)     ? 1 : 0;
      eventMap[EC_COLECO2_STAR]  = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT) ? 1 : 0;
      eventMap[EC_COLECO2_HASH]  = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)  ? 1 : 0;
      eventMap[EC_COLECO2_0]     = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_3)                    ? 1 : 0;
      eventMap[EC_COLECO2_9]     = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_4)                    ? 1 : 0;
   }
   else
   {
      /* MSX Input Part */
      for (i = 0; i < MAX_PADS; i++)
      {
         switch (input_devices[i])
         {
            case RETRO_DEVICE_JOYPAD:
               if (i == 0)
                  for (j = EC_JOY1_UP; j <= (EC_JOY1_BUTTON6); j++)
                     eventMap[j] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, btn_map[j]) ? 1 : 0;
               else if (i == 1)
                  for (j = EC_JOY2_UP; j <= (EC_JOY2_BUTTON6); j++)
                     eventMap[j] = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, btn_map[j]) ? 1 : 0;
               break;
         }
      }

      for (j = 0; j < EC_KEYBOARD_KEYCOUNT; j++)
         eventMap[j] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, btn_map[j]) ? 1 : 0;

      if (input_devices[0] == RETRO_DEVICE_MAPPER && !is_spectra){
         eventMap[EC_LEFT]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)  ? 1 : 0;
         eventMap[EC_RIGHT]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ? 1 : 0;
         eventMap[EC_UP]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)    ? 1 : 0;
         eventMap[EC_DOWN]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)  ? 1 : 0;
         eventMap[EC_RETURN] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)     ? 1 : 0;
         eventMap[EC_SPACE]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)     ? 1 : 0;
         eventMap[EC_CTRL]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)     ? 1 : 0;
         eventMap[EC_GRAPH]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)     ? 1 : 0;
      }
      
      if (input_devices[0] == RETRO_DEVICE_MAPPER && is_spectra){
         eventMap[EC_JOY1_LEFT]    = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)   ? 1 : 0;
         eventMap[EC_JOY1_RIGHT]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)  ? 1 : 0;
         eventMap[EC_JOY1_UP]      = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)     ? 1 : 0;
         eventMap[EC_JOY1_DOWN]    = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)   ? 1 : 0;
         eventMap[EC_JOY1_BUTTON1] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)      ? 1 : 0;
         eventMap[EC_JOY1_BUTTON2] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)      ? 1 : 0;
         eventMap[EC_1]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)      ? 1 : 0;
         eventMap[EC_2]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)      ? 1 : 0;
         eventMap[EC_3]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R)      ? 1 : 0;
         eventMap[EC_4]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L)      ? 1 : 0;
         eventMap[EC_5]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2)     ? 1 : 0;
         eventMap[EC_6]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2)     ? 1 : 0;
         eventMap[EC_7]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3)     ? 1 : 0;
         eventMap[EC_8]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3)     ? 1 : 0;
         eventMap[EC_9]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT) ? 1 : 0;
         eventMap[EC_0]            = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)  ? 1 : 0;

         eventMap[EC_JOY2_LEFT]    = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)   ? 1 : 0;
         eventMap[EC_JOY2_RIGHT]   = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)  ? 1 : 0;
         eventMap[EC_JOY2_UP]      = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)     ? 1 : 0;
         eventMap[EC_JOY2_DOWN]    = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)   ? 1 : 0;
         eventMap[EC_JOY2_BUTTON1] = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)      ? 1 : 0;
         eventMap[EC_JOY2_BUTTON2] = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)      ? 1 : 0;
      }
   }

   ((R800*)boardInfo.cpuRef)->terminate = 0;
   boardInfo.run(boardInfo.cpuRef);   
   RETRO_PERFORMANCE_STOP(core_retro_run);

   if (!use_overscan)
      video_cb(image_buffer + 8 + (image_buffer_current_width * sizeof(uint16_t) * (12 - (msx2_dif / 2))),
         image_buffer_current_width - 16, image_buffer_height - 48 + (msx2_dif * 2), image_buffer_current_width * sizeof(uint16_t));
   else
      video_cb(image_buffer, image_buffer_current_width, image_buffer_height, image_buffer_current_width * sizeof(uint16_t));

}

/* framebuffer */

uint16_t* frameBufferGetLine(FrameBuffer* frameBuffer, int y)
{
   return (image_buffer + y * image_buffer_current_width);
}

FrameBuffer* frameBufferGetDrawFrame()
{
   return (void*)image_buffer;
}
FrameBuffer* frameBufferFlipDrawFrame()
{
   return (void*)image_buffer;
}

static int fbScanLine = 0;
void frameBufferSetScanline(int scanline)
{
   fbScanLine = scanline;
}

int frameBufferGetScanline() {
   return fbScanLine;
}

FrameBufferData* frameBufferDataCreate(int maxWidth, int maxHeight, int defaultHorizZoom)
{
   return (void*)image_buffer;
}

FrameBufferData* frameBufferGetActive()
{
   return (void*)image_buffer;
}

void   frameBufferSetLineCount(FrameBuffer* frameBuffer, int val)
{
   image_buffer_height = val;
}

int    frameBufferGetLineCount(FrameBuffer* frameBuffer) {
   return image_buffer_height;
}

int    frameBufferGetMaxWidth(FrameBuffer* frameBuffer)
{
   return FB_MAX_LINE_WIDTH;
}

int    frameBufferGetDoubleWidth(FrameBuffer* frameBuffer, int y)
{
   return double_width;
}

void   frameBufferSetDoubleWidth(FrameBuffer* frameBuffer, int y, int val)
{
   double_width = val;
   image_buffer_current_width = double_width ? image_buffer_base_width * 2 : image_buffer_base_width;
}

/* libretro stubs */
void retro_set_audio_sample(retro_audio_sample_t unused) {}
bool retro_load_game_special(unsigned a, const struct retro_game_info *b, size_t c){return false;}

void retro_unload_game(void)
{
   if (image_buffer)
      free(image_buffer);
   
   if (properties)
      propDestroy(properties);

   image_buffer = NULL;
   image_buffer_base_width = 0;
   image_buffer_current_width = 0;
   image_buffer_height = 0;
}
unsigned retro_get_region(void){return RETRO_REGION_NTSC;}
void *retro_get_memory_data(unsigned id)
{
   if ( id == RETRO_MEMORY_SYSTEM_RAM )
      return g_mainRam;
   return NULL;
}
size_t retro_get_memory_size(unsigned id)
{
   if ( id == RETRO_MEMORY_SYSTEM_RAM )
      return g_mainRamSize;
   return 0;
}
unsigned retro_api_version(void){return RETRO_API_VERSION;}
size_t retro_serialize_size(void){return 1<<21;}
void retro_cheat_reset(void){}
void retro_cheat_set(unsigned a, bool b, const char * c){}
static int n_serialized = 0;

bool retro_serialize(void *data, size_t size)
{
   int c;
   MemZipFile * memZipFile;
   size_t zip_size = 0, sz;
   char * files;
   MemFile * memFile;

   boardSaveState("mem0",0);
   memZipFile = memZipFileFind("mem0");
   sz         = sizeof(memZipFile->count); 
   memcpy(data, & memZipFile->count, sz); 
   data       = (char*)data + sz;

   for (c = 0;c<memZipFile->count;c++)
   {
      memFile   = memZipFile->memFiles[c];
      zip_size += sizeof(MemFile) + memFile->size;
      sz        = sizeof(memFile->filename); 
      memcpy(data, memFile->filename, sz); 
      data      = (char*)data + sz;
      sz        = sizeof(memFile->size);     
      memcpy(data, &memFile->size, sz); 
      data      = (char*)data + sz;
      sz        = memFile->size;
      memcpy(data, memFile->buffer, sz); 
      data      = (char*)data + sz;
   }

   memZipFileDestroy(memZipFile);
   return true;
}

bool retro_unserialize(const void *data, size_t size)
{
   int c;
   int sz, count = * (int *) data;
   char  * filename;
   data = (char*)data + sizeof(int); 
   for (c = 0; c < count; c++)
   {
      filename = (char *)data;
      /* TODO/FIXME - not C89 compatible - error C2059 MSVC */
      data     = (char*)data + sizeof((MemFile){0}.filename); 
      sz       = * (int *)data;
      data     = (char*)data + sizeof(int); 
      zipSaveFile("mem0", filename, 1, data, sz );
      data     = (char*)data + sz;
   } 
   saveStateCreateForRead("mem0");
   boardInfo.loadState();
   memZipFileDestroy(memZipFileFind("mem0"));
   return true;}

/* Core stubs */
int archPollEvent(){return 0;}
void frameBufferDataDestroy(FrameBufferData* frameData){}
void frameBufferSetActive(FrameBufferData* frameData){}
void frameBufferSetMixMode(FrameBufferMixMode mode, FrameBufferMixMode mask){}
void frameBufferClearDeinterlace(){}
void frameBufferSetInterlace(FrameBuffer* frameBuffer, int val){}
void archTrap(UInt8 value){}
void videoUpdateAll(Video* video, Properties* properties){}

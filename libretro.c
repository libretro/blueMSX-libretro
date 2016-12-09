#include "libretro.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



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
   RETROK_UNKNOWN,         //EC_BKSLASH  24
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
   RETROK_LEFTBRACKET,     //EC_AT       37
   RETROK_RIGHTBRACKET,    //EC_LBRACK   38
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
   RETROK_COLON,           //EC_COLON    51
   RETROK_BACKSLASH,       //EC_RBRACK   52

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
   RETROK_UNKNOWN,         //EC_UNDSCRE  64
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

   RETROK_UNKNOWN,   //EC_COLECO1_0    120
   RETROK_UNKNOWN,   //EC_COLECO1_1    121
   RETROK_UNKNOWN,   //EC_COLECO1_2    122
   RETROK_UNKNOWN,   //EC_COLECO1_3    123
   RETROK_UNKNOWN,   //EC_COLECO1_4    124
   RETROK_UNKNOWN,   //EC_COLECO1_5    125
   RETROK_UNKNOWN,   //EC_COLECO1_6    126
   RETROK_UNKNOWN,   //EC_COLECO1_7    127
   RETROK_UNKNOWN,   //EC_COLECO1_8    128
   RETROK_UNKNOWN,   //EC_COLECO1_9    129
   RETROK_UNKNOWN,   //EC_COLECO1_STAR 130
   RETROK_UNKNOWN,   //EC_COLECO1_HASH 131

   RETROK_UNKNOWN,   //EC_COLECO2_0    140
   RETROK_UNKNOWN,   //EC_COLECO2_1    141
   RETROK_UNKNOWN,   //EC_COLECO2_2    142
   RETROK_UNKNOWN,   //EC_COLECO2_3    143
   RETROK_UNKNOWN,   //EC_COLECO2_4    144
   RETROK_UNKNOWN,   //EC_COLECO2_5    145
   RETROK_UNKNOWN,   //EC_COLECO2_6    146
   RETROK_UNKNOWN,   //EC_COLECO2_7    147
   RETROK_UNKNOWN,   //EC_COLECO2_8    148
   RETROK_UNKNOWN,   //EC_COLECO2_9    149
   RETROK_UNKNOWN,   //EC_COLECO2_STAR 150
   RETROK_UNKNOWN,   //EC_COLECO2_HASH 151
};


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
static int double_width;

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
   info->valid_extensions = "rom|ri|mx1|mx2|col|sg|sc";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->geometry.base_width = image_buffer_base_width ;
   info->geometry.base_height = image_buffer_height ;
   info->geometry.max_width = FB_MAX_LINE_WIDTH ;
   info->geometry.max_height = FB_MAX_LINES ;
   info->geometry.aspect_ratio = 0;
   info->timing.fps = 60.0;
   info->timing.sample_rate = 44100.0;
}

#ifdef LOG_PERFORMANCE
static struct retro_perf_callback perf_cb;
#endif

void retro_init(void)
{
   int i;
   struct retro_log_callback log;


   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else
      log_cb = NULL;

   image_buffer = malloc(FB_MAX_LINE_WIDTH*FB_MAX_LINES*sizeof(uint16_t));
   image_buffer_base_width =  272;
   image_buffer_current_width =  image_buffer_base_width;
   image_buffer_height =  240;
   double_width = 0;

   input_devices[0] = RETRO_DEVICE_JOYPAD;
//   for (i = 0; i < MAX_PADS; i++)
//      input_devices[i] = RETRO_DEVICE_JOYPAD;

#ifdef LOG_PERFORMANCE
   environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb);
#endif

}

void retro_deinit(void)
{
   if (image_buffer)
      free(image_buffer);

   image_buffer = NULL;
   image_buffer_base_width = 0;
   image_buffer_current_width = 0;
   image_buffer_height = 0;

#ifdef LOG_PERFORMANCE
   perf_cb.perf_log();
#endif
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   static const struct retro_variable vars[] = {
      { "bluemsx_msxtype", "Machine Type (Restart); MSX2+|MSXturboR|MSX" },
      { "bluemsx_vdp_synctype", "VDP Sync Type (Restart); Auto|50Hz|60Hz" },
      { "bluemsx_ym2413_enable", "Sound YM2413 Enable (Restart); enabled|disabled" },
      { NULL, NULL },
   };

   static const struct retro_controller_description port[] = {
      { "RetroPad", RETRO_DEVICE_JOYPAD },
      { "RetroKeyboard", RETRO_DEVICE_KEYBOARD },
   };

   static const struct retro_controller_info ports[] = {
      { port, 2 },
      { port, 2 },
      { 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t unused) { }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   switch (device)
   {
      case RETRO_DEVICE_JOYPAD:
         input_devices[port] = RETRO_DEVICE_JOYPAD;
         break;
      case RETRO_DEVICE_KEYBOARD:
         input_devices[port] = RETRO_DEVICE_KEYBOARD;
         break;
      default:
         if (log_cb)
            log_cb(RETRO_LOG_ERROR, "[libretro]: Invalid device, setting type to RETRO_DEVICE_KEYBOARD ...\n");
         input_devices[port] = RETRO_DEVICE_KEYBOARD;
   }
}

void retro_reset(void)
{
   actionEmuResetSoft();
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data, size_t size)
{
   (void)data;
   (void)size;
   return false;
}

bool retro_unserialize(const void *data, size_t size)
{
   (void)data;
   (void)size;
   return false;
}

void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned a, bool b, const char * c) {}


static void extract_directory(char *buf, const char *path, size_t size)
{
   strncpy(buf, path, size - 1);
   buf[size - 1] = '\0';

   char *base = strrchr(buf, '/');
   if (!base)
      base = strrchr(buf, '\\');

   if (base)
      *base = '\0';
   else
      buf[0] = '\0';
}

static char msx_type[256];
static unsigned msx_vdp_synctype;
static bool msx_ym2413_enable;

static void check_variables(void)
{
   struct retro_variable var;
   var.key = "bluemsx_msxtype";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      strcpy(msx_type, var.value);
   else
      strcpy(msx_type, "MSX2+");

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
}

bool retro_load_game(const struct retro_game_info *info)
{
   int i;
   char properties_dir[256], machines_dir[256], mediadb_dir[256];
   const char *dir = NULL;
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
#ifdef _WIN32
   char slash = '\\';
#else
   char slash = '/';
#endif

   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      if (log_cb)
         log_cb(RETRO_LOG_INFO, "RGB565 is not supported.\n");
      return false;
   }

   check_variables();

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
      strcpy(properties_dir, dir);
   else /* Fallback */
      extract_directory(properties_dir, info->path, sizeof(properties_dir));

   snprintf(machines_dir, sizeof(machines_dir), "%s%c%s", properties_dir, slash, "Machines");
   snprintf(mediadb_dir, sizeof(mediadb_dir), "%s%c%s", properties_dir, slash, "Databases");

   propertiesSetDirectory(properties_dir, properties_dir);
   machineSetDirectory(machines_dir);
   //boardSetDirectory(buffer);
   mediaDbLoad(mediadb_dir);
   //mediaDbCreateRomdb();
   //mediaDbCreateDiskdb();
   //mediaDbCreateCasdb();

   properties = propCreate(1, EMU_LANG_ENGLISH, P_KBD_EUROPEAN, P_EMU_SYNCNONE, "");

   strcpy(properties->joy1.type, "joystick");
   properties->joy1.typeId            = JOYSTICK_PORT_JOYSTICK;
   properties->joy1.autofire          = 0;

   strcpy(properties->joy2.type, "joystick");
   properties->joy2.typeId            = JOYSTICK_PORT_JOYSTICK;
   properties->joy2.autofire          = 0;

   properties->emulation.vdpSyncMode       = msx_vdp_synctype;
   properties->video.monitorType           = P_VIDEO_PALNONE;
   strcpy(properties->emulation.machineName, msx_type);

   properties->sound.chip.enableYM2413 = msx_ym2413_enable;

   mixer = mixerCreate();

   emulatorInit(properties, mixer);
   actionInit(video, properties, mixer);
   langInit();
   tapeSetReadOnly(properties->cassette.readOnly);

   langSetLanguage(properties->language);

   joystickPortSetType(0, properties->joy1.typeId);
   joystickPortSetType(1, properties->joy2.typeId);



//   printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
//   printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
//   uartIoSetType(properties->ports.Com.type, properties->ports.Com.fileName);
//   midiIoSetMidiOutType(properties->sound.MidiOut.type, properties->sound.MidiOut.fileName);
//   midiIoSetMidiInType(properties->sound.MidiIn.type, properties->sound.MidiIn.fileName);
//   ykIoSetMidiInType(properties->sound.YkIn.type, properties->sound.YkIn.fileName);

   emulatorRestartSound();

   for (i = 0; i < MIXER_CHANNEL_TYPE_COUNT; i++)
   {
      mixerSetChannelTypeVolume(mixer, i, properties->sound.mixerChannel[i].volume);
      mixerSetChannelTypePan(mixer, i, properties->sound.mixerChannel[i].pan);
      mixerEnableChannelType(mixer, i, properties->sound.mixerChannel[i].enable);
   }

   mixerSetMasterVolume(mixer, properties->sound.masterVolume);
   mixerEnableMaster(mixer, properties->sound.masterEnable);


   mediaDbSetDefaultRomType(properties->cartridge.defaultType);

   strcpy(properties->media.carts[0].fileName , info->path);

   for (i = 0; i < PROP_MAX_CARTS; i++)
   {
      if (properties->media.carts[i].fileName[0])
         insertCartridge(properties, i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip, properties->media.carts[i].type, -1);
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


bool retro_load_game_special(unsigned a, const struct retro_game_info *b, size_t c)
{
   return false;
}

void retro_unload_game(void)
{
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

void *retro_get_memory_data(unsigned id)
{
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   return 0;
}

void timerCallback_global(void* timer);

UInt8 archJoystickGetState(int joystickNo)
{
   return ((eventMap[EC_JOY1_UP]      << 0) |
           (eventMap[EC_JOY1_DOWN]    << 1) |
           (eventMap[EC_JOY1_LEFT]    << 2) |
           (eventMap[EC_JOY1_RIGHT]   << 3) |
           (eventMap[EC_JOY1_BUTTON1] << 4) |
           (eventMap[EC_JOY1_BUTTON2] << 5) |
           (eventMap[EC_JOY1_BUTTON3] << 6) |
           (eventMap[EC_JOY1_BUTTON4] << 7));
}

#define EC_KEYBOARD_KEYCOUNT  94

#ifdef LOG_PERFORMANCE
#define RETRO_PERFORMANCE_INIT(name) static struct retro_perf_counter name = {#name}; if (!name.registered) perf_cb.perf_register(&(name))
#define RETRO_PERFORMANCE_START(name) perf_cb.perf_start(&(name))
#define RETRO_PERFORMANCE_STOP(name) perf_cb.perf_stop(&(name))
#else
#define RETRO_PERFORMANCE_INIT(name)
#define RETRO_PERFORMANCE_START(name)
#define RETRO_PERFORMANCE_STOP(name)
#endif

void retro_run(void)
{
   int i,j;
   bool updated = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();

   RETRO_PERFORMANCE_INIT(core_retro_run);
   RETRO_PERFORMANCE_START(core_retro_run);

   input_poll_cb();

#ifdef PSP
   if(input_devices[0] == RETRO_DEVICE_JOYPAD)
      for (j = EC_JOY1_UP; j <= (EC_JOY1_BUTTON6); j++)
         eventMap[j] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, btn_map[j]) ? 1 : 0;
   else
   {
      eventMap[EC_LEFT]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)  ? 1 : 0;
      eventMap[EC_RIGHT]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ? 1 : 0;
      eventMap[EC_UP]     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)    ? 1 : 0;
      eventMap[EC_DOWN]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)  ? 1 : 0;
      eventMap[EC_RETURN] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)     ? 1 : 0;
      eventMap[EC_SPACE]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)     ? 1 : 0;
      eventMap[EC_CTRL]   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)     ? 1 : 0;
      eventMap[EC_GRAPH]  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)     ? 1 : 0;
   }
#else
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
         case RETRO_DEVICE_KEYBOARD:
            for (j = 0; j < EC_KEYBOARD_KEYCOUNT; j++)
               eventMap[j] = input_state_cb(i, RETRO_DEVICE_KEYBOARD, 0, btn_map[j]) ? 1 : 0;
            break;
      }
   }
#endif

   ((R800*)boardInfo.cpuRef)->terminate = 0;
   boardInfo.run(boardInfo.cpuRef);

   RETRO_PERFORMANCE_STOP(core_retro_run);

   video_cb(image_buffer, image_buffer_current_width, image_buffer_height, image_buffer_current_width * sizeof(uint16_t));


}

unsigned retro_api_version(void) { return RETRO_API_VERSION; }


// framebuffer

uint16_t* frameBufferGetLine(FrameBuffer* frameBuffer, int y) {
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

void frameBufferDataDestroy(FrameBufferData* frameData)
{

}

void frameBufferSetActive(FrameBufferData* frameData)
{

}
void frameBufferSetMixMode(FrameBufferMixMode mode, FrameBufferMixMode mask)
{

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

void frameBufferClearDeinterlace()
{

}
void   frameBufferSetInterlace(FrameBuffer* frameBuffer, int val)
{

}

void archTrap(UInt8 value)
{
}

int archPollEvent()
{
   return 0;
}

void videoUpdateAll(Video* video, Properties* properties)
{

}

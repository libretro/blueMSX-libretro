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


static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;

static Properties* properties;
static Video* video;
static Mixer* mixer;


static uint16_t* image_buffer;
static unsigned image_buffer_width;
static unsigned image_buffer_height;


void retro_get_system_info(struct retro_system_info *info)
{
   info->library_name = "blueMSX";
   info->library_version = "v0.0.1";
   info->need_fullpath = true;
   info->block_extract = false;
   info->valid_extensions = "rom";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->geometry.base_width = image_buffer_width ;
   info->geometry.base_height = image_buffer_height ;
   info->geometry.base_width = 272 ;
   info->geometry.base_height = 240 ;
   info->geometry.max_width = FB_MAX_LINE_WIDTH ;
   info->geometry.max_height = FB_MAX_LINES ;
   info->geometry.aspect_ratio = 0;
   info->timing.fps = 60.0;
   info->timing.sample_rate = 44100.0;
}

void retro_init(void)
{
   struct retro_log_callback log;

   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else
      log_cb = NULL;

   image_buffer = NULL;
   image_buffer_width = 0;
   image_buffer_height = 0;

}

void retro_deinit(void)
{
   if (image_buffer)
      free(image_buffer);
   image_buffer = NULL;
   image_buffer_width = 0;
   image_buffer_height = 0;
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   static const struct retro_variable vars[] = {
      { NULL, NULL },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
}

void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t unused) { }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_set_controller_port_device(unsigned a, unsigned b) {}

void retro_reset(void)
{
   if (image_buffer)
      free(image_buffer);
   image_buffer = NULL;
   image_buffer_width = 0;
   image_buffer_height = 0;
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

bool retro_load_game(const struct retro_game_info *info)
{
   int i;

//   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_0RGB1555;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      if (log_cb)
         log_cb(RETRO_LOG_INFO, "RGB1555 is not supported.\n");
      return false;
   }


   properties = propCreate(1, 0, P_KBD_EUROPEAN, 0, "");

   video = videoCreate();
   videoSetColors(video, properties->video.saturation, properties->video.brightness,
                 properties->video.contrast, properties->video.gamma);
   videoSetScanLines(video, properties->video.scanlinesEnable, properties->video.scanlinesPct);
   videoSetColorSaturation(video, properties->video.colorSaturationEnable, properties->video.colorSaturationWidth);

   mixer = mixerCreate();

   emulatorInit(properties, mixer);
   actionInit(video, properties, mixer);
   langInit();
   tapeSetReadOnly(properties->cassette.readOnly);

   langSetLanguage(properties->language);

   joystickPortSetType(0, properties->joy1.typeId);
   joystickPortSetType(1, properties->joy2.typeId);

   printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
   printerIoSetType(properties->ports.Lpt.type, properties->ports.Lpt.fileName);
   uartIoSetType(properties->ports.Com.type, properties->ports.Com.fileName);
   midiIoSetMidiOutType(properties->sound.MidiOut.type, properties->sound.MidiOut.fileName);
   midiIoSetMidiInType(properties->sound.MidiIn.type, properties->sound.MidiIn.fileName);
   ykIoSetMidiInType(properties->sound.YkIn.type, properties->sound.YkIn.fileName);

   emulatorRestartSound();

   for (i = 0; i < MIXER_CHANNEL_TYPE_COUNT; i++) {
       mixerSetChannelTypeVolume(mixer, i, properties->sound.mixerChannel[i].volume);
       mixerSetChannelTypePan(mixer, i, properties->sound.mixerChannel[i].pan);
       mixerEnableChannelType(mixer, i, properties->sound.mixerChannel[i].enable);
   }

   mixerSetMasterVolume(mixer, properties->sound.masterVolume);
   mixerEnableMaster(mixer, properties->sound.masterEnable);

   videoUpdateAll(video, properties);

   mediaDbSetDefaultRomType(properties->cartridge.defaultType);


   strcpy(properties->media.carts[0].fileName , info->path);

   for (i = 0; i < PROP_MAX_CARTS; i++) {
       if (properties->media.carts[i].fileName[0]) insertCartridge(properties, i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip, properties->media.carts[i].type, -1);
       updateExtendedRomName(i, properties->media.carts[i].fileName, properties->media.carts[i].fileNameInZip);
   }

   for (i = 0; i < PROP_MAX_DISKS; i++) {
       if (properties->media.disks[i].fileName[0]) insertDiskette(properties, i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip, -1);
       updateExtendedDiskName(i, properties->media.disks[i].fileName, properties->media.disks[i].fileNameInZip);
   }

   for (i = 0; i < PROP_MAX_TAPES; i++) {
       if (properties->media.tapes[i].fileName[0]) insertCassette(properties, i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip, 0);
       updateExtendedCasName(i, properties->media.tapes[i].fileName, properties->media.tapes[i].fileNameInZip);
   }

   {
       Machine* machine = machineCreate(properties->emulation.machineName);
       if (machine != NULL) {
           boardSetMachine(machine);
           machineDestroy(machine);
       }
   }
   boardSetFdcTimingEnable(properties->emulation.enableFdcTiming);
   boardSetY8950Enable(properties->sound.chip.enableY8950);
   boardSetYm2413Enable(properties->sound.chip.enableYM2413);
   boardSetMoonsoundEnable(properties->sound.chip.enableMoonsound);
   boardSetVideoAutodetect(properties->video.detectActiveMonitor);

   image_buffer = malloc(FB_MAX_LINE_WIDTH*FB_MAX_LINES*sizeof(uint16_t));
   image_buffer_width =  272;
   image_buffer_height =  240;

   emulatorStart(NULL);





   return true;
}


bool retro_load_game_special(unsigned a, const struct retro_game_info *b, size_t c) { return false; }

void retro_unload_game(void)
{
}

   unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }

void *retro_get_memory_data(unsigned id)
{
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   return 0;
}

void timerCallback(void* timer) ;
#include "InputEvent.h"
void retro_run(void)
{

   int i,j;
   input_poll_cb();

   extern int eventMap[256];

   static int btn_map[256];
   btn_map[RETRO_DEVICE_ID_JOYPAD_B]      = EC_SPACE;
   btn_map[RETRO_DEVICE_ID_JOYPAD_Y]      = EC_CTRL;
   btn_map[RETRO_DEVICE_ID_JOYPAD_SELECT] = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_START]  = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_UP]     = EC_UP;
   btn_map[RETRO_DEVICE_ID_JOYPAD_DOWN]   = EC_DOWN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_LEFT]   = EC_LEFT;
   btn_map[RETRO_DEVICE_ID_JOYPAD_RIGHT]  = EC_RIGHT;
   btn_map[RETRO_DEVICE_ID_JOYPAD_A]      = EC_LSHIFT;
   btn_map[RETRO_DEVICE_ID_JOYPAD_X]      = EC_GRAPH;
   btn_map[RETRO_DEVICE_ID_JOYPAD_L]      = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_R]      = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_L2]     = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_R2]     = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_L3]     = EC_RETURN;
   btn_map[RETRO_DEVICE_ID_JOYPAD_R3]     = EC_RETURN;

   for (i=0; i<16; i++)
      eventMap[btn_map[i]] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i);


   timerCallback(NULL);
//   emulatorRunOne();

   FrameBuffer* frameBuffer;
   frameBuffer = frameBufferFlipViewFrame(0);
   if (frameBuffer == NULL) {
       frameBuffer = frameBufferGetWhiteNoiseFrame();
   }


//   if ((frameBuffer->maxWidth != image_buffer_width)||(frameBuffer->lines != image_buffer_height))
//   {
//      image_buffer_width  = frameBuffer->maxWidth;
//      image_buffer_height = frameBuffer->lines;

//      static struct retro_system_av_info new_av_info;
//      environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &new_av_info);
//   }



   if (frameBuffer->line[0].doubleWidth)
      for (i=0;i<frameBuffer->lines;i++)
         for (j=0;j<frameBuffer->maxWidth;j++)
            image_buffer[j + i * frameBuffer->maxWidth]= frameBuffer->line[i].buffer[j<<1];
   else
      for (i=0;i<frameBuffer->lines;i++)
         memcpy(image_buffer + (i*frameBuffer->maxWidth), frameBuffer->line[i].buffer, frameBuffer->maxWidth * sizeof(uint16_t));

   video_cb(image_buffer,frameBuffer->maxWidth,frameBuffer->lines,frameBuffer->maxWidth * sizeof(uint16_t));

}

unsigned retro_api_version(void) { return RETRO_API_VERSION; }


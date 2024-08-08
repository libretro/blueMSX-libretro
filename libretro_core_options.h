#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include <libretro.h>
#include <retro_inline.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 ********************************
 * Core Option Definitions
 ********************************
*/

/* RETRO_LANGUAGE_ENGLISH */

/* Default language:
 * - All other languages must include the same keys and values
 * - Will be used as a fallback in the event that frontend language
 *   is not available
 * - Will be used as a fallback for any missing entries in
 *   frontend language definition */

struct retro_core_option_definition option_defs_us[] = {
   {
      "bluemsx_msxtype",
      "Machine Type (Restart)",
      "Manually select the machine type you would like the core to start in.",
      {
         { "Auto",   NULL },
         { "MSX",   NULL },
         { "MSXturboR",   NULL },
         { "MSX2",   NULL },
         { "MSX2+",   NULL },
         { "SEGA - SG-1000",   NULL },
         { "SEGA - SC-3000",   NULL },
         { "SEGA - SF-7000",   NULL },
         { "SVI - Spectravideo SVI-318",   NULL },
         { "SVI - Spectravideo SVI-328",   NULL },
         { "SVI - Spectravideo SVI-328 MK2", NULL },
         { "ColecoVision", NULL },
         { "Coleco (Spectravideo SVI-603)", NULL },
         { NULL, NULL },
      },
      "Auto"
   },
   {
      "bluemsx_overscan",
      "Crop Overscan",
      "Forces cropping of overscanned frames",
      {
         { "disabled",   NULL },
         { "enabled",   NULL },
         { "MSX2",   NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "bluemsx_vdp_synctype",
      "VDP Sync Type (Restart)",
      "Match the game/machine region frequency to avoid emulated speed issues.",
      {
         { "Auto",   NULL },
         { "50Hz",   NULL },
         { "60Hz",   NULL },
         { NULL, NULL },
      },
      "Auto"
   },
   {
      "bluemsx_nospritelimits",
      "No Sprite Limit",
      "Remove the 4 sprite per line limit which can reduce or remove sprite flicker in some games.",
      {
         { "OFF",   NULL },
         { "ON",   NULL },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bluemsx_ym2413_enable",
      "Sound YM2413 Enable (Restart)",
      "Awaiting description.",
      {
         { "enabled",   NULL },
         { "disabled",   NULL },
         { NULL, NULL },
      },
      "enabled"
   },
   {
      "bluemsx_cartmapper",
      "Cart Mapper Type (Restart)",
      "When a ROM game or application is in the database, the emulator uses the databases to apply the correct mapper. If the sha1 value of a dump is not yet in the databases, it uses an automatic mapper detection system, but it can fail in some cases. In this situation, you can manually select the correct mapper.",
      {
         { "Auto",   NULL },
         { "Normal",   NULL },
         { "mirrored",   NULL },
         { "basic",   NULL },
         { "0x4000",   NULL },
         { "0xC000",   NULL },
         { "ascii8",   NULL },
         { "ascii8sram",   NULL },
         { "ascii16",   NULL },
         { "ascii16sram",   NULL },
         { "ascii16nf",   NULL },
         { "konami4",   NULL },
         { "konami4nf",   NULL },
         { "konami5",   NULL },
         { "konamisynth",   NULL },
         { "korean80",   NULL },
         { "korean90",   NULL },
         { "korean126",   NULL },
         { "MegaFlashRomScc",   NULL },
         { "MegaFlashRomSccPlus",   NULL },
         { "msxdos2",   NULL },
         { "scc",   NULL },
         { "sccexpanded",   NULL },
         { "sccmirrored",   NULL },
         { "sccplus",   NULL },
         { "snatcher",   NULL },
         { "sdsnatcher",   NULL },
         { "SegaBasic",   NULL },
         { "SG1000",   NULL },
         { "SG1000Castle",   NULL },
         { "SG1000RamA",   NULL },
         { "SG1000RamB",   NULL },
         { "SC3000",   NULL },
         { NULL, NULL },
      },
      "Auto"
   },
   {
      "bluemsx_auto_rewind_cas",
      "Auto Rewind Cassette",
      "",
      {
         { "ON",   NULL },
         { "OFF",   NULL },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bluemsx_sound_io_enable",
      "I/O Sound Enable",
      "Enable the I/O sound (floppy disk access sound).",
      {
         { "enabled",   NULL },
         { "disabled",   NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   { NULL, NULL, NULL, {{0}}, NULL },
};

/* RETRO_LANGUAGE_JAPANESE */

/* RETRO_LANGUAGE_FRENCH */

/* RETRO_LANGUAGE_SPANISH */

/* RETRO_LANGUAGE_GERMAN */

/* RETRO_LANGUAGE_ITALIAN */

/* RETRO_LANGUAGE_DUTCH */

/* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */

/* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */

/* RETRO_LANGUAGE_RUSSIAN */

/* RETRO_LANGUAGE_KOREAN */

/* RETRO_LANGUAGE_CHINESE_TRADITIONAL */

/* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */

/* RETRO_LANGUAGE_ESPERANTO */

/* RETRO_LANGUAGE_POLISH */

/* RETRO_LANGUAGE_VIETNAMESE */

/* RETRO_LANGUAGE_ARABIC */

/* RETRO_LANGUAGE_GREEK */

/* RETRO_LANGUAGE_TURKISH */

/*
 ********************************
 * Language Mapping
 ********************************
*/

struct retro_core_option_definition *option_defs_intl[RETRO_LANGUAGE_LAST] = {
   option_defs_us, /* RETRO_LANGUAGE_ENGLISH */
   NULL,           /* RETRO_LANGUAGE_JAPANESE */
   NULL,           /* RETRO_LANGUAGE_FRENCH */
   NULL,           /* RETRO_LANGUAGE_SPANISH */
   NULL,           /* RETRO_LANGUAGE_GERMAN */
   NULL,           /* RETRO_LANGUAGE_ITALIAN */
   NULL,           /* RETRO_LANGUAGE_DUTCH */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
   NULL,           /* RETRO_LANGUAGE_RUSSIAN */
   NULL,           /* RETRO_LANGUAGE_KOREAN */
   NULL,           /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
   NULL,           /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
   NULL,           /* RETRO_LANGUAGE_ESPERANTO */
   NULL,           /* RETRO_LANGUAGE_POLISH */
   NULL,           /* RETRO_LANGUAGE_VIETNAMESE */
   NULL,           /* RETRO_LANGUAGE_ARABIC */
   NULL,           /* RETRO_LANGUAGE_GREEK */
   NULL,           /* RETRO_LANGUAGE_TURKISH */
};

/*
 ********************************
 * Functions
 ********************************
*/

/* Handles configuration/setting of core options.
 * Should be called as early as possible - ideally inside
 * retro_set_environment(), and no later than retro_load_game()
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb)
{
   unsigned version = 0;

   if (!environ_cb)
      return;

   if (environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version) && (version == 1))
   {
      struct retro_core_options_intl core_options_intl;
      unsigned language = 0;

      core_options_intl.us    = option_defs_us;
      core_options_intl.local = NULL;

      if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
          (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
         core_options_intl.local = option_defs_intl[language];

      environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_intl);
   }
   else
   {
      size_t i;
      size_t num_options               = 0;
      struct retro_variable *variables = NULL;
      char **values_buf                = NULL;

      /* Determine number of options */
      while (true)
      {
         if (option_defs_us[num_options].key)
            num_options++;
         else
            break;
      }

      /* Allocate arrays */
      variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
      values_buf = (char **)calloc(num_options, sizeof(char *));

      if (!variables || !values_buf)
         goto error;

      /* Copy parameters from option_defs_us array */
      for (i = 0; i < num_options; i++)
      {
         const char *key                        = option_defs_us[i].key;
         const char *desc                       = option_defs_us[i].desc;
         const char *default_value              = option_defs_us[i].default_value;
         struct retro_core_option_value *values = option_defs_us[i].values;
         size_t buf_len                         = 3;
         size_t default_index                   = 0;

         values_buf[i] = NULL;

         if (desc)
         {
            size_t num_values = 0;

            /* Determine number of values */
            while (true)
            {
               if (values[num_values].value)
               {
                  /* Check if this is the default value */
                  if (default_value)
                     if (strcmp(values[num_values].value, default_value) == 0)
                        default_index = num_values;

                  buf_len += strlen(values[num_values].value);
                  num_values++;
               }
               else
                  break;
            }

            /* Build values string */
            if (num_values > 1)
            {
               size_t j;

               buf_len += num_values - 1;
               buf_len += strlen(desc);

               values_buf[i] = (char *)calloc(buf_len, sizeof(char));
               if (!values_buf[i])
                  goto error;

               strcpy(values_buf[i], desc);
               strcat(values_buf[i], "; ");

               /* Default value goes first */
               strcat(values_buf[i], values[default_index].value);

               /* Add remaining values */
               for (j = 0; j < num_values; j++)
               {
                  if (j != default_index)
                  {
                     strcat(values_buf[i], "|");
                     strcat(values_buf[i], values[j].value);
                  }
               }
            }
         }

         variables[i].key   = key;
         variables[i].value = values_buf[i];
      }
      
      /* Set variables */
      environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);

error:

      /* Clean up */
      if (values_buf)
      {
         for (i = 0; i < num_options; i++)
         {
            if (values_buf[i])
            {
               free(values_buf[i]);
               values_buf[i] = NULL;
            }
         }

         free(values_buf);
         values_buf = NULL;
      }

      if (variables)
      {
         free(variables);
         variables = NULL;
      }
   }
}

#ifdef __cplusplus
}
#endif

#endif

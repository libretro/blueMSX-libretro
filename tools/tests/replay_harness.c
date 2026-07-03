/* Deterministic replay harness for bluemsx_libretro.
 *
 * A minimal libretro frontend: loads the core, boots the bundled
 * self-contained "MSX2 - C-BIOS" machine with the deterministic RTC
 * option and a fixed test cartridge, runs N frames with no input, and
 * CRC32s every video frame and audio batch.  The final digest must be
 * identical across runs, hosts, compilers and architectures.
 *
 * Build (from the repository root):
 *   gcc -O2 -o /tmp/replay tools/tests/replay_harness.c -ldl
 *
 * Run:
 *   /tmp/replay ./bluemsx_libretro.so ./system/bluemsx \
 *               tools/tests/testcart.rom 600
 *
 * Prints one line per 100 frames plus a final combined digest:
 *   FINAL video=xxxxxxxx audio=xxxxxxxx frames=N samples=M
 */
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* --- minimal libretro declarations (subset of libretro.h) --- */
#define RETRO_API_VERSION 1
#define RETRO_ENVIRONMENT_SET_PIXEL_FORMAT 10
#define RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY 9
#define RETRO_ENVIRONMENT_GET_VARIABLE 15
#define RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE 17
#define RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY 31
#define RETRO_ENVIRONMENT_GET_LOG_INTERFACE 27

struct retro_game_info { const char* path; const void* data; size_t size; const char* meta; };
struct retro_game_geometry { unsigned base_width, base_height, max_width, max_height; float aspect_ratio; };
struct retro_system_timing { double fps, sample_rate; };
struct retro_system_av_info { struct retro_game_geometry geometry; struct retro_system_timing timing; };
struct retro_system_info { const char* library_name; const char* library_version; const char* valid_extensions; bool need_fullpath; bool block_extract; };
struct retro_variable { const char* key; const char* value; };
enum retro_log_level { RETRO_LOG_DEBUG = 0, RETRO_LOG_INFO, RETRO_LOG_WARN, RETRO_LOG_ERROR };
typedef void (*retro_log_printf_t)(enum retro_log_level level, const char* fmt, ...);
struct retro_log_callback { retro_log_printf_t log; };

typedef bool (*retro_environment_t)(unsigned cmd, void* data);
typedef void (*retro_video_refresh_t)(const void* data, unsigned width, unsigned height, size_t pitch);
typedef void (*retro_audio_sample_t)(int16_t left, int16_t right);
typedef size_t (*retro_audio_sample_batch_t)(const int16_t* data, size_t frames);
typedef void (*retro_input_poll_t)(void);
typedef int16_t (*retro_input_state_t)(unsigned port, unsigned device, unsigned index, unsigned id);

/* --- self-contained CRC32 (zlib polynomial), no external deps --- */
static uint32_t crc_table[256];
static void crc_init(void)
{
    uint32_t i, j, c;
    for (i = 0; i < 256; i++) {
        c = i;
        for (j = 0; j < 8; j++)
            c = (c & 1) ? 0xedb88320u ^ (c >> 1) : c >> 1;
        crc_table[i] = c;
    }
}
static uint32_t crc32_upd(uint32_t crc, const void* buf, size_t len)
{
    const uint8_t* p = (const uint8_t*)buf;
    crc ^= 0xffffffffu;
    while (len--)
        crc = crc_table[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    return crc ^ 0xffffffffu;
}

/* --- harness state --- */
static const char* g_system_dir;
static const char* g_save_dir = "/tmp/replay_saves";
static uint32_t g_video_crc = 0;
static uint32_t g_audio_crc = 0;
static uint64_t g_samples = 0;
static int g_verbose = 0;
static const char* g_rtc_mode = "fixed epoch";

static void log_cb(enum retro_log_level level, const char* fmt, ...)
{
    if (!g_verbose && level < RETRO_LOG_WARN)
        return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

static bool env_cb(unsigned cmd, void* data)
{
    switch (cmd) {
    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        return *(unsigned*)data == 2; /* RETRO_PIXEL_FORMAT_RGB565 */
    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        *(const char**)data = g_system_dir;
        return true;
    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
        *(const char**)data = g_save_dir;
        return true;
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        ((struct retro_log_callback*)data)->log = log_cb;
        return true;
    case RETRO_ENVIRONMENT_GET_VARIABLE: {
        struct retro_variable* var = (struct retro_variable*)data;
        if (!strcmp(var->key, "bluemsx_msxtype")) {
            var->value = "MSX2 - C-BIOS";   /* bundled, self-contained */
            return true;
        }
        if (!strcmp(var->key, "bluemsx_rtc_source")) {
            var->value = g_rtc_mode;
            return true;
        }
        return false;                        /* everything else: defaults */
    }
    case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
        *(bool*)data = false;
        return true;
    default:
        return false;
    }
}

static void video_cb(const void* data, unsigned width, unsigned height, size_t pitch)
{
    unsigned y;
    if (!data)
        return;
    for (y = 0; y < height; y++)
        g_video_crc = crc32_upd(g_video_crc,
                                (const uint8_t*)data + y * pitch,
                                width * 2 /* RGB565 */);
}

static void audio_cb(int16_t l, int16_t r)
{
    int16_t s[2] = { l, r };
    g_audio_crc = crc32_upd(g_audio_crc, s, sizeof(s));
    g_samples++;
}

static size_t audio_batch_cb(const int16_t* data, size_t frames)
{
    g_audio_crc = crc32_upd(g_audio_crc, data, frames * 4);
    g_samples += frames;
    return frames;
}

static void input_poll_cb(void) {}
static int16_t input_state_cb(unsigned port, unsigned device, unsigned index, unsigned id)
{
    (void)port; (void)device; (void)index; (void)id;
    return 0;   /* no input: fully scripted (empty) input stream */
}

int main(int argc, char** argv)
{
    if (argc < 5) {
        fprintf(stderr, "usage: %s <core.so> <system_dir> <rom> <frames>"
                        " [--rtc=fixed|host] [-v]\n", argv[0]);
        return 2;
    }
    crc_init();
    const char* core_path = argv[1];
    g_system_dir = argv[2];
    const char* rom_path = argv[3];
    long frames = strtol(argv[4], NULL, 10);
    int i;
    for (i = 5; i < argc; i++) {
        if (!strcmp(argv[i], "-v"))            g_verbose = 1;
        else if (!strcmp(argv[i], "--rtc=host"))  g_rtc_mode = "host clock";
        else if (!strcmp(argv[i], "--rtc=fixed")) g_rtc_mode = "fixed epoch";
    }

    /* start from a clean save directory so battery-backed CMOS from a
     * previous run cannot leak into the comparison */
    if (system("rm -rf /tmp/replay_saves && mkdir -p /tmp/replay_saves") != 0)
        return 1;

    void* core = dlopen(core_path, RTLD_NOW | RTLD_LOCAL);
    if (!core) { fprintf(stderr, "dlopen: %s\n", dlerror()); return 1; }

#define SYM(name) __typeof__(name##_fn) name##_fn = (__typeof__(name##_fn))dlsym(core, #name)
    void (*retro_set_environment_fn)(retro_environment_t);
    void (*retro_set_video_refresh_fn)(retro_video_refresh_t);
    void (*retro_set_audio_sample_fn)(retro_audio_sample_t);
    void (*retro_set_audio_sample_batch_fn)(retro_audio_sample_batch_t);
    void (*retro_set_input_poll_fn)(retro_input_poll_t);
    void (*retro_set_input_state_fn)(retro_input_state_t);
    void (*retro_init_fn)(void);
    void (*retro_deinit_fn)(void);
    bool (*retro_load_game_fn)(const struct retro_game_info*);
    void (*retro_unload_game_fn)(void);
    void (*retro_run_fn)(void);
    unsigned (*retro_api_version_fn)(void);
    void (*retro_get_system_av_info_fn)(struct retro_system_av_info*);

    retro_set_environment_fn      = dlsym(core, "retro_set_environment");
    retro_set_video_refresh_fn    = dlsym(core, "retro_set_video_refresh");
    retro_set_audio_sample_fn     = dlsym(core, "retro_set_audio_sample");
    retro_set_audio_sample_batch_fn = dlsym(core, "retro_set_audio_sample_batch");
    retro_set_input_poll_fn       = dlsym(core, "retro_set_input_poll");
    retro_set_input_state_fn      = dlsym(core, "retro_set_input_state");
    retro_init_fn                 = dlsym(core, "retro_init");
    retro_deinit_fn               = dlsym(core, "retro_deinit");
    retro_load_game_fn            = dlsym(core, "retro_load_game");
    retro_unload_game_fn          = dlsym(core, "retro_unload_game");
    retro_run_fn                  = dlsym(core, "retro_run");
    retro_api_version_fn          = dlsym(core, "retro_api_version");
    retro_get_system_av_info_fn   = dlsym(core, "retro_get_system_av_info");

    if (!retro_run_fn || !retro_load_game_fn) {
        fprintf(stderr, "missing core symbols\n");
        return 1;
    }
    if (retro_api_version_fn() != RETRO_API_VERSION) {
        fprintf(stderr, "api version mismatch\n");
        return 1;
    }

    retro_set_environment_fn(env_cb);
    retro_init_fn();
    retro_set_video_refresh_fn(video_cb);
    retro_set_audio_sample_fn(audio_cb);
    retro_set_audio_sample_batch_fn(audio_batch_cb);
    retro_set_input_poll_fn(input_poll_cb);
    retro_set_input_state_fn(input_state_cb);

    struct retro_game_info info;
    memset(&info, 0, sizeof(info));
    info.path = rom_path;             /* core has need_fullpath = true */

    if (!retro_load_game_fn(&info)) {
        fprintf(stderr, "retro_load_game failed\n");
        return 1;
    }

    struct retro_system_av_info av;
    memset(&av, 0, sizeof(av));
    retro_get_system_av_info_fn(&av);
    fprintf(stderr, "core loaded: %ux%u @ %.4f fps, %.0f Hz audio\n",
            av.geometry.base_width, av.geometry.base_height,
            av.timing.fps, av.timing.sample_rate);

    long f;
    for (f = 1; f <= frames; f++) {
        retro_run_fn();
        if (f % 100 == 0)
            printf("frame %ld: video=%08x audio=%08x samples=%llu\n",
                   f, g_video_crc, g_audio_crc,
                   (unsigned long long)g_samples);
    }

    printf("FINAL video=%08x audio=%08x frames=%ld samples=%llu\n",
           g_video_crc, g_audio_crc, frames,
           (unsigned long long)g_samples);

    retro_unload_game_fn();
    retro_deinit_fn();
    dlclose(core);
    return 0;
}

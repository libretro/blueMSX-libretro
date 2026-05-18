/*
 * libretro-graph.h — RGB565 drawing primitives for OSK overlay
 *
 * Written for blueMSX-libretro.
 * Inspired by the rendering approach used in vice-libretro
 * (https://github.com/libretro/vice-libretro, GPL v2),
 * but implemented independently.
 *
 * Font data (libretro-font.i): ASCII printable glyphs (32-127) and cursor
 * arrow glyphs (27-30) from vice-libretro (GPL v2). Commodore/Amiga-specific
 * glyphs in positions 0-26 and 31 have been replaced with blank data.
 * RGB565 50% blend formula is from vice-libretro (GPL v2).
 */
#ifndef LIBRETRO_GRAPH_H
#define LIBRETRO_GRAPH_H

#include <stdint.h>

#define RGB565(r, g, b) \
   (((uint16_t)((r) >> 3) << 11) | ((uint16_t)((g) >> 2) << 5) | (uint16_t)((b) >> 3))

/* OSK color palette */
#define VKBD_BLACK    RGB565(  5,   5,   5)
#define VKBD_WHITE    RGB565(255, 255, 255)
#define VKBD_KEY_BG   RGB565( 75,  80, 100)   /* blend tint for key face */
#define VKBD_KEY_SEL  RGB565( 70, 130, 230)   /* blue blend tint for selected key */
#define VKBD_KEY_ACT  RGB565(255, 200,  60)   /* amber blend tint for key-press flash */
#define VKBD_KEY_STK  RGB565( 25, 170,  25)   /* green blend tint for sticky */
#define VKBD_KEY_BRD  RGB565(255, 255, 255)   /* white solid border on selected key */
#define VKBD_OSK_BG   RGB565( 15,  20,  35)   /* dark blend tint between keys */
#define VKBD_SHIFT_FG RGB565(255, 195,  30)   /* bright amber secondary labels */

/*
 * Set these from libretro.c before calling vkbd_render() each frame.
 * All draw coordinates are relative to the visible area (0,0 = top-left
 * of what video_cb() will display).
 */
extern uint16_t *vkbd_buf;         /* pointer to image_buffer base */
extern unsigned  vkbd_buf_stride;  /* image_buffer_current_width (row stride) */
extern unsigned  vkbd_view_w;      /* visible width in logical OSK pixels */
extern unsigned  vkbd_view_h;      /* visible height */
extern unsigned  vkbd_x_scale;     /* logical X pixel scale (1=normal, 2=double-width) */
void vkbd_draw_rect_blend(int x, int y, int w, int h, uint16_t color);
void vkbd_draw_border(int x, int y, int w, int h, uint16_t color);
void vkbd_draw_text(int x, int y, uint16_t fg, const char *str);

#endif /* LIBRETRO_GRAPH_H */

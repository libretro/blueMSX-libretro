/*
 * libretro-graph.c — RGB565 drawing primitives for OSK overlay
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
#include "libretro-graph.h"
#include "libretro-font.i"

#include <string.h>

#include <retro_inline.h>

/* ── Globals (set from libretro.c before each vkbd_render call) ────────── */
uint16_t *vkbd_buf        = NULL;
unsigned  vkbd_buf_stride = 0;
unsigned  vkbd_view_w     = 0;
unsigned  vkbd_view_h     = 0;
unsigned  vkbd_x_scale    = 1;

/* ── Internal helpers ────────────────────────────────────────────────────── */

/* RGB565 50% alpha blend (no cross-channel bleed) */
static INLINE uint16_t blend50(uint16_t fg, uint16_t bg)
{
   return (uint16_t)((fg + bg + ((fg ^ bg) & 0x0821u)) >> 1);
}

/* Set one pixel; coordinates are in visible-area space */
static INLINE void set_pixel(int x, int y, uint16_t color)
{
   unsigned sx;
   unsigned base;
   unsigned px;
   if (!vkbd_buf) return;
   if (x < 0 || (unsigned)x >= vkbd_view_w) return;
   if (y < 0 || (unsigned)y >= vkbd_view_h) return;
   sx = vkbd_x_scale ? vkbd_x_scale : 1;
   base = (unsigned)y * vkbd_buf_stride + (unsigned)x * sx;
   for (px = 0; px < sx; px++)
      vkbd_buf[base + px] = color;
}

/* ── Public draw functions ───────────────────────────────────────────────── */

/* Filled rectangle with 50% alpha blend against current buffer content */
void vkbd_draw_rect_blend(int x, int y, int w, int h, uint16_t color)
{
   int i, j;
   unsigned sx;
   unsigned base;
   unsigned px;
   if (!vkbd_buf) return;
   sx = vkbd_x_scale ? vkbd_x_scale : 1;
   for (j = y; j < y + h; j++) {
      if (j < 0 || (unsigned)j >= vkbd_view_h) continue;
      for (i = x; i < x + w; i++) {
         if (i < 0 || (unsigned)i >= vkbd_view_w) continue;
         base = (unsigned)j * vkbd_buf_stride + (unsigned)i * sx;
         for (px = 0; px < sx; px++) {
            uint16_t *p = &vkbd_buf[base + px];
            *p = blend50(color, *p);
         }
      }
   }
}

/* 1-pixel border around a rectangle */
void vkbd_draw_border(int x, int y, int w, int h, uint16_t color)
{
   int i;
   for (i = x; i < x + w; i++) {
      set_pixel(i, y,         color);
      set_pixel(i, y + h - 1, color);
   }
   for (i = y + 1; i < y + h - 1; i++) {
      set_pixel(x,         i, color);
      set_pixel(x + w - 1, i, color);
   }
}

/*
 * Draw text using the 8x8 bitmap font from libretro-font.i.
 * Characters advance 6 pixels horizontally (tight spacing).
 * Special symbols at positions 0-31 (e.g. 0x1b=← 0x1c=↓ 0x1d=→ 0x1e=↑ 0x1f=↵).
 */
void vkbd_draw_text(int x, int y, uint16_t fg, const char *str)
{
   int i, cx, cy;
   if (!str || !vkbd_buf) return;
   for (i = 0; str[i]; i++) {
      unsigned char c = (unsigned char)str[i];
      const unsigned char *glyph;
      if (c >= 128) c = 0;
      glyph = &font_array[c * 8];
      for (cy = 0; cy < 8; cy++) {
         unsigned char row = glyph[cy];
         for (cx = 0; cx < 7; cx++) {
            if (row & (0x80u >> cx))
               set_pixel(x + i * 6 + cx, y + cy, fg);
         }
      }
   }
}





/*
 * libretro-vkbd.h — On-Screen Keyboard for blueMSX-libretro
 *
 * Written for blueMSX-libretro.
 * OSK concept and sticky-modifier approach inspired by vice-libretro
 * (https://github.com/libretro/vice-libretro, GPL v2),
 * but implemented independently.
 */
#ifndef LIBRETRO_VKBD_H
#define LIBRETRO_VKBD_H

#include <stdint.h>

/* 1 while the OSK is displayed */
extern int vkbd_active;

/* Per-key state arrays indexed by EC_* code (size 256) */
extern int osk_keydown[256];          /* frame countdown; key is held while > 0 */
extern int osk_sticky[256];           /* 1 = modifier is latched until next normal key */
extern int osk_sticky_pending_clear;  /* 1 = clear non-CAPS stickies once all keydowns expire */

/* Toggle the OSK on/off */
void vkbd_toggle(void);

/* Read joypad and update OSK cursor/key state (call once per frame when active) */
void vkbd_update_input(const int16_t *joypad_bits);

/* Render the OSK overlay onto the current frame buffer (call before video_cb) */
void vkbd_render(void);

#endif /* LIBRETRO_VKBD_H */

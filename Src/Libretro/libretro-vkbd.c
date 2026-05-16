/*
 * libretro-vkbd.c — On-Screen Keyboard for blueMSX-libretro
 *
 * Written for blueMSX-libretro.
 * OSK concept and sticky-modifier approach inspired by vice-libretro
 * (https://github.com/libretro/vice-libretro, GPL v2),
 * but implemented independently.
 *
 * Toggle : SELECT button (only in MSX/SVI/SG modes)
 * Navigate: D-pad  (with auto-repeat)
 * Press key: A button
 *   - Modifier keys (SHIFT/CTRL/GRAPH/CODE/CAPS): toggle sticky
 *   - Normal keys: inject for KEY_HOLD_FRAMES then auto-release stickies
 * Clear stickies: B button
 *
 * Sticky modifiers are highlighted in green; cursor key is highlighted in
 * light grey.  Arrow symbols use font positions 0x1b=← 0x1c=↓ 0x1d=→ 0x1e=↑.
 */

#include "libretro-vkbd.h"
#include "libretro-graph.h"
#include "InputEvent.h"
#include "libretro.h"   /* RETRO_DEVICE_ID_JOYPAD_* */

#include <string.h>
#include <stdint.h>

/* ── Public state ────────────────────────────────────────────────────────── */
int vkbd_active      = 0;
int osk_keydown[256]       = {0};
int osk_sticky[256]        = {0};
int osk_sticky_pending_clear = 0; /* cleared once all osk_keydowns reach 0 */

/* ── Layout constants ────────────────────────────────────────────────────── */
#define VKBD_COLS  15
#define VKBD_ROWS   6

/* A regular key is held for this many frames (~100 ms @ 60 fps) */
#define KEY_HOLD_FRAMES  6

/* Minimum horizontal padding per side (px) used as the fixed base for every key */
#define VKBD_MIN_PAD     2

/* D-pad auto-repeat: frames until repeat starts, then frames between steps */
#define NAV_DELAY  22
#define NAV_RATE    4

/* ── Cursor ──────────────────────────────────────────────────────────────── */
static int cur_x = 0;
static int cur_y = 1;   /* start on number row */

/* ── Navigation state ────────────────────────────────────────────────────── */
static uint16_t joy_prev   = 0;
static int      nav_dir    = -1;
static int      nav_frames = 0;
static int      a_frames   = 0;

/*
 * Elastic shares per key:
 *   0 = fixed  — width = strlen(label)*6 + 2*VKBD_MIN_PAD
 *   N = elastic — absorbs N proportional shares of the row's leftover space
 * Multi-char modifier/function keys are fixed (content-sized).
 * Single-char letter/digit/symbol keys are elastic (absorb slack, breathe).
 * SPACE gets 5 shares so it dominates row 5 like a real spacebar.
 * BS and RETURN get 2 shares so they are visibly wider than letter keys.
 */
static const int vkbd_key_elastic[VKBD_ROWS][VKBD_COLS] = {
   /* Row 0: F1-F5 elastic(1), SELECT/STOP/HOME/INS/DEL fixed */
   { 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0 },
   /* Row 1: Esc fixed; 1-\ elastic(1); BS elastic(2) */
   { 0, 1,1,1,1,1,1,1,1,1,1,1,1,1, 2 },
   /* Row 2: TAB fixed; Q-] elastic(1); RETURN elastic(2) */
   { 0, 1,1,1,1,1,1,1,1,1,1,1,1, 2, 0 },
   /* Row 3: CTRL fixed; A-` elastic(1) */
   { 0, 1,1,1,1,1,1,1,1,1, 1,1,1, 0,0 },
   /* Row 4: SHIFT/Acc fixed; Z-/ elastic(1) */
   { 0, 1,1,1,1,1,1,1,1,1,1, 0, 0, 0,0 },
   /* Row 5: CAPS/GRAPH/CODE fixed; SPACE elastic(5); arrows elastic(1) */
   { 0,0, 5,0, 1,1,1,1, 0,0,0,0,0,0,0 }
};

/* ── MSX keyboard layout (EC_* codes; -1 = empty / no action) ───────────── */
static const int vkbd_layout[VKBD_ROWS][VKBD_COLS] = {
   /* Row 0: F1-F5, SEL, STOP, HOME, INS, DEL (no ESC — ESC is in row 1) */
   { EC_F1, EC_F2, EC_F3, EC_F4, EC_F5,
     EC_SELECT, EC_STOP, EC_CLS, EC_INS, EC_DEL, -1, -1, -1, -1, -1 },
   /* Row 1: ESC + number row + \ + BS */
   { EC_ESC, EC_1, EC_2, EC_3, EC_4, EC_5, EC_6, EC_7,
     EC_8,   EC_9, EC_0, EC_NEG, EC_CIRCFLX, EC_BKSLASH, EC_BKSPACE },
   /* Row 2: QWERTY + [ + ] + RETURN */
   { EC_TAB, EC_Q, EC_W, EC_E, EC_R, EC_T, EC_Y,
     EC_U,   EC_I, EC_O, EC_P, EC_AT, EC_LBRACK, EC_RETURN, -1 },
   /* Row 3: home row + ; ' \ */
   { EC_CTRL, EC_A, EC_S, EC_D, EC_F, EC_G, EC_H,
     EC_J,    EC_K, EC_L, EC_SEMICOL, EC_COLON, EC_RBRACK, -1, -1 },
   /* Row 4: ZXCV + ACC + both shifts */
   { EC_LSHIFT, EC_Z, EC_X, EC_C, EC_V, EC_B, EC_N,
     EC_M, EC_COMMA, EC_PERIOD, EC_DIV, EC_UNDSCRE, EC_RSHIFT, -1, -1 },
   /* Row 5: modifiers + space + CODE + arrows */
   { EC_CAPS, EC_GRAPH, EC_SPACE, EC_CODE,
     EC_LEFT, EC_UP, EC_DOWN, EC_RIGHT, -1, -1, -1, -1, -1, -1, -1 }
};

/*
 * Display labels for each cell.
 * Single char: direct glyph.  Multi-char: drawn left-aligned then centered.
 * Arrow glyphs: 0x1b=← 0x1c=↓ 0x1d=→ 0x1e=↑  (from libretro-font.i positions 27-30)
 */
static const char *vkbd_labels[VKBD_ROWS][VKBD_COLS] = {
   /* Row 0: no ESC; SEL before STOP */
   { "F1","F2","F3","F4","F5","SELECT","STOP","HOME","INS","DEL","","","","","" },
   /* Row 1: ESC + numbers + = + \ + BS */
   { "ESC","1","2","3","4","5","6","7","8","9","0","-","=","\\","BS" },
   /* POSITIONAL: @=[ (EC_AT), [=] (EC_LBRACK); \x1f=↵ glyph */
   { "TAB","Q","W","E","R","T","Y","U","I","O","P","[","]","\x1f","" },
   /* POSITIONAL: ;=; '=EC_COLON ~=EC_RBRACK */
   { "CTRL","A","S","D","F","G","H","J","K","L",";","'","`","","" },
   /* Acc = accent dead key */
   { "SHIFT","Z","X","C","V","B","N","M",",",".","/","ACC","SHIFT","","" },
   { "CAPS","GRAPH","SPACE","CODE","\x1b","\x1e","\x1c","\x1d","","","","","","","" }
};

/*
 * Shifted (secondary) label for each key.  Shown in compact font at the
 * top-right corner of the key in VKBD_SHIFT_FG colour.  Empty = no label.
 * MSX standard shifted mappings (international QWERTY).
 */
static const char *vkbd_shift_labels[VKBD_ROWS][VKBD_COLS] = {
   /* F1-F5→F6-F10; SEL no shift; STOP no shift; HOME→CLS */
   { "F6","F7","F8","F9","F10","","","CLS","","","","","","","" },
   /* ESC no shift; 1-0→!@#$%^&*(); NEG→_; CIRCFLX→+; BKSLASH→| */
   { "","!","@","#","$","%","^","&","*","(",")","_","+","|","" },
   /* POSITIONAL: [(EC_AT)→{   ](EC_LBRACK)→}   ↵ no shift + dummy */
   { "","","","","","","","","","","","{","}","","" },
   /* POSITIONAL: ;→:   '→"   `→~ + 2 dummies */
   { "","","","","","","","","","",":","\"","~","","" },
   /* ,→<   .→>   /→?   ACC (dead key, no shift label) + 2 dummies */
   { "","","","","","","","","<",">","?","","","","" },
   { "","","","","","","","","","","","","","","" }
};

/* ── Helpers ─────────────────────────────────────────────────────────────── */

static int is_modifier(int ec)
{
   return ec == EC_LSHIFT || ec == EC_RSHIFT ||
          ec == EC_CTRL   || ec == EC_CAPS   ||
          ec == EC_GRAPH  || ec == EC_CODE;
}

/*
 * Compute pixel width of each column for 'row' given 'view_w' total pixels.
 * Empty cells get width 0.  Fixed keys (elastic=0) are sized to their label.
 * Elastic keys share remaining space proportionally; leftover pixels are
 * distributed one-by-one to early elastic share-units so the row is exact.
 */
static void compute_row_widths(int row, int view_w, int col_w[VKBD_COLS])
{
   int col, total_min, total_shares, per_share, leftover_rem;
   const char *lbl;

   total_min    = 0;
   total_shares = 0;
   for (col = 0; col < VKBD_COLS; col++) {
      lbl = vkbd_labels[row][col];
      if (vkbd_layout[row][col] < 0 || !lbl[0]) continue;
      total_min    += (int)strlen(lbl) * 6 + 2 * VKBD_MIN_PAD;
      total_shares += vkbd_key_elastic[row][col];
   }

   {
      int remaining = view_w - total_min;
      if (remaining < 0) remaining = 0;
      per_share    = (total_shares > 0) ? remaining / total_shares : 0;
      leftover_rem = (total_shares > 0) ? remaining - per_share * total_shares : 0;
   }

   for (col = 0; col < VKBD_COLS; col++) {
      int shares, min_w;
      lbl    = vkbd_labels[row][col];
      shares = vkbd_key_elastic[row][col];
      if (vkbd_layout[row][col] < 0 || !lbl[0]) {
         col_w[col] = 0;
         continue;
      }
      min_w = (int)strlen(lbl) * 6 + 2 * VKBD_MIN_PAD;
      if (shares > 0) {
         int bonus = (leftover_rem >= shares) ? shares : leftover_rem;
         leftover_rem -= bonus;
         col_w[col] = min_w + per_share * shares + bonus;
      } else {
         col_w[col] = min_w;
      }
   }
}

static void nav_move(int dir)
{
   int c, steps;
   switch (dir) {
   case 0: /* up */
   case 1: /* down */
   {
      int view_w, col_w_old[VKBD_COLS], col_w_new[VKBD_COLS];
      int cx, x, best, best_dist, center, dist;

      view_w = (vkbd_view_w > 0) ? (int)vkbd_view_w : 256;
      compute_row_widths(cur_y, view_w, col_w_old);

      /* Pixel centre of current key in the old row */
      cx = 0;
      for (c = 0; c < cur_x; c++) cx += col_w_old[c];
      cx += col_w_old[cur_x] / 2;

      if (dir == 0)
         cur_y = (cur_y == 0) ? VKBD_ROWS - 1 : cur_y - 1;
      else
         cur_y = (cur_y == VKBD_ROWS - 1) ? 0 : cur_y + 1;

      compute_row_widths(cur_y, view_w, col_w_new);

      /* Find the key whose centre x is closest in the new row */
      best = 0; best_dist = 999999; x = 0;
      for (c = 0; c < VKBD_COLS; c++) {
         if (col_w_new[c] == 0) continue;
         center = x + col_w_new[c] / 2;
         dist   = center - cx; if (dist < 0) dist = -dist;
         if (dist < best_dist) { best_dist = dist; best = c; }
         x += col_w_new[c];
      }
      cur_x = best;
      break;
   }
   case 2: /* left — skip empty cols */
      steps = VKBD_COLS;
      do {
         cur_x = (cur_x == 0) ? VKBD_COLS - 1 : cur_x - 1;
      } while ((vkbd_layout[cur_y][cur_x] < 0 || !vkbd_labels[cur_y][cur_x][0]) && --steps > 0);
      break;
   case 3: /* right — skip empty cols */
      steps = VKBD_COLS;
      do {
         cur_x = (cur_x == VKBD_COLS - 1) ? 0 : cur_x + 1;
      } while ((vkbd_layout[cur_y][cur_x] < 0 || !vkbd_labels[cur_y][cur_x][0]) && --steps > 0);
      break;
   }
}

/* ── Public API ──────────────────────────────────────────────────────────── */

void vkbd_toggle(void)
{
   vkbd_active = !vkbd_active;
   if (!vkbd_active) {
      memset(osk_keydown, 0, sizeof(osk_keydown));
      memset(osk_sticky,  0, sizeof(osk_sticky));
      osk_sticky_pending_clear = 0;
      joy_prev   = 0;
      nav_dir    = -1;
      nav_frames = 0;
      a_frames   = 0;
   }
}

void vkbd_update_input(const int16_t *joypad_bits)
{
   uint16_t joy     = (uint16_t)joypad_bits[0];
   uint16_t joy_new = joy & ~joy_prev;

   /* ── D-pad navigation ── */
   int dir = -1;
   if      (joy & (1u << RETRO_DEVICE_ID_JOYPAD_UP))    dir = 0;
   else if (joy & (1u << RETRO_DEVICE_ID_JOYPAD_DOWN))  dir = 1;
   else if (joy & (1u << RETRO_DEVICE_ID_JOYPAD_LEFT))  dir = 2;
   else if (joy & (1u << RETRO_DEVICE_ID_JOYPAD_RIGHT)) dir = 3;

   if (dir != nav_dir) {
      nav_dir    = dir;
      nav_frames = 0;
      if (dir >= 0) nav_move(dir);
   } else if (dir >= 0) {
      nav_frames++;
      if (nav_frames >= NAV_DELAY &&
          ((nav_frames - NAV_DELAY) % NAV_RATE) == 0)
         nav_move(dir);
   }

   /* ── A button: press current key (with auto-repeat for normal keys) ── */
   {
      int a_held = (joy     & (1u << RETRO_DEVICE_ID_JOYPAD_A)) != 0;
      int a_new  = (joy_new & (1u << RETRO_DEVICE_ID_JOYPAD_A)) != 0;
      int do_press = 0;

      if (a_new) {
         a_frames = 0;
         do_press = 1;
      } else if (a_held) {
         a_frames++;
         if (a_frames >= NAV_DELAY &&
             ((a_frames - NAV_DELAY) % NAV_RATE) == 0)
            do_press = 1;
      } else {
         a_frames = 0;
      }

      if (do_press) {
         int ec = vkbd_layout[cur_y][cur_x];
         if (ec > 0 && ec < 256) {
            if (is_modifier(ec)) {
               if (a_new) {
                  osk_sticky[ec] ^= 1;              /* toggle visual indicator */
                  if (ec == EC_CAPS)                /* send one pulse to MSX BIOS */
                     osk_keydown[EC_CAPS] = KEY_HOLD_FRAMES;
               }
            } else {
               osk_keydown[ec] = KEY_HOLD_FRAMES;
               if (a_new)   /* schedule sticky clear after keydown expires */
                  osk_sticky_pending_clear = 1;
            }
         }
      }
   }

   /* ── B button: clear all stickies ── */
   if (joy_new & (1u << RETRO_DEVICE_ID_JOYPAD_B)) {
      if (osk_sticky[EC_CAPS])                      /* toggle CAPS off in MSX BIOS */
         osk_keydown[EC_CAPS] = KEY_HOLD_FRAMES;
      memset(osk_sticky, 0, sizeof(osk_sticky));
      osk_sticky_pending_clear = 0;
   }

   joy_prev = joy;
}

void vkbd_render(void)
{
   int row, col;
   int base_key_w, key_h_w, key_h_h, key_h, osk_h, osk_y;
   if (!vkbd_active || !vkbd_buf || vkbd_view_w == 0 || vkbd_view_h == 0)
      return;

   /* ── Geometry ── */
   /* Key height is based on the narrowest row (13 cols) for visual consistency */
   base_key_w = (int)vkbd_view_w / VKBD_COLS;
   if (base_key_w < 10) base_key_w = 10;
   key_h_w = base_key_w * 4 / 3;
   key_h_h = (int)vkbd_view_h / VKBD_ROWS;
   key_h   = key_h_w;
   if (key_h > key_h_h) key_h = key_h_h;
   if (key_h < 1) key_h = 1;

   osk_h = key_h * VKBD_ROWS;
   osk_y = (int)vkbd_view_h - osk_h;   /* bottom-aligned */
   if (osk_y < 0) osk_y = 0;

   /* ── Background ── */
   vkbd_draw_rect_blend(0, osk_y, (int)vkbd_view_w, osk_h, VKBD_OSK_BG);

   /* ── Keys ── */
   for (row = 0; row < VKBD_ROWS; row++) {
      int col_w[VKBD_COLS], col_x[VKBD_COLS];
      int has_keys, ky;

      compute_row_widths(row, (int)vkbd_view_w, col_w);
      has_keys = 0;
      col_x[0] = 0;
      for (col = 0; col < VKBD_COLS; col++) {
         if (col > 0) col_x[col] = col_x[col-1] + col_w[col-1];
         if (col_w[col] > 0) has_keys = 1;
      }
      if (!has_keys) continue;
      ky = osk_y + row * key_h;

      for (col = 0; col < VKBD_COLS; col++) {
         if (col_w[col] == 0) continue;

         int         ec  = vkbd_layout[row][col];
         const char *lbl = vkbd_labels[row][col];
         if (ec < 0 || !lbl[0]) continue;

         int kx = col_x[col];
         int kw = col_w[col];

         /* Choose background color */
         uint16_t bg;
         if (row == cur_y && col == cur_x && ec > 0 && ec < 256 && osk_keydown[ec] > 0)
            bg = VKBD_KEY_ACT;          /* amber flash while key is held */
         else if (row == cur_y && col == cur_x)
            bg = VKBD_KEY_SEL;
         else if (ec > 0 && ec < 256 && osk_sticky[ec])
            bg = VKBD_KEY_STK;
         else
            bg = VKBD_KEY_BG;

         vkbd_draw_rect_blend(kx + 1, ky + 1, kw - 2, key_h - 2, bg);
         if (row == cur_y && col == cur_x)
            vkbd_draw_border(kx, ky, kw, key_h, VKBD_KEY_BRD);

         /* ── Labels ── */
         uint16_t fg = VKBD_WHITE;
         int lbl_len = (int)strlen(lbl);

         /* Secondary (shift) label — upper half of key, gold */
         const char *slbl      = vkbd_shift_labels[row][col];
         int         has_shift  = (slbl[0] != '\0');
         int         half_h     = key_h / 2;

         if (has_shift) {
            int slbl_len = (int)strlen(slbl);
            int slbl_w, stx, sty;
            sty = ky + 1 + (half_h - 2 - 8) / 2;
            if (sty < ky + 1) sty = ky + 1;
            slbl_w = slbl_len * 6;
            stx = kx + 1 + ((kw - 2 - slbl_w) / 2);
            if (stx < kx + 1) stx = kx + 1;
            vkbd_draw_text(stx, sty, VKBD_SHIFT_FG, slbl);
         }

         /* Primary label — lower half (if shift) or full key */
         int lbl_w, tx, ty;
         int inner_y = has_shift ? ky + half_h : ky + 1;
         int inner_h = has_shift ? half_h      : key_h - 2;
         lbl_w = lbl_len * 6;
         tx    = kx + 1 + ((kw - 2 - lbl_w) / 2);
         ty    = inner_y + (inner_h - 8) / 2;
         if (tx < kx + 1) tx = kx + 1;
         if (ty < ky + 1) ty = ky + 1;
         vkbd_draw_text(tx, ty, fg, lbl);
      }
   }
}

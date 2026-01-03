#include "util.h"
#include <ctype.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl.h>

void to_uppercase(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

// Helper to get pixel from 1-bit buffer
static inline uint8_t get_pixel_1bit(const uint8_t *buf, int32_t x, int32_t y, int32_t stride) {
  int32_t byte_idx = y * stride + (x >> 3);
  int32_t bit_idx = 7 - (x & 7);  // MSB first
  return (buf[byte_idx] >> bit_idx) & 1;
}

// Helper to set pixel in 1-bit buffer
static inline void set_pixel_1bit(uint8_t *buf, int32_t x, int32_t y, int32_t stride, uint8_t val) {
  int32_t byte_idx = y * stride + (x >> 3);
  int32_t bit_idx = 7 - (x & 7);  // MSB first
  if (val) {
    buf[byte_idx] |= (1 << bit_idx);
  } else {
    buf[byte_idx] &= ~(1 << bit_idx);
  }
}

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
  // Source canvas is 160x68 (CANVAS_HEIGHT x CANVAS_WIDTH)
  // We rotate 90 degrees CW to get 68x160 for the physical display
  //
  // The canvas buffer layout for 160x68 at 1bpp:
  // - stride = (160 + 7) / 8 = 20 bytes per row
  // - total = 20 * 68 = 1360 bytes
  //
  // After 90 CW rotation, we have 68x160:
  // - new_stride = (68 + 7) / 8 = 9 bytes per row
  // - total = 9 * 160 = 1440 bytes
  //
  // Rotation formula (90 CW): dst(x,y) = src(src_h - 1 - y, x)
  // Where src is 160x68, dst is 68x160

  int32_t src_w = CANVAS_HEIGHT;  // 160
  int32_t src_h = CANVAS_WIDTH;   // 68
  int32_t src_stride = (src_w + 7) / 8;  // 20

  int32_t dst_w = src_h;  // 68
  int32_t dst_h = src_w;  // 160
  int32_t dst_stride = (dst_w + 7) / 8;  // 9

  // Temp buffer for rotated image
  static uint8_t rotated_buf[9 * 160];  // 1440 bytes
  memset(rotated_buf, 0, sizeof(rotated_buf));

  uint8_t *src_buf = (uint8_t *)cbuf;

  // Rotate 90 degrees clockwise
  for (int32_t sy = 0; sy < src_h; sy++) {
    for (int32_t sx = 0; sx < src_w; sx++) {
      uint8_t pixel = get_pixel_1bit(src_buf, sx, sy, src_stride);
      // 90 CW: dst_x = src_h - 1 - sy, dst_y = sx
      int32_t dx = src_h - 1 - sy;
      int32_t dy = sx;
      set_pixel_1bit(rotated_buf, dx, dy, dst_stride, pixel);
    }
  }

  // Now we need to copy rotated data back and update canvas
  // The trick: we write the rotated 68x160 data but LVGL still thinks canvas is 160x68
  // This works because display driver reads the raw buffer
  memcpy(cbuf, rotated_buf, sizeof(rotated_buf));

  lv_obj_invalidate(canvas);
}

void draw_background(lv_obj_t *canvas) {
  lv_draw_rect_dsc_t rect_black_dsc;
  init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  // Canvas is CANVAS_HEIGHT x CANVAS_WIDTH (160x68), fill entire area
  lv_area_t coords = {0, 0, CANVAS_HEIGHT - 1, CANVAS_WIDTH - 1};

  lv_draw_rect(&layer, &rect_black_dsc, &coords);

  lv_canvas_finish_layer(canvas, &layer);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align) {
  lv_draw_label_dsc_init(label_dsc);
  label_dsc->color = color;
  label_dsc->font = font;
  label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
  lv_draw_rect_dsc_init(rect_dsc);
  rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color,
                   uint8_t width) {
  lv_draw_line_dsc_init(line_dsc);
  line_dsc->color = color;
  line_dsc->width = width;
}

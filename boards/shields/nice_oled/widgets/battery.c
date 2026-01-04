#include "battery.h"
#include <fonts.h>
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bolt);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY)
LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);
LV_IMG_DECLARE(crystal_03);
LV_IMG_DECLARE(crystal_04);
LV_IMG_DECLARE(crystal_05);
LV_IMG_DECLARE(crystal_06);
LV_IMG_DECLARE(crystal_07);
LV_IMG_DECLARE(crystal_08);
LV_IMG_DECLARE(crystal_09);
LV_IMG_DECLARE(crystal_10);
LV_IMG_DECLARE(crystal_11);
LV_IMG_DECLARE(crystal_12);
LV_IMG_DECLARE(crystal_13);
LV_IMG_DECLARE(crystal_14);
LV_IMG_DECLARE(crystal_15);
LV_IMG_DECLARE(crystal_16);

#ifndef SET_ANIMATION_SMART_BATTERY_OFF
#define SET_ANIMATION_SMART_BATTERY_OFF &crystal_01
#endif

const lv_img_dsc_t *crystal_imgs_test[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

static lv_obj_t *art = NULL;
static lv_obj_t *art2 = NULL;

void animation_smart_battery_on(lv_layer_t *layer) {
    if (art2) {
        lv_obj_del(art2);
        art2 = NULL;
    }
    art = lv_animimg_create(layer);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)crystal_imgs_test, 16);
    lv_animimg_set_duration(art, CONFIG_NICE_OLED_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y);
}
void animation_smart_battery_off(lv_layer_t *layer) {
    if (art) {
        lv_obj_del(art);
        art = NULL;
    }
    art2 = lv_img_create(layer);
    lv_img_set_src(art2, SET_ANIMATION_SMART_BATTERY_OFF);
    lv_obj_align(art2, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y);
}
#endif

static void draw_level(lv_layer_t *layer, const struct status_state *state) {
    lv_draw_label_dsc_t label_right_dsc;
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_RIGHT);
#else
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#endif // CONFIG_NICE_EPAPER_ON

    char text[10] = {};

    sprintf(text, "%i%%", state->battery);
    // x, y, width, dsc, text
    layer_draw_label(layer, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y, 42, 16, &label_right_dsc, text);
}

static void draw_charging_level(lv_layer_t *layer, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    lv_draw_label_dsc_t label_right_dsc;
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_RIGHT);
#else
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#endif // CONFIG_NICE_EPAPER_ON

    char text[10] = {};

    sprintf(text, "%i", state->battery);
    layer_draw_label(layer, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y, 35, 16, &label_right_dsc, text);
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    layer_draw_image(layer, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X + 36, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y + 2, &bolt, &img_dsc);
#else
    layer_draw_image(layer, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X + 25, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y, &bolt, &img_dsc);
#endif // CONFIG_NICE_EPAPER_ON
}

void draw_battery_status(lv_layer_t *layer, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
    layer_draw_label(layer, 0, 19, 25, 16, &label_left_dsc, "BAT");
#endif // CONFIG_NICE_EPAPER_ON
    if (state->charging) {
        draw_charging_level(layer, state);
    } else {
        draw_level(layer, state);
    }
}

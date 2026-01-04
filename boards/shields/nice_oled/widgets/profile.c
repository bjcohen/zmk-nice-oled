#include <zephyr/kernel.h>
#include "profile.h"
#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
// use custom_fonts.h only for the draw_active_profile_text function
#include <fonts.h>
#include <stdio.h>
#endif // !IS_ENABLED(CONFIG_NICE_EPAPER_ON)

LV_IMG_DECLARE(profiles);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG)
#define OFFSET_X 0
#define OFFSET_Y 129

LV_IMG_DECLARE(profile);
LV_IMG_DECLARE(profile_active);
#endif

#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG)
static void draw_inactive_profiles(lv_layer_t *layer, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    layer_draw_image(layer, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y, &profiles, &img_dsc);
}

static void draw_active_profile(lv_layer_t *layer, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    int offset = state->active_profile_index * 7;

    layer_draw_rect(layer, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X + offset, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y, 3, 3, &rect_white_dsc);
}
#endif // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG)

#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
static void draw_active_profile_text(lv_layer_t *layer, const struct status_state *state) {
    // new label_dsc
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_8, LV_TEXT_ALIGN_LEFT);

    char text[14] = {};
    snprintf(text, sizeof(text), "%d", state->active_profile_index + 1);

    layer_draw_label(layer, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y, 35, 8, &label_dsc, text);
}
#endif // CONFIG_NICE_EPAPER_ON

void draw_profile_status(lv_layer_t *layer, const struct status_state *state) {
#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    draw_active_profile_text(layer, state);
#endif // CONFIG_NICE_EPAPER_ON

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG) //  && IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    for (int i = 0; i < 5; i++) {
        layer_draw_image(layer, OFFSET_X + (i * 14), OFFSET_Y,
                           i == state->active_profile_index ? &profile_active : &profile, &img_dsc);
    }
#else
    draw_inactive_profiles(layer, state);
    draw_active_profile(layer, state);
#endif
}

#pragma once

#include <lvgl.h>
#include "util.h"

struct layer_status_state {
    uint8_t index;
    const char *label;
};

void draw_layer_status(lv_layer_t *layer, const struct status_state *state);

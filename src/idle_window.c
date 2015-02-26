/* ========================================================================== */
/*
 * idle_window.c
 *
 * Project Name: SportStat
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <pebble.h>

// ---------------- Local includes  e.g., "file.h"
#include "idle_window.h"

// ---------------- Constant definitions
#define WAITING_TEXT "Start game on \nAndroid App"
#define WAITING_TEXT_X 0
#define WAITING_TEXT_Y 100
#define WAITING_TEXT_HEIGHT 34
#define BASKETBALL_POS_X 0
#define BASKETBALL_POS_Y 20
#define BASKETBALL_HEIGHT 80

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static TextLayer* waiting_text;
static Layer *window_layer;
static GRect bounds;

static BitmapLayer *basketball_bitmap_layer;
static GBitmap *basketball_bitmap;

// ---------------- Private prototypes
static void load(Window *window);
static void unload(Window *window);
static void add_waiting_text(Layer* window_layer);
static void add_basketball_image(Layer* window_layer);

/* ========================================================================== */

Window *get_idle_window() {
    Window *idle_window = window_create();
    window_set_window_handlers(idle_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
    });

    return idle_window;
}

static void load(Window *window) {
    window_layer = window_get_root_layer(window);
    bounds = layer_get_bounds(window_layer);

    add_basketball_image(window_layer);
    add_waiting_text(window_layer);
}

static void unload(Window *window) {
    text_layer_destroy(waiting_text);
    bitmap_layer_destroy(basketball_bitmap_layer);
    gbitmap_destroy(basketball_bitmap);
}

static void add_waiting_text(Layer* window_layer) {
    waiting_text = text_layer_create((GRect) {
        .origin = { WAITING_TEXT_X, WAITING_TEXT_Y },
        .size = { bounds.size.w, WAITING_TEXT_HEIGHT }
    });
    text_layer_set_text(waiting_text, WAITING_TEXT);
    text_layer_set_text_alignment(waiting_text, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(waiting_text));
}

static void add_basketball_image(Layer* window_layer) {
    basketball_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BASKETBALL);
    basketball_bitmap_layer = bitmap_layer_create((GRect) {
        .origin = { BASKETBALL_POS_X, BASKETBALL_POS_Y },
        .size = { bounds.size.w, BASKETBALL_HEIGHT }
    });
    bitmap_layer_set_alignment(basketball_bitmap_layer, GAlignCenter);
    bitmap_layer_set_bitmap(basketball_bitmap_layer, basketball_bitmap);
    layer_add_child(window_layer, bitmap_layer_get_layer(basketball_bitmap_layer));
}

/* ========================================================================== */
/*
 * game_window.c
 *
 * Project Name: SportStat
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <pebble.h>

// ---------------- Local includes  e.g., "file.h"
#include "game_window.h"
#include "messaging.h"

// ---------------- Constant definitions
#define HEADING_TEXT_X 0
#define HEADING_TEXT_Y 20
#define HEADING_TEXT_HEIGHT 50
#define HEADING_FONT fonts_get_system_font(FONT_KEY_GOTHIC_24)

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static Layer *window_layer;
static GRect bounds;

static TextLayer* heading_text;
static char* heading;

// ---------------- Private prototypes
static void load(Window *window);
static void unload(Window *window);

/* ========================================================================== */

Window *get_summary_window(char* message) {
    Window *summary_window = window_create();
    window_set_window_handlers(summary_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
    });

    heading = message;

    return summary_window;
}

static void load(Window *window) {
    window_layer = window_get_root_layer(window);
    bounds = layer_get_bounds(window_layer);

    heading_text = text_layer_create((GRect) {
        .origin = { HEADING_TEXT_X, HEADING_TEXT_Y },
        .size = { bounds.size.w, HEADING_TEXT_HEIGHT }
    });
    text_layer_set_font(heading_text, HEADING_FONT);
    text_layer_set_text_alignment(heading_text, GTextAlignmentCenter);
    text_layer_set_text(heading_text, heading);
    layer_add_child(window_layer, text_layer_get_layer(heading_text));
}

static void unload(Window *window) {
    text_layer_destroy(heading_text);
}

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

// ---------------- Constant definitions
#define WAITING_TEXT "Game started!"
#define WAITING_TEXT_X 0
#define WAITING_TEXT_Y 50
#define WAITING_TEXT_HEIGHT 34

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static TextLayer* waiting_text;
static Layer *window_layer;
static GRect bounds;

// ---------------- Private prototypes
static void load(Window *window);
static void unload(Window *window);
static void add_filler_text(Layer* window_layer);

/* ========================================================================== */

Window *get_game_window() {
    Window *game_window = window_create();
    window_set_window_handlers(game_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
    });

    return game_window;
}

static void load(Window *window) {
    window_layer = window_get_root_layer(window);
    bounds = layer_get_bounds(window_layer);

    add_filler_text(window_layer);
}

static void unload(Window *window) {
    text_layer_destroy(waiting_text);
}

static void add_filler_text(Layer* window_layer) {
    waiting_text = text_layer_create((GRect) {
        .origin = { WAITING_TEXT_X, WAITING_TEXT_Y },
        .size = { bounds.size.w, WAITING_TEXT_HEIGHT }
    });
    text_layer_set_text(waiting_text, WAITING_TEXT);
    text_layer_set_text_alignment(waiting_text, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(waiting_text));
}


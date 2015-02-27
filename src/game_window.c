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
#define DIVIDER_X 50
#define DIVIDER_Y 0
#define DIVIDER_WIDTH 2

#define LABEL_TEXT_X DIVIDER_X + 10
#define LABEL_FONT fonts_get_system_font(FONT_KEY_GOTHIC_24)
#define ASSIST_TEXT "Assist"
#define ASSIST_TEXT_Y 6
#define ASSIST_TEXT_HEIGHT 30
#define TWO_PT_TEXT "Two point"
#define TWO_PT_TEXT_Y 62
#define TWO_PT_TEXT_HEIGHT 30
#define THREE_PT_TEXT "Three point"
#define THREE_PT_TEXT_Y 118
#define THREE_PT_TEXT_HEIGHT 30


// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static Layer *window_layer;
static GRect bounds;
static InverterLayer* divider;

static TextLayer* assist_text;
static TextLayer* two_pt_text;
static TextLayer* three_pt_text;
static int assists = 0;
static int two_pointers = 0;
static int three_pointers = 0;

// ---------------- Private prototypes
static void load(Window *window);
static void unload(Window *window);
static void add_label_text(Layer* window_layer);
static void add_divider(Layer* window_layer);
static void refresh_points();

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

    add_label_text(window_layer);
    add_divider(window_layer);
}

static void unload(Window *window) {
    text_layer_destroy(assist_text);
    text_layer_destroy(two_pt_text);
    text_layer_destroy(three_pt_text);
    inverter_layer_destroy(divider);
}

void update_game_window(int num_assists, int num_two_pointers,
                        int num_three_pointers) {
    assists = num_assists;
    two_pointers = num_two_pointers;
    three_pointers = num_three_pointers;

    refresh_points();
}

static void refresh_points() {

}

static void add_label_text(Layer* window_layer) {
    assist_text = text_layer_create((GRect) {
        .origin = { LABEL_TEXT_X, ASSIST_TEXT_Y },
        .size = { bounds.size.w, ASSIST_TEXT_HEIGHT }
    });
    text_layer_set_text(assist_text, ASSIST_TEXT);
    text_layer_set_font(assist_text, LABEL_FONT);
    layer_add_child(window_layer, text_layer_get_layer(assist_text));

    two_pt_text = text_layer_create((GRect) {
        .origin = { LABEL_TEXT_X, TWO_PT_TEXT_Y },
        .size = { bounds.size.w, TWO_PT_TEXT_HEIGHT }
    });
    text_layer_set_text(two_pt_text, TWO_PT_TEXT);
    text_layer_set_font(two_pt_text, LABEL_FONT);
    layer_add_child(window_layer, text_layer_get_layer(two_pt_text));

    three_pt_text = text_layer_create((GRect) {
        .origin = { LABEL_TEXT_X, THREE_PT_TEXT_Y },
        .size = { bounds.size.w, THREE_PT_TEXT_HEIGHT }
    });
    text_layer_set_text(three_pt_text, THREE_PT_TEXT);
    text_layer_set_font(three_pt_text, LABEL_FONT);
    layer_add_child(window_layer, text_layer_get_layer(three_pt_text));

}

static void add_divider(Layer* window_layer) {
    divider = inverter_layer_create((GRect) {
        .origin = { DIVIDER_X, DIVIDER_Y },
        .size = { DIVIDER_WIDTH, bounds.size.h }
    });
    layer_add_child(window_layer, inverter_layer_get_layer(divider));
}


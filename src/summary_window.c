    /* ========================================================================== */
/*
 * summary_window.c
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
#define HEADING_TEXT_Y 5
#define HEADING_TEXT_HEIGHT 30
#define HEADING_FONT fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD)

#define ASSISTS_TEXT_FORMAT "%d assists"
#define TWO_PTS_TEXT_FORMAT "%d two pointers"
#define THREE_PTS_TEXT_FORMAT "%d three pointers"
#define TOTAL_PTS_TEXT_FORMAT "%d total indv. points"

#define POINTS_FONT fonts_get_system_font(FONT_KEY_GOTHIC_24)
#define POINTS_TEXT_HEIGHT 28
#define POINTS_TEXT_X 0
#define MAX_CHARS 25

#define ASSISTS_TEXT_Y 35
#define TWO_PTS_TEXT_Y 63
#define THREE_PTS_TEXT_Y 91
#define TOTAL_PTS_TEXT_Y 119

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static Layer *window_layer;
static GRect bounds;

static TextLayer* heading_text;
static char* heading;

static TextLayer* assists_text_layer;
static TextLayer* two_pts_text_layer;
static TextLayer* three_pts_text_layer;
static TextLayer* total_pts_text_layer;

static char assists_text[MAX_CHARS];
static char two_pts_text[MAX_CHARS];
static char three_pts_text[MAX_CHARS];
static char total_pts_text[MAX_CHARS];

// ---------------- Private prototypes
static void load(Window *window);
static void unload(Window *window);

/* ========================================================================== */

Window *get_summary_window(char* message, int assists, int two_pts, int three_pts) {
    Window *summary_window = window_create();
    window_set_window_handlers(summary_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
    });

    heading = message;

    snprintf(assists_text, MAX_CHARS, ASSISTS_TEXT_FORMAT, assists);
    snprintf(two_pts_text, MAX_CHARS, TWO_PTS_TEXT_FORMAT, two_pts);
    snprintf(three_pts_text, MAX_CHARS, THREE_PTS_TEXT_FORMAT, three_pts);
    snprintf(total_pts_text, MAX_CHARS, TOTAL_PTS_TEXT_FORMAT,
        two_pts * 2 + three_pts * 3);

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

    assists_text_layer = text_layer_create((GRect) {
        .origin = { POINTS_TEXT_X, ASSISTS_TEXT_Y },
        .size = { bounds.size.w, POINTS_TEXT_HEIGHT }
    });
    text_layer_set_font(assists_text_layer, POINTS_FONT);
    text_layer_set_text_alignment(assists_text_layer, GTextAlignmentCenter);
    text_layer_set_text(assists_text_layer, assists_text);
    layer_add_child(window_layer, text_layer_get_layer(assists_text_layer));

    two_pts_text_layer = text_layer_create((GRect) {
        .origin = { POINTS_TEXT_X, TWO_PTS_TEXT_Y },
        .size = { bounds.size.w, POINTS_TEXT_HEIGHT }
    });
    text_layer_set_font(two_pts_text_layer, POINTS_FONT);
    text_layer_set_text_alignment(two_pts_text_layer, GTextAlignmentCenter);
    text_layer_set_text(two_pts_text_layer, two_pts_text);
    layer_add_child(window_layer, text_layer_get_layer(two_pts_text_layer));

    three_pts_text_layer = text_layer_create((GRect) {
        .origin = { POINTS_TEXT_X, THREE_PTS_TEXT_Y },
        .size = { bounds.size.w, POINTS_TEXT_HEIGHT }
    });
    text_layer_set_font(three_pts_text_layer, POINTS_FONT);
    text_layer_set_text_alignment(three_pts_text_layer, GTextAlignmentCenter);
    text_layer_set_text(three_pts_text_layer, three_pts_text);
    layer_add_child(window_layer, text_layer_get_layer(three_pts_text_layer));

    total_pts_text_layer = text_layer_create((GRect) {
        .origin = { POINTS_TEXT_X, TOTAL_PTS_TEXT_Y },
        .size = { bounds.size.w, POINTS_TEXT_HEIGHT }
    });
    text_layer_set_font(total_pts_text_layer, POINTS_FONT);
    text_layer_set_text_alignment(total_pts_text_layer, GTextAlignmentCenter);
    text_layer_set_text(total_pts_text_layer, total_pts_text);
    layer_add_child(window_layer, text_layer_get_layer(total_pts_text_layer));

}

static void unload(Window *window) {
    text_layer_destroy(heading_text);
    text_layer_destroy(assists_text_layer);
    text_layer_destroy(two_pts_text_layer);
    text_layer_destroy(three_pts_text_layer);
    text_layer_destroy(total_pts_text_layer);
}


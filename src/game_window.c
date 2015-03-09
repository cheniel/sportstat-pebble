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
#define DIVIDER_X 50
#define DIVIDER_Y 0
#define DIVIDER_WIDTH 2

#define LABEL_FONT fonts_get_system_font(FONT_KEY_GOTHIC_24)
#define LABEL_TEXT_X DIVIDER_X + 10
#define LABEL_TEXT_HEIGHT 30
#define ASSIST_LABEL "Assist"
#define TWO_PT_LABEL "Two point"
#define THREE_PT_LABEL "Three point"
#define ASSIST_LABEL_Y 6
#define TWO_PT_LABEL_Y 62
#define THREE_PT_LABEL_Y 118

#define POINT_FONT fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK)
#define POINT_TEXT_X 0
#define POINT_TEXT_WIDTH DIVIDER_X
#define POINT_TEXT_HEIGHT 42
#define ASSIST_Y 3
#define TWO_PT_Y 59
#define THREE_PT_Y 115
#define MAX_DIGITS 2

#define NUM_SAMPLES 3
#define NOT_SHOOTING_THRESHOLD 3

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static Layer *window_layer;
static GRect bounds;
static InverterLayer* divider;

static TextLayer* assist_label;
static TextLayer* two_pt_label;
static TextLayer* three_pt_label;
static TextLayer* assist_text_layer;
static TextLayer* two_pt_text_layer;
static TextLayer* three_pt_text_layer;
static char assist_text[MAX_DIGITS];
static char two_pt_text[MAX_DIGITS];
static char three_pt_text[MAX_DIGITS];
static int assists = 0;
static int two_pointers = 0;
static int three_pointers = 0;
static bool shooting;
static int consecutive_not_shooting_readings = 0;
static int attempted_shots = 0;

// ---------------- Private prototypes
static void load(Window *window);
static void unload(Window *window);
static void add_label_text(Layer* window_layer);
static void add_point_text(Layer* window_layer);
static void add_divider(Layer* window_layer);
static void refresh_points();
static void click_config_provider(void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_long_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_long_click_handler(ClickRecognizerRef recognizer, void *context);
static void accel_data_handler(AccelData *data, uint32_t num_samples);

/* ========================================================================== */

Window *get_game_window() {
    Window *game_window = window_create();
    window_set_click_config_provider(game_window, click_config_provider);
    window_set_window_handlers(game_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
    });

    accel_data_service_subscribe(NUM_SAMPLES, accel_data_handler);
    accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);

    return game_window;
}

static void accel_data_handler(AccelData *data, uint32_t num_samples) {

    // detect shooting
    if (
        // hand is up
        data[0].x < -900 &&
        data[1].x < -900 &&
        data[2].x < -900 &&

        // not too much tilt
        (data[0].z > -500 && data[0].z < 500) &&
        (data[1].z > -500 && data[1].z < 500) &&
        (data[2].z > -500 && data[2].z < 500) &&
        (data[0].y > -500 && data[0].y < 500) &&
        (data[1].y > -500 && data[1].y < 500) &&
        (data[2].y > -500 && data[2].y < 500) ) {

        // not not shooting, so reset counter
        consecutive_not_shooting_readings = 0;

        // if not already shooting, mark the user as shooting
        if (!shooting) {
            shooting = true;
            vibes_short_pulse();
            send_attempted_shots(++attempted_shots);
        }

    // detect no longer shooting, hand is down
    } else if (data[0].x > -100 &&
               data[1].x > -100 &&
               data[2].x > -100 ) {

        // mark play as not shooting if they have enough consecutive
        // not shooting readings
        if (shooting) {
            consecutive_not_shooting_readings++;
            if (consecutive_not_shooting_readings > NOT_SHOOTING_THRESHOLD) {
                shooting = false;
                consecutive_not_shooting_readings = 0;
            }
        }

    } else {

        // not not shooting, so reset counter
        consecutive_not_shooting_readings = 0;

    }

}

void request_game_window_send() {
    send_data_to_mobile(assists, two_pointers, three_pointers);
    send_attempted_shots(attempted_shots);
}

int get_game_assists() {
    return assists;
}

int get_game_two_pts() {
    return two_pointers;
}

int get_game_three_pts() {
    return three_pointers;
}


static void load(Window *window) {
    window_layer = window_get_root_layer(window);
    bounds = layer_get_bounds(window_layer);

    add_point_text(window_layer);
    add_label_text(window_layer);
    add_divider(window_layer);
}

static void unload(Window *window) {
    text_layer_destroy(assist_label);
    text_layer_destroy(two_pt_label);
    text_layer_destroy(three_pt_label);
    text_layer_destroy(assist_text_layer);
    text_layer_destroy(two_pt_text_layer);
    text_layer_destroy(three_pt_text_layer);
    inverter_layer_destroy(divider);
    accel_data_service_unsubscribe();
}

void update_game_window(int num_assists, int num_two_pointers,
                        int num_three_pointers, int num_attempted_shots) {
    assists = num_assists;
    two_pointers = num_two_pointers;
    three_pointers = num_three_pointers;
    attempted_shots = num_attempted_shots;

    refresh_points();
}

static void refresh_points() {
    // visual overflow issue if >= 100
    snprintf(assist_text, MAX_DIGITS + 1, "%d", assists);
    snprintf(two_pt_text, MAX_DIGITS + 1, "%d", two_pointers);
    snprintf(three_pt_text, MAX_DIGITS + 1, "%d", three_pointers);
    text_layer_set_text(assist_text_layer, assist_text);
    text_layer_set_text(two_pt_text_layer, two_pt_text);
    text_layer_set_text(three_pt_text_layer, three_pt_text);
}

static void add_point_text(Layer* window_layer) {
    assist_text_layer= text_layer_create((GRect) {
        .origin = { POINT_TEXT_X, ASSIST_Y },
        .size = { POINT_TEXT_WIDTH, POINT_TEXT_HEIGHT }
    });
    text_layer_set_font(assist_text_layer, POINT_FONT);
    text_layer_set_text_alignment(assist_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(assist_text_layer));

    two_pt_text_layer = text_layer_create((GRect) {
        .origin = { POINT_TEXT_X, TWO_PT_Y },
        .size = { POINT_TEXT_WIDTH, POINT_TEXT_HEIGHT }
    });
    text_layer_set_font(two_pt_text_layer, POINT_FONT);
    text_layer_set_text_alignment(two_pt_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(two_pt_text_layer));

    three_pt_text_layer = text_layer_create((GRect) {
        .origin = { POINT_TEXT_X, THREE_PT_Y },
        .size = { POINT_TEXT_WIDTH, POINT_TEXT_HEIGHT }
    });
    text_layer_set_font(three_pt_text_layer, POINT_FONT);
    text_layer_set_text_alignment(three_pt_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(three_pt_text_layer));

    refresh_points();
}

static void add_label_text(Layer* window_layer) {
    assist_label= text_layer_create((GRect) {
        .origin = { LABEL_TEXT_X, ASSIST_LABEL_Y },
        .size = { bounds.size.w, LABEL_TEXT_HEIGHT }
    });
    text_layer_set_text(assist_label, ASSIST_LABEL);
    text_layer_set_font(assist_label, LABEL_FONT);
    layer_add_child(window_layer, text_layer_get_layer(assist_label));

    two_pt_label = text_layer_create((GRect) {
        .origin = { LABEL_TEXT_X, TWO_PT_LABEL_Y },
        .size = { bounds.size.w, LABEL_TEXT_HEIGHT }
    });
    text_layer_set_text(two_pt_label, TWO_PT_LABEL);
    text_layer_set_font(two_pt_label, LABEL_FONT);
    layer_add_child(window_layer, text_layer_get_layer(two_pt_label));

    three_pt_label = text_layer_create((GRect) {
        .origin = { LABEL_TEXT_X, THREE_PT_LABEL_Y },
        .size = { bounds.size.w, LABEL_TEXT_HEIGHT }
    });
    text_layer_set_text(three_pt_label, THREE_PT_LABEL);
    text_layer_set_font(three_pt_label, LABEL_FONT);
    layer_add_child(window_layer, text_layer_get_layer(three_pt_label));
}

static void add_divider(Layer* window_layer) {
    divider = inverter_layer_create((GRect) {
        .origin = { DIVIDER_X, DIVIDER_Y },
        .size = { DIVIDER_WIDTH, bounds.size.h }
    });
    layer_add_child(window_layer, inverter_layer_get_layer(divider));
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 0, select_long_click_handler, NULL);
    window_long_click_subscribe(BUTTON_ID_UP, 0, up_long_click_handler, NULL);
    window_long_click_subscribe(BUTTON_ID_DOWN, 0, down_long_click_handler, NULL);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    assists++;
    refresh_points();
    send_data_to_mobile(assists, two_pointers, three_pointers);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    two_pointers++;
    refresh_points();
    send_data_to_mobile(assists, two_pointers, three_pointers);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    three_pointers++;
    refresh_points();
    send_data_to_mobile(assists, two_pointers, three_pointers);
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (assists > 0) {
        assists--;
        refresh_points();
        vibes_short_pulse();
        send_data_to_mobile(assists, two_pointers, three_pointers);
    }
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (two_pointers > 0) {
        two_pointers--;
        refresh_points();
        vibes_short_pulse();
        send_data_to_mobile(assists, two_pointers, three_pointers);
    }
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (three_pointers > 0) {
        three_pointers--;
        refresh_points();
        vibes_short_pulse();
        send_data_to_mobile(assists, two_pointers, three_pointers);
    }
}

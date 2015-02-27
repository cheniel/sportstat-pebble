/* ========================================================================== */
/*
 * messaging.c
 *
 * Project Name: SportStat
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <pebble.h>

// ---------------- Local includes  e.g., "file.h"
#include "messaging.h"
#include "game_window.h"

// ---------------- Constant definitions
#define MSG_GENERIC_STRING 0
#define MSG_ASSIST_COUNT 1
#define MSG_TWO_POINT_COUNT 2
#define MSG_THREE_POINT_COUNT 3
#define MSG_GAME_END 4
#define MSG_GAME_CANCELLED 5
#define MSG_REQUEST_RESPONSE 6
#define MSG_INITIAL_POINTS_LOAD 7

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static Window* game_window = NULL;

// ---------------- Private prototypes
static char *translate_error(AppMessageResult result);
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_fail_handler(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_handler(DictionaryIterator *iterator, void *context);
static void request_response();

/* ========================================================================== */

void send_data_to_mobile(int assists, int two_pts, int three_pts) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_int(iter, MSG_ASSIST_COUNT, &assists, sizeof(assists), false);
    dict_write_int(iter, MSG_TWO_POINT_COUNT, &two_pts, sizeof(two_pts), false);
    dict_write_int(iter, MSG_THREE_POINT_COUNT, &three_pts, sizeof(three_pts), false);
    app_message_outbox_send();
}

static char *translate_error(AppMessageResult result) {
    switch (result) {
        case APP_MSG_OK: return "APP_MSG_OK";
        case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
        case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
        case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
        case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
        case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
        case APP_MSG_BUSY: return "APP_MSG_BUSY";
        case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
        case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
        case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
        case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
        case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
        case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
        case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
        default: return "UNKNOWN ERROR";
    }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Receiving some message");

    // Get the first pair
    Tuple *t = dict_read_first(iterator);

    bool received_score_data = false;
    bool initial_points_load = false;

    int assist_count = 0;
    int two_pt_count = 0;
    int three_pt_count = 0;

    // TODO: handle case where app is closed and user is logging on pebble.
    // There should be some indicator that the data is populated from on resume
    // if so, it should be ignored and the data from the pebble should be sent

    // Process all pairs present
    while (t != NULL) {

        // used to store generic string message
        static char s_buffer[64];

        // Process this pair's key
        switch (t->key) {
            case MSG_GENERIC_STRING:
                snprintf(s_buffer, sizeof(s_buffer), "Received '%s'", t->value->cstring);
                APP_LOG(APP_LOG_LEVEL_DEBUG, s_buffer);
                break;

            case MSG_ASSIST_COUNT:
                received_score_data = true;
                assist_count = t->value->int8;
                break;

            case MSG_TWO_POINT_COUNT:
                received_score_data = true;
                two_pt_count = t->value->int8;
                break;

            case MSG_THREE_POINT_COUNT:
                received_score_data = true;
                three_pt_count = t->value->int8;
                break;

            case MSG_GAME_END:
                window_stack_pop(false);
                window_destroy(game_window);
                game_window = NULL;
                // TODO: go to end game screen
                break;

            case MSG_INITIAL_POINTS_LOAD:
                initial_points_load = true;
                break;
        }

        // Get next pair, if any
        t = dict_read_next(iterator);
    }

    if (received_score_data) {

        // if it is the initial data from the phone but the pebble already
        // has a game up, sync pebble data to phone and ignore message.
        // this handles case where phone is locked and unlocked
        if (game_window != NULL && initial_points_load) {
            request_game_window_send();
            return;
        }

        // open game window if it's not up
        if (game_window == NULL) {
            window_stack_pop(false);
            game_window = get_game_window();
            window_stack_push(game_window, true);
        }

        // update the game window with the data from the phone
        update_game_window(assist_count, two_pt_count, three_pt_count);

    }

}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped: %s", translate_error(reason));
}

static void outbox_fail_handler(DictionaryIterator *iterator,
                         AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message failed: %s", translate_error(reason));
    // if (reason != APP_MSG_OK) {
        // TODO: go to disconnected screen
        // only if game window is not null!
        // also need to handle APP_MSG_BUSY, needs to be resent.
    // }
}

static void outbox_sent_handler(DictionaryIterator *iterator,
                         void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Message sent!");
}

static void request_response() {
    int dummy = 0;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_int(iter, MSG_REQUEST_RESPONSE, &dummy, sizeof(dummy), false);
    app_message_outbox_send();
}

void init_app_messaging() {
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_fail_handler);
    app_message_register_outbox_sent(outbox_sent_handler);
    app_message_open(app_message_inbox_size_maximum(),
                   app_message_outbox_size_maximum());

    request_response();
}

/* ========================================================================== */
/*
 * sportstat.c
 *
 * Project Name: SportStat
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <pebble.h>

// ---------------- Local includes  e.g., "file.h"
#include "idle.h"

// ---------------- Constant definitions
#define ANIMATED true

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
static Window* idle_window;

// ---------------- Private prototypes


/* ========================================================================== */

static void init(void) {
  idle_window = get_idle_window();
  window_stack_push(idle_window, ANIMATED);
}

static void deinit(void) {
  window_destroy(idle_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

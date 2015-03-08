/* ========================================================================== */
/*
 * messaging.h
 *
 * Project Name: SportStat
 */
/* ========================================================================== */
#ifndef MESSAGING_H
#define MESSAGING_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <pebble.h>

// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros
void init_app_messaging();
void send_data_to_mobile(int assists, int two_pts, int three_pts);
void send_attempted_shots(int attempted_shots);

#endif // MESSAGING_H

/* ========================================================================== */
/*
 * summary_window.h
 *
 * Project Name: SportStat
 */
/* ========================================================================== */
#ifndef SUMMARY_H
#define SUMMARY_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <pebble.h>

// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros
Window *get_summary_window(char* message, int assists, int two_pts, int three_pts);

#endif // SUMMARY_H

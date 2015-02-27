/* ========================================================================== */
/*
 * game_window.h
 *
 * Project Name: SportStat
 */
/* ========================================================================== */
#ifndef GAME_H
#define GAME_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <pebble.h>

// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros
Window *get_game_window();
void update_game_window(int num_assists, int num_two_pointers,
                        int num_three_pointers);

#endif // GAME_H

#pragma once
#include "glib.h"

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct {
    Position start_in_button;
    Position start_global;
} DraggingStatePositions;

typedef struct {
    gboolean is_dragging;
    DraggingStatePositions positions;
} DraggingState;

extern void hide_navigation();
extern void show_navigation();

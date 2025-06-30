#pragma once
#include "gtk/gtk.h"

typedef struct FileWidget {
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *container;
} FileWidget;

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
extern void free_file_widget(FileWidget *file_widget);

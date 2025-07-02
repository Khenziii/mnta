#pragma once
#include "gtk/gtk.h"

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Metadata {
    Position saved_location;
} Metadata;

typedef struct Item {
    Metadata metadata;
    char *path;
    // Last segment of path.
    char *name;
} Item;

typedef struct FileWidget {
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *container;
    Item *item;
} FileWidget;

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
extern void show_navigation_hints();
extern void hide_navigation_hints();
extern char** get_navigation_hint_strings(gint amount);

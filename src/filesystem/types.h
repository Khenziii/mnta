#pragma once
#include "./glib.h"
#include "../utils/utils.h"

typedef struct Items {
    Item *items;
    int count;
} Items;

typedef struct DefaultFilesPlacement {
    int files_per_row;
    int margin_horizontal;
    int margin_vertical;
    int container_padding;
} DefaultFilesPlacement;

typedef struct Settings {
    DefaultFilesPlacement *default_files_placement;
    char *editor_launch_command;
    char *background_color_hex;
} Settings;

typedef struct UIState {
    // Where each file has a key of type string, and value of `Item`.
    GHashTable *files;
} UIState;

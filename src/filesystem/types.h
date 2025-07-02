#pragma once
#include "./glib.h"
#include "../utils/utils.h"

typedef struct Items {
    Item *items;
    int count;
} Items;

typedef struct Settings {
    char *editor_launch_command;
} Settings;

typedef struct UIState {
    // Where each file has a key of type string, and value of `Item`.
    GHashTable *files;
} UIState;

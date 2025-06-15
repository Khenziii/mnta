#pragma once
#include "./glib.h"
#include "../core/types.h"

typedef struct Item {
    Metadata metadata;
    char *path;
    // Last segment of path.
    char *name;
} Item;

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

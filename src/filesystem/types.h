#pragma once
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

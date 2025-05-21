#include "../core/core.h"

typedef struct {
    Metadata metadata;
    char *path;
    // Last segment of path.
    char *name;
} Item;

typedef struct {
    Item *items;
    int count;
} Items;

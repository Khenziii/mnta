#include "dirent.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "./types.h"
#include "../core/core.h"

Items process_directory(char path[]) {
    uint capacity = 10;
    uint items_count = 0;
    Item* items = malloc(capacity * sizeof(Item));
    
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(path);
    if (dir == NULL) perror("opendir");
    
    while ((entry = readdir(dir)) != NULL) {
        uint skip_itertaion = strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0;
        if (skip_itertaion) continue;

        char full_path[PATH_MAX];
        if (realpath(entry->d_name, full_path) == NULL) {
            perror("realpath");
        }

        if (items_count >= capacity) {
            capacity *= 2;
            items = realloc(items, capacity * sizeof(Item));
        }

        Item item;
        item.name = strdup(entry->d_name);
        item.path = strdup(full_path);
        item.metadata = get_for_path(full_path, items_count+1);
        items[items_count] = item;
        items_count++;
    }
    
    closedir(dir);

    Items items_wrapper;
    items_wrapper.count = items_count;
    items_wrapper.items = items;
    return items_wrapper;
}

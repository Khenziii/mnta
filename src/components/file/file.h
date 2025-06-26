#pragma once
#include "gtk/gtk.h"
#include "../../filesystem/types.h"

typedef struct Navigation {
    GtkWidget *previous_directory;
} Navigation;

extern void add_file(
    GtkWidget *container,
    Item item,
    void (*on_click)(GtkWidget *container, Item file),
    gboolean draggable,
    gboolean add_to_context,
    gboolean add_to_context_as_navigation_back
);
extern void file_click_handler(GtkWidget *container, Item file);
extern void rerender_navigation_buttons();

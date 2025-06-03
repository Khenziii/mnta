#include "gtk/gtk.h"
#include "glib.h"
#include "../../filesystem/types.h"
#include "../terminal/terminal.h"

void file_click_handler(GtkWidget *container, Item file) {
    g_print("Clicked a button! Context: name - %s, path - %s\n", file.name, file.path);
    spawn_new_terminal(container, file);
}

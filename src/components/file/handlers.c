#include "gtk/gtk.h"
#include "../../filesystem/filesystem.h"
#include "../terminal/terminal.h"

void file_click_handler(GtkWidget *container, Item file) {
    gboolean path_is_directory = directory_exists(file.path);

    if (path_is_directory) {
        switch_directory(container, file.path);
    } else {
        spawn_new_terminal(container, file);
    }
}

#include "../../filesystem/filesystem.h"
#include "../terminal/terminal.h"

void file_click_handler(Item file) {
    gboolean path_is_directory = directory_exists(file.path);

    if (path_is_directory) {
        switch_directory(file.path);
    } else {
        spawn_new_terminal(file);
    }
}

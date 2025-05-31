#include "glib.h"
#include "../../filesystem/types.h"

void file_click_handler(Item file) {
    g_print("Clicked a button! Context: name - %s, path - %s\n", file.name, file.path);
}

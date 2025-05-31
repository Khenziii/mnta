#include "gtk/gtk.h"
#include "../../filesystem/types.h"

extern void add_file(GtkWidget *container, Item item, void (*on_click)(Item file));
extern void file_click_handler(Item file);

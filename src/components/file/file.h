#include "gtk/gtk.h"
#include "../../filesystem/types.h"

extern void add_file(GtkWidget *container, Item item, void (*on_click)(GtkWidget *container, Item file));
extern void file_click_handler(GtkWidget *container, Item file);

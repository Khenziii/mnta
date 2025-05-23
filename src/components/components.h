#include "gtk/gtk.h"

extern GtkWidget* setup_canvas(GtkWidget *window);
extern void add_file(GtkWidget *container, char text[], guint x, guint y, void (*on_click)());

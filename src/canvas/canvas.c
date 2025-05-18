#include <gtk/gtk.h>

void setup_canvas(GtkWidget *window) {
    GtkWidget *container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);

    GtkWidget *drag_button = gtk_button_new_with_label("drag me :3");
    gtk_fixed_put(GTK_FIXED(container), drag_button, 50, 50);
}

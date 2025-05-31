#include <gtk/gtk.h>

GtkWidget* setup_canvas(GtkWidget *window) {
    GtkWidget *container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);
    return container;
}

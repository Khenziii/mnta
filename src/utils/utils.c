#include "../context/context.h"

void hide_navigation() {
    AppContext context = context_get();
    GtkWidget *navigation = context.navigation.previous_directory;

    gtk_widget_hide(navigation);
}

void show_navigation() {
    AppContext context = context_get();
    GtkWidget *navigation = context.navigation.previous_directory;

    gtk_widget_show(navigation);
}

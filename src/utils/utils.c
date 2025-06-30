#include "../context/context.h"

void hide_navigation() {
    AppContext context = context_get();
    GtkWidget *navigation = context.navigation.previous_directory->container;

    gtk_widget_hide(navigation);
}

void show_navigation() {
    AppContext context = context_get();
    GtkWidget *navigation = context.navigation.previous_directory->container;

    gtk_widget_show(navigation);
}

void free_file_widget(FileWidget *file_widget) {
    gtk_widget_destroy(file_widget->label);
    gtk_widget_destroy(file_widget->button);
    gtk_widget_destroy(file_widget->container);
    free(file_widget);
}

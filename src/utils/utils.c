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

void show_navigation_hints() {
    AppContext context = context_get();
    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        FileWidget *file_widget = context.current_file_widgets[i];
        GtkWidget *navigation_hint = file_widget->label;

        gtk_widget_show(navigation_hint);
    }

    GtkWidget *previous_dir_navigation_hint = context.navigation.previous_directory->label;
    gtk_widget_show(previous_dir_navigation_hint);
}

void hide_navigation_hints() {
    AppContext context = context_get();
    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        FileWidget *file_widget = context.current_file_widgets[i];
        GtkWidget *navigation_hint = file_widget->label;

        gtk_widget_hide(navigation_hint);
    }

    GtkWidget *previous_dir_navigation_hint = context.navigation.previous_directory->label;
    gtk_widget_hide(previous_dir_navigation_hint);
}

void free_file_widget(FileWidget *file_widget) {
    gtk_widget_destroy(file_widget->label);
    gtk_widget_destroy(file_widget->button);
    gtk_widget_destroy(file_widget->container);
    free(file_widget);
}

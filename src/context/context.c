#include "gtk/gtk.h"
#include "./context.h"
#include "../utils/utils.h"

static AppContext context;

// This has to be called before using any other methods of the module safely.
AppContext context_setup_default() {
    context.current_canvas_position = g_new(Position, 1);
    context.current_canvas_position->x = 0;
    context.current_canvas_position->y = 0;
    return context;
}

AppContext context_get() {
    return context;
}

AppContext context_add_file_widget(GtkWidget *new_file_widget) {
    size_t new_amount = context.amount_of_current_file_widgets + 1;

    GtkWidget **resized_file_widgets = realloc(context.current_file_widgets, new_amount * sizeof(GtkWidget *));

    resized_file_widgets[context.amount_of_current_file_widgets] = new_file_widget;
    context.current_file_widgets = resized_file_widgets;
    context.amount_of_current_file_widgets = new_amount;

    return context;
}

void context_remove_all_file_widgets() {
    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        GtkWidget *widget = context.current_file_widgets[i];
        gtk_widget_destroy(widget);
    }

    free(context.current_file_widgets);
    context.current_file_widgets = NULL;
    context.amount_of_current_file_widgets = 0;
}

AppContext context_set_is_editor_currently_open(gboolean *is_editor_currently_open) {
    if (is_editor_currently_open) {
        hide_navigation();
    } else {
        show_navigation();
    }

    context.is_editor_currently_open = is_editor_currently_open;
    return context;
}

AppContext context_set_previous_directory_navigation(GtkWidget *button) {
    context.navigation.previous_directory = button;
    return context;
}

AppContext context_set_current_canvas_position(Position *canvas_position) {
    context.current_canvas_position = canvas_position;
    return context;
}

AppContext context_set_current_path(char *new_path) {
    context.current_path = new_path;
    return context;
}

AppContext context_set_canvas(GtkWidget *canvas) {
    context.canvas = canvas;
    return context;
}

AppContext context_set_canvas_container(GtkWidget *canvas_container) {
    context.canvas_container = canvas_container;
    return context;
}

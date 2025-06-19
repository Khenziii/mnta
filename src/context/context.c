#include "gtk/gtk.h"
#include "./context.h"

static AppContext context;

AppContext get_context() {
    return context;
}

AppContext add_file_widget_to_context(GtkWidget *new_file_widget) {
    size_t new_amount = context.amount_of_current_file_widgets + 1;

    GtkWidget **resized_file_widgets = realloc(context.current_file_widgets, new_amount * sizeof(GtkWidget *));

    resized_file_widgets[context.amount_of_current_file_widgets] = new_file_widget;
    context.current_file_widgets = resized_file_widgets;
    context.amount_of_current_file_widgets = new_amount;

    return context;
}

void remove_all_file_widgets_from_context() {
    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        GtkWidget *widget = context.current_file_widgets[i];
        gtk_widget_destroy(widget);
    }

    free(context.current_file_widgets);
    context.current_file_widgets = NULL;
    context.amount_of_current_file_widgets = 0;
}

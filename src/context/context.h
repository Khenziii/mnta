#include "gtk/gtk.h"

typedef struct AppContext {
    GtkWidget **current_file_widgets;
    size_t amount_of_current_file_widgets;
} AppContext;

extern AppContext get_context();
extern AppContext add_file_widget_to_context(GtkWidget *new_file_widget);
extern void remove_all_file_widgets_from_context();

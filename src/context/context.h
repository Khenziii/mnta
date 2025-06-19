#include "gtk/gtk.h"
#include "../components/components.h"

typedef struct AppContext {
    GtkWidget **current_file_widgets;
    size_t amount_of_current_file_widgets;
    Navigation navigation;
    char *current_path;
} AppContext;

extern AppContext get_context();
extern AppContext add_file_widget_to_context(GtkWidget *new_file_widget);
extern AppContext set_previous_directory_navigation(GtkWidget *button);
extern AppContext set_current_path(char *new_path);
extern void remove_all_file_widgets_from_context();

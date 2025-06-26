#include "gtk/gtk.h"
#include "../components/components.h"

typedef struct AppContext {
    GtkWidget **current_file_widgets;
    size_t amount_of_current_file_widgets;
    Navigation navigation;
    char *current_path;
    GtkWidget *canvas_container;
    GtkWidget *canvas;
} AppContext;

extern AppContext context_get();
extern AppContext context_add_file_widget(GtkWidget *new_file_widget);
extern AppContext context_set_previous_directory_navigation(GtkWidget *button);
extern AppContext context_set_current_path(char *new_path);
extern AppContext context_set_canvas(GtkWidget *canvas);
extern AppContext context_set_canvas_container(GtkWidget *canvas_container);
extern void context_remove_all_file_widgets();

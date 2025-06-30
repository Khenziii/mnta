#include "gtk/gtk.h"
#include "../components/components.h"

typedef struct AppContext {
    FileWidget **current_file_widgets;
    size_t amount_of_current_file_widgets;
    Navigation navigation;
    char *current_path;
    GtkWidget *canvas_container;
    GtkWidget *canvas;
    Position *current_canvas_position;
    gboolean *is_editor_currently_open;
} AppContext;

extern AppContext context_setup_default();
extern AppContext context_get();
extern AppContext context_add_file_widget(FileWidget *new_file_widget);
extern AppContext context_set_previous_directory_navigation(FileWidget *file_widget);
extern AppContext context_set_current_canvas_position(Position *canvas_position);
extern AppContext context_set_current_path(char *new_path);
extern AppContext context_set_canvas(GtkWidget *canvas);
extern AppContext context_set_canvas_container(GtkWidget *canvas_container);
extern AppContext context_set_is_editor_currently_open(gboolean *is_editor_currently_open);
extern void context_remove_all_file_widgets();

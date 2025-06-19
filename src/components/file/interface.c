#include "../../context/context.h"

static void add_go_back_button(GtkWidget *container) {
    AppContext context = get_context();
    char previous_path[PATH_MAX];
    sprintf(previous_path, "%s/..", context.current_path);

    Item item;
    item.name = "..";
    item.path = strdup(previous_path);
    item.metadata.saved_location.x = 10;
    item.metadata.saved_location.y = 10;

    add_file(container, item, file_click_handler, FALSE, FALSE, TRUE);
}

void rerender_navigation_buttons(GtkWidget *container) {
    AppContext context = get_context();
    if (context.navigation.previous_directory) gtk_widget_destroy(context.navigation.previous_directory);

    add_go_back_button(container);

    gtk_widget_show_all(container);
}

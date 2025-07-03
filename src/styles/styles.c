#include <gtk/gtkcssprovider.h>
#include "../filesystem/filesystem.h"

static char *css_get_main_window_styles() {
    Settings settings = get_settings();
   
    int needed_chars = snprintf(NULL, 0, "#main-window { background-color: %s; }", settings.background_color_hex);
    char *main_window = malloc(needed_chars + 1);
    sprintf(main_window, "#main-window { background-color: %s; }", settings.background_color_hex);

    return main_window;
}

GtkCssProvider *css_get_provider() {
    GtkCssProvider *css_provider = gtk_css_provider_new();

    char *main_window = css_get_main_window_styles();
    gtk_css_provider_load_from_data(css_provider, main_window, -1, NULL);

    return css_provider;
}

#include <gtk/gtkcssprovider.h>
#include "./colors.c"

const char main_window[] = "#main-window { background-color: " BACKGROUND_HEX "; }";

GtkCssProvider *css_get_provider() {
    GtkCssProvider *css_provider = gtk_css_provider_new();

    gtk_css_provider_load_from_data(css_provider, main_window, -1, NULL);

    return css_provider;
}

#include "glib.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include "./styles/styles.h"
#include "./components/components.h"
#include "./filesystem/filesystem.h"
#include "./context/context.h"

static void activate(GtkApplication *app, gpointer path) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MNTA");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    gtk_window_present(GTK_WINDOW(window));
    gtk_widget_set_name(window, "main-window");

    GdkScreen *screen = gtk_widget_get_screen(window);

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, main_window, -1, NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    setup_canvas(window);

    AppContext context = context_get();

    switch_directory(context.canvas, path);
}

int main(int argc, char *argv[]) {
    char *path = ".";
    if (argc > 1) path = argv[1];

    GtkApplication *app;
    int status;

    app = gtk_application_new("dev.khenzii.mnta", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), path);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

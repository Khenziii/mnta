#include "glib.h"
#include <gtk/gtk.h>
#include "./styles/styles.h"
#include "./components/components.h"
#include "./filesystem/filesystem.h"
#include "./context/context.h"

static void check_if_widget_was_activated(FileWidget *activated_widget_buffer, char *entered_text, FileWidget *possible_widget) {
    GtkWidget *possible_label = possible_widget->label;
    const char *possible_label_text = gtk_label_get_text(GTK_LABEL(possible_label));

    if (strcmp(possible_label_text, entered_text) == 0) {
        *activated_widget_buffer = *possible_widget;
    }
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    AppContext context = context_get();
    if (context.is_editor_currently_open) return FALSE;

    char *key = g_ascii_strup(gdk_keyval_name(event->keyval), -1);
    if (strcmp(key, "F") == 0) {
        strcpy(context.navigation_hints->currently_entered, "");
        
        gboolean *are_navigation_hints_currently_shown = context.navigation_hints->are_currently_shown;
        gboolean *negated = g_new(gboolean, 1);
        *negated = !(*are_navigation_hints_currently_shown);
        context_set_navigation_hints_are_currently_shown(negated);
        return FALSE;
    }
    if (!(*context.navigation_hints->are_currently_shown)) return FALSE;

    GtkWidget *label = context.navigation.previous_directory->label;
    const char *text = gtk_label_get_text(GTK_LABEL(label));

    char *resized_buffer = realloc(context.navigation_hints->currently_entered, strlen(text) + 1);
    context.navigation_hints->currently_entered = resized_buffer;

    strcat(context.navigation_hints->currently_entered, key);
    if (strlen(context.navigation_hints->currently_entered) == strlen(text)) {
        FileWidget *activated_widget = g_new0(FileWidget, 1);
        for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
            FileWidget *current_widget = context.current_file_widgets[i];
            check_if_widget_was_activated(activated_widget, context.navigation_hints->currently_entered, current_widget);
        }

        FileWidget *navigation_widget = context.navigation.previous_directory;
        check_if_widget_was_activated(activated_widget, context.navigation_hints->currently_entered, navigation_widget);

        if (activated_widget->label != NULL) {
            file_click_handler(*activated_widget->item);
        }

        gboolean *navigation_tips_shown = g_new(gboolean, 1);
        *navigation_tips_shown = FALSE;
        context_set_navigation_hints_are_currently_shown(navigation_tips_shown);
        strcpy(context.navigation_hints->currently_entered, "");
    }

    return FALSE;
}

static void activate(GtkApplication *app, gpointer path) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MNTA");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    gtk_window_present(GTK_WINDOW(window));
    gtk_widget_set_name(window, "main-window");
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    GdkScreen *screen = gtk_widget_get_screen(window);

    GtkCssProvider *css_provider = css_get_provider();
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    setup_canvas(window);
    context_setup_default();

    switch_directory(path);
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

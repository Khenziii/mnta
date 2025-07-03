#include "glib.h"
#include <gtk/gtk.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include "./styles/styles.h"
#include "./components/components.h"
#include "./filesystem/filesystem.h"
#include "./context/context.h"

static void make_character_of_label_bold(GtkLabel *label, int index) {
    guint amount_of_bold_chars = index+1;
    const char *label_text = gtk_label_get_text(label);

    // Copy first `amount_of_bold_chars` chars from `label_text`.
    char *bold_text = malloc(amount_of_bold_chars + 1);
    strncpy(bold_text, label_text, amount_of_bold_chars);
    bold_text[amount_of_bold_chars] = '\0';

    char *markup = g_strdup_printf("<b>%s</b>%s", bold_text, &label_text[amount_of_bold_chars]);
    gtk_label_set_markup(GTK_LABEL(label), markup);
}

static void make_matching_chararcters_of_label_bold(GtkLabel *label, char *text) {
    const char *label_text = gtk_label_get_text(label);
    for (int i = 0; i < strlen(text); i++) {
        char current_char = text[i];
        if (label_text[i] == current_char) {
            make_character_of_label_bold(label, i);
        }
    }
}

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

    if (strcmp(key, "H") == 0) {
        move_canvas_left();
        return FALSE;
    }

    if (strcmp(key, "J") == 0) {
        move_canvas_down();
        return FALSE;
    }

    if (strcmp(key, "K") == 0) {
        move_canvas_up();
        return FALSE;
    }

    if (strcmp(key, "L") == 0) {
        move_canvas_right();
        return FALSE;
    }

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
        return FALSE;
    }

    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        FileWidget *current_widget = context.current_file_widgets[i];
        GtkLabel *current_label = GTK_LABEL(current_widget->label);
        make_matching_chararcters_of_label_bold(
            current_label,
            context.navigation_hints->currently_entered
        );
    }
    FileWidget *current_widget = context.navigation.previous_directory;
    GtkLabel *current_label = GTK_LABEL(current_widget->label);
    make_matching_chararcters_of_label_bold(
        current_label,
        context.navigation_hints->currently_entered
    );

    return FALSE;
}

static void activate(GtkApplication *app, char *path) {
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

    gboolean path_is_directory = directory_exists(path);
    if (path_is_directory) {
        switch_directory(path);
    } else {
        char absolute_file_path[PATH_MAX];
        if (realpath(path, absolute_file_path) == NULL) {
            perror("realpath");
        }
        char *absolute_file_path_dup = strdup(absolute_file_path);
        char *dir_path = dirname(absolute_file_path_dup);

        switch_directory(dir_path);

        Item *opened_file = g_new0(Item, 1);
        AppContext context = context_get();
        for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
            FileWidget *current_widget = context.current_file_widgets[i];
            Item *current_file = current_widget->item;

            if (strcmp(current_file->path, absolute_file_path) == 0) {
                *opened_file = *current_file;
            }
        }
        
        if (opened_file->path != NULL) file_click_handler(*opened_file);
    }
}

static void open(GtkApplication *app, GFile **files, gint n_files, const gchar *hint, char *path) {
    activate(app, path);
}

int main(int argc, char *argv[]) {
    char *path = ".";
    if (argc > 1) path = argv[1];

    GtkApplication *app;
    int status;

    app = gtk_application_new("dev.khenzii.mnta", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "activate", G_CALLBACK(activate), path);
    g_signal_connect(app, "open", G_CALLBACK(open), path);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

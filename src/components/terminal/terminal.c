#include "gtk/gtk.h"
#include "stdio.h"
#include "vte/vte.h"
#include "../../filesystem/filesystem.h"
#include "../../context/context.h"

GtkWidget *container;
GtkWidget *terminal;
VteTerminal *vte_terminal;

static void update_terminal_size_to_fit_container() {
    GtkAllocation allocation;
    gtk_widget_get_allocation(container, &allocation);

    gtk_widget_set_size_request(GTK_WIDGET(terminal), allocation.width, allocation.height);
}

static void on_terminal_exit(VteTerminal *passed_terminal, int status, gpointer user_data) {
    g_signal_handlers_disconnect_by_func(container, update_terminal_size_to_fit_container, NULL);
    gtk_container_remove(GTK_CONTAINER(container), GTK_WIDGET(terminal));

    context_set_is_editor_currently_open(FALSE);
}

void spawn_new_terminal(Item file) {
    AppContext context = context_get();
    context_set_is_editor_currently_open((gboolean *)TRUE);

    container = context.canvas_container;
    terminal = vte_terminal_new();
    vte_terminal = VTE_TERMINAL(terminal);

    Settings settings = get_settings();

    char editor_command[1000];
    sprintf(editor_command, "%s %s", settings.editor_launch_command, file.path);
    const char *start_command[] = { "/bin/sh", "-c", editor_command, NULL };

    vte_terminal_spawn_async(
        vte_terminal,
        VTE_PTY_DEFAULT,
        NULL,
        (char **)start_command,
        NULL,
        G_SPAWN_DEFAULT,
        NULL,
        NULL,
        NULL,
        -1,
        NULL,
        NULL,
        NULL
    );

    g_signal_connect(vte_terminal, "child-exited", G_CALLBACK(on_terminal_exit), NULL);
    g_signal_connect(container, "size-allocate", G_CALLBACK(update_terminal_size_to_fit_container), NULL);

    update_terminal_size_to_fit_container();
    gtk_fixed_put(GTK_FIXED(container), terminal, 0, 0);
    gtk_widget_show(terminal);
    gtk_widget_grab_focus(terminal);
}

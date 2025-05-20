#include "glib.h"
#include <gtk/gtk.h>
#include "../utils/utils.h"

typedef struct {
    gboolean is_dragging;
    Position drag_start_in_button_position;
} DraggingState;
DraggingState dragging_state = { FALSE, { 0, 0 } };

gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    GtkWidget *fixed = GTK_WIDGET(user_data);

    if (event->button == GDK_BUTTON_PRIMARY) {
        gint x, y;
        gtk_widget_translate_coordinates(widget, fixed, 0, 0, &x, &y);

        dragging_state.drag_start_in_button_position.x = event->x;
        dragging_state.drag_start_in_button_position.y = event->y;
        dragging_state.is_dragging = TRUE;
    }

    return TRUE;
}

gboolean on_button_release(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    dragging_state.is_dragging = FALSE;
    return TRUE;
}

gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer user_data) {
    if (!dragging_state.is_dragging) return FALSE;

    GtkWidget *fixed = GTK_WIDGET(user_data);

    gint new_x = event->x_root;
    gint new_y = event->y_root;

    gtk_fixed_move(GTK_FIXED(fixed), widget, new_x - dragging_state.drag_start_in_button_position.x, new_y - dragging_state.drag_start_in_button_position.y);
    return TRUE;
}

void setup_canvas(GtkWidget *window) {
    GtkWidget *container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);

    GtkWidget *drag_button = gtk_button_new_with_label("drag me :3");
    gtk_fixed_put(GTK_FIXED(container), drag_button, 0, 0);

    gtk_widget_set_events(container, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);

    g_signal_connect(drag_button, "button-press-event", G_CALLBACK(on_button_press), container);
    g_signal_connect(drag_button, "button-release-event", G_CALLBACK(on_button_release), container);
    g_signal_connect(drag_button, "motion-notify-event", G_CALLBACK(on_motion_notify), container);
}

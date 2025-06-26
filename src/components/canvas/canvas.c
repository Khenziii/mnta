#include <gtk/gtk.h>
#include "../../utils/utils.h"

typedef struct {
    GtkWidget *canvas;
    GtkWidget *canvas_container;
} EventContext;
static DraggingState dragging_state = { FALSE, { 0, 0 } };

static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    dragging_state.positions.start_global.x = event->x_root;
    dragging_state.positions.start_global.y = event->y_root;
    dragging_state.positions.start_in_button.x = event->x;
    dragging_state.positions.start_in_button.y = event->y;
    dragging_state.is_dragging = TRUE;

    return TRUE;
}

static gboolean on_button_release(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    dragging_state.is_dragging = FALSE;

    int new_x = event->x_root;
    int new_y = event->y_root;
    // TODO: save new coordinates and use them when moving files around
    return TRUE;
}

static gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, EventContext *context) {
    if (!dragging_state.is_dragging) return FALSE;

    GtkWidget *canvas_container = GTK_WIDGET(context->canvas_container);

    gint new_x = event->x_root - dragging_state.positions.start_in_button.x;
    gint new_y = event->y_root - dragging_state.positions.start_in_button.y;

    gtk_fixed_move(GTK_FIXED(canvas_container), context->canvas, new_x, new_y);
    return TRUE;
}

GtkWidget* setup_canvas(GtkWidget *window) {
    GtkWidget *canvas_container = gtk_fixed_new();
    GtkWidget *container = gtk_fixed_new();
    EventContext *context = g_new(EventContext, 1);
    context->canvas_container = canvas_container;
    context->canvas = container;

    gtk_widget_set_events(canvas_container, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
    g_signal_connect(canvas_container, "button-press-event", G_CALLBACK(on_button_press), context);
    g_signal_connect(canvas_container, "button-release-event", G_CALLBACK(on_button_release), context);
    g_signal_connect(canvas_container, "motion-notify-event", G_CALLBACK(on_motion_notify), context);

    gtk_widget_set_has_window(canvas_container, TRUE);
    gtk_widget_set_visible(container, TRUE);
    gtk_widget_show_all(canvas_container);

    gtk_container_add(GTK_CONTAINER(canvas_container), container);
    gtk_container_add(GTK_CONTAINER(window), canvas_container);

    return container;
}

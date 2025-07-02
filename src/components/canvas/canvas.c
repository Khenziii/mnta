#include <gtk/gtk.h>
#include "../../utils/utils.h"
#include "../../context/context.h"
#include "glib.h"

#define KEYBOARD_NAVIGATION_SENSITIVITY 25

typedef struct {
    GtkWidget *canvas;
    GtkWidget *canvas_container;
} EventContext;
static DraggingState dragging_state = { FALSE, { { 0, 0 }, { 0, 0 } } };

static Position *calculate_canvas_position(gint x, gint y) {
    gint delta_x = x - dragging_state.positions.start_global.x;
    gint delta_y = y - dragging_state.positions.start_global.y;
    gint new_x = dragging_state.positions.start_in_button.x + delta_x;
    gint new_y = dragging_state.positions.start_in_button.y + delta_y;

    Position *new_canvas_position = g_new(Position, 1);
    new_canvas_position->x = new_x;
    new_canvas_position->y = new_y;
    return new_canvas_position;
}

static gboolean on_press(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    gtk_widget_translate_coordinates(
        context->canvas,
        context->canvas_container,
        0,
        0,
        &dragging_state.positions.start_in_button.x,
        &dragging_state.positions.start_in_button.y
    );
    dragging_state.positions.start_global.x = event->x_root;
    dragging_state.positions.start_global.y = event->y_root;
    dragging_state.is_dragging = TRUE;

    return TRUE;
}

static gboolean on_release(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    dragging_state.is_dragging = FALSE;

    Position *canvas_position = calculate_canvas_position(event->x_root, event->y_root);
    context_set_current_canvas_position(canvas_position);

    return TRUE;
}

static gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, EventContext *context) {
    if (!dragging_state.is_dragging) return FALSE;

    Position *canvas_position = calculate_canvas_position(event->x_root, event->y_root);
    gtk_fixed_move(
        GTK_FIXED(context->canvas_container),
        context->canvas,
        canvas_position->x,
        canvas_position->y
    );
    return TRUE;
}

GtkWidget* setup_canvas(GtkWidget *window) {
    GtkWidget *canvas_container = gtk_fixed_new();
    GtkWidget *canvas = gtk_fixed_new();
    EventContext *context = g_new(EventContext, 1);
    context->canvas_container = canvas_container;
    context->canvas = canvas;

    gtk_widget_set_events(canvas_container, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
    g_signal_connect(canvas_container, "button-press-event", G_CALLBACK(on_press), context);
    g_signal_connect(canvas_container, "button-release-event", G_CALLBACK(on_release), context);
    g_signal_connect(canvas_container, "motion-notify-event", G_CALLBACK(on_motion_notify), context);

    gtk_widget_set_has_window(canvas_container, TRUE);
    gtk_widget_show(canvas_container);
    gtk_widget_show(canvas);

    gtk_container_add(GTK_CONTAINER(canvas_container), canvas);
    gtk_container_add(GTK_CONTAINER(window), canvas_container);

    context_set_canvas(canvas);
    context_set_canvas_container(canvas_container);

    return canvas;
}

static void keyboard_canvas_navigation(gint delta_x, gint delta_y) {
    AppContext context = context_get();

    Position *new_canvas_position = g_new(Position, 1);
    new_canvas_position->x = context.current_canvas_position->x + delta_x;
    new_canvas_position->y = context.current_canvas_position->y + delta_y;

    context_set_current_canvas_position(new_canvas_position);
    gtk_fixed_move(
        GTK_FIXED(context.canvas_container),
        context.canvas,
        new_canvas_position->x,
        new_canvas_position->y
    );
}

void move_canvas_left() {
    keyboard_canvas_navigation(KEYBOARD_NAVIGATION_SENSITIVITY, 0);
}

void move_canvas_right() {
    keyboard_canvas_navigation(-KEYBOARD_NAVIGATION_SENSITIVITY, 0);
}

void move_canvas_up() {
    keyboard_canvas_navigation(0, KEYBOARD_NAVIGATION_SENSITIVITY);
}

void move_canvas_down() {
    keyboard_canvas_navigation(0, -KEYBOARD_NAVIGATION_SENSITIVITY);
}

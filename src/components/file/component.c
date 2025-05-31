#include "glib.h"
#include "gtk/gtk.h"
#include "../../filesystem/types.h"

#define CLICK_THRESHOLD 5

typedef struct {
    GtkWidget *container;
    Item item;
    void (*on_click)(Item file);
} EventContext;

typedef struct {
    Position start_in_button;
    Position start_global;
} DraggingStatePositions;

typedef struct {
    gboolean is_dragging;
    DraggingStatePositions positions;
} DraggingState;
DraggingState dragging_state = { FALSE, { 0, 0 } };

gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    GtkWidget *fixed = GTK_WIDGET(context->container);

    if (event->button == GDK_BUTTON_PRIMARY) {
        dragging_state.positions.start_global.x = event->x_root;
        dragging_state.positions.start_global.y = event->y_root;
        dragging_state.positions.start_in_button.x = event->x;
        dragging_state.positions.start_in_button.y = event->y;
        dragging_state.is_dragging = TRUE;
    }

    return TRUE;
}

gboolean on_button_release(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    dragging_state.is_dragging = FALSE;

    int new_x = event->x_root;
    int new_y = event->y_root;
    gint total_distance =
        abs(dragging_state.positions.start_global.x - new_x)
        + abs(dragging_state.positions.start_global.y - new_y);
    if (total_distance < CLICK_THRESHOLD) context->on_click(context->item);
    
    return TRUE;
}

gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, EventContext *context) {
    if (!dragging_state.is_dragging) return FALSE;

    GtkWidget *fixed = GTK_WIDGET(context->container);

    gint new_x = event->x_root - dragging_state.positions.start_in_button.x;
    gint new_y = event->y_root - dragging_state.positions.start_in_button.y;
    gtk_fixed_move(GTK_FIXED(fixed), widget, new_x, new_y);
    return TRUE;
}

gboolean on_button_destroy(GtkWidget *widget, EventContext *context) {
    g_free(context);
    return TRUE;
}

void add_file(GtkWidget *container, Item item, void (*on_click)()) {
    GtkWidget *drag_button = gtk_button_new_with_label(item.name);
    EventContext *context = g_new(EventContext, 1);
    context->container = container;
    context->item = item;
    context->on_click = on_click;

    gtk_widget_set_events(drag_button, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
    g_signal_connect(drag_button, "button-press-event", G_CALLBACK(on_button_press), context);
    g_signal_connect(drag_button, "button-release-event", G_CALLBACK(on_button_release), context);
    g_signal_connect(drag_button, "motion-notify-event", G_CALLBACK(on_motion_notify), context);
    g_signal_connect(drag_button, "destroy", G_CALLBACK(on_button_destroy), context);

    gtk_fixed_put(GTK_FIXED(container), drag_button, item.metadata.saved_location.x, item.metadata.saved_location.y);
}

#include "gdk/gdk.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "../../filesystem/filesystem.h"
#include "../../context/context.h"

#define CLICK_THRESHOLD 5

typedef struct {
    GtkWidget *container;
    GtkWidget *button_container;
    Item item;
    void (*on_click)(Item file);
    gboolean draggable;
} EventContext;
static DraggingState dragging_state = { FALSE, { { 0, 0 }, { 0, 0 } } };

static gboolean on_press(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    AppContext app_context = context_get();

    dragging_state.positions.start_global.x = event->x_root;
    dragging_state.positions.start_global.y = event->y_root;
    dragging_state.positions.start_in_button.x = event->x + app_context.current_canvas_position->x;
    dragging_state.positions.start_in_button.y = event->y + app_context.current_canvas_position->y;
    dragging_state.is_dragging = TRUE;

    return TRUE;
}

static gboolean on_release(GtkWidget *widget, GdkEventButton *event, EventContext *context) {
    dragging_state.is_dragging = FALSE;

    AppContext app_context = context_get();
    int new_x = event->x_root - dragging_state.positions.start_in_button.x;
    int new_y = event->y_root - dragging_state.positions.start_in_button.y;

    gint total_distance =
        abs(dragging_state.positions.start_global.x - new_x - dragging_state.positions.start_in_button.x)
        + abs(dragging_state.positions.start_global.y - new_y - dragging_state.positions.start_in_button.y);

    if (total_distance < CLICK_THRESHOLD) {
        context->on_click(context->item);
    } else {
        if (!context->draggable) return TRUE;

        update_items_location(context->item, new_x, new_y);
    }
    
    return TRUE;
}

static gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, EventContext *context) {
    if (!dragging_state.is_dragging) return FALSE;

    gint new_x = event->x_root - dragging_state.positions.start_in_button.x;
    gint new_y = event->y_root - dragging_state.positions.start_in_button.y;
    gtk_fixed_move(GTK_FIXED(context->container), context->button_container, new_x, new_y);
    return TRUE;
}

static gboolean on_destroy(GtkWidget *widget, EventContext *context) {
    g_free(context);
    return TRUE;
}

void add_file(
    GtkWidget *container,
    Item item,
    void (*on_click)(Item file),
    gboolean draggable,
    gboolean add_to_context,
    gboolean add_to_context_as_navigation_back
) {
    GtkWidget *button_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *button = gtk_button_new_with_label(item.name);
    GtkWidget *label = gtk_label_new(NULL);

    gtk_widget_show(button_container);
    gtk_widget_show(button);

    gtk_box_pack_start(GTK_BOX(button_container), button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_container), label, TRUE, TRUE, 0);

    EventContext *context = g_new(EventContext, 1);
    context->container = container;
    context->button_container = button_container;
    context->item = item;
    context->on_click = on_click;
    context->draggable = draggable;

    gtk_widget_set_events(button, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
    g_signal_connect(button, "destroy", G_CALLBACK(on_destroy), context);
    g_signal_connect(button, "button-press-event", G_CALLBACK(on_press), context);
    g_signal_connect(button, "button-release-event", G_CALLBACK(on_release), context);
    if (draggable) g_signal_connect(button, "motion-notify-event", G_CALLBACK(on_motion_notify), context);

    gtk_fixed_put(GTK_FIXED(container), button_container, item.metadata.saved_location.x, item.metadata.saved_location.y);

    if (add_to_context || add_to_context_as_navigation_back) {
        FileWidget *file_widget = g_new(FileWidget, 1);
        file_widget->button=button;
        file_widget->label=label;
        file_widget->container=button_container;
        Item *item_buffer = g_new(Item, 1);
        *item_buffer = item;
        file_widget->item=item_buffer;

        if (add_to_context) context_add_file_widget(file_widget);
        if (add_to_context_as_navigation_back) context_set_previous_directory_navigation(file_widget);
    }
}

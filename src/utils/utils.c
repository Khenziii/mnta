#include <math.h>
#include "../context/context.h"

#define VALID_HINT_KEYS "QWERTYUIOPASDGZXCVBNM"

void hide_navigation() {
    AppContext context = context_get();
    GtkWidget *navigation = context.navigation.previous_directory->container;

    gtk_widget_hide(navigation);
}

void show_navigation() {
    AppContext context = context_get();
    GtkWidget *navigation = context.navigation.previous_directory->container;

    gtk_widget_show(navigation);
}

void free_file_widget(FileWidget *file_widget) {
    gtk_widget_destroy(file_widget->label);
    gtk_widget_destroy(file_widget->button);
    gtk_widget_destroy(file_widget->container);
    free(file_widget->item);
    free(file_widget);
}

gint get_optimal_length_of_hint_string(gint amount, gint current_length) {
    gint amount_of_characters = strlen(VALID_HINT_KEYS);
    gint possible_strings = pow(amount_of_characters, current_length);

    if (possible_strings >= amount) return current_length;
    return get_optimal_length_of_hint_string(amount, current_length+1);
}

// Called by `get_combinations_of_strings`, which abstracts away some arguments.
static void get_combinations_from_string(
    char *current_string,
    guint *added_hints,
    gint *amount_to_generate,
    char ***hints,
    gint *combination_length
) {
    if (*added_hints >= *amount_to_generate) {
        return;
    }

    gint current_length = strlen(current_string);
    if (current_length == *combination_length) {
        strcpy((*hints)[*added_hints], current_string);
        (*added_hints)++;
        return;
    }

    for (int i = 0; i < strlen(VALID_HINT_KEYS); i++) {
        char current_key = VALID_HINT_KEYS[i];
        char current_key_string[2] = { current_key, '\0' };
        char *string = malloc(*combination_length + 1);
        strcpy(string, current_string);
        strcat(string, current_key_string);

        get_combinations_from_string(
            string,
            added_hints,
            amount_to_generate,
            hints,
            combination_length
        );

        free(string);
    }
}

static void get_combinations_of_strings(char ***hints, gint *combination_length, gint *amount_to_generate) {
    guint added_hints = 0;
    char *current_string = malloc(*combination_length + 1);
    strcpy(current_string, "");
    get_combinations_from_string(
        current_string,
        &added_hints,
        amount_to_generate,
        hints,
        combination_length
    );
}

char** get_navigation_hint_strings(gint amount) {
    char **hints = malloc(amount * sizeof(char *));
    // We're about to generate `amount` strings.
    // How do we find out how long they have to be?
    //
    // Assume a scenario in which we can compose our strings of 2 characters.
    //
    // If we were to compose a string of length 1 in this circumstances, we'd have 2 strings:
    // A
    // B
    //
    // If we were to compose a string of length 2 in this circumstances, we'd have 4 strings:
    // AA
    // AB
    // BA
    // BB
    //
    // If we were to compose a string of length 3 in this circumstances, we'd have 8 strings:
    // AAA
    // BAA
    // BBA
    // BBB
    // AAB
    // ABB
    // BAB
    // ABA
    //
    // Notice the pattern?
    //
    // amount of characters | length of string | possible strings
    // 2                    | 1                | 2
    // 2                    | 2                | 4
    // 2                    | 3                | 8
    //
    // The amount of strings that we can generate based on the length of string and amount of possible characters is described by this formula:
    // amount of characters ^ length of string = possible strings
    //
    // In our case, we know the amount of characters (see: `VALID_HINT_KEYS` constant) and how many strings we need.
    gint length_of_string = get_optimal_length_of_hint_string(amount, 1);
    for (int i = 0; i < amount; i++) {
        hints[i] = malloc(length_of_string + 1);
    }
    get_combinations_of_strings(&hints, &length_of_string, &amount);

    return hints;
}

void show_navigation_hints() {
    AppContext context = context_get();
    char **navigation_hint_strings = get_navigation_hint_strings(context.amount_of_current_file_widgets + 1);
    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        FileWidget *file_widget = context.current_file_widgets[i];
        GtkWidget *navigation_hint = file_widget->label;

        gtk_label_set_markup(GTK_LABEL(navigation_hint), navigation_hint_strings[i]);
        gtk_widget_show(navigation_hint);
    }

    GtkWidget *previous_dir_navigation_hint = context.navigation.previous_directory->label;
    gtk_widget_show(previous_dir_navigation_hint);
    gtk_label_set_markup(GTK_LABEL(previous_dir_navigation_hint), navigation_hint_strings[context.amount_of_current_file_widgets]);
}

void hide_navigation_hints() {
    AppContext context = context_get();
    for (int i = 0; i < context.amount_of_current_file_widgets; i++) {
        FileWidget *file_widget = context.current_file_widgets[i];
        GtkWidget *navigation_hint = file_widget->label;

        gtk_widget_hide(navigation_hint);
    }

    GtkWidget *previous_dir_navigation_hint = context.navigation.previous_directory->label;
    gtk_widget_hide(previous_dir_navigation_hint);
}

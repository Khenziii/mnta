#include "stdio.h"
#include "glib.h"
#include "cJSON.h"
#include "./types.h"

static char* format_error_message(char *object_name, char *field_name, char *reason) {
    int needed_chars = snprintf(NULL, 0, "Invalid %s: field `%s` %s", object_name, field_name, reason);
    char *error_message = malloc(needed_chars + 1);
    sprintf(error_message, "Invalid %s: field `%s` %s", object_name, field_name, reason);

    return error_message;
}

static void* on_error(char *error_message, cJSON *cjson_object, void *object_to_free) {
    g_print("%s\n", error_message);

    cJSON_Delete(cjson_object);
    g_free(object_to_free);
    free(error_message);
    return NULL;
}

static cJSON* get_field(cJSON *json_object, char *object_name, char *field_name, void *object) {
    cJSON *field_json = cJSON_GetObjectItem(json_object, field_name);
    if (!field_json) {
        return on_error(format_error_message(object_name, field_name, "is missing!"), json_object, object);
    }

    return field_json;
}

static int* get_int_field(cJSON *json_object, char *object_name, char *field_name, void *object) {
    cJSON *field_json = get_field(json_object, object_name, field_name, object);
    if (!cJSON_IsNumber(field_json)) {
        return on_error(format_error_message(object_name, field_name, "is not a number!"), json_object, object);
    }

    return &field_json->valueint;
}

static char* get_string_field(cJSON *json_object, char *object_name, char *field_name, void *object) {
    cJSON *field_json = get_field(json_object, object_name, field_name, object);
    if (!cJSON_IsString(field_json)) {
        return on_error(format_error_message(object_name, field_name, "is not a string!"), json_object, object);
    }

    return field_json->valuestring;
}

static cJSON* get_object_field(cJSON *json_object, char *object_name, char *field_name, void *object) {
    cJSON *field_json = get_field(json_object, object_name, field_name, object);
    if (!cJSON_IsObject(field_json)) {
        return on_error(format_error_message(object_name, field_name, "is not an object!"), json_object, object);
    }

    return field_json;
}

static cJSON* parse_json_string(char *json_string, char *object_name, void *object) {
    cJSON *root = cJSON_Parse(json_string);

    if (!root) {
        int needed_chars = snprintf(NULL, 0, "Invalid %s: failed to parse JSON.", object_name);
        char *error_message = malloc(needed_chars + 1);
        sprintf(error_message, "Invalid %s: failed to parse JSON.", object_name);

        return on_error(error_message, root, object);
    }

    return root;
}

DefaultFilesPlacement *parse_default_files_placement_json(cJSON *root) {
    DefaultFilesPlacement *default_files_placement = g_new(DefaultFilesPlacement, 1);

    int *files_per_row = get_int_field(root, "DefaultFilesPlacement", "files_per_row", default_files_placement);
    default_files_placement->files_per_row = *files_per_row;

    int *margin_vertical = get_int_field(root, "DefaultFilesPlacement", "margin_vertical", default_files_placement);
    default_files_placement->margin_vertical = *margin_vertical;

    int *margin_horizontal = get_int_field(root, "DefaultFilesPlacement", "margin_horizontal", default_files_placement);
    default_files_placement->margin_horizontal = *margin_horizontal;

    int *container_padding = get_int_field(root, "DefaultFilesPlacement", "container_padding", default_files_placement);
    default_files_placement->container_padding = *container_padding;

    return default_files_placement;
}

Settings* parse_settings_json_string(char *json_string) {
    Settings *settings = g_new(Settings, 1);
    cJSON *root = parse_json_string(json_string, "Settings", settings);

    char *editor_launch_command = get_string_field(root, "Settings", "editor_launch_command", settings);
    settings->editor_launch_command = editor_launch_command;

    cJSON *default_files_placement_json = get_object_field(root, "Settings", "default_files_placement", settings);
    DefaultFilesPlacement *default_files_placement = parse_default_files_placement_json(default_files_placement_json);
    settings->default_files_placement=default_files_placement;

    return settings;
}

Position* parse_position_json(cJSON *root) {
    Position *position = g_new(Position, 1);

    int *x = get_int_field(root, "Position", "x", position);
    position->x = *x;

    int *y = get_int_field(root, "Position", "y", position);
    position->y = *y;

    return position;
}

Metadata* parse_metadata_json(cJSON *root) {
    Metadata *metadata = g_new(Metadata, 1);

    cJSON *saved_location_json = get_object_field(root, "Metadata", "saved_location", metadata);
    Position *saved_location = parse_position_json(saved_location_json);
    metadata->saved_location = *saved_location;

    return metadata;
}

Item* parse_item_json(cJSON *root) {
    Item *item = g_new(Item, 1);

    char *path = get_string_field(root, "Item", "path", item);
    item->path = path;

    char *name = get_string_field(root, "Item", "name", item);
    item->name = name;

    cJSON *metadata_json = get_object_field(root, "Item", "metadata", item);
    Metadata *metadata = parse_metadata_json(metadata_json);
    item->metadata = *metadata;

    return item;
}

UIState* parse_ui_state_json_string(char *json_string) {
    UIState *ui_state = g_new(UIState, 1);
    cJSON *root = parse_json_string(json_string, "UIState", ui_state);

    cJSON *files_json = get_object_field(root, "UIState", "files", ui_state);
    ui_state->files = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)free);

    cJSON *file_entry = NULL;
    cJSON_ArrayForEach(file_entry, files_json) {
        char *key_old = file_entry->string;
        char *key = g_strdup(key_old);

        cJSON *item_json = file_entry;
        Item *item = parse_item_json(item_json);
        
        g_hash_table_insert(ui_state->files, key, item);
    }

    return ui_state;
}

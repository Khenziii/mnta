#include <stdio.h>
#include <sys/stat.h>
#include "glib.h"
#include "cJSON.h"
#include "./filesystem.h"
#include "./types.h"

// All those paths are relative to home directory
#define RELATIVE_CONFIG_DIR_PATH "/.config/mnta"
#define RELATIVE_CONFIG_PATH "/.config/mnta/config.json"
#define RELATIVE_UI_STATE_PATH "/.config/mnta/ui-state.json"

char config_dir_path[200];
char config_path[200];
char ui_state_path[200];

static char* read_file_as_string(char *path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        char error_message[256];
        sprintf(error_message, "Failed to open file %s!", path);
        perror(error_message);

        return NULL;
    }

    // Find end of file (it's length).
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    // Go back to the start of the file.
    rewind(file);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        char error_message[256];
        sprintf(error_message, "Failed to allocate %ld bytes!", length + 1);
        perror(error_message);

        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, length, file);
    buffer[read_size] = '\0';

    fclose(file);
    return buffer;
}

static gboolean file_exists(char *path) {
    gboolean exists;
    FILE* file = fopen(path, "rb");

    if (file) {
        exists = TRUE;
        fclose(file);
    } else {
        exists = FALSE;
    }

    return exists;
}

static gboolean directory_exists(char *path) {
    struct stat info;

    if (stat(path, &info) != 0) {
        return FALSE;
    } else if (info.st_mode & S_IFDIR) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void create_file(char *path) {
    FILE *file = fopen(path, "w");

    if (file == NULL) {
        char error_message[256];
        sprintf(error_message, "Failed to create file %s!", path);
        perror(error_message);
    }

    fclose(file);
}

static void create_file_if_doesnt_exist(char *path, void (*on_file_creation)(char *path)) {
    gboolean config_exists = file_exists(path);

    if (config_exists) return;

    create_file(path);

    if (on_file_creation) on_file_creation(path);
}

static void save_string_to_path(char *path, char *content) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        char error_message[256];
        sprintf(error_message, "Failed to write to file %s!", path);
        perror(error_message);

        return;
    }

    fputs(content, file);

    fclose(file);
}

static void add_default_json(char *path, void (*default_config_generator)(cJSON *json_object)) {
    cJSON *json_object = cJSON_CreateObject();

    default_config_generator(json_object);

    char *json_string = cJSON_Print(json_object);

    cJSON_Delete(json_object);
    save_string_to_path(path, json_string);
}

static void default_settings_generator(cJSON *json_object) {
    cJSON_AddStringToObject(json_object, "editor_launch_command", "nvim");
}

static void add_default_settings(char *path) {
    add_default_json(path, default_settings_generator);
}

static void default_ui_state_generator(cJSON *json_object) {
    cJSON *files = cJSON_CreateArray();
    cJSON_AddItemToObject(json_object, "files", files);
}

static void add_default_ui_state(char *path) {
    add_default_json(path, default_ui_state_generator);
}

static void init() {
    char *home = getenv("HOME");

    strcpy(config_dir_path, home);
    strcat(config_dir_path, RELATIVE_CONFIG_DIR_PATH);
    strcpy(config_path, home);
    strcat(config_path, RELATIVE_CONFIG_PATH);
    strcpy(ui_state_path, home);
    strcat(ui_state_path, RELATIVE_UI_STATE_PATH);

    if (!directory_exists(config_dir_path)) mkdir(config_dir_path, 0755);
}

typedef void (*FileGenerator)(char *path);
typedef void *(*JSONParser)(char *json_string);

static void *get_data(char *path, FileGenerator file_generator, JSONParser parser) {
    init();

    create_file_if_doesnt_exist(path, file_generator);

    char *config_string = read_file_as_string(path);
    return parser(config_string);
}

static void load_ui_state_item_into_cjson_object(void *key, void *item_pointer, void *files_pointer) {
    Item *item = item_pointer;
    cJSON *files = files_pointer;

    cJSON *saved_location = cJSON_CreateObject();
    cJSON_AddNumberToObject(saved_location, "x", item->metadata.saved_location.x);
    cJSON_AddNumberToObject(saved_location, "y", item->metadata.saved_location.y);

    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(metadata, "saved_location", saved_location);

    cJSON *file = cJSON_CreateObject();
    cJSON_AddItemToObject(file, "metadata", metadata);
    cJSON_AddStringToObject(file, "path", item->path);
    cJSON_AddStringToObject(file, "name", item->name);

    cJSON_AddItemToObject(files, item->path, file);
}

static cJSON* convert_ui_state_to_cjson_object(UIState ui_state) {
    cJSON *root = cJSON_CreateObject();
    cJSON *files = cJSON_CreateObject();

    g_hash_table_foreach(ui_state.files, load_ui_state_item_into_cjson_object, files);

    cJSON_AddItemToObject(root, "files", files);
    return root;
}

Settings get_settings() {
    char *path = config_path;
    FileGenerator file_generator = add_default_settings;
    JSONParser parser = (void *)parse_settings_json_string;

    Settings *settings = (Settings *)get_data(path, file_generator, parser);
    return *settings;
}

UIState get_ui_state() {
    char *path = ui_state_path;
    FileGenerator file_generator = add_default_ui_state;
    JSONParser parser = (void *)parse_ui_state_json_string;

    UIState *ui_state = (UIState *)get_data(path, file_generator, parser);
    return *ui_state;
}

void write_item_to_ui_state(Item item) {
    UIState ui_state = get_ui_state();
    Item *item_ptr = g_new(Item, 1);
    *item_ptr = item;
    g_hash_table_insert(ui_state.files, g_strdup(item.path), item_ptr);

    cJSON *ui_state_json = convert_ui_state_to_cjson_object(ui_state);
    char *ui_state_json_string = cJSON_Print(ui_state_json);
    save_string_to_path(ui_state_path, ui_state_json_string);
}

void update_items_location(Item item, int new_x, int new_y) {
    UIState ui_state = get_ui_state();
    Item *item_pointer = (Item *)g_hash_table_lookup(ui_state.files, item.path);
    item_pointer->metadata.saved_location.x = new_x;
    item_pointer->metadata.saved_location.y = new_y;

    cJSON *ui_state_json = convert_ui_state_to_cjson_object(ui_state);
    char *ui_state_json_string = cJSON_Print(ui_state_json);
    save_string_to_path(ui_state_path, ui_state_json_string);
}

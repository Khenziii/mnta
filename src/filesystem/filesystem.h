#include "./types.h"

extern Items process_directory(char path[]);
extern Items switch_directory(char *path);
extern void write_item_to_ui_state(Item item);
extern void update_items_location(Item item, int new_x, int new_y);
extern UIState get_ui_state();
extern Settings get_settings();
extern UIState parse_ui_state_json_string(char *json_string);
extern Settings parse_settings_json_string(char *json_string);
extern gboolean directory_exists(char *path);

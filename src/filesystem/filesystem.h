#include "./types.h"

extern Items process_directory(char path[]);
extern void write_item_to_ui_state(Item item);
extern UIState get_ui_state();
extern Settings get_settings();
extern UIState parse_ui_state_json_string(char *json_string);
extern Settings parse_settings_json_string(char *json_string);

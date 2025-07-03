#include "../filesystem/filesystem.h"

Metadata get_for_path(char absolute_path[], int element_count) {
    Settings settings = get_settings();
    DefaultFilesPlacement *files_config = settings.default_files_placement;
    int amount_of_rows = (element_count - 1) / files_config->files_per_row;

    Metadata metadata;
    metadata.saved_location.x = files_config->container_padding + ((element_count - 1 - amount_of_rows * files_config->files_per_row) * files_config->margin_horizontal);
    metadata.saved_location.y = files_config->container_padding + (amount_of_rows * files_config->margin_vertical);

    return metadata;
}

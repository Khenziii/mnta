#include "assert.h"
#include "../filesystem/types.h"

// TODO: support changing this via config.
#define FILES_PER_ROW 5
#define FILES_MARGIN_HORIZONTAL 300
#define FILES_MARGIN_VERTICAL 100
#define FILES_CONTAINER_PADDING 50

Metadata get_for_path(char absolute_path[], int element_count) {
    Metadata metadata;
    int amount_of_rows = (element_count - 1) / FILES_PER_ROW;

    // TODO: try to get this from local storage.
    metadata.saved_location.x = FILES_CONTAINER_PADDING + ((element_count - 1 - amount_of_rows * FILES_PER_ROW) * FILES_MARGIN_HORIZONTAL);
    metadata.saved_location.y = FILES_CONTAINER_PADDING + (amount_of_rows * FILES_MARGIN_VERTICAL);

    return metadata;
}

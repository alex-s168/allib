#include "fixed_list.h"

#include <string.h>

void FixedList_set(FixedList list, size_t index, const void *data) {
    void *where = FixedList_get(list, index);
    memcpy(where, data, list.stride);
}

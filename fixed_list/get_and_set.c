//
// Created by Alexander Nutz on 17/02/2024.
//

#include "fixed_list.h"

#include <string.h>

void FixedList_set(struct FixedList list, size_t index, void *data) {
    void *where = FixedList_get(list, index);
    memcpy(where, data, list.stride);
}

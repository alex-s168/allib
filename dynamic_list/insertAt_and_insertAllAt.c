//
// Created by Alexander Nutz on 17/02/2024.
//

#include "impl_utils.h"
#include <string.h>

/**
 * @param list Self
 * @param list At what position to insert the element
 * @param data The element to add
 * @return 0 if ok
 */
int DynamicList_insertAllAt(struct DynamicList *list, size_t index,
                             void *data, size_t len) {
    if (DynamicList_reserve(list, len))
        return 1;
    void *elem = FixedList_get(list->fixed, index);
    {
        void *src = elem;
        void *dst = FixedList_get(list->fixed, index + len);
        size_t am = list->fixed.len - index;
        memcpy(dst, src, am * list->fixed.stride);
    }
    {
        void *src = data;
        void *dst = elem;
        size_t am = len;
        memcpy(dst, src, am * list->fixed.stride);
    }
    list->fixed.len += len;
    return 0;
}

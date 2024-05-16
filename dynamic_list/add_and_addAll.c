#include "impl_utils.h"
#include <string.h>

/**
 * @param list Self
 * @param data The pointer to the element
 * @return 0 if ok
 */
int DynamicList_add(struct DynamicList *list, void *data) {
    if (DynamicList_reserve(list, 1))
        return 1;

    void *dest = FixedList_get(list->fixed, list->fixed.len);
    memcpy(dest, data, list->fixed.stride);

    list->fixed.len ++;

    return 0;
}

/**
 * @param list Self
 * @param data The array of elements
 * @return 0 if ok
 */
int DynamicList_addAll(struct DynamicList *list, void *data, size_t len) {
    if (DynamicList_reserve(list, len))
        return 1;

    void *dest = FixedList_get(list->fixed, list->fixed.len);
    memcpy(dest, data, list->fixed.stride * len);

    list->fixed.len += len;

    return 0;
}

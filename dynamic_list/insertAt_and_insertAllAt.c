#include "impl_utils.h"
#include <string.h>

/**
 * @param list Self
 * @param list At what position to insert the element
 * @param data The element to add
 * @return 0 if ok
 */
int DynamicList_insertAllAt(DynamicList *list, size_t index,
                            const void *data, size_t len, size_t stride) {

    DynamicList_resizeRel(list, len);

    void *elem = FixedList_get(list->fixed, index);
    {
        void *src = elem;
        void *dst = FixedList_get(list->fixed, index + len);
        size_t am = list->fixed.len - index;
        memcpy(dst, src, am * list->fixed.stride);
    }

    strided_memcpy(elem, list->fixed.stride,
                   data, stride,
                   len, list->fixed.elSize);
    list->fixed.len += len;
    return 0;
}

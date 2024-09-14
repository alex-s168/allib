#include "impl_utils.h"

/**
 * @param list Self
 * @param stride The size of each element
 * @param cap The initial maximum amount of elements
 * @param data The array of data. needs to be at least @arg cap * @arg stride
 * @return 0 if ok
 */
int DynamicList_init(DynamicList *list, size_t stride,
                     Ally ally, size_t initCap) {

    void *alloc = NULL;
    if (initCap > 0) {
        alloc = ally.impl->alloc(ally.state, initCap * stride);
        if (alloc == NULL)
            return 1;
    }
    list->fixed.elSize = stride;
    list->fixed.stride = stride;
    list->fixed.data = alloc;
    list->cap = initCap;
    list->fixed.len = 0;
    list->ally = ally;

    return 0;
}

void DynamicList_clear(DynamicList *list) {
    list->ally.impl->free(list->ally.state, list->fixed.data, list->cap);
    list->cap = 0;
    list->fixed.len = 0;
    list->fixed.data = NULL;
}

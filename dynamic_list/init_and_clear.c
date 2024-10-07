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
        alloc = yalloc(ally, initCap * stride);
        if (alloc == NULL)
            return 1;
    }
    list->fixed.elSize = stride;
    list->fixed.stride = stride;
    list->fixed.data = alloc;
#if A_CFG_DYN_LIST_OVERALLOC
    list->cap = initCap;
#endif
    list->fixed.len = 0;
    list->ally = ally;

    return 0;
}

void DynamicList_clear(DynamicList *list) {
    yfree(list->ally, list->fixed.data, list->cap);
#if A_CFG_DYN_LIST_OVERALLOC
    list->cap = 0;
#endif
    list->fixed.len = 0;
    list->fixed.data = NULL;
}

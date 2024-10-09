#ifndef KOLLEKTIONS_IMPL_UTILS_H
#define KOLLEKTIONS_IMPL_UTILS_H

#include "dynamic_list.h"
#include "../memlib.h"

static void DynamicList_internal_postRemove(DynamicList *list) {
#if A_CFG_DYN_LIST_OVERALLOC
    if ((list->cap - list->fixed.len) * list->fixed.stride >= 64) { // only resize after 64B fre
        DynamicList_shrink(list);
    }
#endif
}

static void DynamicList_resizeRel(DynamicList *list, signed long change) {
    if (change == 0) return;

#if A_CFG_DYN_LIST_OVERALLOC

    if (change > 0) {
        DynamicList_reserve(list, change);
        list->fixed.len += change;
    } else {
        list->fixed.len += change;
        DynamicList_shrink(list);
    }

#else 

    size_t ne = list->fixed.len + change;

    list->fixed.data = yrealloc(list->ally,
                                list->fixed.data,
                                list->fixed.len * list->fixed.stride,
                                ne * list->fixed.stride);

    list->fixed.len = ne;

#endif
}

#endif //KOLLEKTIONS_IMPL_UTILS_H

#ifndef KOLLEKTIONS_IMPL_UTILS_H
#define KOLLEKTIONS_IMPL_UTILS_H

#include "dynamic_list.h"
#include "../memlib.h"

static void DynamicList_internal_postRemove(DynamicList *list) {
    if ((list->cap - list->fixed.len) * list->fixed.stride >= 64) { // only resize after 64B fre
        DynamicList_shrink(list);
    }
}

#endif //KOLLEKTIONS_IMPL_UTILS_H

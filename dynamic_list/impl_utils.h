#ifndef KOLLEKTIONS_IMPL_UTILS_H
#define KOLLEKTIONS_IMPL_UTILS_H

#include "dynamic_list.h"

static void DynamicList_internal_postRemove(struct DynamicList *list) {
    if (list->cap - list->fixed.len > 4) {
        DynamicList_shrink(list);
    }
}

#endif //KOLLEKTIONS_IMPL_UTILS_H

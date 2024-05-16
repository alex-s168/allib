#include "impl_utils.h"

/**
 * Reserves space for at least x additional elements on top of the length of the list
 * @param list Self
 * @param additional The amount of additional elements to reserve on top of the length of the list
 * @return 0 if ok
 */
int DynamicList_reserve(struct DynamicList *list, size_t additional) {
    size_t new = list->fixed.len + additional;

    if (new <= list->cap)
        return 0;

    new = (new + 3) & ~3;

    list->fixed.data = list->ally.impl->realloc(list->ally.state,
                                                list->fixed.data,
                                                list->cap * list->fixed.stride,
                                                new * list->fixed.stride);
    list->cap = new;

    return list->fixed.data == NULL;
}

/**
 * Reserves space for exactly x additional elements on top of the length of the list
 * @param list Self
 * @param additional The amount of additional elements to reserve on top of the length of the list
 * @return 0 if ok
 */
int DynamicList_reserveExact(struct DynamicList *list, size_t additional) {
    size_t new = list->fixed.len + additional;

    if (new <= list->cap)
        return 0;

    list->fixed.data = list->ally.impl->realloc(list->ally.state,
                                                list->fixed.data,
                                                list->cap * list->fixed.stride,
                                                new * list->fixed.stride);
    list->cap = new;

    return list->fixed.data == NULL;
}

/**
 * Shrinks the list allocation to be as big as the list.
 * @param list Self
 * @return 0 if ok
 */
int DynamicList_shrink(struct DynamicList *list) {
    if (list->cap == list->fixed.len)
        return 0;

    list->fixed.data = list->ally.impl->realloc(list->ally.state,
                                                list->fixed.data,
                                                list->cap * list->fixed.stride,
                                                list->fixed.len * list->fixed.stride);
    list->cap = list->fixed.len;

    return list->fixed.data == NULL;
}

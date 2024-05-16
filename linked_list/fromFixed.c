//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

#include <string.h>

static struct DoubleLinkedElement *allocElem(struct DoubleLinkedList *list) {
    return list->ally.impl->alloc(list->ally.state,
                                  sizeof(struct DoubleLinkedElement) + list->stride);
}

/**
 * Creates a new DoubleLinkedList and copies [fixed] into it.
 * @param list
 * @param fixed
 */
void DoubleLinkedList_fromFixed(struct DoubleLinkedList *list, Ally ally, struct FixedList fixed) {
    list->stride = fixed.stride;
    list->size = fixed.len;
    list->ally = ally;
    struct DoubleLinkedElement *prev = NULL;
    for (size_t i = 0; i < fixed.len; i ++) {
        struct DoubleLinkedElement *curr = allocElem(list);
        memcpy(DoubleLinkedElement_data(curr), fixed.data, fixed.stride);
        if (i == 0)
            list->start = curr;
        else
            prev->next = curr;
        curr->prev = prev;
        prev = curr;
        fixed.data += fixed.stride;
    }
    list->end = prev;
}

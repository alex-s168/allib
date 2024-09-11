#include "impl.h"

#include <string.h>

/**
 * Creates a new DoubleLinkedList and copies [fixed] into it.
 * @param list
 * @param fixed
 */
void DoubleLinkedList_copyFromFixed(DoubleLinkedList *list, Ally ally, FixedList fixed) {
    list->elSize = fixed.elSize;
    list->size = fixed.len;
    list->ally = ally;
    struct DoubleLinkedElement *prev = NULL;
    for (size_t i = 0; i < fixed.len; i ++) {
        struct DoubleLinkedElement *curr = allocElem(list);
        memcpy(DoubleLinkedElement_data(curr), fixed.data, fixed.elSize);
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

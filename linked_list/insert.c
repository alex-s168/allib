#include "linked_list.h"

#include <string.h>

static struct DoubleLinkedElement *allocElem(struct DoubleLinkedList *list) {
    return list->ally.impl->alloc(list->ally.state,
                                  sizeof(struct DoubleLinkedElement) + list->stride);
}

void DoubleLinkedList_insertAfter(struct DoubleLinkedList *list, struct DoubleLinkedElement *after, void *data) {
    struct DoubleLinkedElement *elem = allocElem(list);
    memcpy(DoubleLinkedElement_data(elem), data, list->stride);

    elem->prev = after;
    if (after == NULL) {
        elem->next = list->start;
        if (list->start == NULL)
            list->start = elem;
    } else {
        elem->next = after->next;
        after->next = elem;
    }

    if (after == list->end)
        list->end = elem;

    list->size += 1;
}



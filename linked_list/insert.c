#include "impl.h"

#include <string.h>

void DoubleLinkedList_insertAfter(DoubleLinkedList *list, struct DoubleLinkedElement *after, const void *data) {
    struct DoubleLinkedElement *elem = allocElem(list);
    memcpy(DoubleLinkedElement_data(elem), data, list->elSize);

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



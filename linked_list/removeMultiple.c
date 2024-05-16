//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

void DoubleLinkedList_removeMultiple(struct DoubleLinkedList *list, struct DoubleLinkedElement *first, struct DoubleLinkedElement *last, size_t removed) {
    if (first->prev == NULL) {
        // first element in list
        list->start = last->next;
    } else {
        // not first element in list
        first->prev->next = last->next;
    }

    if (last->next == NULL) {
        // last element in list
        list->end = first->prev;
    } else {
        // not last element in list
        last->next->prev = first->prev;
    }

    while (first != NULL) {
        list->ally.impl->free(list->ally.state, first, sizeof(struct DoubleLinkedElement) + list->stride);

        if (first == last)
            break;
        first = first->next;
    }

    list->size -= removed;
}

//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

void DoubleLinkedList_remove(struct DoubleLinkedList *list, struct DoubleLinkedElement *element) {
    if (element->prev == NULL) {
        // first element in list
        list->start = element->next;
    } else {
        // not first element in list
        element->prev->next = element->next;
    }

    if (element->next == NULL) {
        // last element in list
        list->end = element->prev;
    } else {
        // not last element in list
        element->next->prev = element->prev;
    }

    list->ally.impl->free(list->ally.state, element, sizeof(struct DoubleLinkedElement) + list->stride);
    list->size --;
}

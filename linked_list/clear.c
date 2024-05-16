//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

/**
 * @param list Self
 */
void DoubleLinkedList_clear(struct DoubleLinkedList *list) {
    struct DoubleLinkedElement *elem = list->start;
    while (elem != NULL) {
        struct DoubleLinkedElement *next = elem->next;
        list->ally.impl->free(list->ally.state, elem, sizeof(struct DoubleLinkedElement) + list->stride);
        elem = next;
    }
    list->start = NULL;
    list->end = NULL;
    list->size = 0;
}

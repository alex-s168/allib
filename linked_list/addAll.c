//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAll(struct DoubleLinkedList *list, struct DoubleLinkedList *data) {
    if (data->size == 0)
        return;
    list->size += data->size;
    if (list->end != NULL) {
        list->end->next = data->start;
        data->start->prev = list->end;
    }
    list->end = data->end;
    data->end = NULL;
    data->start = NULL;
    data->size = 0;
}

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAllFront(struct DoubleLinkedList *list, struct DoubleLinkedList *data) {
    if (data->size == 0)
        return;
    list->size += data->size;
    if (list->start != NULL) {
        list->start->prev = data->end;
        data->end->next = list->start;
    }
    list->start = data->start;
    data->end = NULL;
    data->start = NULL;
    data->size = 0;
}

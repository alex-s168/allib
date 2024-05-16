//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

/**
 * @param list Self
 * @param data The pointer to the element
 */
void DoubleLinkedList_add(struct DoubleLinkedList *list, void *data) {
    DoubleLinkedList_insertAfter(list, list->end, data);
}

/**
 * @param list Self
 * @param data The pointer to the element
 */
void DoubleLinkedList_addFront(struct DoubleLinkedList *list, void *data) {
    DoubleLinkedList_insertAfter(list, NULL, data);
}

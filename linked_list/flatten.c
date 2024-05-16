//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

/**
 * @param list Self
 * @param dest The destination list where to add all the elements of this list
 */
void DoubleLinkedList_flattenInto(struct DoubleLinkedList *list, struct DynamicList *dest) {
    DynamicList_reserve(dest, list->size);
    struct DoubleLinkedElement *elem = list->start;
    while (elem != NULL) {
        void *data = DoubleLinkedElement_data(elem);
        DynamicList_add(dest, data);
        elem = elem->next;
    }
}

#include "linked_list.h"

/**
 * @param list Self
 * @param dest The destination list where to add all the elements of this list
 */
void DoubleLinkedList_flattenInto(DynamicList *dest, const DoubleLinkedList *list) {
    DynamicList_reserve(dest, list->size);
    struct DoubleLinkedElement *elem = list->start;
    while (elem != NULL) {
        void *data = DoubleLinkedElement_data(elem);
        DynamicList_add(dest, data);
        elem = elem->next;
    }
}

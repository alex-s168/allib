#include "linked_list.h"

void DoubleLinkedList_add(DoubleLinkedList *list, const void *data) {
    DoubleLinkedList_insertAfter(list, list->end, data);
}

void DoubleLinkedList_addFront(DoubleLinkedList *list, const void *data) {
    DoubleLinkedList_insertAfter(list, NULL, data);
}

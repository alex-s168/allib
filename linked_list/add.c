#include "linked_list.h"

void DoubleLinkedList_add(DoubleLinkedList *list, const void *data) {
    DoubleLinkedList_insertAfter(list, list->end, data);
}

void DoubleLinkedList_addFront(DoubleLinkedList *list, const void *data) {
    DoubleLinkedList temp;
    DoubleLinkedList_copyFromFixed(&temp, list->ally, (FixedList) {
            .elSize = list->elSize,
            .stride = list->elSize,
            .data = (void*) data, 
            .len = 1
    });
    DoubleLinkedList_addAllFrontClearOld(list, &temp);
}

#include "impl.h"

void DoubleLinkedList_remove(DoubleLinkedList *list, struct DoubleLinkedElement *element) {
    if (list->start == element) {
        // first element in list
        list->start = element->next;
    } else {
        // not first element in list
        element->prev->next = element->next;
    }

    if (list->end == element) {
        // last element in list
        list->end = element->prev;
    } else {
        // not last element in list
        element->next->prev = element->prev;
    }

    freeElem(list, element);
    list->size --;
}

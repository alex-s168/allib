#include "impl.h"

void DoubleLinkedList_removeMultiple(DoubleLinkedList *list, struct DoubleLinkedElement *first, struct DoubleLinkedElement *last, size_t removed) {
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
        freeElem(list, first);

        if (first == last)
            break;
        first = first->next;
    }

    list->size -= removed;
}

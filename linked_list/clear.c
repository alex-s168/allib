#include "impl.h"

void DoubleLinkedList_clear(DoubleLinkedList *list) {
    struct DoubleLinkedElement *elem = list->start;
    while (elem != NULL) {
        struct DoubleLinkedElement *next = elem->next;
        freeElem(list, elem);
        elem = next;
    }
    list->start = NULL;
    list->end = NULL;
    list->size = 0;
}

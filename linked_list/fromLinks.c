#include "impl.h"

#include <string.h>

void DoubleLinkedList_copyFromLinks(DoubleLinkedList *list, const struct DoubleLinkedElement *first, const struct DoubleLinkedElement *last, size_t elSize, Ally ally) {
    list->ally = ally;
    list->elSize = elSize;
    if (first == NULL || last == NULL) {
        list->start = NULL;
        list->end = NULL;
        list->size = 0;
        return;
    }
    struct DoubleLinkedElement *prev = NULL;
    size_t i = 0;
    while (1) {
        struct DoubleLinkedElement *curr = allocElem(list);
        memcpy(curr, first, sizeof(struct DoubleLinkedElement) + list->elSize);
        if (first->next == NULL)
            break;
        first = first->next;
        if (first > last)
            break;
        if (i == 0)
            list->start = curr;
        else
            prev->next = curr;
        curr->prev = prev;
        prev = curr;
        i ++;
    }
    list->size = i;
}

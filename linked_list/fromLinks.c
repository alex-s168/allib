//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

#include <string.h>

static struct DoubleLinkedElement *allocElem(struct DoubleLinkedList *list) {
    return list->ally.impl->alloc(list->ally.state,
                                  sizeof(struct DoubleLinkedElement) + list->stride);
}

void DoubleLinkedList_fromLinks(struct DoubleLinkedList *list, const struct DoubleLinkedElement *first, const struct DoubleLinkedElement *last, size_t stride, Ally ally) {
    list->ally = ally;
    list->stride = stride;
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
        memcpy(curr, first, sizeof(struct DoubleLinkedElement) + list->stride);
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

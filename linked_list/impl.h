#include "linked_list.h"

static struct DoubleLinkedElement *allocElem(DoubleLinkedList *list) {
    return (struct DoubleLinkedElement *)
        list->ally.impl->alloc(list->ally.state,
                sizeof(struct DoubleLinkedElement) + list->elSize);
}

static void freeElem(DoubleLinkedList *list, struct DoubleLinkedElement * elem) {
    list->ally.impl->free(list->ally.state, elem,
                          sizeof(struct DoubleLinkedElement) + list->elSize);
}

#include "linked_list.h"

static struct DoubleLinkedElement *allocElem(DoubleLinkedList *list) {
    return (struct DoubleLinkedElement *)
        yalloc(list->ally,
                sizeof(struct DoubleLinkedElement) + list->elSize);
}

static void freeElem(DoubleLinkedList *list, struct DoubleLinkedElement * elem) {
    yfree(list->ally, elem,
                          sizeof(struct DoubleLinkedElement) + list->elSize);
}

#include "../linked_list/linked_list.h"

static int* elemAt(DoubleLinkedList *list, size_t i) {
    struct DoubleLinkedElement *elem = DoubleLinkedList_linkAt(list, i);
    if (elem == NULL)
        return NULL;
    int *ptr = (int*)DoubleLinkedElement_data(elem);
    return ptr;
}

#include <stdio.h>

int main() {
    DoubleLinkedList list;
    DoubleLinkedList_init(&list, sizeof(int), getLIBCAlloc());

    DoubleLinkedList_add(&list, (int[]) { 2 });
    DoubleLinkedList_add(&list, (int[]) { 4 });
    DoubleLinkedList_add(&list, (int[]) { 8 }); 
    DoubleLinkedList_addFront(&list, (int[]) { 1 });

    if (list.size != 4)
        return 1;

    struct DoubleLinkedElement *elem = DoubleLinkedList_linkAt(&list, 1);
    if (elem == NULL)
        return 2;
    DoubleLinkedList_remove(&list, elem);

    if (list.size != 3)
        return 3;

    int *e = elemAt(&list, 0);
    if (e == NULL)
        return 4;
    if (*e != 1)
        return 5;

    e = elemAt(&list, 1);
    if (e == NULL)
        return 6;
    if (*e != 4)
        return 7;

    e = elemAt(&list, 2);
    if (e == NULL)
        return 8;
    if (*e != 8)
        return 9;

    DoubleLinkedList_clear(&list);
    return 0;
}

#include "linked_list.h"

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAllClearOld(DoubleLinkedList *list, DoubleLinkedList *data) {
    assert(Ally_eq(list->ally, data->ally));
    if (data->size == 0)
        return;
    list->size += data->size;
    if (list->end != NULL) {
        list->end->next = data->start;
        data->start->prev = list->end;
    }
    list->end = data->end;
    data->end = NULL;
    data->start = NULL;
    data->size = 0;
    assert(list->ally.impl == data->ally.impl);
}

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAllFrontClearOld(DoubleLinkedList *list, DoubleLinkedList *data) {
    assert(Ally_eq(list->ally, data->ally));
    if (data->size == 0)
        return;
    list->size += data->size;
    if (list->start != NULL) {
        list->start->prev = data->end;
        data->end->next = list->start;
    }
    list->start = data->start;
    data->end = NULL;
    data->start = NULL;
    data->size = 0;
}

/**
 * @param list Self
 * @param data The list of elements.
 * use [DoubleLinkedList_addAllClearOld] whenever possible instead!
 */
void DoubleLinkedList_addAllCopy(DoubleLinkedList *list, const DoubleLinkedList *data) {
    DoubleLinkedList temp;
    DoubleLinkedList_copy(&temp, data, list->ally); // copy all nodes ; other parts of copy are basically free 
    DoubleLinkedList_addAllClearOld(list, &temp);
}

/**
 * @param list Self
 * @param data The list of elements.
 * use [DoubleLinkedList_addAllFrontClearOld] whenever possible instead!
*/
void DoubleLinkedList_addAllFrontCopy(DoubleLinkedList *list, const DoubleLinkedList *data) {
    DoubleLinkedList temp;
    DoubleLinkedList_copy(&temp, data, list->ally); // copy all nodes ; other parts of copy are basically free 
    DoubleLinkedList_addAllFrontClearOld(list, &temp);
}

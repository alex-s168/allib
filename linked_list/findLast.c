//
// Created by Alexander Nutz on 21/02/2024.
//

#include "linked_list.h"

#include <string.h>

struct DoubleLinkedFindResult DoubleLinkedList_findLast(struct DoubleLinkedList *list, void *element) {
    size_t index = list->size - 1;
    struct DoubleLinkedElement *elem = list->end;
    while (elem != NULL) {
        void *data = DoubleLinkedElement_data(elem);
        if (memcmp(data, element, list->stride) == 0) {
            return (struct DoubleLinkedFindResult) {
                    .index = index,
                    .data = data,
                    .link = elem
            };
        }

        elem = elem->prev;
        index --;
    }

    return (struct DoubleLinkedFindResult) {
            .index = -1,
            .data = NULL,
            .link = NULL
    };
}

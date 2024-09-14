#include "linked_list.h"
#include <assert.h>

struct DoubleLinkedElement *DoubleLinkedList_linkAt(const DoubleLinkedList *list, size_t index) {
    if (index >= list->size)
        return NULL;

    size_t sizeH = list->size / 2;
    if (index < sizeH) {
        // Low half
        size_t curr = 0;
        struct DoubleLinkedElement *elem = list->start;
        assert(elem);
        while (curr != index) {
            elem = elem->next;
            curr ++;
        }
        return elem;
    } else {
        // High half
        size_t curr = list->size - 1;
        struct DoubleLinkedElement *elem = list->end;
        assert(elem);
        while (curr != index) {
            elem = elem->prev;
            curr --;
        }
        return elem;
    }
}

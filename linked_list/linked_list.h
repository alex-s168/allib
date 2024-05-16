//
// Created by Alexander Nutz on 21/02/2024.
//

#ifndef KOLLEKTIONS_LINKED_LIST_H
#define KOLLEKTIONS_LINKED_LIST_H

#include <stddef.h>

#include "../dynamic_list/dynamic_list.h"
#include "../kallok/kallok.h"
#include "../attrib.h"

struct DoubleLinkedElement {
    struct DoubleLinkedElement *next;
    struct DoubleLinkedElement *prev;
};

static inline void *DoubleLinkedElement_data(struct DoubleLinkedElement *elem) {
    return elem + sizeof(struct DoubleLinkedElement);
}

static struct DoubleLinkedElement *DoubleLinkedElement_goRight(struct DoubleLinkedElement *elem, size_t amount) {
    for (size_t i = 0; i < amount; i ++) {
        if (elem == NULL)
            return NULL;
        elem = elem->next;
    }
    return elem;
}

static struct DoubleLinkedElement *DoubleLinkedElement_goLeft(struct DoubleLinkedElement *elem, size_t amount) {
    for (size_t i = 0; i < amount; i ++) {
        if (elem == NULL)
            return NULL;
        elem = elem->prev;
    }
    return elem;
}

struct DoubleLinkedList {
    size_t size;
    size_t stride;
    struct DoubleLinkedElement *start;
    struct DoubleLinkedElement *end;

INTERNAL
    Ally ally;
};

static void DoubleLinkedList_init(struct DoubleLinkedList *list, size_t stride, Ally ally) {
    list->size = 0;
    list->stride = stride;
    list->start = NULL;
    list->end = NULL;
    list->ally = ally;
}

/**
 * Creates a new DoubleLinkedList and copies [fixed] into it.
 * @param list
 * @param fixed
 */
void DoubleLinkedList_fromFixed(struct DoubleLinkedList *list,
                                Ally ally,
                                struct FixedList fixed);

/**
 * Creates a new DoubleLinkedList and copies the linked elements from [first] (inclusive) to [last] (inclusive) into it
 * @param list
 * @param first
 * @param last
 * @param stride
 * @param ally
 */
void DoubleLinkedList_fromLinks(struct DoubleLinkedList *list,
                                const struct DoubleLinkedElement *first,
                                const struct DoubleLinkedElement *last,
                                size_t stride,
                                Ally ally);

/**
 * Creates a new DoubleLinkedList and copies all the data from [src] into it.
 * Does not clear source.
 * @param dest
 * @param src
 */
static void DoubleLinkedList_copy(struct DoubleLinkedList *dest,
                                  struct DoubleLinkedList *src,
                                  Ally ally) {
    DoubleLinkedList_fromLinks(dest, src->start, src->end, src->stride, ally);
}

/**
 * @param list Self
 */
void DoubleLinkedList_clear(struct DoubleLinkedList *list);

struct DoubleLinkedElement *DoubleLinkedList_linkAt(struct DoubleLinkedList *list, size_t index);

struct DoubleLinkedFindResult {
    struct DoubleLinkedElement *link;
    void *data;
    size_t index;
};

struct DoubleLinkedFindResult DoubleLinkedList_find(struct DoubleLinkedList *list, void *element);

struct DoubleLinkedFindResult DoubleLinkedList_findLast(struct DoubleLinkedList *list, void *element);

/**
 * Removes a link from the list. The link will be deallocated afterwards.
 * @param list
 * @param element
 */
void DoubleLinkedList_remove(struct DoubleLinkedList *list, struct DoubleLinkedElement *element);

/**
 * Removes all links from [first] (inclusive) to [end] (inclusive) from the list and deallocates the links.
 * @param list
 * @param first
 * @param last
 * @param removed
 */
void DoubleLinkedList_removeMultiple(struct DoubleLinkedList *list, struct DoubleLinkedElement *first, struct DoubleLinkedElement *last, size_t removed);

/**
 * @param list self
 * @param after (optional): after which node to insert
 * @param data The pointer to the element
 */
void DoubleLinkedList_insertAfter(struct DoubleLinkedList *list, struct DoubleLinkedElement *after, void *data);

/**
 * @param list Self
 * @param data The pointer to the element
 */
void DoubleLinkedList_add(struct DoubleLinkedList *list, void *data);

/**
 * @param list Self
 * @param data The pointer to the element
 */
void DoubleLinkedList_addFront(struct DoubleLinkedList *list, void *data);

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAll(struct DoubleLinkedList *list, struct DoubleLinkedList *data);

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAllFront(struct DoubleLinkedList *list, struct DoubleLinkedList *data);

/**
 * @param list Self
 * @param dest The destination list where to add all the elements of this list
 */
void DoubleLinkedList_flattenInto(struct DoubleLinkedList *list, struct DynamicList *dest);

#endif //KOLLEKTIONS_LINKED_LIST_H

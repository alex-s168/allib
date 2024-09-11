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

typedef struct {
    size_t size;
    size_t elSize;
    struct DoubleLinkedElement *start;
    struct DoubleLinkedElement *end;

INTERNAL
    Ally ally;
} DoubleLinkedList;

static void DoubleLinkedList_init(DoubleLinkedList *list, size_t elSize, Ally ally) {
    list->size = 0;
    list->elSize = elSize;
    list->start = NULL;
    list->end = NULL;
    list->ally = ally;
}

/**
 * Creates a new DoubleLinkedList and copies [fixed] into it.
 * @param list
 * @param fixed
 */
void DoubleLinkedList_copyFromFixed(DoubleLinkedList *list, Ally ally, FixedList fixed);

/**
 * Creates a new DoubleLinkedList and copies the linked elements from [first] (inclusive) to [last] (inclusive) into it
 * @param list
 * @param first
 * @param last
 * @param elSize
 * @param ally
 */
void DoubleLinkedList_copyFromLinks(DoubleLinkedList *list,
                                    const struct DoubleLinkedElement *first,
                                    const struct DoubleLinkedElement *last,
                                    size_t elSize,
                                    Ally ally);

/**
 * Creates a new DoubleLinkedList and copies all the data from [src] into it.
 * Does not clear source.
 * @param dest uninitialized ; will be initialized afterwards
 * @param src
 */
static void DoubleLinkedList_copy(DoubleLinkedList *dest,
                                  const DoubleLinkedList *src,
                                  Ally ally) {

    DoubleLinkedList_copyFromLinks(dest, src->start, src->end, src->elSize, ally);
}

/**
 * @param list Self
 */
void DoubleLinkedList_clear(DoubleLinkedList *list);

struct DoubleLinkedElement *DoubleLinkedList_linkAt(const DoubleLinkedList *list, size_t index);

struct DoubleLinkedFindResult {
    struct DoubleLinkedElement *link;
    void *data;
    size_t index;
};

struct DoubleLinkedFindResult DoubleLinkedList_find(const DoubleLinkedList *list, const void *element);

struct DoubleLinkedFindResult DoubleLinkedList_findLast(const DoubleLinkedList *list, const void *element);

/**
 * Removes a link from the list. The link will be deallocated afterwards.
 * @param list
 * @param element
 */
void DoubleLinkedList_remove(DoubleLinkedList *list, struct DoubleLinkedElement *element);

/**
 * Removes all links from [first] (inclusive) to [end] (inclusive) from the list and deallocates the links.
 * @param list
 * @param first
 * @param last
 * @param removed count of elements from [first] (inclusive) to [end] (inclusive)
 */
void DoubleLinkedList_removeMultiple(DoubleLinkedList *list,
                                     struct DoubleLinkedElement *first, struct DoubleLinkedElement *last, 
                                     size_t removed);

/**
 * @param list self
 * @param after (optional): after which node to insert
 * @param data The pointer to the element
 */
void DoubleLinkedList_insertAfter(DoubleLinkedList *list, struct DoubleLinkedElement *after,
                                  const void *data);

/**
 * @param list Self
 * @param data The pointer to the element
 */
void DoubleLinkedList_add(DoubleLinkedList *list, const void *data);

/**
 * @param list Self
 * @param data The pointer to the element
 */
void DoubleLinkedList_addFront(DoubleLinkedList *list, const void *data);

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAllClearOld(DoubleLinkedList *list, DoubleLinkedList *data);

/**
 * @param list Self
 * @param data The list of elements.
 * The allocator needs to be exactly the same (a.impl == b.impl && a.state == b.state)
 * The old list will be cleared.
 */
void DoubleLinkedList_addAllFrontClearOld(DoubleLinkedList *list, DoubleLinkedList *data);

/**
 * @param list Self
 * @param data The list of elements.
 * use [DoubleLinkedList_addAllClearOld] whenever possible instead!
 */
void DoubleLinkedList_addAllCopy(DoubleLinkedList *list, const DoubleLinkedList *data);

/**
 * @param list Self
 * @param data The list of elements.
 * use [DoubleLinkedList_addAllFrontClearOld] whenever possible instead!
*/
void DoubleLinkedList_addAllFrontCopy(DoubleLinkedList *list, const DoubleLinkedList *data);

/**
 * @param list Self
 * @param dest The destination list where to add all the elements of this list
 */
void DoubleLinkedList_flattenInto(DynamicList *dest, const DoubleLinkedList *list);

/** 
 * @param list Self ; needs to live at least as long as AnyList 
 */
AnyList DoubleLinkedList_asAny(DoubleLinkedList *list);

/** 
 * @param list Self ; needs to live at least as long as AnyList 
 */
MutAnyList DoubleLinkedList_asMutAny(DoubleLinkedList *list);

extern MutAnyListImpl DoubleLinkedList_mutAnyListImpl;
extern AnyListImpl DoubleLinkedList_anyListImpl;

#endif //KOLLEKTIONS_LINKED_LIST_H

#ifndef KOLLEKTIONS_DYNAMIC_LIST_H
#define KOLLEKTIONS_DYNAMIC_LIST_H

#include <stddef.h>

#include "../lists/AnyList.h"
#include "../fixed_list/fixed_list.h"
#include "../kallok/kallok.h"
#include "../attrib.h"

struct DynamicList {
    struct FixedList fixed;
    size_t cap;

INTERNAL
    Ally ally;
};

// THE RETURNED LIST STILL DEPENDS ON THE LIST INPUT!!!
MutAnyList DynamicList_asAny(struct DynamicList *list);

/**
 * @param list Self
 * @param stride The size of each element
 * @param cap The initial maximum amount of elements
 * @param data The array of data. needs to be at least @arg cap * @arg stride
 * @return 0 if ok
 */
int DynamicList_init(struct DynamicList *list, size_t stride,
                     Ally ally, size_t initCap);

/**
 * @param list Self
 */
void DynamicList_clear(struct DynamicList *list);

/**
 * @param list Self
 * @param data The pointer to the element
 * @return 0 if ok
 */
int DynamicList_add(struct DynamicList *list, void *data);

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_addp(struct DynamicList *list);

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_setp(struct DynamicList *list, size_t pos);

/**
 * @param list Self
 * @param data The array of elements
 * @return 0 if ok
 */
int DynamicList_addAll(struct DynamicList *list, void *data, size_t len);

/**
 * @param list Self
 * @param pos At which index to remove.
 */
void DynamicList_removeAt(struct DynamicList *list, size_t pos);

/**
 * @param list Self
 * @param first The start index where to remove
 * @param last The end index where to remove
 */
void DynamicList_removeRange(struct DynamicList *list,
                             size_t first, size_t last);

/**
 * @param list Self
 * @param list At what position to insert the element
 * @param data The element to add
 * @return 0 if ok
 */
int DynamicList_insertAllAt(struct DynamicList *list, size_t index,
                            void *data, size_t len);

/**
 * @param list Self
 * @param list At what position to insert the element
 * @param data The element to add
 * @return 0 if ok
 */
static inline int DynamicList_insertAt(struct DynamicList *list, size_t index,
                                       void *data) {
    return DynamicList_insertAllAt(list, index, data, 1);
}

/**
 * Reserves space for at least x additional elements on top of the length of the list
 * @param list Self
 * @param additional The amount of additional elements to reserve on top of the length of the list
 * @return 0 if ok
 */
int DynamicList_reserve(struct DynamicList *list, size_t additional);

/**
 * Reserves space for exactly x additional elements on top of the length of the list
 * @param list Self
 * @param additional The amount of additional elements to reserve on top of the length of the list
 * @return 0 if ok
 */
int DynamicList_reserveExact(struct DynamicList *list, size_t additional);

/**
 * Shrinks the list allocation to be as big as the list.
 * @param list Self
 * @return 0 if ok
 */
int DynamicList_shrink(struct DynamicList *list);

#endif //KOLLEKTIONS_DYNAMIC_LIST_H

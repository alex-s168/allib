#ifndef KOLLEKTIONS_FIXED_LIST_H
#define KOLLEKTIONS_FIXED_LIST_H

#include <stddef.h>

#include "../lists/AnyList.h"

/**
 * A basic non-resizable read-write list.
 */
typedef struct {
    size_t stride;
    size_t elSize;
    size_t len;
    void *data;
} FixedList;

#define INDEX_NOT_FOUND (-1)

/**
 * @param list Self 
 * @return immutable any list that DEPENDS ON TEH GIVEN PTR  
 */
AnyList FixedList_asAny(const FixedList * list);

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. INDEX_NOT_FOUND if not found.
 */
int FixedList_indexOf(FixedList list, const void *data);

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. INDEX_NOT_FOUND if not found.
 */
int FixedList_indexOfLast(FixedList list, const void *data);

/**
 * @param list Self
 * @param index The index of the element
 * @return The pointer to the element
 */
static inline void *FixedList_get(FixedList list, size_t index) {
    return (char *) list.data + list.stride * index;
}

/**
 * @param list Self
 * @param index The index of the element
 * @param data The pointer to the element
 */
void FixedList_set(FixedList list, size_t index, const void *data);

extern AnyListImpl FixedList_anyListImpl;

#endif //KOLLEKTIONS_FIXED_LIST_H

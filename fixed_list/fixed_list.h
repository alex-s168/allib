//
// Created by Alexander Nutz on 17/02/2024.
//

#ifndef KOLLEKTIONS_FIXED_LIST_H
#define KOLLEKTIONS_FIXED_LIST_H

#include <stddef.h>

/**
 * A basic non-resizable read-write list.
 */
struct FixedList {
    size_t stride;
    size_t len;
    void *data;
};

#define INDEX_NOT_FOUND (-1)

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. INDEX_NOT_FOUND if not found.
 */
int FixedList_indexOf(struct FixedList list, void *data);

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. INDEX_NOT_FOUND if not found.
 */
int FixedList_indexOfLast(struct FixedList list, void *data);

/**
 * @param list Self
 * @param index The index of the element
 * @return The pointer to the element
 */
static inline void *FixedList_get(struct FixedList list, size_t index) {
    return (char *) list.data + list.stride * index;
}

/**
 * @param list Self
 * @param index The index of the element
 * @param data The pointer to the element
 */
void FixedList_set(struct FixedList list, size_t index, void *data);

#endif //KOLLEKTIONS_FIXED_LIST_H

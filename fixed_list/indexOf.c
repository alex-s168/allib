//
// Created by Alexander Nutz on 17/02/2024.
//

#include "fixed_list.h"

#include <string.h>

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. -1 if not found.
 */
int FixedList_indexOf(struct FixedList list, void *data) {
    void *curr = list.data;
    int currIndex = 0;
    while (currIndex < list.len) {
        if (memcmp(curr, data, list.stride) == 0) {
            return currIndex;
        }

        curr += list.stride;
        currIndex ++;
    }

    return INDEX_NOT_FOUND;
}

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. -1 if not found.
 */
int FixedList_indexOfLast(struct FixedList list, void *data) {
    int i = (int) list.len;
    while (i --> 0) {
        void *curr = list.data + list.stride * i;

        if (memcmp(curr, data, list.stride) == 0) {
            return i;
        }
    }

    return INDEX_NOT_FOUND;
}

#include "fixed_list.h"

#include <string.h>
#include <stdint.h>

#define ELSIZE_SWITCH(tocall, arg0) \
    switch (list.elSize) { \
        case 1: { tocall(arg0, uint8_t ); } \
        case 2: { tocall(arg0, uint16_t); } \
        case 4: { tocall(arg0, uint32_t); } \
        case 8: { tocall(arg0, uint64_t); } \
    }

#define STRIDE_SWITCH(tocall) \
    switch (list.stride) { \
        case 1: ELSIZE_SWITCH(tocall, uint8_t ) \
        case 2: ELSIZE_SWITCH(tocall, uint16_t) \
        case 4: ELSIZE_SWITCH(tocall, uint32_t) \
        case 8: ELSIZE_SWITCH(tocall, uint64_t) \
    }

#define TYPED_INDEX_OF(strideTy, sizeTy) \
    strideTy * curr = list.data; \
    int currIndex = 0; \
    while (currIndex < list.len) { \
        if (*(sizeTy*) &curr[currIndex] == *(sizeTy*) data) { \
            return currIndex; \
        } \
        currIndex ++; \
    } \
    return INDEX_NOT_FOUND;

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. -1 if not found.
 */
int FixedList_indexOf(FixedList list, const void *data) {
    STRIDE_SWITCH(TYPED_INDEX_OF);

    void *curr = list.data;
    int currIndex = 0;
    while (currIndex < list.len) {
        if (memcmp(curr, data, list.elSize) == 0) {
            return currIndex;
        }

        curr += list.stride;
        currIndex ++;
    }

    return INDEX_NOT_FOUND;
}

#define TYPED_INDEX_OF_LAST(strideTy, sizeTy) \
    int i = (int) list.len; \
    while (i --> 0) { \
        strideTy* curr = &((strideTy*)list.data)[i]; \
        if (*(sizeTy*)curr == *(sizeTy*)data) { \
            return i; \
        } \
    } \
    return INDEX_NOT_FOUND;

/**
 * @param list Self
 * @param data The element to search for
 * @return The index of the element. -1 if not found.
 */
int FixedList_indexOfLast(FixedList list, const void *data) {
    STRIDE_SWITCH(TYPED_INDEX_OF_LAST);

    int i = (int) list.len;
    while (i --> 0) {
        void *curr = list.data + list.stride * i;

        if (memcmp(curr, data, list.elSize) == 0) {
            return i;
        }
    }

    return INDEX_NOT_FOUND;
}

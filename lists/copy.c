//
// Created by Alexander Nutz on 17/02/2024.
//

#include "lists.h"

#include <string.h>

/**
 * @param list Self
 * @param dest A uninitialized list
 * @param ally The allocator to use
 * @param additionalCap The additional initial capacity of the new list. (initCap = additionalCap + old.len)
 */
void FixedList_copy(FixedList list,
                    DynamicList *dest,
                    Ally ally,
                    size_t additionalCap) {
    DynamicList_init(dest, list.stride, ally, list.len + additionalCap);
    memcpy(dest->fixed.data, list.data, list.stride * list.len);
    dest->fixed.len += list.len;
}

/**
 * @param list Self
 * @param dest A uninitialized list
 * @param ally The allocator to use
 * @param additionalCap The additional initial capacity of the new list. (initCap = additionalCap + old.len)
 */
void DynamicList_copy(DynamicList list,
                      DynamicList *dest,
                      Ally ally,
                      size_t additionalCap) {
    return FixedList_copy(list.fixed,
                          dest,
                          ally,
                          additionalCap);
}

#ifndef KOLLEKTIONS_LISTS_H
#define KOLLEKTIONS_LISTS_H

#include "../kallok/kallok.h"
#include "../fixed_list/fixed_list.h"
#include "../dynamic_list/dynamic_list.h"
#include "../linked_list/linked_list.h"

/**
 * @param list Self
 * @param dest A uninitialized list
 * @param ally The allocator to use
 * @param additionalCap The additional initial capacity of the new list. (initCap = additionalCap + old.len)
 */
void FixedList_copy(struct FixedList list,
                    struct DynamicList *dest,
                    Ally ally,
                    size_t additionalCap);

/**
 * @param list Self
 * @param dest A uninitialized list
 * @param ally The allocator to use
 * @param additionalCap The additional initial capacity of the new list. (initCap = additionalCap + old.len)
 */
void DynamicList_copy(struct DynamicList list,
                      struct DynamicList *dest,
                      Ally ally,
                      size_t additionalCap);

#endif //KOLLEKTIONS_LISTS_H

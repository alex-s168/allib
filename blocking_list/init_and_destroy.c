//
// Created by Alexander Nutz on 19/02/2024.
//

#include "blocking_list.h"

/**
 * Initializes a normal BlockingList
 * @param list Self
 * @param stride The size of each element
 * @param cap The initial maximum amount of elements
 * @param data The array of data. needs to be at least @arg cap * @arg stride
 * @return The index of the element. -1 if not found.
 */
void BlockingList_init(struct BlockingList *list, size_t stride,
                       Ally ally, size_t initCap) {
    list->useMutex = true;
    mutex_create(&list->mutex);
    DynamicList_init(&list->dyn, stride, ally, initCap);
}

/**
 * Initializes a BlockingList where many threads should have read access at the same time.
 * @param list Self
 * @param stride The size of each element
 * @param cap The initial maximum amount of elements
 * @param data The array of data. needs to be at least @arg cap * @arg stride
 * @return The index of the element. -1 if not found.
 */
void BlockingList_initManyReader(struct BlockingList *list, size_t stride,
                                 Ally ally, size_t initCap) {
    list->useMutex = false;
    rwlock_create(&list->rwlock);
    DynamicList_init(&list->dyn, stride, ally, initCap);
}

void BlockingList_destroy(struct BlockingList *list) {
    BlockingList_startWriteAccess(list);
    DynamicList_clear(&list->dyn);
    BlockingList_stopWriteAccess(list);

    if (list->useMutex) {
        mutex_destroy(&list->mutex);
    } else {
        rwlock_destroy(&list->rwlock);
    }
}

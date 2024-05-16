//
// Created by Alexander Nutz on 19/02/2024.
//

#include "blocking_list.h"

void BlockingList_startReadAccess(struct BlockingList *list) {
    if (list->useMutex) {
        mutex_lock(&list->mutex);
    } else {
        rwlock_r_lock(&list->rwlock);
    }
}

void BlockingList_stopReadAccess(struct BlockingList *list) {
    if (list->useMutex) {
        mutex_unlock(&list->mutex);
    } else {
        rwlock_unlock(&list->rwlock);
    }
}

void BlockingList_startWriteAccess(struct BlockingList *list) {
    if (list->useMutex) {
        mutex_lock(&list->mutex);
    } else {
        rwlock_w_lock(&list->rwlock);
    }
}

void BlockingList_stopWriteAccess(struct BlockingList *list) {
    if (list->useMutex) {
        mutex_unlock(&list->mutex);
    } else {
        rwlock_unlock(&list->rwlock);
    }
}

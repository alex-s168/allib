//
// Created by Alexander Nutz on 17/02/2024.
//

#include "dynamic_list.h"
#include "impl_utils.h"

/**
 * @param list Self
 * @param stride The size of each element
 * @param cap The initial maximum amount of elements
 * @param data The array of data. needs to be at least @arg cap * @arg stride
 * @return 0 if ok
 */
int DynamicList_init(struct DynamicList *list, size_t stride,
                      Ally ally, size_t initCap) {
    void *alloc = NULL;
    if (initCap > 0) {
        alloc = ally.impl->alloc(ally.state, initCap * stride);
        if (alloc == NULL)
            return 1;
    }
    list->fixed.stride = stride;
    list->fixed.data = alloc;
    list->cap = initCap;
    list->fixed.len = 0;
    list->ally = ally;
    return 0;
}

/**
 * @param list Self
 */
void DynamicList_clear(struct DynamicList *list) {
    list->ally.impl->free(list->ally.state, list->fixed.data, list->cap);
    list->cap = 0;
    list->fixed.len = 0;
    list->fixed.data = NULL;
}

static void anyclear(void *self) {
    struct DynamicList *list = self;
    DynamicList_clear(list);
}

static void anyreserve(void *self, size_t additional) {
    struct DynamicList *list = self;
    DynamicList_reserve(list, additional);
}

static void anypushback(void *self, const void *src) {
    struct DynamicList *list = self;
    DynamicList_add(list, (void*) src);
}

static void anypushfront(void *self, const void *src) {
    struct DynamicList *list = self;
    DynamicList_insertAt(list, 0, (void*) src);
}

static void anyremoveat(void *self, size_t index) {
    struct DynamicList *list = self;
    DynamicList_removeAt(list, index);
}

static void anyremoverange(void *self, size_t first, size_t last) {
    struct DynamicList *list = self;
    DynamicList_removeRange(list, first, last);
}

static void anyinsert(void *self, size_t index, const void *data) {
    struct DynamicList *list = self;
    DynamicList_insertAt(list, index, (void*) data);
}

static void anypushbackall(void *self, AnyList *src);
static void anypushfrontall(void *self, AnyList *src);

static MutAnyListImpl mutimpl = (MutAnyListImpl) {
    .clear = anyclear,
    .reserveAdditional = anyreserve,
    .pushBack = anypushback,
    .pushFront = anypushfront,
    .removeAt = anyremoveat,
    .removeRange = anyremoverange,
    .insertAt = anyinsert,
    .pushBackAll = anypushbackall,
    .pushFrontAll = anypushfrontall,
};

static void *anyget(void *self, size_t index) {
    struct DynamicList *list = self;
    return FixedList_get(list->fixed, index);
}

static size_t anylen(void *self) {
    struct DynamicList *list = self;
    return list->fixed.len;
}

static long long int anyfind(void *self, const void *data) {
    struct DynamicList *list = self; 
    return FixedList_indexOf(list->fixed, (void*) data);
}

static AnyListImpl impl = (AnyListImpl) {
    .get = anyget,
    .len = anylen,
    .find = anyfind,
    .sizeOfSelf = sizeof(struct DynamicList),
};

static void anypushbackall(void *self, AnyList *src) {
    struct DynamicList *list = self;
    
    if (src->impl == &impl) {
        struct DynamicList *other = src->self;
        DynamicList_addAll(list, other->fixed.data, other->fixed.len);
    }
    else {
        size_t len = src->impl->len(src->self);
        DynamicList_reserve(list, len);
        for (size_t i = 0; i < len; i ++) {
            void *elem = src->impl->get(src->self, i);
            DynamicList_add(list, elem);
        }
    }
}

static void anypushfrontall(void *self, AnyList *src) {
    struct DynamicList *list = self;
    
    if (src->impl == &impl) {
        struct DynamicList *other = src->self;
        DynamicList_insertAllAt(list, 0, other->fixed.data, other->fixed.len);
    }
    else {
        size_t len = src->impl->len(src->self);
        DynamicList_reserve(list, len);
        for (size_t i = 0; i < len; i ++) {
            void *elem = src->impl->get(src->self, i);
            DynamicList_insertAt(list, i, elem);
        }
    }
}

MutAnyList DynamicList_asAny(struct DynamicList *list) {
    AnyList immut;
    immut.self = list;
    immut.impl = &impl;
    immut.stride = list->fixed.stride;
    
    MutAnyList mut;
    mut.any = immut;
    mut.impl = &mutimpl;

    return mut;
}

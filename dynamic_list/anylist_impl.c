#include "impl_utils.h"

static void anyclear(void *self) {
    DynamicList *list = self;
    DynamicList_clear(list);
}

static void anyreserve(void *self, size_t additional) {
    DynamicList *list = self;
    DynamicList_reserve(list, additional);
}

static void anypushback(void *self, const void *src) {
    DynamicList *list = self;
    DynamicList_add(list, (void*) src);
}

static void anypushfront(void *self, const void *src) {
    DynamicList *list = self;
    DynamicList_insertAt(list, 0, (void*) src);
}

static void anyremoveat(void *self, size_t index) {
    DynamicList *list = self;
    DynamicList_removeAt(list, index);
}

static void anyremoverange(void *self, size_t first, size_t last) {
    DynamicList *list = self;
    DynamicList_removeRange(list, first, last);
}

static void anyinsert(void *self, size_t index, const void *data) {
    DynamicList *list = self;
    DynamicList_insertAt(list, index, (void*) data);
}

static void anypushbackall(void *self, AnyList *src);
static void anypushfrontall(void *self, AnyList *src);

MutAnyListImpl DynamicList_mutAnyListImpl = (MutAnyListImpl) {
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
    DynamicList *list = self;
    return FixedList_get(list->fixed, index);
}

static size_t anylen(void *self) {
    DynamicList *list = self;
    return list->fixed.len;
}

static long long int anyfind(void *self, const void *data) {
    DynamicList *list = self; 
    return FixedList_indexOf(list->fixed, (void*) data);
}

AnyListImpl DynamicList_anyListImpl = (AnyListImpl) {
    .get = anyget,
    .len = anylen,
    .find = anyfind,
    .sizeOfSelf = sizeof(DynamicList),
};

static void anypushbackall(void *self, AnyList *src) {
    DynamicList *list = self;
    
    if (src->impl == &DynamicList_anyListImpl) {
        DynamicList *other = src->self;
        DynamicList_addAll(list, other->fixed.data, other->fixed.len, other->fixed.stride);
    }
    else if (src->impl == &FixedList_anyListImpl) {
        FixedList *other = src->self;
        DynamicList_addAll(list, other->data, other->len, other->stride);
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
    DynamicList *list = self;
    
    if (src->impl == &DynamicList_anyListImpl) {
        DynamicList *other = src->self;
        DynamicList_insertAllAt(list, 0, other->fixed.data, other->fixed.len, other->fixed.stride);
    }
    else if (src->impl == &FixedList_anyListImpl) {
        FixedList *other = src->self;
        DynamicList_insertAllAt(list, 0, other->data, other->len, other->stride);
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

MutAnyList DynamicList_asAny(DynamicList *list) {
    AnyList immut;
    immut.self = list;
    immut.impl = &DynamicList_anyListImpl;
    immut.elSize = list->fixed.elSize;

    MutAnyList mut;
    mut.any = immut;
    mut.impl = &DynamicList_mutAnyListImpl;

    return mut;
}

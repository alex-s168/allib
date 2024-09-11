#include "linked_list.h"
#include "../lists/AnyList.h"

static void* anyget(void* implI, size_t idx) {
    DoubleLinkedList* impl = implI;
    return DoubleLinkedElement_data(DoubleLinkedList_linkAt(impl, idx));
}

static size_t anylen(void* implI) {
    DoubleLinkedList* impl = implI;
    return impl->size;
}

static long long int anyfind(void* implI, const void * search) {
    DoubleLinkedList* impl = implI;
    struct DoubleLinkedFindResult r = DoubleLinkedList_find(impl, search);
    if (r.data == NULL) return -1;
    return r.index;
}

AnyListImpl DoubleLinkedList_anyListImpl = (AnyListImpl) {
    .get = anyget, 
    .len = anylen,
    .find = anyfind,
    .sizeOfSelf = sizeof(DoubleLinkedList),
};

static void clear(void *self) {
    DoubleLinkedList* impl = self;
    DoubleLinkedList_clear(impl);
}

static void removeAt(void *self, size_t index) {
    DoubleLinkedList* impl = self;
    DoubleLinkedList_remove(impl, DoubleLinkedList_linkAt(impl, index));
}

static void removeRange(void *self, size_t first, size_t last) {
    DoubleLinkedList* impl = self;
    struct DoubleLinkedElement *va = DoubleLinkedList_linkAt(impl, first);
    struct DoubleLinkedElement *vb = DoubleLinkedList_linkAt(impl, last);
    DoubleLinkedList_removeMultiple(impl, va, vb, last - first + 1);
}

static void insertAt(void *self, size_t index, const void *value) {
    DoubleLinkedList* impl = self;
    struct DoubleLinkedElement *before = index == 0 ? NULL 
                                                    : DoubleLinkedList_linkAt(impl, index - 1);
    DoubleLinkedList_insertAfter(impl, before, value);
}

static void reserveAdditional(void *self, size_t additional) {
    // noop
}

static void pushBack(void *self, const void *value) {
    DoubleLinkedList* impl = self;
    DoubleLinkedList_add(impl, value);
}

static void pushBackAll(void *self, AnyList *src) {
    DoubleLinkedList* impl = self;

    if (src->impl == &FixedList_anyListImpl) {
        FixedList *other = src->self;
        DoubleLinkedList temp;
        DoubleLinkedList_copyFromFixed(&temp, impl->ally, *other);
        DoubleLinkedList_addAllClearOld(impl, &temp);
    }
    else if (src->impl == &DynamicList_anyListImpl) {
        DynamicList *other = src->self;
        DoubleLinkedList temp;
        DoubleLinkedList_copyFromFixed(&temp, impl->ally, other->fixed);
        DoubleLinkedList_addAllClearOld(impl, &temp);
    }
    else if (src->impl == &DoubleLinkedList_anyListImpl) {
        DoubleLinkedList *other = src->self;
        DoubleLinkedList_addAllCopy(impl, other);
    }
    else {
        for (size_t i = 0; i < src->impl->len(src->self); i ++) {
            void* el = src->impl->get(src->self, i);
            DoubleLinkedList_add(impl, el);
        }
    }
}

static void pushFront(void *self, const void *value) {
    DoubleLinkedList* impl = self;
    DoubleLinkedList_addFront(self, value);
}

static void pushFrontAll(void *self, AnyList *src) {
    DoubleLinkedList* impl = self;

    if (src->impl == &FixedList_anyListImpl) {
        FixedList *other = src->self;
        DoubleLinkedList temp;
        DoubleLinkedList_copyFromFixed(&temp, impl->ally, *other);
        DoubleLinkedList_addAllFrontClearOld(impl, &temp);
    }
    else if (src->impl == &DynamicList_anyListImpl) {
        DynamicList *other = src->self;
        DoubleLinkedList temp;
        DoubleLinkedList_copyFromFixed(&temp, impl->ally, other->fixed);
        DoubleLinkedList_addAllFrontClearOld(impl, &temp);
    }
    else if (src->impl == &DoubleLinkedList_anyListImpl) {
        DoubleLinkedList *other = src->self;
        DoubleLinkedList_addAllFrontCopy(impl, other);
    }
    else {
        size_t len = src->impl->len(src->self);
        for (size_t i = 0; i < len; i ++) {
            void* el = src->impl->get(src->self, len - 1 - i);
            DoubleLinkedList_addFront(impl, el);
        }
    }
}

MutAnyListImpl DoubleLinkedList_mutAnyListImpl = (MutAnyListImpl) {
    .clear = clear,
    .insertAt = insertAt,
    .pushBack = pushBack,
    .pushFront = pushFront,
    .pushBackAll = pushBackAll,
    .pushFrontAll = pushFrontAll,
    .removeRange = removeRange,
    .reserveAdditional = reserveAdditional,
    .removeAt = removeAt,
};

AnyList DoubleLinkedList_asAny(DoubleLinkedList *list) {
    return (AnyList) {
        .impl = &DoubleLinkedList_anyListImpl, 
        .self = list,
        .elSize = list->size,
    };
}

MutAnyList DoubleLinkedList_asMutAny(DoubleLinkedList *list) {
    return (MutAnyList) {
        .any = DoubleLinkedList_asAny(list),
        .impl = &DoubleLinkedList_mutAnyListImpl,
    };
}

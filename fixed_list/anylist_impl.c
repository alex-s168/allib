#include "fixed_list.h"

static void* anyget(void* self, size_t idx) {
    const FixedList * li = self;
    return FixedList_get(*li, idx);
}

static size_t anylen(void* self) {
    const FixedList * li = self;
    return li->len;
}

static long long int anyfind(void* self, const void* cmp) {
    const FixedList * li = self;
    return FixedList_indexOf(*li, cmp);
}

AnyListImpl FixedList_anyListImpl = (AnyListImpl) {
    .get = anyget,
    .len = anylen, 
    .find = anyfind,
    .sizeOfSelf = sizeof(FixedList),
};

AnyList FixedList_asAny(const FixedList * list) {
    return (AnyList) {
        .elSize = list->elSize,
        .self = (void*) list,
        .impl = &FixedList_anyListImpl, 
    };
}

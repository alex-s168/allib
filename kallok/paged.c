//
// Created by Alexander Nutz on 04/03/2024.
//

#include <string.h>

#include "kallok.h"

static void paged_free(void *state, void *alloc, size_t old) {
    (void) state;

    old = makeMultiple(old, page_size());
    struct Page page;
    page.ptr = alloc;
    page.size = old;
    free_page(page);
}

static void *paged_alloc(void *state, size_t new) {
    (void) state;
    struct Page page = alloc_pages(new);
    return page.ptr;
}

static void *paged_realloc(void *state, void *alloc, size_t old, size_t new) {
    (void) state;
    size_t pageSize = page_size();
    new = makeMultiple(new, pageSize);
    if (makeMultiple(old, pageSize) == new)
        return alloc;

    void *newAlloc = paged_alloc(state, new);
    if (old != 0 && alloc != NULL) {
        memcpy(newAlloc, alloc, old);
        paged_free(state, alloc, old);
    }
    return newAlloc;
}

static AllyImpl impl = {
    .free = paged_free,
    .alloc = paged_alloc,
    .realloc = paged_realloc,
};

Ally getPageAlloc() {
    return (Ally) { .state = NULL, .impl = &impl };
}

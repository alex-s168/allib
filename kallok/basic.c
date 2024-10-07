//
// Created by Alexander Nutz on 04/03/2024.
//

#include "kallok.h"
#include "pages.h"
#include <string.h>

static void alloc_free(void *stateIn, void *alloc, size_t old) {
    AllyDynamicBasicState *state = stateIn;

    for (size_t i = 0; i < state->entries_len; i ++) {
        AllyDynamicBasicEntry *entry = &state->entries[i];
        if (alloc < entry->page.ptr)
            continue;
        void *end = entry->page.ptr + entry->page.size;
        if (alloc >= end)
            continue;

        yfree(entry->fixed, alloc, old);

        if (isFixedBasicAllocEmpty(entry->fixed)) {
            free_page(entry->page);    
            memcpy(entry, entry + 1, sizeof(AllyDynamicBasicEntry) * (state->entries_len - i - 1));
            state->entries = yrealloc(state->paged, state->entries,
                                      sizeof(AllyDynamicBasicEntry) * (state->entries_len --),
                                      sizeof(AllyDynamicBasicEntry) * state->entries_len);
        }

        break;
    }
}

static void *alloc_alloc(void *stateIn, size_t new) {
    AllyDynamicBasicState *state = stateIn;

    for (size_t i = 0; i < state->entries_len; i ++) {
        AllyDynamicBasicEntry *entry = &state->entries[i];
        void *alloc = yalloc(entry->fixed, new);
        if (alloc != NULL) {
            return alloc;
        }
    }

    struct Page page = alloc_pages(new);
    new = page.size;

    state->entries = yrealloc(state->paged, state->entries,
                              sizeof(AllyDynamicBasicEntry) * state->entries_len,
                              sizeof(AllyDynamicBasicEntry) * (state->entries_len + 1));
    AllyDynamicBasicEntry *entry = &state->entries[state->entries_len ++];
    entry->page = page;

    if (page.ptr == NULL)
        return NULL;

    entry->fixed = createFixedBasicAlloc(&entry->fixed_state, page.ptr, page.size);
    return yalloc(entry->fixed, new);
}

static void *alloc_realloc(void *stateIn, void *alloc, size_t old, size_t newSize) {
    void *new = alloc_alloc(stateIn, newSize);
    memcpy(new, alloc, old);
    alloc_free(stateIn, alloc, old);
    return new;
}

static AllyImpl impl = {
    .alloc = alloc_alloc,
    .free = alloc_free,
    .realloc = alloc_realloc,
};

#if !A_CFG_ALLY_ONLY_LIBC 
Ally createBasicAlloc(AllyDynamicBasicState *state, bool can_leak) {
    state->paged = getPageAlloc();
    state->entries_len = 0;
    state->entries = NULL;
    state->can_leak = can_leak;
    return (Ally) { .state = state, .impl = &impl };
}
#endif

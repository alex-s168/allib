//
// Created by Alexander Nutz on 06/03/2024.
//

#include "kallok.h"

#if !A_CFG_ALLY_ONLY_LIBC 

static void alloc_free(void *stateIn, void *alloc, size_t old) {
    (void) old;
    AllySingleFixedState *state = stateIn;
    if (alloc != state->start)
        return;
    state->used = false;
}

static void *alloc_alloc(void *stateIn, size_t new) {
    AllySingleFixedState *state = stateIn;
    if (state->used)
        return NULL;
    if (new > state->len)
        return NULL;
    state->used = true;
    return state->start;
}

static void *alloc_realloc(void *stateIn, void *alloc, size_t old, size_t new) {
    (void) alloc;
    (void) old;
    AllySingleFixedState *state = stateIn;
    if (new > state->len)
        return NULL;
    return state->start;
}

static AllyImpl impl = {
        .free = alloc_free,
        .alloc = alloc_alloc,
        .realloc = alloc_realloc,
};

// A simple memory allocator that only allows for one allocation at a time.
// Uses the given fixed array as memry source.
Ally createSingleFixedAlloc(AllySingleFixedState *state, void *data, size_t len) {
    state->used = false;
    state->start = data;
    state->len = len;
    return (Ally) { .impl = &impl, .state = state };
}

#endif

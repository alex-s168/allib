#include "kallok.h"

#include <string.h>

#if !A_CFG_ALLY_ONLY_LIBC 

static void alloc_free(void *stateIn, void *alloc, size_t old) {
    AllyMultiState *state = stateIn;

    unsigned char *real = ((unsigned char *) alloc) - 1;
    if (real[0] == 0) {
        yfree(state->a, (void*)real, old + sizeof(unsigned char));
    } else {
        yfree(state->b, (void*)real, old + sizeof(unsigned char));
    }
}

static void *alloc_alloc(void *stateIn, size_t new) {
    AllyMultiState *state = stateIn;
    unsigned char *a;

    a = yalloc(state->a, new + sizeof(unsigned char));
    if (a == NULL) {
        a = yalloc(state->b, new + sizeof(unsigned char));
        if (a == NULL)
            return NULL;
        a[0] = 1;
    } else {
        a[0] = 0;
    }

    return (void *) (a + 1);
}

static void *alloc_realloc(void *stateIn, void *alloc, size_t old, size_t new) {
    AllyMultiState *state = stateIn;

    void *ret = alloc_alloc(stateIn, new);
    if (ret == NULL) {
        alloc_free(stateIn, alloc, old);
        return NULL;
    }
    memcpy(ret, alloc, old);
    alloc_free(stateIn, alloc, old);
    return ret;
}

static AllyImpl impl = {
    .free = alloc_free,
    .alloc = alloc_alloc,
    .realloc = alloc_realloc,
};

// A simple memory allocator that only allows for one allocation at a time.
// Uses the given fixed array as memry source.
Ally createMultiAlloc(AllyMultiState *state, Ally a, Ally b) {
    state->a = a;
    state->b = b;
    return (Ally) { .impl = &impl, .state = state };
}

#endif

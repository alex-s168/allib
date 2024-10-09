//
// Created by Alexander Nutz on 17/02/2024.
//

#include "kallok.h"

#if !A_CFG_ALLY_ONLY_LIBC 

#include <stdlib.h>

static void  alloc_libc_free(void *state, void *alloc, size_t old) {
    (void) state;
    (void) old;
    return free(alloc);
}

static void *alloc_libc_alloc(void *state, size_t new) {
    (void) state;
    return malloc(new);
}

static void *alloc_libc_realloc(void *state, void *alloc, size_t old, size_t new) {
    (void) state;
    (void) old;
    return realloc(alloc, new);
}

AllyImpl alloc_libc = {
    .free = alloc_libc_free,
    .alloc = alloc_libc_alloc,
    .realloc = alloc_libc_realloc,
};

Ally getLIBCAlloc() {
    return (Ally) {
        .impl = &alloc_libc
    };
}

#endif

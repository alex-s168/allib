#ifndef KALLOK_H
#define KALLOK_H

#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "../attrib.h"
#include "pages.h"
#include "virtual.h"

static size_t makeMultiple(size_t x, size_t y) {
    if (x == 0)
        return 0;

    x += y - 1;
    x -= x % y;
    return x;
}

typedef struct {
    // DOES NOT CRASH IF ALLOC NOT ALLOCATED BY ALLOCATOR
    void (*free)(void *state, void *alloc, size_t old);

    void *(*alloc)(void *state, size_t size);

    void *(*realloc)(void *state, void *alloc, size_t old, size_t size);
} AllyImpl;

typedef struct {
    AllyImpl *impl;
    void *state;
} Ally;

static bool Ally_eq(Ally a, Ally b) {
    return a.impl == b.impl && a.state == b.state;
}

#define yalloc(a, size)               a.impl->alloc(a.state, size)
#define yrealloc(a, alloc, old, size) a.impl->realloc(a.state, alloc, old, size)
#define yfree(a, alloc, old)          a.impl->free(a.state, alloc, old)

static void *ycopy(Ally a, const void *src, size_t size) {
    void *alloc = yalloc(a, size);
    if (alloc == NULL)
        return NULL;
    memcpy(alloc, src, size);
    return alloc;
}

Ally getLIBCAlloc();

struct AllyStats {
    size_t allocs;
    size_t frees;
    size_t reallocs;

UNSAFE 
    Ally parent;
};
Ally getStatAlloc(Ally parent, struct AllyStats *statisticDest);
#ifdef _INC_STDIO
void outputStats(struct AllyStats *stats, FILE *dest);
#endif

typedef struct {
INTERNAL
    bool used;
    void *start;
    size_t len;
} AllySingleFixedState;

// A simple memory allocator that only allows for one allocation at a time.
// Uses the given fixed array as memry source.
Ally createSingleFixedAlloc(AllySingleFixedState *state, void *data, size_t len);

typedef struct {
INTERNAL
    void *start;
    size_t len;
    void *next;
} AllyFixedBasicState;

// A simple memory allocator that allocates elements in a fixed-length array.
// Does not have any memory-defragmentation features
Ally createFixedBasicAlloc(AllyFixedBasicState *state, void *data, size_t limit);
bool isFixedBasicAllocEmpty(Ally);

typedef struct {
INTERNAL
    Ally a;
    Ally b;
} AllyMultiState;

// allloctor that tries to alloc a first and if fail, alloc b
Ally createMultiAlloc(AllyMultiState *state, Ally a, Ally b);

// Allocates full OS pages
// useful for big lists
Ally getPageAlloc();

typedef struct {
INTERNAL
    AllyFixedBasicState fixed_state;
    Ally fixed;
    struct Page page;
} AllyDynamicBasicEntry;

typedef struct {
INTERNAL
    bool can_leak;
    Ally paged;

    AllyDynamicBasicEntry *entries;
    size_t                 entries_len;
} AllyDynamicBasicState;

// Based on \see createFixedBasicAlloc
// __Arena allocator__
Ally createBasicAlloc(AllyDynamicBasicState *state, bool can_leak);

typedef AllyDynamicBasicState AllyStandardState;

static Ally createStandardAlloc(AllyStandardState *state) {
    return createBasicAlloc(state, false);
}

typedef struct {
    size_t refs;
    size_t stride;
    Ally   ally;
} Rc;

#define Rc_data(rc) (((char*)rc) + sizeof(Rc))
static Rc *Rc_new(size_t stride, Ally ally) {
    Rc *rc = (Rc*) yalloc(ally, sizeof(Rc) + stride);
    if (rc == NULL)
        return NULL;
    rc->ally = ally;
    rc->stride = stride;
    rc->refs = 0;
    return rc;
}

static void Rc_inc(Rc *rc) {
    assert(rc != NULL);
    rc->refs ++;
}

static void Rc_dec(Rc *rc) {
    assert(rc != NULL);
    assert(rc->refs != 0);
    rc->refs --;
    if (rc->refs == 0) {
        yfree(rc->ally, rc, sizeof(Rc) + rc->stride);
    }
}

#endif //KALLOK_H

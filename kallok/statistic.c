//
// Created by Alexander Nutz on 29/02/2024.
//

#include "kallok.h"

#if !A_CFG_ALLY_ONLY_LIBC 

struct State {
    struct AllyStats stats;
    Ally parent;
};

void  alloc_stat_free(void *state, void *alloc, size_t old) {
    struct AllyStats *s = state;
    s->frees ++;
    return s->parent.impl->free(s->parent.state, alloc, old);
}

void *alloc_stat_alloc(void *state, size_t new) {
    struct AllyStats *s = state;
    s->allocs ++;
    return s->parent.impl->alloc(s->parent.state, new);
}

void *alloc_stat_realloc(void *state, void *alloc, size_t old, size_t new) {
    struct AllyStats *s = state;
    s->reallocs ++;
    return s->parent.impl->realloc(s->parent.state, alloc, old, new);
}

AllyImpl alloc_stat = {
    .free = alloc_stat_free,
    .alloc = alloc_stat_alloc,
    .realloc = alloc_stat_realloc,
};

Ally getStatAlloc(Ally parent, struct AllyStats *statisticDest) {
    statisticDest->parent = parent;
    statisticDest->allocs = 0;
    statisticDest->frees = 0;
    statisticDest->reallocs = 0;
    return (Ally) { .impl = &alloc_stat, .state = statisticDest };
}

#include "stdio.h"

void outputStats(struct AllyStats *stats, FILE *dest) {
    fprintf(dest, " %zu allocations\n", stats->allocs);
    fprintf(dest, " %zu de-allocations\n", stats->frees);
    fprintf(dest, " %zu re-allocations\n", stats->reallocs);
}
#endif

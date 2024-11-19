#include "kallok.h"

#include <string.h>

#if !A_CFG_ALLY_ONLY_LIBC

static void freeall(AllyFastState* state) {
    for (size_t i = 0; i < state->chunkslen; i ++)
        yfree(state->backing, state->chunks[i].ptr, state->chunks[i].len);
    if (state->chunk != NULL)
        yfree(state->backing, state->chunk, state->chunk_len);
    yfree(state->backing, state->chunks, sizeof(*state->chunks) * state->chunk_len);
    state->chunks = NULL;
    state->chunkslen = 0;
}

static void alloc_free(void *stateIn, void *alloc, size_t old) {
	if (alloc == NULL) return;
    AllyFastState *state = stateIn;
	state->num_alive_allocs --;
	if (state->num_alive_allocs == 0) {
		freeall(state);
	}
}

static void *alloc_alloc(void *stateIn, size_t bytes) {
    AllyFastState *state = stateIn;

    if (state->chunk == NULL || bytes > state->chunk_left) {
        if (state->chunk) {
			void * temp = yrealloc(state->backing, state->chunks,
								   sizeof(*state->chunks) * state->chunkslen,
								   sizeof(*state->chunks) * (state->chunkslen + 1));
			if (temp == NULL) {
				return NULL;
			}
			state->chunks = temp;
			state->chunks[state->chunkslen].ptr = state->chunk;
			state->chunks[state->chunkslen].len = state->chunk_len;
			state->chunkslen ++;
        }

        size_t new = 1024;
        if (bytes > new)
            new = bytes * 2;
        void * temp = yalloc(state->backing, new);
		if (temp == NULL) {
			return NULL;
		}
        state->chunk = temp;
		state->chunk_left = new;
		state->chunk_len = new;
    }

    state->chunk_left -= bytes;
    void *alloc = state->chunk;
    state->chunk = ((char*) state->chunk) + bytes;

	state->num_alive_allocs ++;
    return alloc;
}

static void *alloc_realloc(void *stateIn, void *old, size_t oldBytes, size_t newBytes) {
    AllyFastState *state = stateIn;

    if (newBytes <= oldBytes) {
        return old;
    }

    size_t diff = newBytes - oldBytes;
    if (((char*)old) + oldBytes == state->chunk && diff <= state->chunk_left) {
        (void) alloc_alloc(state, diff);
        return old;
    }

    void * new = alloc_alloc(state, newBytes);
    if (new == NULL)
        return NULL;
    memcpy(new, old, oldBytes);
    return new;
}

static AllyImpl impl = {
    .free = alloc_free,
    .alloc = alloc_alloc,
    .realloc = alloc_realloc,
};

Ally createFastAlloc(AllyFastState *state, Ally backing) {
    memset(state, 0, sizeof(*state));
	state->backing = backing;
    return (Ally) { .impl = &impl, .state = state };
}

void fastAllocFreeAll(Ally fastAlloc) {
    AllyFastState *state = fastAlloc.state;
	freeall(state);
}

#endif

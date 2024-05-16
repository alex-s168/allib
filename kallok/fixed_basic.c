//
// Created by Alexander Nutz on 04/03/2024.
//

#include "kallok.h"

#include <string.h>
#include <stdint.h>

#define CHUNK_DATA_SIZE 32 // wasting half a chunk is still better than wasting almost the full chunk

struct Chunk {
    uint8_t chunksUsed; // = 0
    char data[CHUNK_DATA_SIZE];
};

#define CHUNK_SIZE sizeof(struct Chunk)

bool isFixedBasicAllocEmpty(Ally ally) {
    AllyFixedBasicState *state = ally.state;
    for (size_t i = 0; i < state->len; i ++) {
        struct Chunk *chunk = &((struct Chunk *) state->start)[i];
        if (chunk->chunksUsed > 0)
            return false;
    }
    return true;
}

static void  alloc_fixed_basic_free(void *stateIn, void *alloc, size_t old) {
    (void) old;
    AllyFixedBasicState *state = stateIn;
    struct Chunk *chunk = alloc - offsetof(struct Chunk, data);
    for (size_t i = 0; i < chunk->chunksUsed; i ++) {
        struct Chunk *op = &chunk[i];
        op->chunksUsed = 0;
    }
    if (chunk < (struct Chunk *) state->next)
        state->next = chunk;
}

static void *alloc_fixed_basic_alloc(void *stateIn, size_t new) {
    AllyFixedBasicState *state = stateIn;
    struct Chunk *end = (struct Chunk *) (state->start) + state->len;
    if ((struct Chunk *) state->next + new > end) {
        return NULL;
    }

    new = makeMultiple(new, CHUNK_SIZE);
    size_t chunkAmount = new / CHUNK_SIZE;

    size_t counter = 0;
    struct Chunk *start;
    for (struct Chunk *chunk = state->next; chunk < end;) {
        if (chunk->chunksUsed == 0) {
            if (counter == 0)
                start = chunk;
            counter ++;
            if (counter == chunkAmount) {
                // `chunk` is the last chunk in our allocation

                while ((++ chunk) < end) {
                    if (chunk->chunksUsed == 0) {
                        state->next = chunk;
                        break;
                    }
                }

                start->chunksUsed = chunkAmount;
                return start->data;
            }
            chunk ++;
        } else {
            counter = 0;
            chunk += chunk->chunksUsed;
        }
    }
    return NULL;
}

static void *alloc_fixed_basic_realloc(void *stateIn, void *alloc, size_t old, size_t newSize) {
    void *new = alloc_fixed_basic_alloc(stateIn, newSize);
    if (new == NULL)
        return NULL;
    memcpy(new, alloc, old);
    alloc_fixed_basic_free(stateIn, alloc, old);
    return new;
}

AllyImpl alloc_fixed_basic = {
    .free = alloc_fixed_basic_free,
    .alloc = alloc_fixed_basic_alloc,
    .realloc = alloc_fixed_basic_realloc,
};

Ally createFixedBasicAlloc(AllyFixedBasicState *state, void *data, size_t limit) {
    state->start = state->next = data;
    state->len = limit;
    return (Ally) { .state = state, .impl = &alloc_fixed_basic };
}

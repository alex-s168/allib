//
// Created by Alexander Nutz on 04/03/2024.
//

#include <assert.h>

#include "../kallok/kallok.h"

int main() {
    static AllyDynamicBasicState state;
    Ally ally = createBasicAlloc(&state, false);

    void *a = yalloc(ally, 32);
    void *b = yalloc(ally, 64);
    void *c = yalloc(ally, 32);

    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    // check if we own all allocations -> no segfault
    (void) *(volatile char *)a;
    (void) *(volatile char *)b;
    (void) *(volatile char *)c;

    yfree(ally, a, 32);
    yfree(ally, b, 64);
    yfree(ally, c, 32);

    return 0;
}

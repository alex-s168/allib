#include <assert.h>

#include "../kallok/kallok.h"
#include "test_alloc.h"

int main() {
    static AllyFastState state;
    Ally ally = createFastAlloc(&state, getLIBCAlloc());
	test_alloc(ally);
    return 0;
}

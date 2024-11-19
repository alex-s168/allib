#include <assert.h>

#include "../kallok/kallok.h"
#include "test_alloc.h"

int main() {
    static AllyDynamicBasicState state;
    Ally ally = createBasicAlloc(&state, false);

	{
		test_alloc(ally);
	}

    return 0;
}

#include <assert.h>
#include <stdio.h>

#include "../kallok/kallok.h"
#include "test_alloc.h"

int main() {
    static char data[512];
    static AllyFixedBasicState allyState;
    Ally ally = createFixedBasicAlloc(&allyState, data, 512);

	{
		void *a = yalloc(ally, 32);
		void *b = yalloc(ally, 64);
		void *c = yalloc(ally, 32);

		assert(a != NULL);
		assert(b != NULL);
		assert(c != NULL);

		assert(c > b);
		assert(b > a);

#define test(a) assert(a > (void *) data && a < (void *) data + 512);
		test(a)
		test(b)
		test(c)

		yfree(ally, a, 32);
		yfree(ally, b, 64);
		yfree(ally, c, 32);
	}

	{
		test_alloc(ally);
	}

    return 0;
}

#include <assert.h>

#include "../kallok/kallok.h"
#include "test_alloc.h"

// this test is only for verification of the test program
int main() {
    Ally ally = getLIBCAlloc();
	test_alloc(ally);
    return 0;
}

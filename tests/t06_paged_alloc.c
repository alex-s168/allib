#include <assert.h>

#include "../kallok/kallok.h"
#include "test_alloc.h"

int main() {
    Ally ally = getPageAlloc();
    test_alloc(ally);
    return 0;
}

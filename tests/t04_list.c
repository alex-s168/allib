//
// Created by Alexander Nutz on 17/02/2024.
//

#include "../dynamic_list/dynamic_list.h"

#include <assert.h>

int main() {
    static char alloc[sizeof(int) * 10];
    AllySingleFixedState allyState;
    Ally ally = createSingleFixedAlloc(&allyState, alloc, sizeof(alloc));

    struct DynamicList list;
    DynamicList_init(&list, sizeof(int), ally, 0);

    DynamicList_add(&list, &(int) { 1 });
    DynamicList_add(&list, &(int) { 2 });
    DynamicList_add(&list, &(int) { 3 });
    DynamicList_add(&list, &(int) { 4 });
    DynamicList_add(&list, &(int) { 5 });

    DynamicList_removeRange(&list, 1, 2);

    assert(list.fixed.len == 3);

    int ix = FixedList_indexOf(list.fixed, &(int) { 4 });
    assert(ix == 1);

#define assertElem(i, v) assert(*(int *)FixedList_get(list.fixed, i) == v)

    assertElem(0, 1);
    assertElem(1, 4);
    assertElem(2, 5);

    return 0;
}

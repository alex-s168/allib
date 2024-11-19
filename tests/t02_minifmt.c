#include <stdlib.h>
#include <string.h>

#define MINIFMT_IMPL
#include "../minifmt.h"

// can't be const!
static char fmt_str[] = "Test {row} str 0x{col.h} {test}";

int main() {
    const Fmt fmt = fmt_compile(fmt_str);
    char *res = fmt_fmt_fmt(fmt, (PlaceholderList) {
        .elems = (Placeholder[3]) {
            PLACEHOLDER_LONG("col", 26),
            PLACEHOLDER_LONG("row", 2),
            PLACEHOLDER_DOUBLE("test", 1.32122)
        },
        .elems_size = 3
    });

    fmt_destroy(fmt);

    if (strcmp(res, "Test 2 str 0x1a 1.32122") != 0)
        return 1;

    free(res);

    return 0;
}


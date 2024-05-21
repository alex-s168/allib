#include "niglob.h"

#include <stdio.h>

int main() {
    {
        const char * p = "hello*!";

        printf("%i\n", patterneq(p, "hello"));
        printf("%i\n", patterneq(p, "hello!"));
        printf("%i\n", patterneq(p, "hello world"));
        printf("%i\n", patterneq(p, "hello world!"));
        printf("%i\n", patterneq(p, "hello world! a"));
    }

    puts("");

    {
        curlyexp_res res = curlyexp("a{b,c,}xy{ef,}ne");
        for (size_t i = 0; i < res.count; i ++) {
            puts(res.items[i]);
        }
        curlyexp_free(res);
    }

    puts("");

    {
        niglob_res res = niglob("~/test.kt", NULL);
        for (size_t i = 0; i < res.count; i ++) {
            puts(res.items[i].abspath);
        }
        niglob_free(res);
    }
}

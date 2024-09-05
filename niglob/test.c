#include "niglob.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    {
        const char * p = "hello*!";

        printf("%i\n", patterneq(p, "hello", false));
        printf("%i\n", patterneq(p, "hello!", false));
        printf("%i\n", patterneq(p, "hello world", false));
        printf("%i\n", patterneq(p, "hello world!", false));
        printf("%i\n", patterneq(p, "hello world! a", false));
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
        char* s;
#define test(str) s = niglob_expand_res_prependWinDrive_freeAlloc(niglob_expand(str, NULL)); puts(s); free(s);
        test("~/a/b/c")
        test("/dev/sda1")
        test("C:/a/b/c")
        test("alex~/a/b/c")
#undef  test
    }

    puts("");

    {
        niglob_res res = niglob("~/*.kt", NULL);
        for (size_t i = 0; i < res.count; i ++) {
            puts(res.items[i].abspath);
        }
        niglob_free(res);
    }

    puts("");

    {
        niglob_res res = niglob("~/fakedir/../*.kt", NULL);
        for (size_t i = 0; i < res.count; i ++) {
            puts(res.items[i].abspath);
        }
        niglob_free(res);
    }

    puts("");

    {
        niglob_res res = niglob("/dev/sd{a,b}*", NULL);
        for (size_t i = 0; i < res.count; i ++) {
            puts(res.items[i].abspath);
        }
        niglob_free(res);
    }
}

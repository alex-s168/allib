#ifndef STR_H
#define STR_H

#include <string.h>

/** NULL TERMINATED!!! */
typedef struct {
    size_t len;
    const char *data;
} str;

static inline str str_fromc(const char *c) {
    str str;
    str.len = strlen(c);
    str.data = c;
    return str;
}

static inline str str_empty() {
    str str;
    str.len = 0;
    str.data = NULL;
    return str;
}

#endif

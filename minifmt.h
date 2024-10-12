#ifndef MINIFMT_H
#define MINIFMT_H

#include <stdbool.h>
#include <stddef.h>

/** converts value to ascii into dest; returns position of null terminator; */
char *ll_to_ascii(long long value, char *dest, const int radix);

/** converts value to ascii into dest; returns position of null terminator */
char *d_to_ascii(const double value, char *dest, const size_t accuraccy);

long long ascii_to_ll(const char * ascii, size_t len, const int radix);
double ascii_to_d(const char * ascii, size_t len, const int radix);

typedef struct {
    const char *str;
    size_t      len;
} FmtStrElem;

typedef struct {
    const char *str;
    const char *flag;
} FmtPlaceholderElem;

typedef enum {
    FmtElemType_STR,
    FmtElemType_PLACEHOLDER
} FmtElemType;

typedef struct {
    FmtElemType type;
    union {
        FmtStrElem str;
        FmtPlaceholderElem placeholder;
    };
} FmtElem;

typedef struct {
    FmtElem *elems;
    size_t   elems_size;
} Fmt;

void fmt_add(Fmt *fmt, const FmtElem elem);

/** destroys str; str needs to live as long as returned fmt */
Fmt fmt_compile(char *str);
void fmt_destroy(const Fmt fmt);
char *fmt_fmt(const Fmt fmt, const char *(*placeholder_find)(FmtPlaceholderElem, void *), void *userdata);

// ========================================================================== */

typedef enum {
    PlaceholderType_LONG,
    PlaceholderType_DOUBLE,
    PlaceholderType_STR,
} PlaceholderType;

typedef struct {
    const char      *key;
    PlaceholderType type;
    union {
        long        l;
        double      d;
        const char *s;
    };
} Placeholder;

#define PLACEHOLDER_LONG(k,value)   (Placeholder) { .key = k, .type = PlaceholderType_LONG, .l = value }
#define PLACEHOLDER_DOUBLE(k,value) (Placeholder) { .key = k, .type = PlaceholderType_DOUBLE, .d = value }
#define PLACEHOLDER_STR(k,value)    (Placeholder) { .key = k, .type = PlaceholderType_STR, .s = value }

typedef struct {
    const Placeholder *elems;
    size_t             elems_size;
} PlaceholderList;

const char *placeholder_placeholder_find(FmtPlaceholderElem elem, void *listPtr);

static char *fmt_fmt_fmt(const Fmt fmt, PlaceholderList placeholders) {
    return fmt_fmt(fmt, placeholder_placeholder_find, &placeholders);
}

#endif //MINIFMT_H

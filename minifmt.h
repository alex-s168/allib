// Define MINIFMT_IMPL in a seperate C file and include this header

/*
Copyright 2024 Alexander Nutz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MINIFMT_H
#define MINIFMT_H
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static void rev(char *first, char *last) {
    if (first > last) {
        char *temp = last;
        last = first;
        first = temp;
    }
    
    while (last >= first) {
        const char s = *first;
        *first++ = *last;
        *last-- = s;
    }
}

/** converts value to ascii into dest; returns position of null terminator; */
char *ll_to_ascii(long long value, char *dest, const int radix)
#ifndef MINIFMT_IMPL
;
#else
{
    if (value < 0) {
        *dest++ = '-';
        value = -value;
    } else if (value == 0) {
        *dest++ = '0';
        *dest = '\0';
        return dest;
    }

    char *start = dest;

    while (value) {
        const int i = value % radix;
        value /= radix;
        if (i < 10)
            *dest++ = i + '0';
        else
            *dest++ = i + 'a' - 10;
    }

    rev(start, dest-1);
    
    *dest = '\0';
    // we return pos of nt so we don't increment here
    
    return dest;
}
#endif

/** converts value to ascii into dest; returns position of null terminator */
char *d_to_ascii(const double value, char *dest, const size_t accuraccy)
#ifndef MINIFMT_IMPL
;
#else
{
    const long long valueL = (long long) value;
    dest = ll_to_ascii(valueL, dest, 10);
    *dest++ = '.';
    
    static char fltbuf[200];
    const char *end = ll_to_ascii((long long) ((value - (double) valueL) * pow(10, accuraccy)), fltbuf, 10);
    
    size_t len = end - fltbuf;

    for (size_t i = len; i < accuraccy; i ++)
        *dest++ ='0';

    while (end > fltbuf) {
        if (!(*end == '0' || *end == '\0'))
            break;
        end --;
    }

    len = end - fltbuf + 1;
    
    memcpy(dest, fltbuf, len);
    dest += len;
    
    *dest ='\0';
    return dest;
}
#endif

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

static void fmt_add(Fmt *fmt, const FmtElem elem) {
    fmt->elems = realloc(fmt->elems, (fmt->elems_size + 1) * sizeof(FmtElem));
    fmt->elems[fmt->elems_size ++] = elem;
}

/** destroys str; str needs to live as long as returned fmt */
Fmt fmt_compile(char *str)
#ifndef MINIFMT_IMPL
;
#else
{
    Fmt res;
    res.elems = NULL;
    res.elems_size = 0;

    fmt_add(&res, (FmtElem) {
        .type = FmtElemType_STR,
        .str = (FmtStrElem) {
            .str = str
        }
    });

    const char *placeholder_start = NULL;
    const char *placeholder_flags_start = NULL;
    bool        placeholder = false;
    bool        placeholder_flags = false;

    while (*str != '\0') {
        const char c = *str;

        if (placeholder) {
            if (c == '.' && !placeholder_flags) {
                placeholder_flags = true;
                *str = '\0';
                placeholder_flags_start = str + 1;
            }
            if (c == '}') {
                *str = '\0';

                fmt_add(&res, (FmtElem) {
                    .type = FmtElemType_PLACEHOLDER,
                    .placeholder = (FmtPlaceholderElem) {
                        .str = placeholder_start,
                        .flag = placeholder_flags_start
                    }
                });

                placeholder = false;
                placeholder_flags = false;
                placeholder_flags_start = NULL;

                fmt_add(&res, (FmtElem) {
                    .type = FmtElemType_STR,
                    .str = (FmtStrElem) {
                        .str = str + 1
                    }
                });
            }
        } else {
            if (c == '{') {
                placeholder_start = str + 1;
                placeholder = true;
                *str = '\0';
            }
        }

        str ++;
    }

    for (size_t i = 0; i < res.elems_size; i ++)
        if (res.elems[i].type == FmtElemType_STR)
            res.elems[i].str.len = strlen(res.elems[i].str.str);

    return res;
}
#endif

static void fmt_destroy(const Fmt fmt) {
    free(fmt.elems);
}

char *fmt_fmt(const Fmt fmt, const char *(*placeholder_find)(FmtPlaceholderElem, void *), void *userdata)
#ifndef MINIFMT_IMPL
;
#else
{
    char *res = malloc(1); // nt
    size_t res_len = 0;

    for (size_t i = 0; i < fmt.elems_size; i ++) {
        const FmtElem elem = fmt.elems[i];

        const char *add;
        size_t add_len;
        if (elem.type == FmtElemType_STR) {
            add = elem.str.str;
            add_len = elem.str.len;
        } else {
            add = placeholder_find(elem.placeholder, userdata);
            if (add == NULL)
                goto error;
            add_len = strlen(add);
        }

        if (add_len > 0) {
            res = realloc(res, res_len + add_len + 1); // nt
            memcpy(res + res_len, add, add_len);
            res_len += add_len;
        }
    }

    res[res_len] = '\0';
    return res;

error:
    free(res);
    return NULL;
}
#endif

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

const char *placeholder_placeholder_find(FmtPlaceholderElem elem, void *listPtr)
#ifndef MINIFMT_IMPL
;
#else
{
    const PlaceholderList list = *(PlaceholderList *)listPtr;

    for (size_t i = 0; i < list.elems_size; i ++) {
        const Placeholder *p = &list.elems[i];

        if (strcmp(p->key, elem.str) != 0)
            continue;

        switch (p->type) {
            case PlaceholderType_LONG: {
                int base = 10;
                if (elem.flag != NULL) {
                    if (strchr(elem.flag, 'h') != NULL) {
                        base = 16;
                    } else if (strchr(elem.flag, 'b') != NULL) {
                        base = 2;
                    } else if (strchr(elem.flag, 'o') != NULL) {
                        base = 8;
                    }
                }
                static char buf[200];
                ll_to_ascii(p->l, buf, base);
                return buf;
            }

            case PlaceholderType_DOUBLE: {
                static char buf[200];
                d_to_ascii(p->d, buf, 10);
                return buf;
            }

            case PlaceholderType_STR:
                return p->s;

            default:
                return NULL;
        }
    }

    return NULL;
}
#endif

static char *fmt_fmt_fmt(const Fmt fmt, PlaceholderList placeholders) {
    return fmt_fmt(fmt, placeholder_placeholder_find, &placeholders);
}

#endif //MINIFMT_H

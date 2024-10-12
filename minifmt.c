#include "minifmt.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

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

long long ascii_to_ll(const char * ascii, size_t len, const int radix)
{
    if (len == 0) return 0;

    bool neg = false;

    if (*ascii == '-') {
        ascii ++;
        len --;
        neg = true;
    } else if (*ascii == '+') {
        ascii ++;
        len --;
    }

    if (len == 0) return 0;

    long long val = 0;
    while (true) {
        val *= radix;
        if (*ascii >= '0' && *ascii <= '9') {
            val += *ascii - '0';
        } else {
            val += *ascii - 'a';
        }
        ascii ++;
        len --;
        if (len == 0) break;
    }

    if (neg)
        val = -val;
    return val;
}

static long s_nchr(const char * ascii, size_t len, char c)
{
    for (size_t i = 0; i < len; i ++)
        if (ascii[i] == c)
            return i;
    return -1;
}

double ascii_to_d(const char * ascii, size_t len, const int radix)
{
    long comma = s_nchr(ascii, len, '.');
    size_t biglen =
        (comma == -1)
        ? len
        : ((comma == 0) ? (0) : (comma - 1));

    double val = (double) ascii_to_ll(ascii, biglen, radix);

    if (comma != -1) {
        size_t smallLen = len-biglen-1;
        long long small =
            ascii_to_ll(ascii+comma+1, smallLen, radix);
        val += small / (pow((double) radix, (double) smallLen));
    }

    return val;
}

/** converts value to ascii into dest; returns position of null terminator; */
char *ll_to_ascii(long long value, char *dest, const int radix)
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

/** converts value to ascii into dest; returns position of null terminator */
char *d_to_ascii(const double value, char *dest, const size_t accuraccy)
{
    const long long valueL = (long long) value;
    dest = ll_to_ascii(valueL, dest, 10);
    *dest++ = '.';
    
    char fltbuf[200];
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

void fmt_add(Fmt *fmt, const FmtElem elem) {
    fmt->elems = (FmtElem *) realloc(fmt->elems, (fmt->elems_size + 1) * sizeof(FmtElem));
    fmt->elems[fmt->elems_size ++] = elem;
}

/** destroys str; str needs to live as long as returned fmt */
Fmt fmt_compile(char *str)
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

void fmt_destroy(const Fmt fmt) {
    free(fmt.elems);
}

char *fmt_fmt(const Fmt fmt, const char *(*placeholder_find)(FmtPlaceholderElem, void *), void *userdata)
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

const char *placeholder_placeholder_find(FmtPlaceholderElem elem, void *listPtr)
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

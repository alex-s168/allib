#ifndef SMALLSTR_H
#define SMALLSTR_H

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    /** not above 64 */
    unsigned char len;
    /** padded with zeros; null terminator required! */
    const char (*str)[65];
} opstr;

typedef struct {
    /** padded with zeros; null terminator required! */
    char str[65];
    /** not above 64 */
    unsigned char len;
} smallstr;

static inline opstr smallstr_op(const smallstr *str) {
    opstr s;
    s.len = str->len;
    s.str = (const char (*)[65]) &str->str;
    return s;
} 

int opstr_findzero(opstr str);
int opstr_find(opstr str, char c);
int opstr_countstart(opstr str, opstr other);
bool opstr_startswith(opstr str, opstr other);
int opstr_leading(opstr str, char c);
int opstr_countend(opstr str, opstr other);
bool opstr_endswith(opstr str, opstr other);
int opstr_trailing(opstr str, char c);
int opstr_leadingany(opstr str, const char *some); 
int opstr_trailingany(opstr str, const char *some);

#if defined(_WIN32) && !defined(__MINGW32__)
static size_t strnlen(const char *s, size_t maxlen) {
	size_t len;
	for (len = 0; len < maxlen; len ++, s++)
		if (*s == '\0')
            break;
	return len;
}
#endif

static inline smallstr smallstr_from_trunc(const char *c) {
    size_t take = strnlen(c, 64);
    smallstr str = {0};
    str.len = take;
    memcpy(str.str, c, take);
    return str;
}

static inline smallstr smallstr_empty() {
    smallstr str = {0};
    return str;
}

static inline smallstr smallstr_substr(const smallstr *str, int start, int len) {
    if (start < 0)
        return smallstr_empty();

    if (len < 0)
        len = str->len - start;

    smallstr new = {0};
    new.len = len;
    memcpy(new.str, str->str + start, len);
    return new;
}

#endif

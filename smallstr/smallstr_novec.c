#include "smallstr.h"

int opstr_find(opstr str, char c) {
    for (unsigned char i = 0; i < str.len; i ++)
        if ((*str.str)[i] == c)
            return i;
    return -1;
}

int opstr_findzero(opstr str) {
    return opstr_find(str, '\0');
}

int opstr_countstart(opstr str, opstr other) {
    unsigned char count = other.len;
    if (count > str.len)
        count = str.len;
    for (unsigned char i = 0; i < count; i ++) {
        if ((*str.str)[i] != (*other.str)[i])
            return i;
    }
    return other.len;
}

static inline bool opstr_startswith(opstr str, opstr other) {
    return opstr_countstart(str, other) == other.len;
}

int opstr_leading(opstr str, char c) {
    for (unsigned char i = 0; i < str.len; i ++) {
        if ((*str.str)[i] != c)
            return i;
    }
    return str.len;
}

int opstr_countend(opstr str, opstr other) {
    unsigned char i = other.len;
    if (i > str.len)
        i = str.len;
    unsigned char res = 0;
    while (i --> 0) {
        if ((*str.str)[i] != (*other.str)[i])
            return res;
        res ++;
    }
    return res;
}

static inline bool opstr_endswith(opstr str, opstr other) {
    return opstr_countend(str, other) == other.len;
}

int opstr_trailing(opstr str, char c) {
    unsigned char i = str.len;
    unsigned char take = 0;
    while (i --> 0) {
        if ((*str.str)[i] != c)
            return take;
        take ++;
    }
    return take;
}

int opstr_leadingany(opstr str, const char *some) {
    for (unsigned char i = 0; i < str.len; i ++) {
        if (!strchr(some, (*str.str)[i]))
            return i;
    }
    return str.len;
}

int opstr_trailingany(opstr str, const char *some) {
    unsigned char i = str.len;
    unsigned char take = 0;
    while (i --> 0) {
        if (!strchr(some, (*str.str)[i]))
            return take;
        take ++;
    }
    return take;
} 


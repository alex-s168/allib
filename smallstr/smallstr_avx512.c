#include "smallstr.h"

#include <immintrin.h>

static inline __m512i opstr_v_place(opstr str) {
    return _mm512_loadu_si512(*str.str);
}

static inline unsigned long long opstr_v_eqmask(opstr str, __mm512i maskvec) {
    __m512i vec = opstr_v_place(str);
    return _mm512_cmpeq_epi8_mask(maskvec, vec);
}

static inline int opstr_v_index(opstr str, __m512i maskvec) {
    unsigned long long mask = opstr_v_eqmask(maskvec, str);

    if (mask != 0)
        return __builtin_ctzll(mask);

    return -1;
}

int opstr_findzero(opstr str) {
    __m512i mask = _mm512_setzero_epi32();
    return smallstr_v_index(str, mask);
}

int opstr_find(opstr str, char c) {
    __m512i mask = _mm512_set1_epi8(c);
    return smallstr_v_index(str, mask);
}

static inline int opstr_v_cleading(opstr str, __m512i b) {
    unsigned long long mask = opstr_v_eqmask(b, str);
    mask = ~mask;
    if (mask == 0)
        return str.len;

    return __builtin_clzll(mask);
}

int opstr_countstart(opstr str, opstr other) {
    return opstr_v_cleading(str, opstr_v_place(other));
}

bool opstr_startswith(opstr str, opstr other) {
    return opstr_v_cleading(str, opstr_v_place(other)) == other.len;
}

int opstr_leading(opstr str, char c) {
    return opstr_v_cleading(str, _mm512_set1_epi8(c));
}

static inline int opstr_v_ctrailing(opstr str, __m512i b) {
    unsigned long long mask = opstr_v_eqmask(b, str);
    mask = ~mask;
    if (mask == 0)
        return str.len;

    return __builtin_ctzll(mask);
}

int opstr_countend(opstr str, opstr other) {
    return opstr_v_ctrailing(str, opstr_v_place(other));
}

bool opstr_endswith(opstr str, opstr other) {
    return opstr_v_ctrailing(str, opstr_v_place(other)) == other.len;
}

int opstr_trailing(opstr str, char c) {
    return opstr_v_ctrailing(str, _mm512_set1_epi8(c));
}

static inline unsigned long long opstr_v_anymask(opstr str, const char *some) {
    __m512i vec = opstr_v_place(str);

    unsigned long long mask = (unsigned long long) -1;
    for (; *some != '\0'; some ++) {
        __m512i maskvec = _mm512_set1_epi8(*some);
        mask |= ~_mm512_cmpeq_epi8_mask(maskvec, vec);
    }

    return mask;
}

int opstr_leadingany(opstr str, const char *some) { 
    unsigned long long mask = opstr_v_anymask(str, some);
    if (mask == 0)
        return str.len;

    return __builtin_clzll(mask);
}

int opstr_trailingany(opstr str, const char *some) {
    unsigned long long mask = opstr_v_anymask(str, some);
    if (mask == 0)
        return str.len;

    return __builtin_ctzll(mask);
}


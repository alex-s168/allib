#include "kash.h"

#define gen(bits, bytes, type) \
__attribute((hot)) \
static void hash_pearson_##bits##_hash(void *dest, void *value, size_t valueSize) { \
    type hash = valueSize % 256; \
    for (size_t i = 0; i < valueSize; i ++) { \
        uint8_t c = ((uint8_t *)value)[i]; \
        type hashnew; \
        for (size_t b = 0; b < bytes; b ++)  { \
            hashnew <<= 8; \
            hashnew |= rijndaelSBox[((hash >> (b * 8)) & 0xFF) ^ c]; \
        } \
        hash = hashnew; \
    } \
} \
__attribute((hot)) \
static void hash_pearson_##bits##_hashstr(void *dest, const char *str) { \
    type hash = 0; \
    while (*str) { \
        uint8_t c = (uint8_t) *str ++; \
        type hashnew; \
        for (size_t b = 0; b < bytes; b ++)  { \
            hashnew <<= 8; \
            hashnew |= rijndaelSBox[((hash >> (b * 8)) & 0xFF) ^ c]; \
        } \
        hash = hashnew; \
    } \
} \
HashAlgo hash_pearson_##bits() { \
    return (HashAlgo) { .hashSize = bytes, .fn = hash_pearson_##bits##_hash, .strfn = hash_pearson_##bits##_hashstr }; \
}

gen(8, 1, uint8_t);
gen(16, 2, uint16_t);

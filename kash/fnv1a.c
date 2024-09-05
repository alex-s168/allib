#include "kash.h"
#include <stdint.h>

// non-cryptographic hash function
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash

#define FNV1A(type, prime, offset, dest, value, valueSize) { \
    type hash = offset;                                      \
    for (size_t i = 0; i < valueSize; i ++) {                \
        uint8_t byte = ((uint8_t *)value)[i];                \
        hash ^= byte;                                        \
        hash *= prime;                                       \
    }                                                        \
    *(type *)dest = hash;                                    \
}

#define FNV1A_str(type, prime, offset, dest, str) {          \
    type hash = offset;                                      \
    while (*str) {                                           \
        uint8_t byte = (uint8_t) *str ++;                    \
        hash ^= byte;                                        \
        hash *= prime;                                       \
    }                                                        \
    *(type *)dest = hash;                                    \
}

#define gen(bits, bytes, type, prime, offset) \
__attribute((hot)) \
static void hash_fnv1a_##bits##_hash(void *dest, void *value, size_t valueSize) \
    FNV1A(type, prime, offset, dest, value, valueSize) \
__attribute((hot)) \
static void hash_fnv1a_##bits##_hashstr(void *dest, const char *str) \
    FNV1A_str(type, prime, offset, dest, str) \
HashAlgo hash_fnv1a_##bits() { \
    return (HashAlgo) { .hashSize = bytes, .fn = hash_fnv1a_##bits##_hash, .strfn = hash_fnv1a_##bits##_hashstr }; \
}


gen(32, 4, uint32_t, 0x01000193, 0x811c9dc5)
gen(64, 8, uint64_t, 0x100000001B3, 0xcbf29ce484222325)


static void hash_fnv1a_16_hash(void *dest, void *value, size_t valueSize) {
    uint32_t b32;
    hash_fnv1a_32_hash(&b32, value, valueSize);
    *(uint16_t*)dest = (b32 >> 16) ^ (b32 & 0xFFFF);
}

static void hash_fnv1a_16_hashstr(void *dest, const char *str) {
    uint32_t b32;
    hash_fnv1a_32_hashstr(&b32, str);
    *(uint16_t*)dest = (b32 >> 16) ^ (b32 & 0xFFFF);
}

// xor folding fnv1a 32
HashAlgo hash_fnv1a_16() {
    return (HashAlgo) {
        .hashSize = 2,
        .fn = hash_fnv1a_16_hash,
        .strfn = hash_fnv1a_16_hashstr,
    };
}

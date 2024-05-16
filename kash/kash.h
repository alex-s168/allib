#ifndef KASH_H
#define KASH_H

#include <stddef.h>

typedef struct {
    size_t hashSize;
    void (*fn)(void *dest, void *value, size_t valueSize);
} HashAlgo;

// non-cryptographic hash function
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
HashAlgo hash_fnv1a_32();
HashAlgo hash_fnv1a_64();

#endif //KASH_H

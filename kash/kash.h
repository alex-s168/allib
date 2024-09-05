#ifndef KASH_H
#define KASH_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t hashSize;
    void (*fn)(void *dest, void *value, size_t valueSize);
    void (*strfn)(void *dest, const char * str);
} HashAlgo;

// non-cryptographic hash function
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
// xor folding fnv1a 32:
HashAlgo hash_fnv1a_16();
HashAlgo hash_fnv1a_32();
HashAlgo hash_fnv1a_64();

HashAlgo hash_pearson_8();
HashAlgo hash_pearson_16();

extern const uint8_t rijndaelSBox[256];

#endif //KASH_H

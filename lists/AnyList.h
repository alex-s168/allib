/* Universal list interface */

#include <stddef.h>
#include "../attrib.h"

typedef struct {
    // you can use this to set the value too
    void *(*get)(void *self, size_t index); 
    // -1 if not foud
    long long int (*find)(void *self, const void *compare);
    size_t (*len)(void *self);

    size_t sizeOfSelf;
} AnyListImpl;

typedef struct {
    AnyListImpl *impl;

    size_t elSize;
UNSAFE
    void *self;
} AnyList;

typedef struct {
    void (*clear)(void *self);
    void (*removeAt)(void *self, size_t index);
    void (*removeRange)(void *self, size_t first, size_t last);
    void (*insertAt)(void *self, size_t index, const void *value);
    void (*reserveAdditional)(void *self, size_t additional);
    void (*pushBack)(void *self, const void *value);
    void (*pushBackAll)(void *self, AnyList *src);
    void (*pushFront)(void *self, const void *value);
    void (*pushFrontAll)(void *self, AnyList *src);
} MutAnyListImpl;

typedef struct {
    MutAnyListImpl *impl;

    AnyList any;
} MutAnyList;

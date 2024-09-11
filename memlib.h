#ifndef MEMLIB_H
#define MEMLIB_H

#include <stddef.h>

// optimized strided memcpy
void strided_memcpy(void* dest, size_t dest_stride,
                    const void *src, size_t src_stride,
                    size_t numEl,
                    size_t elSize);

#endif //MEMLIB_H

#include "memlib.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#if __WORDSIZE == 64
# define MALLOC_ALIGNMENT (16)
#elif __WORDSIZE == 32
# define MALLOC_ALIGNMENT (8)
#else 
# error wtf
#endif

static void strided_memcpy_same_stride(char* dest,
                                       const char *src,
                                       size_t numEl,
                                       size_t elSize, 
                                       size_t stride) {

    // no overlap
    if (dest >= src && dest <= src + numEl * stride) {
        __builtin_unreachable();
    }

    if (elSize > stride) {
        __builtin_unreachable();
    }

    if (elSize == 0) {
        __builtin_unreachable();
    }

    bool *mask;

    if (stride > 4096) {
        mask = malloc(stride);
        if (mask == NULL) return;
    } else {
        mask = __builtin_alloca_with_align(stride, MALLOC_ALIGNMENT);
    }

    mask = __builtin_assume_aligned(mask, MALLOC_ALIGNMENT);

    {
        size_t i = 0;
        for (; i < elSize; i ++) {
            mask[i] = true;
        }
        for (; i < stride; i ++) {
            mask[i] = false;
        }
    }

    for (size_t i = 0; i < numEl * stride; i += stride) {
        for (size_t j = 0; j < stride; j ++) {
            if (mask[j]) {
                dest[i + j] = src[i + j];
            }
        }
    }

    if (stride > 4096) {
        free(mask);
    }
}

static void strided_memcpy_diff_stride(char* dest, size_t destStride,
                                       const char *src, size_t srcStride,
                                       size_t numEl,
                                       size_t elSize) {

    // no overlap
    if (dest >= src && dest < src + numEl * srcStride) {
        __builtin_unreachable();
    }

    if (elSize > destStride) {
        __builtin_unreachable();
    }

    if (elSize > srcStride) {
        __builtin_unreachable();
    }

    if (elSize == 0) {
        __builtin_unreachable();
    }

    size_t maxStride = srcStride;
    if (destStride > maxStride)
        maxStride = destStride;

    bool * mask;
    char * tempElem;

    if (maxStride > 4096) {
        mask = malloc(maxStride);
        if (mask == NULL) return;
        tempElem = malloc(maxStride);
        if (tempElem == NULL) return;
    } else {
        mask = __builtin_alloca_with_align(maxStride, MALLOC_ALIGNMENT);
        tempElem = __builtin_alloca_with_align(maxStride, MALLOC_ALIGNMENT);
    }

    mask = __builtin_assume_aligned(mask, MALLOC_ALIGNMENT);
    tempElem = __builtin_assume_aligned(tempElem, MALLOC_ALIGNMENT);

    {
        size_t i = 0;
        for (; i < elSize; i ++) {
            mask[i] = true;
        }
        for (; i < maxStride; i ++) {
            mask[i] = false;
        }
    }

    for (size_t elemId = 0; elemId < numEl; elemId ++) {
        memcpy(tempElem, &src[elemId * srcStride], srcStride);

        size_t destI = elemId * destStride;
        for (size_t j = 0; j < destStride; j ++) {
            if (mask[j]) {
                dest[destI + j]  = tempElem[j];
            }
        }
    }

    if (maxStride > 4096) {
        free(mask);
        free(tempElem);
    }
}

void strided_memcpy(void* dest, size_t dest_stride,
                    const void *src, size_t src_stride,
                    size_t numEl,
                    size_t elSize) {

    if (dest_stride == src_stride) {
        if (dest_stride == elSize) {
            memcpy(dest, src, numEl * elSize);
        }
        else {
            strided_memcpy_same_stride(dest, src, numEl, elSize, dest_stride);
        }
    }
    else {
        strided_memcpy_diff_stride(dest, dest_stride,
                                   src, src_stride,
                                   numEl, elSize);
    }
}

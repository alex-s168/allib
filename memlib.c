#include "memlib.h"

#include <string.h>
#include <stdbool.h>

#define MALLOC_ALIGNMENT (8)
#define OPT_STRIDED_MEMCPY_MAX_STRIDE (128) // should be multiple of target vlen

#ifdef __TINYC__ 
#define __builtin_unreachable() /**/
#endif 

static void strided_memcpy_same_stride(char* dest,
                                       const char *src,
                                       size_t numEl,
                                       size_t elSize, 
                                       size_t stride) {

    if (stride > OPT_STRIDED_MEMCPY_MAX_STRIDE) {
        __builtin_unreachable();
    }

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

    bool mask[OPT_STRIDED_MEMCPY_MAX_STRIDE];

    {
        size_t i = 0;
        for (; i < elSize; i ++) {
            mask[i] = true;
        }
        for (; i < OPT_STRIDED_MEMCPY_MAX_STRIDE; i ++) {
            mask[i] = false;
        }
    }

    for (size_t i = 0; i < numEl * stride; i++) {
        if (mask[i % OPT_STRIDED_MEMCPY_MAX_STRIDE]) {
            dest[i] = src[i];
        }
    }
}

void strided_memcpy(void* dest, size_t dest_stride,
                    const void *src, size_t src_stride,
                    size_t numEl,
                    size_t elSize) {

    // no overlap
    if (dest >= src && dest < src + numEl * src_stride) {
        __builtin_unreachable();
    }

    if (elSize > dest_stride) {
        __builtin_unreachable();
    }

    if (elSize > src_stride) {
        __builtin_unreachable();
    }

    if (elSize == 0) {
        __builtin_unreachable();
    }

    if (dest_stride - elSize > OPT_STRIDED_MEMCPY_MAX_STRIDE || src_stride - elSize > OPT_STRIDED_MEMCPY_MAX_STRIDE) { 
        // other methods are too slow because really sparse data 

        for (size_t i = 0; i < numEl; i ++) {
            memcpy(&dest[dest_stride * i], &src[src_stride * i], elSize);
        }
    }
    else if (dest_stride == src_stride) {
        if (dest_stride == elSize) {
            memcpy(dest, src, numEl * elSize);
        }
        else {
            strided_memcpy_same_stride(dest, src, numEl, elSize, dest_stride);
        }
    }
    else {
        // TODO: implement new variant with scatter & gather 
        for (size_t i = 0; i < numEl; i ++) {
            memcpy(&dest[dest_stride * i], &src[src_stride * i], elSize);
        }
    }

}

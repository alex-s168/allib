// only works for clang but who uses gcc anyways, right?
#define typedef_vector(name, elty, len) \
    typedef elty name __attribute__ (( ext_vector_type (len) ));

#define velemsize(VT) (sizeof(VT) / sizeof(*VT))

#define vload(VT, arr) (*(VT*)(arr))

#define vstore(VT, arr, data) (*(VT*)(arr)) = data;

#define vmaskmov(VT, dest, mask, src) { \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        if (mask[i]) { \
            dest[i] = src[i]; \
        } \
    } \
}

#define vmaskload(VT, dest, mask, arr) \
    vmaskmov(VT, dest, mask, arr)

#define vmaskstore(VT, arr, mask, vec) \
    vmaskmov(VT, arr, mask, vec)

#define vshuffle(VT, dest, src, idcIn) { \
    size_t* idc = (size_t[]) idcIn; \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        dest[idc[i]] = src[i]; \
    } \
}

#define vbinary(VT, dest, src0, op, src1) { \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        dest[i] = src0[i] op src1[i]; \
    } \
}

#define vbinaryfn(VT, dest, src0, op, src1) { \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        dest[i] = op(src0[i], src1[i]); \
    } \
}

#define vunary(VT, dest, op, src) { \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        dest[i] = op (src[i]); \
    } \
}

#define vgather(VT, dest, arr, idc) { \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        dest[i] = arr[idc[i]]; \
    } \
}

#define vscatter(VT, arr, vec, idc) { \
    for (size_t i = 0; i < velemsize(VT); i ++) { \
        arr[idc[i]] = vec[i]; \
    } \
}

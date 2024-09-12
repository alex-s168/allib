#include "../memlib.h"

int main() {
    char data1[126];
    for (size_t i = 0; i < 126; i ++) {
        data1[i] = 69;
    }
    for (size_t i = 1; i < 126; i += 2) {
        data1[i] = 5;
    }

    char data2[63];
    for (size_t i = 0; i < 63; i ++) {
        data2[i] = i;
    }

    // interleave 

    strided_memcpy(data1, 3,
                   data2, 2,
                   63, 1);

    for (size_t i = 1; i < 126; i += 2) {
        if (data1[i] != 5) return 1;
    }
    for (size_t i = 0; i < 63; i ++) {
        if (data1[i * 2] != data2[i]) return 1;
    }

    return 0;
}

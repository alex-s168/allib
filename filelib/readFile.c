#include "filelib.h"

#include <string.h>

str readFile(FILE *file, Ally ally)
{
    char *out = yalloc(ally, 1);
    size_t out_size = 0;

    str line;
    while ((line = readLine(file, ally)) != NULL) {
        char *newbuf = yrealloc(ally, out, out_size, out_size + line.len + (out_size > 0 ? 2 : 1));
        if (newbuf == NULL) {
            yfree(ally, out, out_size);
            yfree(ally, (void*)line.data, line.len);
            return str_empty();
        }
        out = newbuf;
        if (out_size > 0)
            out[out_size ++] = '\n';
        memcpy(out + out_size, line.data, line.len);
        out_size += line.len;

        yfree(ally, (void*)line.data, line.len);
    }

    out[out_size] = '\0';
    return (str) { .len = out_size, .data = out };
}


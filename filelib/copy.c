#include "filelib.h"

void copyFile(FILE *dest, FILE *src, Ally ally)
{
    (void) ally;

    int c;
    while ((c = fgetc(src)) != EOF)
        fputc(c, dest);
}

size_t copyNFile(FILE *dest, FILE *src, size_t count, Ally ally)
{
    if (count == 0)
        return 0;
    
    char *buf = yalloc(ally, count);
    if (buf == NULL)
        return 0;
    const size_t actual = fread(buf, 1, count, src);
    fwrite(buf, 1, actual, dest);
    yfree(ally, buf, count);
    return actual;
}

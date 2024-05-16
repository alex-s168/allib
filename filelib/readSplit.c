#include "filelib.h"

str readSplitting(FILE *src, char split, Ally ally)
{
    if (feof(src))
        return NULL;
    
    static char buf[256];
    size_t buf_len = 0;

    char *bigbuf = yalloc(ally, 1);
    size_t bigbuf_len = 0;
    
    int c;
    while ((c = fgetc(src)) != EOF && c != split) {
        buf[buf_len ++] = c;
        if (buf_len == 256) {
            bigbuf = yrealloc(ally, bigbuf, bigbuf_len, bigbuf_len + buf_len + 1);
            memcpy(bigbuf + bigbuf_len, buf, buf_len);
            bigbuf_len += buf_len;
            buf_len = 0;
        }
    }
    
    bigbuf = yrealloc(ally, bigbuf, bigbuf_len, bigbuf_len + buf_len + 1);
    memcpy(bigbuf + bigbuf_len, buf, buf_len);
    bigbuf_len += buf_len;

    bigbuf[bigbuf_len] = '\0';

    return (str) { .len = bigbuf_len, .data = bigbuf };
}

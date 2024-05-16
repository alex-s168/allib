#include "filelib.h"

str readLine(FILE *file, Ally ally)
{
    if (file == NULL)
        return str_empty();
    
    size_t buf_size = 0;
    size_t buf_cap = 200;
    char *buf = yalloc(ally, buf_cap);
    if (buf == NULL)
        return str_empty();
    buf[buf_cap - 1] = '\0';

    while (fgets(buf + buf_size, buf_cap - buf_size, file) != NULL) {
        const size_t len = strlen(buf + buf_size);
        if (len == 0)
            break;
        buf_size += len;
        if (buf[buf_size - 1] == '\n') {
            buf_size --;
            if (buf[buf_size - 1] == '\r')
                buf_size --;
            break;
        }
        if (buf_size == buf_cap - 1) {
            char *new = yrealloc(ally, buf, buf_cap, buf_cap + 200);
            if (new == NULL) {
                yfree(ally, buf, buf_cap);
                return str_empty();
            }
            buf_cap += 200;
            buf = new;
            buf[buf_cap - 1] = '\0';
        }
    }

    buf[buf_size] = '\0';

    if (feof(file) && buf_size == 0) {
        yfree(ally, buf, buf_cap);
        buf = NULL;
    }

    return (str) { .len = buf_size, .data = buf };
}

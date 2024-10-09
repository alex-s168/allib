#include "filelib.h"

DynamicList TYPES(char)
readLine(FILE *file, Ally ally)
{
    DynamicList TYPES(char) out;
    DynamicList_init(&out, sizeof(char), ally, feof(file) ? 0 : 200);

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\r') {
            (void) getc(file); // '\n'
        }
        if (c == '\r' || c == '\n')
        {
            break;
        }

        DynamicList_add(&out, &c);
    }

    return out;
}

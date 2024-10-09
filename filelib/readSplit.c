#include "filelib.h"

DynamicList TYPES(char)
readSplitting(FILE *file, char split, Ally ally)
{
    DynamicList TYPES(char) out;
    DynamicList_init(&out, sizeof(char), ally, feof(file) ? 0 : 200);

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        char c = fgetc(file);
        if (c == split) {
            break;
        }

        DynamicList_add(&out, &c);
    }

    return out;
}

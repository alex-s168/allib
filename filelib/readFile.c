#include "filelib.h"

DynamicList TYPES(char)
readFile(FILE *file, Ally ally)
{
    DynamicList TYPES(char) out;
    DynamicList_init(&out, sizeof(char), ally, feof(file) ? 0 : 200);

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        DynamicList_add(&out, &c);
    }

    return out;
}


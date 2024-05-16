/*
Copyright 2024 Alexander Nutz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MEMLIB_H
#define MEMLIB_H

#include <stddef.h>
#include <stdlib.h>

typedef struct {
    void   **allocs;
    size_t   allocs_size;
} AllocGroup;

static void allocgroup_init(AllocGroup *group) {
    group->allocs = NULL;
    group->allocs_size = 0;
}

static void allocgroup_free(AllocGroup group) {
    for (size_t i = 0; i < group.allocs_size; i ++)
        free(group.allocs[i]);

    free(group.allocs);
}

static void allocgroup_add(AllocGroup *dest, void *alloc) {
    dest->allocs = realloc(dest->allocs, (dest->allocs_size + 1) * sizeof (char *));
    dest->allocs[dest->allocs_size ++] = alloc;
}

#endif //MEMLIB_H

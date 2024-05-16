//
// Created by Alexander Nutz on 04/03/2024.
//

#ifndef KALLOK_VIRTUAL_H
#define KALLOK_VIRTUAL_H

#include <stddef.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

typedef struct {
    void  *data;
    size_t size;

#ifdef _WIN32
    HANDLE hFile;
    HANDLE hMap;
#else
    int fd;
#endif
} VirtAlloc;

VirtAlloc virtualMap(int *err, const char *path);

void virtualUnmap(VirtAlloc alloc);

#endif //KALLOK_VIRTUAL_H

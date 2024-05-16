//
// Created by Alexander Nutz on 29/02/2024.
//

#include "kallok.h"

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

size_t page_size() {
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    return system_info.dwPageSize;
}

struct Page alloc_pages(size_t bytes) {
    bytes = makeMultiple(bytes, page_size());

    void *p = (void *) VirtualAlloc(NULL,
                                    bytes,
                                    MEM_COMMIT,
                                    PAGE_READWRITE);

    return (struct Page) {
        .ptr = p,
        .size = bytes
    };
}

void make_exec(void *ptr, size_t size) {
    DWORD dummy;
    VirtualProtect(ptr, size, PAGE_EXECUTE_READ, &dummy);
}

void free_page(struct Page page) {
    VirtualFree(page.ptr, 0, MEM_RELEASE);
}

#else

#include <sys/mman.h>
#include <unistd.h>

size_t page_size() {
    return (size_t) getpagesize();
}

struct Page alloc_pages(size_t bytes) {
    bytes = makeMultiple(bytes, page_size());

    void *p = (void *) mmap(0, bytes,
                            PROT_READ | PROT_WRITE | PROT_EXEC,
                            MAP_PRIVATE | MAP_ANONYMOUS,
                            -1, 0);

    return (struct Page) {
        .ptr = p,
        .size = bytes
    };
}

void make_exec(void *ptr, size_t size) {

}

void free_page(struct Page page) {
    munmap(page.ptr, page.size);
}

#endif

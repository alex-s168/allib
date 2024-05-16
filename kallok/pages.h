//
// Created by Alexander Nutz on 29/02/2024.
//

/**
 * Library to allocate OS pages on Windows and Linux.
 * Can also make pages executable for JITs.
 */

#ifndef KALLOK_PAGES_H
#define KALLOK_PAGES_H

#include <stddef.h>

struct Page {
    void   *ptr;
    size_t  size;
};

/**
 * Allocate a readable and writable page
 */
struct Page alloc_pages(size_t bytes);

/**
 * Make a section in the page executable and readonly
 */
void make_exec(void *ptr, size_t size) ;

/**
 * Frees the whole page
 */
void free_page(struct Page page);

size_t page_size();

#endif //KALLOK_PAGES_H

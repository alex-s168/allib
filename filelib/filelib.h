#ifndef FILELIB_H
#define FILELIB_H

#include <stdio.h>

#include "../attrib.h"
#include "../dynamic_list/dynamic_list.h"
#include "../kallok/kallok.h"

/**
 * file should be buffered for performance!
 * Read the next line. Result does not include line terminator
 * works for CRLF and LF
 */
DynamicList TYPES(char) readLine(FILE *file, Ally ally);

/**
 * file should be buffered for performance!
 * Reads the whole file 
 */
DynamicList TYPES(char) readFile(FILE *file, Ally ally);

/**
 * Read until "split" found or end of file; if split found: skip it
 *
 * file should be buffered for performance!
 *
 * example usage:
 * \code 
 * DynamicList s;
 * while ((s = readSplitting(file, '\n', ally)).data)
 * {
 *   puts("\nfound line: ");
 *   fwrite(s.fixed.data, sizeof(char), s.fixed.len, stdout);
 *   DynamicList_clear(&s);
 * }
 * \endcode 
 */
DynamicList TYPES(char) readSplitting(FILE *src, char split, Ally ally);

/** file should be buffered for performance! */
void copyFile(FILE *dest, FILE *src, Ally ally);

/** copies at maximum [count] bytes; return actual number of copied bytes; ally is used to allocate buffers */
size_t copyNFile(FILE *dest, FILE *src, size_t count, Ally ally);

#endif //FILELIB_H

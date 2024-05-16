#ifndef FILELIB_H
#define FILELIB_H

#include <stdio.h>

#include "../str/str.h"
#include "../kallok/kallok.h"

str readLine(FILE *file, Ally ally);
str readFile(FILE *file, Ally ally);
str readSplitting(FILE *src, char split, Ally ally);

void copyFile(FILE *dest, FILE *src, Ally ally);
size_t copyNFile(FILE *dest, FILE *src, size_t count, Ally ally);

#endif //FILELIB_H

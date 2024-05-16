// Define CLI_IMPL in a seperate C file and include this header

/*
Copyright 2024 Alexander Nutz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char **str;
    size_t len;
} Flag;

static bool flagExist(const Flag flag) {
    return flag.str != NULL;
}

Flag getFlag(const int argc, char **argv, const char *flag)
#ifndef CLI_IMPL
;
#else
{
    for (size_t i = 1; i < (size_t) argc; i ++) {
        if (strcmp(argv[i], flag) == 0) {
            size_t len = 1;
            for (size_t j = i + 1; j < (size_t) argc; j ++) {
                if (argv[j][0] == '-')
                    break;
                len ++;
            }
            
            return (Flag) { .str = &argv[i], .len = len };
        }
    }
    
    return (Flag) { .str = NULL, .len = 0 };
}
#endif

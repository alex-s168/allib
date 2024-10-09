// Define CLI_IMPL in a seperate C file and include this header

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

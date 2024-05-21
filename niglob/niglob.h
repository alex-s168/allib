#include <stddef.h>
#include <stdbool.h>

// *             any
// **            literal *

// example:
//   abcgelf   matches pattern   abc*e*f

bool patterneq(const char * pattern, const char * value);

// {option0,..}   one of options; can be nested 

// example: 
//   aa{b{,e},{c,d},}f   yields:  aabf  aabef  aacf  aadf  aaf

typedef struct {
    size_t   count;
    char * * items;
} curlyexp_res;

curlyexp_res curlyexp(const char * pattern);
void curlyexp_free(curlyexp_res res);

// backslashes are relaced with slashes
// (username)~/  is replaced with the homedir of optional username
// C:/path       works on windows: absolute
// /path         absolute
// {option0,..}  one of options
// *             any (part of path segment can be any)
// ../           go up one dir
// //            replaced with slash

// example:
//  ~/abc/{cpu,gpu}{.h,.c}  can yield: /home/alex/abc/cpu.h  /home/alex/abc/cpu.c   /home/alex/abc/gpu.h  /home/alex/abc/gpu.c 

typedef struct {
    size_t       abspath_len;
    const char * abspath;

    size_t       filename_len;
    const char * filename;
} niglob_entry;

typedef struct {
    size_t         count;
    niglob_entry * items;
} niglob_res;

niglob_res niglob(const char * pattern, /* optional */ const char * working_dir);
void niglob_free(niglob_res res);

#include <stddef.h>
#include <stdbool.h>


#ifdef _WIN32 
# define PATH_IGNORE_CAPS true
#else 
# define PATH_IGNORE_CAPS false 
#endif

// *             any
// **            literal *

// example:
//   abcgelf   matches pattern   abc*e*f

bool patterneq(const char * pattern, const char * value, bool ignoreCaps);

// {option0,..}   one of options; can be nested 

// example: 
//   aa{b{,e},{c,d},}f   yields:  aabf  aabef  aacf  aadf  aaf

typedef struct {
    size_t   count;
    char * * items;
} curlyexp_res;

curlyexp_res curlyexp(const char * pattern);
void curlyexp_free(curlyexp_res res);

// strchrnul but only in most outer layer of curly epxpr (= skips curly expr)
char * curlyexp_strchrnul(char * text, char find);

char * getworkingdir_heap();


// backslashes are relaced with slashes
// (username)~/  is replaced with the homedir of optional username
// C:/path       works on windows: absolute
// /path         absolute
// *             any (part of path segment can be any)
// ../           go up one dir
// //            replaced with slash

typedef struct {
    char * str;
    int    win_drive_num;
} niglob_expand_res;

niglob_expand_res niglob_expand(const char * pattern, /* optional */ const char * working_dir);
// returns a new allocation and doesn't destroy old
char * niglob_expand_res_prependWinDrive_copyAlloc(niglob_expand_res res);
// destroys old allocation
char * niglob_expand_res_prependWinDrive_freeAlloc(niglob_expand_res res);

#ifdef _WIN32 
niglob_expand_res niglob_expand_res_ensureDriveOrDefault_WIN32(niglob_expand_res res);
#endif 


// for all curlyexp matches: applies niglob_expand to the pattern, and then matches directories in the system

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

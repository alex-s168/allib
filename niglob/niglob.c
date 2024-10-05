#include "niglob.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define MINIRENT_IMPLEMENTATION
#include "../minirent.h"

#ifdef _WIN32
# include <direct.h>
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#else
# include <unistd.h>
# include <limits.h>
#endif

#define ALLOC_CHECKS

// *             any
// **            literal *

// example:
//   abcgelf   matches pattern   abc*e*f

static bool charEq(char a, char b, bool ignoreCaps) {
    if (ignoreCaps) {
        return tolower(a) == tolower(b);
    }
    return a == b;
}

bool patterneq(const char * pattern, const char * value, bool ignoreCaps) {
    while (*pattern != '\0') {
        char want = pattern[0];

        if (want == '*') {
            char next = pattern[1];
            if (next == '*') {
                if (!charEq(*value, '*', ignoreCaps))
                    return false;
                pattern += 2;
                value ++;
            }
            else if (next == '\0') {
                if (*value == '\0')
                    return false;
                return true;
            }
            else {
                if (charEq(*value, next, ignoreCaps))
                    value ++;
                while (!charEq(*value, next, ignoreCaps)) {
                    if (*value == '\0')
                        return false;
                    value ++;
                }
                value ++;
                pattern += 2;
            }
        }
        else {
            if (!charEq(*value, *pattern, ignoreCaps))
                return false;
            pattern ++;
            value ++;
        }
    }
    return *value == '\0';
}



// {option0,..}   one of options; can be nested 

// example: 
//   aa{b{,e},{c,d},}f   yields:  aabf  aabef  aacf  aadf  aaf

typedef struct {
    const char * ptr;
    size_t       len;
    bool         freepls; // if true, str is nullterminated
} curlyexp_res_int_e;

typedef struct {
    size_t count;
    curlyexp_res_int_e * items;
} curlyexp_res_int;

static void curlyexp_rec(curlyexp_res_int * dest, const char * const pattern, size_t pattern_len) {
    const char * curly_open = NULL;
    for (size_t i = 0; i < pattern_len; i ++) {
        if (pattern[i] == '{') {
            curly_open = pattern + i;
            break;
        }
    }

    if (curly_open == NULL) {
#ifdef ALLOC_CHECKS
        void* oldptr = dest->items;
#endif 
        dest->items = realloc(dest->items, (dest->count + 1) * sizeof(*dest->items));
#ifdef ALLOC_CHECKS
        if (dest->items == NULL) {
            dest->items = oldptr;
            return;
        }
#endif 

        dest->items[dest->count].len = pattern_len;
        dest->items[dest->count].freepls = false;
        dest->items[dest->count++].ptr = pattern;
        return;
    }

    size_t curly_open_idx = curly_open - pattern;

    const char * curly_close = NULL;

    size_t nesting = 0;
    for (size_t i = curly_open_idx; i < pattern_len; i ++) {
        char c = pattern[i];
        if (c == '{') {
            nesting ++;
        } 
        else if (c == '}') {
            if (nesting == 0)
                nesting = 2;
            nesting --;
            if (nesting == 0) {
                curly_close = pattern + i;
                break;
            }
        }
    }

    if (curly_close == NULL)
        return;

    size_t curly_close_idx = curly_close - pattern;

    curlyexp_res_int rest;
    rest.items = NULL;
    rest.count = 0;
    curlyexp_rec(&rest, pattern + curly_close_idx + 1, pattern_len - curly_close_idx - 1);

    curlyexp_res_int self;
    self.items = NULL;
    self.count = 0;

    size_t start = curly_open_idx + 1;
    for (size_t i = start; i < curly_close_idx; i ++) {
        if (pattern[i] == ',') {
            curlyexp_rec(&self, pattern + start, i - start);
            start = i + 1;
        }
    }
    curlyexp_rec(&self, pattern + start, curly_close_idx - start);

    // 0 -> curly_open_idx, this, rest

    for (size_t i = 0; i < self.count; i ++) {
        curlyexp_res_int_e s = self.items[i];

        for (size_t j = 0; j < rest.count; j ++) {
            curlyexp_res_int_e r = rest.items[j];

            size_t len = curly_open_idx + s.len + r.len;
            char * str = malloc(sizeof(char) * (len + 1));
#ifdef ALLOC_CHECKS 
            if (str == NULL)
                continue;
#endif
            memcpy(str, pattern, curly_open_idx);
            memcpy(str + curly_open_idx, s.ptr, s.len);
            memcpy(str + curly_open_idx + s.len, r.ptr, r.len);
            str[curly_open_idx + s.len + r.len] = '\0';

            curlyexp_res_int_e all;
            all.ptr = str;
            all.len = len;
            all.freepls = true;

            dest->items = realloc(dest->items, (dest->count + 1) * sizeof(*dest->items));
#ifdef ALLOC_CHECKS
            if (dest->items == NULL)
                continue;
#endif 
            dest->items[dest->count++] = all;
        }

        if (s.freepls)
            free((void*) s.ptr);
    }

    for (size_t i = 0; i < rest.count; i ++)
        if (rest.items[i].freepls)
            free((void*)rest.items[i].ptr);
}

curlyexp_res curlyexp(const char * pattern) {
    curlyexp_res_int resi;
    resi.items = NULL;
    resi.count = 0;
    curlyexp_rec(&resi, pattern, strlen(pattern));
    curlyexp_res res;
    res.items = malloc(sizeof(char*) * resi.count);
#ifdef ALLOC_CHECKS
    if (res.items == NULL) {
        for (size_t i = 0; i < resi.count; i ++)
            if (resi.items[i].freepls)
                free((char*) resi.items[i].ptr);
        free(resi.items);
        res.count = 0;
        return res;
    }
#endif 
    res.count = resi.count;

    for (size_t i = 0; i < resi.count; i ++) {
        curlyexp_res_int_e e = resi.items[i];

        if (e.freepls) {
            res.items[i] = (char*) e.ptr;
        }
        else {
            char * a = malloc(sizeof(char) * (e.len + 1));
#ifdef ALLOC_CHECKS
            if (a == NULL) {
                i --;
                res.count --;
                continue;
            }
#endif 
            memcpy(a, e.ptr, e.len);
            a[e.len] = '\0';
            res.items[i] = a;
        }
    }

    free(resi.items);
    return res;
}

void curlyexp_free(curlyexp_res res) {
    for (size_t i = 0; i < res.count; i ++)
        free(res.items[i]);
    free(res.items);
}

char * curlyexp_strchrnul(char * text, char find) {
    if (text == NULL) {
        return NULL;
    }
    size_t level = 0;
    char c;
    while ((c = *text) != '\0') {
        if (c == '{') {
            level ++;
        }
        else if (c == '}') {
            level --;
        }
        else if (c == find && level == 0) {
            return text;
        }

        text ++;
    }
    return NULL;
}





// backslashes are relaced with slashes
// (username)~/  is replaced with the homedir of optional username
// C:/path       works only on windows: absolute
// /path         absolute
// {option0,..}  one of options
// *             any
// ../           go up one dir
// //            replaced to /

// example:
//  ~/abc/{cpu,gpu}{.h,.c}  can yield: /home/alex/abc/cpu.h  /home/alex/abc/cpu.c   /home/alex/abc/gpu.h  /home/alex/abc/gpu.c 

#ifdef _WIN32 
static char * strchrnul(char * ptr, char search) {
    while (*ptr != '\0') {
        if (*ptr == search)
            break;
        ptr ++;
    }
    return ptr;
}
#endif 

static void skip_slashes(char * * segment) {
    *segment = curlyexp_strchrnul(*segment, '/');
    if (*segment == NULL) {
        return;
    }

    while (**segment == '/')
       (*segment) ++;
}

static bool niglob_int_rec(const char * parent, niglob_res * res, char * expanded) {
    if (expanded == NULL)
        return true;

    char *segment = expanded;

    skip_slashes(&expanded);
    while (expanded && expanded[0] == '.' && expanded[1] == '.' && (expanded[2] == '/' || expanded[2] == '\0')) {
        skip_slashes(&expanded);
        segment = expanded;
        skip_slashes(&expanded);
        if (segment == NULL) {
            return false;
        }
    }

    char *slash = curlyexp_strchrnul(segment, '/');
    if (slash) {
        *slash = '\0';
    }

    while (*segment == '\0') {
        if (*expanded == '\0')
            return true;
        segment = expanded;
        skip_slashes(&expanded);

        char *slash = curlyexp_strchrnul(segment, '/');
        if (slash) {
            *slash = '\0';
        }
    }

    if (segment == NULL) {
        return false;
    }

    DIR* dir = opendir(parent);
    if (dir == NULL)
        return true;

    struct dirent * entry;
    while ((entry = readdir(dir)) != NULL) {
        const char * name = entry->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        bool matches = patterneq(segment, name, PATH_IGNORE_CAPS);
        if (!matches)
            continue;

        size_t name_len = strlen(name);
        size_t len = strlen(parent) + 1 + name_len;
        char * full = malloc(sizeof(char) * (len + 1 + name_len + 1));
#ifdef ALLOC_CHECKS
        if (full == NULL)
            break;
#endif 
        sprintf(full, "%s/%s", parent, name);
        char * fncopy = full + len + 1; // using buffer for other shit
        memcpy(fncopy, name, name_len + 1);

        if (niglob_int_rec(full, res, expanded)) {
            niglob_entry item;
            item.filename = fncopy;
            item.filename_len = name_len;
            item.abspath = full;
            item.abspath_len = len;
            res->items = realloc(res->items, sizeof(niglob_entry) * (res->count + 1));
#ifdef ALLOC_CHECKS
            if (res->items == NULL) {
                free(full);
                break;
            }
#endif 
            res->items[res->count++] = item;
        }
        else {
            free(full);
        }
    }

    closedir(dir);

    return false;
}

char * getworkingdir_heap() {
#ifdef _WIN32
    char buf[4096];
    _getdcwd(0, buf, 4095);
#else
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
#endif

    size_t len = strlen(buf);
    char* new = malloc(sizeof(char) * len);
    if (new == NULL)
        return NULL; 
    memcpy(new, buf, sizeof(char) * (len + 1)); // nt 
    return new;
}

#define WIN_DRIVE(c) (c - 'A' + 1)

niglob_expand_res niglob_expand(const char * pattern, /* optional */ const char * working_dir) {
    int win_drive = 0; // default drive
    if (pattern[0] != '\0' && pattern[1] == ':') {
        win_drive = WIN_DRIVE(pattern[0]);
        pattern += 2;
    }

    if (working_dir == NULL) {
#ifdef _WIN32
        char buf[4096];
        working_dir = _getdcwd(win_drive, buf, 4095);
#else
        char buf[PATH_MAX];
        working_dir = getcwd(buf, PATH_MAX);
#endif
    }
    // working_dir can still be null!
    
    char * expanded = NULL;
    size_t expanded_len = 0;

    const char * tilde = strchr(pattern, '~');
    if (tilde != NULL) {
        static char username[256];
        unsigned long username_len = tilde - pattern;

        if (username_len == 0) {
#ifdef _WIN32
            username_len = 255;
            if (GetUserName(username, &username_len) == 0) {
                username_len = 0;
            } else {
                username_len --;
            }
#else
            if (getlogin_r(username, 256) == 0) {
                username_len = strlen(username);
            } else {
                username_len = 0;
            }
#endif
        } else {
            if (username_len > 255)
                username_len = 255;

            memcpy(username, tilde + 1, username_len);
            tilde += username_len;
            username[username_len] = '\0';
        }

        // username_len is strlen(username) or 0 on failure

        pattern = tilde + 1;

        if (username_len > 0) {
            const char * prefix;
            size_t       prefix_len;

            // TODO: this does not always work (not always in /Users or /home dir)
#ifdef _WIN32
            prefix = "/Users/";
            prefix_len = 7;
#else
            prefix = "/home/";
            prefix_len = 6;
#endif

            size_t pattern_len = strlen(pattern);
            expanded_len = prefix_len + username_len + 1 + pattern_len;
            expanded = malloc(sizeof(char) * (expanded_len + 1));
#ifdef ALLOC_CHECKS
            if (expanded == NULL)
                goto fail;
#endif 

            char *dst = expanded;
            memcpy(dst, prefix, prefix_len); dst += prefix_len;
            memcpy(dst, username, username_len); dst += username_len;
            *dst++ = '/';
            memcpy(dst, pattern, pattern_len + 1);
        }
    }

    if (expanded_len == 0) {
        size_t pattern_len = strlen(pattern);

        if (pattern[0] == '/' || pattern[0] == '\\') {
            expanded_len = pattern_len;
            expanded = malloc(sizeof(char) * (expanded_len + 1));
#ifdef ALLOC_CHECKS
            if (expanded == NULL)
                goto fail;
#endif 

            memcpy(expanded, pattern, pattern_len + 1);
        } else {
            if (working_dir == NULL)
                goto fail;

            size_t working_dir_len = strlen(working_dir);
            expanded_len = working_dir_len + 1 + pattern_len;
            expanded = malloc(sizeof(char) * (expanded_len + 1));
#ifdef ALLOC_CHECKS
            if (expanded == NULL)
                goto fail;
#endif

            memcpy(expanded, working_dir, working_dir_len);
            expanded[working_dir_len] = '/';
            memcpy(expanded + working_dir_len + 1, pattern, pattern_len + 1);
        }
    }

    // checking for colon again because windows functions might hate us
    if (expanded[0] != '\0' && expanded[1] == ':') {
        int win_drive_2 = WIN_DRIVE(expanded[0]);
        if (win_drive_2 != win_drive)
            goto fail;

        memmove(expanded, expanded + 2, expanded_len + 1);
        expanded_len -= 2;
    }

    for (size_t i = 0; i < expanded_len; i ++)
        if (expanded[i] == '\\')
            expanded[i] = '/';

    return (niglob_expand_res) { expanded, win_drive };

fail:
    if (expanded != NULL)
        free(expanded);

    return (niglob_expand_res) { NULL, 0 };
}

// returns a new allocation and doesn't destroy old
char * niglob_expand_res_prependWinDrive_copyAlloc(niglob_expand_res res) {
    if (res.str == NULL)
        return NULL;

    size_t len = strlen(res.str);

    size_t alloclen = len;
    if (res.win_drive_num != 0) {
        alloclen += 2;
    }

    char * r = malloc(sizeof(char) * (alloclen + 1));
    if (r == NULL)
        return NULL;

    if (res.win_drive_num != 0) {
        r[0] = res.win_drive_num + 'A' - 1;
        r[1] = ':';
        memcpy(r + 2, res.str, len + 1);
    } else {
        memcpy(r, res.str, len + 1);
    }

    return r;
}

// destroys old allocation
char * niglob_expand_res_prependWinDrive_freeAlloc(niglob_expand_res res) {
    if (res.str == NULL)
        return NULL;

    if (res.win_drive_num == 0) {
        return res.str;
    }

    size_t len = strlen(res.str);

    char * r = malloc(sizeof(char) * (len + 3));
    if (r == NULL)
        return NULL;

    r[0] = res.win_drive_num + 'A' - 1;
    r[1] = ':';
    memcpy(r + 2, res.str, len + 1);

    free(res.str);
    return r;
}

#ifdef _WIN32 
niglob_expand_res niglob_expand_res_ensureDriveOrDefault_WIN32(niglob_expand_res res) {
    if (res.win_drive_num == 0) {
        res.win_drive_num = _getdrive();
    }
    return res;
}
#endif 

static void niglob_int(niglob_res * res, const char * pattern, const char * working_dir) {
    niglob_expand_res exp = niglob_expand(pattern, working_dir);
#ifdef _WIN32
    exp = niglob_expand_res_ensureDriveOrDefault_WIN32(exp);
#endif 
    if (exp.str == NULL)
        return;

#ifdef _WIN32
    char drivestr[] = { exp.win_drive_num + 'A' - 1, ':', '/', '\0' };
#else 
    char * drivestr = NULL;
#endif

    niglob_int_rec(drivestr, res, exp.str);
    free(exp.str);
}

niglob_res niglob(const char * pattern, const char * working_dir) {
    if (pattern == NULL)
        perror("\"niglob\" function parameter \"pattern\" can not be NULL!");

    niglob_res res;
    res.items = NULL;
    res.count = 0;

    curlyexp_res curly = curlyexp(pattern);
    for (size_t i = 0; i < curly.count; i ++) {
        niglob_int(&res, curly.items[i], working_dir);
    }
    curlyexp_free(curly);

    return res;
}

void niglob_free(niglob_res res) {
    for (size_t i = 0; i < res.count; i ++) {
        niglob_entry e = res.items[i];
        free((void*) e.abspath); // abspath and filename are same alloc
    }
    free(res.items);
}

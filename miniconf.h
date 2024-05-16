// Define MINICONF_IMPL and FILELIB_IMPL in a seperate C file and include this header

/*
Copyright 2024 Alexander Nutz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MINICONF_H
#define MINICONF_H

#include "filelib.h"
#include "memlib.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char **lines;
    size_t       lines_len;
} Config;

static void config_init(Config *config) {
    config->lines = NULL;
    config->lines_len = 0;
}

static void config_destroy(Config *config) {
    free(config->lines);
    config->lines = NULL;
    config->lines_len = 0;
}

static void config_add_line(Config *dest, const char *line) {
    dest->lines = realloc(dest->lines, (dest->lines_len + 1) * sizeof (char *));
    dest->lines[dest->lines_len ++] = line;
}

static void config_add_all_lines_adv(Config *dest,
                                     const Config src,
                                     const size_t start_line,
                                     const size_t end_line,
                                     const size_t indent)
{
    for (size_t i = start_line; i <= end_line; i ++) {
        config_add_line(dest, src.lines[i] + indent);
    }
}

static void config_add_all_lines(Config *dest, const Config src) {
    if (src.lines_len == 0)
        return;
    config_add_all_lines_adv(dest, src, 0, src.lines_len - 1, 0);
}

static AllocGroup config_add_file(Config *dest, FILE *file) {
    AllocGroup alloc;
    allocgroup_init(&alloc);

    char *line;
    while ((line = readLine(file)) != NULL) {
        config_add_line(dest, line);
        allocgroup_add(&alloc, line);
    }

    return alloc;
}

void config_child(Config *dest,
                  const Config cfg,
                  const char *child,
                  bool *is_found)
#ifndef MINICONF_IMPL
;
#else
{
    bool found = false;
    size_t start_line = 0;
    for (size_t i = 0; i < cfg.lines_len; i ++) {
        const char *line = cfg.lines[i];
        if (line[0] == ' ') // higher indent
            continue;

        const char *col = strchr(line, ':');
        if (col == NULL) // broken file
            continue;

        const size_t len = col - line;

        if (memcmp(child, line, len) != 0)
            continue;

        if (child[len] != '\0')
            continue;

        start_line = i;
        found = true;
        break;
    }
    if (!found)
        return;
    *is_found = found;

    size_t end_line = start_line;
    for (size_t i = start_line + 1; i < cfg.lines_len; i ++) {
        const char *line = cfg.lines[i];
        if (line[0] == ' ') // higher indent
            end_line ++;
        else
            break;
    }

    config_add_line(dest, cfg.lines[start_line]);

    if (end_line - start_line > 0)
        config_add_all_lines_adv(dest, cfg, start_line + 1, end_line, 4);
}
#endif

void config_children(Config *dest,
                     const Config cfg,
                     const char *path_in,
                     bool *is_found)
#ifndef MINICONF_IMPL
;
#else
{
    const size_t len = strlen(path_in);
    char *path = malloc(len + 1);
    memcpy(path, path_in, len + 1);

    Config res;
    config_init(&res);
    config_add_all_lines(&res, cfg);

    for (size_t i = 0; i < len; i ++)
        if (path[i] == '/')
            path[i] = '\0';

    for (size_t i = 0; i < len; i ++) {
        char *segment;
        if (i == 0)
            segment = path;
        else if (path[i] == '\0')
            segment = path + i + 1;
        else
            continue;

        // process path segment

        Config found;
        config_init(&found);

        bool is_found_x;
        config_child(&found, res, segment, &is_found_x);
        if (!is_found_x) {
            *is_found = false;
            goto defer;
        }

        config_destroy(&res);
        res = found;
    }

    config_add_all_lines(dest, res);

    *is_found = true;

defer:
    config_destroy(&res);

    free(path);
}
#endif

typedef enum {
    ConfigType_NUM,
    ConfigType_BOOL,
    ConfigType_STRING,

    ConfigType_BLOCK,
    ConfigType_INVALID,
} ConfigType;

const char *config_value(const Config cfg)
#ifndef MINICONF_IMPL
;
#else
{
    if (cfg.lines_len == 0)
        return NULL;

    const char *colon = strchr(cfg.lines[0], ':');
    if (colon == NULL)
        return NULL;

    return colon + 1 + strspn(colon + 1, " ");
}
#endif

ConfigType config_value_type(const char *value)
#ifndef MINICONF_IMPL
;
#else
{
    if (value == NULL)
        return ConfigType_INVALID;

    switch (value[0]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':

        case 'x':
        case 'b':
            return ConfigType_NUM;

        case 't':
            return ConfigType_BOOL;

        case '"':
            return ConfigType_STRING;

        case '\0':
            return ConfigType_BLOCK;

        default:
            return ConfigType_INVALID;
    }
}
#endif

long config_get_long(const Config cfg, bool *ok)
#ifndef MINICONF_IMPL
;
#else
{
    const char *val = config_value(cfg);
    if (config_value_type(val) != ConfigType_NUM) {
        *ok = false;
        return 0l;
    }

    int base = 10;
    if (*val == 'x') {
        base = 16;
        val ++;
    } else if (*val == 'b') {
        base = 2;
        val ++;
    }

    char *end;
    const long value = strtol(val, &end, base);
    *ok = (*end == '\0');
    return value;
}
#endif

static long config_get_long_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return 0L;
    const long va = config_get_long(select, ok);
    config_destroy(&select);
    return va;
}

double config_get_double(const Config cfg, bool *ok)
#ifndef MINICONF_IMPL
;
#else
{
    const char *val = config_value(cfg);
    if (config_value_type(val) != ConfigType_NUM) {
        *ok = false;
        return 0.0;
    }

    char *end;
    const double value = strtod(val, &end);
    *ok = (*end == '\0');
    return value;
}
#endif

static double config_get_double_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return 0.0;
    const double va = config_get_double(select, ok);
    config_destroy(&select);
    return va;
}

bool config_get_bool(const Config cfg, bool *ok)
#ifndef MINICONF_IMPL
;
#else
{
    const char *val = config_value(cfg);
    if (config_value_type(val) != ConfigType_BOOL) {
        *ok = false;
        return false;
    }

    if (strcmp(val, "true") == 0) {
        *ok = true;
        return true;
    }

    *ok = (strcmp(val, "false") == 0);
    return false;
}
#endif

static bool config_get_bool_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return false;
    const bool va = config_get_bool(select, ok);
    config_destroy(&select);
    return va;
}

const char *config_get_str(const Config cfg, bool *ok)
#ifndef MINICONF_IMPL
;
#else
{
    const char *val = config_value(cfg);
    if (config_value_type(val) != ConfigType_STRING) {
        *ok = false;
        return NULL;
    }
    *ok = true;
    return val + 1; // leading "
}
#endif

static const char *config_get_str_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return NULL;
    const char *va = config_get_str(select, ok);
    config_destroy(&select);
    return va;
}

#endif //MINICONF_H

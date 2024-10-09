#ifndef MINICONF_H
#define MINICONF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "kallok/kallok.h"

typedef struct {
    Ally ally;
    const char **lines;
    size_t       lines_len;
} Config;

static void config_init(Config *config, Ally ally) {
    config->lines = NULL;
    config->lines_len = 0;
    config->ally = ally;
}

static void config_destroy(Config *config) {
    yfree(config->ally, config->lines, sizeof(const char *) * config->lines_len);
    config->lines = NULL;
    config->lines_len = 0;
}

static void config_add_line(Config *dest, const char *line) {
    dest->lines = (const char **) yrealloc(dest->ally, dest->lines, dest->lines_len * sizeof (char *), (dest->lines_len + 1) * sizeof (char *));
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

void config_child(Config *dest,
                  const Config cfg,
                  const char *child,
                  bool *is_found);

void config_children(Config *dest,
                     const Config cfg,
                     const char *path_in,
                     bool *is_found);

typedef enum {
    ConfigType_NUM,
    ConfigType_BOOL,
    ConfigType_STRING,

    ConfigType_BLOCK,
    ConfigType_INVALID,
} ConfigType;

const char *config_value(const Config cfg);

ConfigType config_value_type(const char *value);

long config_get_long(const Config cfg, bool *ok);

static long config_get_long_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select, cfg.ally);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return 0L;
    const long va = config_get_long(select, ok);
    config_destroy(&select);
    return va;
}

double config_get_double(const Config cfg, bool *ok);

static double config_get_double_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select, cfg.ally);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return 0.0;
    const double va = config_get_double(select, ok);
    config_destroy(&select);
    return va;
}

bool config_get_bool(const Config cfg, bool *ok);

static bool config_get_bool_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select, cfg.ally);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return false;
    const bool va = config_get_bool(select, ok);
    config_destroy(&select);
    return va;
}

const char *config_get_str(const Config cfg, bool *ok);

static const char *config_get_str_at(const Config cfg, const char *path, bool *ok) {
    Config select;
    config_init(&select, cfg.ally);
    config_children(&select, cfg, path, ok);
    if (!*ok)
        return NULL;
    const char *va = config_get_str(select, ok);
    config_destroy(&select);
    return va;
}

#endif //MINICONF_H

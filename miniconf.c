#include "miniconf.h"
#include "minifmt.h"

void config_child(Config *dest,
                  const Config cfg,
                  const char *child,
                  bool *is_found)
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

void config_children(Config *dest,
                     const Config cfg,
                     const char *path_in,
                     bool *is_found)
{
    const size_t len = strlen(path_in);
    char *path = yalloc(cfg.ally, len + 1);
    memcpy(path, path_in, len + 1);

    Config res;
    config_init(&res, cfg.ally);
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
        config_init(&found, cfg.ally);

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

    yfree(cfg.ally, path, len + 1);
}

const char *config_value(const Config cfg)
{
    if (cfg.lines_len == 0)
        return NULL;

    const char *colon = strchr(cfg.lines[0], ':');
    if (colon == NULL)
        return NULL;

    return colon + 1 + strspn(colon + 1, " ");
}

ConfigType config_value_type(const char *value)
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

long config_get_long(const Config cfg, bool *ok)
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

    const long value = ascii_to_ll(val, strlen(val), base);
    *ok = true;
    return value;
}

double config_get_double(const Config cfg, bool *ok)
{
    const char *val = config_value(cfg);
    if (config_value_type(val) != ConfigType_NUM) {
        *ok = false;
        return 0.0;
    }

    const double value = ascii_to_d(val, strlen(val), 10);
    *ok = true; 
    return value;
}

bool config_get_bool(const Config cfg, bool *ok)
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

const char *config_get_str(const Config cfg, bool *ok)
{
    const char *val = config_value(cfg);
    if (config_value_type(val) != ConfigType_STRING) {
        *ok = false;
        return NULL;
    }
    *ok = true;
    return val + 1; // leading "
}

#include <string.h>

#define MINICONF_IMPL
#include "../miniconf.h"

#include "../utils.h"

static char conf[] = 
     "status bar:"
"\n" "    visible: true"
"\n" "    scale: 0.4"
"\n" "    background: xFFFFFFFF"
"\n" "    normal:"
"\n" "        format: \"{lang}  {line}/{col}"
"\n" "    input:"
"\n" "        has hint:"
"\n" "            format: \"{hint}: {message}"
"\n" "        has not hint:"
"\n" "            format: \"{message}"
"\n"
;

int main() {
    Ally ally = getLIBCAlloc();

    Config cfg;
    config_init(&cfg, ally);

    SPLITERATE(conf, "\n", line) {
        config_add_line(&cfg, line);
    }

    bool ok;
    const char *value = config_get_str_at(cfg, "status bar/normal/format", &ok);
    if (!ok)
        return 1;

    if (strcmp(value, "{lang}  {line}/{col}") != 0)
        return 2;

    config_destroy(&cfg);

    return 0;
}


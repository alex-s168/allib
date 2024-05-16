# Kollektions & Kallok
Collection and memory allocator framework written in C.
Tested on Linux and Windows.

## Features (Kollektions)
- Fixed List:
  fixed-length mutable list. Is the base for most other collections.
- Dynamic List:
  dynamically-allocated mutable list.
- Doubly Linked List:
  dynamically-allocated mutable list with O(1) remove, add and insert

## Features (Kallok)
- Common interface for all allocators. `Ally` type.
- Page allocator
- LibC allocator
- Basic fixed & dynamic allocators:
  big allocation split into chunks. one allocation consists of multiple chunks.
- Disk-based allocator:
  virtual maps a file into memory.

## Building
This repository currently contains two libraries: `kallok` and `kollektions`.
To build them, you need to first compile the independent `build.c` file.
(The project uses [Build.C](https://github.com/alex-s168/build.c))
But first you should configure some options that can be found in the top of the `build.c` file.
Then run: `./build.exe kallok.a` and `./build.exe kollektions.a`. The output static-libraries will be located in the `build/` directory.
It is also recommended to run `./build.exe tests` afterwards.
# minilibs
A set of simple, multiplatform, header-only libraries for C.

## miniproc
Simple child process pipe IO library for C.

### Example
```c++
ChildProc proc = cpopen("/bin/echo", (const char *[]) { "Hi", NULL });
if (proc.from == NULL) {
    fprintf(stderr, "error: %i\n", errno);
    return 1;
}

while (cpalive(proc)) {
    copyNFile(stdout, proc.from, cpcountnext(proc));
}
copyNFile(stdout, proc.from, cpcountnext(proc));
cpclose(proc);
```

## minitab
Simple string table printing library for C.

### Example
```c++
int main() {
    Table table;
    table.cols = 3;
    table.rows = 3;
    table_create(&table);

    table_color_col(table, ANSI_COLOR_RED, 0);

    table.colsExtraPad[1] = 2;
    table.colsPadLeft[1] = true;

    table_cell(table, 0, 0).text = "Test";

    table_put_fmt(table, 1, 0, "Test %i", 1);
    table_cell(table, 1, 0).color = ANSI_COLOR_GREEN;

    table_print(table, stdout);
    table_delete(&table);
}
```

## miniconf
Simple configuration file parser for C.

### Example
```c++
int main() {
    Config cfg;
    config_init(&cfg);

    FILE *file = fopen("test.miniconf", "r");
    const AllocGroup file_alloc = config_add_file(&cfg, file);
    fclose(file);

    bool ok;
    const char *value = config_get_str_at(cfg, "status bar/normal/format", &ok);
    printf("%i: %s\n", ok, value);

    allocgroup_free(file_alloc);
    config_destroy(&cfg);

    return 0;
}
```
(If you want to get many variables in for example `status bar/normal`, you should use `config_children` and use the temporary result)

config:
```
status bar:
    visible: true
    scale: 0.4
    background: xFFFFFFFF
    normal:
        format: "{lang}  {line}/{col}
    input:
        has hint:
            format: "{hint}: {message}
        has not hint:
            format: "{message}
```
=======

## minifmt
Simple string templating library for C.

### Dependencies
- stdlib.h: malloc, free, realloc
- math.h: pow

### Example
```c++
static const char *fmt_str = "Test {row} str {col.h}";

int main(void) {
    const size_t len = strlen(fmt_str);
    char *str = malloc(len + 1);
    memcpy(str, fmt_str, len + 1);

    const Fmt fmt = fmt_compile(str);
    char *res = fmt_fmt_fmt(fmt, (PlaceholderList) {
        .elems = (Placeholder[]) {
            PLACEHOLDER_LONG("col", 11),
            PLACEHOLDER_LONG("row", 2)
        },
        .elems_size = 2
    });
    printf("%s\n", res);
    free(res);
    fmt_destroy(fmt);

    free(str);
    return 0;
}
```

### Additional features
- `ll_to_ascii`
- `d_to_ascii`

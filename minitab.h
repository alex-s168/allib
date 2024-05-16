// Define MINITAB_IMPL in a seperate C file and include this header

/*
Copyright 2024 Alexander Nutz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MINITAB_H
#define MINITAB_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#ifdef MINITAB_IMPL
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#endif

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    const char *color;
    const char *text;
    void *toFree;
} TableCell;

typedef struct {
    size_t cols;
    size_t rows;
    TableCell *cells;
    bool      *colsPadLeft;
    size_t    *colsExtraPad;
} Table;

/** size needs to be in table already */
void table_create(Table *table)
#ifndef MINITAB_IMPL
;
#else
{
    table->cells = malloc(sizeof(TableCell) * table->rows * table->cols);
    for (size_t i = 0; i < table->rows * table->cols; i ++) {
        table->cells[i].color = NULL;
        table->cells[i].text = NULL;
        table->cells[i].toFree = NULL;
    }
    
    table->colsExtraPad = malloc(sizeof(size_t) * table->cols);
    for (size_t i = 0; i < table->cols; i ++) {
        table->colsExtraPad[i] = 0;
    }

    table->colsPadLeft = malloc(sizeof(bool) * table->cols);
    for (size_t i = 0; i < table->cols; i ++) {
        table->colsPadLeft[i] = false;
    }
}
#endif

#define table_cell(table, x, y) table.cells[(table).cols * (y) + (x)]
#define table_put_fmt(table, x, y, fmt, ...) { \
    TableCell *cell = &table_cell(table, x, y); \
    char *buf = malloc(256); \
    sprintf(buf, fmt, __VA_ARGS__); \
    cell->text = buf; \
    cell->toFree = buf; \
}

void table_delete(Table *table)
#ifndef MINITAB_IMPL
;
#else
{
    for (size_t i = 0; i < table->rows * table->cols; i ++)
        if (table->cells[i].toFree)
            free(table->cells[i].toFree);
    free(table->cells);

    free(table->colsExtraPad);

    free(table->colsPadLeft);
}
#endif

size_t table_col_width(const Table table, const size_t col)
#ifndef MINITAB_IMPL
;
#else
{
    size_t w = 0;
    for (size_t row = 0; row < table.rows; row ++) {
        const char *str = table_cell(table, col, row).text;
        if (str == NULL)
            continue;
        const size_t len = strlen(str);
        if (len > w)
            w = len;
    }
    return w;
}
#endif

void table_print(const Table table, FILE *file)
#ifndef MINITAB_IMPL
;
#else
{
    size_t *widths = malloc(sizeof(size_t) * table.cols);
    for (size_t i = 0; i < table.cols; i ++)
        widths[i] = table_col_width(table, i) + table.colsExtraPad[i];

    for (size_t row = 0; row < table.rows; row ++) {
        const TableCell *row_data = table.cells + table.cols * row;

        for (size_t col = 0; col < table.cols; col ++) {
            const TableCell cell = row_data[col];

            const size_t len = cell.text ? strlen(cell.text) : 0;
            
            if (table.colsPadLeft[col])
                for (size_t i = len; i < widths[col]; i ++)
                    fputc(' ', file);
            
            if (cell.text) {
                if (cell.color)
                    fputs(cell.color, file);

                fputs(cell.text, file);

                if (cell.color)
                    fputs(ANSI_COLOR_RESET, file);
            }

            if (!table.colsPadLeft[col])
                for (size_t i = len; i < widths[col]; i ++)
                    fputc(' ', file);
        }
        
        fputc('\n', file);
    }

    free(widths);
}
#endif

void table_color_col(const Table table, const char *color, size_t col)
#ifndef MINITAB_IMPL
;
#else
{
    for (size_t row = 0; row < table.rows; row ++) {
        table_cell(table, col, row).color = color;
    }
}
#endif

void table_append_row(Table *table)
#ifndef MINITAB_IMPL
;
#else
{
    table->rows ++;
    table->cells = realloc(table->cells, sizeof(TableCell) * table->rows * table->cols);
    assert(table->cells != NULL);
    for (size_t i = 0; i < table->cols; i ++) {
        const size_t index = (table->rows - 1) * table->cols + i;
        table->cells[index].color = NULL;
        table->cells[index].text = NULL;
        table->cells[index].toFree = NULL;
    }
}
#endif

#endif //MINITAB_H

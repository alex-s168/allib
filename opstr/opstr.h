#ifndef SMALLSTR_H
#define SMALLSTR_H

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    /** not above 64 */
    unsigned char len;
    /** padded with zeros; null terminator required! */
    const char (*str)[65];
} opstr;

int opstr_findzero(opstr str);
int opstr_find(opstr str, char c);
int opstr_countstart(opstr str, opstr other);
bool opstr_startswith(opstr str, opstr other);
int opstr_leading(opstr str, char c);
int opstr_countend(opstr str, opstr other);
bool opstr_endswith(opstr str, opstr other);
int opstr_trailing(opstr str, char c);
int opstr_leadingany(opstr str, const char *some); 
int opstr_trailingany(opstr str, const char *some);

#endif

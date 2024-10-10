#ifndef GERMANSTR_H
#define GERMANSTR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/** 8+sizeof(char*) bytes (=> 16 bytes on 64bit platforms)  */
typedef struct
{
    uint32_t len;
    union
    {
        struct 
        {
            char  prefix[4];
            void* __ignore_me;
        } __attribute__ ((packed));

        struct
        {
            char buf[4 + sizeof(char*)];
        } __attribute__ ((packed)) _short;

        struct
        {
            char prefix[4];
            char* ptr;
        } __attribute__ ((packed)) _long;
    };
} __attribute__ ((packed)) germanstr;

static bool germanstr_is_long(germanstr str)
{
    return str.len > (4 + sizeof(char*));
}

/** you should not call this in a loop; use [germanstr_dataptr] instead */
static inline char germanstr_at(germanstr str, uint32_t idx) 
{
    if (germanstr_is_long(str))
    {
        return (idx < 4) ? str._long.prefix[idx]
                         : str._long.ptr[idx];
    }
    else 
    {
        return str._short.buf[idx];
    }
}

static inline void germanstr_set(germanstr* str, uint32_t idx, char value)
{
#if __has_builtin(__builtin_expect)
    if (__builtin_expect(germanstr_is_long(*str), false))
#else 
    if (germanstr_is_long(*str))
#endif 
    {
        if (idx < 4)
        {
            str->_long.prefix[idx] = value;
        }
        str->_long.ptr[idx] = value;
    }
    else 
    {
        str->_short.buf[idx] = value;
    }
}

static inline germanstr germanstr_from(uint32_t len, char * instr)
{
    germanstr str;
    str.len = len;
    str.prefix[0] = 0;
    str.prefix[1] = 0;
    str.prefix[2] = 0;
    str.prefix[3] = 0;
    if (germanstr_is_long(str))
    {
        str._long.ptr = instr;
        memcpy(str._long.prefix, instr, sizeof(char) * 4); 
    }
    else 
    {
        memcpy(str._short.buf, instr, sizeof(char) * len);
    }
    return str;
}

static inline germanstr germanstr_fromc(char * cstr)
{
    return germanstr_from(strlen(cstr), cstr);
}

static inline germanstr germanstr_cstrdup(const char * ptr)
{
    germanstr res = germanstr_fromc((char*) ptr);
    if (germanstr_is_long(res))
        res._long.ptr = strdup(ptr);
    return res;
}

static inline void germanstr_libcfree(germanstr str)
{
    if (germanstr_is_long(str))
    {
        free(str._long.ptr);
    }
}

static inline germanstr germanstr_libcalloc(uint32_t len)
{
    germanstr str;
    str.len = len; 
    str.prefix[0] = 0;
    str.prefix[1] = 0;
    str.prefix[2] = 0;
    str.prefix[3] = 0;
    if (germanstr_is_long(str))
    {
        str._long.ptr = (char*) malloc(sizeof(char) * len);
    }
    return str;
}

static inline char* germanstr_dataptr(germanstr* str)
{
    if (germanstr_is_long(*str))
    {
        return str->_long.ptr;
    }
    else 
    {
        return str->_short.buf;
    }
}

bool germanstr_eq(germanstr a, germanstr b);
bool germanstr_startswith(germanstr str, germanstr with);

static uint16_t germanstr_prefix_fletcher16(germanstr str)
{
    uint16_t sum1 = str.prefix[0] % 255;
    uint16_t sum2 = sum1;
    sum1 = (sum1 + str.prefix[1]) % 255;
    sum2 = (sum2 + sum1) % 255;
    sum1 = (sum1 + str.prefix[2]) % 255;
    sum2 = (sum2 + sum1) % 255;
    sum1 = (sum1 + str.prefix[3]) % 255;
    sum2 = (sum2 + sum1) % 255;
    return (sum2 << 8) | sum1;
}

static uint8_t germanstr_prefix_xor8(germanstr str)
{
    return str.prefix[0] ^ str.prefix[1] ^ str.prefix[2] ^ str.prefix[3];
}

#endif 

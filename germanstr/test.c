#include "germanstr.h"

extern char get1(germanstr str, int _ignore) {
    return germanstr_at(str, 1);
}

extern char get2(germanstr str, int idx) {
    return germanstr_at(str, idx);
}

extern void set1(germanstr* str, int _ignore) {
    germanstr_set(str, 1, 'a');
}

extern void set2(germanstr* str, int idx) {
    germanstr_set(str, idx, 'a');
}

extern bool eq(germanstr a, germanstr b)
{
    if (a.len != b.len) return false;

    for (size_t i = 0; i < a.len; i ++)
    {
        if (germanstr_at(a, i) != germanstr_at(b, i))
        {
            return false; 
        }
    }

    return true;
}

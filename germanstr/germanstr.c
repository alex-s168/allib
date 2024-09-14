#include "germanstr.h"

bool germanstr_eq(germanstr a, germanstr b)
{
    if (a.len != b.len)
    {
        return false;
    }

    // gets compiled as single integer comparision
    if (memcmp(a.prefix, b.prefix, sizeof(char) * 4) != 0)
    {
        return false;
    }

    return memcmp(germanstr_dataptr(&a) + 4,
                  germanstr_dataptr(&b) + 4,
                  a.len - 4) == 0;
}

bool germanstr_startswith(germanstr str, germanstr with)
{
    if (with.len > str.len)
    {
        return false;
    }

    // allows for better code generation

    bool match = true;
    for (size_t i = 0; i < 4; i ++)
    {
        char c0 = str.prefix[i];
        char c1 = with.prefix[i];

        if (i < with.len)
        {
            if (c0 != c1)
            {
                match = false;
            }
        }
    }
    
    if (!match)
    {
        return false;
    }

    return memcmp(germanstr_dataptr(&str) + 4,
                  germanstr_dataptr(&with) + 4,
                  with.len - 4);
}

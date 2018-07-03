#include "pattern.h"

Pattern pattern_init(Wchar *patt_start, Wchar *patt_end)
{
    Pattern patt;
    patt.head   = *patt_start;
    patt.current = patt_start;
    patt.start = patt_start;
    patt.end    = patt_end;
    return patt;
}

bool pattern_has_length(Pattern patt, Uint len)
{
    return (Uint) (patt.end - patt.current) == len;
}


bool pattern_is_empty(Pattern patt)
{
    return patt.current > patt.end;
}


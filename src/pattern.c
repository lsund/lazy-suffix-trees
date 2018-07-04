#include "pattern.h"


Pattern pattern_init(Wchar *start, Wchar *end)
{
    Pattern patt;
    patt.fst     = *start;
    patt.current = start;
    patt.start   = start;
    patt.end     = end;
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


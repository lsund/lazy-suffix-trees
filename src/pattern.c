#include "pattern.h"

Pattern init_pattern(Wchar *patt_start, Wchar *patt_end)
{
    Pattern patt;
    patt.head   = *patt_start;
    patt.cursor = patt_start;
    patt.start = patt_start;
    patt.end    = patt_end;
    return patt;
}

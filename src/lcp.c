#include "lcp.h"

Text text;

Uint lcp(Wchar *start1, Wchar *end1, Wchar *start2, Wchar *end2) {

    Wchar *probe1 = start1, *probe2 = start2;

    while(probe1 <= end1 && probe2 <= end2 && *probe1 == *probe2)
    {
        probe1++;
        probe2++;
    }

    return (Uint) (probe1 - start1);
}


Uint suffixgroup_lcp(Suffix *left, Suffix *right) {

    Uint j = UINT(1);

    while (true) {

        if(*right + j == text.lst) {
            return j;
        }

        Wchar c = *(*left + j);
        Wchar **current;
        for(current = left + 1; current <= right; current++) {

            if(*(*current + j) != c) {
                return j;
            }
        }
        j++;
    }
}

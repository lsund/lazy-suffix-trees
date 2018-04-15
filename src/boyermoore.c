
#include "boyermoore.h"

bool bmhsearch(Uchar *text, Uint textlen, Uchar *pattern, Uchar *patternright)
{
    Uint m, i, j, rmostocc[UCHAR_MAX+1] = {0};
    Sint k;

    m = (Uint) (patternright - pattern + 1);
    for (i = 0; i < m - 1; i++) {
        printf("%c\n", pattern[i]);
        rmostocc[(Uint) pattern[i]] = i + 1;
    }
    for(j = 0; j <= textlen - m; j += m - rmostocc[(Uint) text[j + m - 1]]) {
        for(k = (Sint) (m - 1); k >= 0 && pattern[k] == text[j + k]; k--)
            if (k < 0) {
                return true;
            }
    }
    return false;
}


bool naive_search(Uchar *text, Uint textlen, Uchar *pattern, Uchar *patternright)
{
    Uint m = (Uint) (patternright - pattern);
    Uint j = 0;
    Uint k;

    for (Uint i = 0; i < textlen; i++) {
        k = i;
        for (j = 0; j < m; j++) {
            if (pattern[j] == text[k]) {
                k++;
            } else {
                break;
            }
        }
        if (j == m) {
            return true;
        }
    }
    return false;
}

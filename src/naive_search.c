
#include "naive_search.h"

Uint textlen;

wchar_t *wtext;

bool naive_search(wchar_t *pattern, wchar_t *patternright)
{
    Uint m = (Uint) (patternright - pattern);
    Uint j = 0;
    Uint k;

    for (Uint i = 0; i < textlen; i++) {
        k = i;
        for (j = 0; j < m; j++) {
            if (pattern[j] == wtext[k]) {
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

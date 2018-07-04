#include "get_alpha.h"

void get_characters(Wchar *alpha, Uint *alphasize)
{
    Uint counts[MAX_CHARS + 1] = {0};
    Wchar *text_probe;

    for (text_probe = text.fst; text_probe < text.fst + text.len; text_probe++) {
        counts[(Uint) *text_probe]++;
    }

    Uint i, j;
    for (j = 0, i = 0; i <= MAX_CHARS; i++) {
        if (counts[i] > 0) {
            alpha[j++] = (Wchar) i;
        }
    }
    *alphasize = j;
}


Uint get_max(Wchar *text, Uint textlen)
{

    Wchar *text_probe;
    Uint max = 0;
    for (text_probe = text; text_probe < text + textlen; text_probe++) {
        if ((Uint) *text_probe > max) {
            max = *text_probe;
        }
    }
    return max;
}



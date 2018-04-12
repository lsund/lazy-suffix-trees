
#include "init.h"

Uchar   *text,
        *sentinel,
        characters[UCHAR_MAX + 1],
        **suffixes;

Uint    textlen,
        alphasize,
        *stree,
        leafcount,
        streesize,
        suffixessize,
        *next_free,
        maxunusedsuffixes,
        sort_bufferwidth,
        max_sortbufferwidth,
        occurrence[UCHAR_MAX + 1],
        rootchildtab[UCHAR_MAX + 1];

Bool    rootevaluated;

void inittree(void)
{
    Uint i;

    sentinel  = text + textlen;
    streesize = BRANCHWIDTH;

    get_characters(text, textlen, characters, &alphasize);

    ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
    next_free     = stree;
    suffixessize      = textlen + 1;
    maxunusedsuffixes = suffixessize >> 1;

    ALLOC(suffixes, NULL, Uchar *, suffixessize);
    sort_bufferwidth = 0;
    max_sortbufferwidth  = textlen >> 8;
    rootevaluated    = False;

    for (i = 0; i <= UCHAR_MAX; i++) {
        occurrence[i] = 0;
    }
}



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
        *next_free,
        sort_bufferwidth,
        max_sortbufferwidth,
        suffixhead_occurrence[UCHAR_MAX + 1];


Bool    root_evaluated;

void inittree(void)
{
    Uint i;

    sentinel  = text + textlen;
    streesize = BRANCHWIDTH;

    get_characters(text, textlen, characters, &alphasize);

    ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
    next_free     = stree;

    ALLOC(suffixes, NULL, Uchar *, textlen + 1);
    sort_bufferwidth = 0;
    max_sortbufferwidth  = textlen >> 8;
    root_evaluated    = False;

    for (i = 0; i <= UCHAR_MAX; i++) {
        suffixhead_occurrence[i] = 0;
    }
}


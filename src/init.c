
#include "init.h"

wchar_t   *wtext,
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
        suffixhead_count[UCHAR_MAX + 1];


bool    root_evaluated;

void inittree(void)
{
    Uint i;

    sentinel  = wtext + textlen;
    streesize = BRANCHWIDTH;

    get_characters(characters, &alphasize);

    ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
    next_free     = stree;

    ALLOC(suffixes, NULL, wchar_t *, textlen + 1);
    sort_bufferwidth = 0;
    max_sortbufferwidth  = textlen >> 8;
    root_evaluated    = false;

    for (i = 0; i <= UCHAR_MAX; i++) {
        suffixhead_count[i] = 0;
    }
}


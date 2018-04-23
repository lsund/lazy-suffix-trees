
#include "init.h"

Wchar     *wtext,
            *sentinel,
            characters[MAX_CHARS + 1],
            **suffixes;

Uint        textlen,
            alphasize,
            *stree,
            streesize,
            *next_element,
            sort_bufferwidth,
            max_sortbufferwidth,
            suffixhead_count[MAX_CHARS + 1];


bool    root_evaluated;


static void init_alphabet()
{
    Uint i;
    get_characters(characters, &alphasize);
    for (i = 0; i <= MAX_CHARS; i++) {
        suffixhead_count[i] = 0;
    }
}


static void init_stree()
{
    root_evaluated = false;
    streesize      = EXTENSION_SIZE;
    ALLOC(stree, stree, Uint, streesize);
    ALLOC(suffixes, NULL, Wchar *, textlen + 1);
    next_element   = stree;
}


static void init_sortbuffer()
{
    sort_bufferwidth    = 0;
    max_sortbufferwidth = textlen >> 8;
}


void init()
{
    init_alphabet();
    init_sortbuffer();
    init_stree();
}


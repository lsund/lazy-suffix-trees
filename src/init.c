
#include "init.h"

Table vertices;


bool    root_evaluated;


void init_root_children()
{
    Uint *cursor;
    for(cursor = root_children; cursor <= root_children + MAX_CHARS; cursor++) {
        *cursor = UNDEFINEDSUCC;
    }
}


static void init_alphabet()
{
    Uint i;
    get_characters(text.characters, &text.alphasize);
    for (i = 0; i <= MAX_CHARS; i++) {
        sortbuffer.suffixhead_count[i] = 0;
    }
}


static void init_stree()
{
    root_evaluated = false;
    vertices.size      = EXTENSION_SIZE;
    ALLOC(vertices.first, vertices.first, Uint, vertices.size);
    ALLOC(text.suffixes, NULL, Wchar *, text.len + 1);
    vertices.next   = vertices.first;
}


static void init_sortbuffer()
{
    sortbuffer.size    = 0;
    sortbuffer.maxsize = text.len >> 8;
}


void init()
{
    init_alphabet();
    init_sortbuffer();
    init_stree();
}


void destroy()
{
    free(text.content);
    free(sortbuffer.content);
    free(vertices.first);
    free(text.suffixes);
    sortbuffer.content = NULL;
    text.content       = NULL;
    vertices.first     = NULL;
    text.suffixes      = NULL;
}

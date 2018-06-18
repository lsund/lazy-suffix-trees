
#include "init.h"

STree st;


void init_root_children()
{
    Uint *v;
    for(v = st.rs; v <= st.rs + MAX_CHARS; v++) {
        *v = UNDEF;
    }
}


static void init_alphabet()
{
    Uint i;
    get_characters(text.cs, &text.asize);
    for (i = 0; i <= MAX_CHARS; i++) {
        sortbuffer.suffixhead_count[i] = 0;
    }
}


static void init_stree()
{
    st.root_eval = false;
    init_root_children();
    st.vs.size      = EXTENSION_SIZE;
    ALLOC(st.vs.fst, Uint, st.vs.size);
    ALLOC(text.ss, Wchar *, text.len + 1);
    st.vs.nxt   = st.vs.fst;
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
    free(text.fst);
    free(sortbuffer.content);
    free(st.vs.fst);
    free(text.ss);
    sortbuffer.content = NULL;
    text.fst       = NULL;
    st.vs.fst     = NULL;
    text.ss      = NULL;
}

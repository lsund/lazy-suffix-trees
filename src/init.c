
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
        sb.groupsize[i] = 0;
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
    sb.size    = 0;
    sb.allocsize = text.len >> 8;
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
    free(sb.fst);
    free(st.vs.fst);
    free(text.ss);
    sb.fst = NULL;
    text.fst       = NULL;
    st.vs.fst     = NULL;
    text.ss      = NULL;
}

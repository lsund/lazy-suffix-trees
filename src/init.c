
#include "init.h"


STree st;


static void init_root()
{
    st.root_eval = false;
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


static void init_sortbuffer()
{
    sb.size      = 0;
    sb.allocsize = text.len >> 8;
}

static void init_stree()
{
    init_root();

    // This is a static allocation, which should be used whenever dynamic
    // allocation is used.
    st.vs.size = VERTEX_ALLOC;

    ALLOC(st.vs.fst, Uint, st.vs.size);
    ALLOC(text.ss, Wchar *, text.len + 1);
    st.vs.nxt = st.vs.fst;
}


void stree_init()
{
    init_alphabet();
    init_sortbuffer();
    init_stree();
}


void stree_destroy()
{
    free(sb.fst);
    free(st.vs.fst);
    sb.fst    = NULL;
    st.vs.fst = NULL;
}

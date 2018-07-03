#include "insert.h"

static void init_next_vertex()
{
    *st.vs.nxt = 0;
}


///////////////////////////////////////////////////////////////////////////////
// Public API


Uint insert_leaf_vertex(Wchar fst, Wchar **left, bool root)
{
    init_next_vertex();
    Uint leafnum = SUFFIX_INDEX(left);
    SET_LEFTBOUND(st.vs.nxt, MAKE_LEAF(leafnum));
    if (root) {
        st.rs[fst] = MAKE_LEAF(leafnum);
    }
    st.vs.nxt += LEAF_VERTEXSIZE;
    return leafnum;
}


void insert_inner_vertex(Wchar fst, Wchar **leftb, Wchar **rightb, bool root)
{
    init_next_vertex();
    SET_LEFTBOUND(st.vs.nxt, leftb - text.ss);
    FIRSTCHILD(st.vs.nxt) = MAKE_UNEVAL_VERTEX(rightb - text.ss);

    if (root) {
        st.rs[fst] = INDEX(st.vs.nxt);
    }
    st.vs.nxt += INNER_VERTEXSIZE;
}


Uint insert_sentinel_vertex(Wchar **right, Uint **previousnode)
{
    Uint leafnum = insert_leaf_vertex('\0', right + 1, false);
    *previousnode = st.vs.nxt;
    return leafnum;
}

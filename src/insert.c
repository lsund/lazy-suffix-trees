#include "insert.h"

Uint insert_leaf_vertex(Wchar fst, Wchar **left, bool root)
{
    Uint leafnum = SUFFIX_INDEX(left);
    *st.vs.nxt = MAKE_LEAF(leafnum);
    if (root) {
        st.rs[fst] = MAKE_LEAF(leafnum);
    }
    st.vs.nxt += LEAF_VERTEXSIZE;
    return leafnum;
}


void insert_inner_vertex(Wchar fst, Wchar **leftb, Wchar **rightb, bool root)
{
    *st.vs.nxt = leftb - text.suffixes;
    CHILD(st.vs.nxt) = MAKE_UNEVAL_VERTEX(rightb - text.suffixes);

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


#include "stree.h"


STree st;


Vertex firstchild(VertexP vertex)
{
    VertexP child = st.vs.fst + FIRSTCHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return MAKE_LEFTBOUND(child);
    } else {
        return LEFTBOUND(child);
    }
}


Uint leftbound(VertexP vertex)
{
    if(IS_UNEVALUATED(vertex)) {
        return MAKE_LEFTBOUND(vertex);
    } else {
        return LEFTBOUND(vertex);
    }
}


bool has_root_edge(Wchar c)
{
    return (st.rs[c]) != UNDEF;
}


Vertex edge_length(VertexP vertex)
{
    return firstchild(vertex) - leftbound(vertex);
}


bool is_sentinel(Suffix **s)
{
    if(**s == text.lst) {
        s--;
        return true;
    }
    return false;
}


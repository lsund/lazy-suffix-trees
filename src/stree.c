#include "stree.h"

STree st;

Vertex leftmost_child_textbound(VertexP vertex)
{
    VertexP child = st.vs.fst + CHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return MAKE_LEFTBOUND(child);
    } else {
        return LEFTBOUND(child);
    }
}


Uint text_leftbound(VertexP vertex)
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
    return leftmost_child_textbound(vertex) - LEFTBOUND(vertex);
}



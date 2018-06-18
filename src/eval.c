
#include "eval.h"

static bool skip_sentinel(Wchar ***rightb)
{
    if(**rightb == text.sentinel) {
        rightb--;
        return true;
    }
    return false;
}


static void get_rightb(Wchar ***ret, Wchar **curr, Wchar **rightb, Wchar fst)
{
    Wchar **probe = curr;
    while (probe < rightb && **(probe + 1) == fst) {
        probe++;
    }
    *ret = probe;
}


static void create_edges(Wchar **leftb, Wchar **rightb, Uint **previous, bool isroot)
{
    Wchar **curr_leftb  = NULL;
    Wchar **curr_rightb = NULL;

    for (curr_leftb = leftb; curr_leftb <= rightb; curr_leftb = curr_rightb + 1) {

        Wchar fst = **curr_leftb;
        get_rightb(&curr_rightb, curr_leftb, rightb, fst);
        *previous = st.vs.nxt;

        if (curr_rightb > curr_leftb) {
            insert_inner_vertex(fst, curr_leftb, curr_rightb, isroot);
        } else {
            insert_leaf_vertex(fst, curr_leftb, isroot);
        }
    }
}
static void eval_edges(Wchar **leftb, Wchar **rightb, bool isroot)
{
    bool sentineledge    = skip_sentinel(&rightb);

    if (!isroot) {
        alloc_extend_stree();
    }

    Uint *previous       = NULL;
    create_edges(leftb, rightb, &previous, isroot);

    if (sentineledge) {
        insert_sentinel_vertex(rightb, &previous);
    }

    if (isroot) {
        *st.vs.nxt = MAKE_LEAF_LASTCHILD(text.len);
        st.vs.nxt += LEAF_VERTEXSIZE;
    } else {
        *previous = WITH_LASTCHILDBIT(*previous);
    }
}


static void eval_vertex(VertexP vertex, Wchar ***leftb, Wchar ***rightb)
{
    *leftb   = SUFFIX_LEFTBOUND(vertex);
    *rightb  = SUFFIX_RIGHTBOUND(vertex);

    SET_TEXT_LEFTBOUND(vertex, SUFFIX_INDEX(*leftb));
    CHILD(vertex) = INDEX(st.vs.nxt);

    counting_sort(*leftb, *rightb);
}


void eval_root()
{
    if (!st.root_eval) {
        create_suffix_groups();
        eval_edges(text.suffixes, text.suffixes + text.len - 1, true);
        st.root_eval = true;
    }
}

void eval_branch(VertexP vertex)
{
    Wchar **leftb;
    Wchar **rightb;
    eval_vertex(vertex, &leftb, &rightb);
    eval_edges(leftb, rightb, false);
}

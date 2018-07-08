#include "eval.h"

static void get_group_rightbound(
        Suffix **group_rightb,
        Suffix *group_leftb,
        Wchar **rightb,
        Wchar fst
    )
{
    Wchar **curr_suffix = group_leftb;
    while (curr_suffix < rightb && **(curr_suffix + 1) == fst) {
        curr_suffix++;
    }
    *group_rightb = curr_suffix;
}


static void insert_edges_aux(
        Wchar **leftb,
        Wchar **rightb,
        Uint **lchild,
        bool isroot
    )
{
    Wchar **curr_suffix;
    Wchar **group_rightb;

    for (curr_suffix = leftb; curr_suffix <= rightb; curr_suffix = group_rightb + 1) {

        Wchar fst = **curr_suffix;
        get_group_rightbound(&group_rightb, curr_suffix, rightb, fst);

        *lchild = st.vs.nxt;
        if (group_rightb > curr_suffix) {
            insert_inner_vertex(fst, curr_suffix, group_rightb, isroot);
        } else {
            insert_leaf_vertex(fst, curr_suffix, isroot);
        }
    }
}


static void insert_edges(Suffix *leftb, Suffix *rightb, bool isroot)
{
    if (!isroot) {
        // Possibly dynamically extend tree here
        /* alloc_extend_stree(); */
    }

    Uint *lchild;
    insert_edges_aux(leftb, rightb, &lchild, isroot);

    if (is_sentinel(&rightb)) {
        insert_sentinel_vertex(rightb, &lchild);
    }

    if (isroot) {
        *st.vs.nxt = MAKE_LASTCHILD(MAKE_LEAF(text.len));
        st.vs.nxt += LEAF_VERTEXSIZE;
    } else {
        *lchild = MAKE_LASTCHILD(*lchild);
    }
}


static void get_unevaluated_suffixes(VertexP v, Suffix **leftb, Suffix **rightb)
{
    // Find out what subsequence of suffixes this vertex corresponds to
    *leftb   = SUFFIX_LEFTBOUND(v);
    *rightb  = SUFFIX_RIGHTBOUND(v);

    SET_LEFTBOUND(v, SUFFIX_INDEX(*leftb));
    FIRSTCHILD(v) = INDEX(st.vs.nxt);

    // Sort the suffixes according to first character
    counting_sort(*leftb, *rightb);
}


static void eval_inner(VertexP v)
{
    Wchar **leftb;
    Wchar **rightb;
    get_unevaluated_suffixes(v, &leftb, &rightb);
    insert_edges(leftb, rightb, false);
}


void eval_root()
{
    if (!st.root_eval) {
        initialize_suffixes();
        insert_edges(text.ss, text.ss + text.len - 1, true);
        st.root_eval = true;
    }
}

void eval_if_uneval(VertexP *v)
{
    if(IS_UNEVALUATED(*v)) {
        Uint index = INDEX(*v);
        *v = st.vs.fst + index;
        eval_inner(*v);
    }
}

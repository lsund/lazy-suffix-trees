
#include "eval.h"

static bool is_last_suffix(Wchar ***rightb)
{
    if(**rightb == text.lst) {
        rightb--;
        return true;
    }
    return false;
}


static void get_groupbound(
        Wchar ***group_rightb,
        Wchar **group_leftb,
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


static void create_edges(Wchar **leftb, Wchar **rightb, Uint **lchild, bool isroot)
{
    Wchar **curr_suffix  = NULL;
    Wchar **group_rightb = NULL;

    for (curr_suffix = leftb; curr_suffix <= rightb; curr_suffix = group_rightb + 1) {

        Wchar fst = **curr_suffix;
        get_groupbound(&group_rightb, curr_suffix, rightb, fst);

        *lchild = st.vs.nxt;
        if (group_rightb > curr_suffix) {
            insert_inner_vertex(fst, curr_suffix, group_rightb, isroot);
        } else {
            insert_leaf_vertex(fst, curr_suffix, isroot);
        }
    }
}


static void eval_edges(Wchar **leftb, Wchar **rightb, bool isroot)
{
    if (!isroot) {
        alloc_extend_stree();
    }

    Uint *lchild;
    create_edges(leftb, rightb, &lchild, isroot);

    if (is_last_suffix(&rightb)) {
        insert_sentinel_vertex(rightb, &lchild);
    }

    if (isroot) {
        *st.vs.nxt = MAKE_LEAF_LASTCHILD(text.len);
        st.vs.nxt += LEAF_VERTEXSIZE;
    } else {
        *lchild = MAKE_LASTCHILD(*lchild);
    }
}


static void eval_vertex(VertexP vertex, Wchar ***leftb, Wchar ***rightb)
{
    // Find out what subsequence of suffixes this vertex corresponds to
    *leftb   = SUFFIX_LEFTBOUND(vertex);
    *rightb  = SUFFIX_RIGHTBOUND(vertex);

    SET_LEFTBOUND(vertex, SUFFIX_INDEX(*leftb));
    CHILD(vertex) = INDEX(st.vs.nxt);

    // Sort the suffixes according to first character
    counting_sort(*leftb, *rightb);
}


void eval_root()
{
    if (!st.root_eval) {
        create_suffix_groups();
        eval_edges(text.ss, text.ss + text.len - 1, true);
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


#include "eval.h"

Wchar   *wtext, *sentinel;

Uint    *next_free, root_children[MAX_CHARS + 1];


static bool skip_sentinel(Wchar ***rightb)
{
    if(**rightb == sentinel) {
        rightb--;
        return true;
    }
    return false;
}


static void get_bound(Wchar ***ret, Wchar **curr, Wchar **rightb, Wchar first)
{
    Wchar **probe = curr;
    while (probe < rightb && **(probe + 1) == first) {
        probe++;
    }
    *ret = probe;
}


static Uint create_edges(Wchar **leftb, Wchar **rightb, Uint **previous, bool isroot)
{
    Uint firstchild     = UNDEFREFERENCE;
    Wchar **rightbound = NULL;
    Wchar **probe      = NULL;

    for (probe = leftb; probe <= rightb; probe = rightbound + 1) {

        Wchar first = **probe;
        get_bound(&rightbound, probe, rightb, **probe);
        *previous = next_free;

        if (rightbound > probe) {
            create_inner_vertex(&firstchild, first, probe, rightbound, isroot);
        } else {
            create_leaf_vertex(first, probe, isroot);
        }
    }

    return firstchild;
}


static Uint eval_edges(Wchar **leftb, Wchar **rightb, bool isroot)
{
    bool sentineledge    = skip_sentinel(&rightb);

    if (isroot) {
        init_root_children();
    } else {
        alloc_extend_stree();
    }

    Uint *previous       = NULL;
    Uint firstchild = create_edges(leftb, rightb, &previous, isroot);

    if (sentineledge) {
        create_sentinel_vertex(rightb, &previous);
    }

    if (isroot) {
        SET_LEAF(next_free, textlen | RIGHTMOSTCHILDBIT);
        next_free++;
    } else {
        *previous |= RIGHTMOSTCHILDBIT;
    }

    return firstchild;
}


void evaluate_root_lazy()
{
    if (!root_evaluated) {
        create_suffix_groups();
        eval_edges(suffixes, suffixes + textlen - 1, true);
        root_evaluated = true;
    }
}


void eval_node(Uint node)
{
    Uint *vertex = stree + node;

    Wchar **leftb   = GET_LEFTBOUNDARY(vertex);
    Wchar **rightb  = GET_RIGHTBOUNDARY(vertex);

    SET_LP(vertex, SUFFIX_STARTINDEX(leftb));
    SET_FIRSTCHILD(vertex, INDEX(next_free));

    counting_sort(leftb, rightb);
    eval_edges(leftb, rightb, false);
}

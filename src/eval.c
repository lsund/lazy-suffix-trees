
#include "eval.h"

wchar_t   *wtext, *sentinel;

Uint    *next_free, root_children[MAX_CHARS + 1];


static bool skip_sentinel(wchar_t ***rightb)
{
    if(**rightb == sentinel) {
        rightb--;
        return true;
    }
    return false;
}


static void get_bound(
                wchar_t ***bound_ptr,
                wchar_t **current,
                wchar_t **rightb,
                wchar_t firstchar
            )
{
    wchar_t **probe;
    for(probe = current; probe < rightb && **(probe + 1) == firstchar; probe++) {
        ;
    }
    *bound_ptr = probe;
}


static Uint create_edges(wchar_t **leftb, wchar_t **rightb, Uint **previous, bool isroot)
{
    Uint firstchild     = UNDEFREFERENCE;
    wchar_t **rightbound = NULL;
    wchar_t **probe      = NULL;

    for (probe = leftb; probe <= rightb; probe = rightbound + 1) {

        wchar_t firstchar = **probe;
        get_bound(&rightbound, probe, rightb, **probe);
        *previous = next_free;

        if (rightbound > probe) {
            create_inner_vertex(&firstchild, firstchar, probe, rightbound, isroot);
        } else {
            create_leaf_vertex(firstchar, probe, isroot);
        }
    }

    return firstchild;
}


static Uint eval_edges(wchar_t **leftb, wchar_t **rightb, bool isroot)
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

    wchar_t **leftb   = GET_LEFTBOUNDARY(vertex);
    wchar_t **rightb  = GET_RIGHTBOUNDARY(vertex);

    SET_LP(vertex, SUFFIX_STARTINDEX(leftb));
    SET_FIRSTCHILD(vertex, INDEX(next_free));

    counting_sort(leftb, rightb);
    eval_edges(leftb, rightb, false);
}

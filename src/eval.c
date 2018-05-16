
#include "eval.h"

Wchar   *wtext, *sentinel;

Uint    *next_element, root_children[MAX_CHARS + 1];


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


static void create_edges(Wchar **leftb, Wchar **rightb, Uint **previous, bool isroot)
{
    Wchar **curr_rightb = NULL;
    Wchar **probe      = NULL;

    for (probe = leftb; probe <= rightb; probe = curr_rightb + 1) {

        Wchar first = **probe;
        get_bound(&curr_rightb, probe, rightb, first);
        *previous = next_element;

        if (curr_rightb > probe) {
            create_inner_vertex(first, probe, curr_rightb, isroot);
        } else {
            create_leaf_vertex(first, probe, isroot);
        }
    }
}


static void eval_edges(Wchar **leftb, Wchar **rightb, bool isroot)
{
    bool sentineledge    = skip_sentinel(&rightb);

    if (isroot) {
        init_root_children();
    } else {
        alloc_extend_stree();
    }

    Uint *previous       = NULL;
    create_edges(leftb, rightb, &previous, isroot);

    if (sentineledge) {
        create_sentinel_vertex(rightb, &previous);
    }

    if (isroot) {
        SET_LEAF(next_element, WITH_LASTCHILDBIT(textlen));
        next_element += LEAF_VERTEXSIZE;
    } else {
        *previous = WITH_LASTCHILDBIT(*previous);
    }
}


void evaluate_root()
{
    if (!root_evaluated) {
        create_suffix_groups();
        eval_edges(suffixes, suffixes + textlen - 1, true);
        root_evaluated = true;
    }
}


void eval_node(Uint node)
{
    Uint *vertex = vertices + node;

    Wchar **leftb   = GET_LEFTB(vertex);
    Wchar **rightb  = GET_RIGHTB(vertex);

    SET_LP(vertex, SUFFIX_INDEX(leftb));
    CHILD(vertex) =  REF_TO_INDEX(next_element);

    counting_sort(leftb, rightb);

    eval_edges(leftb, rightb, false);
}

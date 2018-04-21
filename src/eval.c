
#include "eval.h"

wchar_t   *wtext, *sentinel;

Uint    *next_free, root_children[MAX_CHARS + 1];


static bool skip_sentinel(wchar_t ***right)
{
    if(**right == sentinel) {
        right--;
        return true;
    }
    return false;
}


static void get_bound(
                wchar_t ***bound_ptr,
                wchar_t **current,
                wchar_t **right,
                wchar_t firstchar
            )
{
    wchar_t **probe;
    for(probe = current; probe < right && **(probe + 1) == firstchar; probe++) {
        ;
    }
    *bound_ptr = probe;
}


static Uint evalsuccedges(wchar_t **left, wchar_t **right)
{
    wchar_t **rightbound, **probe;
    Uint firstbranch = UNDEFREFERENCE, *previous = NULL;

    alloc_extend_stree();
    bool sentineledge = skip_sentinel(&right);


    for (probe = left; probe <= right; probe = rightbound + 1) {

        get_bound(&rightbound, probe, right, **probe);
        previous = next_free;

        if(rightbound > probe) {
            create_inner_vertex(&firstbranch, probe, rightbound);
        } else {
            create_leaf_vertex(probe);
        }
    }

    if (sentineledge) {
        create_sentinel_vertex(right, &previous);
    }
    *previous |= RIGHTMOSTCHILDBIT;

    return firstbranch;
}


// Evaluates all outgoing edges from the root. This is a specialization of
// `evaledges`, and in addition it initialized `root_children`
Uint evalrootsuccedges(wchar_t **left, wchar_t **right)
{
    wchar_t firstchar, **r, **l;
    Uint *rptr, firstbranch = UNDEFREFERENCE;

    for(rptr = root_children; rptr <= root_children + MAX_CHARS; rptr++) {
        *rptr = UNDEFINEDSUCC;
    }
    for(l = left; l <= right; l = r + 1) {
        // First Phase
        for(firstchar=**l,r=l; r<right && **(r+1)==firstchar; r++)
        {
            ;
        }
        if(r > l) {
            firstbranch = create_root_child(firstbranch, firstchar, l, r);
        } else {
            create_root_leaf(firstchar, l);
        }
    }
    SET_LEAF(next_free,textlen | RIGHTMOSTCHILDBIT);
    next_free++;
    return firstbranch;
}

void eval_node(Uint node)
{
    Uint *vertex;
    wchar_t **left, **right;

    vertex = stree + node;
    left   = GET_LEFTBOUNDARY(vertex);
    right  = GET_RIGHTBOUNDARY(vertex);
    SET_LP(vertex, SUFFIX_STARTINDEX(left));
    SET_FIRSTCHILD(vertex, INDEX(next_free));

    counting_sort(left, right);

    (void) evalsuccedges(left,right);
}

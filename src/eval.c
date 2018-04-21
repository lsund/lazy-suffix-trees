
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

static void create_inner_vertex(Uint *firstbranch, wchar_t **l, wchar_t **r)
{
    if(*firstbranch == UNDEFREFERENCE) {
        *firstbranch = INDEX(next_free);
    }
    STORE_SUFFIX_BOUNDARIES(next_free, l, r);
    // store l and r. resume later with this unevaluated node
    next_free += BRANCHWIDTH;
}


static Uint create_root_child(
        Uint firstbranch,
        wchar_t firstchar,
        wchar_t **l,
        wchar_t **r
    )
{
    if(firstbranch == UNDEFREFERENCE) {
        firstbranch = INDEX(next_free);
    }

    STORE_SUFFIX_BOUNDARIES(next_free, l, r);
    // store l and r. resume later with this unevaluated branch node
    root_children[firstchar] = INDEX(next_free);
    next_free += BRANCHWIDTH;

    return firstbranch;
}


static void create_root_leaf(wchar_t firstchar, wchar_t **l)
{
    Uint leafnum = SUFFIX_STARTINDEX(l);
    SET_LEAF(next_free, leafnum);
    root_children[firstchar] = leafnum | LEAFBIT;
    next_free++;
}


static Uint create_leaf_vertex(wchar_t **l)
{
    Uint leafnum = SUFFIX_STARTINDEX(l);
    SET_LEAF(next_free, leafnum);
    next_free++;
    return leafnum;
}


static Uint create_sentinel_vertex(wchar_t **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex(right + 1);
    *previousnode = next_free;
    return leafnum;
}


static void get_bound(wchar_t ***bound_ptr, wchar_t **probe, wchar_t **right, wchar_t firstchar)
{
    wchar_t **bound;
    for(bound = probe; bound < right && **(bound + 1) == firstchar; bound++) {
        ;
    }
    *bound_ptr = bound;
}


static Uint evalsuccedges(wchar_t **left, wchar_t **right)
{
    wchar_t firstchar, **rightbound, **probe;
    Uint firstbranch = UNDEFREFERENCE, *previousnode = NULL;

    alloc_extend_stree();
    bool sentineledge = skip_sentinel(&right);


    for (probe = left; probe <= right; probe = rightbound + 1) {

        firstchar = **probe;
        get_bound(&rightbound, probe, right, firstchar);
        previousnode = next_free;

        if(rightbound > probe) {
            create_inner_vertex(&firstbranch, probe, rightbound);
        } else {
            create_leaf_vertex(probe);
        }
    }

    if (sentineledge) {
        create_sentinel_vertex(right, &previousnode);
    }
    *previousnode |= RIGHTMOSTCHILDBIT;

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

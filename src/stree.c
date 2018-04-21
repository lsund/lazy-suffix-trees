
#include "stree.h"


void create_root_leaf(wchar_t firstchar, wchar_t **left)
{
    Uint leafnum = SUFFIX_STARTINDEX(left);
    SET_LEAF(next_free, leafnum);
    root_children[firstchar] = leafnum | LEAFBIT;
    next_free++;
}


Uint create_root_child(
        Uint firstbranch,
        wchar_t firstchar,
        wchar_t **left,
        wchar_t **right
    )
{
    if(firstbranch == UNDEFREFERENCE) {
        firstbranch = INDEX(next_free);
    }

    STORE_SUFFIX_BOUNDARIES(next_free, left, right);
    // store l and r. resume later with this unevaluated branch node
    root_children[firstchar] = INDEX(next_free);
    next_free += BRANCHWIDTH;

    return firstbranch;
}


Uint create_leaf_vertex(wchar_t **left)
{
    Uint leafnum = SUFFIX_STARTINDEX(left);
    SET_LEAF(next_free, leafnum);
    next_free++;
    return leafnum;
}


void create_inner_vertex(Uint *firstbranch, wchar_t **l, wchar_t **r)
{
    if(*firstbranch == UNDEFREFERENCE) {
        *firstbranch = INDEX(next_free);
    }
    // store l and r. resume later with this unevaluated node
    STORE_SUFFIX_BOUNDARIES(next_free, l, r);
    next_free += BRANCHWIDTH;
}


Uint create_sentinel_vertex(wchar_t **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex(right + 1);
    *previousnode = next_free;
    return leafnum;
}



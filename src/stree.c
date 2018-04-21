
#include "stree.h"


void init_root_children()
{
    Uint *probe;
    for(probe = root_children; probe <= root_children + MAX_CHARS; probe++) {
        *probe = UNDEFINEDSUCC;
    }
}


Uint create_leaf_vertex(Wchar first, Wchar **left, bool root)
{
    Uint leafnum = SUFFIX_STARTINDEX(left);
    SET_LEAF(next_free_cell, leafnum);
    if (root) {
        root_children[first] = leafnum | LEAFBIT;
    }
    next_free_cell++;
    return leafnum;
}



void create_inner_vertex(
        Wchar first,
        Wchar **left,
        Wchar **right,
        bool root
    )
{
    STORE_SUFFIX_BOUNDARIES(next_free_cell, left, right);
    // store l and r. resume later with this unevaluated branch node
    if (root) {
        root_children[first] = INDEX(next_free_cell);
    }
    next_free_cell += BRANCHWIDTH;
}


Uint create_sentinel_vertex(Wchar **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex('\0', right + 1, false);
    *previousnode = next_free_cell;
    return leafnum;
}



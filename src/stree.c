
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
    SET_LEAF(next_free, leafnum);
    if (root) {
        root_children[first] = leafnum | LEAFBIT;
    }
    next_free++;
    return leafnum;
}



void create_inner_vertex(
        Uint *firstchild,
        Wchar first,
        Wchar **left,
        Wchar **right,
        bool root
    )
{
    if(*firstchild == UNDEFREFERENCE) {
        *firstchild = INDEX(next_free);
    }

    STORE_SUFFIX_BOUNDARIES(next_free, left, right);
    // store l and r. resume later with this unevaluated branch node
    if (root) {
        root_children[first] = INDEX(next_free);
    }
    next_free += BRANCHWIDTH;
}


Uint create_sentinel_vertex(Wchar **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex('\0', right + 1, false);
    *previousnode = next_free;
    return leafnum;
}




#include "stree.h"


void init_root_children()
{
    Uint *probe;
    for(probe = root_children; probe <= root_children + MAX_CHARS; probe++) {
        *probe = UNDEFINEDSUCC;
    }
}


Uint create_leaf_vertex(wchar_t firstchar, wchar_t **left, bool root)
{
    Uint leafnum = SUFFIX_STARTINDEX(left);
    SET_LEAF(next_free, leafnum);
    if (root) {
        root_children[firstchar] = leafnum | LEAFBIT;
    }
    next_free++;
    return leafnum;
}



void create_inner_vertex(
        Uint *firstbranch,
        wchar_t firstchar,
        wchar_t **left,
        wchar_t **right,
        bool root
    )
{
    if(*firstbranch == UNDEFREFERENCE) {
        *firstbranch = INDEX(next_free);
    }

    STORE_SUFFIX_BOUNDARIES(next_free, left, right);
    // store l and r. resume later with this unevaluated branch node
    if (root) {
        root_children[firstchar] = INDEX(next_free);
    }
    next_free += BRANCHWIDTH;
}


/* void create_inner_vertex(Uint *firstbranch, wchar_t **l, wchar_t **r) */
/* { */
/*     if(*firstbranch == UNDEFREFERENCE) { */
/*         *firstbranch = INDEX(next_free); */
/*     } */
/*     // store l and r. resume later with this unevaluated node */
/*     STORE_SUFFIX_BOUNDARIES(next_free, l, r); */
/*     next_free += BRANCHWIDTH; */
/* } */


Uint create_sentinel_vertex(wchar_t **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex('\0', right + 1, false);
    *previousnode = next_free;
    return leafnum;
}



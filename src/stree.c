
#include "stree.h"

Uint    root_children[MAX_CHARS + 1];

void init_root_children()
{
    Uint *cursor;
    for(cursor = root_children; cursor <= root_children + MAX_CHARS; cursor++) {
        *cursor = UNDEFINEDSUCC;
    }
}


Uint create_leaf_vertex(Wchar first, Wchar **left, bool root)
{
    Uint leafnum = SUFFIX_INDEX(left);
    *vertices.next = WITH_LEAFBIT(leafnum);
    if (root) {
        root_children[first] = leafnum | LEAFBIT;
    }
    vertices.next += LEAF_VERTEXSIZE;
    return leafnum;
}



void create_inner_vertex(Wchar first, Wchar **leftb, Wchar **rightb, bool root)
{
    *vertices.next = leftb - suffixes;
    CHILD(vertices.next) = WITH_UNEVALBIT(rightb - suffixes);

    if (root) {
        root_children[first] = INDEX(vertices.next);
    }
    vertices.next += INNER_VERTEXSIZE;
}


Uint create_sentinel_vertex(Wchar **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex('\0', right + 1, false);
    *previousnode = vertices.next;
    return leafnum;
}


void stree_destroy()
{
    free(wtext);
    free(sortbuffer);
    free(vertices.first);
    free(suffixes);
    free(leaf_nums);
    free(recurse_suffixes);
    sortbuffer       = NULL;
    wtext            = NULL;
    vertices.first    = NULL;
    leaf_nums        = NULL;
    suffixes         = NULL;
    recurse_suffixes = NULL;
}

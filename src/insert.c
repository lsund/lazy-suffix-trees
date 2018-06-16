#include "insert.h"

Uint insert_leaf_vertex(Wchar first, Wchar **left, bool root)
{
    Uint leafnum = SUFFIX_INDEX(left);
    *vertices.next = MAKE_LEAF(leafnum);
    if (root) {
        root_children[first] = MAKE_LEAF(leafnum);
    }
    vertices.next += LEAF_VERTEXSIZE;
    return leafnum;
}


void insert_inner_vertex(Wchar first, Wchar **leftb, Wchar **rightb, bool root)
{
    *vertices.next = leftb - text.suffixes;
    CHILD(vertices.next) = WITH_UNEVALBIT(rightb - text.suffixes);

    if (root) {
        root_children[first] = INDEX(vertices.next);
    }
    vertices.next += INNER_VERTEXSIZE;
}


Uint insert_sentinel_vertex(Wchar **right, Uint **previousnode)
{
    Uint leafnum = insert_leaf_vertex('\0', right + 1, false);
    *previousnode = vertices.next;
    return leafnum;
}


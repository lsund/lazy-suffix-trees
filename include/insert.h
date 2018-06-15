#ifndef INSERT_H
#define INSERT_H

#include "types.h"
#include "stree.h"

void insert_inner_vertex(
        Wchar firstchar,
        Wchar **left,
        Wchar **right,
        bool root
    );

Uint insert_leaf_vertex(Wchar firstchar, Wchar **left, bool root);

Uint insert_sentinel_vertex(Wchar **right, Uint **previousnode);

#endif

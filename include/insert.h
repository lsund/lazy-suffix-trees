#ifndef INSERT_H
#define INSERT_H

#include "types.h"
#include "stree.h"

// Insert a new leaf vertex under the current vertex being evaluated. Returns
// the leaf number of the inserted leaf.
Uint insert_leaf_vertex(Wchar firstchar, Wchar **leftb, bool root);

// Insert a new inner vertex under the current vertex being evaluated.
void insert_inner_vertex(
        Wchar firstchar,
        Wchar **leftb,
        Wchar **rightb,
        bool root
    );

// Inserts the leaf vertex whose edge only is marked with the sentinel
// character.
Uint insert_sentinel_vertex(Wchar **rightb, Uint **prev);

#endif

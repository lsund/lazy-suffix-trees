#ifndef STREEDEF_H
#define STREEDEF_H

#include "types.h"
#include "config.h"

// The root is at the first address in the vertex table
#define ROOT            st.vs.fst

// To retrieve an index in the table, subtract the root
#define INDEX(P)        ((Uint) ((P) - ROOT))


// A Table represents an array for storing vertices. It provides access to the
// first vertex stored, the next unallocated address and the total number of
// vertices stored in the table as its size.
typedef struct table {
    VertexP fst;
    VertexP nxt;
    Uint size;
} Table;


// A STree represents a suffix tree. It provides access to the table of
// vertices, the root children as entry point to the tree and a boolean value
// indicating if the root has been evaluated or not.
typedef struct stree {
    Table vs;
    Uint rs[MAX_CHARS + 1];
    bool root_eval;
} STree;

extern STree st;

#endif

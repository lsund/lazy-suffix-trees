#ifndef STREEDEF_H
#define STREEDEF_H

#include "types.h"
#include "config.h"

typedef struct table {
    Uint *fst;
    Uint *nxt;
    Uint size;
} Table;

// The root is at the first address in the vertex table
#define ROOT            st.vs.fst

// To retrieve an index in the table, subtract the root
#define INDEX(P)        ((Uint) ((P) - ROOT))

typedef struct stree {
    Table vs;
    Uint rs[MAX_CHARS + 1];
    bool root_eval;
} STree;

extern STree st;

#endif

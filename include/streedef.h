#ifndef STREEDEF_H
#define STREEDEF_H

#include "types.h"
#include "basedef.h"

typedef struct table {
    Uint *fst;
    Uint *nxt;
    Uint size;
} Table;

typedef struct stree {
    Table vs;
    Uint rs[MAX_CHARS + 1];
    bool root_eval;
} STree;

extern STree st;

#endif

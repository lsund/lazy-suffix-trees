#ifndef SORTBUFFER_H
#define SORTBUFFER_H

#include "types.h"
#include "config.h"

// The sortbuffer represents the allocated memory for when sorting a set of
// unevaluated suffixes.
typedef struct sortbuffer {
    // Points to the first element.
    Suffix *fst;
    // The suffixes are grouped according to their first character. This array
    // holds, for each character the number of elements in that group.
    Uint groupsize[MAX_CHARS + 1];
    // The total number of suffixes.
    Uint size;
    // The total size of allocated memory.
    Uint allocsize;
} Sortbuffer;

extern Sortbuffer sb;

#endif

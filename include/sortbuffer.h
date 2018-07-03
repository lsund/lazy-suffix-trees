#ifndef SORTBUFFER_H
#define SORTBUFFER_H

#include "types.h"
#include "config.h"

typedef struct sortbuffer {
    Suffix *fst;                    // First element
    Uint groupsize[MAX_CHARS + 1];  // Groupsizes in the current vertex
    Uint size;                      // Number of elements in the buffer
    Uint allocsize;                 // Size allocated
} Sortbuffer;

extern Sortbuffer sb;

#endif

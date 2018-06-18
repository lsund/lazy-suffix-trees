#ifndef SORTBUFFER_H
#define SORTBUFFER_H

#include "types.h"
#include "basedef.h"

typedef struct sortbuffer {
    Wchar **content;
    // number of occurences for the first character of the current suffixes
    Uint suffixhead_count[MAX_CHARS + 1];
    // number of elements
    Uint size;
    Uint maxsize;
} Sortbuffer;

extern Sortbuffer sortbuffer;

#endif

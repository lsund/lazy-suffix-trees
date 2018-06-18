#ifndef SORTBUFFER_H
#define SORTBUFFER_H

#include "types.h"
#include "basedef.h"

typedef struct sortbuffer {
    // space to be used by `sort_buffer`
    Wchar **content;
    Uint suffixhead_count[MAX_CHARS + 1];
    // number of elements
    Uint size;
    Uint maxsize;
} Sortbuffer;

extern Sortbuffer sortbuffer;

// maximal number of elements in `sbufferspace`
extern Uint max_sortbufferwidth;

#endif

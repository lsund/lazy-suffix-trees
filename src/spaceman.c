
#include "spaceman.h"

Wchar **suffixes, **sortbuffer;

Uint    sortbufferwidth,
        max_sortbufferwidth,
        *next_element,
        *vertices,
        streesize,
        textlen;


Wchar **alloc_sortbuffer(Wchar **left, Wchar **right) {

    Uint width = (Uint) (right - left + 1);

    if(sortbufferwidth > max_sortbufferwidth && max_sortbufferwidth > width) {

        sortbufferwidth = max_sortbufferwidth;
        ALLOC(sortbuffer, sortbuffer, Wchar *, sortbufferwidth);

    } else {

        if(width > sortbufferwidth) {
            sortbufferwidth = width;
            ALLOC(sortbuffer, sortbuffer, Wchar *, sortbufferwidth);
        }

    }
    return sortbuffer;
}


void alloc_extend_stree(void)
{
    Uint next_free_index = REF_TO_INDEX(next_element);

    if(next_free_index >= streesize) {

        streesize += (textlen / 10);
        printf("Allocating more...");
        ALLOC(vertices, vertices, Uint, streesize + EXTENSION_SIZE);
        next_element = vertices + next_free_index;

    }
}


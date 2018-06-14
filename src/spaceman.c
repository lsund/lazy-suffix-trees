
#include "spaceman.h"

Wchar **suffixes, **sortbuffer;

Uint    sortbufferwidth,
        max_sortbufferwidth,
        textlen;

Table vertices;


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
    Uint next_free_index = INDEX(vertices.next);

    if(next_free_index >= vertices.size) {

        vertices.size += (textlen / 10);
        ALLOC(vertices.first, vertices.first, Uint, vertices.size + EXTENSION_SIZE);
        vertices.next = vertices.first + next_free_index;

    }
}


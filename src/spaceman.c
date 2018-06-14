
#include "spaceman.h"

Sortbuffer sortbuffer;

Table vertices;

Wchar **alloc_sortbuffer(Wchar **left, Wchar **right) {

    Uint width = (Uint) (right - left + 1);

    if(sortbuffer.size > sortbuffer.maxsize && sortbuffer.maxsize > width) {

        sortbuffer.size = sortbuffer.maxsize;
        ALLOC(sortbuffer.content, sortbuffer.content, Wchar *, sortbuffer.size);

    } else {

        if(width > sortbuffer.size) {
            sortbuffer.size = width;
            ALLOC(sortbuffer.content, sortbuffer.content, Wchar *, sortbuffer.size);
        }

    }
    return sortbuffer.content;
}


void alloc_extend_stree(void)
{
    Uint next_free_index = INDEX(vertices.next);

    if(next_free_index >= vertices.size) {

        vertices.size += (text.len / 10);
        ALLOC(vertices.first, vertices.first, Uint, vertices.size + EXTENSION_SIZE);
        vertices.next = vertices.first + next_free_index;

    }
}



#include "spaceman.h"

Sortbuffer sortbuffer;

STree st;

Wchar **alloc_sortbuffer(Wchar **left, Wchar **right) {

    Uint width = (Uint) (right - left + 1);

    if(sortbuffer.size > sortbuffer.maxsize && sortbuffer.maxsize > width) {

        sortbuffer.size = sortbuffer.maxsize;
        ALLOC(sortbuffer.content, Wchar *, sortbuffer.size);

    } else {

        if(width > sortbuffer.size) {
            sortbuffer.size = width;
            ALLOC(sortbuffer.content, Wchar *, sortbuffer.size);
        }

    }
    return sortbuffer.content;
}


void alloc_extend_stree(void)
{
    Uint next_free_index = INDEX(st.vs.nxt);

    if(next_free_index >= st.vs.size) {

        st.vs.size += (text.len / 10);
        ALLOC(st.vs.fst, Uint, st.vs.size + EXTENSION_SIZE);
        st.vs.nxt = st.vs.fst + next_free_index;

    }
}


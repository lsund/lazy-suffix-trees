
#include "spaceman.h"

Sortbuffer sb;

STree st;

Wchar **alloc_sortbuffer(Wchar **left, Wchar **right) {

    Uint width = (Uint) (right - left + 1);

    if(sb.size > sb.allocsize && sb.allocsize > width) {

        sb.size = sb.allocsize;
        ALLOC(sb.fst, Wchar *, sb.size);

    } else if (width > sb.size) {

        sb.size = width;
        ALLOC(sb.fst, Wchar *, sb.size);

    }
    return sb.fst;
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


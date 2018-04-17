
#include "spaceman.h"

wchar_t **suffixes, **sortbuffer;

Uint    sortbufferwidth,
        max_sortbufferwidth,
        *next_free,
        *stree,
        streesize,
        textlen;


wchar_t **get_sortbuffer(wchar_t **left, wchar_t **right) {

    Uint width = (Uint) (right - left + 1);

    if(sortbufferwidth > max_sortbufferwidth && max_sortbufferwidth > width) {

        sortbufferwidth = max_sortbufferwidth;
        ALLOC(sortbuffer, sortbuffer, wchar_t *, sortbufferwidth);

    } else {

        if(width > sortbufferwidth) {
            sortbufferwidth = width;
            ALLOC(sortbuffer, sortbuffer, wchar_t *, sortbufferwidth);
        }

    }
    return sortbuffer;
}


void allocstree(void)
{
    Uint tmpindex = INDEX(next_free);
    if(tmpindex >= streesize) {
        streesize += (textlen / 10);
        ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
        // update necessary, since stree may have been moved.
        next_free = stree + tmpindex;
    }
}



#include "spaceman.h"

Uchar **suffixes, **sortbuffer;

Uint    sortbufferwidth,
        max_sortbufferwidth,
        *next_free,
        *stree,
        streesize,
        textlen;


Uchar **get_sortbuffer(Uchar **left, Uchar **right) {

    Uint width = (Uint) (right - left + 1);

    if(sortbufferwidth > max_sortbufferwidth && max_sortbufferwidth > width) {

        sortbufferwidth = max_sortbufferwidth;
        ALLOC(sortbuffer, sortbuffer, Uchar *, sortbufferwidth);

    } else {

        if(width > sortbufferwidth) {
            sortbufferwidth = width;
            ALLOC(sortbuffer, sortbuffer, Uchar *, sortbufferwidth);
        }

    }
    return sortbuffer;
}


void allocstree(void)
{
    Uint tmpindex = INDEX(next_free);
    if(tmpindex >= streesize)
    {
        streesize += (textlen / 10);
        ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
        // update necessary, since stree may have been moved.
        next_free = stree + tmpindex;
    }
}


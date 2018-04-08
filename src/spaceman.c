
#include "spaceman.h"

Uchar **suffixes, **sbufferspace;

Uint    sbufferwidth,
        maxsbufferwidth,
        *nextfreeentry,
        *stree,
        streesize,
        textlen;

Uchar **getsbufferspacelazy(Uchar **left, Uchar **right)
{
    Uint width = (Uint) (right - left + 1);

    if(sbufferwidth > maxsbufferwidth && maxsbufferwidth > width)
    {
        sbufferwidth = maxsbufferwidth;
        ALLOC(sbufferspace, sbufferspace, Uchar *, sbufferwidth);
    } else
    {
        if(width > sbufferwidth)
        {
            sbufferwidth = width;
            ALLOC(sbufferspace,sbufferspace,Uchar *,sbufferwidth);
        }
    }
    return sbufferspace;
}


void allocstree(void)
{
    Uint tmpindex = INDEX(nextfreeentry);
    if(tmpindex >= streesize)
    {
        streesize += (textlen / 10);
        ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
        // update necessary, since stree may have been moved.
        nextfreeentry = stree + tmpindex;
    }
}



#include "spaceman.h"

Uchar **getsbufferspaceeager(Uchar **left, Uchar **right)
{
    Uint width = (Uint) (right - left + 1);

    if(width > (Uint) (left-suffixes))
    {
        DEBUG2(3,"#bucket: left=%lu width=%lu\n",(Showuint) (left-suffixes),
                (Showuint) width);
        if(width > sbufferwidth) {
            sbufferwidth = width;
            ALLOC(sbufferspace, sbufferspace, Uchar *, sbufferwidth);
        }
        return sbufferspace;
    }

    return left - width;
}

Uchar **getsbufferspacelazy(Uchar **left, Uchar **right)
{
    Uint width = (Uint) (right - left + 1);

    if(sbufferwidth > maxsbufferwidth && maxsbufferwidth > width)
    {
        sbufferwidth = maxsbufferwidth;
        ALLOC(sbufferspace,sbufferspace,Uchar *,sbufferwidth);
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
    Uint tmpindex = NODEINDEX(nextfreeentry);
    if(tmpindex >= streesize)
    {
        streesize += (textlen / 10);
        ALLOC(stree, stree, Uint, streesize + MAXSUCCSPACE);
        // update necessary, since stree may have been moved.
        nextfreeentry = stree + tmpindex;
    }
}


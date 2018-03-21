
#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include "types.h"

BOOL bmhsearch(
        void *info,
        Uchar *text,
        Uint textlen,
        Uchar *pattern,
        Uchar *patternright
    );

#endif

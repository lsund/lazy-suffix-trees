
#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include "types.h"

bool bmhsearch(Uchar *text, Uint textlen, Uchar *pattern, Uchar *patternright);


bool naive_search(Uchar *text, Uint textlen, Uchar *pattern, Uchar *patternright);

#endif

#ifndef LCP_H
#define LCP_H

#include "types.h"

extern Uchar *sentinel;

Uint lcp(Uchar *start1, Uchar *end1, Uchar *start2, Uchar *end2);

Uint grouplcp(Uchar **left, Uchar **right);

#endif

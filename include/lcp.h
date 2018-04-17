#ifndef LCP_H
#define LCP_H

#include "types.h"

Uint lcp(wchar_t *start1, wchar_t *end1, wchar_t *start2, wchar_t *end2);

Uint grouplcp(wchar_t **left, wchar_t **right);

#endif

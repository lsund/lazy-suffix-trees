#ifndef LCP_H
#define LCP_H

#include "types.h"
#include "text.h"

// Let x be the string that starts at address `start1` and ends at `end1`
// Let y be the string that starts at address `start2` and ends at `end2`
// Then this function returns the longest common prefix of x and y.
Uint lcp(Wchar *start1, Wchar *end1, Wchar *start2, Wchar *end2);

// The longest common prefix for the group of suffixes, laying between adresses
// `left` and `right`.
Uint suffixgroup_lcp(Wchar **left, Wchar **right);

#endif

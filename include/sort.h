
#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H

#include "types.h"
#include "spaceman.h"


wchar_t *wtext, *sentinel, **sbuffer, **suffixes;
Uint textlen, alphasize;

void counting_sort(wchar_t **left, wchar_t **right, Uint prefixlen);

// Determines the groups for all suffixes of the input string
//
// Scan all suffixes, determining the size of each group and then sorts the all
// suffixes into the array suffixes.
void create_suffix_groups(void);

#endif

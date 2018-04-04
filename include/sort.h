
#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H

#include "types.h"
#include "debug.h"

extern Uint occurrence[UCHAR_MAX + 1];
extern Uchar characters[UCHAR_MAX + 1];

extern Uchar *text, *sentinel, **sbuffer, **suffixes;
extern Uint textlen, alphasize;

void counting_sort(Uchar **left,Uchar **right, Uint prefixlen);

// Determines the groups for all suffixes of the input string
//
// Scan all suffixes, determining the size of each group and then sorts the all
// suffixes into the array suffixes.
void create_suffix_groups(void);

#endif

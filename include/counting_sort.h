
#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H

#include "types.h"
#include "debugdef.h"

extern Uint occurrence[UCHAR_MAX + 1];
extern Uchar characters[UCHAR_MAX + 1];

extern Uchar *text, *sentinel, **sbuffer, **suffixes;
extern Uint textlen, alphasize;

void sortByChar(Uchar **left,Uchar **right,Uint prefixlen);

void sortByChar0(void);

#endif

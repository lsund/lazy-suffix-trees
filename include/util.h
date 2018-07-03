#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#include "types.h"
#include "init.h"
#include "eval.h"

// Transform bytes into megabytes
#define MEGABYTES(V)            ((double) (V)/((UINT(1) << 20) - 1))

void showstring(Uchar *left, Uchar *right);

void showpattern(Uchar *w, Uint wlen);

void printtime(FILE *file);

Uint randlen(Uint m_min, Uint m_max);

size_t strlenw(Wchar *s);

bool contains(Uint *arr, Uint len, Uint *vals);

void reverse(Wchar *s,Uint len);

#endif

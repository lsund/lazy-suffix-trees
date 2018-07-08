#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#include "types.h"
#include "init.h"
#include "eval.h"

// Bytes into megabytes.
#define MEGABYTES(B)            ((double) (B)/((UINT(1) << 20) - 1))

// A random number in range [a, b].
Uint rand_range(Uint a, Uint b);

// Does the array `arr` of length `len` contain all the values in the array.
// `vals`?
bool contains(Uint *arr, Uint len, Uint *vals);

// Reverses the string `s` of length `len`.
void reverse(Wchar *s, Uint len);

// Attempt to parse `str` for an unsigned integer.
void parse_Uint(char *str, Uint *d);

// Attempt to parse `str` for a floating point integer.
void parse_float(char *str, float *f);

#endif

#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#include "types.h"
#include "init.h"
#include "eval.h"

extern Uchar *sentinel, *text, characters[UCHAR_MAX + 1];

extern Uint maxstacksize, textlen, alphasize;

void showstring(Uchar *left, Uchar *right);

void showpattern(Uchar *w, Uint wlen);

#endif

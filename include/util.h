/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#include "types.h"
#include "init.h"
#include "eval.h"

void showstring(Uchar *left, Uchar *right);

void showpattern(Uchar *w, Uint wlen);

void printtime();

Uint randlen(Uint m_min, Uint m_max);

size_t strlenw(Wchar *s);

bool contains(Uint *arr, Uint len, Uint val);

#endif

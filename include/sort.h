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

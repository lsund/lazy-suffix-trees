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

#ifndef EVAL_H
#define EVAL_H

#include "externs.h"
#include "types.h"
#include "basedef.h"
#include "spaceman.h"
#include "lcp.h"
#include "sort.h"
#include "stree.h"


Uint evalrootsuccedges(wchar_t **left,wchar_t **right);

void eval_node(Uint node);

void evaluateeager(void);


#endif

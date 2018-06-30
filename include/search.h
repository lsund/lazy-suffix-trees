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

#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>

#include "basedef.h"
#include "types.h"
#include "spaceman.h"
#include "io.h"
#include "util.h"
#include "spaceman.h"
#include "lcp.h"
#include "search.h"
#include "clock.h"
#include "reverse.h"
#include "pattern.h"
#include "match.h"

bool search(Wchar *current_pattern, Uint patternlen);

void search_many(const char *path, int npatterns, Wchar ***o_patterns);


#endif

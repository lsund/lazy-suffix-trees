/*
 * Copyright by Stefan Kurtz (C) 2000-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 */
/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef SEARCHPAT_H
#define SEARCHPAT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "globals.h"
#include "types.h"
#include "debug.h"
#include "char.h"
#include "boyermoore.h"
#include "reverse.h"

#define MAXPATTERNLEN 1024

BOOL search_one_pattern(
        BOOL (*occurs) (Uchar *, Uchar *, Uchar *),
        Uint patternlen,
        char *mypattern
    );



void searchpattern_benchmark(
        BOOL (*occurs) (Uchar *, Uchar *, Uchar *),
        Uint trials,
        Uint minpatternlen,
        Uint maxpatternlen
    );

#endif

#ifndef WOTD_H
#define WOTD_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include "types.h"
#include "spaceman.h"
#include "bitvector.h"
#include "io.h"
#include "basedef.h"
#include "util.h"
#include "spaceman.h"
#include "lcp.h"
#include "occurs.h"
#include "search.h"
#include "clock.h"


///////////////////////////////////////////////////////////////////////////////
// Functions


void inittree(void);

void evaluateeager(void);

void run_patterns(const char *path, Bool evaleager, int npatterns, char ***o_patterns);

void run_benchmark(
        const char *path,
        Bool evaleager,
        Uint trials,
        Uint minpat,
        Uint maxpat
    );

#endif

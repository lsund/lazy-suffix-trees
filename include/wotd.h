#ifndef WOTD_H
#define WOTD_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include "types.h"
#include "spacedef.h"
#include "experim.h"
#include "intbits.h"
#include "filehandle.h"
#include "basedef.h"
#include "util.h"
#include "spaceman.h"
#include "lcp.h"
#include "occurs.h"
#include "search.h"
#include "readfile.h"
#include "clock.h"


///////////////////////////////////////////////////////////////////////////////
// Functions


void inittree(void);

void evaluateeager(void);

void wotd(BOOL evaleager, int npatterns, char ***o_patterns);

void wotd_benchmark(
        BOOL evaleager,
        Uint trials,
        Uint minpat,
        Uint maxpat
    );

#endif

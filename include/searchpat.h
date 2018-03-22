/*
 *  Ludvig Sundström 2018
 */

#ifndef SEARCHPAT_H
#define SEARCHPAT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "types.h"
#include "debugdef.h"
#include "chardef.h"
#include "boyermoore.h"
#include "reverse.h"

#define MAXPATTERNLEN 1024

void search_one_pattern(
        BOOL (*occurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
        void *occursinfo,
        Uchar *text,
        Uint textlen,
        Uint patternlen,
        char *mypattern
    );


void searchpattern_benchmark(
        BOOL (*occurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
        void *occursinfo,
        Uchar *text,Uint textlen,
        float trialpercentage,
        Uint minpatternlen,
        Uint maxpatternlen
    );

#endif

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
 */

/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */


#include "search.h"


static void checkargs(const Uint n, const Uint m)
{
    if(m > (Uint) MAXPATTERNLEN) {
        fprintf(
            stderr,
            "maxpatternlen=%lu > %lu\n",
            (Showuint) m,
            (Showuint) MAXPATTERNLEN
        );
        exit(EXIT_FAILURE);
    }
    if(n <= m) {
        fprintf(
            stderr,
            "textlen=%lu <= maxpatternlen = %lu\n",
            (Showuint) n,
            (Showuint) m
        );
        exit(EXIT_FAILURE);
    }
}


static int checkargs_benchmark(
                const Uint n,
                const Uint m_max,
                const Uint m_min,
                const float trialpercentage
            )
{
    checkargs(n, m_max);

    if(m_max < m_min) {
        fprintf(
            stderr,
            "maxpatternlen=%lu < %lu\n",
            (Showuint) m_max,
            (Showuint) m_min
        );
        exit(EXIT_FAILURE);
    }

    Uint trials;
    if(trialpercentage >= 0.0) {
        trials = (Uint) (trialpercentage * n);
    } else {
        fprintf(stderr,"trialpercentage negative %f", trialpercentage);
        exit(EXIT_FAILURE);
    }

    return trials;
}


static BOOL make_pattern(Uint m, Uchar *pattern, char *mypattern)
{
    for(Uint j = 0; j < m; j++) {

        pattern[j] = mypattern[j];

        if(ISSPECIAL(pattern[j])) {
            return True;
        }
    }
    return False;
}


static BOOL make_text_pattern(Uint n, Uint m, Uchar *text, Uchar *pattern)
{
    Uint start = (Uint) (drand48() * (double) (n-m));

    if(start > n - m) {
        fprintf(stderr,"Not enough characters left\n");
        exit(EXIT_FAILURE);
    }

    for(Uint j = 0; j < m; j++) {

        pattern[j] = text[start + j];

        if(ISSPECIAL(pattern[j])) {
            return True;
        }
    }
    return False;
}


static Uint randlen(Uint m_min, Uint m_max)
{
    return m_min == m_max ? m_min : (m_min + (drand48() * (double) (m_max-m_min+1)));
}


///////////////////////////////////////////////////////////////////////////////
// Public Interface


BOOL search_one_pattern(
        BOOL (*occurs) (Uchar *, Uint, Uchar *, Uchar *),
        Uchar *text,
        Uint n,
        Uint m,
        char *mypattern
    )
{

    checkargs(n, m);

    Uchar pattern[MAXPATTERNLEN + 1];
    BOOL special = make_pattern(m, pattern, mypattern);

    BOOL patternoccurs;
    if(!special) {
        patternoccurs = occurs(text, n, pattern, pattern + m - 1);
    } else {
        exit(EXIT_FAILURE);
    }

    return patternoccurs;
}


void searchpattern_benchmark(
        BOOL (*occurs) (Uchar *,Uint,Uchar *,Uchar *),
        Uchar *text,
        Uint n,
        float trialpercentage,
        Uint m_min,
        Uint m_max
    )
{
    Uint i, m, patternstat[MAXPATTERNLEN+1] = {0};
    BOOL patternoccurs;

    Uint trials = checkargs_benchmark(n, m_max, m_min, trialpercentage);

    printf(
        "# trials %lu minpat %lu maxpat %lu\n",
        (Showuint) trials,
        (Showuint) m_min,
        (Showuint) m_max
    );

    srand48(42349421);

    for(i = 0; i < trials; i++)
    {
        m = randlen(m_min, m_max);
        patternstat[m]++;

        Uchar pattern[MAXPATTERNLEN+1];
        BOOL special = make_text_pattern(n, m, text, pattern);

        if(!special)
        {
            // Every second pattern
            if(i & 1)
            {
                reverse(pattern, m); // Reverse every second string
            }

            patternoccurs =
                occurs(text, n, pattern, pattern+m-1);
        }
    }

    DEBUGCODE(1,showpatternstat(&patternstat[0]));

    DEBUG1(1,"%lu pattern processed as expected\n",(Showuint) trials);
}


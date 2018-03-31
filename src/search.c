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


static void checkargs_benchmark(
                const Uint n,
                const Uint m_max,
                const Uint m_min
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
        BOOL (*occurs) (Uchar *, Uchar *, Uchar *),
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
        patternoccurs = occurs(text, pattern, pattern + m - 1);
    } else {
        exit(EXIT_FAILURE);
    }

    return patternoccurs;
}


void searchpattern_benchmark(
        BOOL (*occurs) (Uchar *, Uchar *, Uchar *),
        Uchar *text,
        Uint n,
        Uint trials,
        Uint m_min,
        Uint m_max
    )
{
    Uint i, m, patternstat[MAXPATTERNLEN+1] = {0};

    checkargs_benchmark(n, m_max, m_min);

    srand48(42349421);

    for(i = 0; i < trials; i++) {
        m = randlen(m_min, m_max);
        patternstat[m]++;

        Uchar pattern[MAXPATTERNLEN+1];
        BOOL special = make_text_pattern(n, m, text, pattern);

        if (!special) {
            // Reverse Every second pattern
            if (i & 1) {
                reverse(pattern, m);
            }

            occurs(text, pattern, pattern+m-1);
        }
    }

    DEBUGCODE(1,showpatternstat(&patternstat[0]));

}


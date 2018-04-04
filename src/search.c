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


Bool (*searchfun) (Uchar *, Uchar *, Uchar *);


static Bool make_pattern(Uint m, Uchar *pattern, char *mypattern)
{
    for(Uint j = 0; j < m; j++) {

        pattern[j] = mypattern[j];

        if(ISSPECIAL(pattern[j])) {
            return True;
        }
    }
    return False;
}


static Bool make_text_pattern(Uint n, Uint m, Uchar *text, Uchar *pattern)
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


///////////////////////////////////////////////////////////////////////////////
// Public Interface


Bool search_one_pattern(
        Uint m,
        char *mypattern
    )
{

    Uchar pattern[MAXPATTERNLEN + 1];
    Bool special = make_pattern(m, pattern, mypattern);

    Bool patternoccurs;
    if(!special) {
        patternoccurs = searchfun(text, pattern, pattern + m - 1);
    } else {
        exit(EXIT_FAILURE);
    }

    return patternoccurs;
}


void searchpattern_benchmark(
        Uint trials,
        Uint m_min,
        Uint m_max
    )
{
    Uint i, m, patternstat[MAXPATTERNLEN+1] = {0};

    // Magic number seed
    srand48(42349421);

    for(i = 0; i < trials; i++) {
        m = randlen(m_min, m_max);
        patternstat[m]++;

        Uchar pattern[MAXPATTERNLEN+1];
        Bool special = make_text_pattern(textlen, m, text, pattern);

        if (!special) {
            // Reverse Every second pattern
            if (i & 1) {
                reverse(pattern, m);
            }

            searchfun(text, pattern, pattern+m-1);
        }
    }

    DEBUGCODE(1,showpatternstat(&patternstat[0]));

}


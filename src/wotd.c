/*
  Copyright by Stefan Kurtz (C) 1999-2003
  =====================================
  You may use, copy and distribute this file freely as long as you
   - do not change the file,
   - leave this copyright notice in the file,
   - do not make any profit with the distribution of this file
   - give credit where credit is due
  You are not allowed to copy or distribute this file otherwise
  The commercial usage and distribution of this file is prohibited
  Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
*/

/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz.
 * For full source control tree, see https://github.com/lsund/wotd
 */

#include "wotd.h"

///////////////////////////////////////////////////////////////////////////////
// Global Strings

// points to input string `t` of length `n`
Uchar *text;

// points to `t[n] = undefined`
Uchar *sentinel;

// characters in `t` in alphabetical order
Uchar characters[UCHAR_MAX + 1];

// array of pointers to suffixes of `t`
Uchar **suffixes;

// pointers into suffixes are with relation to this variable
Uchar **suffixbase;

// buffer for sorting sort suffixes in `src/sort.c`
Uchar **sbuffer;

// space to be used by `sbuffer`
Uchar **sbufferspace = NULL;

///////////////////////////////////////////////////////////////////////////////
// Global Integers

Uint lastrootchild;

Uint maxwidth;

Uint branchcount;

Uint leafcount;

// length of `t`
Uint  textlen;

Uint maxstacksize;

// size of the alphabet `A`
Uint alphasize;

// index of characters in the alphabet `A`
Uint alphaindex[UCHAR_MAX + 1];

// number of occurrences of all characters
Uint occurrence[UCHAR_MAX + 1];

// table to hold suffix tree representation
Uint *stree = NULL;

// number of integers in `stree` allocated
Uint streesize;

// pointer to next unused element in `stree`
Uint *nextfreeentry;

// number of elements in `sbufferspace`
Uint sbufferwidth;

// maximal number of elements in `sbufferspace`
Uint maxsbufferwidth;

// number of unprocessed suffixes (for eager evaluation)
Uint suffixessize;

// when reached, then move and halve space for suffixes
Uint maxunusedsuffixes;

// constant time access to successors of `root`
Uint rootchildtab[UCHAR_MAX + 1];

// flag indicating that the root has been evaluated
BOOL  rootevaluated;


///////////////////////////////////////////////////////////////////////////////
// Functions


void wotd(BOOL evaleager, int npatterns, char ***o_patterns)
{

    char **patterns = *o_patterns;

    if(evaleager) {

        inittree();
        initclock();
        evaluateeager();
        DEBUGCODE(3,showstree());
        DEBUGCODE(3,showtree());
        FREESPACE(suffixes);

    } else {

        initclock();
        inittree();

    }

    int noccurs = 0;
    FILE *fp = createfilehandle("", 0, "data/out.txt", "a");

    for(int j = 0; j < npatterns; j++) {
        BOOL res =
            search_one_pattern(
                evaleager ? occurseager : occurslazy,
                text,
                textlen,
                strlen(patterns[j]),
                patterns[j]
            );
        printf("%s occurs? %d\n", patterns[j], res);
        if (res) {
            fprintf(fp, "%s\n", patterns[j]);
            noccurs++;
        }
    }
    printf("noccurs: %d\n", noccurs);

    DEBUG3(2,"#maxstack=%lu %lu %lu ",
            (Showuint) maxstacksize,
            (Showuint) textlen,
            (Showuint) NODEINDEX(nextfreeentry));
    DEBUG2(2,"%lu %.2f ",(Showuint) maxwidth,(double) maxwidth/textlen);
    DEBUG2(2,"%lu %.2f\n",(Showuint) sbufferwidth,(double) sbufferwidth/textlen);
    DEBUG4(2,"#q=%lu l=%lu %lu %.2f\n",
            (Showuint) branchcount,
            (Showuint) leafcount,
            (Showuint) (branchcount*BRANCHWIDTH+leafcount),
            (double) (4*(branchcount*BRANCHWIDTH+ leafcount))/textlen);
    DELETEFILEHANDLE(fp);
}


void wotd_benchmark(
        BOOL evaleager,
        float rho,
        Uint minpat,
        Uint maxpat
    )
{
    inittree();
    if(evaleager)
    {
        initclock();
        evaluateeager();
        DEBUGCODE(3,showstree());
        DEBUGCODE(3,showtree());
        FREESPACE(suffixes);
    }

    if(maxpat > 0 && maxpat <= textlen && rho != 0.0)
    {
        searchpattern_benchmark(
                evaleager ? occurseager : occurslazy,
                text,
                textlen,
                rho,
                minpat,
                maxpat);
    }

    DEBUG3(2,"#maxstack=%lu %lu %lu ",
            (Showuint) maxstacksize,
            (Showuint) textlen,
            (Showuint) NODEINDEX(nextfreeentry));
    DEBUG2(2,"%lu %.2f ",(Showuint) maxwidth,(double) maxwidth/textlen);
    DEBUG2(2,"%lu %.2f\n",(Showuint) sbufferwidth,(double) sbufferwidth/textlen);
    DEBUG4(2,"#q=%lu l=%lu %lu %.2f\n",
            (Showuint) branchcount,
            (Showuint) leafcount,
            (Showuint) (branchcount*BRANCHWIDTH+leafcount),
            (double) (4*(branchcount*BRANCHWIDTH+ leafcount))/textlen);
}

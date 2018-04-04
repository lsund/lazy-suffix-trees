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
// Functions


static void init_evaluation(BOOL evaleager)
{
    inittree();
    initclock();

    if (evaleager) {
        evaluateeager();
        FREE(suffixes);
    }
}


static void print_statistics(FILE *fp, int trials)
{
    fprintf(fp, "%lu ", (Showuint) trials);
    fprintf(fp, "%.2f\n", getruntime()/(double) ITER);
}


///////////////////////////////////////////////////////////////////////////////
// Public API


void wotd(const char *path, BOOL evaleager, int npatterns, char ***o_patterns)
{

    init_evaluation(evaleager);

    BOOL (*eager_or_lazy) (Uchar *, Uchar *, Uchar *)  =
        evaleager ? occurseager : occurslazy;

    int noccurs     = 0;
    char **patterns = *o_patterns;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        char *pattern = patterns[j];
        Uint patternlen = strlen(pattern);

        BOOL exists = search_one_pattern( eager_or_lazy, patternlen, pattern);

        if (exists) {
            fprintf(fp, "%s\n", patterns[j]);
            noccurs++;
        }
    }

    printf("noccurs: %d\n", noccurs);
    printtime();

    fclose(fp);
}


void wotd_benchmark(
        const char *path,
        BOOL evaleager,
        Uint trials,
        Uint minpat,
        Uint maxpat
    )
{

    init_evaluation(evaleager);

    BOOL (*eager_or_lazy) (Uchar *, Uchar *, Uchar *)  =
        evaleager ? occurseager : occurslazy;

    if (maxpat > 0 && maxpat <= textlen && trials > 0) {
        searchpattern_benchmark(eager_or_lazy, trials, minpat, maxpat);
    }

    FILE *fp = open_append(path);
    print_statistics(fp, trials);
    printtime();

    fclose(fp);
}

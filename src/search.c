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


#include "search.h"


///////////////////////////////////////////////////////////////////////////////
// Functions


static void print_statistics(FILE *fp, int trials)
{
    fprintf(fp, "%lu ", (Showuint) trials);
    fprintf(fp, "%.2f\n", getruntime()/(double) ITER);
}


static Bool copy_pattern(Uchar *pattern, char *current_pattern, Uint len)
{
    for(Uint i = 0; i < len; i++) {

        pattern[i] = current_pattern[i];

        if (ISSPECIAL(pattern[i])) {
            return True;
        }
    }

    return False;
}


static Bool sample_random_pattern(Uchar *pattern, Uint patternlen)
{
    Uint start = (Uint) (drand48() * (double) (textlen - patternlen));

    if(start > textlen - patternlen) {
        fprintf(stderr,"Not enough characters left\n");
        exit(EXIT_FAILURE);
    }

    for(Uint j = 0; j < patternlen; j++) {

        pattern[j] = text[start + j];

        if(ISSPECIAL(pattern[j])) {
            return True;
        }
    }
    pattern[patternlen] = '\0';
    return False;
}


static Bool try_search_pattern(char *current_pattern, Uint patternlen)
{

    Uchar pattern[MAXPATTERNLEN + 1];
    Bool special = copy_pattern(pattern, current_pattern, patternlen);

    if (special) {
        ERROR("Found an unparsable pattern");
    }

    return search(pattern, pattern + patternlen - 1);

}


// Randomly sampling patterns from the text, reversing every second to simulate
// the case where a pattern does not exist.
static void iterate_search_patterns(Uint trials, Uint minlen, Uint maxlen)
{
    Uint patternlen;

    // Magic number seed
    srand48(42349421);

    for(Uint i = 0; i < trials; i++) {

        Uchar pattern[MAXPATTERNLEN + 1];
        patternlen = randlen(minlen, maxlen);

        Bool special = sample_random_pattern(pattern, patternlen);

        if (!special) {
            if (i & 1) {
                reverse(pattern, patternlen);
            }
            search(pattern, pattern + patternlen - 1);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Public API


void search_patterns(const char *path, int npatterns, char ***patterns_ptr)
{

    inittree();
    initclock();

    int noccurs     = 0;
    char **patterns = *patterns_ptr;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        char *current_pattern = patterns[j];
        Uint patternlen = strlen(current_pattern);

        Bool exists = try_search_pattern(current_pattern, patternlen);

        if (exists) {
            fprintf(fp, "%s\n", patterns[j]);
            noccurs++;
        }
    }

    printf("noccurs: %d\n", noccurs);
    printtime();

    fclose(fp);
}


void search_benchmark(const char *path, Uint trials, Uint minpat, Uint maxpat)
{

    inittree();
    initclock();

    if (maxpat > textlen) {
        ERROR("Max pattern length must be smaller than the text length");
    }

    iterate_search_patterns(trials, minpat, maxpat);

    FILE *fp = open_append(path);
    print_statistics(fp, trials);
    printtime();

    fclose(fp);
}

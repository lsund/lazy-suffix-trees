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
    fprintf(fp, "%lu ", (Ulong) trials);
    fprintf(fp, "%.2f\n", getruntime()/(double) ITER);
}


static bool copy_pattern(wchar_t *pattern, wchar_t *current_pattern, Uint len)
{
    *(pattern + len) = '\0';
    for(Uint i = 0; i < len; i++) {

        pattern[i] = current_pattern[i];

        if (ISSPECIAL(pattern[i])) {
            return true;
        }
    }

    return false;
}


static bool sample_random_pattern(wchar_t *pattern, Uint patternlen)
{
    Uint start = (Uint) (drand48() * (double) (textlen - patternlen));

    if(start > textlen - patternlen) {
        fprintf(stderr,"Not enough characters left\n");
        exit(EXIT_FAILURE);
    }

    for(Uint j = 0; j < patternlen; j++) {

        /* pattern[j] = text[start + j]; */
        pattern[j] = wtext[start + j];

        if(ISSPECIAL(pattern[j])) {
            return true;
        }
    }
    pattern[patternlen] = '\0';
    return false;
}


// Randomly sampling patterns from the text, reversing every second to simulate
// the case where a pattern does not exist.
static void search_random_patterns(Uint trials, Uint minlen, Uint maxlen)
{
    Uint patternlen;

    // Magic number seed
    srand48(42349421);

    for(Uint i = 0; i < trials; i++) {

        wchar_t pattern[MAXPATTERNLEN + 1];
        patternlen = randlen(minlen, maxlen);

        bool special = sample_random_pattern(pattern, patternlen);

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


// Search for one pattern in the tree
bool try_search(wchar_t *current_pattern, Uint patternlen)
{

    wchar_t pattern[MAXPATTERNLEN + 1];
    bool special = copy_pattern(pattern, current_pattern, patternlen);

    if (special) {
        ERROR("Found an unparsable pattern");
    }

    return search(pattern, pattern + patternlen - 1);

}


// Search for many patterns in the tree
void try_search_patterns(const char *path, int npatterns, wchar_t ***patterns_ptr)
{
    int noccurs     = 0;
    wchar_t **patterns = *patterns_ptr;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        wchar_t *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        bool exists = try_search(current_pattern, patternlen);

        if (exists) {
            fprintf(fp, "%ls\n", patterns[j]);
            noccurs++;
        }
    }

    printf("noccurs: %d\n", noccurs);
    printtime();

    fclose(fp);
}


// Sample random patterns and serch for them
void try_search_random_patterns(const char *path, Uint trials, Uint minpat, Uint maxpat)
{
    if (maxpat > textlen) {
        ERROR("Max pattern length must be smaller than the text length");
    }

    search_random_patterns(trials, minpat, maxpat);

    FILE *fp = open_append(path);
    print_statistics(fp, trials);
    printtime();

    fclose(fp);
}

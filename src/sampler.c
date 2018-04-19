#include "sampler.h"

static bool sample(wchar_t *pattern, Uint patternlen)
{
    Uint start = (Uint) (drand48() * (double) (textlen - patternlen));

    if(start > textlen - patternlen) {
        fprintf(stderr,"Not enough characters left\n");
        exit(EXIT_FAILURE);
    }

    for(Uint j = 0; j < patternlen; j++) {
        pattern[j] = wtext[start + j];
    }

    pattern[patternlen] = '\0';
    return false;
}


// Randomly sampling patterns from the text, reversing every second to simulate
// the case where a pattern does not exist.
static void sample_search(Uint trials, Uint minlen, Uint maxlen)
{
    Uint patternlen;

    // Magic number seed
    srand48(42349421);

    for(Uint i = 0; i < trials; i++) {

        wchar_t pattern[MAXPATTERNLEN + 1];
        patternlen = randlen(minlen, maxlen);

        sample(pattern, patternlen);

        if (i & 1) {
            reverse(pattern, patternlen);
        }
        search(pattern, pattern + patternlen - 1);
    }
}


// Sample random patterns and serch for them
void search_samples(const char *path, Uint trials, Uint minpat, Uint maxpat)
{
    if (maxpat > textlen) {
        fprintf(stderr, "Max pattern length must be smaller than the text length");
    }

    sample_search(trials, minpat, maxpat);

    FILE *fp = open_append(path);
    fprintf(fp, "%lu ", (Ulong) trials);
    fprintf(fp, "%.2f\n", getruntime()/(double) ITER);
    printtime();

    fclose(fp);
}

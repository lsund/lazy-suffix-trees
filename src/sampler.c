#include "sampler.h"

static bool sample(Wchar *pattern, Uint patternlen)
{
    Uint start = (Uint) (drand48() * (double) (text.len - patternlen));

    if(start > text.len - patternlen) {
        fprintf(stderr,"Not enough characters left\n");
        exit(EXIT_FAILURE);
    }

    for(Uint j = 0; j < patternlen; j++) {
        pattern[j] = text.content[start + j];
    }

    pattern[patternlen] = '\0';
    return false;
}


// Randomly sampling patterns from the text, reversing every second to simulate
// the case where a pattern does not exist.
static void sample_search(Uint trials, Uint minlen, Uint maxlen, FILE *fp)
{
    Uint patternlen;

    // Magic number seed
    srand48(42349421);

    for(Uint i = 0; i < trials; i++) {

        Wchar pattern[MAXPATTERNLEN + 1];
        patternlen = randlen(minlen, maxlen);

        sample(pattern, patternlen);
        fprintf(fp, "%ls\n", pattern);

        if (i & 1) {
            reverse(pattern, patternlen);
        }

        Pattern patt = init_pattern(pattern, pattern + patternlen - 1);
        search(patt);
    }
}


// Sample random patterns and serch for them
void search_samples(const char *path, Uint trials, Uint minpat, Uint maxpat)
{
    if (maxpat > text.len) {
        fprintf(stderr, "Max pattern length must be smaller than the text length");
    }

    FILE *fp = open_append(path);

    sample_search(trials, minpat, maxpat, fp);

    printtime();

    fclose(fp);
}

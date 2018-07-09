#include "search.h"
#include "io.h"

static void usage()
{
    fprintf(stdout, "\nusage: gk TEXTFILE PATTERNFILE MODE\n\n");
    fprintf(stdout, "TEXTFILE: the path to a file containing the text\n");
    fprintf(stdout, "PATTERNFILE: path to a file containing set of patterns\n\n");
    fprintf(stdout, "MODE: either 'run' or 'bench'\n\n");
}


///////////////////////////////////////////////////////////////////////////////
// Public API


int main(int argc, char *argv[])
{
    char *textfile, *patternfile;
    bool sample_patterns = false;

    setlocale(LC_ALL, LOCALE);
    if (argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        textfile    = argv[1];
        sample_patterns = true;
    } else if (argc == 3) {
        textfile    = argv[1];
        patternfile = argv[2];
    } else {
        usage();
        exit(EXIT_FAILURE);
    }
    printf("Loading a text file based on the locale: %s\n", LOCALE);
    text_initialize(textfile);

    stree_init();
    clock_init();

    // If only one input file is given, the program samples this file for
    // random patterns which is then searched for.
    if (sample_patterns) {

        printf("Searching for random patterns in %s\n", textfile);
        printf("Printing the found patterns to %s\n", PATTERN_OUT);

        search_random(PATTERN_OUT, N_RAND_PATTERNS, RAND_MINLEN, RAND_MAXLEN);
        text_destroy();

    // If two input files are given, then the first one is assumed to contain a
    // text for which a suffix tree is to be built and the second one is
    // assumed to specify the patterns, separated by newlines.
    } else {

        printf("Querying %s for the patterns in %s\n", textfile, patternfile);
        Wchar **patterns = (Wchar **) malloc(sizeof(char *) * MAX_PATTERNS);
        Uint npatterns  = patterns_initialize(patternfile, MAX_PATTERNS, &patterns);
        search_many(npatterns, &patterns);
        text_destroy();
        for (int i = npatterns; i >= 0; i--) {
            free(patterns[i]);
        }
        free(patterns);

    }

    stree_destroy(&st);

    return EXIT_SUCCESS;
}

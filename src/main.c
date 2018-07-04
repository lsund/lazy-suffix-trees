#include "search.h"
#include "io.h"

// The matching patterns will be logged to this file
const char *pattern_out = "patterns-found.txt";


static void usage()
{
    fprintf(stdout, "\nusage: gk TEXTFILE PATTERNFILE MODE\n\n");
    fprintf(stdout, "TEXTFILE: the path to a file containing the text\n");
    fprintf(stdout, "PATTERNFILE: path to a file containing set of patterns\n\n");
    fprintf(stdout, "MODE: either 'run' or 'bench'\n\n");
}


static void free_text_and_patterns(Wchar **patterns, int npatterns)
{
    text_destroy();
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);
}


///////////////////////////////////////////////////////////////////////////////
// Public API


int main(int argc, char *argv[])
{
    char *textfile, *patternfile;
    bool sample_patterns = false;

    const char *locale = "en_US.utf8";

    setlocale(LC_ALL, locale);
    if (argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        textfile        = argv[1];
        sample_patterns = true;
    } else if (argc == 3) {
        textfile    = argv[1];
        patternfile = argv[2];
    } else {
        usage();
        exit(EXIT_FAILURE);
    }
    printf("Loading a text file based on the locale: %s\n", locale);
    text_initialize(textfile);

    stree_init();
    clock_init();

    if (sample_patterns) {

        int minpat = 10;
        int maxpat = 20;
        search_random(pattern_out, 10000, minpat, maxpat);

    } else {

        Wchar **patterns = (Wchar **) malloc(sizeof(char *) * MAX_PATTERNS);
        Uint npatterns  = patterns_initialize(patternfile, MAX_PATTERNS, &patterns);
        printf("npatterns: %lu\n", npatterns);
        search_many(pattern_out, npatterns, &patterns);
        free_text_and_patterns(patterns, npatterns);

    }

    return EXIT_SUCCESS;
}

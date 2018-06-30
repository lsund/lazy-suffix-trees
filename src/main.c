#include "search.h"
#include "sampler.h"
#include "io.h"

static void usage()
{
    fprintf(stdout, "\nusage: gk TEXTFILE PATTERNFILE MODE\n\n");
    fprintf(stdout, "TEXTFILE: the path to a file containing the text\n");
    fprintf(stdout, "PATTERNFILE: path to a file containing set of patterns\n\n");
    fprintf(stdout, "MODE: either 'run' or 'bench'\n\n");
}


static void free_text_and_patterns(Wchar **patterns, int npatterns)
{
    free_text();
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
    file_to_string(textfile);

    if(text.len > MAXTEXTLEN) {
        fprintf(stderr, "Text is too large\n");
    }
    init();
    initclock();

    const char *outpath = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/136" ;

    if (sample_patterns) {

        int minpat = 10;
        int maxpat = 20;
        search_samples(outpath, 10000, minpat, maxpat);

        printf("%lu\n", text.asize);

    } else {
        Wchar **patterns = (Wchar **) malloc(sizeof(char *) * MAX_PATTERNS);
        Uint npatterns  = file_to_strings(patternfile, MAX_PATTERNS, &patterns);
        printf("npatterns: %lu\n", npatterns);
        search_many(outpath, npatterns, &patterns);
        free_text_and_patterns(patterns, npatterns);
    }

    return EXIT_SUCCESS;
}

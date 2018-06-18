#include "pattern_searcher.h"
#include "sampler.h"
#include "io.h"

const char *outpath = "data/out.txt";
const char *locale = "en_US.utf8";

static void usage()
{
    fprintf(stdout, "\nusage: gk TEXTFILE PATTERNFILE MODE\n\n");
    fprintf(stdout, "TEXTFILE: the path to a file containing the text\n");
    fprintf(stdout, "PATTERNFILE: path to a file containing set of patterns\n\n");
    fprintf(stdout, "MODE: either 'run' or 'bench'\n\n");
}


static void freespace(Wchar **patterns, int npatterns)
{
    freetextspace();
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        usage();
        exit(EXIT_FAILURE);
    }

    char *textfile, *patternfile, *mode;

    textfile    = argv[1];
    patternfile = argv[2];
    mode        = argv[3];

    setlocale(LC_ALL, locale);
    printf("Loading a text file based on the locale: %s\n", locale);
    file_to_string(textfile);

    Wchar **patterns = (Wchar **) malloc(sizeof(char *) * MAX_PATTERNS);
    Uint npatterns  = file_to_strings(patternfile, MAX_PATTERNS, &patterns);

    if(text.len > MAXTEXTLEN) {
        fprintf(stderr, "Text is too large\n");
    }

    init();
    initclock();

    if (strcmp(mode, "sample") == 0) {

        int minpat = 10;
        int maxpat = 20;
        search_samples(outpath, 100, minpat, maxpat);

    } else {
        printf("npatterns: %lu\n", npatterns);
        search_patterns(outpath, npatterns, &patterns);
    }

    freespace(patterns, npatterns);

    return EXIT_SUCCESS;
}

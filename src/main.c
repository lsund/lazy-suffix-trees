#include "pattern_searcher.h"
#include "sampler.h"

Uint textlen, max_codepoint;

int main(int argc,char *argv[])
{
    char *filename, *patternfile, *mode;
    Uint patternslen;

    if (argc == 1) {

        filename = "data/small.txt";
        patternfile = "data/small-patt.txt";
        mode = "run";

    } else if (argc == 4) {

        filename = argv[1];
        patternfile     = argv[2];
        mode = argv[3];
    } else {

        fprintf(stderr, "Wrong amount of arguments supplied");
        exit(EXIT_FAILURE);

    }

    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(filename, "r");
    wtext = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    sentinel  = wtext + textlen;

    max_codepoint = get_max(wtext, textlen);
    printf("max character value: %lu\n", max_codepoint);
    fclose(in);

    if(wtext == NULL) {
        fprintf(stderr, "Cannot open file");
    }

    Wchar **patterns = (Wchar **) malloc(sizeof(char *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);

    if(textlen > MAXTEXTLEN) {
        fprintf(stderr, "Text too large, see MAXTEXTLEN");
    }

    const char *path = "data/out.txt";

    init();
    initclock();

    if (strcmp(mode, "bench") == 0) {

        int minpat = 10;
        int maxpat = 20;

        search_samples(path, 100, minpat, maxpat);

    } else {
        printf("npatterns: %d\n", npatterns);
        search_patterns(path, npatterns, &patterns);

    }

    freetextspace();

    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}

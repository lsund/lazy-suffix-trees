#include "search.h"

Uint textlen, max_codepoint;

int main(int argc,char *argv[])
{
    char *filename, *patternfile, *mode;
    Uint patternslen;

    if (argc == 1) {

        filename = "data/small.txt";
        patternfile = "data/small-patt.txt";
        mode = "run";

    } else {

        CHECKARGNUM(4, "filename patternfile (bench|run)");

        filename = argv[1];
        patternfile     = argv[2];
        mode = argv[3];
    }


    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(filename, "r");
    wtext = malloc(sizeof(wchar_t) * MAXTEXTLEN);
    wint_t c;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    printf("max character value: %lu\n", max_codepoint);
    fclose(in);

    if(wtext == NULL) {
        fprintf(stderr, "Cannot open file");
    }

    wchar_t **patterns = (wchar_t **) malloc(sizeof(char *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);

    if(textlen > MAXTEXTLEN) {
        fprintf(stderr, "Text too large, see MAXTEXTLEN");
    }

    const char *path = "data/out.txt";

    inittree();
    initclock();

    if (strcmp(mode, "bench") == 0) {

        int minpat = 500;
        int maxpat = 1000;

        try_search_random_patterns(path, 100000, minpat, maxpat);

    } else {
        printf("npatterns: %d\n", npatterns);
        try_search_patterns(path, npatterns, &patterns);

    }

    freetextspace();

    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}

#include "search.h"

/* Uchar *text; */
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
    fclose(in);

    if(wtext == NULL) {
        ERROR("Cannot open file");
    }

    int size        = 128;
    wchar_t **patterns = (wchar_t **) malloc(sizeof(char *) * size);
    int npatterns  = file_to_strings(patternfile, &patternslen, size, &patterns);

    if(textlen > MAXTEXTLEN) {
        ERROR("Text too large, see MAXTEXTLEN");
    }

    const char *path = "data/out.txt";

    inittree();
    initclock();

    if (strcmp(mode, "bench") == 0) {

        int minpat = 500;
        int maxpat = 1000;

        try_search_random_patterns(path, 100000, minpat, maxpat);

    } else {

        try_search_patterns(path, npatterns, &patterns);

    }

    freetextspace();

    for (; npatterns >= 0; npatterns--) {
        free(patterns[npatterns]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}

#include "search.h"

Uchar *text;
Uint textlen, max_codepoint;

int main(int argc,char *argv[])
{
    char *filename, *patternfile;
    Uint patternslen;

    CHECKARGNUM(4, "filename patternfile (bench|run)");
    DEBUGLEVELSET;

    filename = argv[1];

    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(filename, "r");
    wchar_t *wtext = malloc(sizeof(wchar_t) * MAXTEXTLEN);
    wint_t c;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    printf("%lu\n", textlen);
    max_codepoint = get_max(wtext, textlen);
    fclose(in);

    textlen = 0;
    text = (Uchar *) file2String(filename, &textlen);

    if(text == NULL) {
        ERROR("Cannot open file");
    }

    patternfile     = argv[2];
    int size        = 128;
    char **patterns = (char **) malloc(sizeof(char *) * size);
    int npatterns  = file2Array(patternfile, &patternslen, size, &patterns);

    if(textlen > MAXTEXTLEN) {
        ERROR("Text too large, see MAXTEXTLEN");
    }

    const char *path = "data/out.txt";

    inittree();
    initclock();

    if (strcmp(argv[3], "bench") == 0) {

        int minpat = 500;
        int maxpat = 1000;

        try_search_random_patterns(path, 100000, minpat, maxpat);

    } else {

        try_search_patterns(path, npatterns, &patterns);

    }

    freetextspace(text, textlen);

    for (; npatterns >= 0; npatterns--) {
        free(patterns[npatterns]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}

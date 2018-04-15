#include "search.h"

extern Uchar *text;
extern Uint textlen;

int main(int argc,char *argv[])
{
    char *filename, *patternfile;
    Uint patternslen;

    CHECKARGNUM(4, "filename patternfile (bench|run)");
    DEBUGLEVELSET;

    filename = argv[1];
    text = (Uchar *) file2String(filename, &textlen);

    /* Uint len = 4; */
    /* Uint *test = (Uint *) malloc(sizeof(Uint)); */
    /* printf("%s\n", text); */
    /* while (true) { */
    /*     from_utf8(&text, text + len, test); */
    /*     if (*test == 0) { */
    /*         break; */
    /*     } */
    /*     printf("%lu\n", *test); */
    /* } */
    /* exit(EXIT_SUCCESS); */

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

    /* setlocale(LC_ALL, "en_US.utf8"); */
    /* FILE *in = fopen("data/utftest.txt", "r"); */
    /* wchar_t *text = malloc(sizeof(wchar_t) * MAXTEXTLEN); */
    /* wint_t c; */
    /* Uint i = 0; */
    /* while ((c = fgetwc(in)) != WEOF) { */
    /*     text[i] = c; */
    /*     i++; */
    /* } */
    /* text[i + 1] = '\0'; */
    /* printf("%lu\n", i); */
    /* get_wcharacters(in, text, 0, NULL, 0); */
    /* fclose(in); */

    if (strcmp(argv[3], "bench") == 0) {

        int minpat = 500;
        int maxpat = 1000;

        search_benchmark(path, 100000, minpat, maxpat);

    } else {

        search_patterns(path, npatterns, &patterns);

    }

    freetextspace(text, textlen);

    for (; npatterns >= 0; npatterns--) {
        free(patterns[npatterns]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}

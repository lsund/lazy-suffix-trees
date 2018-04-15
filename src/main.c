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

    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen("data/utftest.txt", "r");
    wchar_t *wtext = malloc(sizeof(wchar_t) * MAXTEXTLEN);
    wint_t c;
    Uint i = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[i] = c;
        i++;
    }
    wtext[i + 1] = '\0';
    printf("%lu\n", i);
    get_wcharacters(in, wtext, 0, NULL, 0);
    fclose(in);

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

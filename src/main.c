#include "pattern_searcher.h"
#include "sampler.h"

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
    text.fst = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    while ((c = fgetwc(in)) != WEOF) {
        text.fst[text.len] = c;
        text.len++;
    }
    text.fst[text.len + 1] = '\0';
    text.lst  = text.fst + text.len;

    text.maxc = get_max(text.fst, text.len);
    printf("max character value: %lu\n", text.maxc);
    fclose(in);

    if(text.fst == NULL) {
        fprintf(stderr, "Cannot open file");
    }

    Wchar **patterns = (Wchar **) malloc(sizeof(char *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);

    if(text.len > MAXTEXTLEN) {
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

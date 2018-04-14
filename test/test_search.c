#include "test.h"

extern Uchar *text;
extern Uint textlen;

char *utest_search()
{
    Uint patternslen;
    /* char *patternfile = "data/test-patterns.txt"; */
    /* char *textfile = "data/dataset/005.txt"; */
    char *patternfile = "data/existing.txt";
    char *textfile = "data/data.xml";

    text = (Uchar *) file2String(textfile, &textlen);
    Uint size = 1001;
    char **patterns = (char **) malloc(sizeof(Uchar *) * size);
    Uint npatterns   = file2Array(patternfile, &patternslen, size, &patterns);
    search_patterns("data/out.txt", npatterns, &patterns);
    inittree();
    for (Uint j = 0; j < npatterns; j++) {

        char *current_pattern = patterns[j];
        Uint patternlen = strlen(current_pattern);
        Bool really_exists = naive_search(
                                text,
                                textlen,
                                (Uchar *) current_pattern,
                                ((Uchar *) current_pattern) + patternlen
                             );

        /* printf("%s\n", current_pattern); */
        Bool exists = search_pattern(current_pattern, patternlen);
        printf("%d %d\n", really_exists, exists);
    }

    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}


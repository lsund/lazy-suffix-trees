#include "test.h"

char *utest_search()
{
    Uint textlen;
    Uchar *text = (Uchar *) file2String("data/dataset/005.txt", &textlen);

    Uint patternslen;
    Uint size = 1001;
    char *patternfile = "data/random-patterns.txt";
    char **patterns = (char **) malloc(sizeof(Uchar *) * size);
    Uint npatterns   = file2Array(patternfile, &patternslen, size, &patterns);
    for (Uint j = 0; j < 100; j++) {

        char *current_pattern = patterns[j];
        Uint patternlen = strlen(current_pattern);
        Bool exists = naive_search(text, textlen, (Uchar *) current_pattern,
                                    ((Uchar *) current_pattern) + patternlen);

        printf("%d\n", exists);
    }

    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}


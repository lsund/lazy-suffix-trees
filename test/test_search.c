#include "test.h"

extern Uchar *text;
extern Uint textlen;

static Uint min(const Uint a, const Uint b)
{
    return a < b ? a : b;
}


char *utest_search_for(char *patternfile, char *textfile)
{
    Uint patternslen;
    text = (Uchar *) file2String(textfile, &textlen);
    Uint size = 1001;
    char **patterns = (char **) malloc(sizeof(Uchar *) * size);
    Uint npatterns   = file2Array(patternfile, &patternslen, size, &patterns);
    inittree();
    for (Uint j = 0; j < min(npatterns, 100); j++) {

        char *current_pattern = patterns[j];
        Uint patternlen = strlen(current_pattern);
        Bool really_exists = naive_search(
                                text,
                                textlen,
                                (Uchar *) current_pattern,
                                ((Uchar *) current_pattern) + patternlen
                             );

        Bool exists = search_pattern(current_pattern, patternlen);
        if (really_exists != exists) {
            printf("%d %d\n", really_exists, exists);
            printf("%s\n", patterns[j]);
        }
        mu_assert(
            "Naive and suffix tree sourch should be the same.",
            really_exists == exists
        );
        /* printf("%lu\n", j); */
    }
    return NULL;
}

char *utest_search()
{

    char *error;

    mu_message(DATA, "Akz patterns\n");
    error = utest_search_for("data/10000.txt", "data/data.xml");
    if (error) return error;

    mu_message(DATA, "Easy test patterns\n");
    error = utest_search_for("data/test-patterns.txt", "data/dataset/005.txt");
    if (error) return error;

    mu_message(DATA, "Random exiting patterns\n");
    error = utest_search_for("data/random-patterns.txt", "data/dataset/005.txt");
    if (error) return error;

    mu_message(DATA, "Random non-existing patterns\n");
    error = utest_search_for(
                "data/random-patterns-non-existing.txt",
                "data/dataset/005.txt"
            );
    if (error) return error;

    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}


#include "test.h"

wchar_t *wtext;
Uint textlen, max_codepoint;

static Uint min(const Uint a, const Uint b)
{
    return a < b ? a : b;
}


char *utest_search_for(char *patternfile, char *textfile)
{
    Uint patternslen;
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
    wtext = malloc(sizeof(wchar_t) * MAXTEXTLEN);
    wint_t c;
    textlen = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    Uint nlines = 1000;
    wchar_t **patterns = (wchar_t **) malloc(sizeof(wchar_t *) * nlines);
    int npatterns  = file_to_strings(patternfile, &patternslen, nlines, &patterns);
    inittree();
    for (Uint j = 0; j < min(npatterns, 100); j++) {

        wchar_t *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);
        wchar_t *end = current_pattern + patternlen;

        bool exists = try_search(current_pattern, patternlen);
        bool really_exists = naive_search(current_pattern, end);
        if (really_exists != exists) {
            printf("%d %d\n", really_exists, exists);
            printf("Fail on: %ls\n", patterns[j]);
        }
        mu_assert(
            "Naive and suffix tree sourch should be the same.",
            really_exists == exists
        );
    }
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);
    return NULL;
}

char *utest_search()
{

    char *error;

    mu_message(DATA, "Trivial\n");
    error = utest_search_for("data/small-patt.txt", "data/small.txt");
    if (error) return error;

    mu_message(DATA, "Easy test patterns\n");
    error = utest_search_for("data/test-patterns.txt", "data/dataset/005.txt");
    if (error) return error;

    mu_message(DATA, "Random existing patterns\n");
    error = utest_search_for("data/random-patterns.txt", "data/dataset/005.txt");
    if (error) return error;

    mu_message(DATA, "Random non-existing patterns\n");
    error = utest_search_for(
                "data/random-patterns-non-existing.txt",
                "data/dataset/005.txt"
            );
    if (error) return error;

    /* mu_message(DATA, "Akz patterns\n"); */
    /* error = utest_search_for("data/10000.txt", "data/data.xml"); */
    /* if (error) return error; */


    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}


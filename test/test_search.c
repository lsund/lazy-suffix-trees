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
    wchar_t **patterns = (wchar_t **) malloc(sizeof(wchar_t *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    inittree();

    int exists_n = 0, rexists_n = 0;
    for (Uint j = 0; j < min(npatterns, 20); j++) {

        wchar_t *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        wchar_t *end = current_pattern + patternlen;

        bool exists = try_search(current_pattern, patternlen);
        bool really_exists = naive_search(current_pattern, end);

        printf("%d %d\n", really_exists, exists);
        if (exists) {
            exists_n++;
        }
        if (really_exists) {
            rexists_n++;
        }
        if (really_exists) {
        }
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
    printf("count: %d %d\n", exists_n, rexists_n);
    free(patterns);
    return NULL;
}

char *utest_search()
{

    char *error;

    /* mu_message(DATA, "Trivial\n"); */
    /* error = utest_search_for("data/small-patt.txt", "data/small.txt"); */
    /* if (error) return error; */

    /* mu_message(DATA, "Easy test patterns\n"); */
    /* error = utest_search_for("data/test-patterns.txt", "data/dataset/005.txt"); */
    /* if (error) return error; */

    /* mu_message(DATA, "Random existing patterns\n"); */
    /* error = utest_search_for("data/random-patterns.txt", "data/dataset/005.txt"); */
    /* if (error) return error; */

    /* mu_message(DATA, "Random non-existing patterns\n"); */
    /* error = utest_search_for( */
    /*             "data/random-patterns-non-existing.txt", */
    /*             "data/dataset/005.txt" */
    /*         ); */
    /* if (error) return error; */

    /* mu_message(DATA, "Trivial AKZ patterns\n"); */
    /* error = utest_search_for("data/akz/trivial.txt", "data/data.xml"); */
    /* if (error) return error; */

    mu_message(DATA, "Akz patterns\n");
    error = utest_search_for("data/10000.txt", "data/data.xml");
    if (error) return error;

    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}

